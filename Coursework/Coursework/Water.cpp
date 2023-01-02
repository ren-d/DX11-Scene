#include "Water.h"
Water::Water(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightMap)
{
	m_mesh = nullptr;
	m_device = device;
	m_deviceContext = deviceContext;
	setTexture(texture);
	m_heightMap = heightMap;
	m_steepness = 0.0f;
	m_waveLength = 10.0f;
	m_gravity = 9.8f;
}

Water::~Water()
{

}

void Water::render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, BasicShader* shader, LightSource* lights[4], float deltaTime)
{

	buildTransformations(world);
	m_mesh->sendData(m_deviceContext);
	shader->setShaderParameters(m_deviceContext, world, view, projection, m_texture, m_steepness, m_waveLength, m_gravity, deltaTime, *lights);
	shader->render(m_deviceContext, m_mesh->getIndexCount());
}

float* Water::getSteepness()
{
	return &m_steepness;
}

float* Water::getWaveLength()
{
	return &m_waveLength;
}

float* Water::getGravity()
{
	return &m_gravity;
}