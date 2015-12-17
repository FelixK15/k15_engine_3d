float4 TransformVertex(in float4 pos : POSITION,
					in float2 uv : TEXCOORD1,
					in float4x4 mvp : MVPMATRIX,
					out float2 out_uv : TEXCOORD1)
{
	out_uv = uv;
	return pos * mvp;
}