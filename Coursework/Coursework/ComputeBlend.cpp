#include "ComputeBlend.h"

ComputeBlend::ComputeBlend(ID3D11Device* device, HWND hwnd, int w, int h) : BaseShader(device, hwnd)
{
	// set initial values
	sWidth = w;
	sHeight = h;
	initShader(L"computeBlendTextures_cs.cso", NULL);
}

ComputeBlend::~ComputeBlend() // release heap allocated data
{
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

	if (blendBuffer)
	{
		blendBuffer->Release();
		blendBuffer = 0;
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

	if (m_srvTexOutput)
	{
		m_srvTexOutput->Release();
		m_srvTexOutput = 0;
	}

}

void ComputeBlend::initShader(const wchar_t* cfile, const wchar_t* blank)
{
	loadComputeShader(cfile);
	createOutputUAV();

	D3D11_BUFFER_DESC blendBufferDesc;

	blendBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	blendBufferDesc.ByteWidth = sizeof(BlendingBufferType);
	blendBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	blendBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	blendBufferDesc.MiscFlags = 0;
	blendBufferDesc.StructureByteStride = 0;


	renderer->CreateBuffer(&blendBufferDesc, NULL, &blendBuffer);
}

void ComputeBlend::createOutputUAV()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = sWidth / 1.1;
	textureDesc.Height = sHeight / 1.1;
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

void ComputeBlend::setShaderParameters(ID3D11DeviceContext* dc, ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2, float intensity, float gamma)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	BlendingBufferType* intenPtr;
	dc->Map(blendBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	intenPtr = (BlendingBufferType*)mappedResource.pData;
	intenPtr->blending = XMFLOAT4(intensity,gamma,0,0);


	dc->Unmap(blendBuffer, 0);
	dc->CSSetConstantBuffers(0, 1, &blendBuffer);

	dc->CSSetShaderResources(0, 1, &texture1);
	dc->CSSetShaderResources(1, 1, &texture2);
	dc->CSSetUnorderedAccessViews(0, 1, &m_uavAccess, 0);
}

void ComputeBlend::unbind(ID3D11DeviceContext* dc)
{
	ID3D11ShaderResourceView* nullSRV[] = { NULL };
	dc->CSSetShaderResources(0, 1, nullSRV);

	// Unbind output from compute shader
	ID3D11UnorderedAccessView* nullUAV[] = { NULL };
	dc->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

	// Disable Compute Shader
	dc->CSSetShader(nullptr, nullptr, 0);
}