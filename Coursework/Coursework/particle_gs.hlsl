// triangle_gs
// Geometry shader that generates a triangle for every vertex.

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
    OutputType output;
	
	// Move the vertex away from the point position
    output.position = input[0].position + float4(0.0, 1.0, 0.0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex;
    output.normal = mul(input[0].normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	// Move the vertex away from the point position
    output.position = input[0].position + float4(-1.0, 0.0, 0.0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex;
    output.normal = mul(input[0].normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	// Move the vertex away from the point position
    output.position = input[0].position + float4(1.0, 0.0, 1.0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex;
    output.normal = mul(input[0].normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);
    
    output.position = input[0].position + float4(0.0, 1.0, 1.0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex;
    output.normal = mul(input[0].normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	// Move the vertex away from the point position
    output.position = input[0].position + float4(-1.0, 1.0, 0.0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex;
    output.normal = mul(input[0].normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

	// Move the vertex away from the point position
    output.position = input[0].position + float4(1.0, 0.0, 0.0, 0.0);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.tex = input[0].tex;
    output.normal = mul(input[0].normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

    triStream.RestartStrip();
}