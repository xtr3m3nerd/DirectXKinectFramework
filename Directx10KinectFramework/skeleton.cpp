#include "skeleton.h"


//Needs lots of work! 

SkeletonClass::SkeletonClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_SkeletonModel = 0;
}


SkeletonClass::SkeletonClass(const SkeletonClass& other)
{
}


SkeletonClass::~SkeletonClass()
{
}


bool SkeletonClass::Initialize(ID3D10Device* device)
{
	bool result;


	// Load in the Skeleton data.
	result = InitialSkeleton();
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}


void SkeletonClass::Shutdown()
{
	// Release the vertex and index buffers.
	ShutdownBuffers();

	// Release the Skeleton data.
	ReleaseSkeleton();

	return;
}


void SkeletonClass::Render(ID3D10Device* device)
{
	UpdateBuffers(device);
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(device);

	return;
}

bool SkeletonClass::UpdateSkeleton( const NUI_SKELETON_DATA & skel)
{
	m_Skeleton = skel;

	for(int i = 0; i < 20; i++)
	{
		m_SkeletonModel[i].x = m_Skeleton.SkeletonPositions[i].x;
		m_SkeletonModel[i].y = m_Skeleton.SkeletonPositions[i].y;
		m_SkeletonModel[i].z = m_Skeleton.SkeletonPositions[i].z;
	}
	return true; 
}

int SkeletonClass::GetIndexCount()
{
	return m_indexCount;
}


bool SkeletonClass::InitializeBuffers(ID3D10Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	
	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}


	//Build basic skeleton model to start 
	vertices[0].position.x = 0; vertices[0].position.y = 0; vertices[0].position.z = 0;
	vertices[1].position.x = 0; vertices[1].position.y = 0.5; vertices[1].position.z = 0;
	vertices[2].position.x = 0; vertices[2].position.y = 1; vertices[2].position.z = 0;
	vertices[3].position.x = 0; vertices[3].position.y = 1.5; vertices[3].position.z = 0;
	vertices[4].position.x = 0.3; vertices[4].position.y = 1; vertices[4].position.z = 0;
	vertices[5].position.x = 0.7; vertices[5].position.y = 1; vertices[5].position.z = 0;
	vertices[6].position.x = 1; vertices[6].position.y = 1; vertices[6].position.z = 0;
	vertices[7].position.x = 1.2; vertices[7].position.y = 1; vertices[7].position.z = 0;
	vertices[8].position.x = -0.3; vertices[8].position.y = 1; vertices[8].position.z = 0;
	vertices[9].position.x = -0.7; vertices[9].position.y = 1; vertices[9].position.z = 0;
	vertices[10].position.x = -1; vertices[10].position.y = 1; vertices[10].position.z = 0;
	vertices[11].position.x = -1.2; vertices[11].position.y = 1; vertices[11].position.z = 0;
	vertices[12].position.x = 0.3; vertices[12].position.y = 0; vertices[12].position.z = 0;
	vertices[13].position.x = 0.3; vertices[13].position.y = -0.5; vertices[13].position.z = 0;
	vertices[14].position.x = 0.3; vertices[14].position.y = -1; vertices[14].position.z = 0;
	vertices[15].position.x = 0.3; vertices[15].position.y = -1; vertices[15].position.z = 0;
	vertices[16].position.x = -0.3; vertices[16].position.y = 0; vertices[16].position.z = 0;
	vertices[17].position.x = -0.3; vertices[17].position.y = -0.5; vertices[17].position.z = 0;
	vertices[18].position.x = -0.3; vertices[18].position.y = -1; vertices[18].position.z = 0;
	vertices[19].position.x = -0.3; vertices[19].position.y = -1; vertices[19].position.z = 0;


	
	vertices[8].position.x = 0.226816; vertices[8].position.y = 0.499682; vertices[8].position.z = 2.57746;
	vertices[9].position.x = 0.486843; vertices[9].position.y = 0.397854; vertices[9].position.z = 2.53366;
	vertices[10].position.x = 0.728001; vertices[10].position.y = 0.426659; vertices[10].position.z = 2.45857;
	vertices[11].position.x = 0.785332; vertices[11].position.y = 0.437153; vertices[11].position.z = 2.41497;

	for(int i = 0; i < 20 ;i++)
	{
		vertices[i].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	}

	indices[0] = 0; indices[1] = 1;
	indices[2] = 1; indices[3] = 2;
	indices[4] = 2; indices[5] = 3;
	indices[6] = 2; indices[7] = 4;
	indices[8] = 4; indices[9] = 5;
	indices[10] = 5; indices[11] = 6;
	indices[12] = 6; indices[13] = 7;
	indices[14] = 2; indices[15] = 8;
	indices[16] = 8; indices[17] = 9;
	indices[18] = 9; indices[19] = 10;
	indices[20] = 10; indices[21] = 11;
	indices[22] = 0; indices[23] = 12;
	indices[24] = 12; indices[25] = 13;
	indices[26] = 13; indices[27] = 14;
	indices[28] = 14; indices[29] = 15;
	indices[30] = 0; indices[31] = 16;
	indices[32] = 16; indices[33] = 17;
	indices[34] = 17; indices[35] = 18;
	indices[36] = 18; indices[37] = 19;


	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
    indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void SkeletonClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void SkeletonClass::RenderBuffers(ID3D10Device* device)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
    stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	device->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    device->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}

