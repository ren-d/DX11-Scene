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

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;

};

struct OutputType
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD;
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
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    float3 vertexPosition, vertexnormal;
    OutputType output;
 
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
    float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.y);
    float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.y);
    vertexPosition = lerp(v1, v2, uvwCoord.x);
    

    
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

    
    // calculate normal of the vertex
    float3 normal = normalize(cross(binormal, tangent));
    vertexnormal = normal;
    
    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    
    output.depthPosition = output.position;
    
    return output;
}

