#version 460 core


uniform mat4 ModelMatrix;
//enough for static simulations project i dont want to make unifrom buffer since mostly i m use going to show one object at a time if not then change uniform
uniform mat4 ViewMatrix;
uniform mat4 ProjMatrix;

void main(){
	mat4 res = ProjMatrix* ViewMatrix* ModelMatrix;
	
	
}