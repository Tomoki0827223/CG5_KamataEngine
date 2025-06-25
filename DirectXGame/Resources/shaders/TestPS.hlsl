#include "Test.hlsli"

Texture2D<float32_t4> gTexture : register(t0); // SRV
SamplerState gSampler : register(s0); // Sampler

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float32_t2 uv = input.texcoord;
    float32_t4 textureColor = gTexture.Sample(gSampler, uv);

    //output.color = textureColor; // non effect
    output.color = float4(1, 0, 0, 1);
    
    return output;
}