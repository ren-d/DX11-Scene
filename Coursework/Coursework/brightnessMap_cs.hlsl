Texture2D inputTexture : register(t0);
RWTexture2D<float4> outputTex : register(u0);

cbuffer ThresholdBuffer : register(b0)
{
    float4 threshold;
};


[numthreads(16,16, 1)]
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    uint2 inPixel = uint2(DispatchThreadID.x, DispatchThreadID.y);
    float3 luminance = float3(0.2126, 0.7152, 0.0722);
    
    float pixelBrightness = (dot(inputTexture[inPixel].rgb, luminance));
    
    bool isPastThreshold = pixelBrightness > threshold.x;
    
    switch (isPastThreshold)
    {
        case true:
            outputTex[inPixel] = inputTexture[inPixel];
            break;
        case false:
            outputTex[inPixel] = float4(0, 0, 0, 0);
            break;
    }

        
}