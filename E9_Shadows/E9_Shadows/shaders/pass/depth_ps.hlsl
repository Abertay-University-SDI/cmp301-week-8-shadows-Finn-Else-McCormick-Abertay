
struct InputType
{
    float4 position : SV_POSITION;
};

float4 main(InputType input) : SV_TARGET
{
    float depth = input.position.z / input.position.w;
    return float4(depth, depth, depth, 1.0);
}