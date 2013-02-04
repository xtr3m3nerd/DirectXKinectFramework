#ifndef _SKELETONCLASS_H_
#define _SKELETONCLASS_H_



//////////////
// INCLUDES //
//////////////
#include <d3d10.h>
#include <d3dx10.h>
#include <NuiApi.h>


////////////////////////////////////////////////////////////////////////////////
// Class: Skeleton - this class serves as an advanced model class that takes in kinect data and allows us to render it
////////////////////////////////////////////////////////////////////////////////
class SkeletonClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR4 color;
	};

	struct ModelType
	{
		float x, y, z;
	};

public:
	SkeletonClass();
	SkeletonClass(const SkeletonClass&);
	~SkeletonClass();

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
	ModelType* m_SkeletonModel;//Need to figure out method for rendering skeleton frame. 
	NUI_SKELETON_DATA m_Skeleton; 
};

#endif