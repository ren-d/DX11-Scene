#include "Water.h"
Water::Water(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightMap)
{
	m_mesh = nullptr;
	m_device = device;
	m_deviceContext = deviceContext;
	setTexture(texture);
	m_heightMap = heightMap;
	waves[0] = new Wave();
	waves[0]->direction = XMFLOAT2(1, 0);
	waves[0]->steepness = 0.264f;
	waves[0]->waveLength = 64.0f;
	waves[1] = new Wave();
	waves[1]->direction = XMFLOAT2(0, 1);
	waves[1]->steepness = 0.236f;
	waves[1]->waveLength = 16.0f;
	waves[2] = new Wave();
	waves[2]->direction = XMFLOAT2(-0.947, 0.912);
	waves[2]->steepness = 0.402f;
	waves[2]->waveLength = 64.0f;
	waves[3] = new Wave();
	waves[3]->direction = XMFLOAT2(0, -0.9);
	waves[3]->steepness = 0.102f;
	waves[3]->waveLength = 16.0f;
}

Water::~Water()
{

}

void Water::render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, WaterShader* shader, LightSource* lights[4], float deltaTime, Camera* camera)
{
	buildTransformations(world);
	m_mesh->sendData(m_deviceContext);
	shader->setShaderParameters(m_deviceContext, world, view, projection, m_texture, deltaTime, lights, waves, camera);
	shader->render(m_deviceContext, m_mesh->getIndexCount());
}

Wave* Water::getWave(int id)
{
	return waves[id];
}

void Water::setWaveDir(int id, XMFLOAT2 direction)
{
	waves[id]->direction = direction;
}

