K15_Vector4 TransformVertex(in K15_Vector4 pos : POSITION,
							in K15_Vector2 uv : TEXCOORD1,
							in K15_Matrix4 mvp : MVPMATRIX,
							out K15_Vector2 out_uv : TEXCOORD1)
{
	out_uv = uv;
	return pos * mvp;
}