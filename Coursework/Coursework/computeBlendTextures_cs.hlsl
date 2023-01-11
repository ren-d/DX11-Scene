Texture2D inputTexture1 : register(t0);
Texture2D inputTexture2 : register(t1);
RWTexture2D<float4> outputTex : register(u0);

cbuffer IntensityBuffer : register(b0)
{
    float4 blending;
};

[numthreads(8, 1, 1)]
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    uint2 inPixel = uint2(DispatchThreadID.x, DispatchThreadID.y);
    float4 hdrColour = inputTexture1[inPixel];
    float4 bloomColour = inputTexture2[inPixel];
    float4 baseColour = float4(1,1,1,1);
    
    float4 finalColour = hdrColour + (bloomColour * blending.x);
    
    finalColour = pow(finalColour, baseColour / blending.y);
    
    outputTex[inPixel] = finalColour;

}