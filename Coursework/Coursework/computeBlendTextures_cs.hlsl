// Blending compute shader, based on learnopengl Bloom tutorial converted into a compute shader


Texture2D inputTexture1 : register(t0);
Texture2D inputTexture2 : register(t1);

RWTexture2D<float4> outputTex : register(u0);

cbuffer IntensityBuffer : register(b0)
{
    float4 blending; // x = bloom intensity
                     // y = gamma
};

[numthreads(8, 8, 1)] // will use 64 threads for each group
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    
    uint2 inPixel = uint2(DispatchThreadID.x, DispatchThreadID.y); // current pixel
    
    // return texture values for the current pixel
    float4 hdrColour = inputTexture1[inPixel];
    float4 bloomColour = inputTexture2[inPixel];
    
    // used for gamma calculation
    float4 baseColour = float4(1,1,1,1);
    
    // additive blending
    float4 finalColour = hdrColour + (bloomColour * blending.x);
    
    // change gamma based on GUI input for basic tone mapping
    finalColour = pow(finalColour, baseColour / blending.y);
    
    outputTex[inPixel] = finalColour;

}