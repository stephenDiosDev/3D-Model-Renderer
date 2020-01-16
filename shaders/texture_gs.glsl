#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Data
{
        vec2 texCoord;
} data_in[];

out Data
{
        vec2 texCoord;
} data_out;

void main()
{
	gl_Position = gl_in[0].gl_Position;
        data_out.texCoord = data_in[0].texCoord;
	EmitVertex();
	
	gl_Position = gl_in[1].gl_Position;
        data_out.texCoord = data_in[1].texCoord;
	EmitVertex();
	
	gl_Position = gl_in[2].gl_Position;
        data_out.texCoord = data_in[2].texCoord;
	EmitVertex();

	EndPrimitive();
}
