#include "kinect.h"
#include <mmsystem.h>
#include <assert.h>
#include <strsafe.h>
#include <wchar.h>

#include <fstream>
using namespace std;

// Maximum size for Depth and Color data 
#define width 640
#define height 480

KinectClass::KinectClass()
{
	m_Sensor = 0;
    Nui_Zero();
	
	//zero out streams of data from kinect
	m_RGBStream = 0;
	m_DepthStream = 0;
	m_SkeletonEvent = 0;

	//zero out RGB camera data
	m_RGBdata = 0;
	m_RGBposX = 0;
	m_RGBposY = 0;
	m_RGBscale = 1.0f;
	m_RGBAlpha = 1.0f;

	//zero out Depth camera data
	m_Depthdata = 0;
	m_DepthposX = 0;
	m_DepthposY = 0;
	m_Depthscale = 1.0f;
	m_DepthAlpha = 1.0f;


	m_Skeleton = 0; 

	//zero out test variables, remove later
	x = 0;
	y = 0;
	z = 0;

}

KinectClass::~KinectClass()
{
    Nui_Zero();
}

bool KinectClass::Initialize(ID3D10Device* d3d)
{
	bool result; 
	
	// Initialize kinect sensor
	result = Kinect_Init(d3d);
	if(!result)
	{
		return false;
	}	

	//Create dynamic RGB Textures to forward kinect image data for rendering
	m_RGBdata = new DynamicBitmapClass;
	if(!m_RGBdata)
	{
		return false;
	}

	//initialize RGB texture
	result = m_RGBdata->Initialize(d3d,800,600,width,height);
	if(!result)
	{
		return false;
	}

	//Create dynamic Depth Textures to forward kinect depth data for rendering
	m_Depthdata = new DynamicBitmapClass;
	if(!m_Depthdata)
	{
		return false;
	}

	//initialize Depth texture
	result = m_Depthdata->Initialize(d3d,800,600,width,height);
	if(!result)
	{
		return false;
	}

	//Create Skeleton object to manage skeleton data
	m_Skeleton = new SkeletonClass;
	if(!m_Skeleton)
	{
		return false;
	}

	// initialize skeleton
	result = m_Skeleton->Initialize(d3d);
	if(!result)
	{
		return false;
	}

	ofstream myfile ("example.txt");
	if (myfile.is_open())
	{
		myfile << "Storage for Kinect data of right arm\n";
		myfile << "File format:\n";
		myfile << " TimeStamp : Shoulder X Y Z : Elbow X Y Z : Wrist X Y Z : Hand X Y Z ";
		myfile << endl; 

		myfile.close();
	}
	else
	{
		return false;
	}

	return true;
}

void KinectClass::Shutdown()
{
	// Release the skeleton object
	if(m_Skeleton)
	{
		m_Skeleton->Shutdown();
		delete m_Skeleton;
		m_Skeleton = 0;
	}

	// Release the RGB dynamic texture
	if(m_RGBdata)
	{
		m_RGBdata->Shutdown();
		delete m_RGBdata;
		m_RGBdata = 0;
	}

	// Release the Depth dynamic texture
	if(m_Depthdata)
	{
		m_Depthdata->Shutdown();
		delete m_Depthdata;
		m_Depthdata = 0;
	}

	//Release the skeleton stream data 
	if ( m_SkeletonEvent && ( m_SkeletonEvent != INVALID_HANDLE_VALUE ) )
    {
        CloseHandle( m_SkeletonEvent );
        m_SkeletonEvent = NULL;
    }

	Nui_Zero();
}

bool KinectClass::Kinect_Init(ID3D10Device* d3d)
{
	HRESULT  hr;

    // Get a working kinect sensor
    int numSensors;
    if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) 
	{
		return false;
	}
    if (NuiCreateSensorByIndex(0, &m_Sensor) < 0) 
	{
		return false;
	}

	 DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON |  NUI_INITIALIZE_FLAG_USES_COLOR;

    // Initialize sensor
    hr = m_Sensor->NuiInitialize(nuiFlags);
	if ( E_NUI_SKELETAL_ENGINE_BUSY == hr )
    {
        nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH |  NUI_INITIALIZE_FLAG_USES_COLOR;
        hr = m_Sensor->NuiInitialize( nuiFlags) ;
    }

	if ( FAILED( hr ) )
    {
        if ( E_NUI_DEVICE_IN_USE == hr )
        {
            //return false;
        }
        else
        {
            return false;
        }
    }

	//Set up Skeleton Stream
	m_SkeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	DWORD skeletonTrackingFlags = NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE;

    if ( HasSkeletalEngine( m_Sensor ) )
    {
        hr = m_Sensor->NuiSkeletonTrackingEnable( m_SkeletonEvent, skeletonTrackingFlags );
        if( FAILED( hr ) )
        {
            return false;
        }
    }

	//Set up RGB stream
    m_Sensor->NuiImageStreamOpen(
        NUI_IMAGE_TYPE_COLOR,            // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_640x480,    // Image resolution
        0,      // Image stream flags, e.g. near mode
        2,      // Number of frames to buffer
        NULL,   // Event handle
        &m_RGBStream);

	//Set up Depth stream
	m_Sensor->NuiImageStreamOpen(
        HasSkeletalEngine(m_Sensor) ? NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX : NUI_IMAGE_TYPE_DEPTH,
        NUI_IMAGE_RESOLUTION_640x480,
        NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE,
        2,
        NULL,
        &m_DepthStream );

	if(!m_Sensor)
	{
		return false;
	}


    return true;
}

