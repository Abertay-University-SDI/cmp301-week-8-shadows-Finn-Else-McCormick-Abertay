// Blur postprocessing shader

Texture2D texture0 : register(t0);
SamplerState samplerState : register(s0);

cbuffer ScreenBuffer : register(b1)
{
    float width;
    float height;
    float _padding_s1;
    float _padding_s2;
}

#define NUM_WEIGHTS 5
cbuffer BlurBuffer : register(b2)
{
    float weights[NUM_WEIGHTS];
}

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);
    
    float radius = 2;
    
    float texelHeight = 1 / height;
    
    color += texture0.Sample(samplerState, input.tex) * weights[0];
    for (int i = 1; i < NUM_WEIGHTS; ++i)
    {
        color += texture0.Sample(samplerState, input.tex + float2(0, texelHeight * -i * radius)) * weights[i];
        color += texture0.Sample(samplerState, input.tex + float2(0, texelHeight * i * radius)) * weights[i];
    }
    
    color.a = 1;

    return color;
}