Texture2D inputTexture : register(t0);
RWTexture2D<float4> outputTex : register(u0);


[numthreads(256, 1, 1)]
void main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
    uint2 inPixel = uint2(DispatchThreadID.x, DispatchThreadID.y);
    
    float average = 0.2126 * inputTexture[inPixel].r + 0.7152 * inputTexture[inPixel].g + 0.0722 * inputTexture[inPixel].b;
    

    if (average > 0.8f)
    {
        outputTex[inPixel] = inputTexture[inPixel];
    }
    else
    {
        outputTex[inPixel] = float4(0, 0, 0, 0);

    }
    
        
}