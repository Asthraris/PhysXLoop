#include "Renderer.hpp"
#include <Eigen/dense>

#define WIN_X 1080
#define WIN_Y 720
#define INITIAL_TRANFORM_SSBO_SIZE 10
#define FLOATS_PER_MATRIX4 16

Color bg = { 0.8f,0.8f,0.8f,1.0f };

float Timer() {
	static double lastTime = 0.0;
	double currtime = glfwGetTime();
	float dt = float(currtime - lastTime);
	lastTime = currtime;
	return dt;
}
Renderer::Renderer(std::shared_ptr<std::vector<Body>> Ent):Entities(Ent), library()
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

	/*  IDHAR MUJHE JAB ENHANCE KARNA HOGA TAB ME WHEN I GET PERFORMNACE BOTTLENECK MAI MODEL MATRIX  MAKE SSBO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &trans_ssbo_id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, trans_ssbo_id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, INITIAL_TRANFORM_SSBO_SIZE * FLOATS_PER_MATRIX4 * sizeof(float), e_scene->get);
	*/
	glGenVertexArrays(1, &CUBE_SHAPE_VAO);
	glGenBuffers(GL_VERTEX_ARRAY, &CUBE_VBO);
	glBindBuffer(GL_VERTEX_ARRAY, CUBE_VBO);
	glBufferData(GL_VERTEX_ARRAY, library->Cube_shape_vertex->getSizeofShapeVertex(), library->Cube_shape_vertex->vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
	glGenBuffers(GL_ELEMENT_ARRAY_BUFFER, &CUBE_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CUBE_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, library->Cube_shape_vertex->getSizeofShapeIndices(), library->Cube_shape_vertex->indices.data(), GL_STATIC_DRAW);


	glGenVertexArrays(1, &SPHERE_SHAPE_VAO);
	glGenBuffers(GL_VERTEX_ARRAY, &SPHERE_VBO);
	glBindBuffer(GL_VERTEX_ARRAY, SPHERE_VBO);
	glBufferData(GL_VERTEX_ARRAY, library->Sphere_shape_vertex->getSizeofShapeVertex(), library->Sphere_shape_vertex->vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
	glGenBuffers(GL_ELEMENT_ARRAY_BUFFER, &SPHERE_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SPHERE_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, library->Sphere_shape_vertex->getSizeofShapeIndices(), library->Sphere_shape_vertex->indices.data(), GL_STATIC_DRAW);


	e_cam = std::make_unique<ArcBall>(45, 0.1f, 10.f, (float)WIN_X / (float)WIN_Y);
	while (!glfwWindowShouldClose(window)) 
	{
		deltaTime = Timer();
		e_cam->Update(window);
		e_shader->upload2GPU(VIEW, e_cam->renderView());
		e_shader->upload2GPU(PERS, e_cam->getProjMatrix());


		engineUpdate(deltaTime); 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Body entity : *Entities) {
			e_shader->upload2GPU(MODEL, entity.ConstructTransformMat());
			if (entity.Body_shape == CUBE) {
				glBindVertexArray(CUBE_SHAPE_VAO);
				glDrawElements(GL_TRIANGLES, library->INDICES_COUNT_CUBE, GL_UNSIGNED_INT, nullptr);
			}
			else {
				glBindVertexArray(SPHERE_SHAPE_VAO);
				glDrawElements(GL_TRIANGLES, library->INDICES_COUNT_SPHERE, GL_UNSIGNED_INT, nullptr);
			}

			//drawcall
		}
		

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}
