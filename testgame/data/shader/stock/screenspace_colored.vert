float4 TransformVertex(in float2 pos : POSITION,
		       in float3 color : COLOR1,
		       in float4x4 gui_proj : GUIPROJECTIONMATRIX,
		       out float3 out_color : COLOR1)
{
	float4 p = float4(pos, 1.f, 1.0f);
	float4 transformedPos = gui_proj * p;
	out_color = color;
	return transformedPos;
}

