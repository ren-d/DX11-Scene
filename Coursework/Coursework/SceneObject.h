#pragma once
#include "DXF.h"

class SceneObject 
{
public:
    SceneObject();
    ~SceneObject();

    void render(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, BaseShader* shader);
    bool setMesh(BaseMesh* mesh);
    BaseMesh* getMesh();
private:
    BaseMesh* m_mesh;
};

