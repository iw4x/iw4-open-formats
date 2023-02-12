#include <std_include.hpp>

#include "ifx.hpp"
#include "itechniqueset.hpp"
#include "utils/io.hpp"
#include "utils/string.hpp"

#define IW4X_FX_VERSION 2

namespace iw4of::interfaces
{
	bool ifx::write_internal(const native::XAssetHeader& header) const
	{
		AssertSize(native::FxEffectDef, 32);

		utils::stream buffer;
		buffer.save_array("IW4xFx  ", 8); // No idea what to fill in
		buffer.save_object(IW4X_FX_VERSION);

		auto asset = header.fx;
		buffer.save_object(*asset);

		if (asset->name)
		{
			buffer.save_string(asset->name);
		}

		if (asset->elemDefs)
		{
			buffer.save_array(asset->elemDefs, asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot);

			for (int i = 0; i < (asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot); ++i)
			{
				native::FxElemDef* elem_def = &asset->elemDefs[i];

				if (elem_def->velSamples)
				{
					AssertSize(native::FxElemVelStateSample, 96);
					buffer.save_array(elem_def->velSamples, elem_def->velIntervalCount + 1);
				}

				if (elem_def->visSamples)
				{
					AssertSize(native::FxElemVisStateSample, 48);
					buffer.save_array(elem_def->visSamples, elem_def->visStateIntervalCount + 1);
				}

				// Save_FxElemDefVisuals
				{
					if (elem_def->elemType == native::FX_ELEM_TYPE_DECAL)
					{
						if (elem_def->visuals.markArray)
						{
							buffer.save_array(elem_def->visuals.markArray, elem_def->visualCount);

							for (char j = 0; j < elem_def->visualCount; ++j)
							{
								if (elem_def->visuals.markArray[j].materials[0])
								{
									buffer.save_string(elem_def->visuals.markArray[j].materials[0]->info.name);
								}

								if (elem_def->visuals.markArray[j].materials[1])
								{
									buffer.save_string(elem_def->visuals.markArray[j].materials[1]->info.name);
								}
							}
						}
					}
					else if (elem_def->visualCount > 1)
					{
						if (elem_def->visuals.array)
						{
							buffer.save_array(elem_def->visuals.array, elem_def->visualCount);

							for (char j = 0; j < elem_def->visualCount; ++j)
							{
								write(&elem_def->visuals.array[j], elem_def->elemType, &buffer);
							}
						}
					}
					else if (elem_def->visualCount == 1)
					{
						write(&elem_def->visuals.instance, elem_def->elemType, &buffer);
					}
				}

				if (elem_def->effectOnImpact.handle)
				{
					buffer.save_string(elem_def->effectOnImpact.name);
				}

				if (elem_def->effectOnDeath.handle)
				{
					buffer.save_string(elem_def->effectOnDeath.name);
				}

				if (elem_def->effectEmitted.handle)
				{
					buffer.save_string(elem_def->effectEmitted.name);
				}

				// Save_FxElemExtendedDefPtr
				{
					if (elem_def->elemType == native::FX_ELEM_TYPE_TRAIL)
					{
						// Save_FxTrailDef
						{
							if (elem_def->extended.trailDef)
							{
								AssertSize(native::FxTrailDef, 36);

								native::FxTrailDef* trailDef = elem_def->extended.trailDef;
								buffer.save_object(*trailDef);

								if (trailDef->verts)
								{
									AssertSize(native::FxTrailVertex, 20);

									buffer.save_array(trailDef->verts, trailDef->vertCount);
								}

								if (trailDef->inds)
								{
									buffer.save_array(trailDef->inds, trailDef->indCount);
								}
							}
						}
					}
					else if (elem_def->elemType == native::FX_ELEM_TYPE_SPARK_FOUNTAIN)
					{
						if (elem_def->extended.sparkFountainDef)
						{
							buffer.save_object(*elem_def->extended.sparkFountainDef);
						}
					}
				}
			}
		}

		return utils::io::write_file(get_work_path(header).string(), buffer.to_buffer());
	}

