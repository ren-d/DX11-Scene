#pragma once
#include "BaseShader.h"
#include "Light.h"
class ModelShader :
    public BaseShader
{

public:

    struct LightBufferType
    {
        XMFLOAT4 diffuseColour;
        XMFLOAT3 lightDirection;
        float padding;
    };
    ModelShader(ID3D11Device* device, HWND hwnd);
    ~ModelShader();
    void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalTexture, Light* light);

private:
    void initShader(const wchar_t* vs, const wchar_t* ps);

private:
    ID3D11Buffer* matrixBuffer;
    ID3D11Buffer* lightBuffer;
    ID3D11SamplerState* sampleState;
};

