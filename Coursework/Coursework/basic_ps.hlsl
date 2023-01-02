Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 lightPosition[4];
    float4 lightDirection[4];
    float4 diffuseColour[4];
    float4 ambient;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = ambient + saturate(diffuse * intensity);
    return colour;
}

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
    float4 lightColour = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 textureColour;
    
    lightColour = calculateLighting(-lightDirection[0].xyz, input.normal, lightColour[0]);
    textureColour = texture0.Sample(Sampler0, input.tex);
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    return textureColour * lightColour;
}