#ifndef _KINECTCLASS_H_
#define _KINECTCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "kinect10.lib")

//////////////
// INCLUDES //
//////////////
#include <Windows.h>
#include <Ole2.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

//Should redesign so that directx dependancies are not needed
#include <d3d10.h>
#include <d3dx10.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "input.h"
#include "dynamicbitmap.h" 
#include "skeleton.h"


////////////////////////////////////////////////////////////////////////////////
// Class: Kinect
////////////////////////////////////////////////////////////////////////////////
class KinectClass
{
public:
	KinectClass();
	KinectClass(const KinectClass&);
	~KinectClass();

	bool Initialize(ID3D10Device* d3d);
	void Shutdown();

	bool Kinect_Init(ID3D10Device* d3d);
	void Nui_Zero();

	bool Frame(InputClass* input);

	bool Render(ID3D10Device* device, int texture);

	ID3D10ShaderResourceView* GetShaderResourceViewRGB();
	ID3D10ShaderResourceView* GetShaderResourceViewDepth();

	//Tempory until differnt system for alpha blending implemented
	float GetDepthBlend();

	//Tempory Functions to retrive Hand position to test skeleton funtionality
	float Getx(){return x;}
	float Gety(){return y;}
	float Getz(){return z;}

private:
	//Functions to Obtain Data from Kinect
	bool GetKinectRGB(); 
	bool GetKinectDepth();
	bool GetKinectSkeleton();
	
	bool OutputToFile();
	
	// Kinect variables
	HANDLE m_RGBStream;              // The identifier of the Kinect's RGB Camera
	HANDLE m_DepthStream;              // The identifier of the Kinect's RGB Camera
	HANDLE m_SkeletonEvent;            // The identifier of the Skeleton Event tracker
	INuiSensor* m_Sensor;            // The kinect sensor

	
	//RGB data 
	DynamicBitmapClass* m_RGBdata;
	int m_RGBposX, m_RGBposY;
	float m_RGBscale;
	float m_RGBAlpha; // Obsolete variable

	//Depth data
	DynamicBitmapClass* m_Depthdata;
	int m_DepthposX, m_DepthposY;
	float m_Depthscale;
	float m_DepthAlpha;

	//Skeleton data
	NUI_SKELETON_FRAME m_SkeletonFrame;
	SkeletonClass* m_Skeleton; 


	//Tempory for testing skeleton funtionality, remove later
	float x,y,z;

};

#endif