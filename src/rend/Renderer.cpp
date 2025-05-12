#include "Renderer.hpp"
#include <Eigen/dense>

#define WIN_X 1080
#define WIN_Y 720
Color bg = { 0.8f,0.8f,0.8f,1.0f };

float Timer() {
	static double lastTime = 0.0;
	double currtime = glfwGetTime();
	float dt = float(currtime - lastTime);
	lastTime = currtime;
	return dt;
}
Renderer::Renderer(std::shared_ptr<Scene> scene):
	e_scene(scene)
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
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WIN_X, WIN_Y);
}

Renderer::~Renderer()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::run(std::function<void(float)> engineUpdate)
{
	e_shader = std::make_unique<Shader>("src/rend/shaders/basic.vert", "src/rend/shaders/basic.frag");
	e_shader->Activate();
	Eigen::Matrix4f model_mat ;



	e_cam = std::make_unique<ArcBall>(45, 0.1f, 10.f, (float)WIN_X / (float)WIN_Y);
	while (!glfwWindowShouldClose(window)) 
	{
		deltaTime = Timer();
		e_cam->Update(window);
		e_shader->upload2GPU(VIEW, e_cam->renderView());
		e_shader->upload2GPU(PERS, e_cam->getProjMatrix());


		engineUpdate(deltaTime); 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto entity : e_scene->Static_entities) {
			e_shader->upload2GPU(MODEL, entity.ConstructTransformMat());

		}
		for (auto entity : e_scene->Static_entities) {
			e_shader->upload2GPU(MODEL, entity.ConstructTransformMat());

		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}
