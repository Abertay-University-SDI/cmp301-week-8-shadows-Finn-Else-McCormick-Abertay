// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
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
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 lightColour;
    
    textureColour = texture0.Sample(sampler0, input.tex);
    if (disableTexture == 1.0) { textureColour = float4(1.0, 1.0, 1.0, 1.0); }
    
    lightColour = calculateLighting(-lights[0].direction, input.normal, float4(lights[0].diffuse, 1));
    
    if (showNormals == 1.0) { return float4(input.normal, 1); }
	
    return lightColour * textureColour;
}