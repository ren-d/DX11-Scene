// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
Texture2D heightMapTexture : register(t1);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 diffuseColour;
    float3 lightDirection;
    float padding;
};
cbuffer HeightBuffer : register(b1)
{
    float4 amplitude;
};
struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float getHeight(float2 uv)
{
    float4 height = heightMapTexture.SampleLevel(sampler0, uv, 0);
    
    return height.x * amplitude.x;;
}

//USE DIFFERENT NORMAL CALCULATION
float3 calculateNormals(float2 uv)
{
    float h = getHeight(uv);
    float tw = 0;
    float val;
    
    heightMapTexture.GetDimensions(0, tw, tw, val);
    float uvOff = 1.0f / tw;
    float worldstep = 100 * uvOff;
    float heightN = getHeight(float2(uv.x, uv.y + uvOff));
    float heightS = getHeight(float2(uv.x, uv.y - uvOff));
    float heightE = getHeight(float2(uv.x + uvOff, uv.y));
    float heightW = getHeight(float2(uv.x - uvOff, uv.y));

    float3 biUp = float3(0, heightN - h, -1 * worldstep);
    float3 biDown = float3(0, heightS - h, 1 * worldstep);
    float3 tanLeft = float3(1 * worldstep, heightE - h, 0);
    float3 tanRight = float3(-1 * worldstep, heightW - h, 0);
    
    
    float3 crossProduct = (cross(biUp, tanRight) + cross(tanRight, biDown) + cross(biDown, tanLeft) + cross(tanLeft, biUp)) / 4;
    return normalize(crossProduct);

}
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = float4(0.2, 0.2, 0.2, 1.0f) + saturate(diffuse * intensity);
    return colour;
}


float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 lightColour;
    float3 normal = calculateNormals(input.tex);
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
   
    lightColour = calculateLighting(-lightDirection, normal, diffuseColour);
    textureColour = texture0.Sample(sampler0, input.tex);
	
    return textureColour * lightColour;
}


