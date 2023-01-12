// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

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

cbuffer ShadowBuffer : register(b2)
{
    matrix lightViewMatrix[24];
    matrix lightProjectionMatrix[24];
}

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;

};

struct OutputType
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAl;
    float4 lightViewPos[24] : TEXCOORD1;
};

static float PI = 3.14159265f;
static float GRAVITY = 9.8f;
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


[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch, InputType input2)
{
    float3 vertexPosition, vertexnormal;
    OutputType output;
 
    // correctly changes new vertex to its correct position
    float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.y);
    float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.y);
    vertexPosition = lerp(v1, v2, uvwCoord.x);
    
    // correctly changes new normal to its correct value
    float3 n1 = lerp(patch[0].normal, patch[1].normal, uvwCoord.y);
    float3 n2 = lerp(patch[3].normal, patch[2].normal, uvwCoord.y);
    vertexnormal = lerp(n1, n2, uvwCoord.x);
    
    // correctly changes new tex coord to its correct value
    float2 t1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.y);
    float2 t2 = lerp(patch[3].tex, patch[2].tex, uvwCoord.y);
    float2 texCoord = lerp(t1, t2, uvwCoord.x);
    
    
    // gersner waves
    float3 waterPoint = vertexPosition;
    float3 tangent = float3(1, 0, 0);
    float3 binormal = float3(0, 0, 1);
    float3 finalPoint = waterPoint;

    
    // add gersner waves to the vertex
    finalPoint += GerstnerWave(waves[0], waterPoint, tangent, binormal);
    finalPoint += GerstnerWave(waves[1], waterPoint, tangent, binormal);
    finalPoint += GerstnerWave(waves[2], waterPoint, tangent, binormal);
    finalPoint += GerstnerWave(waves[3], waterPoint, tangent, binormal);
    vertexPosition = finalPoint;
    
    output.worldPosition = finalPoint;
    
    // calculate normal of the vertex
    float3 normal = normalize(cross(binormal, tangent));
    vertexnormal = normal;
    
    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    
    
    // used for shadow calculations
    for (int i = 0; i < 24; i++)
    {
        output.lightViewPos[i] = mul(float4(vertexPosition, 1.0f), worldMatrix);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
        output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
    
    }

        
    output.normal = mul(vertexnormal, (float3x3) worldMatrix);

    output.tangent = mul(tangent, (float3x3) worldMatrix);
 
    output.binormal = mul(binormal, (float3x3) worldMatrix);


    output.tex = texCoord;
    
    return output;
}

