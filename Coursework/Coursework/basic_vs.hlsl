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
    float steepness;
    float waveLength;
    float gravity;
    float timeInSeconds;
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
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

static float PI = 3.14159265f;
// gersner wave algorithm
float3 vertexManipulation(float4 position)
{
    float3 p = position.xyz;
    float k = 2 * PI / waveLength;
    float c = sqrt(gravity / k); //gravity
    float f = k * (p.x - c * timeInSeconds);
    float a = steepness / k;
    p.x += a * cos(f);
    p.y =  a * sin(f);
    
    
    return p;
}

float3 recalculateNormals(float4 position)
{
    float3 p = position.xyz;
    float k = 2 * PI / waveLength;
    float c = sqrt(gravity / k); //gravity
    float f = k * (p.x - c * timeInSeconds);

    float3 tangent = normalize(float3(1 - k * steepness * sin(f), k * steepness * cos(f), 0));
    float3 normal = float3(-tangent.y, tangent.x, 0.0f);
    
    return normal;
    
}

OutputType main(InputType input)
{
    OutputType output;

    input.normal = recalculateNormals(input.position);
    input.position.xyz = vertexManipulation(input.position);
    
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