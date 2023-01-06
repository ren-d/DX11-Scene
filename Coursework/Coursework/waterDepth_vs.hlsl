struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};


OutputType main(InputType input)
{
    OutputType output;

    output.position = input.position;

    return output;
}