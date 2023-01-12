#pragma once
#include "SceneObject.h"
#include "ParticleShader.h"
class ParticleSystem :
    public SceneObject
{
public:
	ParticleSystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
	~ParticleSystem();

	void render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ParticleShader* shader);


private:


};

