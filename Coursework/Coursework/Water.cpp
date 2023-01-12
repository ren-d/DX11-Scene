#include "Water.h"
Water::Water(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap1, ID3D11ShaderResourceView* normalMap2)
{
	m_mesh = nullptr;
	m_device = device;
	m_deviceContext = deviceContext;
	setTexture(texture);
	m_normalMaps[0] = normalMap1;
	m_normalMaps[1] = normalMap2;

	initWaves();

}

void Water::initWaves()
{
	// waves created and initialised
	m_waves[0] = new Wave();
	m_waves[0]->direction = XMFLOAT2(1, 0);
	m_waves[0]->steepness = 0.264f;
	m_waves[0]->waveLength = 64.0f;
	m_waves[1] = new Wave();
	m_waves[1]->direction = XMFLOAT2(0, 1);
	m_waves[1]->steepness = 0.236f;
	m_waves[1]->waveLength = 16.0f;
	m_waves[2] = new Wave();
	m_waves[2]->direction = XMFLOAT2(-0.947, 0.912);
	m_waves[2]->steepness = 0.402f;
	m_waves[2]->waveLength = 64.0f;
	m_waves[3] = new Wave();
	m_waves[3]->direction = XMFLOAT2(0, -0.9);
	m_waves[3]->steepness = 0.102f;
	m_waves[3]->waveLength = 16.0f;
}

Water::~Water()
{
	if (m_normalMaps)
	{
		m_normalMaps[0]->Release();
		m_normalMaps[0] = 0;

		m_normalMaps[1]->Release();
		m_normalMaps[1] = 0;
	}

	delete[] m_waves;


	SceneObject::~SceneObject();
}

// easy wrapper for rendering
void Water::render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, WaterShader* shader, LightSource* lights[4], ShadowMap* maps[24], float timeInSeconds, Camera* camera, float tessellation, int viewMode)
{

	m_mesh->sendData(m_deviceContext);
	shader->setShaderParameters(m_deviceContext, world, view, projection, m_texture, m_normalMaps, maps, timeInSeconds, lights, m_waves, camera, tessellation, viewMode);
	shader->render(m_deviceContext, m_mesh->getIndexCount());
}

// calculates depth map with waves applied
void Water::renderDepth(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, WaterDepthShader* shader, float timeInSeconds, Camera* camera)
{
	m_mesh->sendData(m_deviceContext);
	shader->setShaderParameters(m_deviceContext, world, view, projection, m_waves, timeInSeconds, camera);
	shader->render(m_deviceContext, m_mesh->getIndexCount());

}
Wave* Water::getWave(int id)
{
	return m_waves[id];
}

void Water::setWaveDir(int id, XMFLOAT2 direction)
{
	m_waves[id]->direction = direction;
}

