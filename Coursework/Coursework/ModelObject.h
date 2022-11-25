#pragma once
#include "SceneObject.h"
#include "ModelShader.h"
class ModelObject :
    public SceneObject
{
public:
	ModelObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap);
	~ModelObject();

	void render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ModelShader* shader, Light* light);
	void setModel(AModel* model);
private:
	ID3D11ShaderResourceView* m_normalMap;
	AModel* m_model;
};

