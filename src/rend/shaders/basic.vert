#version 460 core
layout(location =0)in vec3 vPos;


uniform mat4 ModelMatrix;
//enough for static simulations project i dont want to make unifrom buffer since mostly i m use going to show one object at a time if not then change uniform
uniform mat4 ViewMatrix;
uniform mat4 ProjMatrix;

void main(){
	gl_Position = ProjMatrix * ViewMatrix* ModelMatrix * vec4(vPos,1.0);	
}