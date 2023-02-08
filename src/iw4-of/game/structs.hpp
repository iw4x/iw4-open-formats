#pragma once

namespace iw4of::native
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	enum XAssetType
	{
		ASSET_TYPE_PHYSPRESET = 0x0,
		ASSET_TYPE_PHYSCOLLMAP = 0x1,
		ASSET_TYPE_XANIMPARTS = 0x2,
		ASSET_TYPE_XMODEL_SURFS = 0x3,
		ASSET_TYPE_XMODEL = 0x4,
		ASSET_TYPE_MATERIAL = 0x5,
		ASSET_TYPE_PIXELSHADER = 0x6,
		ASSET_TYPE_VERTEXSHADER = 0x7,
		ASSET_TYPE_VERTEXDECL = 0x8,
		ASSET_TYPE_TECHNIQUE_SET = 0x9,
		ASSET_TYPE_IMAGE = 0xA,
		ASSET_TYPE_SOUND = 0xB,
		ASSET_TYPE_SOUND_CURVE = 0xC,
		ASSET_TYPE_LOADED_SOUND = 0xD,
		ASSET_TYPE_CLIPMAP_SP = 0xE,
		ASSET_TYPE_CLIPMAP_MP = 0xF,
		ASSET_TYPE_COMWORLD = 0x10,
		ASSET_TYPE_GAMEWORLD_SP = 0x11,
		ASSET_TYPE_GAMEWORLD_MP = 0x12,
		ASSET_TYPE_MAP_ENTS = 0x13,
		ASSET_TYPE_FXWORLD = 0x14,
		ASSET_TYPE_GFXWORLD = 0x15,
		ASSET_TYPE_LIGHT_DEF = 0x16,
		ASSET_TYPE_UI_MAP = 0x17,
		ASSET_TYPE_FONT = 0x18,
		ASSET_TYPE_MENULIST = 0x19,
		ASSET_TYPE_MENU = 0x1A,
		ASSET_TYPE_LOCALIZE_ENTRY = 0x1B,
		ASSET_TYPE_WEAPON = 0x1C,
		ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1D,
		ASSET_TYPE_FX = 0x1E,
		ASSET_TYPE_IMPACT_FX = 0x1F,
		ASSET_TYPE_AITYPE = 0x20,
		ASSET_TYPE_MPTYPE = 0x21,
		ASSET_TYPE_CHARACTER = 0x22,
		ASSET_TYPE_XMODELALIAS = 0x23,
		ASSET_TYPE_RAWFILE = 0x24,
		ASSET_TYPE_STRINGTABLE = 0x25,
		ASSET_TYPE_LEADERBOARD = 0x26,
		ASSET_TYPE_STRUCTURED_DATA_DEF = 0x27,
		ASSET_TYPE_TRACER = 0x28,
		ASSET_TYPE_VEHICLE = 0x29,
		ASSET_TYPE_ADDON_MAP_ENTS = 0x2A,
		ASSET_TYPE_COUNT = 0x2B,
		ASSET_TYPE_STRING = 0x2B,
		ASSET_TYPE_ASSETLIST = 0x2C,
		ASSET_TYPE_INVALID = -1,
	};


	enum StaticModelFlag : char
	{
		STATIC_MODEL_FLAG_SUB_INDEX_MASK = 0x7,
		STATIC_MODEL_FLAG_NO_CAST_SHADOW = 0x10,
		STATIC_MODEL_FLAG_GROUND_LIGHTING = 0x20,
	};

	union SoundAliasFlags
	{
#pragma warning(push)
#pragma warning(disable: 4201)
		struct
		{
			unsigned int looping : 1;
			unsigned int isMaster : 1;
			unsigned int isSlave : 1;
			unsigned int fullDryLevel : 1;
			unsigned int noWetLevel : 1;
			unsigned int unknown : 1;
			unsigned int unk_is3D : 1;
			unsigned int type : 2;
			unsigned int channel : 6;
		}; 
#pragma warning(pop)
		unsigned int intValue;
	};

	static_assert(sizeof(SoundAliasFlags) == sizeof(unsigned int));

	enum SndChannel
	{
		SND_CHANNEL_PHYSICS,
		SND_CHANNEL_AMBDIST1,
		SND_CHANNEL_AMBDIST2,
		SND_CHANNEL_AUTO,
		SND_CHANNEL_AUTO2,
		SND_CHANNEL_AUTODOG,
		SND_CHANNEL_BULLETIMPACT,
		SND_CHANNEL_BULLETWHIZBY,
		SND_CHANNEL_EXPLOSIVEIMPACT,
		SND_CHANNEL_ELEMENT,
		SND_CHANNEL_AUTO2D,
		SND_CHANNEL_VEHICLE,
		SND_CHANNEL_VEHICLELIMITED,
		SND_CHANNEL_MENU,
		SND_CHANNEL_BODY,
		SND_CHANNEL_BODY2D,
		SND_CHANNEL_RELOAD,
		SND_CHANNEL_RELOAD2D,
		SND_CHANNEL_ITEM,
		SND_CHANNEL_EFFECTS1,
		SND_CHANNEL_EFFECTS2,
		SND_CHANNEL_WEAPON,
		SND_CHANNEL_WEAPON2D,
		SND_CHANNEL_NONSHOCK,
		SND_CHANNEL_VOICE,
		SND_CHANNEL_LOCAL,
		SND_CHANNEL_LOCAL2,
		SND_CHANNEL_LOCAL3,
		SND_CHANNEL_AMBIENT,
		SND_CHANNEL_HURT,
		SND_CHANNEL_PLAYER1,
		SND_CHANNEL_PLAYER2,
		SND_CHANNEL_MUSIC,
		SND_CHANNEL_MUSICNOPAUSE,
		SND_CHANNEL_MISSION,
		SND_CHANNEL_ANNOUNCER,
		SND_CHANNEL_SHELLSHOCK,

		SND_CHANNEL_COUNT
	};

	enum FogTypes
	{
		FOG_NORMAL = 0x1,
		FOG_DFOG = 0x2,
	};

	enum GfxRenderTargetId
	{
		R_RENDERTARGET_SAVED_SCREEN = 0x0,
		R_RENDERTARGET_FRAME_BUFFER = 0x1,
		R_RENDERTARGET_SCENE = 0x2,
		R_RENDERTARGET_RESOLVED_POST_SUN = 0x3,
		R_RENDERTARGET_RESOLVED_SCENE = 0x4,
		R_RENDERTARGET_FLOAT_Z = 0x5,
		R_RENDERTARGET_PINGPONG_0 = 0x6,
		R_RENDERTARGET_PINGPONG_1 = 0x7,
		R_RENDERTARGET_POST_EFFECT_0 = 0x8,
		R_RENDERTARGET_POST_EFFECT_1 = 0x9,
		R_RENDERTARGET_SHADOWMAP_LARGE = 0xA,
		R_RENDERTARGET_SHADOWMAP_SMALL = 0xB,
		R_RENDERTARGET_COUNT = 0xC,
		R_RENDERTARGET_NONE = 0xD,
	};

	enum snd_alias_type_t : char
	{
		SAT_UNKNOWN = 0x0,
		SAT_LOADED = 0x1,
		SAT_STREAMED = 0x2,
		SAT_VOICED = 0x3,
		SAT_COUNT,
	};

	struct ComPrimaryLight
	{
		unsigned char type;
		unsigned char canUseShadowMap;
		unsigned char exponent;
		unsigned char unused;
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		float cosHalfFovExpanded;
		float rotationLimit;
		float translationLimit;
		const char* defName;
	};

	struct ComWorld
	{
		const char* name;
		int isInUse;
		unsigned int primaryLightCount;
		ComPrimaryLight* primaryLights;
	};

	struct GfxImageFileHeader
	{
		char tag[3];
		char version;
		int flags;
		char format;
		short dimensions[3];
		int fileSizeForPicmip[4];
	};

	struct TriggerModel
	{
		int contents;
		unsigned __int16 hullCount;
		unsigned __int16 firstHull;
	};

	struct Bounds
	{
		float midPoint[3];
		float halfSize[3];
	};

	struct TriggerHull
	{
		Bounds bounds;
		int contents;
		unsigned __int16 slabCount;
		unsigned __int16 firstSlab;
	};

	struct TriggerSlab
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct MapTriggers
	{
		unsigned int count;
		TriggerModel* models;
		unsigned int hullCount;
		TriggerHull* hulls;
		unsigned int slabCount;
		TriggerSlab* slabs;
	};

	struct XModelTagPos
	{
		float x;
		float y;
		float z;
	};

	struct srfTriangles_t
	{
		unsigned int vertexLayerData;
		unsigned int firstVertex;
		unsigned short vertexCount;
		unsigned short triCount;
		unsigned int baseIndex;
	};

	struct GfxSurfaceLightingAndFlagsFields
	{
		unsigned char lightmapIndex;
		unsigned char reflectionProbeIndex;
		unsigned char primaryLightIndex;
		unsigned char flags;
	};

	union GfxSurfaceLightingAndFlags
	{
		GfxSurfaceLightingAndFlagsFields fields;
		unsigned int packed;
	};

	struct GfxSurface
	{
		srfTriangles_t tris;
		struct Material* material;
		GfxSurfaceLightingAndFlags laf;
	};

	struct GfxPortalWritable
	{
		bool isQueued;
		bool isAncestor;
		unsigned char recursionDepth;
		unsigned char hullPointCount;
		float(*hullPoints)[2];
		struct GfxPortal* queuedParent;
	};

	struct DpvsPlane
	{
		float coeffs[4];
	};

	struct GfxPortal
	{
		GfxPortalWritable writable;
		DpvsPlane plane;
		vec3_t* vertices;
		unsigned short cellIndex;
		unsigned char vertexCount;
		float hullAxis[2][3];
	};

	struct GfxCell
	{
		Bounds bounds;
		int portalCount;
		GfxPortal* portals;
		unsigned char reflectionProbeCount;
		unsigned char* reflectionProbes;
	};


	union PackedUnitVec
	{
		unsigned int packed;
		char array[4];
	};

	union GfxColor
	{
		unsigned int packed;
		unsigned char array[4];
	};

	struct GfxPackedPlacement
	{
		float origin[3];
		float axis[3][3];
		float scale;
	};

	struct GfxWorldVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		float texCoord[2];
		float lmapCoord[2];
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

