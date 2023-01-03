#pragma once
#include "SceneObject.h"
#include "Water.h"
#include "WaterShader.h"
#include "LightSource.h"

class Water :  public SceneObject
{
public:
	
	Water(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightMap);
	~Water();

	void render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, WaterShader* shader, LightSource* lights[4], float deltaTime);

	Wave* getWave(int id);
	void setWaveDir(int id, XMFLOAT2 direction);
private:
	ID3D11ShaderResourceView* m_heightMap;
	Wave* waves[4];
};

