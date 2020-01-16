#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Data
{
        vec3 position;
        vec3 normal;
} data_in[];

out Data
{
        vec3 position;
        vec3 normal;
} data_out;

void main()
{


        vec3 A = data_in[0].position;
        vec3 B = data_in[1].position;
        vec3 C = data_in[2].position;

        vec3 AB = vec3(B.x - A.x, B.y - A.y, B.z - A.z);
        vec3 AC = vec3(C.x - A.x, C.y - A.y, C.z - A.z);

        //get cross prod
        vec3 crossed = cross(AB, AC);
        vec3 norm = normalize(crossed);

        gl_Position = gl_in[0].gl_Position;
        data_out.position = data_in[0].position;
        data_out.normal = norm;
	EmitVertex();
	
        gl_Position = gl_in[1].gl_Position;
        data_out.position = data_in[1].position;
        data_out.normal = norm;
	EmitVertex();
	
        gl_Position = gl_in[2].gl_Position;
        data_out.position = data_in[2].position;
        data_out.normal = norm;
	EmitVertex();

	EndPrimitive();
}
