#include "ComputeBrightness.h"

ComputeBrightness::ComputeBrightness(ID3D11Device* device, HWND hwnd, int w, int h) : BaseShader(device, hwnd)
{
	sWidth = w;
	sHeight = h;
	initShader(L"brightnessMap_cs.cso", NULL);
}

ComputeBrightness::~ComputeBrightness()
{
	// release heap allocated data

	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	if (thresholdBuffer)
	{
		thresholdBuffer->Release();
		thresholdBuffer = 0;
	}


	if (srv)
	{
		srv->Release();
		srv = 0;
	}


	if (uav)
	{
		uav->Release();
		uav = 0;
	}


	if (m_tex)
	{
		m_tex->Release();
		m_tex = 0;
	}


	if (m_uavAccess)
	{
		m_uavAccess->Release();
		m_uavAccess = 0;
	}


	if (m_srvTexOutput)
	{
		m_srvTexOutput->Release();
		m_srvTexOutput = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void ComputeBrightness::initShader(const wchar_t* cfile, const wchar_t* blank)
{
	loadComputeShader(cfile);
	createOutputUAV();

	// buffer descriptions
	D3D11_BUFFER_DESC thresholdBufferDesc;

	thresholdBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	thresholdBufferDesc.ByteWidth = sizeof(ThresholdBufferType);
	thresholdBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	thresholdBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	thresholdBufferDesc.MiscFlags = 0;
	thresholdBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&thresholdBufferDesc, 0, &thresholdBuffer);
}

void ComputeBrightness::createOutputUAV() // create unordered acces view 
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = sWidth;
	textureDesc.Height = sHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_tex = 0;
	renderer->CreateTexture2D(&textureDesc, 0, &m_tex);

	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
	ZeroMemory(&descUAV, sizeof(descUAV));
	descUAV.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; ;// DXGI_FORMAT_UNKNOWN;
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	descUAV.Texture2D.MipSlice = 0;
	renderer->CreateUnorderedAccessView(m_tex, &descUAV, &m_uavAccess);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	renderer->CreateShaderResourceView(m_tex, &srvDesc, &m_srvTexOutput);
}

void ComputeBrightness::setShaderParameters(ID3D11DeviceContext* dc, ID3D11ShaderResourceView* texture1, float threshold)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// set constant buffers
	ThresholdBufferType* threshPtr;
	dc->Map(thresholdBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	threshPtr = (ThresholdBufferType*)mappedResource.pData;
	threshPtr->threshold = XMFLOAT4(threshold, 0, 0, 0);


	dc->Unmap(thresholdBuffer, 0);
	dc->CSSetConstantBuffers(0, 1, &thresholdBuffer);

	dc->CSSetShaderResources(0, 1, &texture1);
	dc->CSSetUnorderedAccessViews(0, 1, &m_uavAccess, 0);
}

void ComputeBrightness::unbind(ID3D11DeviceContext* dc)
{
	ID3D11ShaderResourceView* nullSRV[] = { NULL };
	dc->CSSetShaderResources(0, 1, nullSRV);

	// Unbind output from compute shader
	ID3D11UnorderedAccessView* nullUAV[] = { NULL };
	dc->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

	// Disable Compute Shader
	dc->CSSetShader(nullptr, nullptr, 0);
}