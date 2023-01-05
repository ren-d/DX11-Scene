

Texture2D texture0 : register(t0);
Texture2D normalMap : register(t1);
Texture2D normalMap2 : register(t2);
Texture2D depthMap : register(t3);
SamplerState Sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer LightBuffer : register(b1)
{
    float4 lightPosition[4];
    float4 lightDirection[4];
    float4 diffuseColour[4];
    float4 specularColour[4];
    float4 specularPower[4];
    float4 attenuation[4];
    float4 spotlightConeAngles[4];
    float4 ambient;
    
    matrix lightViewMatrix[2];
    matrix lightProjectionMatrix[2];
    
};

cbuffer CameraBuffer : register(b2)
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


struct InputType
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAl;
    float4 lightViewPos : TEXCOORD1;
};



float4 calculateFinalLighting(int numberOfLights, float3 normal, float3 worldPosition)
{
    float4 lightColour[4];
    float distance, constantFactor,
    linearFactor, quadraticFactor, attenuationValue, falloff, outerCone, theta, innerCone, ambientAtten;
    float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    for (int i = 0; i < numberOfLights; i++)
    {
        switch (lightPosition[i].w)
        {
            case 0:
                lightColour[i] = calculateLighting(-lightDirection[i].xyz, normal, diffuseColour[i]);
                break;
            
            case 1:
                specular = calculateSpecular(
                    normalize(lightPosition[i].xyz - worldPosition),
                    normal,
                    normalize(cameraPosition.xyz - worldPosition),
                    specularColour[i],
                    specularPower[i].x
                );
            
            
                distance = length(lightPosition[i].xyz - worldPosition);
                constantFactor = attenuation[i].x;
                linearFactor = attenuation[i].y;
                quadraticFactor = attenuation[i].z;
                
                attenuationValue = 1 / (constantFactor + (linearFactor * distance) + (quadraticFactor * pow(distance, 2)));
                lightColour[i] = ambient * attenuationValue;
                lightColour[i] += calculateLighting(distance, normal, diffuseColour[i]) * attenuationValue;
                lightColour[i] += specular * attenuationValue;
                
                break;
            
            case 2:
                specular = calculateSpecular(
                    normalize(lightPosition[i].xyz - worldPosition),
                    normal,
                    normalize(cameraPosition.xyz - worldPosition),
                    specularColour[i],
                    specularPower[i].x
                );
                float3 lightDir = normalize(lightPosition[i].xyz - worldPosition);
                innerCone = cos(radians(spotlightConeAngles[i].x));
                outerCone = cos(radians(spotlightConeAngles[i].y));
                theta = dot(lightDir, normalize(-lightDirection[i].xyz));
                float epsilon = innerCone - outerCone;
                float intensity = clamp((theta - outerCone) / epsilon, 0.0f, 1.0f);

                distance = length(lightPosition[i].xyz - worldPosition);
                constantFactor = attenuation[i].x;
                linearFactor = attenuation[i].y;
                quadraticFactor = attenuation[i].z;
                
                attenuationValue = 1 / (constantFactor + (linearFactor * distance) + (quadraticFactor * pow(distance, 2)));
                lightColour[i] = ambient * attenuationValue;
                lightColour[i] += (calculateLighting(distance, normal, diffuseColour[i]) * intensity) * attenuationValue;
                lightColour[i] += (specular * attenuationValue) * intensity;

            
            
                break;
            
            default:
                float4(1.0f, 0.0f, 1.0f, 1.0f); // easy to debug colour
                break;

        }
    }
    return saturate(float4(
    lightColour[0].r + lightColour[1].r + lightColour[2].r + lightColour[3].r,
    lightColour[0].g + lightColour[1].g + lightColour[2].g + lightColour[3].g,
    lightColour[0].b + lightColour[1].b + lightColour[2].b + lightColour[3].b,
    1.0f
    ));
     // light type is stored in the position w value
    
}


float magnitude(float3 _vector)
{
    return sqrt(pow(_vector.x, 2) + pow(_vector.y, 2) + pow(_vector.z, 2));

}

float3 recalculateNormals(float3 currentNormal, float3 bumpMap)
{
    float3 bumpNormal; // Sample the pixel in the bump map.
   

    float3 tangent1 = cross(currentNormal, float3(0, 0, 1));
    float3 tangent2 = cross(currentNormal, float3(0, 1, 0));
    float3 tangent;
    if (magnitude(tangent1) > magnitude(tangent2))
    {
        tangent = tangent1;
    }
    else
    {
        tangent = tangent2;
    }
    

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;
 
    // Calculate the normal from the data in the bump map.
    float3 N = currentNormal;
    float3 T = normalize(tangent - dot(tangent, N) * N);
    float3 B = cross(N, T);
   
    float3x3 TBN = float3x3(T, B, N);
    
    return mul(bumpMap, TBN);
}


float4 main(InputType input) : SV_TARGET
{
    
    float4 lightColour = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    
    float4 textureColour;
    float3 newNormals;

    
    
    

    lightColour = calculateFinalLighting(4, input.normal, input.worldPosition);
    
    textureColour = texture0.Sample(Sampler0, input.tex);
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    return lightColour * textureColour;
}