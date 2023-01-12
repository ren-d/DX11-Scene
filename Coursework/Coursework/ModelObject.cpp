#include "ModelObject.h"
ModelObject::ModelObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap, ID3D11ShaderResourceView* specMap)
{
	m_mesh = nullptr;
	m_model = nullptr;
	m_device = device;
	m_deviceContext = deviceContext;
	setTexture(texture);
	m_normalMap = normalMap;
	m_specMap = specMap;

}

ModelObject::~ModelObject()
{
	if (m_normalMap)
	{
		m_normalMap->Release();
		m_normalMap = 0;
	}

	if (m_specMap)
	{
		m_specMap->Release();
		m_specMap = 0;
	}

	SceneObject::~SceneObject();
}



void ModelObject::render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ModelShader* shader, LightSource* lights[4], Camera* camera, ShadowMap* shadowMaps[4], int viewMode)
{
	// easy wrapper for rendering objects
	m_model->sendData(m_deviceContext);
	shader->setShaderParameters(m_deviceContext, world, view, projection, m_texture, m_normalMap, m_specMap, shadowMaps, lights, camera, viewMode);
	shader->render(m_deviceContext, m_model->getIndexCount());
}
