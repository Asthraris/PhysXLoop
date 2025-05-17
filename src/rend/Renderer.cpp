#include "Renderer.hpp"
#include <Eigen/dense>


//debug
#include <iostream>

constexpr int WIN_SIZE[2] = { 1080,720 };
constexpr int INITIAL_TRANFORM_SSBO_SIZE = 10, FLOATS_PER_MATRIX4 = 16;


Color bg = { 0.8f,0.8f,0.8f,1.0f };

float Timer() {
	static double lastTime = 0.0;
	double currtime = glfwGetTime();
	float dt = float(currtime - lastTime);
	lastTime = currtime;
	return dt;
}

void Renderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	if (renderer && renderer->e_cam) {
		renderer->e_cam->setScrollDelta(static_cast<float>(yoffset));
	}
}


Renderer::Renderer(std::shared_ptr<std::vector<std::unique_ptr<Body>>> Ent):Entities(Ent)
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//core since i m pro ;}
	//create win and set it as primary space to use gl
	window = glfwCreateWindow(WIN_SIZE[0], WIN_SIZE[1], "PISICS Engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	//load gl functions 
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glClearColor(bg.r, bg.g, bg.b, bg.a);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WIN_SIZE[0], WIN_SIZE[1]);


	//this func gives access to renderer class to glfw
	glfwSetWindowUserPointer(window, this);



	library = new MeshLibrary();
}

Renderer::~Renderer()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::run(std::function<void(float)> engineUpdate)
{
	glfwSetScrollCallback(window,Renderer::scroll_callback);

	e_shader = std::make_unique<Shader>("src/rend/shaders/basic.vert","src/rend/shaders/basic.frag");
	e_shader->Activate();


	if (library->Cube_shape_vertex == nullptr)exit(2);
	if (library->Sphere_shape_vertex == nullptr)exit(2);

	glGenVertexArrays(1, &CUBE_SHAPE_VAO);
	glBindVertexArray(CUBE_SHAPE_VAO);
	glGenBuffers(1, &CUBE_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
	glBufferData(GL_ARRAY_BUFFER, library->Cube_shape_vertex->getSizeofShapeVertex(), library->Cube_shape_vertex->vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(myVec3), nullptr);
	glGenBuffers(1, &CUBE_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CUBE_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, library->Cube_shape_vertex->getSizeofShapeIndices(), library->Cube_shape_vertex->indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	glGenVertexArrays(1, &SPHERE_SHAPE_VAO);
	glBindVertexArray(SPHERE_SHAPE_VAO);
	glGenBuffers(1, &SPHERE_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, SPHERE_VBO);
	glBufferData(GL_ARRAY_BUFFER, library->Sphere_shape_vertex->getSizeofShapeVertex(), library->Sphere_shape_vertex->vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(myVec3), nullptr);
	glGenBuffers(1, &SPHERE_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SPHERE_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, library->Sphere_shape_vertex->getSizeofShapeIndices(), library->Sphere_shape_vertex->indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	e_cam = std::make_unique<ArcBall>(45, 0.1f, 10.f, ((float)WIN_SIZE[0] / (float)WIN_SIZE[1]));
	while (!glfwWindowShouldClose(window)) 
	{
		deltaTime = Timer();
		e_cam->Update(window);
		e_shader->upload2GPU(VIEW, e_cam->renderView());
		e_shader->upload2GPU(PERS, e_cam->getProjMatrix());


		engineUpdate(deltaTime); 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& entity : *Entities) {
			e_shader->upload2GPU(MODEL, entity->ConstructTransformMat());
			if (entity->Body_shape == CUBE) {
				glBindVertexArray(CUBE_SHAPE_VAO);
				glDrawElements(GL_TRIANGLES, library->INDICES_COUNT_CUBE, GL_UNSIGNED_INT, nullptr);
			}
			else {
				glBindVertexArray(SPHERE_SHAPE_VAO);
				glDrawElements(GL_TRIANGLES, library->INDICES_COUNT_SPHERE, GL_UNSIGNED_INT, nullptr);
			}
		}
		

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}
