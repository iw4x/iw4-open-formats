#include <std_include.hpp>

#include "ixanimparts.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#define IW4X_ANIM_VERSION 3

bool iw4of::interfaces::ixanimparts::write_internal(const native::XAssetHeader& header) const
{
    assert(header.parts);
    utils::stream buffer;
    buffer.save_array("IW4xAnim", 8);
    buffer.save_object(IW4X_ANIM_VERSION);

    auto parts = header.parts;

    buffer.save_array(parts, 1);

    if (parts->name)
    {
        buffer.save_string(parts->name);
    }

    for (int i = 0; i < parts->boneCount[native::XAnimPartType::PART_TYPE_ALL]; ++i)
    {
        buffer.save_string(assets->read_from_stringtable(parts->names[i]));
    }

    if (parts->notify)
    {
        buffer.save_array(parts->notify, parts->notifyCount);

        for (int i = 0; i < parts->notifyCount; ++i)
        {
            buffer.save_string(assets->read_from_stringtable(parts->notify[i].name));
        }
    }

    if (parts->dataByte)
    {
        buffer.save_array(parts->dataByte, parts->dataByteCount);
    }

    if (parts->dataShort)
    {
        buffer.save_array(parts->dataShort, parts->dataShortCount);
    }

    if (parts->dataInt)
    {
        buffer.save_array(parts->dataInt, parts->dataIntCount);
    }

    if (parts->randomDataByte)
    {
        buffer.save_array(parts->randomDataByte, parts->randomDataByteCount);
    }

    if (parts->randomDataShort)
    {
        buffer.save_array(parts->randomDataShort, parts->randomDataShortCount);
    }

    if (parts->randomDataInt)
    {
        buffer.save_array(parts->randomDataInt, parts->randomDataIntCount);
    }

    if (parts->indices.data)
    {
        if (parts->numframes < 256)
        {
            buffer.save_array(parts->indices._1, parts->indexCount);
        }
        else
        {
            buffer.save_array(parts->indices._2, parts->indexCount);
        }
    }

    if (parts->deltaPart)
    {
        auto delta = parts->deltaPart;
        static_assert(sizeof (native::XAnimDeltaPart) == 12);

        buffer.save_object(*delta);
        if (delta->trans)
        {
            buffer.save_object(*delta->trans);
            if (delta->trans->size)
            {
                buffer.save_object(delta->trans->u.frames);

                if (parts->numframes > 0xFF)
                {
                    buffer.save_array(delta->trans->u.frames.indices._2, delta->trans->size + 1);
                }
                else
                {
                    buffer.save_array(delta->trans->u.frames.indices._1, delta->trans->size + 1);
                }

                if (delta->trans->u.frames.frames._1)
                {
                    if (delta->trans->smallTrans)
                    {
                        buffer.save(delta->trans->u.frames.frames._1, 3, delta->trans->size + 1);
                    }
                    else
                    {
                        buffer.save(delta->trans->u.frames.frames._2, 6, delta->trans->size + 1);
                    }
                }
            }
            else
            {
                buffer.save_array(delta->trans->u.frame0, 3);
            }
        }

        if (delta->quat2)
        {
            buffer.save_object(*delta->quat2);

            if (delta->quat2->size)
            {
                buffer.save_object(delta->quat2->u.frames);

                if (parts->numframes > 0xFF)
                {
                    buffer.save(delta->quat2->u.frames.indices._2, 2, delta->quat2->size + 1);
                }
                else
                {
                    buffer.save(delta->quat2->u.frames.indices._1, 1, delta->quat2->size + 1);
                }

                if (delta->quat2->u.frames.frames)
                {
                    buffer.save(delta->quat2->u.frames.frames, 4, delta->quat2->size + 1);
                }
            }
            else
            {
                buffer.save_object(delta->quat2->u.frame0);
            }
        }

        if (delta->quat)
        {
            buffer.save_object(*delta->quat);

            if (delta->quat->size)
            {
                buffer.save_object(&delta->quat->u.frames);

                if (parts->numframes > 0xFF)
                {
                    buffer.save(delta->quat->u.frames.indices._2, 2, delta->quat->size + 1);
                }
                else
                {
                    buffer.save(delta->quat->u.frames.indices._1, 1, delta->quat->size + 1);
                }

                if (delta->quat->u.frames.frames)
                {
                    buffer.save(delta->quat->u.frames.frames, 4, delta->quat->size + 1);
                }
            }
            else
            {
                buffer.save_object(delta->quat->u.frame0);
            }
        }
    }

    return utils::io::write_file(get_work_path(header).string(), buffer.to_buffer());
}

