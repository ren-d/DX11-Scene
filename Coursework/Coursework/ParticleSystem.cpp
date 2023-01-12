#include "ParticleSystem.h"
ParticleSystem::ParticleSystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture)
{

	m_deviceContext = deviceContext;
	m_device = device;
}

ParticleSystem::~ParticleSystem()
{

}



void ParticleSystem::render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ParticleShader* shader)
{

	m_mesh->sendData(m_deviceContext);
	shader->setShaderParameters(m_deviceContext, world, view, projection, m_texture);
	shader->render(m_deviceContext, m_mesh->getIndexCount());
}

