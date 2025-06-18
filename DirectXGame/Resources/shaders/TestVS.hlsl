#include "Test.hlsli"

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    output.position = input.position;
    output.texcoord = input.texcoord; // ←これが必須
    
    return output;
}