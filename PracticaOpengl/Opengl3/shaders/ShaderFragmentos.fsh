#version 330 core
uniform sampler2D tex;
uniform sampler2D tex2;
uniform float alpha;
in vec2 otexCoord;

out vec4 outColor;

void main()
{
	outColor = alpha*texture(tex, otexCoord) + (1-alpha)*texture(tex2, otexCoord);
}