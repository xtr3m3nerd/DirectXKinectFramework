#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3d.h"
#include "camera.h"
#include "model.h"
#include "colorshader.h"
#include "textureshader.h"
#include "transparentshader.h"
#include "text.h"
#include "bitmap.h"

#include "kinect.h" 
#include "input.h" 


////////////////////////////////////////////////////////////////////////////////
// Class: Graphics
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float, InputClass*);
	bool Render();

private:

	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_TextureShader;
	TransparentShaderClass* m_TransparentShader;
	TextClass* m_Text;

	//Graphics Testing Tools
	ModelClass* m_Model1;
	ModelClass* m_Model2;
	BitmapClass* m_Bitmap;

	//Tempory Location for Kinect Class, Move to system after fixes are made to graphics
	KinectClass* m_Kinect;

	//Tempory Data for altering camera data, remove later with control integration
	float cameraPos;

};

#endif