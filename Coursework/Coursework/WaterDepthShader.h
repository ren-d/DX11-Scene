#pragma once
#include "DXF.h"
#include "structsforuse.h"

class WaterDepthShader :
	public BaseShader
{
public:

	struct WaterBufferType
	{
		XMFLOAT4 waves[4];
		XMFLOAT4 timeInSeconds;
	};

	struct TessellationBufferType
	{
		XMFLOAT4 tessellation;
		XMFLOAT4 camera;
		XMFLOAT4 cameraDir;
	};

	WaterDepthShader(ID3D11Device* device, HWND hwnd);
	~WaterDepthShader();

	void setShaderParameters(
		ID3D11DeviceContext* deviceContext,
		const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection,
		Wave* waves[4],
		float timeInSeconds,
		Camera* camera
	);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);
	void initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename);

private:
	ID3D11Buffer* waterBuffer;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* tessellationBuffer;
};