#pragma pack(push, 4)
	struct GfxStaticModelDrawInst
	{
		GfxPackedPlacement placement;
		struct XModel* model;
		unsigned __int16 cullDist;
		unsigned __int16 lightingHandle;
		unsigned char reflectionProbeIndex;
		unsigned char primaryLightIndex;
		unsigned char flags;
		unsigned char firstMtlSkinIndex;
		GfxColor groundLighting;
		unsigned __int16 cacheId[4];
	};


	struct SunLightParseParams
	{
		char name[64];
		float ambientScale;
		float ambientColor[3];
		float diffuseFraction;
		float sunLight;
		float sunColor[3];
		float diffuseColor[3];
		bool diffuseColorHasBeenSet;
		float angles[3];
	};

	struct Picmip
	{
		char platform[2];
	};

	struct CardMemory
	{
		int platform[2];
	};

	struct GfxImageLoadDef
	{
		char levelCount;
		char pad[3];
		int flags;
		int format;
		int resourceSize;
		char data[1];
	};

	union GfxTexture
	{
		// IDirect3DBaseTexture9* basemap;
		// IDirect3DTexture9* map;
		// IDirect3DVolumeTexture9* volmap;
		// IDirect3DCubeTexture9* cubemap;
		GfxImageLoadDef* loadDef;
	};

	enum GfxImageCategory : unsigned char
	{
		IMG_CATEGORY_UNKNOWN = 0x0,
		IMG_CATEGORY_AUTO_GENERATED = 0x1,
		IMG_CATEGORY_LIGHTMAP = 0x2,
		IMG_CATEGORY_LOAD_FROM_FILE = 0x3,
		IMG_CATEGORY_RAW = 0x4,
		IMG_CATEGORY_WATER = 0x5,
		IMG_CATEGORY_RENDERTARGET = 0x6,
		IMG_CATEGORY_TEMP = 0x7,
	};

	struct GfxImage
	{
		GfxTexture texture;
		unsigned char mapType;
		GfxImageCategory semantic;
		unsigned char category;
		bool useSrgbReads;
		Picmip picmip;
		bool noPicmip;
		char track;
		CardMemory cardMemory;
		unsigned short width;
		unsigned short height;
		unsigned short depth;
		bool delayLoadPixels;
		const char* name;
	};

	struct GfxLightmapArray
	{
		GfxImage* primary;
		GfxImage* secondary;
	};

	struct GfxLightImage
	{
		GfxImage* image;
		unsigned char samplerState;
	};

	struct GfxLightDef
	{
		const char* name;
		GfxLightImage attenuation;
		int lmapLookupStart;
	};

	struct GfxReflectionProbe
	{
		float origin[3];
	};

	struct GfxReflectionProbeReferenceOrigin
	{
		float origin[3];
	};

	struct GfxReflectionProbeReference
	{
		unsigned char index;
	};

	struct GfxWorldVertexData
	{
		GfxWorldVertex* vertices;
		void* worldVb;
	};

	struct GfxWorldVertexLayerData
	{
		unsigned char* data;
		void* layerVb;
	};

	struct GfxWorldDraw
	{
		unsigned int reflectionProbeCount;
		GfxImage** reflectionProbes;
		GfxReflectionProbe* reflectionProbeOrigins;
		GfxTexture* reflectionProbeTextures;
		int lightmapCount;
		GfxLightmapArray* lightmaps;
		GfxTexture* lightmapPrimaryTextures;
		GfxTexture* lightmapSecondaryTextures;
		GfxImage* lightmapOverridePrimary;
		GfxImage* lightmapOverrideSecondary;
		unsigned int vertexCount;
		GfxWorldVertexData vd;
		unsigned int vertexLayerDataSize;
		GfxWorldVertexLayerData vld;
		unsigned int indexCount;
		unsigned __int16* indices;
	};


	struct GfxSky
	{
		int skySurfCount;
		int* skyStartSurfs;
		GfxImage* skyImage;
		int skySamplerState;
	};


	struct GfxHeroOnlyLight
	{
		char type;
		char unused[3];
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		int exponent;
	};

	struct GfxStaticModelInst
	{
		Bounds bounds;
		float lightingOrigin[3];
	};

	struct GfxSurfaceBounds
	{
		Bounds bounds;
	};

	struct GfxDrawSurfFields
	{
		unsigned long long unused : 1;
		unsigned long long primarySortKey : 6;
		unsigned long long surfType : 4;
		unsigned long long viewModelRender : 1;
		unsigned long long sceneLightIndex : 8;
		unsigned long long useHeroLighting : 1;
		unsigned long long prepass : 2;
		unsigned long long materialSortedIndex : 12;
		unsigned long long customIndex : 5;
		unsigned long long hasGfxEntIndex : 1;
		unsigned long long reflectionProbeIndex : 8;
		unsigned long long objectId : 15;
	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		unsigned long long packed;
	};

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount;
		unsigned int staticSurfaceCount;
		unsigned int staticSurfaceCountNoDecal;
		unsigned int litOpaqueSurfsBegin;
		unsigned int litOpaqueSurfsEnd;
		unsigned int litTransSurfsBegin;
		unsigned int litTransSurfsEnd;
		unsigned int shadowCasterSurfsBegin;
		unsigned int shadowCasterSurfsEnd;
		unsigned int emissiveSurfsBegin;
		unsigned int emissiveSurfsEnd;
		unsigned int smodelVisDataCount;
		unsigned int surfaceVisDataCount;
		char* smodelVisData[3];
		char* surfaceVisData[3];
		unsigned __int16* sortedSurfIndex;
		GfxStaticModelInst* smodelInsts;
		GfxSurface* surfaces;
		GfxSurfaceBounds* surfacesBounds;
		GfxStaticModelDrawInst* smodelDrawInsts;
		GfxDrawSurf* surfaceMaterials;
		unsigned int* surfaceCastsSunShadow;
		volatile int usageCount;
	};

	struct GfxLightGridEntry
	{
		unsigned short colorsIndex;
		unsigned char primaryLightIndex;
		unsigned char needsTrace;
	};

	struct GfxLightGridColors
	{
		unsigned char rgb[56][3];
	};

	struct GfxLightGrid
	{
		bool hasLightRegions;
		unsigned int lastSunPrimaryLightIndex;
		unsigned short mins[3];
		unsigned short maxs[3];
		unsigned int rowAxis;
		unsigned int colAxis;
		unsigned short* rowDataStart;
		unsigned int rawRowDataSize;
		unsigned char* rawRowData;
		unsigned int entryCount;
		GfxLightGridEntry* entries;
		unsigned int colorCount;
		GfxLightGridColors* colors;
	};

	struct MaterialMemory
	{
		Material* material;
		int memory;
	};

	struct sunflare_t
	{
		bool hasValidData;
		Material* spriteMaterial;
		Material* flareMaterial;
		float spriteSize;
		float flareMinSize;
		float flareMinDot;
		float flareMaxSize;
		float flareMaxDot;
		float flareMaxAlpha;
		int flareFadeInTime;
		int flareFadeOutTime;
		float blindMinDot;
		float blindMaxDot;
		float blindMaxDarken;
		int blindFadeInTime;
		int blindFadeOutTime;
		float glareMinDot;
		float glareMaxDot;
		float glareMaxLighten;
		int glareFadeInTime;
		int glareFadeOutTime;
		float sunFxPosition[3];
	};

	struct cplane_s
	{
		float normal[3];
		float dist;
		unsigned char type;
		unsigned char pad[3];
	};

	struct cbrushside_t
	{
		cplane_s* plane;
		unsigned short materialNum;
		unsigned char firstAdjacentSideOffset;
		unsigned char edgeCount;
	};

	struct cbrushWrapper_t
	{
		unsigned short numsides;
		unsigned short glassPieceIndex;
		cbrushside_t* sides;
		unsigned char* baseAdjacentSide;
		short axialMaterialNum[2][3];
		unsigned char firstAdjacentSideOffsets[2][3];
		unsigned char edgeCount[2][3];
	};

	struct BrushWrapper
	{
		Bounds bounds;
		cbrushWrapper_t brush;
		int totalEdgeCount;
		cplane_s* planes;
	};

	enum PhysicsGeomType
	{
		PHYS_GEOM_NONE = 0x0,
		PHYS_GEOM_BOX = 0x1,
		PHYS_GEOM_BRUSHMODEL = 0x2,
		PHYS_GEOM_BRUSH = 0x3,
		PHYS_GEOM_COLLMAP = 0x4,
		PHYS_GEOM_CYLINDER = 0x5,
		PHYS_GEOM_CAPSULE = 0x6,
		PHYS_GEOM_GLASS = 0x7,
		PHYS_GEOM_COUNT = 0x8
	};

	struct PhysGeomInfo
	{
		BrushWrapper* brushWrapper;
		PhysicsGeomType type;
		float orientation[3][3];
		Bounds bounds;
	};

	struct PhysMass
	{
		float centerOfMass[3];
		float momentsOfInertia[3];
		float productsOfInertia[3];
	};

	struct PhysCollmap
	{
		const char* name;
		unsigned int count;
		PhysGeomInfo* geoms;
		PhysMass mass;
		Bounds bounds;
	};

	struct GfxWorldDpvsPlanes
	{
		int cellCount;
		cplane_s* planes;
		unsigned short* nodes;
		unsigned int* sceneEntCellBits;
	};

	struct GfxAabbTree
	{
		Bounds bounds;
		unsigned short childCount;
		unsigned short surfaceCount;
		unsigned short startSurfIndex;
		unsigned short surfaceCountNoDecal;
		unsigned short startSurfIndexNoDecal;
		unsigned short smodelIndexCount;
		unsigned short* smodelIndexes;
		int childrenOffset;
	};

	struct GfxCellTree
	{
		GfxAabbTree* aabbTree;
	};

	struct GfxCellTreeCount
	{
		int aabbTreeCount;
	};

	struct GfxBrushModelWritable
	{
		Bounds bounds;
	};

	struct GfxBrushModel
	{
		GfxBrushModelWritable writable;
		Bounds bounds;
		float radius;
		unsigned short surfaceCount;
		unsigned short startSurfIndex;
		unsigned short surfaceCountNoDecal;
	};

	struct BModelDrawInfo
	{
		unsigned short surfId;
	};

	struct GfxSceneDynBrush
	{
		BModelDrawInfo info;
		unsigned short dynEntId;
	};

	struct XModelDrawInfo
	{
		unsigned char hasGfxEntIndex;
		unsigned char lod;
		unsigned short surfId;
	};

	struct GfxSceneDynModel
	{
		XModelDrawInfo info;
		unsigned short dynEntId;
	};

	struct GfxShadowGeometry
	{
		unsigned short  surfaceCount;
		unsigned short  smodelCount;
		unsigned short* sortedSurfIndex;
		unsigned short* smodelIndex;
	};

	struct GfxLightRegionAxis
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct GfxLightRegionHull
	{
		float kdopMidPoint[9];
		float kdopHalfSize[9];
		unsigned int axisCount;
		GfxLightRegionAxis* axis;
	};

	struct GfxLightRegion
	{
		unsigned int hullCount;
		GfxLightRegionHull* hulls;
	};

	struct GfxWorldDpvsDynamic
	{
		unsigned int dynEntClientWordCount[2];
		unsigned int dynEntClientCount[2];
		unsigned int* dynEntCellBits[2];
		char* dynEntVisData[2][3];
	};

	struct GfxWorld
	{
		const char* name;
		const char* baseName;
		int planeCount;
		int nodeCount;
		unsigned int surfaceCount;
		int skyCount;
		GfxSky* skies;
		unsigned int lastSunPrimaryLightIndex;
		unsigned int primaryLightCount;
		unsigned int sortKeyLitDecal;
		unsigned int sortKeyEffectDecal;
		unsigned int sortKeyEffectAuto;
		unsigned int sortKeyDistortion;
		GfxWorldDpvsPlanes dpvsPlanes;
		GfxCellTreeCount* aabbTreeCounts;
		GfxCellTree* aabbTrees;
		GfxCell* cells;
		GfxWorldDraw draw;
		GfxLightGrid lightGrid;
		int modelCount;
		GfxBrushModel* models;
		Bounds bounds;
		unsigned int checksum;
		int materialMemoryCount;
		MaterialMemory* materialMemory;
		sunflare_t sun;
		float outdoorLookupMatrix[4][4];
		GfxImage* outdoorImage;
		unsigned int* cellCasterBits;
		unsigned int* cellHasSunLitSurfsBits;
		GfxSceneDynModel* sceneDynModel;
		GfxSceneDynBrush* sceneDynBrush;
		unsigned int* primaryLightEntityShadowVis;
		unsigned int* primaryLightDynEntShadowVis[2];
		unsigned char* nonSunPrimaryLightForModelDynEnt;
		GfxShadowGeometry* shadowGeom;
		GfxLightRegion* lightRegion;
		GfxWorldDpvsStatic dpvs;
		GfxWorldDpvsDynamic dpvsDyn;
		unsigned int mapVtxChecksum;
		unsigned int heroOnlyLightCount;
		GfxHeroOnlyLight* heroOnlyLights;
		char fogTypesAllowed;
	};

