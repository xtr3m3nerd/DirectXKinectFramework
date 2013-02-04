#ifndef _DYNAMICBITMAPCLASS_H_
#define _DYNAMICBITMAPCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "texture.h"


////////////////////////////////////////////////////////////////////////////////
// Class: DynamicBitmap
////////////////////////////////////////////////////////////////////////////////
class DynamicBitmapClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	DynamicBitmapClass();
	DynamicBitmapClass(const DynamicBitmapClass&);
	~DynamicBitmapClass();

	bool Initialize(ID3D10Device*, int, int, int, int);
	void Shutdown();
	bool Render(ID3D10Device*);

	int GetIndexCount();
	ID3D10ShaderResourceView* GetTexture();

	ID3D10Texture2D* GetTextureEdit();
	bool UpdatePosition(int, int, float);

private:
	bool InitializeBuffers(ID3D10Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(int, int, float);
	void RenderBuffers(ID3D10Device*);


private:
	ID3D10Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ID3D10Texture2D* m_Texture;
	ID3D10ShaderResourceView* m_shaderResourceView;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
	float m_previousScale;
};

#endif