#include "Cube.h"

struct Vertex
{
	XMFLOAT3 pos;
};

Vertex vertices[] =
{
	XMFLOAT3(-1.0f,  1.0f, -1.0f),
	XMFLOAT3(1.0f,  1.0f, -1.0f),
	XMFLOAT3(1.0f,  1.0f,  1.0f),
	XMFLOAT3(-1.0f,  1.0f,  1.0f),

	XMFLOAT3(-1.0f, -1.0f, -1.0f),
	XMFLOAT3(1.0f, -1.0f, -1.0f),
	XMFLOAT3(1.0f, -1.0f,  1.0f),
	XMFLOAT3(-1.0f, -1.0f,  1.0f),

	XMFLOAT3(-1.0f, -1.0f,  1.0f),
	XMFLOAT3(-1.0f, -1.0f, -1.0f),
	XMFLOAT3(-1.0f,  1.0f, -1.0f),
	XMFLOAT3(-1.0f,  1.0f,  1.0f),

	XMFLOAT3(1.0f, -1.0f,  1.0f),
	XMFLOAT3(1.0f, -1.0f, -1.0f),
	XMFLOAT3(1.0f,  1.0f, -1.0f),
	XMFLOAT3(1.0f,  1.0f,  1.0f),

	XMFLOAT3(-1.0f, -1.0f, -1.0f),
	XMFLOAT3(1.0f, -1.0f, -1.0f),
	XMFLOAT3(1.0f,  1.0f, -1.0f),
	XMFLOAT3(-1.0f,  1.0f, -1.0f),

	XMFLOAT3(-1.0f, -1.0f,  1.0f),
	XMFLOAT3(1.0f, -1.0f,  1.0f),
	XMFLOAT3(1.0f,  1.0f,  1.0f),
	XMFLOAT3(-1.0f,  1.0f,  1.0f)
};

WORD indices[] =
{
	3,   1,  0,  2,  1,  3,
	6,   4,  5,  7,  4,  6,
	11,  9,  8, 10,  9, 11,
	14, 12, 13, 15, 12, 14,
	19, 17, 16, 18, 17, 19,
	22, 20, 21, 23, 20, 22
};

HRESULT hr;

Cube::Cube()
{
	m_pVS = NULL;
	m_pPS = NULL;
	m_pInputLayout = NULL;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pColorMap = NULL;
	m_pColorMapSampler = NULL;
	m_pViewCB = NULL;
	m_pProjCB = NULL;
	m_pWorldCB = NULL;

	m_rotationX = 0.0f;
	m_rotationY = 0.5f;
	m_rotationZ = 0.0f;

	m_translationX = 0.0f;
	m_translationY = 0.0f;
	m_translationZ = 4.0f;

	m_scaleX = 0.3f;
	m_scaleY = 0.3f;
	m_scaleZ = 0.3f;

}

Cube::~Cube()
{
}

bool Cube::LoadContent()
{
	ID3DBlob* v_shader = this->SetVertexShader("cube_shader.fx");
	D3D11_SUBRESOURCE_DATA v_buffer = this->SetVertexBuffer();

	SetInputLayout(v_shader);
	SetPixelShader("cube_shader.fx");
	SetIndexBuffer(v_buffer);
	//LoadTexture((LPCWSTR)"borg.dds");
	SetConstBuffer();
	InitMatrixes();

	return true;
}

void Cube::UnloadContent()
{
	m_pVS = NULL;
	m_pPS = NULL;
	m_pInputLayout = NULL;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pColorMap = NULL;
	m_pColorMapSampler = NULL;
	m_pViewCB = NULL;
	m_pProjCB = NULL;
	m_pWorldCB = NULL;


}

