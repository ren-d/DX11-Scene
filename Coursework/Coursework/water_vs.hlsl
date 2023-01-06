// texture vertex shader
// Basic shader for rendering textured geometry


struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


OutputType main(InputType input)
{
    OutputType output;

    output.position = input.position;
    output.tex = input.tex;
    output.normal = input.normal;


    return output;
}