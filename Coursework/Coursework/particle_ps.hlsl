struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
	return float4(input.tex.x, input.tex.y, 0.0f, 1.0f);
}