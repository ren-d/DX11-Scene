// Model Shader
// used for rendering imported models with, normal maps, specular maps and lighting

#pragma once
#include "BaseShader.h"
#include "LightSource.h"
class ModelShader :
    public BaseShader
{

public:
    // buffer definitions (for size calculation)
    struct MiscParamType
    {
        XMFLOAT4 viewMode;
    };


    struct LightBufferType
    {
        XMFLOAT4 lightPosition[4];
        XMFLOAT4 lightDirection[4];
        XMFLOAT4 diffuseColour[4];
        XMFLOAT4 specularColour[4];
        XMFLOAT4 specularPower[4];
        XMFLOAT4 attenuation[4];
        XMFLOAT4 spotlightConeAngles[4];
        XMFLOAT4 ambientColour;

    };

    struct CameraBufferType
    {
        XMFLOAT4 cameraPostion;
        XMFLOAT4 cameraDirection;
    };

    struct ShadowBufferType
    {
        XMMATRIX lightViewMatrix[24];
        XMMATRIX lightProjectionMatrix[24];
    };

    

    ModelShader(ID3D11Device* device, HWND hwnd);
    ~ModelShader();

    void setShaderParameters(
        ID3D11DeviceContext* deviceContext, 
        const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, 
        ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* specTexture, 
        ShadowMap* depthMaps[4],
        LightSource* lights[4], 
        Camera* camera,
        int viewMode
    );

private:
    void initShader(const wchar_t* vs, const wchar_t* ps);

private:
    ID3D11Buffer* matrixBuffer;
    ID3D11Buffer* lightBuffer;
    ID3D11Buffer* cameraBuffer;
    ID3D11Buffer* shadowBuffer;
    ID3D11Buffer* miscParamBuffer;

    ID3D11SamplerState* sampleState;
    ID3D11SamplerState* shadowSampleState;
};

