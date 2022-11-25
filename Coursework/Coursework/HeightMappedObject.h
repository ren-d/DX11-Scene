#pragma once
#include "SceneObject.h"
#include "HeightMapShader.h"
class HeightMappedObject :  public SceneObject
{
public:
	HeightMappedObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightMap);
	~HeightMappedObject();

	void render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, HeightMapShader* shader, Light* light);
	float* getAmplitude();
private:
	ID3D11ShaderResourceView* m_heightMap;
	float m_amplitude;
};

