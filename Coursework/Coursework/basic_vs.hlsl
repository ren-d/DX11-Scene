// texture vertex shader
// Basic shader for rendering textured geometry

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer WaveBuffer : register(b1)
{
    float4 waves[4];
    float4 timeInSeconds;

}

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

static float4 dir = float4(1.0f, 1.0f, 0.0f,0.0f);
static float PI = 3.14159265f;
static float GRAVITY = 9.8f;
// gersner wave algorithm
float3 GerstnerWave(float4 wave, float3 position, inout float3 tangent, inout float3 binormal)
{
    float steepness = wave.z;
    float waveLength = wave.w;

    float k = 2 * PI / waveLength;
    float c = sqrt(GRAVITY / k); //gravity
    float2 d = normalize(wave.xy);
    float f = k * (dot(d, position.xz) - c * timeInSeconds.x);
    float a = steepness / k;
 
    tangent += float3(
        -d.x * d.x * (steepness * sin(f)),
	    d.x * (steepness * cos(f)),
	    -d.x * d.y * (steepness * sin(f))
        );
    
    binormal += float3(
        -d.x * d.x * (steepness * sin(f)),
        d.y * (steepness * cos(f)),
        -d.y * d.y * (steepness * sin(f))
        );
    
    return float3(
    d.x * (a * cos(f)),
    a * sin(f),
    d.y * (a * cos(f))
    );
}



OutputType main(InputType input)
{
    OutputType output;

    float3 waterPoint = input.position;
    float3 tangent = float3(1, 0, 0);
    float3 binormal = float3(0, 0, 1);
    float3 finalPoint = waterPoint;
    
    finalPoint += GerstnerWave(waves[0], waterPoint, tangent, binormal);
    finalPoint += GerstnerWave(waves[1], waterPoint, tangent, binormal);
    finalPoint += GerstnerWave(waves[2], waterPoint, tangent, binormal);
    finalPoint += GerstnerWave(waves[3], waterPoint, tangent, binormal);
    float3 normal = normalize(cross(binormal, tangent));
    input.position.xyz = finalPoint;
    input.normal = normal;
    output.worldPosition = finalPoint;
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    return output;
}