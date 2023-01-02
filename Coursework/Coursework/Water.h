#pragma once
#include "SceneObject.h"
#include "Water.h"
#include "WaterShader.h"
#include "BasicShader.h"
#include "LightSource.h"
class Water :  public SceneObject
{
public:
	Water(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightMap);
	~Water();

	void render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, BasicShader* shader, LightSource* lights[4], float deltaTime);
	float* getSteepness();
	float* getWaveLength();
	float* getGravity();
private:
	ID3D11ShaderResourceView* m_heightMap;
	float m_steepness;
	float m_waveLength;
	float m_gravity;
};

