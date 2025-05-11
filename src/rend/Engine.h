#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include"../Utils.hpp"
#include"Shader.h"
#include"ArcBall.hpp"


class Engine
{
	GLFWwindow* window;
	std::unique_ptr<Shader> e_shader;
	std::unique_ptr<ArcBall> e_cam;
public:
	Engine();
	~Engine();
	void run();
};