void Cube::Update()
{
	// Position cube in the world
	
	/*m_rotation = XMMatrixRotationRollPitchYaw(m_rotationX, m_rotationY, m_rotationZ);
	m_translation = XMMatrixTranslation(m_translationX, m_translationY, m_translationZ);
	XMMATRIX w_pos = XMMatrixTranspose(XMMatrixTranspose(m_rotation * m_translation));*/

	//XMMATRIX r = XMMatrixRotationRollPitchYaw(m_rotationX, m_rotationY, m_rotationZ);
	/*float y = 0.00004f;
	float x = 0.00004f;*/
	
	Scale(0.3f, 0.4f, 0.5f);

	//Rotate(x, y, 0.0f);
}

void Cube::Render()
{
	// Check if D3D is ready
	if (m_pD3DContext == NULL)
		return;

	// Clear back buffer
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pD3DContext->ClearRenderTargetView(m_pD3DRenderTargetView, color);
	m_pD3DContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Stride and offset
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Set vertex buffer
	m_pD3DContext->IASetInputLayout(m_pInputLayout);
	m_pD3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pD3DContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set shaders
	m_pD3DContext->VSSetShader(m_pVS, 0, 0);
	m_pD3DContext->PSSetShader(m_pPS, 0, 0);
	m_pD3DContext->PSSetShaderResources(0, 1, &m_pColorMap);
	m_pD3DContext->PSSetSamplers(0, 1, &m_pColorMapSampler);

	
	// Draw triangles
	m_pD3DContext->DrawIndexed(36, 0, 0);

	// Present back buffer to display
	m_pSwapChain->Present(0, 0);
}

void Cube::Rotate(float X, float Y, float Z)
{
	m_rotationX += X;
	m_rotationY += Y;
	m_rotationZ += Z;

	UpdateConstBuffer(GetWorldPosition());
}

void Cube::Scale(float X, float Y, float Z)
{
	m_scaleX = X;
	m_scaleY = Y;
	m_scaleZ = Z;

	UpdateConstBuffer(GetWorldPosition());
}

ID3DBlob* Cube::SetVertexShader(LPCSTR FilePath)
{
	ID3DBlob* pVSBuffer = NULL;
	bool res = CompileShader(FilePath, "VS_Main", "vs_4_0", &pVSBuffer);
	if (res == false) {
		::MessageBox(m_hWnd, TEXT("Unable to load vertex shader"), TEXT("ERROR"), MB_OK);
		return false;
	}

	// Create vertex shader
	HRESULT hr;
	hr = m_pD3DDevice->CreateVertexShader(
		pVSBuffer->GetBufferPointer(),
		pVSBuffer->GetBufferSize(),
		0, &m_pVS);
	if (FAILED(hr)) {
		if (pVSBuffer)
			pVSBuffer->Release();
		return nullptr;
	}

	return pVSBuffer;
}

bool Cube::SetPixelShader(LPCSTR FilePath)
{
	// Compile pixel shader
	ID3DBlob* pPSBuffer = NULL;
	bool res = CompileShader(FilePath, "PS_Main", "ps_4_0", &pPSBuffer);
	if (res == false) {
		::MessageBox(m_hWnd, TEXT("Unable to load pixel shader"), TEXT("ERROR"), MB_OK);
		return false;
	}

	// Create pixel shader
	hr = m_pD3DDevice->CreatePixelShader(
		pPSBuffer->GetBufferPointer(),
		pPSBuffer->GetBufferSize(),
		0, &m_pPS);
	if (FAILED(hr)) {
		return false;
	}

	// Cleanup PS buffer
	pPSBuffer->Release();
	pPSBuffer = NULL;

	return true;
}

bool Cube::SetInputLayout(ID3DBlob* pVSBuffer)
{
	// Define input layout
	D3D11_INPUT_ELEMENT_DESC shaderInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numLayoutElements = ARRAYSIZE(shaderInputLayout);

	// Create input layout
	hr = m_pD3DDevice->CreateInputLayout(
		shaderInputLayout, numLayoutElements,
		pVSBuffer->GetBufferPointer(),
		pVSBuffer->GetBufferSize(),
		&m_pInputLayout);
	if (FAILED(hr)) {
		return false;
	}

	// Release VS buffer
	pVSBuffer->Release();
	pVSBuffer = NULL;

	return true;
}

