#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d10.h>
#include <d3dx10.h>


////////////////////////////////////////////////////////////////////////////////
// Class: Texture
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D10Device*, WCHAR*);
	void Shutdown();

	ID3D10ShaderResourceView* GetTexture();

private:
	ID3D10ShaderResourceView* m_texture;
};

#endif