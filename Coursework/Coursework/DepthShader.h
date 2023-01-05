#pragma once
#include "DXF.h"
#include "structsforuse.h"

class DepthShader :
    public BaseShader
{
public:

	struct WaterBufferType
	{
		XMFLOAT4 isWater; 
		XMFLOAT4 waves[4];
		XMFLOAT4 timeInSeconds;
	};

	DepthShader(ID3D11Device* device, HWND hwnd);
	~DepthShader();

	void setShaderParameters(
		ID3D11DeviceContext* deviceContext, 
		const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection
	);

	void setShaderParameters(
		ID3D11DeviceContext* deviceContext, 
		const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, 
		Wave* waves[4], 
		float timeInSeconds
	);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* waterBuffer;
	ID3D11Buffer* matrixBuffer;
};

