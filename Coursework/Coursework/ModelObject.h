// Model Object class
// used to make rendering models easier

#pragma once
#include "SceneObject.h"
#include "ModelShader.h"
class ModelObject :
    public SceneObject
{
public:
	ModelObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap, ID3D11ShaderResourceView* specMap);
	~ModelObject();

	void render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ModelShader* shader, LightSource* lights[4], Camera* camera, ShadowMap* shadowMaps[4], int viewMode);

private:
	ID3D11ShaderResourceView* m_normalMap;
	ID3D11ShaderResourceView* m_specMap;

};

