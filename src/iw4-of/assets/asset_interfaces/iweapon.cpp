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

        if (!utils::io::file_exists(path))
        {
            return nullptr;
        }

        native::XAssetHeader asset{};

        rapidjson::Document weapon_json;
        try
        {
            auto contents = utils::io::read_file(path);
            weapon_json.Parse(contents.data());
        }
        catch (const std::exception& e)
        {
            print_error("Invalid weapon json for {} (broken json {})\n", name, e.what());
        }

        if (!weapon_json.IsObject())
        {
            print_error("Invalid weapon json for {} (Is it zonebuilder format?)\n", name);
            return nullptr;
        }

        if (weapon_json["version"].Get<int32_t>() != IW4X_WEAP_VERSION)
        {
            print_error(
                "Invalid weapon json version for {}, expected {} and got {}\n", name, IW4X_WEAP_VERSION, weapon_json["version"].Get<std::string>());
            return nullptr;
        }

        read_variant(asset, weapon_json, {nullptr});

        return asset.data;
    }

    std::filesystem::path interfaces::iweapon::get_file_name(const std::string& basename) const
    {
        return std::format("{}.iw4x.json", basename);
    }

    void iweapon::write_xmodels_array(const std::string& name, native::XModel** arr, size_t count, native::XModel** original_arr,
                                      rapidjson::Value& container, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const

    {
        bool write_variant = true;
        if (original_arr)
        {
            write_variant = false;
            for (size_t i = 0; i < count; i++)
            {
                if (original_arr[i] && arr[i] != original_arr[i])
                {
                    write_variant = true;
                    break;
                }
            }
        }

        if (write_variant)
        {
            rapidjson::Value xmodels_json(rapidjson::kArrayType);
            for (size_t i = 0; i < count; i++)
            {
                if (arr[i])
                {
                    xmodels_json.PushBack(RAPIDJSON_STR(arr[i]->name), allocator);
                    assets->write(iw4of::native::ASSET_TYPE_XMODEL, arr[i]);
                }
                else
                {
                    xmodels_json.PushBack(rapidjson::Value(rapidjson::kNullType), allocator);
                }
            }

            const auto n = local_allocator.duplicate_string(name);
            container.AddMember(RAPIDJSON_STR(n), xmodels_json, allocator);
        }
    }

    void iweapon::write_sounds_array(const std::string& name, native::snd_alias_list_t** arr, size_t count, native::snd_alias_list_t** original_arr,
                                     rapidjson::Value& container, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const
    {
        if (arr == nullptr)
        {
            return;
        }

        bool should_write = original_arr == nullptr;
        rapidjson::Value sounds_array(rapidjson::kArrayType);
        for (size_t i = 0; i < count; i++)
        {
            if (arr[i])
            {
                if (original_arr && original_arr[i] != arr[i])
                {
                    should_write = true;
                }

                assets->write(iw4of::native::XAssetType::ASSET_TYPE_SOUND, arr[i]);
                sounds_array.PushBack(RAPIDJSON_STR(arr[i]->aliasName), allocator);
            }
            else
            {
                sounds_array.PushBack(rapidjson::Value(rapidjson::kNullType), allocator);
            }
        }

        if (should_write)
        {
            const auto n = local_allocator.duplicate_string(name);
            container.AddMember(RAPIDJSON_STR(n), sounds_array, allocator);
        }
    }

    void iweapon::read_weapon_anims(const std::string& name, const char*** destination, const rapidjson::Value& container) const
    {
        const auto rapidjson_key = RAPIDJSON_STR(name.data());
        if (container.HasMember(rapidjson_key) && container[rapidjson_key].IsObject())
        {
            const auto obj = container[rapidjson_key].GetObject();
            if (destination)
            {
                *destination = local_allocator.allocate_array<const char*>(native::weapAnimFiles_t::NUM_WEAP_ANIMS);

                for (const auto& elem : obj)
                {
                    if (elem.value.IsString())
                    {
                        const auto val = elem.value.GetString();
                        for (size_t i = 0; i < native::weapAnimFiles_t::NUM_WEAP_ANIMS; i++)
                        {
                            if (std::string(native::weapAnimFiles_Names[i]) == elem.name.GetString())
                            {
                                (*destination)[i] = local_allocator.duplicate_string(val);

                                if (val && strnlen(val, 1) > 0)
                                {
                                    const auto anim =
                                        reinterpret_cast<native::XAnimParts*>(assets->find_other_asset(native::ASSET_TYPE_XANIMPARTS, val));

                                    if (anim)
                                    {
                                        // good
                                    }
                                    else
                                    {
                                        print_error("Could not find anim {}!", val);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    void iweapon::read_sounds_array(const std::string& name, native::snd_alias_list_t*** destination, const rapidjson::Value& container) const
    {
        const auto rapidjson_key = RAPIDJSON_STR(name.data());
        if (container.HasMember(rapidjson_key) && container[rapidjson_key].IsArray())
        {
            const auto arr = container[rapidjson_key].GetArray();
            if (destination)
            {
                *destination = local_allocator.allocate_array<native::snd_alias_list_t*>(arr.Size());

                for (size_t i = 0; i < arr.Size(); i++)
                {
                    const auto& elem = arr[i];
                    if (elem.IsNull())
                    {
                        (*destination)[i] = nullptr; // Unnecessary
                    }
                    else
                    {
                        const auto val = elem.GetString();
                        (*destination)[i] = find<native::snd_alias_list_t>(native::XAssetType::ASSET_TYPE_SOUND, val);
                    }
                }
            }
        }
    }

    void iweapon::read_xmodels_array(const std::string& name, native::XModel*** destination, const rapidjson::Value& container) const
    {
        const auto rapidjson_key = RAPIDJSON_STR(name.data());
        if (container.HasMember(rapidjson_key) && container[rapidjson_key].IsArray())
        {
            const auto arr = container[rapidjson_key].GetArray();
            if (destination)
            {
                *destination = local_allocator.allocate_array<native::XModel*>(arr.Size());

                for (size_t i = 0; i < arr.Size(); i++)
                {
                    const auto& elem = arr[i];

                    if (elem.IsNull())
                    {
                        continue;
                    }

                    const auto val = elem.GetString();
                    (*destination)[i] = find<native::XModel>(native::XAssetType::ASSET_TYPE_XMODEL, val);
                }
            }
        }
    }

    void iweapon::write_weapon_anims(const std::string& name, const char** arr, const char** original_arr, rapidjson::Value& container,
                                     rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const
    {
        bool should_write = original_arr == nullptr;
        if (arr)
        {
            rapidjson::Value szAnims(rapidjson::kObjectType);
            for (size_t i = 0; i < native::weapAnimFiles_t::NUM_WEAP_ANIMS; i++)
            {
                if (arr[i])
                {
                    szAnims.AddMember(RAPIDJSON_STR(native::weapAnimFiles_Names[i]), RAPIDJSON_STR(arr[i]), allocator);

                    if (strnlen(arr[i], 1) > 0)
                    {
                        const auto anim = assets->find_other_asset(native::ASSET_TYPE_XANIMPARTS, arr[i]);
                        if (anim)
                        {
                            assets->write(native::ASSET_TYPE_XANIMPARTS, anim);
                        }
                        else
                        {
                            print_error("Could not find anim {}!", arr[i]);
                        }
                    }
                }
                else
                {
                    szAnims.AddMember(RAPIDJSON_STR(native::weapAnimFiles_Names[i]), rapidjson::Value(rapidjson::kNullType), allocator);
                }

                if (!original_arr || 
                    ((arr[i] == nullptr) != (original_arr[i] == nullptr)) ||
                    arr[i] && std::string(arr[i]) != original_arr[i]
                )
                {
                    should_write = true;
                }
            }

            if (should_write)
            {
                const auto str = local_allocator.duplicate_string(name);
                container.AddMember(RAPIDJSON_STR(str), szAnims, allocator);
            }
        }
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

#define WRITE_MEMBER_NAMED_ENUM(obj, member, name_type)                                                   \
    if (original_weapon == nullptr || obj->member != original_##obj->member)                              \
    {                                                                                                     \
        container.AddMember(#member, RAPIDJSON_STR(name_type[static_cast<int>(obj->member)]), allocator); \
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

#define WRITE_MEMBER_ARRAY(obj, member) WRITE_MEMBER_ARRAY_WITH_COUNT(obj, member, ARRAYSIZE(obj->member))

#define WRITE_MEMBER_ARRAY_WITH_COUNT(obj, member, count)        \
    {                                                            \
        const size_t _array_size = count;                        \
        bool _write = false;                                     \
        if (original_weapon != nullptr)                          \
        {                                                        \
            for (size_t _ = 0; _ < _array_size; _++)             \
            {                                                    \
                if (obj->member[_] != original_##obj->member[_]) \
                {                                                \
                    _write = true;                               \
                    break;                                       \
                }                                                \
            }                                                    \
        }                                                        \
                                                                 \
        if (original_weapon == nullptr || _write)                \
        {                                                        \
            rapidjson::Value arr(rapidjson::kArrayType);         \
            for (size_t _ = 0; _ < _array_size; _++)             \
            {                                                    \
                arr.PushBack(obj->member[_], allocator);         \
            }                                                    \
            container.AddMember(#member, arr, allocator);        \
        }                                                        \
    }

#define WRITE_MATERIAL_NAME(obj, member)                                                                              \
    if (obj->member)                                                                                                  \
    {                                                                                                                 \
        if (original_weapon == nullptr || original_##obj->member == nullptr || obj->member != original_##obj->member) \
        {                                                                                                             \
            container.AddMember(#member, RAPIDJSON_STR(obj->member->info.name), allocator);                           \
        }                                                                                                             \
    }

#define WRITE_MEMBER_MATERIAL(obj, member)                                   \
    if (obj->member)                                                         \
    {                                                                        \
        assets->write(native::XAssetType::ASSET_TYPE_MATERIAL, obj->member); \
        WRITE_MATERIAL_NAME(obj, member);                                    \
    }

#define WRITE_SOUND_NAME(obj, member)                                                                                 \
    if (obj->member)                                                                                                  \
    {                                                                                                                 \
        if (original_weapon == nullptr || original_##obj->member == nullptr || obj->member != original_##obj->member) \
        {                                                                                                             \
            container.AddMember(#member, RAPIDJSON_STR(obj->member->aliasName), allocator);                           \
        }                                                                                                             \
    }

#define WRITE_ASSET_NAME(obj, member)                                                                                 \
    if (obj->member)                                                                                                  \
    {                                                                                                                 \
        if (original_weapon == nullptr || original_##obj->member == nullptr || obj->member != original_##obj->member) \
        {                                                                                                             \
            container.AddMember(#member, RAPIDJSON_STR(obj->member->name), allocator);                                \
        }                                                                                                             \
    }

#define WRITE_MEMBER_IF_NOT_NULL(obj, member) \
    if (obj->member)                          \
    {                                         \
        WRITE_MEMBER(obj, member);            \
    }

#define WRITE_MEMBER_SOUND(obj, member)                                   \
    if (obj->member)                                                      \
    {                                                                     \
        assets->write(native::XAssetType::ASSET_TYPE_SOUND, obj->member); \
        WRITE_SOUND_NAME(obj, member);                                    \
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
            name = name.substr(1, name.length() - suffix.length() - 1);
        }

        container.AddMember("szInternalName", RAPIDJSON_STR(local_allocator.duplicate_string(name)), allocator);

        WRITE_STR_MEMBER_IF_NOT_NULL(weapon, szDisplayName);

        WRITE_SCRIPTSTRING_MEMBER_ARRAY(weapon, hideTags, 32);
        write_weapon_anims("szXAnims", weapon->szXAnims, original_weapon ? original_weapon->szXAnims : nullptr, container, allocator);

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
        if (original_weapon &&
            weapon->weapDef->inventoryType !=
                native::WEAPINVENTORY_ALTMODE && // << Prevents stack overflow due to infinite weapon looping between alt & main
            weapon->szAltWeaponName &&
            strnlen(weapon->szAltWeaponName, 1) > 0)
        {
            const auto alt = assets->find_other_asset(native::ASSET_TYPE_WEAPON, weapon->szAltWeaponName);
            if (alt)
            {
                assets->write(native::ASSET_TYPE_WEAPON, alt);
            }
            else
            {
                print_error("Could not find alt weapon {} for weapon {}!", weapon->szAltWeaponName, full_name);
            }
        }

        WRITE_MEMBER_IF_NOT_NULL(weapon, altWeaponIndex);
        WRITE_MEMBER_IF_NOT_NULL(weapon, iAltRaiseTime);
        WRITE_MEMBER_MATERIAL(weapon, killIcon);
        WRITE_MEMBER_MATERIAL(weapon, dpadIcon);
        WRITE_MEMBER_IF_NOT_NULL(weapon, fireAnimLength);
        WRITE_MEMBER_IF_NOT_NULL(weapon, iFirstRaiseTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon, ammoDropStockMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon, adsDofStart);
        WRITE_MEMBER_IF_NOT_NULL(weapon, adsDofEnd);

        {
            rapidjson::Value accuracy_graphs(rapidjson::kArrayType);
            bool should_write = original_weapon == nullptr;
            for (size_t i = 0; i < 2; i++)
            {
                rapidjson::Value accuracy_graph_knots(rapidjson::kArrayType);
                const auto same_knot_count_as_original =
                    original_weapon && weapon->accuracyGraphKnotCount[i] == original_weapon->accuracyGraphKnotCount[i];

                for (size_t j = 0; j < weapon->accuracyGraphKnotCount[i]; j++)
                {
                    const auto knot = weapon->accuracyGraphKnots[i][j];

                    if (same_knot_count_as_original && *knot != *original_weapon->accuracyGraphKnots[i][j])
                    {
                        should_write = true;
                    }

                    accuracy_graph_knots.PushBack(*knot, allocator);
                }

                accuracy_graphs.PushBack(accuracy_graph_knots, allocator);
            }

            if (should_write)
            {
                container.AddMember("accuracyGraphs", accuracy_graphs, allocator);
            }
        }

        WRITE_MEMBER_IF_NOT_NULL(weapon, motionTracker);
        WRITE_MEMBER_IF_NOT_NULL(weapon, enhanced);
        WRITE_MEMBER_IF_NOT_NULL(weapon, dpadIconShowsAmmo);

        // Dump weapondef
        const auto weapon_def = weapon->weapDef;
        const auto original_weapon_def = (original_weapon ? original_weapon->weapDef : nullptr);

        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szOverlayName);
        this->write_xmodels_array(
            "gunXModel", weapon_def->gunXModel, 16, original_weapon_def ? original_weapon_def->gunXModel : nullptr, container, allocator);

        WRITE_MEMBER_ASSET(weapon->weapDef, handXModel, native::XAssetType::ASSET_TYPE_XMODEL);

        // szXAnimsRightHanded
        write_weapon_anims("szXAnimsRightHanded",
                           weapon->weapDef->szXAnimsRightHanded,
                           original_weapon_def ? original_weapon_def->szXAnimsRightHanded : nullptr,
                           container,
                           allocator);
        // szXAnimsLeftHanded
        write_weapon_anims("szXAnimsLeftHanded",
                           weapon->weapDef->szXAnimsLeftHanded,
                           original_weapon_def ? original_weapon_def->szXAnimsLeftHanded : nullptr,
                           container,
                           allocator);

        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szModeName);
        WRITE_SCRIPTSTRING_MEMBER_ARRAY(weapon->weapDef, notetrackSoundMapKeys, 16);
        WRITE_SCRIPTSTRING_MEMBER_ARRAY(weapon->weapDef, notetrackSoundMapValues, 16);
        WRITE_SCRIPTSTRING_MEMBER_ARRAY(weapon->weapDef, notetrackRumbleMapKeys, 16);
        WRITE_SCRIPTSTRING_MEMBER_ARRAY(weapon->weapDef, notetrackRumbleMapValues, 16);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, playerAnimType);

        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, weapType, iw4of::native::weapType_t_Names);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, weapClass, iw4of::native::weapClass_t_Names);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, penetrateType, iw4of::native::PenetrateType_Names);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, inventoryType, iw4of::native::weapInventoryType_t_Names);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, fireType, iw4of::native::weapFireType_t_Names);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, offhandClass, iw4of::native::OffhandClass_Names);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, stance, iw4of::native::weapStance_t_Names);

        WRITE_MEMBER_ASSET(weapon->weapDef, viewFlashEffect, native::XAssetType::ASSET_TYPE_FX);
        WRITE_MEMBER_ASSET(weapon->weapDef, worldFlashEffect, native::XAssetType::ASSET_TYPE_FX);

        WRITE_MEMBER_SOUND(weapon->weapDef, pickupSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, pickupSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, ammoPickupSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, ammoPickupSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, projectileSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, pullbackSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, pullbackSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, fireSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, fireSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, fireSoundPlayerAkimbo);
        WRITE_MEMBER_SOUND(weapon->weapDef, fireLoopSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, fireLoopSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, fireStopSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, fireStopSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, fireLastSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, fireLastSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, emptyFireSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, emptyFireSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, meleeSwipeSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, meleeSwipeSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, meleeHitSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, meleeMissSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, rechamberSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, rechamberSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, reloadSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, reloadSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, reloadEmptySound);
        WRITE_MEMBER_SOUND(weapon->weapDef, reloadEmptySoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, reloadStartSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, reloadStartSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, reloadEndSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, reloadEndSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, detonateSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, detonateSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, nightVisionWearSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, nightVisionWearSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, nightVisionRemoveSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, nightVisionRemoveSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, altSwitchSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, altSwitchSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, raiseSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, raiseSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, firstRaiseSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, firstRaiseSoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, putawaySound);
        WRITE_MEMBER_SOUND(weapon->weapDef, putawaySoundPlayer);
        WRITE_MEMBER_SOUND(weapon->weapDef, scanSound);

        write_sounds_array(
            "bounceSound", weapon_def->bounceSound, 31, original_weapon_def ? original_weapon_def->bounceSound : nullptr, container, allocator);

        WRITE_MEMBER_ASSET(weapon->weapDef, viewShellEjectEffect, native::XAssetType::ASSET_TYPE_FX);
        WRITE_MEMBER_ASSET(weapon->weapDef, worldShellEjectEffect, native::XAssetType::ASSET_TYPE_FX);
        WRITE_MEMBER_ASSET(weapon->weapDef, viewLastShotEjectEffect, native::XAssetType::ASSET_TYPE_FX);
        WRITE_MEMBER_ASSET(weapon->weapDef, worldLastShotEjectEffect, native::XAssetType::ASSET_TYPE_FX);

        WRITE_MEMBER_MATERIAL(weapon->weapDef, reticleCenter);
        WRITE_MEMBER_MATERIAL(weapon->weapDef, reticleSide);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReticleCenterSize);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReticleSideSize);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReticleMinOfs);

        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, activeReticleType, iw4of::native::activeReticleType_t_Names);

        WRITE_MEMBER_ARRAY(weapon->weapDef, vStandMove);
        WRITE_MEMBER_ARRAY(weapon->weapDef, vStandRot);
        WRITE_MEMBER_ARRAY(weapon->weapDef, strafeMove);
        WRITE_MEMBER_ARRAY(weapon->weapDef, strafeRot);
        WRITE_MEMBER_ARRAY(weapon->weapDef, vDuckedOfs);
        WRITE_MEMBER_ARRAY(weapon->weapDef, vDuckedMove);
        WRITE_MEMBER_ARRAY(weapon->weapDef, vDuckedRot);
        WRITE_MEMBER_ARRAY(weapon->weapDef, vProneOfs);
        WRITE_MEMBER_ARRAY(weapon->weapDef, vProneMove);
        WRITE_MEMBER_ARRAY(weapon->weapDef, vProneRot);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fPosMoveRate);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fPosProneMoveRate);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fStandMoveMinSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fDuckedMoveMinSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fProneMoveMinSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fPosRotRate);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fPosProneRotRate);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fStandRotMinSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fDuckedRotMinSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fProneRotMinSpeed);

        this->write_xmodels_array(
            "worldModel", weapon_def->worldModel, 16, original_weapon_def ? original_weapon_def->worldModel : nullptr, container, allocator);

        WRITE_MEMBER_ASSET(weapon->weapDef, worldClipModel, native::ASSET_TYPE_XMODEL);
        WRITE_MEMBER_ASSET(weapon->weapDef, rocketModel, native::ASSET_TYPE_XMODEL);
        WRITE_MEMBER_ASSET(weapon->weapDef, knifeModel, native::ASSET_TYPE_XMODEL);
        WRITE_MEMBER_ASSET(weapon->weapDef, worldKnifeModel, native::ASSET_TYPE_XMODEL);
        WRITE_MEMBER_MATERIAL(weapon->weapDef, hudIcon);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, hudIconRatio, iw4of::native::weaponIconRatioType_t_Names);
        WRITE_MEMBER_MATERIAL(weapon->weapDef, pickupIcon);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, pickupIconRatio, iw4of::native::weaponIconRatioType_t_Names);
        WRITE_MEMBER_MATERIAL(weapon->weapDef, ammoCounterIcon);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, ammoCounterIconRatio, iw4of::native::weaponIconRatioType_t_Names);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, ammoCounterClip, iw4of::native::ammoCounterClipType_t_Names);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iStartAmmo);
        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szAmmoName);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iAmmoIndex);
        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szClipName);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iClipIndex);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iMaxAmmo);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, shotCount);
        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szSharedAmmoCapName);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iSharedAmmoCapIndex);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iSharedAmmoCap);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, damage);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, playerDamage);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iMeleeDamage);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iDamageType);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iFireDelay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iMeleeDelay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, meleeChargeDelay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iDetonateDelay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iRechamberTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, rechamberTimeOneHanded);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iRechamberBoltTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iHoldFireTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iDetonateTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iMeleeTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, meleeChargeTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, reloadShowRocketTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadEmptyTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadAddTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadStartTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadStartAddTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadEndTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iDropTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iRaiseTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iAltDropTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, quickDropTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, quickRaiseTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iBreachRaiseTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iEmptyRaiseTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iEmptyDropTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, sprintInTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, sprintLoopTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, sprintOutTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, stunnedTimeBegin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, stunnedTimeLoop);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, stunnedTimeEnd);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionWearTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionWearTimeFadeOutEnd);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionWearTimePowerUp);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionRemoveTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionRemoveTimePowerDown);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionRemoveTimeFadeInStart);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fuseTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, aiFuseTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, autoAimRange);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, aimAssistRange);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, aimAssistRangeAds);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, aimPadding);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, enemyCrosshairRange);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, moveSpeedScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsMoveSpeedScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, sprintDurationScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsZoomInFrac);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsZoomOutFrac);

        WRITE_MEMBER_MATERIAL(weapon->weapDef, overlayMaterial);
        WRITE_MEMBER_MATERIAL(weapon->weapDef, overlayMaterialLowRes);
        WRITE_MEMBER_MATERIAL(weapon->weapDef, overlayMaterialEMP);
        WRITE_MEMBER_MATERIAL(weapon->weapDef, overlayMaterialEMPLowRes);

        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, overlayReticle, iw4of::native::weapOverlayReticle_t_Names);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, overlayInterface, iw4of::native::WeapOverlayInteface_t_Names);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, overlayWidth);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, overlayHeight);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, overlayWidthSplitscreen);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, overlayHeightSplitscreen);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsBobFactor);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewBobMult);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadStandMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadDuckedMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadProneMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, hipSpreadStandMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, hipSpreadDuckedMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, hipSpreadProneMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadDecayRate);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadFireAdd);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadTurnAdd);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadMoveAdd);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadDuckedDecay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadProneDecay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipReticleSidePos);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsIdleAmount);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipIdleAmount);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsIdleSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, hipIdleSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fIdleCrouchFactor);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fIdleProneFactor);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fGunMaxPitch);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fGunMaxYaw);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, swayMaxAngle);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, swayLerpSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, swayPitchScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, swayYawScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, swayHorizScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, swayVertScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, swayShellShockScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayMaxAngle);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayLerpSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayPitchScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayYawScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayHorizScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayVertScale);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsViewErrorMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsViewErrorMax);

        // WRITE_MEMBER_ASSET(weapon->weapDef, physCollmap, native::ASSET_TYPE_PHYSCOLLMAP);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, dualWieldViewModelOffset);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, killIconRatio, native::weaponIconRatioType_t_Names);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadAmmoAdd);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadStartAdd);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, ammoDropStockMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, ammoDropClipPercentMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, ammoDropClipPercentMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iExplosionRadius);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iExplosionRadiusMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iExplosionInnerDamage);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iExplosionOuterDamage);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, damageConeAngle);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bulletExplDmgMult);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bulletExplRadiusMult);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iProjectileSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iProjectileSpeedUp);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iProjectileSpeedForward);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iProjectileActivateDist);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, projLifetime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, timeToAccelerate);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, projectileCurvature);

        WRITE_MEMBER_ASSET(weapon->weapDef, projectileModel, native::ASSET_TYPE_XMODEL);

        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, projExplosion, native::weapProjExposion_t_Names);
        WRITE_MEMBER_ASSET(weapon->weapDef, projExplosionEffect, native::ASSET_TYPE_FX);
        WRITE_MEMBER_ASSET(weapon->weapDef, projDudEffect, native::ASSET_TYPE_FX);
        WRITE_MEMBER_SOUND(weapon->weapDef, projExplosionSound);
        WRITE_MEMBER_SOUND(weapon->weapDef, projDudSound);

        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, stickiness, native::WeapStickinessType_Names);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, lowAmmoWarningThreshold);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, ricochetChance);

        // Bounce
        if (weapon->weapDef->parallelBounce)
        {
            bool should_write = original_weapon_def == nullptr;
            rapidjson::Value parallel_bounce(rapidjson::kArrayType);
            for (size_t i = 0; i < 31; i++)
            {
                should_write |= original_weapon_def == nullptr || original_weapon_def->parallelBounce[i] != weapon_def->parallelBounce[i];
                parallel_bounce.PushBack(weapon->weapDef->parallelBounce[i], allocator);
            }

            if (should_write)
            {
                container.AddMember("parallelBounce", parallel_bounce, allocator);
            }
        }

        if (weapon->weapDef->perpendicularBounce)
        {
            bool should_write = original_weapon_def == nullptr;
            rapidjson::Value perpendicular_bounce(rapidjson::kArrayType);
            for (size_t i = 0; i < 31; i++)
            {
                should_write |= original_weapon_def == nullptr || original_weapon_def->perpendicularBounce[i] != weapon_def->perpendicularBounce[i];

                perpendicular_bounce.PushBack(weapon->weapDef->perpendicularBounce[i], allocator);
            }

            if (should_write)
            {
                container.AddMember("perpendicularBounce", perpendicular_bounce, allocator);
            }
        }

        WRITE_MEMBER_ASSET(weapon->weapDef, projTrailEffect, native::ASSET_TYPE_FX);
        WRITE_MEMBER_ASSET(weapon->weapDef, projBeaconEffect, native::ASSET_TYPE_FX);

        WRITE_MEMBER_ARRAY_WITH_COUNT(weapon->weapDef, vProjectileColor, 3);
        WRITE_MEMBER_NAMED_ENUM(weapon->weapDef, guidedMissileType, native::guidedMissileType_t_Names);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, maxSteeringAccel);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, projIgnitionDelay);
        WRITE_MEMBER_ASSET(weapon->weapDef, projIgnitionEffect, native::ASSET_TYPE_FX);
        WRITE_MEMBER_SOUND(weapon->weapDef, projIgnitionSound);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsAimPitch);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsCrosshairInFrac);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsCrosshairOutFrac);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsGunKickReducedKickBullets);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsGunKickReducedKickPercent);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickPitchMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickPitchMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickYawMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickYawMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickAccel);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickSpeedMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickSpeedDecay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickStaticDecay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewKickPitchMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewKickPitchMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewKickYawMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewKickYawMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewScatterMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewScatterMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsSpread);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, hipGunKickReducedKickBullets);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, hipGunKickReducedKickPercent);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickPitchMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickPitchMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickYawMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickYawMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickAccel);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickSpeedMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickSpeedDecay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickStaticDecay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewKickPitchMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewKickPitchMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewKickYawMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewKickYawMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewScatterMin);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewScatterMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fightDist);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, maxDist);

        {
            rapidjson::Value original_accuracy_graphs(rapidjson::kArrayType);
            bool should_write = original_weapon == nullptr;
            for (size_t i = 0; i < 2; i++)
            {
                rapidjson::Value graph(rapidjson::kObjectType);
                rapidjson::Value original_accuracy_graph_knots(rapidjson::kArrayType);
                const auto same_knot_count_as_original =
                    original_weapon && weapon_def->originalAccuracyGraphKnotCount[i] == original_weapon_def->originalAccuracyGraphKnotCount[i];

                for (size_t j = 0; j < weapon_def->originalAccuracyGraphKnotCount[i]; j++)
                {
                    const auto knot = weapon_def->originalAccuracyGraphKnots[i][j];

                    if (same_knot_count_as_original && *knot != *original_weapon_def->originalAccuracyGraphKnots[i][j])
                    {
                        should_write = true;
                    }

                    original_accuracy_graph_knots.PushBack(*knot, allocator);
                }

                graph.AddMember("knots", original_accuracy_graph_knots, allocator);
                graph.AddMember("accuracyGraphName", RAPIDJSON_STR(weapon_def->accuracyGraphName[i]), allocator);

                original_accuracy_graphs.PushBack(graph, allocator);
            }

            if (should_write)
            {
                container.AddMember("originalAccuracyGraphs", original_accuracy_graphs, allocator);
            }
        }

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iPositionReloadTransTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, leftArc);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, rightArc);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, topArc);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bottomArc);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, accuracy);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, aiSpread);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, playerSpread);

        WRITE_MEMBER_ARRAY(weapon->weapDef, minTurnSpeed);
        WRITE_MEMBER_ARRAY(weapon->weapDef, maxTurnSpeed);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, pitchConvergenceTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, yawConvergenceTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, suppressTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, maxRange);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fAnimHorRotateInc);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fPlayerPositionDist);

        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szUseHintString);
        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, dropHintString);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, iUseHintStringIndex);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, dropHintStringIndex);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, horizViewJitter);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, vertViewJitter);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, scanSpeed);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, scanAccel);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, scanPauseTime);

        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szScript);
        WRITE_MEMBER_ARRAY(weapon->weapDef, fOOPosAnimLength);

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, minDamage);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, minPlayerDamage);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fMaxDamageRange);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, fMinDamageRange);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, destabilizationRateTime);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, destabilizationCurvatureMax);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, destabilizeDistance);

        WRITE_MEMBER_ARRAY_WITH_COUNT(weapon->weapDef, locationDamageMultipliers, 20);
        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, fireRumble);
        WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, meleeImpactRumble);

        WRITE_MEMBER_ASSET(weapon->weapDef, tracerType, native::ASSET_TYPE_TRACER);

        if (weapon_def->weapClass == native::WEAPCLASS_TURRET)
        {
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretScopeZoomRate);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretScopeZoomMin);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretScopeZoomMax);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretOverheatUpRate);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretOverheatDownRate);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretOverheatPenalty);

            WRITE_MEMBER_SOUND(weapon->weapDef, turretOverheatSound);
            WRITE_MEMBER_ASSET(weapon->weapDef, turretOverheatEffect, native::ASSET_TYPE_FX);
            WRITE_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinRumble);

            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinSpeed);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinUpTime);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinDownTime);

            WRITE_MEMBER_SOUND(weapon->weapDef, turretBarrelSpinMaxSnd);

            write_sounds_array("turretBarrelSpinUpSnd",
                               weapon_def->turretBarrelSpinUpSnd,
                               4,
                               original_weapon_def ? original_weapon_def->turretBarrelSpinUpSnd : nullptr,
                               container,
                               allocator);
            write_sounds_array("turretBarrelSpinDownSnd",
                               weapon_def->turretBarrelSpinDownSnd,
                               4,
                               original_weapon_def ? original_weapon_def->turretBarrelSpinDownSnd : nullptr,
                               container,
                               allocator);
        }

        if (weapon_def->weapClass == native::weapClass_t::WEAPCLASS_ROCKETLAUNCHER)
        {
            WRITE_MEMBER_SOUND(weapon->weapDef, missileConeSoundAlias);
            WRITE_MEMBER_SOUND(weapon->weapDef, missileConeSoundAliasAtBase);

            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundRadiusAtTop);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundRadiusAtBase);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundHeight);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundOriginOffset);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundVolumescaleAtCore);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundVolumescaleAtEdge);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundVolumescaleCoreSize);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchAtTop);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchAtBottom);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchTopSize);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchBottomSize);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundCrossfadeTopSize);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundCrossfadeBottomSize);
        }

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, sharedAmmo);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, lockonSupported);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, requireLockonToFire);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bigExplosion);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, noAdsWhenMagEmpty);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, avoidDropCleanup);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, inheritsPerks);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, crosshairColorChange);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bRifleBullet);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, armorPiercing);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bBoltAction);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, aimDownSight);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bRechamberWhileAds);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bBulletExplosiveDamage);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bCookOffHold);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bClipOnly);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, noAmmoPickup);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, adsFireOnly);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, cancelAutoHolsterWhenEmpty);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, disableSwitchToWhenEmpty);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, suppressAmmoReserveDisplay);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, laserSightDuringNightvision);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, markableViewmodel);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, noDualWield);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, flipKillIcon);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bNoPartialReload);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bSegmentedReload);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, blocksProne);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, silenced);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, isRollingGrenade);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, projExplosionEffectForceNormalUp);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, bProjImpactExplode);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, stickToPlayers);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, hasDetonator);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, disableFiring);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, timedDetonation);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, rotate);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, holdButtonToThrow);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, freezeMovementWhenFiring);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, thermalScope);
        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, altModeSameWeapon);

        if (weapon_def->weapClass == native::weapClass_t::WEAPCLASS_TURRET)
        {
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinEnabled);
        }

        if (weapon_def->weapClass == native::weapClass_t::WEAPCLASS_ROCKETLAUNCHER)
        {
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundEnabled);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchshiftEnabled);
            WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundCrossfadeEnabled);
        }

        WRITE_MEMBER_IF_NOT_NULL(weapon->weapDef, offhandHoldIsCancelable);

        // dump variants
        rapidjson::Value variants(rapidjson::kArrayType);

        for (const auto& attachment_name : weapon_attachments)
        {
            std::string variant_name = base_name + "_" + attachment_name + suffix;
            if (auto* variant_weapon = find<native::WeaponCompleteDef>(native::ASSET_TYPE_WEAPON, variant_name))
            {
                rapidjson::Value variant_container(rapidjson::kObjectType);
                write_variant(variant_container, allocator, {const_cast<iw4of::native::WeaponCompleteDef*>(weapon)}, {variant_weapon});

                assets->mark_written_elsewhere(native::ASSET_TYPE_WEAPON, variant_weapon);
                variants.PushBack(variant_container, allocator);
            }
        }

        if (variants.Size() > 0)
        {
            container.AddMember("variants", variants, allocator);
        }

        return true;
    } // namespace iw4of::interfaces

    bool iweapon::read_variant(native::XAssetHeader& asset, const rapidjson::Value& schematic, const native::XAssetHeader& original_asset) const
    {
        // copy original
        asset.weapon = local_allocator.allocate<native::WeaponCompleteDef>();

        const auto weapon = asset.weapon;
        const auto original = original_asset.weapon;

        if (original)
        {
            std::memcpy(weapon, original, sizeof native::WeaponCompleteDef);
        }

        weapon->weapDef = local_allocator.allocate<native::WeaponDef>();

        if (original)
        {
            std::memcpy(weapon->weapDef, original->weapDef, sizeof native::WeaponDef);
        }

        const rapidjson::Value* obj = nullptr;

        if (original)
        {
            obj = &schematic;
        }
        else
        {
            obj = &schematic["weapon"];
        }

        const rapidjson::Value& json_variant = *obj;

        const auto json_name = json_variant["szInternalName"].GetString();

        std::string full_name;

        std::string suffix{};
        std::string original_name{};
        std::string base_name{};

        if (original)
        {
            original_name = original->szInternalName;
        }
        else
        {
            full_name = json_name;
            original_name = full_name;
        }

        if (original_name.ends_with("_mp"))
        {
            suffix = "_mp";
        }

        base_name = original_name.substr(0, original_name.size() - suffix.size());

        if (original)
        {
            std::string variant_name = json_name;
            full_name = base_name + "_" + variant_name + suffix;
        }

        weapon->szInternalName = local_allocator.duplicate_string(full_name);

#define READ_MEMBER_ASSET(obj, member, t, xasset_type)                         \
    if (json_variant.HasMember(#member) && json_variant[#member].IsString())   \
    {                                                                          \
        obj->member = find<t>(xasset_type, json_variant[#member].GetString()); \
    }

#define READ_MEMBER_SOUND(obj, member) READ_MEMBER_ASSET(obj, member, native::snd_alias_list_t, native::XAssetType::ASSET_TYPE_SOUND)
#define READ_MEMBER_MATERIAL(obj, member) READ_MEMBER_ASSET(obj, member, native::Material, native::XAssetType::ASSET_TYPE_MATERIAL)

#define READ_MEMBER_ARRAY_WITH_COUNT(obj, member, count, t)                 \
    if (json_variant.HasMember(#member) && json_variant[#member].IsArray()) \
    {                                                                       \
        const rapidjson::Value& arr = json_variant[#member];                \
        if (arr.Size() == count)                                            \
        {                                                                   \
            obj->member = local_allocator.allocate_array<t>(count);         \
            for (rapidjson::SizeType _ = 0; _ < count; _++)                 \
            {                                                               \
                obj->member[_] = static_cast<t>(arr[_].Get<double>());      \
            }                                                               \
        }                                                                   \
    }

#define READ_SCRIPTSTRING_MEMBER_ARRAY(obj, member, count)                                                                  \
    if (json_variant.HasMember(#member))                                                                                    \
    {                                                                                                                       \
        obj->member = local_allocator.allocate_array<uint16_t>(count);                                                      \
        const size_t _arrSize = static_cast<size_t>(json_variant[#member].Size());                                          \
        for (size_t _ = 0; _ < std::min(static_cast<size_t>(count), _arrSize); _++)                                         \
        {                                                                                                                   \
            if (json_variant[#member][_].IsString())                                                                        \
            {                                                                                                               \
                obj->member[_] = static_cast<uint16_t>(assets->write_in_stringtable(json_variant[#member][_].GetString())); \
            }                                                                                                               \
        }                                                                                                                   \
    }
#define READ_MEMBER_NAMED_ENUM(obj, member, enm_type)                                                 \
    if (json_variant.HasMember(#member) && !json_variant[#member].IsNull())                           \
    {                                                                                                 \
        bool _found = false;                                                                          \
        for (size_t _ = 0; _ < ARRAYSIZE(##enm_type##_Names); _++)                                    \
        {                                                                                             \
            if (std::string(##enm_type##_Names[_]) == std::string(json_variant[#member].GetString())) \
            {                                                                                         \
                obj->member = static_cast<enm_type>(_);                                               \
                _found = true;                                                                        \
                break;                                                                                \
            }                                                                                         \
        }                                                                                             \
                                                                                                      \
        if (!_found) obj->member = static_cast<enm_type>(0);                                          \
    }

#define READ_STR_MEMBER_IF_NOT_NULL(obj, member)                             \
    if (json_variant.HasMember(#member) && json_variant[#member].IsString()) \
    {                                                                        \
        std::string _json_string = json_variant[#member].GetString();        \
        if (!obj->member || _json_string != obj->member)                     \
        {                                                                    \
            obj->member = local_allocator.duplicate_string(_json_string);    \
        }                                                                    \
    }

#define READ_MEMBER_ARRAY(obj, member, t)                                        \
    {                                                                            \
        if (json_variant.HasMember(#member) && json_variant[#member].IsArray())  \
        {                                                                        \
            const rapidjson::Value& _arr = json_variant[#member];                \
            if (_arr.Size() == ARRAYSIZE(obj->member))                           \
            {                                                                    \
                for (rapidjson::SizeType _ = 0; _ < ARRAYSIZE(obj->member); _++) \
                {                                                                \
                    obj->member[_] = static_cast<t>(_arr[_].Get<double>());      \
                }                                                                \
            }                                                                    \
        }                                                                        \
    }

#define READ_MEMBER_IF_NOT_NULL(obj, member, t)                            \
    if (json_variant.HasMember(#member))                                   \
    {                                                                      \
        obj->member = static_cast<t>(json_variant[#member].Get<double>()); \
    }

#define READ_INT_MEMBER_IF_NOT_NULL(obj, member) READ_MEMBER_IF_NOT_NULL(obj, member, int)
#define READ_FLOAT_MEMBER_IF_NOT_NULL(obj, member) READ_MEMBER_IF_NOT_NULL(obj, member, float)
#define READ_BOOL_MEMBER_IF_NOT_NULL(obj, member)        \
    if (json_variant.HasMember(#member))                 \
    {                                                    \
        obj->member = json_variant[#member].Get<bool>(); \
    }

        READ_STR_MEMBER_IF_NOT_NULL(weapon, szDisplayName);

        READ_SCRIPTSTRING_MEMBER_ARRAY(weapon, hideTags, 32);
        read_weapon_anims("szXAnims", &weapon->szXAnims, json_variant);

        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon, fAdsZoomFov);
        READ_INT_MEMBER_IF_NOT_NULL(weapon, iAdsTransInTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon, iAdsTransOutTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon, iClipSize);
        READ_MEMBER_IF_NOT_NULL(weapon, impactType, native::ImpactType);
        READ_INT_MEMBER_IF_NOT_NULL(weapon, iFireTime);
        READ_MEMBER_IF_NOT_NULL(weapon, dpadIconRatio, native::weaponIconRatioType_t);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon, penetrateMultiplier);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon, fAdsViewKickCenterSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon, fHipViewKickCenterSpeed);

        READ_STR_MEMBER_IF_NOT_NULL(weapon, szAltWeaponName);

        if (weapon->szAltWeaponName == nullptr)
        {
            weapon->szAltWeaponName = local_allocator.duplicate_string("");
        }

        if (original && weapon->szAltWeaponName && strnlen(weapon->szAltWeaponName, 1) > 0)
        {
            // Ignore if missing, an error will pop up during marking anyway
            [[maybe_unused]] const auto altWeapon =
                reinterpret_cast<native::XAnimParts*>(assets->find_other_asset(native::ASSET_TYPE_WEAPON, weapon->szAltWeaponName));
        }

        READ_INT_MEMBER_IF_NOT_NULL(weapon, altWeaponIndex);
        READ_INT_MEMBER_IF_NOT_NULL(weapon, iAltRaiseTime);
        READ_MEMBER_ASSET(weapon, killIcon, native::Material, native::XAssetType::ASSET_TYPE_MATERIAL);
        READ_MEMBER_ASSET(weapon, dpadIcon, native::Material, native::XAssetType::ASSET_TYPE_MATERIAL);
        READ_INT_MEMBER_IF_NOT_NULL(weapon, fireAnimLength);
        READ_INT_MEMBER_IF_NOT_NULL(weapon, iFirstRaiseTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon, ammoDropStockMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon, adsDofStart);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon, adsDofEnd);

        if (json_variant.HasMember("accuracyGraphs") && json_variant["accuracyGraphs"].IsArray())
        {
            weapon->accuracyGraphKnotCount[0] = 0;
            weapon->accuracyGraphKnotCount[1] = 0;

            // Check assault_rifle.accu for an example of what the data looks like
            // const auto arr =json_variant["accuracyGraphs"].GetArray();
            //         for (size_t i = 0; i < std::min(2, static_cast<int>(arr.Size())); i++)
            //{
            //	const auto knots = arr[i].GetArray();
            //	weapon->accuracyGraphKnotCount[i] = knots.Size();

            //	weapon->accuracyGraphKnots[i] =
            // reinterpret_cast<float(*)[2]>(local_allocator.allocate_array<float*>(weapon->accuracyGraphKnotCount[i]));

            //	for (size_t j = 0; j < weapon->accuracyGraphKnotCount[i]; j++)
            //	{
            //		*weapon->accuracyGraphKnots[i][j] = 1.f;
            //	}
            //}
        }

        READ_BOOL_MEMBER_IF_NOT_NULL(weapon, motionTracker);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon, enhanced);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon, dpadIconShowsAmmo);

        // Dump weapondef
        auto weapon_def = weapon->weapDef;

        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szOverlayName);
        this->read_xmodels_array("gunXModel", &weapon_def->gunXModel, json_variant);

        READ_MEMBER_ASSET(weapon->weapDef, handXModel, native::XModel, native::XAssetType::ASSET_TYPE_XMODEL);

        // szXAnimsRightHanded
        read_weapon_anims("szXAnimsRightHanded", &weapon->weapDef->szXAnimsRightHanded, json_variant);
        // szXAnimsLeftHanded
        read_weapon_anims("szXAnimsLeftHanded", &weapon->weapDef->szXAnimsLeftHanded, json_variant);

        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szModeName);
        READ_SCRIPTSTRING_MEMBER_ARRAY(weapon->weapDef, notetrackSoundMapKeys, 16);
        READ_SCRIPTSTRING_MEMBER_ARRAY(weapon->weapDef, notetrackSoundMapValues, 16);
        READ_SCRIPTSTRING_MEMBER_ARRAY(weapon->weapDef, notetrackRumbleMapKeys, 16);
        READ_SCRIPTSTRING_MEMBER_ARRAY(weapon->weapDef, notetrackRumbleMapValues, 16);

        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, playerAnimType);

        READ_MEMBER_NAMED_ENUM(weapon->weapDef, weapType, iw4of::native::weapType_t);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, weapClass, iw4of::native::weapClass_t);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, penetrateType, iw4of::native::PenetrateType);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, inventoryType, iw4of::native::weapInventoryType_t);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, fireType, iw4of::native::weapFireType_t);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, offhandClass, iw4of::native::OffhandClass);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, stance, iw4of::native::weapStance_t);

        READ_MEMBER_ASSET(weapon->weapDef, viewFlashEffect, native::FxEffectDef, native::XAssetType::ASSET_TYPE_FX);
        READ_MEMBER_ASSET(weapon->weapDef, worldFlashEffect, native::FxEffectDef, native::XAssetType::ASSET_TYPE_FX);

        READ_MEMBER_SOUND(weapon->weapDef, pickupSound);
        READ_MEMBER_SOUND(weapon->weapDef, pickupSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, ammoPickupSound);
        READ_MEMBER_SOUND(weapon->weapDef, ammoPickupSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, projectileSound);
        READ_MEMBER_SOUND(weapon->weapDef, pullbackSound);
        READ_MEMBER_SOUND(weapon->weapDef, pullbackSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, fireSound);
        READ_MEMBER_SOUND(weapon->weapDef, fireSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, fireSoundPlayerAkimbo);
        READ_MEMBER_SOUND(weapon->weapDef, fireLoopSound);
        READ_MEMBER_SOUND(weapon->weapDef, fireLoopSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, fireStopSound);
        READ_MEMBER_SOUND(weapon->weapDef, fireStopSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, fireLastSound);
        READ_MEMBER_SOUND(weapon->weapDef, fireLastSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, emptyFireSound);
        READ_MEMBER_SOUND(weapon->weapDef, emptyFireSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, meleeSwipeSound);
        READ_MEMBER_SOUND(weapon->weapDef, meleeSwipeSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, meleeHitSound);
        READ_MEMBER_SOUND(weapon->weapDef, meleeMissSound);
        READ_MEMBER_SOUND(weapon->weapDef, rechamberSound);
        READ_MEMBER_SOUND(weapon->weapDef, rechamberSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, reloadSound);
        READ_MEMBER_SOUND(weapon->weapDef, reloadSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, reloadEmptySound);
        READ_MEMBER_SOUND(weapon->weapDef, reloadEmptySoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, reloadStartSound);
        READ_MEMBER_SOUND(weapon->weapDef, reloadStartSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, reloadEndSound);
        READ_MEMBER_SOUND(weapon->weapDef, reloadEndSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, detonateSound);
        READ_MEMBER_SOUND(weapon->weapDef, detonateSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, nightVisionWearSound);
        READ_MEMBER_SOUND(weapon->weapDef, nightVisionWearSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, nightVisionRemoveSound);
        READ_MEMBER_SOUND(weapon->weapDef, nightVisionRemoveSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, altSwitchSound);
        READ_MEMBER_SOUND(weapon->weapDef, altSwitchSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, raiseSound);
        READ_MEMBER_SOUND(weapon->weapDef, raiseSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, firstRaiseSound);
        READ_MEMBER_SOUND(weapon->weapDef, firstRaiseSoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, putawaySound);
        READ_MEMBER_SOUND(weapon->weapDef, putawaySoundPlayer);
        READ_MEMBER_SOUND(weapon->weapDef, scanSound);

        read_sounds_array("bounceSound", reinterpret_cast<native::snd_alias_list_t***>(&weapon_def->bounceSound), json_variant);

        READ_MEMBER_ASSET(weapon->weapDef, viewShellEjectEffect, native::FxEffectDef, native::XAssetType::ASSET_TYPE_FX);
        READ_MEMBER_ASSET(weapon->weapDef, worldShellEjectEffect, native::FxEffectDef, native::XAssetType::ASSET_TYPE_FX);
        READ_MEMBER_ASSET(weapon->weapDef, viewLastShotEjectEffect, native::FxEffectDef, native::XAssetType::ASSET_TYPE_FX);
        READ_MEMBER_ASSET(weapon->weapDef, worldLastShotEjectEffect, native::FxEffectDef, native::XAssetType::ASSET_TYPE_FX);

        READ_MEMBER_MATERIAL(weapon->weapDef, reticleCenter);
        READ_MEMBER_MATERIAL(weapon->weapDef, reticleSide);

        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReticleCenterSize);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReticleSideSize);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReticleMinOfs);

        READ_MEMBER_NAMED_ENUM(weapon->weapDef, activeReticleType, iw4of::native::activeReticleType_t);

        READ_MEMBER_ARRAY(weapon->weapDef, vStandMove, float);
        READ_MEMBER_ARRAY(weapon->weapDef, vStandRot, float);
        READ_MEMBER_ARRAY(weapon->weapDef, strafeMove, float);
        READ_MEMBER_ARRAY(weapon->weapDef, strafeRot, float);
        READ_MEMBER_ARRAY(weapon->weapDef, vDuckedOfs, float);
        READ_MEMBER_ARRAY(weapon->weapDef, vDuckedMove, float);
        READ_MEMBER_ARRAY(weapon->weapDef, vDuckedRot, float);
        READ_MEMBER_ARRAY(weapon->weapDef, vProneOfs, float);
        READ_MEMBER_ARRAY(weapon->weapDef, vProneMove, float);
        READ_MEMBER_ARRAY(weapon->weapDef, vProneRot, float);

        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fPosMoveRate);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fPosProneMoveRate);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fStandMoveMinSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fDuckedMoveMinSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fProneMoveMinSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fPosRotRate);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fPosProneRotRate);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fStandRotMinSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fDuckedRotMinSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fProneRotMinSpeed);

        this->read_xmodels_array("worldModel", &weapon_def->worldModel, json_variant);

        READ_MEMBER_ASSET(weapon->weapDef, worldClipModel, native::XModel, native::ASSET_TYPE_XMODEL);
        READ_MEMBER_ASSET(weapon->weapDef, rocketModel, native::XModel, native::ASSET_TYPE_XMODEL);
        READ_MEMBER_ASSET(weapon->weapDef, knifeModel, native::XModel, native::ASSET_TYPE_XMODEL);
        READ_MEMBER_ASSET(weapon->weapDef, worldKnifeModel, native::XModel, native::ASSET_TYPE_XMODEL);
        READ_MEMBER_MATERIAL(weapon->weapDef, hudIcon);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, hudIconRatio, iw4of::native::weaponIconRatioType_t);
        READ_MEMBER_MATERIAL(weapon->weapDef, pickupIcon);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, pickupIconRatio, iw4of::native::weaponIconRatioType_t);
        READ_MEMBER_MATERIAL(weapon->weapDef, ammoCounterIcon);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, ammoCounterIconRatio, iw4of::native::weaponIconRatioType_t);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, ammoCounterClip, iw4of::native::ammoCounterClipType_t);

        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iStartAmmo);
        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szAmmoName);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iAmmoIndex);
        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szClipName);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iClipIndex);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iMaxAmmo);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, shotCount);
        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szSharedAmmoCapName);

        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iSharedAmmoCapIndex);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iSharedAmmoCap);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, damage);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, playerDamage);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iMeleeDamage);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iDamageType);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iFireDelay);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iMeleeDelay);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, meleeChargeDelay);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iDetonateDelay);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iRechamberTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, rechamberTimeOneHanded);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iRechamberBoltTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iHoldFireTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iDetonateTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iMeleeTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, meleeChargeTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, reloadShowRocketTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadEmptyTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadAddTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadStartTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadStartAddTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadEndTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iDropTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iRaiseTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iAltDropTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, quickDropTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, quickRaiseTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iBreachRaiseTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iEmptyRaiseTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iEmptyDropTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, sprintInTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, sprintLoopTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, sprintOutTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, stunnedTimeBegin);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, stunnedTimeLoop);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, stunnedTimeEnd);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionWearTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionWearTimeFadeOutEnd);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionWearTimePowerUp);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionRemoveTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionRemoveTimePowerDown);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, nightVisionRemoveTimeFadeInStart);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, fuseTime);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, aiFuseTime);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, autoAimRange);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, aimAssistRange);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, aimAssistRangeAds);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, aimPadding);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, enemyCrosshairRange);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, moveSpeedScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsMoveSpeedScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, sprintDurationScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsZoomInFrac);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsZoomOutFrac);

        READ_MEMBER_MATERIAL(weapon->weapDef, overlayMaterial);
        READ_MEMBER_MATERIAL(weapon->weapDef, overlayMaterialLowRes);
        READ_MEMBER_MATERIAL(weapon->weapDef, overlayMaterialEMP);
        READ_MEMBER_MATERIAL(weapon->weapDef, overlayMaterialEMPLowRes);

        READ_MEMBER_NAMED_ENUM(weapon->weapDef, overlayReticle, iw4of::native::weapOverlayReticle_t);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, overlayInterface, iw4of::native::WeapOverlayInteface_t);

        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, overlayWidth);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, overlayHeight);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, overlayWidthSplitscreen);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, overlayHeightSplitscreen);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsBobFactor);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewBobMult);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadStandMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadDuckedMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadProneMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, hipSpreadStandMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, hipSpreadDuckedMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, hipSpreadProneMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadDecayRate);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadFireAdd);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadTurnAdd);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadMoveAdd);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadDuckedDecay);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipSpreadProneDecay);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipReticleSidePos);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsIdleAmount);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipIdleAmount);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsIdleSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, hipIdleSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fIdleCrouchFactor);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fIdleProneFactor);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fGunMaxPitch);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fGunMaxYaw);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, swayMaxAngle);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, swayLerpSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, swayPitchScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, swayYawScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, swayHorizScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, swayVertScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, swayShellShockScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayMaxAngle);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayLerpSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayPitchScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayYawScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayHorizScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsSwayVertScale);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsViewErrorMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsViewErrorMax);

        // READ_MEMBER_ASSET(weapon->weapDef, physCollmap, native::ASSET_TYPE_PHYSCOLLMAP);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, dualWieldViewModelOffset);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, killIconRatio, native::weaponIconRatioType_t);

        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadAmmoAdd);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iReloadStartAdd);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, ammoDropStockMin);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, ammoDropClipPercentMin);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, ammoDropClipPercentMax);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iExplosionRadius);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iExplosionRadiusMin);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iExplosionInnerDamage);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iExplosionOuterDamage);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, damageConeAngle);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, bulletExplDmgMult);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, bulletExplRadiusMult);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iProjectileSpeed);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iProjectileSpeedUp);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iProjectileSpeedForward);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iProjectileActivateDist);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, projLifetime);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, timeToAccelerate);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, projectileCurvature);

        READ_MEMBER_ASSET(weapon->weapDef, projectileModel, native::XModel, native::ASSET_TYPE_XMODEL);

        READ_MEMBER_NAMED_ENUM(weapon->weapDef, projExplosion, native::weapProjExposion_t);
        READ_MEMBER_ASSET(weapon->weapDef, projExplosionEffect, native::FxEffectDef, native::ASSET_TYPE_FX);
        READ_MEMBER_ASSET(weapon->weapDef, projDudEffect, native::FxEffectDef, native::ASSET_TYPE_FX);
        READ_MEMBER_SOUND(weapon->weapDef, projExplosionSound);
        READ_MEMBER_SOUND(weapon->weapDef, projDudSound);

        READ_MEMBER_NAMED_ENUM(weapon->weapDef, stickiness, native::WeapStickinessType);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, lowAmmoWarningThreshold);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, ricochetChance);

        // Bounce
        if (json_variant.HasMember("parallelBounce"))
        {
            weapon->weapDef->parallelBounce = local_allocator.allocate_array<float>(31);
            const auto arr = json_variant["parallelBounce"].GetArray();
            for (int i = 0; i < std::min(static_cast<int>(arr.Size()), 31); i++)
            {
                weapon->weapDef->parallelBounce[i] = arr[i].Get<float>();
            }
        }

        if (json_variant.HasMember("perpendicularBounce"))
        {
            weapon->weapDef->parallelBounce = local_allocator.allocate_array<float>(31);
            const auto arr = json_variant["perpendicularBounce"].GetArray();
            for (int i = 0; i < std::min(static_cast<int>(arr.Size()), 31); i++)
            {
                weapon->weapDef->parallelBounce[i] = arr[i].Get<float>();
            }
        }

        READ_MEMBER_ASSET(weapon->weapDef, projTrailEffect, native::FxEffectDef, native::ASSET_TYPE_FX);
        READ_MEMBER_ASSET(weapon->weapDef, projBeaconEffect, native::FxEffectDef, native::ASSET_TYPE_FX);

        READ_MEMBER_ARRAY(weapon->weapDef, vProjectileColor, float);
        READ_MEMBER_NAMED_ENUM(weapon->weapDef, guidedMissileType, native::guidedMissileType_t);

        READ_MEMBER_IF_NOT_NULL(weapon->weapDef, maxSteeringAccel, float);
        READ_MEMBER_IF_NOT_NULL(weapon->weapDef, projIgnitionDelay, int);
        READ_MEMBER_ASSET(weapon->weapDef, projIgnitionEffect, native::FxEffectDef, native::ASSET_TYPE_FX);
        READ_MEMBER_SOUND(weapon->weapDef, projIgnitionSound);

        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsAimPitch);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsCrosshairInFrac);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsCrosshairOutFrac);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsGunKickReducedKickBullets);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, adsGunKickReducedKickPercent);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickPitchMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickPitchMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickYawMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickYawMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickAccel);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickSpeedMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickSpeedDecay);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsGunKickStaticDecay);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewKickPitchMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewKickPitchMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewKickYawMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewKickYawMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewScatterMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsViewScatterMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAdsSpread);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, hipGunKickReducedKickBullets);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, hipGunKickReducedKickPercent);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickPitchMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickPitchMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickYawMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickYawMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickAccel);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickSpeedMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickSpeedDecay);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipGunKickStaticDecay);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewKickPitchMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewKickPitchMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewKickYawMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewKickYawMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewScatterMin);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fHipViewScatterMax);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fightDist);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, maxDist);

        // not ported for now because they're a bit annoying and not very useful for PVP, but maybe one day
        weapon->weapDef->accuracyGraphName[0] = local_allocator.allocate_array<char>(1);
        weapon->weapDef->accuracyGraphName[1] = local_allocator.allocate_array<char>(1);
        weapon->weapDef->originalAccuracyGraphKnotCount[0] = 0;
        weapon->weapDef->originalAccuracyGraphKnotCount[1] = 0;
        /*
                const char* accuracyGraphName[2];
                        float (*originalAccuracyGraphKnots[2])[2];
                        unsigned __int16 originalAccuracyGraphKnotCount[2];
        */

        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iPositionReloadTransTime);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, leftArc);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, rightArc);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, topArc);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, bottomArc);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, accuracy);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, aiSpread);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, playerSpread);

        READ_MEMBER_ARRAY(weapon->weapDef, minTurnSpeed, float);
        READ_MEMBER_ARRAY(weapon->weapDef, maxTurnSpeed, float);

        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, pitchConvergenceTime);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, yawConvergenceTime);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, suppressTime);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, maxRange);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fAnimHorRotateInc);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fPlayerPositionDist);

        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szUseHintString);
        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, dropHintString);

        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, iUseHintStringIndex);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, dropHintStringIndex);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, horizViewJitter);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, vertViewJitter);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, scanSpeed);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, scanAccel);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, scanPauseTime);

        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, szScript);
        READ_MEMBER_ARRAY(weapon->weapDef, fOOPosAnimLength, float);

        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, minDamage);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, minPlayerDamage);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fMaxDamageRange);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, fMinDamageRange);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, destabilizationRateTime);
        READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, destabilizationCurvatureMax);
        READ_INT_MEMBER_IF_NOT_NULL(weapon->weapDef, destabilizeDistance);

        READ_MEMBER_ARRAY_WITH_COUNT(weapon->weapDef, locationDamageMultipliers, 20, float);
        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, fireRumble);
        READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, meleeImpactRumble);

        READ_MEMBER_ASSET(weapon->weapDef, tracerType, native::TracerDef, native::ASSET_TYPE_TRACER);

        if (weapon_def->weapClass == native::WEAPCLASS_TURRET)
        {
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, turretScopeZoomRate);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, turretScopeZoomMin);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, turretScopeZoomMax);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, turretOverheatUpRate);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, turretOverheatDownRate);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, turretOverheatPenalty);

            READ_MEMBER_SOUND(weapon->weapDef, turretOverheatSound);
            READ_MEMBER_ASSET(weapon->weapDef, turretOverheatEffect, native::FxEffectDef, native::ASSET_TYPE_FX);
            READ_STR_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinRumble);

            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinSpeed);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinUpTime);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinDownTime);

            READ_MEMBER_SOUND(weapon->weapDef, turretBarrelSpinMaxSnd);

            read_sounds_array(
                "turretBarrelSpinUpSnd", reinterpret_cast<native::snd_alias_list_t***>(&weapon_def->turretBarrelSpinUpSnd), json_variant);
            read_sounds_array(
                "turretBarrelSpinDownSnd", reinterpret_cast<native::snd_alias_list_t***>(&weapon_def->turretBarrelSpinDownSnd), json_variant);
        }

        if (weapon_def->weapClass == native::weapClass_t::WEAPCLASS_ROCKETLAUNCHER)
        {
            READ_MEMBER_SOUND(weapon->weapDef, missileConeSoundAlias);
            READ_MEMBER_SOUND(weapon->weapDef, missileConeSoundAliasAtBase);

            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundRadiusAtTop);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundRadiusAtBase);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundHeight);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundOriginOffset);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundVolumescaleAtCore);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundVolumescaleAtEdge);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundVolumescaleCoreSize);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchAtTop);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchAtBottom);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchTopSize);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchBottomSize);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundCrossfadeTopSize);
            READ_FLOAT_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundCrossfadeBottomSize);
        }

        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, sharedAmmo);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, lockonSupported);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, requireLockonToFire);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bigExplosion);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, noAdsWhenMagEmpty);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, avoidDropCleanup);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, inheritsPerks);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, crosshairColorChange);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bRifleBullet);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, armorPiercing);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bBoltAction);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, aimDownSight);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bRechamberWhileAds);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bBulletExplosiveDamage);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bCookOffHold);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bClipOnly);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, noAmmoPickup);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, adsFireOnly);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, cancelAutoHolsterWhenEmpty);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, disableSwitchToWhenEmpty);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, suppressAmmoReserveDisplay);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, laserSightDuringNightvision);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, markableViewmodel);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, noDualWield);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, flipKillIcon);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bNoPartialReload);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bSegmentedReload);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, blocksProne);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, silenced);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, isRollingGrenade);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, projExplosionEffectForceNormalUp);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, bProjImpactExplode);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, stickToPlayers);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, hasDetonator);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, disableFiring);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, timedDetonation);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, rotate);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, holdButtonToThrow);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, freezeMovementWhenFiring);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, thermalScope);
        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, altModeSameWeapon);

        if (weapon_def->weapClass == native::weapClass_t::WEAPCLASS_TURRET)
        {
            READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, turretBarrelSpinEnabled);
        }

        if (weapon_def->weapClass == native::weapClass_t::WEAPCLASS_ROCKETLAUNCHER)
        {
            READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundEnabled);
            READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundPitchshiftEnabled);
            READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, missileConeSoundCrossfadeEnabled);
        }

        READ_BOOL_MEMBER_IF_NOT_NULL(weapon->weapDef, offhandHoldIsCancelable);

        bool success = true;

        // Read other variants
        if (json_variant.HasMember("variants"))
        {
            const auto& variants = json_variant["variants"];

            for (const auto& other_variant : variants.GetArray())
            {
                std::string variant_name = base_name + "_" + other_variant["szInternalName"].GetString() + suffix;

                native::XAssetHeader other_weapon_header{};

                success |= read_variant(other_weapon_header, other_variant, {weapon});

                if (other_weapon_header.data && success)
                {
                    assets->request_mark_asset(native::ASSET_TYPE_WEAPON, other_weapon_header.data);
                }
            }
        }

        return success;
    }

} // namespace iw4of::interfaces
