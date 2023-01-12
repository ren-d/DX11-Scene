#include "SceneObject.h"
SceneObject::SceneObject()
{
	m_mesh = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	setTexture(nullptr);
	m_transformMatrix = XMMatrixIdentity();
	m_translation = XMFLOAT3(0, 0, 0);
	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;
	m_scale = XMFLOAT3(1, 1, 1);

}

SceneObject::SceneObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture)
{
	m_mesh = nullptr;
	m_device = device;
	m_deviceContext = deviceContext;
	setTexture(texture);
	m_transformMatrix = XMMatrixIdentity();
	m_translation = XMFLOAT3(0, 0, 0);
	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;
	m_scale = XMFLOAT3(1, 1, 1);

}

SceneObject::~SceneObject()
{
	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	delete m_mesh;
	delete m_model;


}

void SceneObject::renderDepth(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, DepthShader* shader)
{
	// checks if object is a mesh or model
	// easy wrapper for rendering
	if (m_mesh != nullptr)
	{
		m_mesh->sendData(m_deviceContext);
		shader->setShaderParameters(m_deviceContext, world, view, projection);
		shader->render(m_deviceContext, m_mesh->getIndexCount());
	}
	else
	{
		m_model->sendData(m_deviceContext);
		shader->setShaderParameters(m_deviceContext, world, view, projection);
		shader->render(m_deviceContext, m_model->getIndexCount());
	}

}

bool SceneObject::setMesh(BaseMesh* mesh)
{
	return m_mesh = mesh;
}

bool SceneObject::setTexture(ID3D11ShaderResourceView* texture)
{
	return m_texture = texture;
}

BaseMesh* SceneObject::getMesh()
{
	return m_mesh;
}

void SceneObject::translate(XMFLOAT3 translation)
{
	m_translation = translation;
}

void SceneObject::rotateX(float rotx)
{
	m_rotX = rotx;
}
void SceneObject::rotateY(float roty)
{
	m_rotY = roty;
}
void SceneObject::rotateZ(float rotz)
{
	m_rotZ = rotz;
}

void SceneObject::scale(XMFLOAT3 scale)
{
	m_scale = scale;
}
void SceneObject::setModel(AModel* model)
{
	m_model = model;
}

void SceneObject::buildTransformations(XMMATRIX& world) // Ignore, is not used in this project.
{
	XMMATRIX translation = XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
	XMMATRIX rotationX = XMMatrixRotationX(m_rotX);
	XMMATRIX rotationY = XMMatrixRotationY(m_rotY);
	XMMATRIX rotationZ = XMMatrixRotationZ(m_rotZ);
	XMMATRIX rotation = XMMatrixIdentity();
	XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	
	rotation = XMMatrixMultiply(rotation, rotationX);
	rotation = XMMatrixMultiply(rotation, rotationY);
	rotation = XMMatrixMultiply(rotation, rotationZ);

	m_transformMatrix = XMMatrixIdentity();
	m_transformMatrix = XMMatrixMultiply(m_transformMatrix, scale);
	m_transformMatrix = XMMatrixMultiply(m_transformMatrix, rotation);
	m_transformMatrix = XMMatrixMultiply(m_transformMatrix, translation);

	world = XMMatrixMultiply(world, m_transformMatrix);

}

