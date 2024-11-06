// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState sampler0 : register(s0);

struct LightType
{
    float3 diffuse;
    float intensity;
    float3 specular;
    float specularPower;
    float4 position;
    float3 direction;
    float umbraAngle;
    float3 distanceFalloff;
    float penumbraAngle;
};

cbuffer TimeBuffer : register(b0)
{
    float elapsedTime;
    float showNormals;
    float disableTexture;
    float normalOffset;
}

#define NUM_LIGHTS 8
cbuffer LightBuffer : register(b1)
{
    float3 ambientColour;
    float padding;
    LightType lights[NUM_LIGHTS];
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
};

float calculateFalloff(float fac, float3 params)
{
    return 1 / (params.x + (params.y * fac) + (params.z * pow(fac, 2)));
}

float3 calculateDiffuseLighting(LightType light, float3 normal, float3 worldPosition)
{
    float3 lightColour;
    // Directional
    if (light.position.w == 0)
    {
        float intensity = saturate(light.intensity * dot(normal, light.direction));
        lightColour = saturate(light.diffuse * intensity);
    }
    // Punctual
    else
    {
        float3 lightVector = light.position.xyz - worldPosition;
        float distance = length(lightVector);
        float3 lightDir = normalize(lightVector);
        
        float spotFac = dot(-lightDir, light.direction);
        float umbraCosine = cos(light.umbraAngle);
        float penumbraAngleClamped = clamp(light.penumbraAngle, light.umbraAngle + 0.001, radians(180) - 0.001);
        float penumbraCosine = cos(penumbraAngleClamped);
        
        float umbraFac = saturate((spotFac - umbraCosine) / (penumbraCosine - umbraCosine));
        
        float spotFalloff = smoothstep(0, 1, umbraFac);
        
        float intensity = saturate(light.intensity * dot(normal, lightDir) * spotFalloff * calculateFalloff(distance, light.distanceFalloff));
        lightColour = saturate(light.diffuse * intensity);
    }
    return lightColour;
}

float3 calculateSpecularLighting(LightType light, float3 normal, float3 viewVector, float3 worldPosition)
{
    float3 lightDir;
    float spotFalloff = 1;
    if (light.position.w == 0)
    {
        return float3(0, 0, 0);
    }
    else
    {
        float3 lightVector = light.position.xyz - worldPosition;
        lightDir = normalize(lightVector);
        float spotFac = dot(-lightDir, light.direction);
        float umbraCosine = cos(light.umbraAngle);
        float penumbraAngleClamped = clamp(light.penumbraAngle, light.umbraAngle + 0.001, radians(180) - 0.001);
        float penumbraCosine = cos(penumbraAngleClamped);
        
        float umbraFac = saturate((spotFac - umbraCosine) / (penumbraCosine - umbraCosine));
        
        spotFalloff = smoothstep(0, 1, umbraFac);
    }
    float3 halfway = normalize(lightDir + viewVector);
    float specularIntensity = pow(max(dot(normal, halfway), 0.0), light.specularPower) * spotFalloff;
    return saturate(light.specular * specularIntensity);
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
    float3 diffuseColour;
    float3 specularColour;
    
    textureColour = albedoTexture.Sample(sampler0, input.tex);
    if (disableTexture == 1.0) { textureColour = float4(1.0, 1.0, 1.0, 1.0); }
    
    //input.normal = normalTexture.Sample(sampler0, input.tex).rgb;
    
    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        diffuseColour = saturate(diffuseColour + calculateDiffuseLighting(lights[i], input.normal, input.worldPosition));
        specularColour = saturate(specularColour + calculateSpecularLighting(lights[i], input.normal, input.viewVector, input.worldPosition));
    }
    
    diffuseColour = saturate(diffuseColour + ambientColour);
    
    if (showNormals == 1.0) { return float4(input.normal, 1); }
	
    return textureColour * float4(diffuseColour, 1) + float4(specularColour, 1);
}



