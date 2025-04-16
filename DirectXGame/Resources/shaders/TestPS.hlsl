struct PixselShaderOutput
{
    float4 color : SV_TARGET0;
};

PixselShaderOutput main()
{
    PixselShaderOutput output;
    output.color = float4(1.0, 1.0, 1.0, 1.0);
    return output;
}