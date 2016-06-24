

#include "DexSkinMesh.h"
#include "..\DexBase\DexDMap.h"


class DexSkinMeshFF : public DexSkinMesh
{
	Dex_DeclareClass(DexSkinMeshFF, 0)
		
	friend class DexModelDaeLoader;
public:
	DexSkinMeshFF();
	virtual ~DexSkinMeshFF();

	struct stFFJoint
	{
		Joint* pDaeJoint;
		Joint* pMwsJoint;
	};
protected:
	DVector<Joint*>		 m_vecMwsJoints;		//joint
	Joint*				 m_mwsRootJoint;
	DMap<DString, stFFJoint> m_mapJoints;
public:
	Joint* FindMwsJoint(int32 index);
	Joint* FindMwsJoint(DString name);
	Joint* AddMwsJoint(DString name, int32 index, DexSkinMesh::Joint* pFather, DexMatrix4x4 father_matrix);

public:
	virtual bool Update(int32 delta);
	virtual bool Render();

};