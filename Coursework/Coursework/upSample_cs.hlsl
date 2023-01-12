// up sampling compute shader

Texture2D inputTexture : register(t0);
RWTexture2D<float4> outputTex : register(u0);

[numthreads(8, 8, 1)]
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    uint2 inPixel = uint2(DispatchThreadID.x, DispatchThreadID.y);
    uint2 inPixel2 = uint2(DispatchThreadID.x, DispatchThreadID.y) * 2;
    
    // sample four pixels from the input texture and blend them together
    float4 hIntensity0 = lerp(inputTexture[inPixel], inputTexture[inPixel + uint2(1, 0)], 0.5);
    float4 hIntensity1 = lerp(inputTexture[inPixel + uint2(0, 1)], inputTexture[inPixel + uint2(1, 1)], 0.5);
    float4 intensity = lerp(hIntensity0, hIntensity1, 0.5);

    // write the blended pixel to the output texture
    outputTex[inPixel2] = float4(1 * intensity.rgb, 1.0);

    // write the same pixel to the four neighboring pixels in the output texture
    outputTex[inPixel2 + uint2(1, 0)] = float4(intensity.rgb, 1.0);
    outputTex[inPixel2 + uint2(0, 1)] = float4(intensity.rgb, 1.0);
    outputTex[inPixel2 + uint2(1, 1)] = float4(intensity.rgb, 1.0);

}