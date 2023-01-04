

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
    float4 spotlightConeAngles[4];
    float4 ambient;
    
};

cbuffer CameraBuffer : register(b1)
{
    float4 cameraPosition;
    float4 cameraDirection;
}

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
    return specularColour * specularIntensity;
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
    
    float4 finalColour = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 specular = float4(0.0f,0.0f,0.0f,1.0f);
    float4 lightColour[4];
    float4 textureColour;
    float distance, constantFactor,
    linearFactor, quadraticFactor, attenuationValue, falloff, outerCone, theta, innerCone,ambientAtten;
    for (int i = 0; i < 4; i++)
    {
        // light type is stored in the position w value
        switch (lightPosition[i].w)
        {
            case 0:
                lightColour[i] = calculateLighting(-lightDirection[i].xyz, input.normal, diffuseColour[i]);
                break;
            
            case 1:
                specular = calculateSpecular(
                    normalize(lightPosition[i].xyz - input.worldPosition),
                    input.normal,
                    normalize(cameraPosition.xyz - input.worldPosition),
                    specularColour[i],
                    specularPower[i].x
                );
            
            
                distance = length(lightPosition[i].xyz - input.worldPosition);
                constantFactor = attenuation[i].x;
                linearFactor = attenuation[i].y;
                quadraticFactor = attenuation[i].z;
                
                attenuationValue = 1 / (constantFactor + (linearFactor * distance) + (quadraticFactor * pow(distance, 2)));
                lightColour[i] = ambient * attenuationValue;
                lightColour[i] += calculateLighting(distance, input.normal, diffuseColour[i]) * attenuationValue;
                lightColour[i] += specular * attenuationValue;
                
                break;
            
            case 2:
                specular = calculateSpecular(
                    normalize(lightPosition[i].xyz - input.worldPosition),
                    input.normal,
                    normalize(cameraPosition.xyz - input.worldPosition),
                    specularColour[i],
                    specularPower[i].x
                );
                float3 lightDir = normalize(lightPosition[i].xyz - input.worldPosition);
                innerCone = cos(radians(spotlightConeAngles[i].x));
                outerCone = cos(radians(spotlightConeAngles[i].y));
                theta = dot(lightDir, normalize(-lightDirection[i].xyz));
                float epsilon = innerCone - outerCone;
                float intensity = clamp((theta - outerCone) / epsilon, 0.0f, 1.0f);

                distance = length(lightPosition[i].xyz - input.worldPosition);
                constantFactor = attenuation[i].x;
                linearFactor = attenuation[i].y;
                quadraticFactor = attenuation[i].z;
                
                attenuationValue = 1 / (constantFactor + (linearFactor * distance) + (quadraticFactor * pow(distance, 2)));
                lightColour[i] = ambient * attenuationValue;
                lightColour[i] += (calculateLighting(distance, input.normal, diffuseColour[i]) * intensity) * attenuationValue;
                lightColour[i] += (specular * attenuationValue) * intensity;

            
            
                break;
            
            default:
                return float4(1.0f, 0.0f, 1.0f, 1.0f); // easy to debug colour
                break;

        }
    }
    
    finalColour = float4(
    clamp(lightColour[0].r + lightColour[1].r + lightColour[2].r + lightColour[3].r, 0.0f, 1.0f),
    clamp(lightColour[0].g + lightColour[1].g + lightColour[2].g + lightColour[3].g, 0.0f, 1.0f),
    clamp(lightColour[0].b + lightColour[1].b + lightColour[2].b + lightColour[3].b, 0.0f, 1.0f),
    1.0f
    );
    textureColour = texture0.Sample(Sampler0, input.tex);
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    return  finalColour * textureColour;
}