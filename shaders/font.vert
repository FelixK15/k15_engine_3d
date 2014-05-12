in vec4 a_Position;
in vec2 a_TexCoord;

out vec2 v_UV;

void main(void)
{
	v_UV = a_TexCoord;
	gl_Position = a_Position;
}