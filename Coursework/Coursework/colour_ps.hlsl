// basic colour shader
cbuffer ColourBuffer : register(b0)
{
    float4 colour;
}


struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
    return colour;
}