	void* ifx::read_internal(const std::string& name) const
	{
		const auto& path = get_work_path(name).string();

		if (utils::io::file_exists(path))
		{
			auto contents = utils::io::read_file(path);
			utils::stream::reader buffer(&local_allocator, contents);

			__int64 magic = buffer.read<__int64>();
			if (std::memcmp(&magic, "IW4xFx  ", 8))
			{
				print_error("Reading fx '{}' failed, header is invalid!", name);
				return nullptr;
			}

			int version = buffer.read<int>();
			if (version > IW4X_FX_VERSION)
			{
				print_error("Reading fx '{}' failed, expected version is {}, but it was {}!", name, IW4X_FX_VERSION, version);
				return nullptr;
			}

			native::FxEffectDef* asset = buffer.read_object<native::FxEffectDef>();

			if (asset->name)
			{
				asset->name = buffer.read_cstring();
			}

			if (asset->elemDefs)
			{
				asset->elemDefs = buffer.read_array<native::FxElemDef>(asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot);

				for (int i = 0; i < (asset->elemDefCountEmission + asset->elemDefCountLooping + asset->elemDefCountOneShot); ++i)
				{
					native::FxElemDef* elemDef = &asset->elemDefs[i];

					if (elemDef->velSamples)
					{
						elemDef->velSamples = buffer.read_array<native::FxElemVelStateSample>(elemDef->velIntervalCount + 1);
					}

					if (elemDef->visSamples)
					{
						elemDef->visSamples = buffer.read_array<native::FxElemVisStateSample>(elemDef->visStateIntervalCount + 1);
					}

					// Save_FxElemDefVisuals
					{
						if (elemDef->elemType == native::FX_ELEM_TYPE_DECAL)
						{
							if (elemDef->visuals.markArray)
							{
								elemDef->visuals.markArray = buffer.read_array<native::FxElemMarkVisuals>(elemDef->visualCount);

								for (char j = 0; j < elemDef->visualCount; ++j)
								{
									if (elemDef->visuals.markArray[j].materials[0])
									{
										elemDef->visuals.markArray[j].materials[0] = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, buffer.read_string().data());
									}

									if (elemDef->visuals.markArray[j].materials[1])
									{
										elemDef->visuals.markArray[j].materials[1] = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, buffer.read_string().data());
									}
								}
							}
						}
						else if (elemDef->visualCount > 1)
						{
							if (elemDef->visuals.array)
							{
								elemDef->visuals.array = buffer.read_array<native::FxElemVisuals>(elemDef->visualCount);

								for (char j = 0; j < elemDef->visualCount; ++j)
								{
									read(&elemDef->visuals.array[j], elemDef->elemType, &buffer);
								}
							}
						}
						else if (elemDef->visualCount == 1)
						{
							read(&elemDef->visuals.instance, elemDef->elemType, &buffer);
						}
					}

					if (elemDef->effectOnImpact.handle)
					{
						elemDef->effectOnImpact.handle = find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, buffer.read_string().data());
					}

					if (elemDef->effectOnDeath.handle)
					{
						elemDef->effectOnDeath.handle = find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, buffer.read_string().data());
					}

					if (elemDef->effectEmitted.handle)
					{
						elemDef->effectEmitted.handle = find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, buffer.read_string().data());
					}

					// Save_FxElemExtendedDefPtr
					{

						if (elemDef->elemType == native::FX_ELEM_TYPE_TRAIL)
						{
							// Save_FxTrailDef
							{
								if (elemDef->extended.trailDef)
								{
									native::FxTrailDef* trailDef = buffer.read_object<native::FxTrailDef>();
									elemDef->extended.trailDef = trailDef;

									if (trailDef->verts)
									{
										trailDef->verts = buffer.read_array<native::FxTrailVertex>(trailDef->vertCount);
									}

									if (trailDef->inds)
									{
										trailDef->inds = buffer.read_array<unsigned short>(trailDef->indCount);
									}
								}
							}
						}
						else if (version >= 2)
						{
							if (elemDef->elemType == native::FX_ELEM_TYPE_SPARK_FOUNTAIN)
							{
								if (elemDef->extended.sparkFountainDef)
								{
									elemDef->extended.sparkFountainDef = buffer.read_object<native::FxSparkFountainDef>();
								}
							}
						}
					}
				}
			}

			return asset;
		}

		return nullptr;
	}

	std::filesystem::path iw4of::interfaces::ifx::get_file_name(const std::string& basename) const
	{
		return std::format("{}.iw4xFx");
	}

	void ifx::write(const native::FxElemVisuals* visuals, char elemType, utils::stream* buffer) const
	{
		switch (elemType)
		{
		case native::FX_ELEM_TYPE_MODEL:
			if (visuals->model)
			{
				buffer->save_string(visuals->model->name);
			}

			break;

		case native::FX_ELEM_TYPE_OMNI_LIGHT:
		case native::FX_ELEM_TYPE_SPOT_LIGHT:
			break;

		case native::FX_ELEM_TYPE_SOUND:
			if (visuals->soundName)
			{
				buffer->save_string(visuals->soundName);
			}

			break;

		case native::FX_ELEM_TYPE_RUNNER:
			if (visuals->effectDef.handle)
			{
				buffer->save_string(visuals->effectDef.handle->name);
			}

			break;

		default:
			if (visuals->material)
			{
				buffer->save_string(visuals->material->info.name);
			}

			break;
		}
	}

	void interfaces::ifx::read(native::FxElemVisuals* visuals, char elemType, utils::stream::reader* reader) const
	{
		switch (elemType)
		{
		case native::FX_ELEM_TYPE_MODEL:
		{
			if (visuals->model)
			{
				visuals->model = find<native::XModel>(native::XAssetType::ASSET_TYPE_XMODEL, reader->read_string().data());
			}

			break;
		}

		case native::FX_ELEM_TYPE_OMNI_LIGHT:
		case native::FX_ELEM_TYPE_SPOT_LIGHT:
			break;

		case native::FX_ELEM_TYPE_SOUND:
		{
			if (visuals->soundName)
			{
				visuals->soundName = reader->read_cstring();
			}
			break;
		}

		case native::FX_ELEM_TYPE_RUNNER:
		{
			if (visuals->effectDef.handle)
			{
				visuals->effectDef.handle = find<native::FxEffectDef>(native::XAssetType::ASSET_TYPE_FX, reader->read_string().data());
			}

			break;
		}

		default:
		{
			if (visuals->material)
			{
				visuals->material = find<native::Material>(native::XAssetType::ASSET_TYPE_MATERIAL, reader->read_string().data());
			}

			break;
		}
		}
	}

}