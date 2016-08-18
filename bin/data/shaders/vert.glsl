#version 150

in vec4 in_pos;
in vec4 vertex_color;
in vec4 vertex_normal;

out Vertex{
	vec4 pos;
	vec4 color;
    vec4 normal;
} vertex;


void main(){
	vertex.pos = in_pos;
	vertex.color =  vertex_color;
    vertex.normal = vertex_normal;
}
