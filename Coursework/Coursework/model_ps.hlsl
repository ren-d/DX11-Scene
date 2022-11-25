Texture2D texture0 : register(t0);
Texture2D textureNormalMap : register(t1);
SamplerState Sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 diffuseColour;
    float3 lightDirection;
    float padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = float4(0.2, 0.2, 0.2, 1.0f) + saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float4 lightColour;
    float4 textureColour;
    float4 bumpMap;
    float3 bumpNormal;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
        // Sample the pixel in the bump map.
    bumpMap = textureNormalMap.Sample(Sampler0, input.tex);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    // Calculate the normal from the data in the bump map.
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	
    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);
    lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
    textureColour = texture0.Sample(Sampler0, input.tex);
    return textureColour * lightColour;
}