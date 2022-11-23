#include "SceneObject.h"
SceneObject::SceneObject()
{
	m_mesh = nullptr;
}

SceneObject::~SceneObject()
{

}

void SceneObject::render(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, BaseShader* shader)
{

}
bool SceneObject::setMesh(BaseMesh* mesh)
{
	return m_mesh = mesh;
}

BaseMesh* SceneObject::getMesh()
{
	return m_mesh;
}