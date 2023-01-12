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
    float2 tex : TEXCOORD0;
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
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;

};




// Patch constant function
ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputType output;


    // Calculate the tessellation factors based on some desired tessellation density
    output.edges[0] = tessellation_value;
    output.edges[1] = tessellation_value;
    output.edges[2] = tessellation_value;
    output.edges[3] = tessellation_value;
    output.inside[0] = tessellation_value;
    output.inside[1] = tessellation_value;

    return output;
}

// Hull shader
[domain("quad")]
[partitioning("fractional_odd")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

    // Interpolate the position and normal of the control point based on the tessellation factors
    output.position = patch[pointId].position;
    output.normal = patch[pointId].normal;
    output.tex = patch[pointId].tex;

    return output;
}