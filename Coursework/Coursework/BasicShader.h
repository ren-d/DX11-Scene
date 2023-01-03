#pragma once
#include "DXF.h"
#include "LightSource.h"

using namespace std;
using namespace DirectX;

struct Wave {
    XMFLOAT2 direction;
    float steepness,
        waveLength;
};

class BasicShader :
    public BaseShader
{
    
public:
    // Buffer Type structs
    struct WaterBufferType
    {
        XMFLOAT4 waves[4];
        XMFLOAT4 timeInSeconds;
    };
    struct LightBufferType
    {
        XMFLOAT4 lightPosition[4];
        XMFLOAT4 lightDirection[4];
        XMFLOAT4 diffuseColour[4];
        XMFLOAT4 ambientColour;
    };

    BasicShader(ID3D11Device* device, HWND hwnd);
    ~BasicShader();
    void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture,float time, LightSource lights[4], Wave* waves[4]);

private:
    void initShader(const wchar_t* vs, const wchar_t* ps);

private:
    ID3D11Buffer* matrixBuffer;
    ID3D11Buffer* waterBuffer;
    ID3D11Buffer* lightBuffer;
    ID3D11SamplerState* sampleState;
};


