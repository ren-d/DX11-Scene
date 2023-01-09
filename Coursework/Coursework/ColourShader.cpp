#include "ColourShader.h"

ColourShader::ColourShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"colour_vs.cso", L"colour_ps.cso");
}

ColourShader::~ColourShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void ColourShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC colourBufferDesc;


	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	colourBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	colourBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	colourBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colourBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colourBufferDesc.MiscFlags = 0;
	colourBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&colourBufferDesc, NULL, &colourBuffer);


}


void ColourShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, XMFLOAT4 colour)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Send matrix data
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	ColourBufferType* colourPtr;
	result = deviceContext->Map(colourBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	colourPtr = (ColourBufferType*)mappedResource.pData;

	colourPtr->colour = colour;

	deviceContext->Unmap(colourBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &colourBuffer);
	// Set shader texture and sampler resource in the pixel shader.

}





