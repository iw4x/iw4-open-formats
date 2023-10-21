#include <std_include.hpp>

#include "iweapon.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>
#include <utils/json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#define IW4X_WEAP_VERSION 1

namespace iw4of::interfaces
{
    bool iweapon::write_internal(const native::XAssetHeader& header) const
    {
        auto asset = header.weapon;

        rapidjson::Document output(rapidjson::kObjectType);
        auto& allocator = output.GetAllocator();

        output.AddMember("version", IW4X_WEAP_VERSION, allocator);

        output.AddMember("name", RAPIDJSON_STR(asset->szInternalName), allocator);

        rapidjson::Value container(rapidjson::kObjectType);

        write_variant(container, allocator, {nullptr}, header);

        output.AddMember("weapon", container, allocator);

        rapidjson::StringBuffer buff;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
        output.Accept(writer);

        utils::io::write_file(get_work_path(header).string(), buff.GetString());
        return true;
    }

    void* interfaces::iweapon::read_internal(const std::string& name) const
    {
        auto path = get_work_path(name).string();

        if (!utils::io::file_exists(path)) return nullptr;

        native::WeaponCompleteDef* asset = local_allocator.allocate<native::WeaponCompleteDef>();

        rapidjson::Document weaponJson;
        try
        {
            auto contents = utils::io::read_file(path);
            weaponJson.Parse(contents.data());
        }
        catch (const std::exception& e)
        {
            print_error("Invalid weapon json for {} (broken json {})\n", name, e.what());
        }

        if (!weaponJson.IsObject())
        {
            print_error("Invalid weapon json for {} (Is it zonebuilder format?)\n", name);
            return nullptr;
        }

        if (weaponJson["version"].Get<int32_t>() != IW4X_WEAP_VERSION)
        {
            print_error(
                "Invalid weapon json version for {}, expected {} and got {}\n", name, IW4X_WEAP_VERSION, weaponJson["version"].Get<std::string>());
            return nullptr;
        }

        return asset;
    }

    std::filesystem::path interfaces::iweapon::get_file_name(const std::string& basename) const
    {
        return std::format("{}.iw4x.json", basename);
    }

