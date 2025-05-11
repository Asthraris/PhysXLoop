#include "Engine.h"



#define WIN_X 1080
#define WIN_Y 720


#include <Eigen/dense>

Color bg = { 0.8f,0.8f,0.8f,1.0f };


Engine::Engine()
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//core since i m pro ;}
	//create win and set it as primary space to use gl
	window = glfwCreateWindow(WIN_X, WIN_Y, "PISICS Engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	//load gl functions 
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glClearColor(bg.r, bg.g, bg.b, bg.a);
}

Engine::~Engine()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::run()
{
	e_shader = std::make_unique<Shader>("src/rend/shaders/basic.vert", "src/rend/shaders/basic.frag");
	e_shader->Activate();

	e_cam = std::make_unique<ArcBall>(45, 0.1f, 10.f, (float)WIN_X / (float)WIN_Y);
	while (!glfwWindowShouldClose(window)) 
	{
		e_cam->Update(window);
		e_shader->upload2GPU(MODEL, nullptr);
		e_shader->upload2GPU(VIEW, e_cam->renderView());
		e_shader->upload2GPU(PERS, e_cam->getProjMatrix());

		glClear(GL_COLOR_BUFFER_BIT);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}
