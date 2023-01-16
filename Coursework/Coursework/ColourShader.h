// Baisc shader for creating a basic mesh with colour (no lighting)
#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class ColourShader : public BaseShader
{
public:
	// buffer definitions (for size calculation)
	struct ColourBufferType
	{
		XMFLOAT4 colour;
	};

	ColourShader(ID3D11Device* device, HWND hwnd);
	~ColourShader();

	void setShaderParameters(
		ID3D11DeviceContext* deviceContext,
		const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection,
		XMFLOAT4 colour
	);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* colourBuffer;
	ID3D11SamplerState* sampleState;

};

