#ifndef _KINEMATICARM_H_
#define _KINEMATICARM_H_



//////////////
// INCLUDES //
//////////////
#include <d3d10.h>
#include <d3dx10.h>
#include <NuiApi.h>

////////////////////////////////////////////////////////////////////////////////
// Class: Kinematic Arm - This attempt to improve accuracy of 
////////////////////////////////////////////////////////////////////////////////
class KinematicArmClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR4 color;
	};
	struct Joint
	{
		float x, y, z;
	};

public:
	KinematicArmClass();
	KinematicArmClass(const KinematicArmClass&);
	~KinematicArmClass();

	bool Initialize(ID3D10Device*);
	void Shutdown();
	void Render(ID3D10Device*);

	bool UpdateSkeleton( const NUI_SKELETON_DATA &); 

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D10Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D10Device*);
	bool UpdateBuffers(ID3D10Device*);

	bool InitialSkeleton();
	void ReleaseSkeleton();

private:
	ID3D10Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Joint* m_Joints;//Need to figure out method for rendering skeleton frame. 
	NUI_SKELETON_DATA m_Skeleton; 
};

#endif