void KinectClass::Nui_Zero()
{
	if ( m_Sensor != NULL )
    {
        m_Sensor->Release();
        m_Sensor = NULL;
    }
}

bool KinectClass::Frame(InputClass* input)
{
	bool result;

	//Inputs for modifing RGB picture information and location
	if(input->IsKeyDown(VK_UP))      m_RGBposY -= 1;
	if(input->IsKeyDown(VK_DOWN))    m_RGBposY += 1;
	if(input->IsKeyDown(VK_LEFT))    m_RGBposX -= 1;
	if(input->IsKeyDown(VK_RIGHT))   m_RGBposX += 1;
	if(input->IsKeyDown(VK_SPACE))   m_RGBscale += 0.01f;
	if(input->IsKeyDown(VK_CONTROL)) m_RGBscale -= 0.01f;
	if(input->IsKeyDown('1'))        m_RGBAlpha += 0.01f;
	if(input->IsKeyDown('2'))        m_RGBAlpha -= 0.01f;

	if(m_RGBAlpha > 1.0f) m_RGBAlpha = 1.0f;
	if(m_RGBAlpha < 0.0f) m_RGBAlpha = 0.0f;

	m_RGBdata->UpdatePosition(m_RGBposX,m_RGBposY,m_RGBscale);

	//Inputs for modifing Depth picture information and location
	if(input->IsKeyDown('W'))        m_DepthposY -= 1;
	if(input->IsKeyDown('S'))        m_DepthposY += 1;
	if(input->IsKeyDown('A'))        m_DepthposX -= 1;
	if(input->IsKeyDown('D'))        m_DepthposX += 1;
	if(input->IsKeyDown('Q'))        m_Depthscale += 0.01f;
	if(input->IsKeyDown('E'))        m_Depthscale -= 0.01f;
	if(input->IsKeyDown('3'))        m_DepthAlpha += 0.01f;
	if(input->IsKeyDown('4'))        m_DepthAlpha -= 0.01f;

	if(m_DepthAlpha > 1.0f) m_DepthAlpha = 1.0f;
	if(m_DepthAlpha < 0.0f) m_DepthAlpha = 0.0f;

	m_Depthdata->UpdatePosition(m_DepthposX,m_DepthposY,m_Depthscale);

	//Update RGB stream
	result = GetKinectRGB();
	if(!result)
	{
		return false; 
	}

	//Update Depth stream
	result = GetKinectDepth();
	if(!result)
	{
		return false; 
	}

	//Update Skeleton stream
	result = GetKinectSkeleton();
	if(!result)
	{
		return false; 
	}


	return true;
}

bool KinectClass::Render(ID3D10Device* device,int texture)
{
	//Rewrite this to pass through dynamic list of needed renders 
	switch (texture)
	{
	case 0:
		m_RGBdata->Render(device);
		break;
	case 1:
		m_Depthdata->Render(device);
		break;
	case 2:
		m_Skeleton->Render(device);
		break;
	default: 
		break;
	}

	return true;
}

ID3D10ShaderResourceView* KinectClass::GetShaderResourceViewRGB()
{
	return m_RGBdata->GetTexture();
}

ID3D10ShaderResourceView* KinectClass::GetShaderResourceViewDepth()
{
	return m_Depthdata->GetTexture();
}

float KinectClass::GetDepthBlend()
{
	return m_DepthAlpha;
}

bool KinectClass::GetKinectRGB()
{
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
    bool processedFrame = true;

	//Get RGB data
	HRESULT hr = m_Sensor->NuiImageStreamGetNextFrame(m_RGBStream, 0, &imageFrame);

    if ( FAILED(hr) )
	{
		if(hr == S_FALSE)
		{
			return false;
		}
		if(hr == E_INVALIDARG)
		{
			return false;
		}
		if(hr == E_NUI_FRAME_NO_DATA)
		{
			return true;
		}
		if(hr == E_POINTER)
		{
			return false;
		}
		return false;
	}

	//set up dynamic mapping
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);


	if (LockedRect.Pitch != 0)
    {
        const BYTE* curr = (const BYTE*) LockedRect.pBits;
        const BYTE* dataEnd = curr + (width*height)*4;

		ID3D10Texture2D* tex = m_RGBdata->GetTextureEdit();


		D3D10_MAPPED_TEXTURE2D mappedTex;
		tex->Map( D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex );


		int alpha = m_RGBAlpha*255; 

		UCHAR* pTexels = (UCHAR*)mappedTex.pData;

		//Temp data vales for conversion from BGR to RGB
		BYTE Blue;
		BYTE Green;
		BYTE Red;
		BYTE Alpha;
		
		//Copy over data from stream to dynamic bitmap 
		while (curr < dataEnd) 
		{
			Blue = *curr++;
			Green = *curr++;
			Red = *curr++;
			Alpha = *curr++;

            *pTexels++ = Red;
			*pTexels++ = Green;
			*pTexels++ = Blue;
			*pTexels++ = alpha;
        }

		tex->Unmap( D3D10CalcSubresource(0, 0, 1) );
    }

	texture->UnlockRect(0);
    m_Sensor->NuiImageStreamReleaseFrame(m_RGBStream, &imageFrame);

	return true;
}

