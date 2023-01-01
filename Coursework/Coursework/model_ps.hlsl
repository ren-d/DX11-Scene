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
    float3 tangent : TANGENT;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = float4(0.2, 0.2, 0.2, 1.0f) + saturate(diffuse * intensity);
    return colour;
}

float magnitude(float3 _vector)
{
    return sqrt(pow(_vector.x, 2) + pow(_vector.y, 2) + pow(_vector.z, 2));

}

float4 main(InputType input) : SV_TARGET
{
    float4 lightColour;
    float4 textureColour;
    float3 bumpMap;
    float3 bumpNormal;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
        // Sample the pixel in the bump map.
    bumpMap = textureNormalMap.Sample(Sampler0, input.tex).rgb;

    float3 tangent1 = cross(input.normal, float3(0, 0, 1));
    float3 tangent2 = cross(input.normal, float3(0, 01, 0));
    float3 tangent;
    if (magnitude(tangent1) > magnitude(tangent2))
    {
        tangent = tangent1;
    }
    else
    {
        tangent = tangent2;
    }
    

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;
 
    // Calculate the normal from the data in the bump map.
    float3 N = input.normal;
    float3 T = normalize(tangent - dot(tangent, N) * N);
    float3 B = cross(N, T);
   
    float3x3 TBN = float3x3(T, B, N);
    
    float3 bumpedNormalMap = mul(bumpMap, TBN);
    // Normalize the resulting bump normal.
    bumpedNormalMap.z = -bumpedNormalMap.z;
    lightColour = calculateLighting(-lightDirection, bumpedNormalMap, diffuseColour);
    textureColour = texture0.Sample(Sampler0, input.tex);
    return textureColour * lightColour;
}