void* iw4of::interfaces::ixanimparts::read_internal(const std::string& name) const
{
    const auto& path = get_work_path(name).string();

    if (utils::io::file_exists(path))
    {
        auto contents = utils::io::read_file(path);
        utils::stream::reader reader(&local_allocator, contents);

        int64_t magic = reader.read<int64_t>();
        if (std::memcmp(&magic, "IW4xAnim", 8))
        {
            print_error("Reading animation '{}' failed, header is invalid!", name);
            return nullptr;
        }

        int32_t version = reader.read<int32_t>();
        if (version > IW4X_ANIM_VERSION)
        {
            print_error("Reading animation '{}' failed, expected version is {}, but it was {}!", name, IW4X_ANIM_VERSION, version);
            return nullptr;
        }

        native::XAnimParts* xanim = reader.read_array<native::XAnimParts>();

        if (xanim)
        {
            if (xanim->name)
            {
                xanim->name = reader.read_cstring();
            }

            if (xanim->names)
            {
                xanim->names = local_allocator.allocate_array<uint16_t>(xanim->boneCount[native::PART_TYPE_ALL]);
                for (int i = 0; i < xanim->boneCount[native::PART_TYPE_ALL]; ++i)
                {
                    xanim->names[i] = static_cast<std::uint16_t>(assets->write_in_stringtable(reader.read_cstring()));
                }
            }

            if (xanim->notify)
            {
                xanim->notify = reader.read_array<native::XAnimNotifyInfo>(xanim->notifyCount);

                for (int i = 0; i < xanim->notifyCount; ++i)
                {
                    xanim->notify[i].name = static_cast<std::uint16_t>(assets->write_in_stringtable(reader.read_cstring()));
                }
            }

            if (xanim->dataByte)
            {
                xanim->dataByte = reader.read_array<char>(xanim->dataByteCount);
            }

            if (xanim->dataShort)
            {
                xanim->dataShort = reader.read_array<uint16_t>(xanim->dataShortCount);
            }

            if (xanim->dataInt)
            {
                xanim->dataInt = reader.read_array<int32_t>(xanim->dataIntCount);
            }

            if (xanim->randomDataByte)
            {
                xanim->randomDataByte = reader.read_array<char>(xanim->randomDataByteCount);
            }

            if (xanim->randomDataShort)
            {
                xanim->randomDataShort = reader.read_array<uint16_t>(xanim->randomDataShortCount);
            }

            if (xanim->randomDataInt)
            {
                xanim->randomDataInt = reader.read_array<int32_t>(xanim->randomDataIntCount);
            }

            if (xanim->indices.data)
            {
                if (xanim->numframes < 256)
                {
                    xanim->indices._1 = reader.read_array<char>(xanim->indexCount);
                }
                else
                {
                    xanim->indices._2 = reader.read_array<uint16_t>(xanim->indexCount);
                }
            }

            if (version > 1)
            {
                if (xanim->deltaPart)
                {
                    xanim->deltaPart = reader.read_object<native::XAnimDeltaPart>();
                    auto delta = xanim->deltaPart;
                    if (delta->trans)
                    {
                        delta->trans = reader.read_object<native::XAnimPartTrans>();
                        if (delta->trans->size)
                        {
                            delta->trans->u.frames = reader.read<native::XAnimPartTransFrames>();

                            if (xanim->numframes > 0xFF)
                            {
                                auto indices2 = reader.read_array<uint16_t>(delta->trans->size + 1);
                                memcpy(delta->trans->u.frames.indices._2, indices2, sizeof(short) * (delta->trans->size + 1));
                            }
                            else
                            {
                                auto indices1 = reader.read_array<char>(delta->trans->size + 1);
                                memcpy(delta->trans->u.frames.indices._1, indices1, delta->trans->size + 1);
                            }

                            if (delta->trans->u.frames.frames._1)
                            {
                                if (delta->trans->smallTrans)
                                {
                                    delta->trans->u.frames.frames._1 = reinterpret_cast<char(*)[3]>(3, (delta->trans->size + 1));
                                }
                                else
                                {
                                    delta->trans->u.frames.frames._2 = reinterpret_cast<uint16_t(*)[3]>(6, (delta->trans->size + 1));
                                }
                            }
                        }
                        else
                        {
                            auto frames = reader.read_object<native::vec3_t>();
                            memcpy(delta->trans->u.frame0, frames, 3 * sizeof(float));
                        }
                    }

                    if (version > 2)
                    {
                        if (delta->quat2)
                        {
                            delta->quat2 =  reader.read_object<iw4of::native::XAnimDeltaPartQuat2>();

                            if (delta->quat2->size)
                            {
                                delta->quat2->u.frames = reader.read<iw4of::native::XAnimDeltaPartQuatDataFrames2>();

                                if (xanim->numframes > 0xFF)
                                {
									auto indices2 = reader.read_array<uint16_t>(delta->quat2->size + 1);
									memcpy(delta->quat2->u.frames.indices._2, indices2, sizeof(uint16_t) * (delta->quat2->size + 1));
                                }
                                else
                                {
									auto indices1 = reader.read_array<uint8_t>(delta->quat2->size + 1);
									memcpy(delta->quat2->u.frames.indices._1, indices1, sizeof(uint8_t) * (delta->quat2->size + 1));
                                }

                                if (delta->quat2->u.frames.frames)
                                {
									// Pairs of uint16_t
									auto frames = reader.read_array<uint16_t>(2 * (delta->quat2->size + 1));
									memcpy(delta->quat2->u.frames.frames, frames, sizeof(uint16_t) * 2 *  (delta->quat2->size + 1));
                                }
                            }
                            else
                            {
								auto frames = reader.read_array<uint16_t>(2);
								memcpy(delta->quat2->u.frame0, frames, 2 * sizeof(uint16_t));
                            }
                        }

                        if (delta->quat)
                        {
                            delta->quat =  reader.read_object<iw4of::native::XAnimDeltaPartQuat>();

                            if (delta->quat->size)
                            {
                                delta->quat->u.frames = reader.read<iw4of::native::XAnimDeltaPartQuatDataFrames>();

                                if (xanim->numframes > 0xFF)
                                {
									auto indices2 = reader.read_array<uint16_t>(delta->quat->size + 1);
									memcpy(delta->quat->u.frames.indices._2, indices2, sizeof(uint16_t) * (delta->quat->size + 1));
                                }
                                else
                                {
									auto indices1 = reader.read_array<uint8_t>(delta->quat->size + 1);
									memcpy(delta->quat->u.frames.indices._1, indices1, sizeof(uint8_t) * (delta->quat->size + 1));
                                }

                                if (delta->quat->u.frames.frames)
                                {
									// Pairs of uint16_t
									auto frames = reader.read_array<uint16_t>(4 * (delta->quat->size + 1));
									memcpy(delta->quat->u.frames.frames, frames, sizeof(uint16_t) * 4 *  (delta->quat->size + 1));
                                }
                            }
                            else
                            {
								auto frames = reader.read_array<uint16_t>(4);
								memcpy(delta->quat->u.frame0, frames, 4 * sizeof(uint16_t));
                            }
                        }
                    }
					else
					{
						delta->quat = nullptr;
						delta->quat2 = nullptr;
					}
                }
            }

            if (!reader.end())
            {
                print_error("Reading animation '{}' failed, remaining raw data found!", name);
                return nullptr;
            }

            return xanim;
        }

        return nullptr;
    }

    return nullptr;
}

std::filesystem::path iw4of::interfaces::ixanimparts::get_file_name(const std::string& basename) const
{
    return std::format("{}.iw4xAnim", basename);
}
