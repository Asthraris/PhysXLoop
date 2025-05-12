#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>

#include"../Utils.hpp"
#include"Shader.hpp"
#include"ArcBall.hpp"
#include"../Scene.hpp"




class Renderer
{
	GLFWwindow* window;
	std::unique_ptr<Shader> e_shader;
	std::unique_ptr<ArcBall> e_cam;
	std::shared_ptr<Scene> e_scene;
	float deltaTime;
	
public:
	Renderer(std::shared_ptr<Scene>);
	~Renderer();
	void run(std::function<void(float)> engineUpdate);//lambda func take float and return void
};

