Texture2D inputTexture : register(t0);
RWTexture2D<float4> outputTex : register(u0);


[numthreads(8, 8, 1)]
void main( uint3 DispatchThreadID : SV_DispatchThreadID )
{
    uint2 inPixel = uint2(DispatchThreadID.x, DispatchThreadID.y ) * 2;
    
    
    float4 hIntensity0 = lerp(inputTexture[inPixel], inputTexture[inPixel + uint2(1, 0)], 0.5);
    float4 hIntensity1 = lerp(inputTexture[inPixel + uint2(0, 1)], inputTexture[inPixel + uint2(1, 1)], 0.5);
    float4 intensity = lerp(hIntensity0, hIntensity1, 0.5);
    
    outputTex[uint2(DispatchThreadID.x, DispatchThreadID.y)] = float4(intensity.rgb, 1.0);



}