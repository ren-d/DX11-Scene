Texture2D texture0 : register(t0);
Texture2D textureNormalMap : register(t1);
Texture2D textureSpecMap : register(t2);

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
    
};

cbuffer CameraBuffer : register(b2)
{
    float4 cameraPosition;
    float4 cameraDirection;
}



struct InputType
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
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
    return specularColour * specularIntensity;
}


float4 calculateFinalLighting(int numberOfLights, float3 normal, float3 worldPosition, float4 specularMap)
{
    float4 lightColour[4];
    float distance, constantFactor,
    linearFactor, quadraticFactor, attenuationValue, 
    outerCone, theta, innerCone, ambientAtten;

    float4 specular = float4(0.0f,0.0f,0.0f, 1.0f);
    
    for (int i = 0; i < numberOfLights; i++)
    {
        switch (lightPosition[i].w) // light type is stored in the position w value
        {
            case 0: // directional light calculation
                lightColour[i] = calculateLighting(-lightDirection[i].xyz, normal, diffuseColour[i]);
                break;
            
            case 1: // point light calculatioin
                
                specular = calculateSpecular(
                    normalize(lightPosition[i].xyz - worldPosition),
                    normal,
                    normalize(cameraPosition.xyz - worldPosition),
                    specularMap * specularColour[i],
                    specularPower[i].x
                );
                distance = length(lightPosition[i].xyz - worldPosition);
                constantFactor = attenuation[i].x;
                linearFactor = attenuation[i].y;
                quadraticFactor = attenuation[i].z;
                
                attenuationValue = 1 / (constantFactor + (linearFactor * distance) + (quadraticFactor * pow(distance, 2)));
                lightColour[i] = ambient * attenuationValue;
                lightColour[i] += calculateLighting(distance, normal, diffuseColour[i]) * attenuationValue;
                lightColour[i].rgb += specular.rgb;
                
                break;
            
            case 2: // spotlight calculation
                specular = calculateSpecular(
                    normalize(lightPosition[i].xyz - worldPosition),
                    normal,
                    normalize(cameraPosition.xyz - worldPosition),
                    specularMap * specularColour[i],
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
                lightColour[i].rgb += (specular.rgb * attenuationValue) * 50;

                break;
            
            default:
                float4(1.0f, 0.0f, 1.0f, 1.0f); // easy to debug colour
                break;

        }
    }
    
    // final colour value
    return float4(
    lightColour[0].r + lightColour[1].r + lightColour[2].r + lightColour[3].r,
    lightColour[0].g + lightColour[1].g + lightColour[2].g + lightColour[3].g,
    lightColour[0].b + lightColour[1].b + lightColour[2].b + lightColour[3].b,
    1.0f
    );
     
    
}

float magnitude(float3 _vector)
{
    return sqrt(pow(_vector.x, 2) + pow(_vector.y, 2) + pow(_vector.z, 2));

}

float3 recalculateNormals(float3 currentNormal, float3 bumpMap)
{
    float3 bumpNormal; // Sample the pixel in the bump map.
   

    float3 tangent1 = cross(currentNormal, float3(0, 0, 1)); // forward vector
    float3 tangent2 = cross(currentNormal, float3(0, 1, 0)); // up vector
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
    float4 lightColour = float4(0.0f, 0.0f, 0.0f,1.0f);
    float4 textureColour;
    float3 newNormals;
    float3 bumpMap;
    float4 specMap = textureSpecMap.Sample(Sampler0, input.tex).rgba;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    bumpMap = textureNormalMap.Sample(Sampler0, input.tex).rgb;
    // Normalize the resulting bump normal.
    
    newNormals = recalculateNormals(input.normal, bumpMap);
    
    lightColour = calculateFinalLighting(4, newNormals, input.worldPosition, specMap);
    textureColour = texture0.Sample(Sampler0, input.tex);
    

    return lightColour * textureColour;
}

