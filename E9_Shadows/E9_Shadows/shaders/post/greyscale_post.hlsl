
Texture2D texture0 : register(t0);
SamplerState samplerState : register(s0);

cbuffer TimeBuffer : register(b0)
{
    float elapsedTime;
    float _padding_t[3];
}

cbuffer ScreenBuffer : register(b1)
{
    float width;
    float height;
    float _padding_s[2];
}

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    float4 inputCol = texture0.Sample(samplerState, input.tex);
    
    float value = (inputCol.r + inputCol.g + inputCol.b) / 3.0;
    
    float outputCol = float4(value, value, value, inputCol.a);
    
    return outputCol;
}