Texture2D texture0 : register(t0);
Texture2D textureNormalMap : register(t1);
Texture2D textureSpecMap : register(t2);
Texture2D depthMap : register(t3);

SamplerState Sampler0 : register(s0);
SamplerState shadowSampler : register(s1);

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
    float4 lightViewPos[2] : TEXCOORD1;
};

// calculate direction lighting
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 calculateSpecular(float3 lightDirection, float3 normal, float3 viewVector, float4 specularColour, float specularPower)
{
	//blinn-phong specular calculation
    float3 halfway = normalize(lightDirection + viewVector);
    float specularIntensity = pow(max(dot(normal, halfway), 0.0), specularPower);
    return specularColour * specularIntensity;
}

float calculateAttenuation(int iterator_id, float distance)
{
    float constantFactor, linearFactor, quadraticFactor;
    
    constantFactor = attenuation[iterator_id].x;
    linearFactor = attenuation[iterator_id].y;
    quadraticFactor = attenuation[iterator_id].z;
    
    return 1 / (constantFactor + (linearFactor * distance) + (quadraticFactor * pow(distance, 2)));
}

float calculateSpotlight(int iterator_id, float3 lightDir)
{
    float outerCone, theta, innerCone;
     
    innerCone = cos(radians(spotlightConeAngles[iterator_id].x));
    outerCone = cos(radians(spotlightConeAngles[iterator_id].y));
    theta = dot(lightDir, normalize(-lightDirection[iterator_id].xyz));
    
    float epsilon = innerCone - outerCone;
    
    return clamp((theta - outerCone) / epsilon, 0.0f, 1.0f);

}

bool hasDepthData(float2 uv)
{
    if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
    {
        return false;
    }
    return true;
}

bool isInShadow(Texture2D sMap, float2 uv, float4 lightViewPosition, float bias)
{
    float depthValue;
    float lightDepthValue;
    // Sample the shadow map (get depth of geometry)

    depthValue = sMap.Sample(shadowSampler, uv).r;
	// Calculate the depth from the light.
    lightDepthValue = lightViewPosition.z / lightViewPosition.w;
    lightDepthValue -= bias;


	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        return false;
    }

    return true;
}

float2 getProjectiveCoords(float4 lightViewPosition)
{
    // Calculate the projected texture coordinates.
    float2 projTex = lightViewPosition.xy / lightViewPosition.w;
    projTex *= float2(0.5, -0.5);
    projTex += float2(0.5f, 0.5f);
    return projTex;
}
float4 calculateFinalLighting(int numberOfLights, float3 normal, float3 worldPosition, float4 specularMap, float4 lightViewPos)
{
    float4 lightColour[4];
    float distance,
     attenuation;

    float4 specular = specularMap;
    
    for (int i = 0; i < numberOfLights; i++)
    {
        distance = length(lightPosition[i].xyz - worldPosition);
        
        switch (lightPosition[i].w) // light type is stored in the position w value
        {
            case 0: // directional light calculation
                float2 pTexCoord;
                pTexCoord = getProjectiveCoords(lightViewPos);
                lightColour[i] = ambient;
                // Shadow test. Is or isn't in shadow
                if (hasDepthData(pTexCoord))
                {
                    // Has depth map data
                    if (!isInShadow(depthMap, pTexCoord, lightViewPos, 0.005))
                    {
                         // is NOT in shadow, therefore light

                        lightColour[i] += calculateLighting(-lightDirection[i].xyz, normal, diffuseColour[i]);
 
                    }
        
               
                    
                }
                break;
            
            
            // point light calculation
            case 1: 
                
                // Blinn-Phong Specular Calculation
                specular = calculateSpecular(
                    normalize(lightPosition[i].xyz - worldPosition),
                    normal,
                    normalize(cameraPosition.xyz - worldPosition),
                    specularMap * specularColour[i],
                    specularPower[i].x
                );
                attenuation = calculateAttenuation(i, distance);
            
                lightColour[i] = ambient * attenuation;
                lightColour[i] += calculateLighting(distance, normal, diffuseColour[i]) * attenuation;
                lightColour[i].rgb += specular.rgb * attenuation;
                
                break;
            
            case 2: // Blinn-Phong Specular Calculation
                specular = calculateSpecular(
                    normalize(lightPosition[i].xyz - worldPosition),
                    normal,
                    normalize(cameraPosition.xyz - worldPosition),
                    specularMap * specularColour[i],
                    specularPower[i].x
                );
            
                attenuation = calculateAttenuation(i, distance);
            
                float3 lightDir = normalize(lightPosition[i].xyz - worldPosition);
                float intensity = calculateSpotlight(i, lightDir);
            
                
                lightColour[i] = ambient * attenuation;
                lightColour[i] += (calculateLighting(distance, normal, diffuseColour[i]) * intensity) * attenuation;
                lightColour[i].rgb += (specular.rgb * attenuation) * intensity;

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
    
    lightColour = calculateFinalLighting(4, newNormals, input.worldPosition, specMap, input.lightViewPos[0]);
    textureColour = texture0.Sample(Sampler0, input.tex);
    
    return float4(input.tex.x, input.tex.y, 0.0f, 1.0f);
    return lightColour * textureColour;
}

