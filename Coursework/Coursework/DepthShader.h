// Depth Map Shader
// used to create a depth map of a static object
// used for shadow mapping

#pragma once
#include "DXF.h"
#include "structsforuse.h"

class DepthShader :
    public BaseShader
{
public:

	DepthShader(ID3D11Device* device, HWND hwnd);
	~DepthShader();

	void setShaderParameters(
		ID3D11DeviceContext* deviceContext, 
		const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection
	);



private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:

	ID3D11Buffer* matrixBuffer;
};