D3D11_SUBRESOURCE_DATA Cube::SetVertexBuffer()
{
	// Vertex description
	D3D11_BUFFER_DESC vertexDesc;
	::ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(Vertex) * 24;

	// Resource data
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	// Create vertex buffer
	hr = m_pD3DDevice->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);
	if (FAILED(hr)) {
		::MessageBox(m_hWnd, TEXT("Unable to set vertex buffer"), TEXT("ERROR"), MB_OK);
	}

	return resourceData;
}

bool Cube::SetIndexBuffer(D3D11_SUBRESOURCE_DATA resourceData)
{
	// Create index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	::ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(WORD) * 36;
	indexBufferDesc.CPUAccessFlags = 0;
	resourceData.pSysMem = indices;
	hr = m_pD3DDevice->CreateBuffer(&indexBufferDesc, &resourceData, &m_pIndexBuffer);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool Cube::LoadTexture(LPCWSTR src_path)
{
	// Load texture
	HRESULT hr = ::D3DX11CreateShaderResourceViewFromFile(
		m_pD3DDevice, (LPCSTR)src_path, 0, 0, &m_pColorMap, 0);
	if (FAILED(hr)) {
		::MessageBox(m_hWnd, (LPCSTR)"Unable to load texture", (LPCSTR)"ERROR", MB_OK);
		return false;
	}

	// Texture sampler
	D3D11_SAMPLER_DESC textureDesc;
	::ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	textureDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	textureDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	textureDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	textureDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	textureDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_pD3DDevice->CreateSamplerState(&textureDesc, &m_pColorMapSampler);

	if (FAILED(hr)) {
		::MessageBox(m_hWnd, (LPCSTR)"Unable to create texture sampler state", (LPCSTR)"ERROR", MB_OK);
		return false;
	}

	return true;
}

bool Cube::SetConstBuffer()
{
	// Create constant buffers (see fx-file)
	D3D11_BUFFER_DESC constBufferDesc;
	::ZeroMemory(&constBufferDesc, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_pD3DDevice->CreateBuffer(&constBufferDesc, 0, &m_pViewCB);
	if (FAILED(hr)) {
		return false;
	}
	hr = m_pD3DDevice->CreateBuffer(&constBufferDesc, 0, &m_pProjCB);
	if (FAILED(hr)) {
		return false;
	}
	hr = m_pD3DDevice->CreateBuffer(&constBufferDesc, 0, &m_pWorldCB);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool Cube::InitMatrixes()
{
	// Initialize matrixes
	m_viewMatrix = XMMatrixIdentity();
	m_projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, 640.0f / 480.0f, 0.01f, 100.0f);
	m_viewMatrix = XMMatrixTranspose(m_viewMatrix);
	m_projMatrix = XMMatrixTranspose(m_projMatrix);

	return true;
}

void Cube::UpdateConstBuffer(XMMATRIX w_pos)
{
	// Update constant buffers
	m_pD3DContext->UpdateSubresource(m_pWorldCB, 0, 0, &w_pos, 0, 0);
	m_pD3DContext->UpdateSubresource(m_pViewCB, 0, 0, &m_viewMatrix, 0, 0);
	m_pD3DContext->UpdateSubresource(m_pProjCB, 0, 0, &m_projMatrix, 0, 0);

	// Upload constant buffers to GPU
	m_pD3DContext->VSSetConstantBuffers(0, 1, &m_pWorldCB);
	m_pD3DContext->VSSetConstantBuffers(1, 1, &m_pViewCB);
	m_pD3DContext->VSSetConstantBuffers(2, 1, &m_pProjCB);
}

XMMATRIX Cube::GetWorldPosition()
{
	XMMATRIX r = XMMatrixRotationRollPitchYaw(m_rotationX, m_rotationY, m_rotationZ);
	XMMATRIX t = XMMatrixTranslation(m_translationX, m_translationY, m_translationZ);
	XMMATRIX s = XMMatrixScaling(m_scaleX, m_scaleY, m_scaleZ);
	return XMMatrixTranspose(r * t * s);
}
