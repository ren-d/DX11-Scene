cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer WaveBuffer : register(b1)
{
    float4 isWater;
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
    float4 depthPosition : TEXCOORD0;
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
    
    /* ensures depth map is accurate when the object is water
            1 = water
            0 = not water
    */
    if(isWater.x == 1)
    {
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
        
        output.position = mul(input.position, worldMatrix);
        output.position = mul(output.position, viewMatrix);
        output.position = mul(output.position, projectionMatrix);

        // Store the position value in a second input value for depth value calculations.
        output.depthPosition = output.position;
    }
    else
    {
          
        output.position = mul(input.position, worldMatrix);
        output.position = mul(output.position, viewMatrix);
        output.position = mul(output.position, projectionMatrix);


        output.depthPosition = output.position;
    }

	
    return output;
}