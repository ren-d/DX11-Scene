// Water Shader
// used for tessellating, animation, vertex transformation, lighting the water object inside the scene

#pragma once
#include "DXF.h"
#include "LightSource.h"

using namespace std;
using namespace DirectX;

#include "structsforuse.h"

class WaterShader :
    public BaseShader
{
    
public:

    // buffer definitions (for size calculation)
    struct MiscParamType
    {
        XMFLOAT4 viewMode;
    };

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

    struct TessellationBufferType
    {
        XMFLOAT4 tessellation;
        XMFLOAT4 camera;
        XMFLOAT4 cameraDir;
    };

    WaterShader(ID3D11Device* device, HWND hwnd);
    ~WaterShader();

    void setShaderParameters(
        ID3D11DeviceContext* deviceContext, 
        const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, 
        ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap[2], 
        ShadowMap* depthMaps[24], 
        float timeInSeconds, 
        LightSource* lights[4], 
        Wave* waves[4], 
        Camera* camera,
        float tessellation, 
        int viewMode
    );

private:
    void initShader(const wchar_t* vs, const wchar_t* ps);
    void initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename);

private:
    ID3D11Buffer* matrixBuffer;
    ID3D11Buffer* waterBuffer;
    ID3D11Buffer* lightBuffer;
    ID3D11Buffer* cameraBuffer;
    ID3D11Buffer* shadowBuffer;
    ID3D11Buffer* tessellationBuffer;
    ID3D11Buffer* miscParamBuffer;

    ID3D11SamplerState* sampleState;
    ID3D11SamplerState* shadowSampleState;




};


