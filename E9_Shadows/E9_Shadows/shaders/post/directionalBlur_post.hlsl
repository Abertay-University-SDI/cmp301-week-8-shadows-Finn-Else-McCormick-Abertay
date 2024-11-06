// Directional blur postprocessing shader

Texture2D texture0 : register(t0);
SamplerState samplerState : register(s0);

cbuffer ScreenBuffer : register(b1)
{
    float width;
    float height;
    float _spadding1;
    float _spadding2;
}

cbuffer DirectionalBlurBuffer : register(b2)
{
    float angle;
    float distance;
    int samples;
    float _bpadding;
}

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);
    
    float2 texelSize = float2(1 / width, 1 / height);
    
    float2 blurVec = float2(cos(angle), sin(angle)) * (distance / samples);
    blurVec.x *= texelSize.x;
    blurVec.y *= texelSize.y;
    
    for (int i = 0; i < samples; ++i)
    {
        float2 samplePoint = input.tex + blurVec * i;
        color += texture0.Sample(samplerState, samplePoint) / samples;
    }
    
    color.a = 1;

    return color;
}