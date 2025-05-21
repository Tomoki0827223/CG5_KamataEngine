struct PixselShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixselShaderOutput main()
{
    PixselShaderOutput output;
    output.color = float32_t4(1.0, 1.0, 1.0, 1.0);
    return output;
}