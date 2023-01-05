#include "ModelObject.h"
ModelObject::ModelObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap, ID3D11ShaderResourceView* m_specMap)
{
	m_mesh = nullptr;
	m_model = nullptr;
	m_device = device;
	m_deviceContext = deviceContext;
	setTexture(texture);
	m_normalMap = normalMap;

}

ModelObject::~ModelObject()
{

}

void ModelObject::setModel(AModel* model)
{
	m_model = model;
}

void ModelObject::render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ModelShader* shader, LightSource* lights[4], Camera* camera)
{

	
	m_model->sendData(m_deviceContext);
	shader->setShaderParameters(m_deviceContext, world, view, projection, m_texture, m_normalMap, m_specMap, lights, camera);
	shader->render(m_deviceContext, m_model->getIndexCount());
}