bool SkeletonClass::UpdateBuffers(ID3D10Device* device)
{

	VertexType* vertices;
	void* verticesPtr;
	HRESULT result;

	if((m_SkeletonModel[0].x == 0) && (m_SkeletonModel[0].y == 0) && (m_SkeletonModel[0].z == 0))
	{
		return true; 
	}
	
	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	for(DWORD i = 0; i < m_vertexCount; ++i) 
	{
		vertices[i].position = D3DXVECTOR3(m_SkeletonModel[i].x, m_SkeletonModel[i].y, m_SkeletonModel[i].z);
		vertices[i].color =  D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f); 

		if( i >= 8 && i <= 11)
		{
			vertices[i].color =  D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f); 
		}

		if(m_Skeleton.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_INFERRED)
		{
			vertices[i].color =  D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		}

		if(m_Skeleton.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_NOT_TRACKED)
		{
			vertices[i].color =  D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	// Initialize the vertex buffer pointer to null first.
	verticesPtr = 0;

	// Lock the vertex buffer.
	result = m_vertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&verticesPtr);
	if(FAILED(result))
	{
		return false;
	}

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	m_vertexBuffer->Unmap();

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


bool SkeletonClass::InitialSkeleton()
{
	m_vertexCount = 20;
	m_indexCount = 38; 

	m_SkeletonModel = new ModelType[m_vertexCount];
	if(!m_SkeletonModel)
	{
		return false;
	}

	//Build basic skeleton model to start 
	m_SkeletonModel[0].x = 0; m_SkeletonModel[0].y = 0; m_SkeletonModel[0].z = 0;
	m_SkeletonModel[1].x = 0; m_SkeletonModel[1].y = 0.5; m_SkeletonModel[1].z = 0;
	m_SkeletonModel[2].x = 0; m_SkeletonModel[2].y = 1; m_SkeletonModel[2].z = 0;
	m_SkeletonModel[3].x = 0; m_SkeletonModel[3].y = 1.5; m_SkeletonModel[3].z = 0;
	m_SkeletonModel[4].x = 0.3; m_SkeletonModel[4].y = 1; m_SkeletonModel[4].z = 0;
	m_SkeletonModel[5].x = 0.7; m_SkeletonModel[5].y = 1; m_SkeletonModel[5].z = 0;
	m_SkeletonModel[6].x = 1; m_SkeletonModel[6].y = 1; m_SkeletonModel[6].z = 0;
	m_SkeletonModel[7].x = 1.2; m_SkeletonModel[7].y = 1; m_SkeletonModel[7].z = 0;
	m_SkeletonModel[8].x = -0.3; m_SkeletonModel[8].y = 1; m_SkeletonModel[8].z = 0;
	m_SkeletonModel[9].x = -0.7; m_SkeletonModel[9].y = 1; m_SkeletonModel[9].z = 0;
	m_SkeletonModel[10].x = -1; m_SkeletonModel[10].y = 1; m_SkeletonModel[10].z = 0;
	m_SkeletonModel[11].x = -1.2; m_SkeletonModel[11].y = 1; m_SkeletonModel[11].z = 0;
	m_SkeletonModel[12].x = 0.3; m_SkeletonModel[12].y = 0; m_SkeletonModel[12].z = 0;
	m_SkeletonModel[13].x = 0.3; m_SkeletonModel[13].y = -0.5; m_SkeletonModel[13].z = 0;
	m_SkeletonModel[14].x = 0.3; m_SkeletonModel[14].y = -1; m_SkeletonModel[14].z = 0;
	m_SkeletonModel[15].x = 0.3; m_SkeletonModel[15].y = -1; m_SkeletonModel[15].z = 0;
	m_SkeletonModel[16].x = -0.3; m_SkeletonModel[16].y = 0; m_SkeletonModel[16].z = 0;
	m_SkeletonModel[17].x = -0.3; m_SkeletonModel[17].y = -0.5; m_SkeletonModel[17].z = 0;
	m_SkeletonModel[18].x = -0.3; m_SkeletonModel[18].y = -1; m_SkeletonModel[18].z = 0;
	m_SkeletonModel[19].x = -0.3; m_SkeletonModel[19].y = -1; m_SkeletonModel[19].z = 0;


	return true;
}

void SkeletonClass::ReleaseSkeleton()
{
	if(m_SkeletonModel)
	{
		delete [] m_SkeletonModel;
		m_SkeletonModel = 0;
	}

	return;
}