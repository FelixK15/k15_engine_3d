float4 TransformVertex(in float2 pos : POSITION,
		       in float3 color : COLOR1,
		       out float3 out_color : COLOR1)
{
	out_color = color;
	return float4(pos, 1.0, 1.0);
}

