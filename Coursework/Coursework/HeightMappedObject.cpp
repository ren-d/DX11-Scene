#include "HeightMappedObject.h"
HeightMappedObject::HeightMappedObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightMap)
{
	m_mesh = nullptr;
	m_device = device;
	m_deviceContext = deviceContext;
	setTexture(texture);
	m_heightMap = heightMap;
	m_amplitude = 1.0f;
}

HeightMappedObject::~HeightMappedObject()
{

}

void HeightMappedObject::render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, HeightMapShader* shader, Light* light)
{

	buildTransformations(world);
	m_mesh->sendData(m_deviceContext);
	shader->setShaderParameters(m_deviceContext, world, view, projection, m_texture, m_heightMap, m_amplitude, light);
	shader->render(m_deviceContext, m_mesh->getIndexCount());
}

float* HeightMappedObject::getAmplitude()
{
	return &m_amplitude;
}