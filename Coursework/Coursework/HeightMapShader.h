#pragma once
#include "BaseShader.h"
#include "Light.h"
class HeightMapShader :
    public BaseShader
{
    
public:
    struct HeightMapBufferType
    {
        XMFLOAT4 manipulation;
    };

    struct LightBufferType
    {
        XMFLOAT4 diffuseColour;
        XMFLOAT3 lightDirection;
        float padding;
    };
    HeightMapShader(ID3D11Device* device, HWND hwnd);
    ~HeightMapShader();
    void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightTexture, float amplitude, Light* light);

private:
    void initShader(const wchar_t* vs, const wchar_t* ps);

private:
    ID3D11Buffer* matrixBuffer;
    ID3D11Buffer* heightMapBuffer;
    ID3D11Buffer* lightBuffer;
    ID3D11SamplerState* sampleState;
};

