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

    enum FxElemType
    {
        FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
        FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
        FX_ELEM_TYPE_TAIL = 0x2,
        FX_ELEM_TYPE_TRAIL = 0x3,
        FX_ELEM_TYPE_CLOUD = 0x4,
        FX_ELEM_TYPE_SPARK_CLOUD = 0x5,
        FX_ELEM_TYPE_SPARK_FOUNTAIN = 0x6,
        FX_ELEM_TYPE_MODEL = 0x7,
        FX_ELEM_TYPE_OMNI_LIGHT = 0x8,
        FX_ELEM_TYPE_SPOT_LIGHT = 0x9,
        FX_ELEM_TYPE_SOUND = 0xA,
        FX_ELEM_TYPE_DECAL = 0xB,
        FX_ELEM_TYPE_RUNNER = 0xC,
        FX_ELEM_TYPE_COUNT = 0xD,
        FX_ELEM_TYPE_LAST_SPRITE = 0x3,
        FX_ELEM_TYPE_LAST_DRAWN = 0x9,
    };

    enum GfxImageFileFormat
    {
        IMG_FORMAT_INVALID = 0x0,
        IMG_FORMAT_BITMAP_RGBA = 0x1,
        IMG_FORMAT_BITMAP_RGB = 0x2,
        IMG_FORMAT_BITMAP_LUMINANCE_ALPHA = 0x3,
        IMG_FORMAT_BITMAP_LUMINANCE = 0x4,
        IMG_FORMAT_BITMAP_ALPHA = 0x5,
        IMG_FORMAT_WAVELET_RGBA = 0x6,
        IMG_FORMAT_WAVELET_RGB = 0x7,
        IMG_FORMAT_WAVELET_LUMINANCE_ALPHA = 0x8,
        IMG_FORMAT_WAVELET_LUMINANCE = 0x9,
        IMG_FORMAT_WAVELET_ALPHA = 0xA,
        IMG_FORMAT_DXT1 = 0xB,
        IMG_FORMAT_DXT3 = 0xC,
        IMG_FORMAT_DXT5 = 0xD,
        IMG_FORMAT_DXN = 0xE,
        IMG_FORMAT_DXT3A_AS_LUMINANCE = 0xF,
        IMG_FORMAT_DXT5A_AS_LUMINANCE = 0x10,
        IMG_FORMAT_DXT3A_AS_ALPHA = 0x11,
        IMG_FORMAT_DXT5A_AS_ALPHA = 0x12,
        IMG_FORMAT_DXT1_AS_LUMINANCE_ALPHA = 0x13,
        IMG_FORMAT_DXN_AS_LUMINANCE_ALPHA = 0x14,
        IMG_FORMAT_DXT1_AS_LUMINANCE = 0x15,
        IMG_FORMAT_DXT1_AS_ALPHA = 0x16,
        IMG_FORMAT_COUNT = 0x17,
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
#pragma warning(disable : 4201)
        struct
        {
            uint32_t looping : 1;
            uint32_t isMaster : 1;
            uint32_t isSlave : 1;
            uint32_t fullDryLevel : 1;
            uint32_t noWetLevel : 1;
            uint32_t unknown : 1;
            uint32_t unk_is3D : 1;
            uint32_t type : 2;
            uint32_t channel : 6;
        };
#pragma warning(pop)
        uint32_t intValue;
    };

    static_assert(sizeof(SoundAliasFlags) == sizeof(uint32_t));

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
        uint8_t type;
        uint8_t canUseShadowMap;
        uint8_t exponent;
        uint8_t unused;
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
        int32_t isInUse;
        uint32_t primaryLightCount;
        ComPrimaryLight* primaryLights;
    };

    struct GfxImageFileHeader
    {
        char tag[3];
        char version;
        int32_t flags;
        char format;
        short dimensions[3];
        int32_t fileSizeForPicmip[4];
    };

    struct TriggerModel
    {
        int32_t contents;
        uint16_t hullCount;
        uint16_t firstHull;
    };

    struct Bounds
    {
        float midPoint[3];
        float halfSize[3];
    };

    struct TriggerHull
    {
        Bounds bounds;
        int32_t contents;
        uint16_t slabCount;
        uint16_t firstSlab;
    };

    struct TriggerSlab
    {
        float dir[3];
        float midPoint;
        float halfSize;
    };

    struct MapTriggers
    {
        uint32_t count;
        TriggerModel* models;
        uint32_t hullCount;
        TriggerHull* hulls;
        uint32_t slabCount;
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
        uint32_t vertexLayerData;
        uint32_t firstVertex;
        uint16_t vertexCount;
        uint16_t triCount;
        uint32_t baseIndex;
    };

    struct GfxSurfaceLightingAndFlagsFields
    {
        uint8_t lightmapIndex;
        uint8_t reflectionProbeIndex;
        uint8_t primaryLightIndex;
        uint8_t flags;
    };

    union GfxSurfaceLightingAndFlags
    {
        GfxSurfaceLightingAndFlagsFields fields;
        uint32_t packed;
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
        uint8_t recursionDepth;
        uint8_t hullPointCount;
        float (*hullPoints)[2];
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
        uint16_t cellIndex;
        uint8_t vertexCount;
        float hullAxis[2][3];
    };

    struct GfxCell
    {
        Bounds bounds;
        int32_t portalCount;
        GfxPortal* portals;
        uint8_t reflectionProbeCount;
        uint8_t* reflectionProbes;
    };

    union PackedUnitVec
    {
        uint32_t packed;
        char array[4];
    };

    union GfxColor
    {
        uint32_t packed;
        uint8_t array[4];
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
        uint16_t cullDist;
        uint16_t lightingHandle;
        uint8_t reflectionProbeIndex;
        uint8_t primaryLightIndex;
        uint8_t flags;
        uint8_t firstMtlSkinIndex;
        GfxColor groundLighting;
        uint16_t cacheId[4];
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
        int32_t platform[2];
    };

    struct GfxImageLoadDef
    {
        char levelCount;
        char pad[3];
        int32_t flags;
        int32_t format;
        int32_t resourceSize;
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

    enum GfxImageCategory : uint8_t
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
        uint8_t mapType;
        GfxImageCategory semantic;
        uint8_t category;
        bool useSrgbReads;
        Picmip picmip;
        bool noPicmip;
        char track;
        CardMemory cardMemory;
        uint16_t width;
        uint16_t height;
        uint16_t depth;
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
        uint8_t samplerState;
    };

    struct GfxLightDef
    {
        const char* name;
        GfxLightImage attenuation;
        int32_t lmapLookupStart;
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
        uint8_t index;
    };

    struct GfxWorldVertexData
    {
        GfxWorldVertex* vertices;
        void* worldVb;
    };

    struct GfxWorldVertexLayerData
    {
        uint8_t* data;
        void* layerVb;
    };

    struct GfxWorldDraw
    {
        uint32_t reflectionProbeCount;
        GfxImage** reflectionProbes;
        GfxReflectionProbe* reflectionProbeOrigins;
        GfxTexture* reflectionProbeTextures;
        int32_t lightmapCount;
        GfxLightmapArray* lightmaps;
        GfxTexture* lightmapPrimaryTextures;
        GfxTexture* lightmapSecondaryTextures;
        GfxImage* lightmapOverridePrimary;
        GfxImage* lightmapOverrideSecondary;
        uint32_t vertexCount;
        GfxWorldVertexData vd;
        uint32_t vertexLayerDataSize;
        GfxWorldVertexLayerData vld;
        uint32_t indexCount;
        uint16_t* indices;
    };

    struct GfxSky
    {
        int32_t skySurfCount;
        int* skyStartSurfs;
        GfxImage* skyImage;
        int32_t skySamplerState;
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
        int32_t exponent;
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
        uint64_t unused : 1;
        uint64_t primarySortKey : 6;
        uint64_t surfType : 4;
        uint64_t viewModelRender : 1;
        uint64_t sceneLightIndex : 8;
        uint64_t useHeroLighting : 1;
        uint64_t prepass : 2;
        uint64_t materialSortedIndex : 12;
        uint64_t customIndex : 5;
        uint64_t hasGfxEntIndex : 1;
        uint64_t reflectionProbeIndex : 8;
        uint64_t objectId : 15;
    };

    union GfxDrawSurf
    {
        GfxDrawSurfFields fields;
        uint64_t packed;
    };

    struct GfxWorldDpvsStatic
    {
        uint32_t smodelCount;
        uint32_t staticSurfaceCount;
        uint32_t staticSurfaceCountNoDecal;
        uint32_t litOpaqueSurfsBegin;
        uint32_t litOpaqueSurfsEnd;
        uint32_t litTransSurfsBegin;
        uint32_t litTransSurfsEnd;
        uint32_t shadowCasterSurfsBegin;
        uint32_t shadowCasterSurfsEnd;
        uint32_t emissiveSurfsBegin;
        uint32_t emissiveSurfsEnd;
        uint32_t smodelVisDataCount;
        uint32_t surfaceVisDataCount;
        char* smodelVisData[3];
        char* surfaceVisData[3];
        uint16_t* sortedSurfIndex;
        GfxStaticModelInst* smodelInsts;
        GfxSurface* surfaces;
        GfxSurfaceBounds* surfacesBounds;
        GfxStaticModelDrawInst* smodelDrawInsts;
        GfxDrawSurf* surfaceMaterials;
        uint32_t* surfaceCastsSunShadow;
        volatile int usageCount;
    };

    struct GfxLightGridEntry
    {
        uint16_t colorsIndex;
        uint8_t primaryLightIndex;
        uint8_t needsTrace;
    };

    struct GfxLightGridColors
    {
        uint8_t rgb[56][3];
    };

    struct GfxLightGrid
    {
        bool hasLightRegions;
        uint32_t lastSunPrimaryLightIndex;
        uint16_t mins[3];
        uint16_t maxs[3];
        uint32_t rowAxis;
        uint32_t colAxis;
        uint16_t* rowDataStart;
        uint32_t rawRowDataSize;
        uint8_t* rawRowData;
        uint32_t entryCount;
        GfxLightGridEntry* entries;
        uint32_t colorCount;
        GfxLightGridColors* colors;
    };

    struct MaterialMemory
    {
        Material* material;
        int32_t memory;
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
        int32_t flareFadeInTime;
        int32_t flareFadeOutTime;
        float blindMinDot;
        float blindMaxDot;
        float blindMaxDarken;
        int32_t blindFadeInTime;
        int32_t blindFadeOutTime;
        float glareMinDot;
        float glareMaxDot;
        float glareMaxLighten;
        int32_t glareFadeInTime;
        int32_t glareFadeOutTime;
        float sunFxPosition[3];
    };

    struct cplane_s
    {
        float normal[3];
        float dist;
        uint8_t type;
        uint8_t pad[3];
    };

    struct cbrushside_t
    {
        cplane_s* plane;
        uint16_t materialNum;
        uint8_t firstAdjacentSideOffset;
        uint8_t edgeCount;
    };

    struct cbrushWrapper_t
    {
        uint16_t numsides;
        uint16_t glassPieceIndex;
        cbrushside_t* sides;
        uint8_t* baseAdjacentSide;
        short axialMaterialNum[2][3];
        uint8_t firstAdjacentSideOffsets[2][3];
        uint8_t edgeCount[2][3];
    };

    struct BrushWrapper
    {
        Bounds bounds;
        cbrushWrapper_t brush;
        int32_t totalEdgeCount;
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
        uint32_t count;
        PhysGeomInfo* geoms;
        PhysMass mass;
        Bounds bounds;
    };

    struct GfxWorldDpvsPlanes
    {
        int32_t cellCount;
        cplane_s* planes;
        uint16_t* nodes;
        uint32_t* sceneEntCellBits;
    };

    struct GfxAabbTree
    {
        Bounds bounds;
        uint16_t childCount;
        uint16_t surfaceCount;
        uint16_t startSurfIndex;
        uint16_t surfaceCountNoDecal;
        uint16_t startSurfIndexNoDecal;
        uint16_t smodelIndexCount;
        uint16_t* smodelIndexes;
        int32_t childrenOffset;
    };

    struct GfxCellTree
    {
        GfxAabbTree* aabbTree;
    };

    struct GfxCellTreeCount
    {
        int32_t aabbTreeCount;
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
        uint16_t surfaceCount;
        uint16_t startSurfIndex;
        uint16_t surfaceCountNoDecal;
    };

    struct BModelDrawInfo
    {
        uint16_t surfId;
    };

    struct GfxSceneDynBrush
    {
        BModelDrawInfo info;
        uint16_t dynEntId;
    };

    struct XModelDrawInfo
    {
        uint8_t hasGfxEntIndex;
        uint8_t lod;
        uint16_t surfId;
    };

    struct GfxSceneDynModel
    {
        XModelDrawInfo info;
        uint16_t dynEntId;
    };

    struct GfxShadowGeometry
    {
        uint16_t surfaceCount;
        uint16_t smodelCount;
        uint16_t* sortedSurfIndex;
        uint16_t* smodelIndex;
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
        uint32_t axisCount;
        GfxLightRegionAxis* axis;
    };

    struct GfxLightRegion
    {
        uint32_t hullCount;
        GfxLightRegionHull* hulls;
    };

    struct GfxWorldDpvsDynamic
    {
        uint32_t dynEntClientWordCount[2];
        uint32_t dynEntClientCount[2];
        uint32_t* dynEntCellBits[2];
        char* dynEntVisData[2][3];
    };

    struct GfxWorld
    {
        const char* name;
        const char* baseName;
        uint32_t planeCount;
        uint32_t nodeCount;
        uint32_t surfaceCount;
        int32_t skyCount;
        GfxSky* skies;
        uint32_t lastSunPrimaryLightIndex;
        uint32_t primaryLightCount;
        uint32_t sortKeyLitDecal;
        uint32_t sortKeyEffectDecal;
        uint32_t sortKeyEffectAuto;
        uint32_t sortKeyDistortion;
        GfxWorldDpvsPlanes dpvsPlanes;
        GfxCellTreeCount* aabbTreeCounts;
        GfxCellTree* aabbTrees;
        GfxCell* cells;
        GfxWorldDraw draw;
        GfxLightGrid lightGrid;
        int32_t modelCount;
        GfxBrushModel* models;
        Bounds bounds;
        uint32_t checksum;
        int32_t materialMemoryCount;
        MaterialMemory* materialMemory;
        sunflare_t sun;
        float outdoorLookupMatrix[4][4];
        GfxImage* outdoorImage;
        uint32_t* cellCasterBits;
        uint32_t* cellHasSunLitSurfsBits;
        GfxSceneDynModel* sceneDynModel;
        GfxSceneDynBrush* sceneDynBrush;
        uint32_t* primaryLightEntityShadowVis;
        uint32_t* primaryLightDynEntShadowVis[2];
        uint8_t* nonSunPrimaryLightForModelDynEnt;
        GfxShadowGeometry* shadowGeom;
        GfxLightRegion* lightRegion;
        GfxWorldDpvsStatic dpvs;
        GfxWorldDpvsDynamic dpvsDyn;
        uint32_t mapVtxChecksum;
        uint32_t heroOnlyLightCount;
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
        uint32_t packed;
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
        uint16_t mins[3];
        uint16_t maxs[3];
    };

    struct XSurfaceCollisionNode
    {
        XSurfaceCollisionAabb aabb;
        uint16_t childBeginIndex;
        uint16_t childCount;
    };

    struct XSurfaceCollisionLeaf
    {
        uint16_t triangleBeginIndex;
    };

    struct XSurfaceCollisionTree
    {
        float trans[3];
        float scale[3];
        uint32_t nodeCount;
        XSurfaceCollisionNode* nodes;
        uint32_t leafCount;
        XSurfaceCollisionLeaf* leafs;
    };

    struct XRigidVertList
    {
        uint16_t boneOffset;
        uint16_t vertCount;
        uint16_t triOffset;
        uint16_t triCount;
        XSurfaceCollisionTree* collisionTree;
    };

    struct XSurfaceVertexInfo
    {
        short vertCount[4];
        uint16_t* vertsBlend;
    };

    struct XSurface
    {
        uint8_t tileMode;
        bool deformed;
        uint16_t vertCount;
        uint16_t triCount;
        char zoneHandle;
        uint16_t baseTriIndex;
        uint16_t baseVertIndex;
        uint16_t* triIndices;
        XSurfaceVertexInfo vertInfo;
        GfxPackedVertex* verts0;
        uint32_t vertListCount;
        XRigidVertList* vertList;
        int32_t partBits[6];
    };

    struct XModelSurfs
    {
        const char* name;
        XSurface* surfaces;
        int32_t numSurfaces;
        int32_t partBits[6];
    };

    struct XModelLodInfo
    {
        float dist;
        uint16_t numsurfs;
        uint16_t surfIndex;
        XModelSurfs* modelSurfs;
        int32_t partBits[6];
        XSurface* surfs;
        char lod;
        char smcBaseIndexPlusOne;
        char smcSubIndexMask;
        char smcBucket;
    };

    struct PhysPreset
    {
        const char* name;
        int32_t type;
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
        int32_t numCollTris;
        Bounds bounds;
        int32_t boneIdx;
        int32_t contents;
        int32_t surfFlags;
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
        uint8_t numBones;
        uint8_t numRootBones;
        uint8_t numsurfs;
        char lodRampType;
        float scale;
        uint32_t noScalePartBits[6];
        uint16_t* boneNames;
        uint8_t* parentList;
        short (*quats)[4];
        float (*trans)[3];
        uint8_t* partClassification;
        DObjAnimMat* baseMat;
        Material** materialHandles;
        XModelLodInfo lodInfo[4];
        char maxLoadedLod;
        char numLods;
        char collLod;
        uint8_t flags;
        XModelCollSurf_s* collSurfs;
        int32_t numCollSurfs;
        int32_t contents;
        XBoneInfo* boneInfo;
        float radius;
        Bounds bounds;
        int32_t memUsage;
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
        int32_t surfaceFlags;
        int32_t contentFlags;
    };

    struct cLeafBrushNodeLeaf_t
    {
        uint16_t* brushes;
    };

    struct cLeafBrushNode_t
    {
        char axis;
        int16_t leafBrushCount;
        int32_t contents;
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
        uint16_t triggerIndex;
        uint8_t sunPrimaryLightIndex;
    };

    struct MapEnts
    {
        const char* name;
        char* entityString;
        int32_t numEntityChars;
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
        uint16_t brushModel;
        uint16_t physicsBrushModel;
        struct FxEffectDef* destroyFx;
        PhysPreset* physPreset;
        int32_t health;
        PhysMass mass;
        int32_t contents;
    };

    struct DynEntityClient
    {
        int32_t physObjId;
        uint16_t flags;
        uint16_t lightingHandle;
        int32_t health;
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
        uint16_t firstCollAabbIndex;
        uint16_t collAabbCount;
        int32_t brushContents;
        int32_t terrainContents;
        Bounds bounds;
        int32_t leafBrushNode;
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
        int32_t surfaceFlags;
        int32_t contents;
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
        uint16_t childOffset[2];
    };

    union cLeafBrushNodeData_t
    {
        cLeafBrushNodeLeaf_t leaf;
        cLeafBrushNodeChildren_t children;
    };

    struct cLeafBrushNode_s
    {
        uint8_t axis;
        short leafBrushCount;
        int32_t contents;
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
        uint8_t triCount;
        uint8_t borderCount;
        uint8_t firstVertSegment;
        int32_t firstTri;
        CollisionBorder* borders;
    };

    struct CollisionAabbTree
    {
        float midPoint[3];
        uint16_t materialIndex;
        uint16_t childCount;
        float halfSize[3];
        uint32_t u;
    };

    struct cbrush_t
    {
        uint16_t numsides;
        uint16_t glassPieceIndex;
        cbrushside_t* sides;
        uint8_t* baseAdjacentSide;
        uint16_t axialMaterialNum[2][3];
        uint8_t firstAdjacentSideOffsets[2][3];
        uint8_t edgeCount[2][3];
    };

    struct SModelAabbNode
    {
        Bounds bounds;
        uint16_t firstChild;
        uint16_t childCount;
    };

    struct DynEntityPose
    {
        GfxPlacement pose;
        float radius;
    };

    struct DynEntityColl
    {
        uint16_t sector;
        uint16_t nextEntInSector;
        float linkMins[2];
        float linkMaxs[2];
    };

    struct clipMap_t
    {
        const char* name;
        int32_t isInUse;
        uint32_t planeCount;
        cplane_s* planes;
        uint32_t numStaticModels;
        cStaticModel_s* staticModelList;
        uint32_t numMaterials;
        ClipMaterial* materials;
        uint32_t numBrushSides;
        cbrushside_t* brushsides;
        uint32_t numBrushEdges;
        uint8_t* brushEdges;
        uint32_t numNodes;
        cNode_t* nodes;
        uint32_t numLeafs;
        cLeaf_t* leafs;
        uint32_t leafbrushNodesCount;
        cLeafBrushNode_s* leafbrushNodes;
        uint32_t numLeafBrushes;
        uint16_t* leafbrushes;
        uint32_t numLeafSurfaces;
        uint32_t* leafsurfaces;
        uint32_t vertCount;
        float (*verts)[3];
        uint32_t triCount;
        uint16_t* triIndices;
        uint8_t* triEdgeIsWalkable;
        uint32_t borderCount;
        CollisionBorder* borders;
        uint32_t partitionCount;
        CollisionPartition* partitions;
        uint32_t aabbTreeCount;
        CollisionAabbTree* aabbTrees;
        uint32_t numSubModels;
        cmodel_t* cmodels;
        uint16_t numBrushes;
        cbrush_t* brushes;
        Bounds* brushBounds;
        int* brushContents;
        MapEnts* mapEnts;
        uint16_t smodelNodeCount;
        SModelAabbNode* smodelNodes;
        uint16_t dynEntCount[2];
        DynEntityDef* dynEntDefList[2];
        DynEntityPose* dynEntPoseList[2];
        DynEntityClient* dynEntClientList[2];
        DynEntityColl* dynEntCollList[2];
        uint32_t checksum;
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
    enum MaterialTextureSource : uint32_t
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

    static const char* Debug_MaterialTextureSourceNames[]{NAMEOF(TEXTURE_SRC_CODE_BLACK),
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
                                                          NAMEOF(TEXTURE_SRC_CODE_ALTERNATE_SCENE)};

    static_assert(ARRAYSIZE(Debug_MaterialTextureSourceNames) == MaterialTextureSource::TEXTURE_SRC_CODE_COUNT);

    enum StateFlags : uint8_t
    {
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
        uint8_t unk1 : 1; // 0x1
        uint8_t addShadowToPrimaryLight : 1; // 0x2
        uint8_t isFoliageRequiresGroundLighting : 1; // 0x4
        uint8_t unk4 : 1; // 0x8
        uint8_t unk5 : 1; // 0x10
        uint8_t unk6 : 1; // 0x20
        uint8_t unk7 : 1; // 0x40
        uint8_t unkCastShadowMaybe : 1; // 0x80
    };

    union MaterialGameFlags
    {
        MaterialGameFlagsFields fields;
        uint8_t packed;
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
        int32_t M;
        int32_t N;
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
        uint32_t nameHash;
        char nameStart;
        char nameEnd;
        uint8_t samplerState;
        uint8_t semantic;
        MaterialTextureDefInfo u;
    };

    struct MaterialConstantDef
    {
        uint32_t nameHash;
        char name[12];
        float literal[4];
    };

    enum GfxSurfaceStatebitOp0 : uint32_t
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

    enum GfxSurfaceStatebitOp1 : uint32_t
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

    struct GfxStatebitsFlags
    {
        GfxSurfaceStatebitOp0 loadbit0;
        GfxSurfaceStatebitOp1 loadbit1;
    };

    union GfxStateBits
    {
        GfxStatebitsFlags flags;
        uint32_t loadBits[2];
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

    static const char* Debug_MaterialShaderArgumentTypeName[]{NAMEOF(MTL_ARG_MATERIAL_VERTEX_CONST),
                                                              NAMEOF(MTL_ARG_LITERAL_VERTEX_CONST),
                                                              NAMEOF(MTL_ARG_MATERIAL_PIXEL_SAMPLER),

                                                              NAMEOF(MTL_ARG_CODE_VERTEX_CONST = 0x3),
                                                              NAMEOF(MTL_ARG_CODE_PIXEL_SAMPLER = 0x4),
                                                              NAMEOF(MTL_ARG_CODE_PIXEL_CONST = 0x5),

                                                              NAMEOF(MTL_ARG_MATERIAL_PIXEL_CONST = 0x6),
                                                              NAMEOF(MTL_ARG_LITERAL_PIXEL_CONST = 0x7)};

    static_assert(ARRAYSIZE(Debug_MaterialShaderArgumentTypeName) == MaterialShaderArgumentType::MTL_ARG_COUNT);

    enum MaterialUpdateFrequency
    {
        MTL_UPDATE_PER_PRIM = 0x0,
        MTL_UPDATE_PER_OBJECT = 0x1,
        MTL_UPDATE_RARELY = 0x2,
        MTL_UPDATE_CUSTOM = 0x3,
    };

    static const MaterialUpdateFrequency codeSamplerUpdateFrequency[]{
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
        uint8_t gameFlags;
        uint8_t sortKey;
        uint8_t textureAtlasRowCount;
        uint8_t textureAtlasColumnCount;
        GfxDrawSurf drawSurf;
        uint32_t surfaceTypeBits;
        uint16_t hashIndex;
    };

    struct Material
    {
        MaterialInfo info;
        uint8_t stateBitsEntry[48];
        uint8_t textureCount;
        uint8_t constantCount;
        uint8_t stateBitsCount;
        uint8_t stateFlags;
        uint8_t cameraRegion; // GfxCameraRegionType
        struct MaterialTechniqueSet* techniqueSet;
        struct MaterialTextureDef* textureTable;
        MaterialConstantDef* constantTable;
        GfxStateBits* stateBitsTable;
    };

    enum MaterialStreamRoutingSource : uint8_t
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

    enum MaterialStreamRoutingDestination : uint8_t
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

    enum ShaderCodeConstants : uint16_t
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

    static const MaterialUpdateFrequency codeConstUpdateFrequency[]{
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
    static const char* Debug_ShaderCodeConstantsNames[]{NAMEOF(CONST_SRC_CODE_LIGHT_POSITION),
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
                                                        NAMEOF(CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2)};

    static_assert(ARRAYSIZE(Debug_ShaderCodeConstantsNames) == ShaderCodeConstants::CONST_SRC_TOTAL_COUNT);

    struct GfxPixelShaderLoadDef
    {
        uint32_t* program;
        uint16_t programSize;
        uint16_t loadForRenderer;
    };

    struct MaterialPixelShaderProgram
    {
        void /*IDirect3DPixelShader9*/* ps;
        GfxPixelShaderLoadDef loadDef;
    };

    struct MaterialPixelShader
    {
        const char* name;
        MaterialPixelShaderProgram prog;
    };

    struct GfxVertexShaderLoadDef
    {
        uint32_t* program;
        uint16_t programSize;
        uint16_t loadForRenderer;
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
        uint16_t index;
        uint8_t firstRow;
        uint8_t rowCount;
    };

    union MaterialArgumentDef
    {
        float (*literalConst)[4];
        MaterialArgumentCodeConst codeConst;
        uint32_t codeSampler;
        uint32_t nameHash;
    };

    struct MaterialShaderArgument
    {
        uint16_t type;
        uint16_t dest;
        MaterialArgumentDef u;
    };

    struct MaterialPass
    {
        MaterialVertexDeclaration* vertexDecl;
        MaterialVertexShader* vertexShader;
        MaterialPixelShader* pixelShader;
        uint8_t perPrimArgCount;
        uint8_t perObjArgCount;
        uint8_t stableArgCount;
        uint8_t customSamplerFlags;
        MaterialShaderArgument* args;
    };

    struct MaterialTechnique
    {
        const char* name;
        uint16_t flags;
        uint16_t passCount;
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
        int32_t scrollTimeMsec;
        int32_t repeatDist;
        float invSplitDist;
        float invSplitArcDist;
        float invSplitTime;
        int32_t vertCount;
        FxTrailVertex* verts;
        int32_t indCount;
        uint16_t* inds;
    };

    struct FxSparkFountainDef
    {
        float gravity;
        float bounceFrac;
        float bounceRand;
        float sparkSpacing;
        float sparkLength;
        int32_t sparkCount;
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
        int32_t base;
        int32_t amplitude;
    };

    struct FxSpawnDefLooping
    {
        int32_t intervalMsec;
        int32_t count;
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
        uint8_t behavior;
        uint8_t index;
        uint8_t fps;
        uint8_t loopCount;
        uint8_t colIndexBits;
        uint8_t rowIndexBits;
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
        uint8_t color[4];
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
        int32_t flags;
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
        uint8_t elemType;
        uint8_t visualCount;
        uint8_t velIntervalCount;
        uint8_t visStateIntervalCount;
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
        uint8_t sortOrder;
        uint8_t lightingFrac;
        uint8_t useItemClip;
        uint8_t fadeInfo;
    };

    struct FxEffectDef
    {
        const char* name;
        int32_t flags;
        int32_t totalSize;
        int32_t msecLoopingLife;
        int32_t elemDefCountLooping;
        int32_t elemDefCountOneShot;
        int32_t elemDefCountEmission;
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
#pragma warning(disable : 4201)
        struct
        {
            FxSpatialFrame frame;
            float radius;
        };
#pragma warning(pop)
        uint32_t nextFree;
    };

    struct FxGlassPieceState
    {
        float texCoordOrigin[2];
        uint32_t supportMask;
        uint16_t initIndex;
        uint16_t geoDataStart;
        uint8_t defIndex;
        uint8_t pad[5];
        uint8_t vertCount;
        uint8_t holeDataCount;
        uint8_t crackDataCount;
        uint8_t fanDataCount;
        uint16_t flags;
        float areaX2;
    };

    struct FxGlassPieceDynamics
    {
        int32_t fallTime;
        int32_t physObjId;
        int32_t physJointId;
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
        uint16_t uniqueVertCount;
        uint8_t touchVert;
        uint8_t pad[1];
    };

    struct FxGlassCrackHeader
    {
        uint16_t uniqueVertCount;
        uint8_t beginVertIndex;
        uint8_t endVertIndex;
    };

    union FxGlassGeometryData
    {
        FxGlassVertex vert;
        FxGlassHoleHeader hole;
        FxGlassCrackHeader crack;
        uint8_t asBytes[4];
        short anonymous[2];
    };

    struct FxGlassInitPieceState
    {
        FxSpatialFrame frame;
        float radius;
        float texCoordOrigin[2];
        uint32_t supportMask;
        float areaX2;
        uint8_t defIndex;
        uint8_t vertCount;
        uint8_t fanDataCount;
        uint8_t pad[1];
    };

    struct FxGlassSystem
    {
        int32_t time;
        int32_t prevTime;
        uint32_t defCount;
        uint32_t pieceLimit;
        uint32_t pieceWordCount;
        uint32_t initPieceCount;
        uint32_t cellCount;
        uint32_t activePieceCount;
        uint32_t firstFreePiece;
        uint32_t geoDataLimit;
        uint32_t geoDataCount;
        uint32_t initGeoDataCount;
        FxGlassDef* defs;
        FxGlassPiecePlace* piecePlaces;
        FxGlassPieceState* pieceStates;
        FxGlassPieceDynamics* pieceDynamics;
        FxGlassGeometryData* geoData;
        uint32_t* isInUse;
        uint32_t* cellBits;
        uint8_t* visData;
        float (*linkOrg)[3];
        float* halfThickness;
        uint16_t* lightingHandles;
        FxGlassInitPieceState* initPieceStates;
        FxGlassGeometryData* initGeoData;
        bool needToCompactData;
        char initCount;
        float effectChanceAccum;
        int32_t lastPieceDeletionTime;
    };

    struct FxWorld
    {
        const char* name;
        FxGlassSystem glassSys;
    };

    struct _AILSOUNDINFO
    {
        int32_t format;
        const void* data_ptr;
        uint32_t data_len;
        uint32_t rate;
        int32_t bits;
        int32_t channels;
        uint32_t samples;
        uint32_t block_size;
        const void* initial_ptr;
    };

    struct SndCurve
    {
        const char* filename;
        uint16_t knotCount;
        float knots[16][2];
    };

    struct StreamedSound
    {
        const char* dir;
        const char* name;
    };

    struct AILSOUNDINFO
    {
        int32_t format;
        const void* data_ptr;
        uint32_t data_len;
        uint32_t rate;
        int32_t bits;
        int32_t channels;
        uint32_t samples;
        uint32_t block_size;
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
        uint8_t type;
        uint8_t exists;
        SoundFileRef u;
    };

    struct MSSSpeakerLevels
    {
        int32_t speaker;
        int32_t numLevels;
        float levels[2];
    };

    struct MSSChannelMap
    {
        int32_t speakerCount;
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
        const char* mixerGroup;
        SoundFile* soundFile;
        int32_t sequence;
        float volMin;
        float volMax;
        float pitchMin;
        float pitchMax;
        float distMin;
        float distMax;
        float velocityMin;
        SoundAliasFlags flags;
        float slavePercentage;
        float probability;
        float lfePercentage;
        float centerPercentage;
        int32_t startDelay;
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
        uint32_t count;
    };

    struct G_GlassPiece
    {
        uint16_t damageTaken;
        uint16_t collapseTime;
        int32_t lastStateChangeTime;
        char impactDir;
        char impactPos[2];
    };

    struct G_GlassName
    {
        char* nameStr;
        uint16_t name;
        uint16_t pieceCount;
        uint16_t* pieceIndices;
    };

    struct G_GlassData
    {
        G_GlassPiece* glassPieces;
        uint32_t pieceCount;
        uint16_t damageToWeaken;
        uint16_t damageToDestroy;
        uint32_t glassNameCount;
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
        uint32_t compressedLen;
        uint32_t len;
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
        uint16_t* _2;
        void* data;
    };

    struct XAnimNotifyInfo
    {
        uint16_t name;
        float time;
    };

    union XAnimDynamicFrames
    {
        char (*_1)[3];
        uint16_t (*_2)[3];
    };

    union XAnimDynamicIndicesTrans
    {
        char _1[1];
        uint16_t _2[1];
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
        uint16_t size;
        char smallTrans;
        XAnimPartTransData u;
    };

    union XAnimDynamicIndicesQuat2
    {
        char _1[1];
        uint16_t _2[1];
    };

    struct XAnimDeltaPartQuatDataFrames2
    {
        int16_t (*frames)[2];
        XAnimDynamicIndicesQuat2 indices;
    };

    union XAnimDeltaPartQuatData2
    {
        XAnimDeltaPartQuatDataFrames2 frames;
        uint16_t frame0[2];
    };

    struct XAnimDeltaPartQuat2
    {
        uint16_t size;
        XAnimDeltaPartQuatData2 u;
    };

    union XAnimDynamicIndicesQuat
    {
        char _1[1];
        uint16_t _2[1];
    };

    struct XAnimDeltaPartQuatDataFrames
    {
        int16_t (*frames)[4];
        XAnimDynamicIndicesQuat indices;
    };

    union XAnimDeltaPartQuatData
    {
        XAnimDeltaPartQuatDataFrames frames;
        short frame0[4];
    };

    struct XAnimDeltaPartQuat
    {
        uint16_t size;
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
        uint16_t dataByteCount;
        uint16_t dataShortCount;
        uint16_t dataIntCount;
        uint16_t randomDataByteCount;
        uint16_t randomDataIntCount;
        uint16_t numframes;
        char flags;
        uint8_t boneCount[10];
        uint8_t notifyCount;
        char assetType;
        bool isDefault;
        uint32_t randomDataShortCount;
        uint32_t indexCount;
        float framerate;
        float frequency;
        uint16_t* names;
        char* dataByte;
        uint16_t* dataShort;
        int* dataInt;
        uint16_t* randomDataShort;
        char* randomDataByte;
        int* randomDataInt;
        XAnimIndices indices;
        XAnimNotifyInfo* notify;
        XAnimDeltaPart* deltaPart;
    };

    struct TracerDef
    {
        const char* name;
        Material* material;
        unsigned int drawInterval;
        float speed;
        float beamLength;
        float beamWidth;
        float screwRadius;
        float screwDist;
        float colors[5][4];
    };

    enum ImpactType
    {
        IMPACT_TYPE_NONE = 0x0,
        IMPACT_TYPE_BULLET_SMALL = 0x1,
        IMPACT_TYPE_BULLET_LARGE = 0x2,
        IMPACT_TYPE_BULLET_AP = 0x3,
        IMPACT_TYPE_BULLET_EXPLODE = 0x4,
        IMPACT_TYPE_SHOTGUN = 0x5,
        IMPACT_TYPE_SHOTGUN_EXPLODE = 0x6,
        IMPACT_TYPE_GRENADE_BOUNCE = 0x7,
        IMPACT_TYPE_GRENADE_EXPLODE = 0x8,
        IMPACT_TYPE_ROCKET_EXPLODE = 0x9,
        IMPACT_TYPE_PROJECTILE_DUD = 0xA,
        IMPACT_TYPE_COUNT = 0xB,
    };

    enum WeapStickinessType
    {
        WEAPSTICKINESS_NONE = 0x0,
        WEAPSTICKINESS_ALL = 0x1,
        WEAPSTICKINESS_ALL_ORIENT = 0x2,
        WEAPSTICKINESS_GROUND = 0x3,
        WEAPSTICKINESS_GROUND_WITH_YAW = 0x4,
        WEAPSTICKINESS_KNIFE = 0x5,
        WEAPSTICKINESS_COUNT = 0x6,
    };

    enum weapType_t
    {
        WEAPTYPE_BULLET = 0x0,
        WEAPTYPE_GRENADE = 0x1,
        WEAPTYPE_PROJECTILE = 0x2,
        WEAPTYPE_RIOTSHIELD = 0x3,
        WEAPTYPE_NUM = 0x4,
    };

	enum weapClass_t
    {
        WEAPCLASS_RIFLE = 0x0,
        WEAPCLASS_SNIPER = 0x1,
        WEAPCLASS_MG = 0x2,
        WEAPCLASS_SMG = 0x3,
        WEAPCLASS_SPREAD = 0x4,
        WEAPCLASS_PISTOL = 0x5,
        WEAPCLASS_GRENADE = 0x6,
        WEAPCLASS_ROCKETLAUNCHER = 0x7,
        WEAPCLASS_TURRET = 0x8,
        WEAPCLASS_THROWINGKNIFE = 0x9,
        WEAPCLASS_NON_PLAYER = 0xA,
        WEAPCLASS_ITEM = 0xB,
        WEAPCLASS_NUM = 0xC,
    };

    enum PenetrateType
    {
        PENETRATE_TYPE_NONE = 0x0,
        PENETRATE_TYPE_SMALL = 0x1,
        PENETRATE_TYPE_MEDIUM = 0x2,
        PENETRATE_TYPE_LARGE = 0x3,
        PENETRATE_TYPE_COUNT = 0x4,
    };

    enum weapInventoryType_t
    {
        WEAPINVENTORY_PRIMARY = 0x0,
        WEAPINVENTORY_OFFHAND = 0x1,
        WEAPINVENTORY_ITEM = 0x2,
        WEAPINVENTORY_ALTMODE = 0x3,
        WEAPINVENTORY_EXCLUSIVE = 0x4,
        WEAPINVENTORY_SCAVENGER = 0x5,
        WEAPINVENTORYCOUNT = 0x6,
    };

    enum weapFireType_t
    {
        WEAPON_FIRETYPE_FULLAUTO = 0x0,
        WEAPON_FIRETYPE_SINGLESHOT = 0x1,
        WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
        WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
        WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
        WEAPON_FIRETYPE_DOUBLEBARREL = 0x5,
        WEAPON_FIRETYPECOUNT = 0x6,
        WEAPON_FIRETYPE_BURSTFIRE_FIRST = 0x2,
        WEAPON_FIRETYPE_BURSTFIRE_LAST = 0x4,
    };

    enum weapAnimFiles_t
    {
        WEAP_ANIM_ROOT = 0x0,
        WEAP_ANIM_IDLE = 0x1,
        WEAP_ANIM_EMPTY_IDLE = 0x2,
        WEAP_ANIM_FIRE = 0x3,
        WEAP_ANIM_HOLD_FIRE = 0x4,
        WEAP_ANIM_LASTSHOT = 0x5,
        WEAP_ANIM_RECHAMBER = 0x6,
        WEAP_ANIM_MELEE = 0x7,
        WEAP_ANIM_MELEE_CHARGE = 0x8,
        WEAP_ANIM_RELOAD = 0x9,
        WEAP_ANIM_RELOAD_EMPTY = 0xA,
        WEAP_ANIM_RELOAD_START = 0xB,
        WEAP_ANIM_RELOAD_END = 0xC,
        WEAP_ANIM_RAISE = 0xD,
        WEAP_ANIM_FIRST_RAISE = 0xE,
        WEAP_ANIM_BREACH_RAISE = 0xF,
        WEAP_ANIM_DROP = 0x10,
        WEAP_ANIM_ALT_RAISE = 0x11,
        WEAP_ANIM_ALT_DROP = 0x12,
        WEAP_ANIM_QUICK_RAISE = 0x13,
        WEAP_ANIM_QUICK_DROP = 0x14,
        WEAP_ANIM_EMPTY_RAISE = 0x15,
        WEAP_ANIM_EMPTY_DROP = 0x16,
        WEAP_ANIM_SPRINT_IN = 0x17,
        WEAP_ANIM_SPRINT_LOOP = 0x18,
        WEAP_ANIM_SPRINT_OUT = 0x19,
        WEAP_ANIM_STUNNED_START = 0x1A,
        WEAP_ANIM_STUNNED_LOOP = 0x1B,
        WEAP_ANIM_STUNNED_END = 0x1C,
        WEAP_ANIM_DETONATE = 0x1D,
        WEAP_ANIM_NIGHTVISION_WEAR = 0x1E,
        WEAP_ANIM_NIGHTVISION_REMOVE = 0x1F,
        WEAP_ANIM_ADS_FIRE = 0x20,
        WEAP_ANIM_ADS_LASTSHOT = 0x21,
        WEAP_ANIM_ADS_RECHAMBER = 0x22,
        WEAP_ANIM_ADS_UP = 0x23,
        WEAP_ANIM_ADS_DOWN = 0x24,
        NUM_WEAP_ANIMS = 0x25,
        WEAP_ANIM_VIEWMODEL_START = 0x1,
        WEAP_ANIM_VIEWMODEL_END = 0x23,
    };

	static const char* weapAnimFiles_Names[] = 
    {
        NAMEOF(WEAP_ANIM_ROOT),
        NAMEOF(WEAP_ANIM_IDLE),
        NAMEOF(WEAP_ANIM_EMPTY_IDLE),
        NAMEOF(WEAP_ANIM_FIRE),
        NAMEOF(WEAP_ANIM_HOLD_FIRE),
        NAMEOF(WEAP_ANIM_LASTSHOT),
        NAMEOF(WEAP_ANIM_RECHAMBER),
        NAMEOF(WEAP_ANIM_MELEE),
        NAMEOF(WEAP_ANIM_MELEE_CHARGE),
        NAMEOF(WEAP_ANIM_RELOAD),
        NAMEOF(WEAP_ANIM_RELOAD_EMPTY),
        NAMEOF(WEAP_ANIM_RELOAD_START),
        NAMEOF(WEAP_ANIM_RELOAD_END),
        NAMEOF(WEAP_ANIM_RAISE),
        NAMEOF(WEAP_ANIM_FIRST_RAISE),
        NAMEOF(WEAP_ANIM_BREACH_RAISE),
        NAMEOF(WEAP_ANIM_DROP),
        NAMEOF(WEAP_ANIM_ALT_RAISE),
        NAMEOF(WEAP_ANIM_ALT_DROP),
        NAMEOF(WEAP_ANIM_QUICK_RAISE),
        NAMEOF(WEAP_ANIM_QUICK_DROP),
        NAMEOF(WEAP_ANIM_EMPTY_RAISE),
        NAMEOF(WEAP_ANIM_EMPTY_DROP),
        NAMEOF(WEAP_ANIM_SPRINT_IN),
        NAMEOF(WEAP_ANIM_SPRINT_LOOP),
        NAMEOF(WEAP_ANIM_SPRINT_OUT),
        NAMEOF(WEAP_ANIM_STUNNED_START),
        NAMEOF(WEAP_ANIM_STUNNED_LOOP),
        NAMEOF(WEAP_ANIM_STUNNED_END),
        NAMEOF(WEAP_ANIM_DETONATE),
        NAMEOF(WEAP_ANIM_NIGHTVISION_WEAR),
        NAMEOF(WEAP_ANIM_NIGHTVISION_REMOVE),
        NAMEOF(WEAP_ANIM_ADS_FIRE),
        NAMEOF(WEAP_ANIM_ADS_LASTSHOT),
        NAMEOF(WEAP_ANIM_ADS_RECHAMBER),
        NAMEOF(WEAP_ANIM_ADS_UP),
        NAMEOF(WEAP_ANIM_ADS_DOWN)
    };

    enum OffhandClass
    {
        OFFHAND_CLASS_NONE = 0x0,
        OFFHAND_CLASS_FRAG_GRENADE = 0x1,
        OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
        OFFHAND_CLASS_FLASH_GRENADE = 0x3,
        OFFHAND_CLASS_THROWINGKNIFE = 0x4,
        OFFHAND_CLASS_OTHER = 0x5,
        OFFHAND_CLASS_COUNT = 0x6,
    };
	
	enum weapStance_t
    {
        WEAPSTANCE_STAND = 0x0,
        WEAPSTANCE_DUCK = 0x1,
        WEAPSTANCE_PRONE = 0x2,
        WEAPSTANCE_NUM = 0x3,
    };

    enum weaponIconRatioType_t
    {
        WEAPON_ICON_RATIO_1TO1 = 0x0,
        WEAPON_ICON_RATIO_2TO1 = 0x1,
        WEAPON_ICON_RATIO_4TO1 = 0x2,
        WEAPON_ICON_RATIO_COUNT = 0x3,
    };

    /* 113 */
    enum activeReticleType_t
    {
        VEH_ACTIVE_RETICLE_NONE = 0x0,
        VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 0x1,
        VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 0x2,
        VEH_ACTIVE_RETICLE_COUNT = 0x3,
    };

    /* 114 */
    enum ammoCounterClipType_t
    {
        AMMO_COUNTER_CLIP_NONE = 0x0,
        AMMO_COUNTER_CLIP_MAGAZINE = 0x1,
        AMMO_COUNTER_CLIP_SHORTMAGAZINE = 0x2,
        AMMO_COUNTER_CLIP_SHOTGUN = 0x3,
        AMMO_COUNTER_CLIP_ROCKET = 0x4,
        AMMO_COUNTER_CLIP_BELTFED = 0x5,
        AMMO_COUNTER_CLIP_ALTWEAPON = 0x6,
        AMMO_COUNTER_CLIP_COUNT = 0x7,
    };

    /* 115 */
    enum weapProjExposion_t
    {
        WEAPPROJEXP_GRENADE = 0x0,
        WEAPPROJEXP_ROCKET = 0x1,
        WEAPPROJEXP_FLASHBANG = 0x2,
        WEAPPROJEXP_NONE = 0x3,
        WEAPPROJEXP_DUD = 0x4,
        WEAPPROJEXP_SMOKE = 0x5,
        WEAPPROJEXP_HEAVY = 0x6,
        WEAPPROJEXP_NUM = 0x7,
    };

    /* 116 */
    enum guidedMissileType_t
    {
        MISSILE_GUIDANCE_NONE = 0x0,
        MISSILE_GUIDANCE_SIDEWINDER = 0x1,
        MISSILE_GUIDANCE_HELLFIRE = 0x2,
        MISSILE_GUIDANCE_JAVELIN = 0x3,
        MISSILE_GUIDANCE_COUNT = 0x4,
    };

    enum weapOverlayReticle_t
    {
        WEAPOVERLAYRETICLE_NONE = 0x0,
        WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
        WEAPOVERLAYRETICLE_NUM = 0x2,
    };

    enum WeapOverlayInteface_t
    {
        WEAPOVERLAYINTERFACE_NONE = 0x0,
        WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
        WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
        WEAPOVERLAYINTERFACECOUNT = 0x3,
    };

    struct __declspec(align(4)) WeaponDef
    {
        const char* szOverlayName;
        XModel** gunXModel;
        XModel* handXModel;
        const char** szXAnimsRightHanded;
        const char** szXAnimsLeftHanded;
        const char* szModeName;
        unsigned __int16* notetrackSoundMapKeys;
        unsigned __int16* notetrackSoundMapValues;
        unsigned __int16* notetrackRumbleMapKeys;
        unsigned __int16* notetrackRumbleMapValues;
        int playerAnimType;
        weapType_t weapType;
        weapClass_t weapClass;
        PenetrateType penetrateType;
        weapInventoryType_t inventoryType;
        weapFireType_t fireType;
        OffhandClass offhandClass;
        weapStance_t stance;
        FxEffectDef* viewFlashEffect;
        FxEffectDef* worldFlashEffect;
        snd_alias_list_t* pickupSound;
        snd_alias_list_t* pickupSoundPlayer;
        snd_alias_list_t* ammoPickupSound;
        snd_alias_list_t* ammoPickupSoundPlayer;
        snd_alias_list_t* projectileSound;
        snd_alias_list_t* pullbackSound;
        snd_alias_list_t* pullbackSoundPlayer;
        snd_alias_list_t* fireSound;
        snd_alias_list_t* fireSoundPlayer;
        snd_alias_list_t* fireSoundPlayerAkimbo;
        snd_alias_list_t* fireLoopSound;
        snd_alias_list_t* fireLoopSoundPlayer;
        snd_alias_list_t* fireStopSound;
        snd_alias_list_t* fireStopSoundPlayer;
        snd_alias_list_t* fireLastSound;
        snd_alias_list_t* fireLastSoundPlayer;
        snd_alias_list_t* emptyFireSound;
        snd_alias_list_t* emptyFireSoundPlayer;
        snd_alias_list_t* meleeSwipeSound;
        snd_alias_list_t* meleeSwipeSoundPlayer;
        snd_alias_list_t* meleeHitSound;
        snd_alias_list_t* meleeMissSound;
        snd_alias_list_t* rechamberSound;
        snd_alias_list_t* rechamberSoundPlayer;
        snd_alias_list_t* reloadSound;
        snd_alias_list_t* reloadSoundPlayer;
        snd_alias_list_t* reloadEmptySound;
        snd_alias_list_t* reloadEmptySoundPlayer;
        snd_alias_list_t* reloadStartSound;
        snd_alias_list_t* reloadStartSoundPlayer;
        snd_alias_list_t* reloadEndSound;
        snd_alias_list_t* reloadEndSoundPlayer;
        snd_alias_list_t* detonateSound;
        snd_alias_list_t* detonateSoundPlayer;
        snd_alias_list_t* nightVisionWearSound;
        snd_alias_list_t* nightVisionWearSoundPlayer;
        snd_alias_list_t* nightVisionRemoveSound;
        snd_alias_list_t* nightVisionRemoveSoundPlayer;
        snd_alias_list_t* altSwitchSound;
        snd_alias_list_t* altSwitchSoundPlayer;
        snd_alias_list_t* raiseSound;
        snd_alias_list_t* raiseSoundPlayer;
        snd_alias_list_t* firstRaiseSound;
        snd_alias_list_t* firstRaiseSoundPlayer;
        snd_alias_list_t* putawaySound;
        snd_alias_list_t* putawaySoundPlayer;
        snd_alias_list_t* scanSound;
        snd_alias_list_t** bounceSound;
        FxEffectDef* viewShellEjectEffect;
        FxEffectDef* worldShellEjectEffect;
        FxEffectDef* viewLastShotEjectEffect;
        FxEffectDef* worldLastShotEjectEffect;
        Material* reticleCenter;
        Material* reticleSide;
        int iReticleCenterSize;
        int iReticleSideSize;
        int iReticleMinOfs;
        activeReticleType_t activeReticleType;
        float vStandMove[3];
        float vStandRot[3];
        float strafeMove[3];
        float strafeRot[3];
        float vDuckedOfs[3];
        float vDuckedMove[3];
        float vDuckedRot[3];
        float vProneOfs[3];
        float vProneMove[3];
        float vProneRot[3];
        float fPosMoveRate;
        float fPosProneMoveRate;
        float fStandMoveMinSpeed;
        float fDuckedMoveMinSpeed;
        float fProneMoveMinSpeed;
        float fPosRotRate;
        float fPosProneRotRate;
        float fStandRotMinSpeed;
        float fDuckedRotMinSpeed;
        float fProneRotMinSpeed;
        XModel** worldModel;
        XModel* worldClipModel;
        XModel* rocketModel;
        XModel* knifeModel;
        XModel* worldKnifeModel;
        Material* hudIcon;
        weaponIconRatioType_t hudIconRatio;
        Material* pickupIcon;
        weaponIconRatioType_t pickupIconRatio;
        Material* ammoCounterIcon;
        weaponIconRatioType_t ammoCounterIconRatio;
        ammoCounterClipType_t ammoCounterClip;
        int iStartAmmo;
        const char* szAmmoName;
        int iAmmoIndex;
        const char* szClipName;
        int iClipIndex;
        int iMaxAmmo;
        int shotCount;
        const char* szSharedAmmoCapName;
        int iSharedAmmoCapIndex;
        int iSharedAmmoCap;
        int damage;
        int playerDamage;
        int iMeleeDamage;
        int iDamageType;
        int iFireDelay;
        int iMeleeDelay;
        int meleeChargeDelay;
        int iDetonateDelay;
        int iRechamberTime;
        int rechamberTimeOneHanded;
        int iRechamberBoltTime;
        int iHoldFireTime;
        int iDetonateTime;
        int iMeleeTime;
        int meleeChargeTime;
        int iReloadTime;
        int reloadShowRocketTime;
        int iReloadEmptyTime;
        int iReloadAddTime;
        int iReloadStartTime;
        int iReloadStartAddTime;
        int iReloadEndTime;
        int iDropTime;
        int iRaiseTime;
        int iAltDropTime;
        int quickDropTime;
        int quickRaiseTime;
        int iBreachRaiseTime;
        int iEmptyRaiseTime;
        int iEmptyDropTime;
        int sprintInTime;
        int sprintLoopTime;
        int sprintOutTime;
        int stunnedTimeBegin;
        int stunnedTimeLoop;
        int stunnedTimeEnd;
        int nightVisionWearTime;
        int nightVisionWearTimeFadeOutEnd;
        int nightVisionWearTimePowerUp;
        int nightVisionRemoveTime;
        int nightVisionRemoveTimePowerDown;
        int nightVisionRemoveTimeFadeInStart;
        int fuseTime;
        int aiFuseTime;
        float autoAimRange;
        float aimAssistRange;
        float aimAssistRangeAds;
        float aimPadding;
        float enemyCrosshairRange;
        float moveSpeedScale;
        float adsMoveSpeedScale;
        float sprintDurationScale;
        float fAdsZoomInFrac;
        float fAdsZoomOutFrac;
        Material* overlayMaterial;
        Material* overlayMaterialLowRes;
        Material* overlayMaterialEMP;
        Material* overlayMaterialEMPLowRes;
        weapOverlayReticle_t overlayReticle;
        WeapOverlayInteface_t overlayInterface;
        float overlayWidth;
        float overlayHeight;
        float overlayWidthSplitscreen;
        float overlayHeightSplitscreen;
        float fAdsBobFactor;
        float fAdsViewBobMult;
        float fHipSpreadStandMin;
        float fHipSpreadDuckedMin;
        float fHipSpreadProneMin;
        float hipSpreadStandMax;
        float hipSpreadDuckedMax;
        float hipSpreadProneMax;
        float fHipSpreadDecayRate;
        float fHipSpreadFireAdd;
        float fHipSpreadTurnAdd;
        float fHipSpreadMoveAdd;
        float fHipSpreadDuckedDecay;
        float fHipSpreadProneDecay;
        float fHipReticleSidePos;
        float fAdsIdleAmount;
        float fHipIdleAmount;
        float adsIdleSpeed;
        float hipIdleSpeed;
        float fIdleCrouchFactor;
        float fIdleProneFactor;
        float fGunMaxPitch;
        float fGunMaxYaw;
        float swayMaxAngle;
        float swayLerpSpeed;
        float swayPitchScale;
        float swayYawScale;
        float swayHorizScale;
        float swayVertScale;
        float swayShellShockScale;
        float adsSwayMaxAngle;
        float adsSwayLerpSpeed;
        float adsSwayPitchScale;
        float adsSwayYawScale;
        float adsSwayHorizScale;
        float adsSwayVertScale;
        float adsViewErrorMin;
        float adsViewErrorMax;
        PhysCollmap* physCollmap;
        float dualWieldViewModelOffset;
        weaponIconRatioType_t killIconRatio;
        int iReloadAmmoAdd;
        int iReloadStartAdd;
        int ammoDropStockMin;
        int ammoDropClipPercentMin;
        int ammoDropClipPercentMax;
        int iExplosionRadius;
        int iExplosionRadiusMin;
        int iExplosionInnerDamage;
        int iExplosionOuterDamage;
        float damageConeAngle;
        float bulletExplDmgMult;
        float bulletExplRadiusMult;
        int iProjectileSpeed;
        int iProjectileSpeedUp;
        int iProjectileSpeedForward;
        int iProjectileActivateDist;
        float projLifetime;
        float timeToAccelerate;
        float projectileCurvature;
        XModel* projectileModel;
        weapProjExposion_t projExplosion;
        FxEffectDef* projExplosionEffect;
        FxEffectDef* projDudEffect;
        snd_alias_list_t* projExplosionSound;
        snd_alias_list_t* projDudSound;
        WeapStickinessType stickiness;
        float lowAmmoWarningThreshold;
        float ricochetChance;
        float* parallelBounce;
        float* perpendicularBounce;
        FxEffectDef* projTrailEffect;
        FxEffectDef* projBeaconEffect;
        float vProjectileColor[3];
        guidedMissileType_t guidedMissileType;
        float maxSteeringAccel;
        int projIgnitionDelay;
        FxEffectDef* projIgnitionEffect;
        snd_alias_list_t* projIgnitionSound;
        float fAdsAimPitch;
        float fAdsCrosshairInFrac;
        float fAdsCrosshairOutFrac;
        int adsGunKickReducedKickBullets;
        float adsGunKickReducedKickPercent;
        float fAdsGunKickPitchMin;
        float fAdsGunKickPitchMax;
        float fAdsGunKickYawMin;
        float fAdsGunKickYawMax;
        float fAdsGunKickAccel;
        float fAdsGunKickSpeedMax;
        float fAdsGunKickSpeedDecay;
        float fAdsGunKickStaticDecay;
        float fAdsViewKickPitchMin;
        float fAdsViewKickPitchMax;
        float fAdsViewKickYawMin;
        float fAdsViewKickYawMax;
        float fAdsViewScatterMin;
        float fAdsViewScatterMax;
        float fAdsSpread;
        int hipGunKickReducedKickBullets;
        float hipGunKickReducedKickPercent;
        float fHipGunKickPitchMin;
        float fHipGunKickPitchMax;
        float fHipGunKickYawMin;
        float fHipGunKickYawMax;
        float fHipGunKickAccel;
        float fHipGunKickSpeedMax;
        float fHipGunKickSpeedDecay;
        float fHipGunKickStaticDecay;
        float fHipViewKickPitchMin;
        float fHipViewKickPitchMax;
        float fHipViewKickYawMin;
        float fHipViewKickYawMax;
        float fHipViewScatterMin;
        float fHipViewScatterMax;
        float fightDist;
        float maxDist;
        const char* accuracyGraphName[2];
        float (*originalAccuracyGraphKnots[2])[2];
        unsigned __int16 originalAccuracyGraphKnotCount[2];
        int iPositionReloadTransTime;
        float leftArc;
        float rightArc;
        float topArc;
        float bottomArc;
        float accuracy;
        float aiSpread;
        float playerSpread;
        float minTurnSpeed[2];
        float maxTurnSpeed[2];
        float pitchConvergenceTime;
        float yawConvergenceTime;
        float suppressTime;
        float maxRange;
        float fAnimHorRotateInc;
        float fPlayerPositionDist;
        const char* szUseHintString;
        const char* dropHintString;
        int iUseHintStringIndex;
        int dropHintStringIndex;
        float horizViewJitter;
        float vertViewJitter;
        float scanSpeed;
        float scanAccel;
        int scanPauseTime;
        const char* szScript;
        float fOOPosAnimLength[2];
        int minDamage;
        int minPlayerDamage;
        float fMaxDamageRange;
        float fMinDamageRange;
        float destabilizationRateTime;
        float destabilizationCurvatureMax;
        int destabilizeDistance;
        float* locationDamageMultipliers;
        const char* fireRumble;
        const char* meleeImpactRumble;
        TracerDef* tracerType;
        float turretScopeZoomRate;
        float turretScopeZoomMin;
        float turretScopeZoomMax;
        float turretOverheatUpRate;
        float turretOverheatDownRate;
        float turretOverheatPenalty;
        snd_alias_list_t* turretOverheatSound;
        FxEffectDef* turretOverheatEffect;
        const char* turretBarrelSpinRumble;
        float turretBarrelSpinSpeed;
        float turretBarrelSpinUpTime;
        float turretBarrelSpinDownTime;
        snd_alias_list_t* turretBarrelSpinMaxSnd;
        snd_alias_list_t* turretBarrelSpinUpSnd[4];
        snd_alias_list_t* turretBarrelSpinDownSnd[4];
        snd_alias_list_t* missileConeSoundAlias;
        snd_alias_list_t* missileConeSoundAliasAtBase;
        float missileConeSoundRadiusAtTop;
        float missileConeSoundRadiusAtBase;
        float missileConeSoundHeight;
        float missileConeSoundOriginOffset;
        float missileConeSoundVolumescaleAtCore;
        float missileConeSoundVolumescaleAtEdge;
        float missileConeSoundVolumescaleCoreSize;
        float missileConeSoundPitchAtTop;
        float missileConeSoundPitchAtBottom;
        float missileConeSoundPitchTopSize;
        float missileConeSoundPitchBottomSize;
        float missileConeSoundCrossfadeTopSize;
        float missileConeSoundCrossfadeBottomSize;
        bool sharedAmmo;
        bool lockonSupported;
        bool requireLockonToFire;
        bool bigExplosion;
        bool noAdsWhenMagEmpty;
        bool avoidDropCleanup;
        bool inheritsPerks;
        bool crosshairColorChange;
        bool bRifleBullet;
        bool armorPiercing;
        bool bBoltAction;
        bool aimDownSight;
        bool bRechamberWhileAds;
        bool bBulletExplosiveDamage;
        bool bCookOffHold;
        bool bClipOnly;
        bool noAmmoPickup;
        bool adsFireOnly;
        bool cancelAutoHolsterWhenEmpty;
        bool disableSwitchToWhenEmpty;
        bool suppressAmmoReserveDisplay;
        bool laserSightDuringNightvision;
        bool markableViewmodel;
        bool noDualWield;
        bool flipKillIcon;
        bool bNoPartialReload;
        bool bSegmentedReload;
        bool blocksProne;
        bool silenced;
        bool isRollingGrenade;
        bool projExplosionEffectForceNormalUp;
        bool bProjImpactExplode;
        bool stickToPlayers;
        bool hasDetonator;
        bool disableFiring;
        bool timedDetonation;
        bool rotate;
        bool holdButtonToThrow;
        bool freezeMovementWhenFiring;
        bool thermalScope;
        bool altModeSameWeapon;
        bool turretBarrelSpinEnabled;
        bool missileConeSoundEnabled;
        bool missileConeSoundPitchshiftEnabled;
        bool missileConeSoundCrossfadeEnabled;
        bool offhandHoldIsCancelable;
    };
	
	struct  WeaponCompleteDef
    {
        const char* szInternalName;
        WeaponDef* weapDef;
        const char* szDisplayName;
        unsigned __int16* hideTags;
        const char** szXAnims;
        float fAdsZoomFov;
        int iAdsTransInTime;
        int iAdsTransOutTime;
        int iClipSize;
        ImpactType impactType;
        int iFireTime;
        weaponIconRatioType_t dpadIconRatio;
        float penetrateMultiplier;
        float fAdsViewKickCenterSpeed;
        float fHipViewKickCenterSpeed;
        const char* szAltWeaponName;
        unsigned int altWeaponIndex;
        int iAltRaiseTime;
        Material* killIcon;
        Material* dpadIcon;
        int fireAnimLength;
        int iFirstRaiseTime;
        int ammoDropStockMax;
        float adsDofStart;
        float adsDofEnd;
        unsigned __int16 accuracyGraphKnotCount[2];
        float (*accuracyGraphKnots[2])[2];
        bool motionTracker;
        bool enhanced;
        bool dpadIconShowsAmmo;
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
        // GameWorldSp* gameWorldSp;
        GameWorldMp* gameWorldMp;
        MapEnts* mapEnts;
        FxWorld* fxWorld;
        GfxWorld* gfxWorld;
        GfxLightDef* lightDef;
        // Font_s* font;
        // MenuList* menuList;
        // menuDef_t* menu;
        // LocalizeEntry* localize;
        WeaponCompleteDef* weapon;
        // SndDriverGlobals* sndDriverGlobals;
        FxEffectDef* fx;
        // FxImpactTable* impactFx;
        RawFile* rawfile;
        // StringTable* stringTable;
        // LeaderboardDef* leaderboardDef;
        // StructuredDataDefSet* structuredDataDefSet;
        // TracerDef* tracerDef;
        // VehicleDef* vehDef;
        // AddonMapEnts* addonMapEnts;
    };

    struct XAsset
    {
        XAssetType type;
        XAssetHeader header;
    };
} // namespace iw4of::native
