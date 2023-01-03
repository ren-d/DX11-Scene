

Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 lightPosition[4];
    float4 lightDirection[4];
    float4 diffuseColour[4];
    float4 specularColour[4];
    float4 specularPower[4];
    float4 attenuation[4];
    float4 ambient;
};

// calculate direction lighting
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = ambient + saturate(diffuse * intensity);
    return colour;
}

float4 calculateSpecular(float3 lightDirection, float3 normal, float3 viewVector, float4 specularColour, float specularPower)
{
	//blinn-phong specular calculation
    float3 halfway = normalize(lightDirection + viewVector);
    float specularIntensity = pow(max(dot(normal, halfway), 0.0), specularPower);
    return saturate(specularColour * specularIntensity);
}

float4 calculateLightingDirection(float3 lightDirection, float3 normal, float4 ldiffuse)
{
    float intensity = saturate(dot(normal, -lightDirection));
    float4 colour = saturate(ldiffuse * intensity);
    return colour;
}

struct InputType
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};




float4 main(InputType input) : SV_TARGET
{
    
    float4 lightColour = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 textureColour;
    float distance, constantFactor,
    linearFactor, quadraticFactor, attenuationValue;
    for (int i = 0; i < 4; i++)
    {
        // light type is stored in the position w value
        switch (lightPosition[i].w)
        {
            case 0:
                lightColour += calculateLighting(-lightDirection[0].xyz, input.normal, diffuseColour[i]);
                break;
            
            case 1:
                float4 newColour;
                distance = length(lightPosition[i].xyz - input.worldPosition);
                constantFactor = attenuation[i].x;
                linearFactor = attenuation[i].y;
                quadraticFactor = attenuation[i].z;
                
                attenuationValue = 1 / (constantFactor + (linearFactor * distance) + (quadraticFactor * pow(distance, 2)));
                newColour[i] = ambient + calculateLighting(distance, input.normal, diffuseColour[i]);
                newColour[i] *= attenuationValue;
                lightColour += newColour[i];
                break;
            
            case 2:
                return float4(1.0f, 1.0f, 1.0f, 1.0f);
                break;
            
            default:
                return float4(1.0f, 0.0f, 1.0f, 1.0f);
                break;

        }
    }
       
    textureColour = texture0.Sample(Sampler0, input.tex);
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    return textureColour * lightColour;
}