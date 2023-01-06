// Tessellation Hull Shader
// Prepares control points for tessellation

cbuffer TessellationBufffer : register(b0)
{
    float4 tessellation_value;
    float4 camera_pos;
    float4 camera_dir;
};
struct InputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
    float3 normal : NORMAL;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
    float3 normal : NORMAL;
};



ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputType output;

    float dist = sqrt(pow(inputPatch[0].position.x - camera_pos.x, 2) + pow(inputPatch[0].position.y - camera_pos.y, 2) + pow(inputPatch[0].position.z - camera_pos.z, 2));
    float distcalc = abs((dist - 100) / dist) * 0.01; // distance calculator (inverse)
    float factor = lerp(0, 64, distcalc);
    
    // Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = factor;
    output.edges[1] = factor;
    output.edges[2] = factor;
    output.edges[3] = factor;
    // Set the tessellation factor for tessallating inside the triangle.
    output.inside[0] = factor;
    output.inside[1] = factor;
    return output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]

OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;


    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    // Set the input colour as the output colour.
    output.colour = patch[pointId].colour;
    
    output.normal = patch[pointId].normal;
    


    return output;
}