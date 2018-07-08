#pragma once
#include "Base.h"

class Cube : public Base
{
// Constructors
public:
	Cube();
	virtual ~Cube();

// Public Methods
public:
	virtual bool LoadContent();
	virtual void UnloadContent();

	virtual void Update();
	virtual void Render();
	virtual void Rotate(float X, float Y, float Z);
	virtual void Scale(float X, float Y, float Z);

private:
	virtual ID3DBlob* SetVertexShader(LPCSTR FilePath);
	virtual bool SetPixelShader(LPCSTR FilePath);
	virtual bool SetInputLayout(ID3DBlob* pVSBuffer);
	virtual D3D11_SUBRESOURCE_DATA SetVertexBuffer();
	virtual bool SetIndexBuffer(D3D11_SUBRESOURCE_DATA resourceData);
	virtual bool LoadTexture(LPCWSTR src_path);
	virtual bool SetConstBuffer();
	virtual bool InitMatrixes();

	virtual void UpdateConstBuffer(XMMATRIX w_pos);
	virtual XMMATRIX GetWorldPosition();

// Members
protected:
	ID3D11VertexShader * m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11ShaderResourceView* m_pColorMap;
	ID3D11SamplerState* m_pColorMapSampler;
	ID3D11Buffer* m_pViewCB;
	ID3D11Buffer* m_pProjCB;
	ID3D11Buffer* m_pWorldCB;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projMatrix;

	float m_rotationX;
	float m_rotationY;
	float m_rotationZ;

	float m_translationX;
	float m_translationY;
	float m_translationZ;

	float m_scaleX;
	float m_scaleY;
	float m_scaleZ;

	XMMATRIX m_rotation;
	XMMATRIX m_translation;
	XMMATRIX m_scale;
};

