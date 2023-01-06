#pragma once
#include "SceneObject.h"
#include "Water.h"
#include "WaterShader.h"
#include "LightSource.h"
#include "WaterDepthShader.h"
class Water :  public SceneObject
{
public:
	
	Water(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap1, ID3D11ShaderResourceView* normalMap2);
	~Water();

	void initWaves();
	void render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, WaterShader* shader, LightSource* lights[4], ShadowMap* maps[2], float timeInSeconds, Camera* camera);
	void renderDepth(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, WaterDepthShader* shader, float timeInSeconds, Camera* camera);

	Wave* getWave(int id);
	void setWaveDir(int id, XMFLOAT2 direction);

private:
	
	ID3D11ShaderResourceView* m_normalMaps[2];
	Wave* m_waves[4];
};

