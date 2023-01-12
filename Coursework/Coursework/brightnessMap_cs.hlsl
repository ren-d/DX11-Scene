// Brightness map compute shader, based on learnopengl Bloom tutorial converted into a compute shader

Texture2D inputTexture : register(t0);
RWTexture2D<float4> outputTex : register(u0);

cbuffer ThresholdBuffer : register(b0)
{
    float4 threshold;
};


[numthreads(8,8, 1)] // will use 64 threads for each group

void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    uint2 inPixel = uint2(DispatchThreadID.x, DispatchThreadID.y); // current pixel
    
    // Luminance values corresponding to how brightness is percieved by the eye
    float3 luminance = float3(0.2126, 0.7152, 0.0722);
    
    // returns the dot product of the current pixels brightness and the luminance
    float pixelBrightness = (dot(inputTexture[inPixel].rgb, luminance));
    
    // checks if the pixel is past a threshold and decides whether to write it or not
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