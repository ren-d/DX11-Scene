#include "ModelShader.h"


ModelShader::ModelShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"model_vs.cso", L"model_ps.cso");
}

ModelShader::~ModelShader() // release heap allocated data
{

	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

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

	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}

	if (cameraBuffer)
	{
		cameraBuffer->Release();
		cameraBuffer = 0;
	}

	if (shadowBuffer)
	{
		shadowBuffer->Release();
		shadowBuffer = 0;
	}

	if (miscParamBuffer)
	{
		miscParamBuffer->Release();
		miscParamBuffer = 0;
	}

	if (shadowSampleState)
	{
		shadowSampleState->Release();
		shadowSampleState = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void ModelShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC shadowBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);


	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);


	
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);


	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;


	renderer->CreateBuffer(&cameraBufferDesc, NULL, &cameraBuffer);

	shadowBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	shadowBufferDesc.ByteWidth = sizeof(ShadowBufferType);
	shadowBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shadowBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	shadowBufferDesc.MiscFlags = 0;
	shadowBufferDesc.StructureByteStride = 0;


	renderer->CreateBuffer(&shadowBufferDesc, NULL, &shadowBuffer);

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	renderer->CreateSamplerState(&samplerDesc, &sampleState);



	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	renderer->CreateSamplerState(&samplerDesc, &shadowSampleState);


	D3D11_BUFFER_DESC miscParamBufferDesc;
	miscParamBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	miscParamBufferDesc.ByteWidth = sizeof(MiscParamType);
	miscParamBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	miscParamBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	miscParamBufferDesc.MiscFlags = 0;
	miscParamBufferDesc.StructureByteStride = 0;

	renderer->CreateBuffer(&miscParamBufferDesc, NULL, &miscParamBuffer);


}


void ModelShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* specTexture, ShadowMap* depthMaps[4], LightSource* lights[4], Camera* camera, int viewMode)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	
	XMMATRIX tworld, tview, tproj;


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
	deviceContext->PSSetConstantBuffers(0, 1, &matrixBuffer);



	// Send Shadow Data
	ShadowBufferType* shadowPtr;
	result = deviceContext->Map(shadowBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	shadowPtr = (ShadowBufferType*)mappedResource.pData;

	// directions for each light to recalculate point light view matrix
	XMFLOAT3 directions[6];
	directions[0] = XMFLOAT3(1, 0, 0);
	directions[1] = XMFLOAT3(-1, 0, 0);
	directions[2] = XMFLOAT3(0, 1, 0);
	directions[3] = XMFLOAT3(0, -1, 0);
	directions[4] = XMFLOAT3(0, 0, 1);
	directions[5] = XMFLOAT3(0, 0, -1);
	for (int i = 0; i < 4; i++)
	{
		switch (lights[i]->getLightType())
		{
		case 0:
			shadowPtr->lightViewMatrix[i * 6] = XMMatrixTranspose(lights[i]->getViewMatrix());
			shadowPtr->lightProjectionMatrix[i * 6] = XMMatrixTranspose(lights[i]->getOrthoMatrix());
			break;
		case 1:
			for (int j = 0; j < 6; j++)
			{
				lights[i]->setDirection(directions[j].x, directions[j].y, directions[j].z);
				lights[i]->generateViewMatrix();
				shadowPtr->lightViewMatrix[(i * 6) + j] = XMMatrixTranspose(lights[i]->getViewMatrix());
				shadowPtr->lightProjectionMatrix[(i * 6) + j] = XMMatrixTranspose(lights[i]->getProjectionMatrix());
			}

			break;
		case 2:
			float fov = XMConvertToRadians(*lights[i]->getOuterCone() * 2.0f);
			shadowPtr->lightViewMatrix[i * 6] = XMMatrixTranspose(lights[i]->getViewMatrix());
			shadowPtr->lightProjectionMatrix[i * 6] = XMMatrixTranspose(XMMatrixPerspectiveFovLH(fov, 1, 5.0f, 100.f));
			break;
		}
	}
	deviceContext->Unmap(shadowBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &shadowBuffer);


	// Send Light Data
	LightBufferType* lightPtr;
	result = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	for (int i = 0; i < 4; i++)
	{
		lightPtr->lightPosition[i] = XMFLOAT4(lights[i]->getPosition().x, lights[i]->getPosition().y, lights[i]->getPosition().z, lights[i]->getLightType());
		lightPtr->lightDirection[i] = XMFLOAT4(lights[i]->getDirection().x, lights[i]->getDirection().y, lights[i]->getDirection().z, 0.0f);
		lightPtr->diffuseColour[i] = lights[i]->getDiffuseColour();
		lightPtr->specularColour[i] = lights[i]->getSpecularColour();
		lightPtr->specularPower[i] = XMFLOAT4(*lights[i]->getSpecularPower(), 0.0f, 0.0f, 0.0f);
		lightPtr->attenuation[i] = XMFLOAT4(
			*lights[i]->getConstantFactor(),
			*lights[i]->getLinearfactor(),
			*lights[i]->getQuadraticFactor(), 0.0f
		);
		lightPtr->spotlightConeAngles[i] = XMFLOAT4(*lights[i]->getInnerCone(), *lights[i]->getOuterCone(), 0.0f, 0.0f);
	}
	lightPtr->ambientColour = lights[0]->getAmbientColour();
	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(1, 1, &lightBuffer);

	
	CameraBufferType* cameraPtr;
	result = deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	cameraPtr = (CameraBufferType*)mappedResource.pData;
	cameraPtr->cameraPostion = XMFLOAT4(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z, 1.0f);
	cameraPtr->cameraDirection = XMFLOAT4(camera->getRotation().x, camera->getRotation().y, camera->getRotation().z, 1.0f);

	deviceContext->Unmap(cameraBuffer, 0);
	deviceContext->PSSetConstantBuffers(2, 1, &cameraBuffer);

	result = deviceContext->Map(miscParamBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	MiscParamType* miscParamPtr = (MiscParamType*)mappedResource.pData;
	miscParamPtr->viewMode = XMFLOAT4(viewMode, 0.0f, 0.0f, 0.0f);

	deviceContext->Unmap(miscParamBuffer, 0);
	deviceContext->PSSetConstantBuffers(3, 1, &miscParamBuffer);

	// Set shader texture and sampler resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &normalTexture);
	deviceContext->PSSetShaderResources(2, 1, &specTexture);

	// set depth maps
	ID3D11ShaderResourceView* depth[24];
	for (int i = 0; i < 24; i++)
	{
		depth[i] = depthMaps[i]->getDepthMapSRV();
	}
	deviceContext->PSSetShaderResources(3, 24, depth);


	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &shadowSampleState);

}





