

#ifndef _DEX_MODEL_MS3D_H_
#define _DEX_MODEL_MS3D_H_

#include "DexModelBase.h"

/* 
	MS3D STRUCTURES 
*/

// byte-align structures
#ifdef _MSC_VER
#	pragma pack( push, 1 )  
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#endif

// File header
struct MS3DHeader
{
	char m_ID[10];
	int32 m_version;
} PACK_STRUCT;

// Vertex information
struct MS3DVertex
{
	int8 m_flags;
	float m_vertex[3];
	char m_boneID;
	int8 m_refCount;
} PACK_STRUCT;

// Triangle information
struct MS3DTriangle
{
	int16 m_flags;
	int16 m_vertexIndices[3];
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	int8 m_smoothingGroup;
	int8 m_groupIndex;
} PACK_STRUCT;

// Material information
struct MS3DMaterial
{
    char m_name[32];
    float m_ambient[4];
    float m_diffuse[4];
    float m_specular[4];
    float m_emissive[4];
    float m_shininess;	// 0.0f - 128.0f
    float m_transparency;	// 0.0f - 1.0f
    int8 m_mode;	// 0, 1, 2 is unused now
    char m_texture[128];
    char m_alphamap[128];
} PACK_STRUCT;

//	Joint information
struct MS3DJoint
{
	int8 m_flags;
	char m_name[32];
	char m_parentName[32];
	float m_rotation[3];
	float m_translation[3];
	int16 m_numRotationKeyframes;
	int16 m_numTranslationKeyframes;
} PACK_STRUCT;

// Keyframe data
struct MS3DKeyframe
{
	float m_time;
	float m_parameter[3];
} PACK_STRUCT;

// Default alignment
#ifdef _MSC_VER
#	pragma pack(pop)
#endif

class DexModelMs3d: public DexModelBase
{
	Dex_DeclareClass(DexModelMs3d, 0)
public:
	DexModelMs3d();
	virtual ~DexModelMs3d();

public:
	virtual bool Update(int delta);
	virtual bool Render();
	virtual bool LoadModel(const char* filename); //日后扩展从字节流中加载文件
};

#endif