bool KinectClass::GetKinectDepth()
{
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
    bool processedFrame = true;

	//Get RGB data
	HRESULT hr = m_Sensor->NuiImageStreamGetNextFrame(m_DepthStream, 0, &imageFrame);

    if ( FAILED(hr) )
	{
		if(hr == S_FALSE)
		{
			return false;
		}
		if(hr == E_INVALIDARG)
		{
			return false;
		}
		if(hr == E_NUI_FRAME_NO_DATA)
		{
			return true;
		}
		if(hr == E_POINTER)
		{
			return false;
		}
		return false;
	}

	//set up dynamic mapping
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);


	if (LockedRect.Pitch != 0)
    {
        const USHORT* curr = (const USHORT*) LockedRect.pBits;
        const USHORT* dataEnd = curr + (width*height);

		ID3D10Texture2D* tex = m_Depthdata->GetTextureEdit();

		D3D10_MAPPED_TEXTURE2D mappedTex;
		tex->Map( D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex );


		UCHAR* pTexels = (UCHAR*)mappedTex.pData;


		int alpha = 255;

		// Copy over depth values and convert into greyscale image format
		while (curr < dataEnd) 
		{
			// Get depth in millimeters
            USHORT depth = NuiDepthPixelToDepth(*curr++);

            // Draw a grayscale image of the depth:
            // B,G,R are all set to depth%256, alpha set to 1.
            for (int i = 0; i < 3; ++i)
                *pTexels++ = (BYTE)( (depth%256));
            *pTexels++ = 255;
        }


		tex->Unmap( D3D10CalcSubresource(0, 0, 1) );
    }

	texture->UnlockRect(0);
    m_Sensor->NuiImageStreamReleaseFrame(m_DepthStream, &imageFrame);

	return true;
}

bool KinectClass::GetKinectSkeleton()
{

    bool foundSkeleton = false;
	int skel = 0;

	// Get Skeleton Frame Data 
    if ( SUCCEEDED(m_Sensor->NuiSkeletonGetNextFrame( 0, &m_SkeletonFrame )) )
    {
        for ( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )
        {
            NUI_SKELETON_TRACKING_STATE trackingState = m_SkeletonFrame.SkeletonData[i].eTrackingState;

            if ( trackingState == NUI_SKELETON_TRACKED || trackingState == NUI_SKELETON_POSITION_ONLY )
            {
				skel = i; // Set to track first skeleton we find
                foundSkeleton = true;
            }
        }
    }

    // no skeletons!
    if( !foundSkeleton )
    {
        return true;
    }

    // smooth out the skeleton data
    HRESULT hr = m_Sensor->NuiTransformSmooth(&m_SkeletonFrame,NULL);
    if ( FAILED(hr) )
    {
        return false;
    }

	m_Skeleton->UpdateSkeleton(m_SkeletonFrame.SkeletonData[skel]);


	//Tempory data to test if skeleton data is functioninf properly 
	x = m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x;
	y = m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y;
	z = m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z;

	ofstream myfile ("example.txt", ios::app);
	if (myfile.is_open())
	{
		INT64 currentTime;

		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

		myfile << currentTime;
		myfile << " : " ;

		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x;
		myfile << " ";
		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y;
		myfile << " ";
		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z;
		myfile << " : ";

		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].x;
		myfile << " ";
		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].y;
		myfile << " ";
		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].z;
		myfile << " : ";

		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].x;
		myfile << " ";
		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].y;
		myfile << " ";
		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].z;
		myfile << " : ";

		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x;
		myfile << " ";
		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y;
		myfile << " ";
		myfile << m_SkeletonFrame.SkeletonData[skel].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z;
		//myfile << " : ";

		myfile << endl; 

		myfile.close();
	}
	else
	{
		return false;
	}
	

    return true;
}

bool KinectClass::OutputToFile()
{
	ofstream myfile ("example.txt", ios::app);
	if (myfile.is_open())
	{
		myfile << "This is a line.\n";
		myfile.close();
	}
	else
	{
		return false;
	}
}