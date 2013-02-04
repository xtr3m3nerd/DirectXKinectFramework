#include "graphics.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model1 = 0;
	m_Model2 = 0;
	m_ColorShader = 0;
	m_TextureShader = 0;
	m_TransparentShader = 0;
	m_Text = 0;
	m_Bitmap = 0;

	m_Kinect = 0;

	cameraPos = -3.0f;

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

		
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);


	// Create the first model object.
	m_Model1 = new ModelClass;
	if(!m_Model1)
	{
		return false;
	}

	// Initialize the first model object.
	result = m_Model1->Initialize(m_D3D->GetDevice(), L"../Directx10KinectFramework/data/dirt01.dds", "../Directx10KinectFramework/data/square.txt");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the first model object.", L"Error", MB_OK);
		return false;
	}

	// Create the second model object.
	m_Model2 = new ModelClass;
	if(!m_Model2)
	{
		return false;
	}

	// Initialize the second model object.
	result = m_Model2->Initialize(m_D3D->GetDevice(), L"../Directx10KinectFramework/data/stone01.dds", "../Directx10KinectFramework/data/square.txt");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the second model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the transparent shader object.
	m_TransparentShader = new TransparentShaderClass;
	if(!m_TransparentShader)
	{
		return false;
	}

	// Initialize the transparent shader object.
	result = m_TransparentShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the transparent shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Directx10KinectFramework/data/seafloor.dds", 256, 256);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	
	// Create the kinect object.
	m_Kinect = new KinectClass;
	if(!m_Kinect)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Kinect->Initialize(m_D3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the kinect object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	
	// Release the bitmap object.
	if(m_Kinect)
	{
		m_Kinect->Shutdown();
		delete m_Kinect;
		m_Kinect = 0;
	}

	// Release the bitmap object.
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the transparent shader object.
	if(m_TransparentShader)
	{
		m_TransparentShader->Shutdown();
		delete m_TransparentShader;
		m_TransparentShader = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the color shader object.
	if(m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the second model object.
	if(m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	// Release the first model object.
	if(m_Model1)
	{
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(int fps, int cpu, float frameTime, InputClass* input)
{
	bool result;


	// Set the frames per second.
	result = m_Text->SetFps(fps);
	if(!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu);
	if(!result)
	{
		return false;
	}
	
	//Update the Kinect
	result = m_Kinect->Frame(input);
	if(!result)
	{
		return false;
	}

	// Test Skeleton hand position 
	result = m_Text->SetPos(m_Kinect->Getx(), input->MouseY(), cameraPos);
	if(!result)
	{
		return false;
	}

	if(input->IsKeyDown('Y'))        cameraPos -= 0.01;
	if(input->IsKeyDown('H'))        cameraPos += 0.01;

	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	float blendAmount;


	// Set the blending amount to 50%.
	blendAmount = 0.5f;

	// Clear the scene to the color of the fog.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	

	//D3DXMatrixTranslation(&worldMatrix, -0.0f, 0.0f, 6.0f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Model1->Render(m_D3D->GetDevice());


	// Render the model using the texture shader.
	//m_TextureShader->Render(m_D3D->GetDevice(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model1->GetTexture());

	// Translate to the right by one unit and towards the camera by one unit.
	//D3DXMatrixTranslation(&worldMatrix, 1.0f, 0.0f, 5.0f);

	// Put the second square model on the graphics pipeline.
	//m_Model2->Render(m_D3D->GetDevice());

	// Render the second square model with the stone texture and use the 50% blending amount for transparency.
	//m_TransparentShader->Render(m_D3D->GetDevice(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model2->GetTexture(), blendAmount);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Use to test 2D graphical output
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing
	//m_Bitmap->Render(m_D3D->GetDevice(), 300, 100);

	// Render the bitmap using the texture shader.
	//m_TextureShader->Render(m_D3D->GetDevice(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//Render the Kinect 
	m_Kinect->Render(m_D3D->GetDevice(), 0);// RGB

	m_TextureShader->Render(m_D3D->GetDevice(), 6, worldMatrix, viewMatrix, projectionMatrix, m_Kinect->GetShaderResourceViewRGB());

	m_Kinect->Render(m_D3D->GetDevice(), 1);// Depth

	m_TransparentShader->Render(m_D3D->GetDevice(), 6, worldMatrix, viewMatrix, projectionMatrix, m_Kinect->GetShaderResourceViewDepth(),m_Kinect->GetDepthBlend());


	// Render the text strings.
	m_Text->Render(m_D3D->GetDevice(), worldMatrix, orthoMatrix);


	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	m_Camera->SetPosition(0.0f, 0.0f, cameraPos);
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Model1->Render(m_D3D->GetDevice());


	// Render the model using the texture shader.
	//m_TextureShader->Render(m_D3D->GetDevice(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model1->GetTexture());


	//Render the Skeleton
	m_Kinect->Render(m_D3D->GetDevice(), 2);// Skeleton
	m_ColorShader->Render(m_D3D->GetDevice(), 38, worldMatrix, viewMatrix, projectionMatrix);

	
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

