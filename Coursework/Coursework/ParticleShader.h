#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class ParticleShader : public BaseShader
{


public:

	ParticleShader(ID3D11Device* device, HWND hwnd);
	~ParticleShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);
	void initShader(const wchar_t* vs, const wchar_t* gs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;


};