#pragma pack(pop)

	enum file_image_flags_t
	{
		IMG_FLAG_NOPICMIP = 0x1,
		IMG_FLAG_NOMIPMAPS = 0x2,
		IMG_FLAG_STREAMING = 0x4,
		IMG_FLAG_LEGACY_NORMALS = 0x8,
		IMG_FLAG_CLAMP_U = 0x10,
		IMG_FLAG_CLAMP_V = 0x20,
		IMG_FLAG_ALPHA_WEIGHTED_COLORS = 0x40,
		IMG_FLAG_DXTC_APPROX_WEIGHTS = 0x80,
		IMG_FLAG_GAMMA_NONE = 0x0,
		IMG_FLAG_GAMMA_SRGB = 0x100,
		IMG_FLAG_GAMMA_PWL = 0x200,
		IMG_FLAG_GAMMA_2 = 0x300,
		IMG_FLAG_MAPTYPE_2D = 0x0,
		IMG_FLAG_MAPTYPE_CUBE = 0x10000,
		IMG_FLAG_MAPTYPE_3D = 0x20000,
		IMG_FLAG_MAPTYPE_1D = 0x30000,
		IMG_FLAG_NORMALMAP = 0x40000,
		IMG_FLAG_INTENSITY_TO_ALPHA = 0x80000,
		IMG_FLAG_DYNAMIC = 0x1000000,
		IMG_FLAG_RENDER_TARGET = 0x2000000,
		IMG_FLAG_SYSTEMMEM = 0x4000000,
	};
	
	union PackedTexCoords
	{
		unsigned int packed;
	};

	struct GfxPackedVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		PackedTexCoords texCoord;
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};


	struct XSurfaceCollisionAabb
	{
		unsigned short mins[3];
		unsigned short maxs[3];
	};

	struct XSurfaceCollisionNode
	{
		XSurfaceCollisionAabb aabb;
		unsigned short childBeginIndex;
		unsigned short childCount;
	};

	struct XSurfaceCollisionLeaf
	{
		unsigned short triangleBeginIndex;
	};

	struct XSurfaceCollisionTree
	{
		float trans[3];
		float scale[3];
		unsigned int nodeCount;
		XSurfaceCollisionNode* nodes;
		unsigned int leafCount;
		XSurfaceCollisionLeaf* leafs;
	};

	struct XRigidVertList
	{
		unsigned short boneOffset;
		unsigned short vertCount;
		unsigned short triOffset;
		unsigned short triCount;
		XSurfaceCollisionTree* collisionTree;
	};

	struct XSurfaceVertexInfo
	{
		short vertCount[4];
		unsigned short* vertsBlend;
	};

	struct XSurface
	{
		unsigned char tileMode;
		bool deformed;
		unsigned __int16 vertCount;
		unsigned __int16 triCount;
		char zoneHandle;
		unsigned __int16 baseTriIndex;
		unsigned __int16 baseVertIndex;
		unsigned __int16* triIndices;
		XSurfaceVertexInfo vertInfo;
		GfxPackedVertex* verts0;
		unsigned int vertListCount;
		XRigidVertList* vertList;
		int partBits[6];
	};

	struct XModelSurfs
	{
		const char* name;
		XSurface* surfaces;
		int numSurfaces;
		int partBits[6];
	};

	struct XModelLodInfo
	{
		float dist;
		unsigned __int16 numsurfs;
		unsigned __int16 surfIndex;
		XModelSurfs* modelSurfs;
		int partBits[6];
		XSurface* surfs;
		char lod;
		char smcBaseIndexPlusOne;
		char smcSubIndexMask;
		char smcBucket;
	};

	struct PhysPreset
	{
		const char* name;
		int type;
		float mass;
		float bounce;
		float friction;
		float bulletForceScale;
		float explosiveForceScale;
		const char* sndAliasPrefix;
		float piecesSpreadFraction;
		float piecesUpwardVelocity;
		bool tempDefaultToCylinder;
		bool perSurfaceSndAlias;
	};


	struct XModelCollTri_s
	{
		float plane[4];
		float svec[4];
		float tvec[4];
	};

	struct XModelCollSurf_s
	{
		XModelCollTri_s* collTris;
		int numCollTris;
		Bounds bounds;
		int boneIdx;
		int contents;
		int surfFlags;
	};

	struct XBoneInfo
	{
		Bounds bounds;
		float radiusSquared;
	};

	struct DObjAnimMat
	{
		float quat[4];
		float trans[3];
		float transWeight;
	};

	struct XModel
	{
		const char* name;
		unsigned char numBones;
		unsigned char numRootBones;
		unsigned char numsurfs;
		char lodRampType;
		float scale;
		unsigned int noScalePartBits[6];
		unsigned __int16* boneNames;
		unsigned char* parentList;
		short(*quats)[4];
		float(*trans)[3];
		unsigned char* partClassification;
		DObjAnimMat* baseMat;
		Material** materialHandles;
		XModelLodInfo lodInfo[4];
		char maxLoadedLod;
		char numLods;
		char collLod;
		unsigned char flags;
		XModelCollSurf_s* collSurfs;
		int numCollSurfs;
		int contents;
		XBoneInfo* boneInfo;
		float radius;
		Bounds bounds;
		int memUsage;
		bool bad;
		PhysPreset* physPreset;
		PhysCollmap* physCollmap;
	};

	static_assert(sizeof XModel == 304);

	struct cStaticModel_t
	{
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		float absmin[3];
		float absmax[3];
	};

	struct dmaterial_t
	{
		char* material;
		int surfaceFlags;
		int contentFlags;
	};

	struct cLeafBrushNodeLeaf_t
	{
		unsigned short* brushes;
	};

	struct cLeafBrushNode_t
	{
		char axis;
		__int16 leafBrushCount;
		int contents;
		cLeafBrushNodeLeaf_t data;
		char pad[8];
	};

	struct GfxPlacement
	{
		float quat[4];
		float origin[3];
	};

	struct Stage
	{
		const char* name;
		float origin[3];
		unsigned short triggerIndex;
		unsigned char sunPrimaryLightIndex;
	};

	struct MapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;
		Stage* stages;
		char stageCount;
	};

	enum DynEntityType
	{
		DYNENT_TYPE_INVALID = 0x0,
		DYNENT_TYPE_CLUTTER = 0x1,
		DYNENT_TYPE_DESTRUCT = 0x2,
		DYNENT_TYPE_COUNT = 0x3,
	};

	struct DynEntityDef
	{
		DynEntityType type;
		GfxPlacement pose;
		XModel* xModel;
		unsigned short brushModel;
		unsigned short physicsBrushModel;
		struct FxEffectDef* destroyFx;
		PhysPreset* physPreset;
		int health;
		PhysMass mass;
		int contents;
	};

	struct DynEntityClient
	{
		int physObjId;
		unsigned short flags;
		unsigned short lightingHandle;
		int health;
	};

	struct cStaticModel_s
	{
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		Bounds absBounds;
	};

	struct cLeaf_t
	{
		unsigned short firstCollAabbIndex;
		unsigned short collAabbCount;
		int brushContents;
		int terrainContents;
		Bounds bounds;
		int leafBrushNode;
	};

	struct cmodel_t
	{
		Bounds bounds;
		float radius;
		cLeaf_t leaf;
	};

	struct ClipMaterial
	{
		const char* name;
		int surfaceFlags;
		int contents;
	};

	struct cNode_t
	{
		cplane_s* plane;
		short children[2];
	};

	struct cLeafBrushNodeChildren_t
	{
		float dist;
		float range;
		unsigned short childOffset[2];
	};

	union cLeafBrushNodeData_t
	{
		cLeafBrushNodeLeaf_t leaf;
		cLeafBrushNodeChildren_t children;
	};

	struct cLeafBrushNode_s
	{
		unsigned char axis;
		short leafBrushCount;
		int contents;
		cLeafBrushNodeData_t data;
	};

	struct CollisionBorder
	{
		float distEq[3];
		float zBase;
		float zSlope;
		float start;
		float length;
	};

	struct CollisionPartition
	{
		unsigned char triCount;
		unsigned char borderCount;
		unsigned char firstVertSegment;
		int firstTri;
		CollisionBorder* borders;
	};

	struct CollisionAabbTree
	{
		float midPoint[3];
		unsigned short materialIndex;
		unsigned short childCount;
		float halfSize[3];
		unsigned int u;
	};


	struct cbrush_t
	{
		unsigned short numsides;
		unsigned short glassPieceIndex;
		cbrushside_t* sides;
		unsigned char* baseAdjacentSide;
		unsigned short axialMaterialNum[2][3];
		unsigned char firstAdjacentSideOffsets[2][3];
		unsigned char edgeCount[2][3];
	};


	struct SModelAabbNode
	{
		Bounds bounds;
		unsigned short firstChild;
		unsigned short childCount;
	};

	struct DynEntityPose
	{
		GfxPlacement pose;
		float radius;
	};

	struct DynEntityColl
	{
		unsigned short sector;
		unsigned short nextEntInSector;
		float linkMins[2];
		float linkMaxs[2];
	};

	struct clipMap_t
	{
		const char* name;
		int isInUse;
		unsigned int planeCount;
		cplane_s* planes;
		unsigned int numStaticModels;
		cStaticModel_s* staticModelList;
		unsigned int numMaterials;
		ClipMaterial* materials;
		unsigned int numBrushSides;
		cbrushside_t* brushsides;
		unsigned int numBrushEdges;
		unsigned char* brushEdges;
		unsigned int numNodes;
		cNode_t* nodes;
		unsigned int numLeafs;
		cLeaf_t* leafs;
		unsigned int leafbrushNodesCount;
		cLeafBrushNode_s* leafbrushNodes;
		unsigned int numLeafBrushes;
		unsigned __int16* leafbrushes;
		unsigned int numLeafSurfaces;
		unsigned int* leafsurfaces;
		unsigned int vertCount;
		float(*verts)[3];
		unsigned int triCount;
		unsigned __int16* triIndices;
		unsigned char* triEdgeIsWalkable;
		unsigned int borderCount;
		CollisionBorder* borders;
		unsigned int partitionCount;
		CollisionPartition* partitions;
		unsigned int aabbTreeCount;
		CollisionAabbTree* aabbTrees;
		unsigned int numSubModels;
		cmodel_t* cmodels;
		unsigned __int16 numBrushes;
		cbrush_t* brushes;
		Bounds* brushBounds;
		int* brushContents;
		MapEnts* mapEnts;
		unsigned __int16 smodelNodeCount;
		SModelAabbNode* smodelNodes;
		unsigned __int16 dynEntCount[2];
		DynEntityDef* dynEntDefList[2];
		DynEntityPose* dynEntPoseList[2];
		DynEntityClient* dynEntClientList[2];
		DynEntityColl* dynEntCollList[2];
		unsigned int checksum;
	};

	enum MaterialTechniqueType
	{
		TECHNIQUE_DEPTH_PREPASS = 0x0,
		TECHNIQUE_BUILD_FLOAT_Z = 0x1,
		TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
		TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
		TECHNIQUE_UNLIT = 0x4,
		TECHNIQUE_EMISSIVE = 0x5,
		TECHNIQUE_EMISSIVE_DFOG = 0x6,
		TECHNIQUE_EMISSIVE_SHADOW = 0x7,
		TECHNIQUE_EMISSIVE_SHADOW_DFOG = 0x8,
		TECHNIQUE_LIT_BEGIN = 0x9,
		TECHNIQUE_LIT = 0x9,
		TECHNIQUE_LIT_DFOG = 0xA,
		TECHNIQUE_LIT_SUN = 0xB,
		TECHNIQUE_LIT_SUN_DFOG = 0xC,
		TECHNIQUE_LIT_SUN_SHADOW = 0xD,
		TECHNIQUE_LIT_SUN_SHADOW_DFOG = 0xE,
		TECHNIQUE_LIT_SPOT = 0xF,
		TECHNIQUE_LIT_SPOT_DFOG = 0x10,
		TECHNIQUE_LIT_SPOT_SHADOW = 0x11,
		TECHNIQUE_LIT_SPOT_SHADOW_DFOG = 0x12,
		TECHNIQUE_LIT_OMNI = 0x13,
		TECHNIQUE_LIT_OMNI_DFOG = 0x14,
		TECHNIQUE_LIT_OMNI_SHADOW = 0x15,
		TECHNIQUE_LIT_OMNI_SHADOW_DFOG = 0x16,
		TECHNIQUE_LIT_INSTANCED = 0x17,
		TECHNIQUE_LIT_INSTANCED_DFOG = 0x18,
		TECHNIQUE_LIT_INSTANCED_SUN = 0x19,
		TECHNIQUE_LIT_INSTANCED_SUN_DFOG = 0x1A,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW = 0x1B,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG = 0x1C,
		TECHNIQUE_LIT_INSTANCED_SPOT = 0x1D,
		TECHNIQUE_LIT_INSTANCED_SPOT_DFOG = 0x1E,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW = 0x1F,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG = 0x20,
		TECHNIQUE_LIT_INSTANCED_OMNI = 0x21,
		TECHNIQUE_LIT_INSTANCED_OMNI_DFOG = 0x22,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW = 0x23,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG = 0x24,
		TECHNIQUE_LIT_END = 0x25,
		TECHNIQUE_LIGHT_SPOT = 0x25,
		TECHNIQUE_LIGHT_OMNI = 0x26,
		TECHNIQUE_LIGHT_SPOT_SHADOW = 0x27,
		TECHNIQUE_FAKELIGHT_NORMAL = 0x28,
		TECHNIQUE_FAKELIGHT_VIEW = 0x29,
		TECHNIQUE_SUNLIGHT_PREVIEW = 0x2A,
		TECHNIQUE_CASE_TEXTURE = 0x2B,
		TECHNIQUE_WIREFRAME_SOLID = 0x2C,
		TECHNIQUE_WIREFRAME_SHADED = 0x2D,
		TECHNIQUE_DEBUG_BUMPMAP = 0x2E,
		TECHNIQUE_DEBUG_BUMPMAP_INSTANCED = 0x2F,
		TECHNIQUE_COUNT = 0x30,
		TECHNIQUE_TOTAL_COUNT = 0x31,
		TECHNIQUE_NONE = 0x32,
	};

