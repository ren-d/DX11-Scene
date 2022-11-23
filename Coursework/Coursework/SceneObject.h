#pragma once
#include "BasicShader.h"
#include "DXF.h"

class SceneObject 
{
public:
    SceneObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
    ~SceneObject();

    void render(XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, BasicShader* shader);
    bool setMesh(BaseMesh* mesh);
    bool setTexture(ID3D11ShaderResourceView* texture);
    void translate(XMFLOAT3 translation);
    void rotateX(float rotx);
    void rotateY(float roty);
    void rotateZ(float rotz);
    void scale(XMFLOAT3 scale);
    void buildTransformations(XMMATRIX& world);
    BaseMesh* getMesh();
private:
    BaseMesh* m_mesh;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    ID3D11ShaderResourceView* m_texture;
    float m_rotX, m_rotY, m_rotZ;
    XMFLOAT3 m_translation, m_scale;
    XMMATRIX m_transformMatrix;
};