    bool iweapon::write_variant(rapidjson::Value& container, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator,
                                const native::XAssetHeader& weapon_original, const native::XAssetHeader& weapon_variant) const
    {
        const auto* original_weapon = weapon_original.weapon;
        const auto* weapon = weapon_variant.weapon;
        const std::string full_name = weapon->szInternalName;
        std::string suffix{};
        std::string base_name = full_name;
        if (full_name.ends_with("_mp"))
        {
            suffix = "_mp";
            base_name = base_name.substr(0, base_name.length() - suffix.length());
        }

#define WRITE_MEMBER(obj, member)                                            \
    if (original_weapon == nullptr || obj->member != original_##obj->member) \
    {                                                                        \
        container.AddMember(#member, obj->member, allocator);                \
    }

#define WRITE_STR_MEMBER_IF_NOT_NULL(obj, member)                                                        \
    if (obj->member)                                                                                     \
    {                                                                                                    \
        if (original_weapon == nullptr || original_##obj->member == nullptr ||                           \
            (obj->member != original_##obj->member && strcmp(obj->member, original_##obj->member) != 0)) \
        {                                                                                                \
            container.AddMember(#member, RAPIDJSON_STR(obj->member), allocator);                         \
        }                                                                                                \
    }

#define WRITE_MATERIAL_NAME(obj, member)                                                                              \
    if (obj->member)                                                                                                  \
    {                                                                                                                 \
        if (original_weapon == nullptr || original_##obj->member == nullptr || obj->member != original_##obj->member) \
        {                                                                                                             \
            container.AddMember(#member, RAPIDJSON_STR(obj->member->info.name), allocator);                           \
        }                                                                                                             \
    }

#define WRITE_ASSET_NAME(obj, member)                                                                                  \
    if (obj->member)                                                                                                   \
    {                                                                                                                  \
        if (original_weapon == nullptr || original_##obj->member == nullptr || (obj->member != original_##obj->member) \
        {                                                                                                \
            container.AddMember(#member, RAPIDJSON_STR(obj->member->name), allocator);                         \
        }                                                                                                              \
    }

#define WRITE_MEMBER_IF_NOT_NULL(obj, member) \
    if (obj->member)                          \
    {                                         \
        WRITE_MEMBER(obj, member);            \
    }

#define WRITE_MEMBER_MATERIAL(obj, member)                                   \
    if (obj->member)                                                         \
    {                                                                        \
        assets->write(native::XAssetType::ASSET_TYPE_MATERIAL, obj->member); \
        WRITE_MATERIAL_NAME(obj, member);                                    \
    }

#define WRITE_MEMBER_ASSET(obj, member, xassettype) \
    if (obj->member)                                \
    {                                               \
        assets->write(xassettype, obj->member);     \
        WRITE_ASSET_NAME(obj, member);              \
    }

#define WRITE_SCRIPTSTRING_MEMBER_ARRAY(obj, member, count)                                     \
    if (obj->member && (original_weapon == nullptr || (obj->member != original_##obj->member))) \
    {                                                                                           \
        rapidjson::Value arr(rapidjson::kArrayType);                                            \
        for (size_t _ = 0; _ < count; _++)                                                      \
        {                                                                                       \
            if (obj->member[_])                                                                 \
            {                                                                                   \
                std::string str = assets->read_from_stringtable(obj->member[_]);                \
                arr.PushBack(RAPIDJSON_STR(local_allocator.duplicate_string(str)), allocator);  \
            }                                                                                   \
        }                                                                                       \
                                                                                                \
        container.AddMember(#member, arr, allocator);                                           \
    }

#define WRITE_STRING_MEMBER_ARRAY(obj, member, count)                                           \
    if (obj->member && (original_weapon == nullptr || (obj->member != original_##obj->member))) \
    {                                                                                           \
        rapidjson::Value arr(rapidjson::kArrayType);                                            \
        for (size_t _ = 0; _ < count; _++)                                                      \
        {                                                                                       \
            if (obj->member[_])                                                                 \
            {                                                                                   \
                arr.PushBack(RAPIDJSON_STR(obj->member[_]), allocator);                         \
            }                                                                                   \
        }                                                                                       \
                                                                                                \
        container.AddMember(#member, arr, allocator);                                           \
    }
        std::string name = weapon->szInternalName;

        if (original_weapon)
        {
            name = name.substr(strnlen(original_weapon->szInternalName, name.size() - 1) - suffix.length());
            name = name.substr(1, name.length() - suffix.length()-1);
        }

        container.AddMember("szInternalName", RAPIDJSON_STR(local_allocator.duplicate_string(name)), allocator);

        WRITE_STR_MEMBER_IF_NOT_NULL(weapon, szDisplayName);

        WRITE_SCRIPTSTRING_MEMBER_ARRAY(weapon, hideTags, 32);

        if (weapon->szXAnims)
        {
            rapidjson::Value szAnims(rapidjson::kObjectType);
            for (size_t i = 0; i < native::weapAnimFiles_t::NUM_WEAP_ANIMS; i++)
            {
                if (weapon->szXAnims[i] && strnlen(weapon->szXAnims[i], 1) > 0 &&
                    (!original_weapon || !original_weapon->szXAnims || weapon->szXAnims[i] != original_weapon->szXAnims[i]))
                {
                    szAnims.AddMember(RAPIDJSON_STR(native::weapAnimFiles_Names[i]), RAPIDJSON_STR(weapon->szXAnims[i]), allocator);
                }
            }

            if (szAnims.MemberCount() > 0)
            {
                container.AddMember("szXAnims", szAnims, allocator);
            }
        }

        WRITE_MEMBER_IF_NOT_NULL(weapon, fAdsZoomFov);
        WRITE_MEMBER_IF_NOT_NULL(weapon, iAdsTransInTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon, iAdsTransOutTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon, iClipSize);
        WRITE_MEMBER_IF_NOT_NULL(weapon, impactType);
        WRITE_MEMBER_IF_NOT_NULL(weapon, iFireTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon, dpadIconRatio);
        WRITE_MEMBER_IF_NOT_NULL(weapon, penetrateMultiplier);
        WRITE_MEMBER_IF_NOT_NULL(weapon, fAdsViewKickCenterSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon, fHipViewKickCenterSpeed);
        WRITE_STR_MEMBER_IF_NOT_NULL(weapon, szAltWeaponName);
        WRITE_MEMBER_IF_NOT_NULL(weapon, altWeaponIndex);
        WRITE_MEMBER_IF_NOT_NULL(weapon, iAltRaiseTime);
        WRITE_MEMBER_MATERIAL(weapon, killIcon);
        WRITE_MEMBER_MATERIAL(weapon, dpadIcon);
        WRITE_MEMBER_IF_NOT_NULL(weapon, fireAnimLength);
        WRITE_MEMBER_IF_NOT_NULL(weapon, iFirstRaiseTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon, ammoDropStockMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon, adsDofStart);
        WRITE_MEMBER_IF_NOT_NULL(weapon, adsDofEnd);
        WRITE_MEMBER_IF_NOT_NULL(weapon, motionTracker);
        WRITE_MEMBER_IF_NOT_NULL(weapon, enhanced);
        WRITE_MEMBER_IF_NOT_NULL(weapon, dpadIconShowsAmmo);

        // for (size_t i = 0; i < 2; i++)
        //{
        //
        //      unsigned __int16 accuracyGraphKnotCount[2];
        //      float(*accuracyGraphKnots[2])[2];
        //}

        // dump variants
        rapidjson::Value variants(rapidjson::kArrayType);

        for (const auto& attachment_name : weapon_attachments)
        {
            std::string variant_name = base_name + "_" + attachment_name + suffix;
            if (const auto* variant_weapon = find<native::WeaponCompleteDef>(native::ASSET_TYPE_WEAPON, variant_name))
            {
                rapidjson::Value variant_container(rapidjson::kObjectType);
                write_variant(variant_container,
                              allocator,
                              {const_cast<iw4of::native::WeaponCompleteDef*>(weapon)},
                              {const_cast<iw4of::native::WeaponCompleteDef*>(variant_weapon)});

                variants.PushBack(variant_container, allocator);
            }
        }

        if (variants.Size() > 0)
        {
            container.AddMember("variants", variants, allocator);
        }

        return false;
    } // namespace iw4of::interfaces

} // namespace iw4of::interfaces