#define NAMEOF(s) #s
	enum MaterialTextureSource : unsigned int
	{
		TEXTURE_SRC_CODE_BLACK = 0x0,
		TEXTURE_SRC_CODE_WHITE = 0x1,
		TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP = 0x2,
		TEXTURE_SRC_CODE_MODEL_LIGHTING = 0x3,
		TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY = 0x4,
		TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY = 0x5,
		TEXTURE_SRC_CODE_SHADOWMAP_SUN = 0x6,
		TEXTURE_SRC_CODE_SHADOWMAP_SPOT = 0x7,
		TEXTURE_SRC_CODE_FEEDBACK = 0x8,
		TEXTURE_SRC_CODE_RESOLVED_POST_SUN = 0x9,
		TEXTURE_SRC_CODE_RESOLVED_SCENE = 0xA,
		TEXTURE_SRC_CODE_POST_EFFECT_0 = 0xB,
		TEXTURE_SRC_CODE_POST_EFFECT_1 = 0xC,
		TEXTURE_SRC_CODE_LIGHT_ATTENUATION = 0xD,
		TEXTURE_SRC_CODE_OUTDOOR = 0xE,
		TEXTURE_SRC_CODE_FLOATZ = 0xF,
		TEXTURE_SRC_CODE_PROCESSED_FLOATZ = 0x10,
		TEXTURE_SRC_CODE_RAW_FLOATZ = 0x11,
		TEXTURE_SRC_CODE_HALF_PARTICLES = 0x12,
		TEXTURE_SRC_CODE_HALF_PARTICLES_Z = 0x13,
		TEXTURE_SRC_CODE_CASE_TEXTURE = 0x14,
		TEXTURE_SRC_CODE_CINEMATIC_Y = 0x15,
		TEXTURE_SRC_CODE_CINEMATIC_CR = 0x16,
		TEXTURE_SRC_CODE_CINEMATIC_CB = 0x17,
		TEXTURE_SRC_CODE_CINEMATIC_A = 0x18,
		TEXTURE_SRC_CODE_REFLECTION_PROBE = 0x19,
		TEXTURE_SRC_CODE_ALTERNATE_SCENE = 0x1A,
		TEXTURE_SRC_CODE_COUNT = 0x1B,
	};

	static const char* Debug_MaterialTextureSourceNames[]
	{
		NAMEOF(TEXTURE_SRC_CODE_BLACK),
		NAMEOF(TEXTURE_SRC_CODE_WHITE),
		NAMEOF(TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP),
		NAMEOF(TEXTURE_SRC_CODE_MODEL_LIGHTING),
		NAMEOF(TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY),
		NAMEOF(TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY),
		NAMEOF(TEXTURE_SRC_CODE_SHADOWMAP_SUN),
		NAMEOF(TEXTURE_SRC_CODE_SHADOWMAP_SPOT),
		NAMEOF(TEXTURE_SRC_CODE_FEEDBACK),
		NAMEOF(TEXTURE_SRC_CODE_RESOLVED_POST_SUN),
		NAMEOF(TEXTURE_SRC_CODE_RESOLVED_SCENE),
		NAMEOF(TEXTURE_SRC_CODE_POST_EFFECT_0),
		NAMEOF(TEXTURE_SRC_CODE_POST_EFFECT_1),
		NAMEOF(TEXTURE_SRC_CODE_LIGHT_ATTENUATION),
		NAMEOF(TEXTURE_SRC_CODE_OUTDOOR),
		NAMEOF(TEXTURE_SRC_CODE_FLOATZ),
		NAMEOF(TEXTURE_SRC_CODE_PROCESSED_FLOATZ),
		NAMEOF(TEXTURE_SRC_CODE_RAW_FLOATZ),
		NAMEOF(TEXTURE_SRC_CODE_HALF_PARTICLES),
		NAMEOF(TEXTURE_SRC_CODE_HALF_PARTICLES_Z),
		NAMEOF(TEXTURE_SRC_CODE_CASE_TEXTURE),
		NAMEOF(TEXTURE_SRC_CODE_CINEMATIC_Y),
		NAMEOF(TEXTURE_SRC_CODE_CINEMATIC_CR),
		NAMEOF(TEXTURE_SRC_CODE_CINEMATIC_CB),
		NAMEOF(TEXTURE_SRC_CODE_CINEMATIC_A),
		NAMEOF(TEXTURE_SRC_CODE_REFLECTION_PROBE),
		NAMEOF(TEXTURE_SRC_CODE_ALTERNATE_SCENE)
	};

	static_assert(ARRAYSIZE(Debug_MaterialTextureSourceNames) == MaterialTextureSource::TEXTURE_SRC_CODE_COUNT);

	enum StateFlags : unsigned char {
		STATE_FLAG_CULL_BACK = 0x1,
		STATE_FLAG_AMBIENT = 0x2,
		STATE_FLAG_DECAL = 0x4,
		STATE_FLAG_WRITES_DEPTH = 0x8,
		STATE_FLAG_USES_DEPTH_BUFFER = 0x10,
		STATE_FLAG_USES_STENCIL_BUFFER = 0x20,
		STATE_FLAG_CULL_BACK_SHADOW = 0x40,
	};
	enum MaterialStateSet
	{
		MTL_STATE_SET_ALPHA_TEST = 0x0,
		MTL_STATE_SET_BLEND_FUNC_RGB = 0x1,
		MTL_STATE_SET_BLEND_FUNC_ALPHA = 0x2,
		MTL_STATE_SET_CULL_FACE = 0x3,
		MTL_STATE_SET_DEPTH_TEST = 0x4,
		MTL_STATE_SET_DEPTH_WRITE = 0x5,
		MTL_STATE_SET_COLOR_WRITE = 0x6,
		MTL_STATE_SET_GAMMA_WRITE = 0x7,
		MTL_STATE_SET_POLYGON_OFFSET = 0x8,
		MTL_STATE_SET_STENCIL = 0x9,
		MTL_STATE_SET_WIREFRAME = 0xA,
		MTL_STATE_SET_COUNT = 0xB,
	};

	struct MaterialGameFlagsFields
	{
		unsigned char unk1 : 1; // 0x1
		unsigned char addShadowToPrimaryLight : 1; // 0x2
		unsigned char isFoliageRequiresGroundLighting : 1; // 0x4
		unsigned char unk4 : 1; // 0x8
		unsigned char unk5 : 1; // 0x10
		unsigned char unk6 : 1; // 0x20
		unsigned char unk7 : 1; // 0x40
		unsigned char unkCastShadowMaybe : 1; // 0x80
	};

	union MaterialGameFlags
	{
		MaterialGameFlagsFields fields;
		unsigned char packed;
	};

	enum GfxCameraRegionType
	{
		CAMERA_REGION_LIT_OPAQUE = 0x0,
		CAMERA_REGION_LIT_TRANS = 0x1,
		CAMERA_REGION_EMISSIVE = 0x2,
		CAMERA_REGION_DEPTH_HACK = 0x3,
		CAMERA_REGION_COUNT = 0x4,
		CAMERA_REGION_NONE = 0x4,
	};

	enum TextureSemantic
	{
		TS_2D = 0x0,
		TS_FUNCTION = 0x1,
		TS_COLOR_MAP = 0x2,
		TS_DETAIL_MAP = 0x3,
		TS_UNUSED_2 = 0x4,
		TS_NORMAL_MAP = 0x5,
		TS_UNUSED_3 = 0x6,
		TS_UNUSED_4 = 0x7,
		TS_SPECULAR_MAP = 0x8,
		TS_UNUSED_5 = 0x9,
		TS_UNUSED_6 = 0xA,
		TS_WATER_MAP = 0xB,
		TS_DISPLACEMENT_MAP = 0xC
	};

	struct WaterWritable
	{
		float floatTime;
	};

	struct complex_s
	{
		float real;
		float imag;
	};

	struct water_t
	{
		WaterWritable writable;
		complex_s* H0;
		float* wTerm;
		int M;
		int N;
		float Lx;
		float Lz;
		float gravity;
		float windvel;
		float winddir[2];
		float amplitude;
		float codeConstant[4];
		GfxImage* image;
	};

	union MaterialTextureDefInfo
	{
		GfxImage* image;
		water_t* water;
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		unsigned char samplerState;
		unsigned char semantic;
		MaterialTextureDefInfo u;
	};

	struct MaterialConstantDef
	{
		unsigned int nameHash;
		char name[12];
		float literal[4];
	};

	enum GfxSurfaceStatebitOp0 : unsigned int
	{
		GFXS0_SRCBLEND_RGB_SHIFT = 0x0,
		GFXS0_SRCBLEND_RGB_MASK = 0xF,
		GFXS0_DSTBLEND_RGB_SHIFT = 0x4,
		GFXS0_DSTBLEND_RGB_MASK = 0xF0,
		GFXS0_BLENDOP_RGB_SHIFT = 0x8,
		GFXS0_BLENDOP_RGB_MASK = 0x700,
		GFXS0_BLEND_RGB_MASK = 0x7FF,
		GFXS0_ATEST_DISABLE = 0x800,
		GFXS0_ATEST_GT_0 = 0x1000,
		GFXS0_ATEST_LT_128 = 0x2000,
		GFXS0_ATEST_GE_128 = 0x3000,
		GFXS0_ATEST_MASK = 0x3000,
		GFXS0_CULL_SHIFT = 0xE,
		GFXS0_CULL_NONE = 0x4000,
		GFXS0_CULL_BACK = 0x8000,
		GFXS0_CULL_FRONT = 0xC000,
		GFXS0_CULL_MASK = 0xC000,
		GFXS0_SRCBLEND_ALPHA_SHIFT = 0x10,
		GFXS0_SRCBLEND_ALPHA_MASK = 0xF0000,
		GFXS0_DSTBLEND_ALPHA_SHIFT = 0x14,
		GFXS0_DSTBLEND_ALPHA_MASK = 0xF00000,
		GFXS0_BLENDOP_ALPHA_SHIFT = 0x18,
		GFXS0_BLENDOP_ALPHA_MASK = 0x7000000,
		GFXS0_BLEND_ALPHA_MASK = 0x7FF0000,
		GFXS0_COLORWRITE_RGB = 0x8000000,
		GFXS0_COLORWRITE_ALPHA = 0x10000000,
		GFXS0_COLORWRITE_MASK = 0x18000000,
		GFXS0_GAMMAWRITE = 0x40000000,
		GFXS0_POLYMODE_LINE = 0x80000000
	};

	enum GfxSurfaceStatebitOp1 : unsigned int
	{
		GFXS1_DEPTHWRITE = 0x1,
		GFXS1_DEPTHTEST_DISABLE = 0x2,
		GFXS1_DEPTHTEST_SHIFT = 0x2,
		GFXS1_DEPTHTEST_ALWAYS = 0x0,
		GFXS1_DEPTHTEST_LESS = 0x4,
		GFXS1_DEPTHTEST_EQUAL = 0x8,
		GFXS1_DEPTHTEST_LESSEQUAL = 0xC,
		GFXS1_DEPTHTEST_MASK = 0xC,
		GFXS1_POLYGON_OFFSET_SHIFT = 0x4,
		GFXS1_POLYGON_OFFSET_0 = 0x0,
		GFXS1_POLYGON_OFFSET_1 = 0x10,
		GFXS1_POLYGON_OFFSET_2 = 0x20,
		GFXS1_POLYGON_OFFSET_SHADOWMAP = 0x30,
		GFXS1_POLYGON_OFFSET_MASK = 0x30,
		GFXS1_STENCIL_FRONT_ENABLE = 0x40,
		GFXS1_STENCIL_BACK_ENABLE = 0x80,
		GFXS1_STENCIL_MASK = 0xC0,
		GFXS1_STENCIL_FRONT_PASS_SHIFT = 0x8,
		GFXS1_STENCIL_FRONT_FAIL_SHIFT = 0xB,
		GFXS1_STENCIL_FRONT_ZFAIL_SHIFT = 0xE,
		GFXS1_STENCIL_FRONT_FUNC_SHIFT = 0x11,
		GFXS1_STENCIL_FRONT_MASK = 0xFFF00,
		GFXS1_STENCIL_BACK_PASS_SHIFT = 0x14,
		GFXS1_STENCIL_BACK_FAIL_SHIFT = 0x17,
		GFXS1_STENCIL_BACK_ZFAIL_SHIFT = 0x1A,
		GFXS1_STENCIL_BACK_FUNC_SHIFT = 0x1D,
		GFXS1_STENCIL_BACK_MASK = 0xFFF00000,
		GFXS1_STENCILFUNC_FRONTBACK_MASK = 0xE00E0000,
		GFXS1_STENCILOP_FRONTBACK_MASK = 0x1FF1FF00,
	};

	enum GfxStencilOp
	{
		GFXS_STENCILOP_KEEP = 0x0,
		GFXS_STENCILOP_ZERO = 0x1,
		GFXS_STENCILOP_REPLACE = 0x2,
		GFXS_STENCILOP_INCRSAT = 0x3,
		GFXS_STENCILOP_DECRSAT = 0x4,
		GFXS_STENCILOP_INVERT = 0x5,
		GFXS_STENCILOP_INCR = 0x6,
		GFXS_STENCILOP_DECR = 0x7,

		GFXS_STENCILOP_COUNT,
		GFXS_STENCILOP_MASK = 0x7
	};

	struct GfxStatebitsFlags {
		GfxSurfaceStatebitOp0 loadbit0;
		GfxSurfaceStatebitOp1 loadbit1;
	};

	union GfxStateBits
	{
		GfxStatebitsFlags flags;
		unsigned int loadBits[2];
	};

	enum MaterialShaderArgumentType
	{
		MTL_ARG_MATERIAL_VERTEX_CONST = 0x0, // stable
		MTL_ARG_LITERAL_VERTEX_CONST = 0x1, // stable
		MTL_ARG_MATERIAL_PIXEL_SAMPLER = 0x2, // stable

		MTL_ARG_CODE_PRIM_BEGIN = 0x3,

		MTL_ARG_CODE_VERTEX_CONST = 0x3, // stable object prim
		MTL_ARG_CODE_PIXEL_SAMPLER = 0x4, // stable object
		MTL_ARG_CODE_PIXEL_CONST = 0x5, // stable

		MTL_ARG_CODE_PRIM_END = 0x6,

		MTL_ARG_MATERIAL_PIXEL_CONST = 0x6, // stable
		MTL_ARG_LITERAL_PIXEL_CONST = 0x7, // stable
		MTL_ARG_COUNT = 0x8,
	};

	static const char* Debug_MaterialShaderArgumentTypeName[]
	{
		NAMEOF(MTL_ARG_MATERIAL_VERTEX_CONST),
		NAMEOF(MTL_ARG_LITERAL_VERTEX_CONST),
		NAMEOF(MTL_ARG_MATERIAL_PIXEL_SAMPLER),

		NAMEOF(MTL_ARG_CODE_VERTEX_CONST = 0x3),
		NAMEOF(MTL_ARG_CODE_PIXEL_SAMPLER = 0x4),
		NAMEOF(MTL_ARG_CODE_PIXEL_CONST = 0x5),

		NAMEOF(MTL_ARG_MATERIAL_PIXEL_CONST = 0x6),
		NAMEOF(MTL_ARG_LITERAL_PIXEL_CONST = 0x7)
	};

	static_assert(ARRAYSIZE(Debug_MaterialShaderArgumentTypeName) == MaterialShaderArgumentType::MTL_ARG_COUNT);

	enum MaterialUpdateFrequency
	{
		MTL_UPDATE_PER_PRIM = 0x0,
		MTL_UPDATE_PER_OBJECT = 0x1,
		MTL_UPDATE_RARELY = 0x2,
		MTL_UPDATE_CUSTOM = 0x3,
	};

	static const MaterialUpdateFrequency codeSamplerUpdateFrequency[]
	{
		MTL_UPDATE_RARELY, // BLACK 
		MTL_UPDATE_RARELY, // WHITE 
		MTL_UPDATE_RARELY, // IDENTITY_NORMAL_MAP 
		MTL_UPDATE_RARELY, // MODEL_LIGHTING 
		MTL_UPDATE_CUSTOM, // LIGHTMAP_PRIMARY 
		MTL_UPDATE_CUSTOM, // LIGHTMAP_SECONDARY 
		MTL_UPDATE_RARELY, // SHADOWMAP_SUN 
		MTL_UPDATE_RARELY, // SHADOWMAP_SPOT 
		MTL_UPDATE_PER_OBJECT, // FEEDBACK 
		MTL_UPDATE_RARELY, // RESOLVED_POST_SUN 
		MTL_UPDATE_RARELY, // RESOLVED_SCENE 
		MTL_UPDATE_RARELY, // POST_EFFECT_0 
		MTL_UPDATE_RARELY, // POST_EFFECT_1 
		MTL_UPDATE_PER_OBJECT, // LIGHT_ATTENUATION 
		MTL_UPDATE_RARELY, // OUTDOOR 
		MTL_UPDATE_RARELY, // FLOATZ 
		MTL_UPDATE_RARELY, // PROCESSED_FLOATZ 
		MTL_UPDATE_RARELY, // RAW_FLOATZ 
		MTL_UPDATE_RARELY, // HALF_PARTICLES 
		MTL_UPDATE_RARELY, // HALF_PARTICLES_Z 
		MTL_UPDATE_PER_OBJECT, // CASE_TEXTURE 
		MTL_UPDATE_PER_OBJECT, // CINEMATIC_Y 
		MTL_UPDATE_PER_OBJECT, // CINEMATIC_CR 
		MTL_UPDATE_PER_OBJECT, // CINEMATIC_CB 
		MTL_UPDATE_PER_OBJECT, // CINEMATIC_A 
		MTL_UPDATE_CUSTOM, // REFLECTION_PROBE 
		MTL_UPDATE_RARELY, // ALTERNATE_SCENE 
	};

	static_assert(ARRAYSIZE(codeSamplerUpdateFrequency) == TEXTURE_SRC_CODE_COUNT);

	struct MaterialInfo
	{
		char* name;
		unsigned char gameFlags;
		unsigned char sortKey;
		unsigned char textureAtlasRowCount;
		unsigned char textureAtlasColumnCount;
		GfxDrawSurf drawSurf;
		unsigned int surfaceTypeBits;
	};

	struct Material
	{
		MaterialInfo info;
		unsigned char stateBitsEntry[48];
		unsigned char textureCount;
		unsigned char constantCount;
		unsigned char stateBitsCount;
		unsigned char stateFlags;
		unsigned char cameraRegion; // GfxCameraRegionType
		struct MaterialTechniqueSet* techniqueSet;
		struct MaterialTextureDef* textureTable;
		MaterialConstantDef* constantTable;
		GfxStateBits* stateBitsTable;
	};

	enum MaterialStreamRoutingSource : unsigned char
	{
		STREAM_SRC_POSITION = 0x0,
		STREAM_SRC_COLOR = 0x1,
		STREAM_SRC_TEXCOORD_0 = 0x2,
		STREAM_SRC_NORMAL = 0x3,
		STREAM_SRC_TANGENT = 0x4,
		STREAM_SRC_OPTIONAL_BEGIN = 0x5,
		STREAM_SRC_PRE_OPTIONAL_BEGIN = 0x4,
		STREAM_SRC_TEXCOORD_1 = 0x5,
		STREAM_SRC_TEXCOORD_2 = 0x6,
		STREAM_SRC_NORMAL_TRANSFORM_0 = 0x7,
		STREAM_SRC_NORMAL_TRANSFORM_1 = 0x8,
		STREAM_SRC_COUNT = 0x9,
	};

	enum MaterialStreamRoutingDestination : unsigned char
	{
		STREAM_DST_POSITION = 0x0,
		STREAM_DST_NORMAL = 0x1,
		STREAM_DST_COLOR_0 = 0x2,
		STREAM_DST_COLOR_1 = 0x3,
		STREAM_DST_DEPTH = 0x4,
		STREAM_DST_TEXCOORD_0 = 0x5,
		STREAM_DST_TEXCOORD_1 = 0x6,
		STREAM_DST_TEXCOORD_2 = 0x7,
		STREAM_DST_TEXCOORD_3 = 0x8,
		STREAM_DST_TEXCOORD_4 = 0x9,
		STREAM_DST_TEXCOORD_5 = 0xA,
		STREAM_DST_TEXCOORD_6 = 0xB,
		STREAM_DST_TEXCOORD_7 = 0xC,
		STREAM_DST_COUNT = 0xD,
	};


	struct MaterialStreamRouting
	{
		MaterialStreamRoutingSource source;
		MaterialStreamRoutingDestination dest;
	};

	struct MaterialVertexStreamRouting
	{
		MaterialStreamRouting data[13];
		void* decl[16];
	};

	struct MaterialVertexDeclaration
	{
		const char* name;
		char streamCount;
		bool hasOptionalSource;
		MaterialVertexStreamRouting routing;
	};

	enum ShaderCodeConstants : unsigned short
	{
		CONST_SRC_CODE_LIGHT_POSITION = 0x0,
		CONST_SRC_CODE_LIGHT_DIFFUSE = 0x1,
		CONST_SRC_CODE_LIGHT_SPECULAR = 0x2,
		CONST_SRC_CODE_LIGHT_SPOTDIR = 0x3,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS = 0x4,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT = 0x5,
		CONST_SRC_CODE_PARTICLE_CLOUD_COLOR = 0x6,
		CONST_SRC_CODE_GAMETIME = 0x7,
		CONST_SRC_CODE_PIXEL_COST_FRACS = 0x8,
		CONST_SRC_CODE_PIXEL_COST_DECODE = 0x9,
		CONST_SRC_CODE_FILTER_TAP_0 = 0xA,
		CONST_SRC_CODE_FILTER_TAP_1 = 0xB,
		CONST_SRC_CODE_FILTER_TAP_2 = 0xC,
		CONST_SRC_CODE_FILTER_TAP_3 = 0xD,
		CONST_SRC_CODE_FILTER_TAP_4 = 0xE,
		CONST_SRC_CODE_FILTER_TAP_5 = 0xF,
		CONST_SRC_CODE_FILTER_TAP_6 = 0x10,
		CONST_SRC_CODE_FILTER_TAP_7 = 0x11,
		CONST_SRC_CODE_COLOR_MATRIX_R = 0x12,
		CONST_SRC_CODE_COLOR_MATRIX_G = 0x13,
		CONST_SRC_CODE_COLOR_MATRIX_B = 0x14,
		CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET = 0x15,
		CONST_SRC_CODE_RENDER_TARGET_SIZE = 0x16,
		CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR = 0x17,
		CONST_SRC_CODE_DOF_EQUATION_SCENE = 0x18,
		CONST_SRC_CODE_DOF_LERP_SCALE = 0x19,
		CONST_SRC_CODE_DOF_LERP_BIAS = 0x1A,
		CONST_SRC_CODE_DOF_ROW_DELTA = 0x1B,
		CONST_SRC_CODE_MOTION_MATRIX_X = 0x1C,
		CONST_SRC_CODE_MOTION_MATRIX_Y = 0x1D,
		CONST_SRC_CODE_MOTION_MATRIX_W = 0x1E,
		CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION = 0x1F,
		CONST_SRC_CODE_SHADOWMAP_SCALE = 0x20,
		CONST_SRC_CODE_ZNEAR = 0x21,
		CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE = 0x22,
		CONST_SRC_CODE_DEBUG_BUMPMAP = 0x23,
		CONST_SRC_CODE_MATERIAL_COLOR = 0x24,
		CONST_SRC_CODE_FOG = 0x25,
		CONST_SRC_CODE_FOG_COLOR_LINEAR = 0x26,
		CONST_SRC_CODE_FOG_COLOR_GAMMA = 0x27,
		CONST_SRC_CODE_FOG_SUN_CONSTS = 0x28,
		CONST_SRC_CODE_FOG_SUN_COLOR_LINEAR = 0x29,
		CONST_SRC_CODE_FOG_SUN_COLOR_GAMMA = 0x2A,
		CONST_SRC_CODE_FOG_SUN_DIR = 0x2B,
		CONST_SRC_CODE_GLOW_SETUP = 0x2C,
		CONST_SRC_CODE_GLOW_APPLY = 0x2D,
		CONST_SRC_CODE_COLOR_BIAS = 0x2E,
		CONST_SRC_CODE_COLOR_TINT_BASE = 0x2F,
		CONST_SRC_CODE_COLOR_TINT_DELTA = 0x30,
		CONST_SRC_CODE_COLOR_TINT_QUADRATIC_DELTA = 0x31,
		CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS = 0x32,
		CONST_SRC_CODE_ENVMAP_PARMS = 0x33,
		CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST = 0x34,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST = 0x35,
		CONST_SRC_CODE_COMPOSITE_FX_DISTORTION = 0x36,
		CONST_SRC_CODE_POSTFX_FADE_EFFECT = 0x37,
		CONST_SRC_CODE_VIEWPORT_DIMENSIONS = 0x38,
		CONST_SRC_CODE_FRAMEBUFFER_READ = 0x39,
		CONST_SRC_CODE_BASE_LIGHTING_COORDS = 0x3A,
		CONST_SRC_CODE_LIGHT_PROBE_AMBIENT = 0x3B,
		CONST_SRC_CODE_NEARPLANE_ORG = 0x3C,
		CONST_SRC_CODE_NEARPLANE_DX = 0x3D,
		CONST_SRC_CODE_NEARPLANE_DY = 0x3E,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE = 0x3F,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET = 0x40,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0 = 0x41,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX1 = 0x42,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX2 = 0x43,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR0 = 0x44,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR1 = 0x45,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR2 = 0x46,
		CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM0 = 0x47,
		CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM1 = 0x48,
		CONST_SRC_CODE_DEPTH_FROM_CLIP = 0x49,
		CONST_SRC_CODE_CODE_MESH_ARG_0 = 0x4A,
		CONST_SRC_CODE_CODE_MESH_ARG_1 = 0x4B,
		CONST_SRC_CODE_VIEW_MATRIX = 0x4C,
		CONST_SRC_CODE_INVERSE_VIEW_MATRIX = 0x4D,
		CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX = 0x4E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX = 0x4F,
		CONST_SRC_CODE_PROJECTION_MATRIX = 0x50,
		CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX = 0x51,
		CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX = 0x52,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX = 0x53,
		CONST_SRC_CODE_VIEW_PROJECTION_MATRIX = 0x54,
		CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX = 0x55,
		CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x56,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x57,
		CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX = 0x58,
		CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX = 0x59,
		CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x5A,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x5B,
		CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5C,
		CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5D,
		CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5F,
		CONST_SRC_CODE_WORLD_MATRIX0 = 0x60,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX0 = 0x61,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0 = 0x62,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0 = 0x63,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX0 = 0x64,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0 = 0x65,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0 = 0x66,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0 = 0x67,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x68,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x69,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x6A,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x6B,
		CONST_SRC_CODE_WORLD_MATRIX1 = 0x6C,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX1 = 0x6D,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX1 = 0x6E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX1 = 0x6F,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX1 = 0x70,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX1 = 0x71,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX1 = 0x72,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1 = 0x73,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x74,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x75,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x76,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x77,
		CONST_SRC_CODE_WORLD_MATRIX2 = 0x78,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX2 = 0x79,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX2 = 0x7A,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX2 = 0x7B,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX2 = 0x7C,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX2 = 0x7D,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX2 = 0x7E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2 = 0x7F,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x80,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x81,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x82,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x83,
		CONST_SRC_TOTAL_COUNT = 0x84,
		CONST_SRC_NONE = 0x85,
	};


	static const MaterialUpdateFrequency codeConstUpdateFrequency[]
	{
		MTL_UPDATE_RARELY, // LIGHT_POSITION 
		MTL_UPDATE_RARELY, // LIGHT_DIFFUSE 
		MTL_UPDATE_RARELY, // LIGHT_SPECULAR 
		MTL_UPDATE_RARELY, // LIGHT_SPOTDIR 
		MTL_UPDATE_RARELY, // LIGHT_SPOTFACTORS 
		MTL_UPDATE_RARELY, // LIGHT_FALLOFF_PLACEMENT 
		MTL_UPDATE_RARELY, // PARTICLE_CLOUD_COLOR 
		MTL_UPDATE_RARELY, // GAMETIME 
		MTL_UPDATE_RARELY, // PIXEL_COST_FRACS 
		MTL_UPDATE_RARELY, // PIXEL_COST_DECODE 
		MTL_UPDATE_RARELY, // FILTER_TAP_0 
		MTL_UPDATE_RARELY, // FILTER_TAP_1 
		MTL_UPDATE_RARELY, // FILTER_TAP_2 
		MTL_UPDATE_RARELY, // FILTER_TAP_3 
		MTL_UPDATE_RARELY, // FILTER_TAP_4 
		MTL_UPDATE_RARELY, // FILTER_TAP_5 
		MTL_UPDATE_RARELY, // FILTER_TAP_6 
		MTL_UPDATE_RARELY, // FILTER_TAP_7 
		MTL_UPDATE_RARELY, // COLOR_MATRIX_R 
		MTL_UPDATE_RARELY, // COLOR_MATRIX_G 
		MTL_UPDATE_RARELY, // COLOR_MATRIX_B 
		MTL_UPDATE_RARELY, // SHADOWMAP_POLYGON_OFFSET 
		MTL_UPDATE_RARELY, // RENDER_TARGET_SIZE 
		MTL_UPDATE_RARELY, // DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR 
		MTL_UPDATE_RARELY, // DOF_EQUATION_SCENE 
		MTL_UPDATE_RARELY, // DOF_LERP_SCALE 
		MTL_UPDATE_RARELY, // DOF_LERP_BIAS 
		MTL_UPDATE_RARELY, // DOF_ROW_DELTA 
		MTL_UPDATE_RARELY, // MOTION_MATRIX_X 
		MTL_UPDATE_RARELY, // MOTION_MATRIX_Y 
		MTL_UPDATE_RARELY, // MOTION_MATRIX_W 
		MTL_UPDATE_RARELY, // SHADOWMAP_SWITCH_PARTITION 
		MTL_UPDATE_RARELY, // SHADOWMAP_SCALE 
		MTL_UPDATE_RARELY, // ZNEAR 
		MTL_UPDATE_RARELY, // LIGHTING_LOOKUP_SCALE 
		MTL_UPDATE_RARELY, // DEBUG_BUMPMAP 
		MTL_UPDATE_RARELY, // MATERIAL_COLOR 
		MTL_UPDATE_RARELY, // FOG 
		MTL_UPDATE_RARELY, // FOG_COLOR_LINEAR 
		MTL_UPDATE_RARELY, // FOG_COLOR_GAMMA 
		MTL_UPDATE_RARELY, // FOG_SUN_CONSTS 
		MTL_UPDATE_RARELY, // FOG_SUN_COLOR_LINEAR 
		MTL_UPDATE_RARELY, // FOG_SUN_COLOR_GAMMA 
		MTL_UPDATE_RARELY, // FOG_SUN_DIR 
		MTL_UPDATE_RARELY, // GLOW_SETUP 
		MTL_UPDATE_RARELY, // GLOW_APPLY 
		MTL_UPDATE_RARELY, // COLOR_BIAS 
		MTL_UPDATE_RARELY, // COLOR_TINT_BASE 
		MTL_UPDATE_RARELY, // COLOR_TINT_DELTA 
		MTL_UPDATE_RARELY, // COLOR_TINT_QUADRATIC_DELTA 
		MTL_UPDATE_RARELY, // OUTDOOR_FEATHER_PARMS 
		MTL_UPDATE_RARELY, // ENVMAP_PARMS 
		MTL_UPDATE_RARELY, // SUN_SHADOWMAP_PIXEL_ADJUST 
		MTL_UPDATE_RARELY, // SPOT_SHADOWMAP_PIXEL_ADJUST 
		MTL_UPDATE_RARELY, // COMPOSITE_FX_DISTORTION 
		MTL_UPDATE_RARELY, // POSTFX_FADE_EFFECT 
		MTL_UPDATE_RARELY, // VIEWPORT_DIMENSIONS 
		MTL_UPDATE_RARELY, // FRAMEBUFFER_READ 
		MTL_UPDATE_PER_PRIM, // BASE_LIGHTING_COORDS 
		MTL_UPDATE_PER_PRIM, // LIGHT_PROBE_AMBIENT 
		MTL_UPDATE_RARELY, // NEARPLANE_ORG 
		MTL_UPDATE_RARELY, // NEARPLANE_DX 
		MTL_UPDATE_RARELY, // NEARPLANE_DY 
		MTL_UPDATE_RARELY, // CLIP_SPACE_LOOKUP_SCALE 
		MTL_UPDATE_RARELY, // CLIP_SPACE_LOOKUP_OFFSET 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_MATRIX0 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_MATRIX1 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_MATRIX2 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_SPARK_COLOR0 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_SPARK_COLOR1 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_SPARK_COLOR2 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_FOUNTAIN_PARM0 
		MTL_UPDATE_PER_OBJECT, // PARTICLE_FOUNTAIN_PARM1 
		MTL_UPDATE_PER_OBJECT, // DEPTH_FROM_CLIP 
		MTL_UPDATE_PER_OBJECT, // CODE_MESH_ARG_0 
		MTL_UPDATE_PER_OBJECT, // CODE_MESH_ARG_1 
		MTL_UPDATE_PER_OBJECT, // VIEW_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_VIEW_MATRIX 
		MTL_UPDATE_PER_OBJECT, // TRANSPOSE_VIEW_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_VIEW_MATRIX 
		MTL_UPDATE_PER_OBJECT, // PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // TRANSPOSE_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // VIEW_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_VIEW_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // TRANSPOSE_VIEW_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX 
		MTL_UPDATE_PER_OBJECT, // SHADOW_LOOKUP_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_SHADOW_LOOKUP_MATRIX 
		MTL_UPDATE_PER_OBJECT, // TRANSPOSE_SHADOW_LOOKUP_MATRIX 
		MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // WORLD_OUTDOOR_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX 
		MTL_UPDATE_PER_PRIM, // WORLD_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_MATRIX0 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_MATRIX0 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_MATRIX0 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_PROJECTION_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_PROJECTION_MATRIX0 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 
		MTL_UPDATE_PER_PRIM, // WORLD_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_MATRIX1 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_MATRIX1 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_MATRIX1 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_PROJECTION_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_PROJECTION_MATRIX1 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 
		MTL_UPDATE_PER_PRIM, // WORLD_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_MATRIX2 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_MATRIX2 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_MATRIX2 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2 
		MTL_UPDATE_PER_PRIM, // WORLD_VIEW_PROJECTION_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_PROJECTION_MATRIX2 
		MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 
		MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 
	};

	static_assert(ARRAYSIZE(codeConstUpdateFrequency) == CONST_SRC_TOTAL_COUNT);
	static const char* Debug_ShaderCodeConstantsNames[]
	{
		NAMEOF(CONST_SRC_CODE_LIGHT_POSITION),
		NAMEOF(CONST_SRC_CODE_LIGHT_DIFFUSE),
		NAMEOF(CONST_SRC_CODE_LIGHT_SPECULAR),
		NAMEOF(CONST_SRC_CODE_LIGHT_SPOTDIR),
		NAMEOF(CONST_SRC_CODE_LIGHT_SPOTFACTORS),
		NAMEOF(CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_COLOR),
		NAMEOF(CONST_SRC_CODE_GAMETIME),
		NAMEOF(CONST_SRC_CODE_PIXEL_COST_FRACS),
		NAMEOF(CONST_SRC_CODE_PIXEL_COST_DECODE),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_0),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_1),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_2),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_3),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_4),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_5),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_6),
		NAMEOF(CONST_SRC_CODE_FILTER_TAP_7),
		NAMEOF(CONST_SRC_CODE_COLOR_MATRIX_R),
		NAMEOF(CONST_SRC_CODE_COLOR_MATRIX_G),
		NAMEOF(CONST_SRC_CODE_COLOR_MATRIX_B),
		NAMEOF(CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET),
		NAMEOF(CONST_SRC_CODE_RENDER_TARGET_SIZE),
		NAMEOF(CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR),
		NAMEOF(CONST_SRC_CODE_DOF_EQUATION_SCENE),
		NAMEOF(CONST_SRC_CODE_DOF_LERP_SCALE),
		NAMEOF(CONST_SRC_CODE_DOF_LERP_BIAS),
		NAMEOF(CONST_SRC_CODE_DOF_ROW_DELTA),
		NAMEOF(CONST_SRC_CODE_MOTION_MATRIX_X),
		NAMEOF(CONST_SRC_CODE_MOTION_MATRIX_Y),
		NAMEOF(CONST_SRC_CODE_MOTION_MATRIX_W),
		NAMEOF(CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION),
		NAMEOF(CONST_SRC_CODE_SHADOWMAP_SCALE),
		NAMEOF(CONST_SRC_CODE_ZNEAR),
		NAMEOF(CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE),
		NAMEOF(CONST_SRC_CODE_DEBUG_BUMPMAP),
		NAMEOF(CONST_SRC_CODE_MATERIAL_COLOR),
		NAMEOF(CONST_SRC_CODE_FOG),
		NAMEOF(CONST_SRC_CODE_FOG_COLOR_LINEAR),
		NAMEOF(CONST_SRC_CODE_FOG_COLOR_GAMMA),
		NAMEOF(CONST_SRC_CODE_FOG_SUN_CONSTS),
		NAMEOF(CONST_SRC_CODE_FOG_SUN_COLOR_LINEAR),
		NAMEOF(CONST_SRC_CODE_FOG_SUN_COLOR_GAMMA),
		NAMEOF(CONST_SRC_CODE_FOG_SUN_DIR),
		NAMEOF(CONST_SRC_CODE_GLOW_SETUP),
		NAMEOF(CONST_SRC_CODE_GLOW_APPLY),
		NAMEOF(CONST_SRC_CODE_COLOR_BIAS),
		NAMEOF(CONST_SRC_CODE_COLOR_TINT_BASE),
		NAMEOF(CONST_SRC_CODE_COLOR_TINT_DELTA),
		NAMEOF(CONST_SRC_CODE_COLOR_TINT_QUADRATIC_DELTA),
		NAMEOF(CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS),
		NAMEOF(CONST_SRC_CODE_ENVMAP_PARMS),
		NAMEOF(CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST),
		NAMEOF(CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST),
		NAMEOF(CONST_SRC_CODE_COMPOSITE_FX_DISTORTION),
		NAMEOF(CONST_SRC_CODE_POSTFX_FADE_EFFECT),
		NAMEOF(CONST_SRC_CODE_VIEWPORT_DIMENSIONS),
		NAMEOF(CONST_SRC_CODE_FRAMEBUFFER_READ),
		NAMEOF(CONST_SRC_CODE_BASE_LIGHTING_COORDS),
		NAMEOF(CONST_SRC_CODE_LIGHT_PROBE_AMBIENT),
		NAMEOF(CONST_SRC_CODE_NEARPLANE_ORG),
		NAMEOF(CONST_SRC_CODE_NEARPLANE_DX),
		NAMEOF(CONST_SRC_CODE_NEARPLANE_DY),
		NAMEOF(CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE),
		NAMEOF(CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR0),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR1),
		NAMEOF(CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR2),
		NAMEOF(CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM0),
		NAMEOF(CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM1),
		NAMEOF(CONST_SRC_CODE_DEPTH_FROM_CLIP),
		NAMEOF(CONST_SRC_CODE_CODE_MESH_ARG_0),
		NAMEOF(CONST_SRC_CODE_CODE_MESH_ARG_1),
		NAMEOF(CONST_SRC_CODE_VIEW_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_VIEW_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX),
		NAMEOF(CONST_SRC_CODE_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_VIEW_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX),
		NAMEOF(CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX),
		NAMEOF(CONST_SRC_CODE_WORLD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0),
		NAMEOF(CONST_SRC_CODE_WORLD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX1),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX1),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX1),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1),
		NAMEOF(CONST_SRC_CODE_WORLD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX2),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX2),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2),
		NAMEOF(CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX2),
		NAMEOF(CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2),
		NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2)
	};

	static_assert(ARRAYSIZE(Debug_ShaderCodeConstantsNames) == ShaderCodeConstants::CONST_SRC_TOTAL_COUNT);


	struct GfxPixelShaderLoadDef
	{
		unsigned int* program;
		unsigned short programSize;
		unsigned short loadForRenderer;
	};

	struct MaterialPixelShaderProgram
	{
		void/*IDirect3DPixelShader9*/* ps;
		GfxPixelShaderLoadDef loadDef;
	};

	struct MaterialPixelShader
	{
		const char* name;
		MaterialPixelShaderProgram prog;
	};


	struct GfxVertexShaderLoadDef
	{
		unsigned int* program;
		unsigned short programSize;
		unsigned short loadForRenderer;
	};

	struct MaterialVertexShaderProgram
	{
		void* vs;
		GfxVertexShaderLoadDef loadDef;
	};

	struct MaterialVertexShader
	{
		const char* name;
		MaterialVertexShaderProgram prog;
	};

	struct MaterialArgumentCodeConst
	{
		unsigned short index;
		unsigned char firstRow;
		unsigned char rowCount;
	};

	union MaterialArgumentDef
	{
		const float(*literalConst)[4];
		MaterialArgumentCodeConst codeConst;
		unsigned int codeSampler;
		unsigned int nameHash;
	};

	struct MaterialShaderArgument
	{
		unsigned short type;
		unsigned short dest;
		MaterialArgumentDef u;
	};

	struct MaterialPass
	{
		MaterialVertexDeclaration* vertexDecl;
		MaterialVertexShader* vertexShader;
		MaterialPixelShader* pixelShader;
		unsigned char perPrimArgCount;
		unsigned char perObjArgCount;
		unsigned char stableArgCount;
		unsigned char customSamplerFlags;
		MaterialShaderArgument* args;
	};

	struct MaterialTechnique
	{
		const char* name;
		unsigned short flags;
		unsigned short passCount;
		MaterialPass passArray[1];
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		char worldVertFormat;
		bool hasBeenUploaded;
		char unused[1];
		MaterialTechniqueSet* remappedTechniqueSet;
		MaterialTechnique* techniques[48];
	};

	union FxEffectDefRef
	{
		struct FxEffectDef* handle;
		const char* name;
	};

	union FxElemVisuals
	{
		const void* anonymous;
		Material* material;
		XModel* model;
		FxEffectDefRef effectDef;
		const char* soundName;
	};

	struct FxElemMarkVisuals
	{
		Material* materials[2];
	};

	union FxElemDefVisuals
	{
		FxElemMarkVisuals* markArray;
		FxElemVisuals* array;
		FxElemVisuals instance;
	};


	struct FxTrailVertex
	{
		float pos[2];
		float normal[2];
		float texCoord;
	};

	struct FxTrailDef
	{
		int scrollTimeMsec;
		int repeatDist;
		float invSplitDist;
		float invSplitArcDist;
		float invSplitTime;
		int vertCount;
		FxTrailVertex* verts;
		int indCount;
		unsigned short* inds;
	};

	struct FxSparkFountainDef
	{
		float gravity;
		float bounceFrac;
		float bounceRand;
		float sparkSpacing;
		float sparkLength;
		int sparkCount;
		float loopTime;
		float velMin;
		float velMax;
		float velConeFrac;
		float restSpeed;
		float boostTime;
		float boostFactor;
	};

	union FxElemExtendedDefPtr
	{
		FxTrailDef* trailDef;
		FxSparkFountainDef* sparkFountainDef;
		char* unknownDef;
	};

	struct FxIntRange
	{
		int base;
		int amplitude;
	};

	struct FxSpawnDefLooping
	{
		int intervalMsec;
		int count;
	};

	struct FxSpawnDefOneShot
	{
		FxIntRange count;
	};

	union FxSpawnDef
	{
		FxSpawnDefLooping looping;
		FxSpawnDefOneShot oneShot;
	};


	struct FxFloatRange
	{
		float base;
		float amplitude;
	};

	struct FxElemAtlas
	{
		unsigned char behavior;
		unsigned char index;
		unsigned char fps;
		unsigned char loopCount;
		unsigned char colIndexBits;
		unsigned char rowIndexBits;
		short entryCount;
	};

	struct FxElemVec3Range
	{
		float base[3];
		float amplitude[3];
	};

	struct FxElemVelStateInFrame
	{
		FxElemVec3Range velocity;
		FxElemVec3Range totalDelta;
	};


	struct FxElemVelStateSample
	{
		FxElemVelStateInFrame local;
		FxElemVelStateInFrame world;
	};

	struct FxElemVisualState
	{
		unsigned char color[4];
		float rotationDelta;
		float rotationTotal;
		float size[2];
		float scale;
	};

	struct FxElemVisStateSample
	{
		FxElemVisualState base;
		FxElemVisualState amplitude;
	};
	
	struct FxElemDef
	{
		int flags;
		FxSpawnDef spawn;
		FxFloatRange spawnRange;
		FxFloatRange fadeInRange;
		FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		FxIntRange spawnDelayMsec;
		FxIntRange lifeSpanMsec;
		FxFloatRange spawnOrigin[3];
		FxFloatRange spawnOffsetRadius;
		FxFloatRange spawnOffsetHeight;
		FxFloatRange spawnAngles[3];
		FxFloatRange angularVelocity[3];
		FxFloatRange initialRotation;
		FxFloatRange gravity;
		FxFloatRange reflectionFactor;
		FxElemAtlas atlas;
		unsigned char elemType;
		unsigned char visualCount;
		unsigned char velIntervalCount;
		unsigned char visStateIntervalCount;
		FxElemVelStateSample* velSamples;
		FxElemVisStateSample* visSamples;
		FxElemDefVisuals visuals;
		Bounds collBounds;
		FxEffectDefRef effectOnImpact;
		FxEffectDefRef effectOnDeath;
		FxEffectDefRef effectEmitted;
		FxFloatRange emitDist;
		FxFloatRange emitDistVariance;
		FxElemExtendedDefPtr extended;
		unsigned char sortOrder;
		unsigned char lightingFrac;
		unsigned char useItemClip;
		unsigned char fadeInfo;
	};

	struct FxEffectDef
	{
		const char* name;
		int flags;
		int totalSize;
		int msecLoopingLife;
		int elemDefCountLooping;
		int elemDefCountOneShot;
		int elemDefCountEmission;
		FxElemDef* elemDefs;
	};

	struct FxGlassDef
	{
		float halfThickness;
		float texVecs[2][2];
		GfxColor color;
		Material* material;
		Material* materialShattered;
		PhysPreset* physPreset;
	};

	struct FxSpatialFrame
	{
		float quat[4];
		float origin[3];
	};

	union FxGlassPiecePlace
	{
#pragma warning(push)
#pragma warning(disable: 4201)
		struct
		{
			FxSpatialFrame frame;
			float radius;
		};
#pragma warning(pop)
		unsigned int nextFree;
	};

	struct FxGlassPieceState
	{
		float texCoordOrigin[2];
		unsigned int supportMask;
		unsigned short initIndex;
		unsigned short geoDataStart;
		unsigned char defIndex;
		unsigned char pad[5];
		unsigned char vertCount;
		unsigned char holeDataCount;
		unsigned char crackDataCount;
		unsigned char fanDataCount;
		unsigned short flags;
		float areaX2;
	};

	struct FxGlassPieceDynamics
	{
		int fallTime;
		int physObjId;
		int physJointId;
		float vel[3];
		float avel[3];
	};


	struct FxGlassVertex
	{
		short x;
		short y;
	};

	struct FxGlassHoleHeader
	{
		unsigned short uniqueVertCount;
		unsigned char touchVert;
		unsigned char pad[1];
	};

	struct FxGlassCrackHeader
	{
		unsigned short uniqueVertCount;
		unsigned char beginVertIndex;
		unsigned char endVertIndex;
	};

	union FxGlassGeometryData
	{
		FxGlassVertex vert;
		FxGlassHoleHeader hole;
		FxGlassCrackHeader crack;
		unsigned char asBytes[4];
		short anonymous[2];
	};

	struct FxGlassInitPieceState
	{
		FxSpatialFrame frame;
		float radius;
		float texCoordOrigin[2];
		unsigned int supportMask;
		float areaX2;
		unsigned char defIndex;
		unsigned char vertCount;
		unsigned char fanDataCount;
		unsigned char pad[1];
	};

	struct FxGlassSystem
	{
		int time;
		int prevTime;
		unsigned int defCount;
		unsigned int pieceLimit;
		unsigned int pieceWordCount;
		unsigned int initPieceCount;
		unsigned int cellCount;
		unsigned int activePieceCount;
		unsigned int firstFreePiece;
		unsigned int geoDataLimit;
		unsigned int geoDataCount;
		unsigned int initGeoDataCount;
		FxGlassDef* defs;
		FxGlassPiecePlace* piecePlaces;
		FxGlassPieceState* pieceStates;
		FxGlassPieceDynamics* pieceDynamics;
		FxGlassGeometryData* geoData;
		unsigned int* isInUse;
		unsigned int* cellBits;
		unsigned char* visData;
		float(*linkOrg)[3];
		float* halfThickness;
		unsigned __int16* lightingHandles;
		FxGlassInitPieceState* initPieceStates;
		FxGlassGeometryData* initGeoData;
		bool needToCompactData;
		char initCount;
		float effectChanceAccum;
		int lastPieceDeletionTime;
	};

	struct FxWorld
	{
		const char* name;
		FxGlassSystem glassSys;
	};



	struct _AILSOUNDINFO
	{
		int format;
		const void* data_ptr;
		unsigned int data_len;
		unsigned int rate;
		int bits;
		int channels;
		unsigned int samples;
		unsigned int block_size;
		const void* initial_ptr;
	};

	struct SndCurve
	{
		const char* filename;
		unsigned __int16 knotCount;
		float knots[16][2];
	};

	struct StreamedSound
	{
		const char* dir;
		const char* name;
	};

	struct AILSOUNDINFO
	{
		int format;
		const void* data_ptr;
		unsigned int data_len;
		unsigned int rate;
		int bits;
		int channels;
		unsigned int samples;
		unsigned int block_size;
		const void* initial_ptr;
	};

	struct MssSound
	{
		AILSOUNDINFO info;
		char* data;
	};

	struct LoadedSound
	{
		char* name;
		MssSound sound;
	};

	union SoundFileRef
	{
		LoadedSound* loadSnd;
		StreamedSound streamSnd;
	};

	struct SoundFile
	{
		unsigned char type;
		unsigned char exists;
		SoundFileRef u;
	};


	struct MSSSpeakerLevels
	{
		int speaker;
		int numLevels;
		float levels[2];
	};

	struct MSSChannelMap
	{
		int speakerCount;
		MSSSpeakerLevels speakers[6];
	};

	struct SpeakerMap
	{
		bool isDefault;
		const char* name;
		MSSChannelMap channelMaps[2][2];
	};

	const struct snd_alias_t
	{
		const char* aliasName;
		const char* subtitle;
		const char* secondaryAliasName;
		const char* chainAliasName;
		SoundFile* soundFile;
		int sequence;
		float volMin;
		float volMax;
		float pitchMin;
		float pitchMax;
		float distMin;
		float distMax;
		SoundAliasFlags flags;
		float slavePercentage;
		float probability;
		float lfePercentage;
		float centerPercentage;
		int startDelay;
		SndCurve* volumeFalloffCurve;
		float envelopMin;
		float envelopMax;
		float envelopPercentage;
		SpeakerMap* speakerMap;
	};

	struct snd_alias_list_t
	{
		const char* aliasName;
		snd_alias_t* head;
		unsigned int count;
	};


	struct G_GlassPiece
	{
		unsigned __int16 damageTaken;
		unsigned __int16 collapseTime;
		int lastStateChangeTime;
		char impactDir;
		char impactPos[2];
	};

	struct G_GlassName
	{
		char* nameStr;
		unsigned __int16 name;
		unsigned __int16 pieceCount;
		unsigned __int16* pieceIndices;
	};

	struct G_GlassData
	{
		G_GlassPiece* glassPieces;
		unsigned int pieceCount;
		unsigned __int16 damageToWeaken;
		unsigned __int16 damageToDestroy;
		unsigned int glassNameCount;
		G_GlassName* glassNames;
		char pad[108];
	};


	struct GameWorldMp
	{
		const char* name;
		G_GlassData* g_glassData;
	};

	struct RawFile
	{
		const char* name;
		unsigned int compressedLen;
		unsigned int len;
		char* buffer;
	};


	enum XAnimPartType
	{
		PART_TYPE_NO_QUAT = 0x0,
		PART_TYPE_HALF_QUAT = 0x1,
		PART_TYPE_FULL_QUAT = 0x2,
		PART_TYPE_HALF_QUAT_NO_SIZE = 0x3,
		PART_TYPE_FULL_QUAT_NO_SIZE = 0x4,
		PART_TYPE_SMALL_TRANS = 0x5,
		PART_TYPE_TRANS = 0x6,
		PART_TYPE_TRANS_NO_SIZE = 0x7,
		PART_TYPE_NO_TRANS = 0x8,
		PART_TYPE_ALL = 0x9,
		PART_TYPE_COUNT = 0xA,
	};

	union XAnimIndices
	{
		char* _1;
		unsigned short* _2;
		void* data;
	};

	struct XAnimNotifyInfo
	{
		unsigned short name;
		float time;
	};

	union XAnimDynamicFrames
	{
		char(*_1)[3];
		unsigned __int16(*_2)[3];
	};

	union XAnimDynamicIndicesTrans
	{
		char _1[1];
		unsigned short _2[1];
	};

	struct XAnimPartTransFrames
	{
		float mins[3];
		float size[3];
		XAnimDynamicFrames frames;
		XAnimDynamicIndicesTrans indices;
	};

	union XAnimPartTransData
	{
		XAnimPartTransFrames frames;
		vec3_t frame0;
	};

	struct XAnimPartTrans
	{
		unsigned short size;
		char smallTrans;
		XAnimPartTransData u;
	};

	union XAnimDynamicIndicesQuat2
	{
		char _1[1];
		unsigned short _2[1];
	};

	struct XAnimDeltaPartQuatDataFrames2
	{
		__int16(*frames)[2];
		XAnimDynamicIndicesQuat2 indices;
	};

	union XAnimDeltaPartQuatData2
	{
		XAnimDeltaPartQuatDataFrames2 frames;
		unsigned short frame0[2];
	};

	struct XAnimDeltaPartQuat2
	{
		unsigned short size;
		XAnimDeltaPartQuatData2 u;
	};

	union XAnimDynamicIndicesQuat
	{
		char _1[1];
		unsigned short _2[1];
	};

	struct XAnimDeltaPartQuatDataFrames
	{
		__int16(*frames)[4];
		XAnimDynamicIndicesQuat indices;
	};

	union XAnimDeltaPartQuatData
	{
		XAnimDeltaPartQuatDataFrames frames;
		short frame0[4];
	};

	struct XAnimDeltaPartQuat
	{
		unsigned short size;
		XAnimDeltaPartQuatData u;
	};

	struct XAnimDeltaPart
	{
		XAnimPartTrans* trans;
		XAnimDeltaPartQuat2* quat2;
		XAnimDeltaPartQuat* quat;
	};

	struct XAnimParts
	{
		const char* name;
		unsigned short dataByteCount;
		unsigned short dataShortCount;
		unsigned short dataIntCount;
		unsigned short randomDataByteCount;
		unsigned short randomDataIntCount;
		unsigned short numframes;
		char flags;
		unsigned char boneCount[10];
		char notifyCount;
		char assetType;
		bool isDefault;
		unsigned int randomDataShortCount;
		unsigned int indexCount;
		float framerate;
		float frequency;
		unsigned short* names;
		char* dataByte;
		unsigned short* dataShort;
		int* dataInt;
		unsigned short* randomDataShort;
		char* randomDataByte;
		int* randomDataInt;
		XAnimIndices indices;
		XAnimNotifyInfo* notify;
		XAnimDeltaPart* deltaPart;
	};

	union XAssetHeader
	{
		void* data;
		PhysPreset* physPreset;
		PhysCollmap* physCollmap;
		XAnimParts* parts;
		XModelSurfs* modelSurfs;
		XModel* model;
		Material* material;
		MaterialPixelShader* pixelShader;
		MaterialVertexShader* vertexShader;
		MaterialVertexDeclaration* vertexDecl;
		MaterialTechniqueSet* techniqueSet;
		GfxImage* image;
		snd_alias_list_t* sound;
		SndCurve* sndCurve;
		LoadedSound* loadSnd;
		clipMap_t* clipMap;
		ComWorld* comWorld;
		//GameWorldSp* gameWorldSp;
		GameWorldMp* gameWorldMp;
		MapEnts* mapEnts;
		FxWorld* fxWorld;
		GfxWorld* gfxWorld;
		GfxLightDef* lightDef;
		//Font_s* font;
		//MenuList* menuList;
		//menuDef_t* menu;
		//LocalizeEntry* localize;
		//WeaponCompleteDef* weapon;
		//SndDriverGlobals* sndDriverGlobals;
		FxEffectDef* fx;
		//FxImpactTable* impactFx;
		RawFile* rawfile;
		//StringTable* stringTable;
		//LeaderboardDef* leaderboardDef;
		//StructuredDataDefSet* structuredDataDefSet;
		//TracerDef* tracerDef;
		//VehicleDef* vehDef;
		//AddonMapEnts* addonMapEnts;
	};
}