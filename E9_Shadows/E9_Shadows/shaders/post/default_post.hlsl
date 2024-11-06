// Default postprocessing pixel shader (no effect)

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
    float4 outputColor;
	
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    outputColor = texture0.Sample(samplerState, input.tex);

    return outputColor;
}