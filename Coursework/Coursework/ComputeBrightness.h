#pragma once
// Brightness Map Compute Shader class
// takes a texture and returns only the brightest values
// used for bloom calculations

#include "DXF.h"

using namespace std;
using namespace DirectX;

class ComputeBrightness :
	public BaseShader
{
public:

	// buffer definitions (for size calculation)
	struct ThresholdBufferType
	{
		XMFLOAT4 threshold;
	};
	ComputeBrightness(ID3D11Device* device, HWND hwnd, int w, int h);
	~ComputeBrightness();

	void setShaderParameters(ID3D11DeviceContext* dc, ID3D11ShaderResourceView* texture1, float threshold);
	void createOutputUAV();
	ID3D11ShaderResourceView* getSRV() { return m_srvTexOutput; };
	void unbind(ID3D11DeviceContext* dc);


private:
	void initShader(const wchar_t* cfile, const wchar_t* blank);

	ID3D11Buffer* thresholdBuffer;

	ID3D11ShaderResourceView* srv;
	ID3D11UnorderedAccessView* uav;

	// texture set
	ID3D11Texture2D* m_tex;
	ID3D11UnorderedAccessView* m_uavAccess;
	ID3D11ShaderResourceView* m_srvTexOutput;

	int sWidth;
	int sHeight;
};

