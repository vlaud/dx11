struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D tex : TEXTURE : register(t0);
SamplerState samp : SAMPLER : register(s0);

float4 ps(PS_INPUT input) : SV_Target
{
    return tex.Sample(samp, input.uv);
}