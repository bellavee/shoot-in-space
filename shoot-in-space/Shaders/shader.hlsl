// TriangleVS.hlsl
float4 main(float4 position : POSITION) : SV_POSITION
{
    return position;
}

// TrianglePS.hlsl
float4 main() : SV_TARGET
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f); // Red color
}
