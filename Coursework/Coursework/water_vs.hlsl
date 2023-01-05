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
    float3 tangent : TANGENT;
    float3 binormal : BINORMAl;
    float4 lightViewPos : TEXCOORD1;
    
};


static float PI = 3.14159265f;
static float GRAVITY = 9.8f;

// gersner wave algorithm
float3 GerstnerWave(float4 wave, float3 position, inout float3 tangent, inout float3 binormal)
{
    float steepness = wave.z;
    float waveLength = wave.w;

    // Gersner Algorithm
    float k = 2 * PI / waveLength;
    float c = sqrt(GRAVITY / k); 
    float2 d = normalize(wave.xy);
    float f = k * (dot(d, position.xz) - c * timeInSeconds.x);
    float a = steepness / k;
 
    //calculate tangent of the vertex
    tangent += float3(
        -d.x * d.x * (steepness * sin(f)),
	    d.x * (steepness * cos(f)),
	    -d.x * d.y * (steepness * sin(f))
        );
    
    //calculate the binormal of the vertex
    binormal += float3(
        -d.x * d.x * (steepness * sin(f)),
        d.y * (steepness * cos(f)),
        -d.y * d.y * (steepness * sin(f))
        );
    
    // returns calculated position
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
    
    // add gersner waves to the vertex
    finalPoint += GerstnerWave(waves[0], waterPoint, tangent, binormal);
    finalPoint += GerstnerWave(waves[1], waterPoint, tangent, binormal);
    finalPoint += GerstnerWave(waves[2], waterPoint, tangent, binormal);
    finalPoint += GerstnerWave(waves[3], waterPoint, tangent, binormal);
    input.position.xyz = finalPoint;
    
    
    // calculate normal of the vertex
    float3 normal = normalize(cross(binormal, tangent));
    input.normal = normal;
    
    // send vertex data to pixel shader
    output.worldPosition = finalPoint;
    
    
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.lightViewPos = mul(input.position, worldMatrix);
        
    output.normal = mul(input.normal, (float3x3) worldMatrix);

    
    output.tangent = mul(tangent, (float3x3) worldMatrix);
 
    
    output.binormal = mul(binormal, (float3x3) worldMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex ;

    return output;
}