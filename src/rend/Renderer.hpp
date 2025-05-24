#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>

#include"../Utils.hpp"
#include"Shader.hpp"
#include"ArcBall.hpp"
#include"Renderer_Data.hpp"
#include"../PhysicsBody.h"


struct Playbar {
	bool Simulate;
	bool Move;
	//int listof demo avail
};


class Renderer
{
	GLFWwindow* window;
	std::unique_ptr<Shader> e_shader;
	std::unique_ptr<ArcBall> e_cam;
	MeshLibrary* library;
	std::shared_ptr<std::vector<std::unique_ptr<Body>>> Entities;

	// trans_ssbo_id;
	unsigned int CUBE_SHAPE_VAO, CUBE_VBO, CUBE_EBO;
	unsigned int SPHERE_SHAPE_VAO, SPHERE_VBO, SPHERE_EBO;


	float deltaTime;
	Playbar playbar;
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void imGuiInit();
	void imGuiDraw();
	void imGuiRender();

	void setupBuffers();
	
public:
	Renderer(std::shared_ptr<std::vector<std::unique_ptr<Body>>> Ent);
	~Renderer();
	void run(std::function<void(float)> engineUpdate , std::function <void()>engineUi);//lambda func take float and return void
};

