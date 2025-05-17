#pragma once
#include <glad/glad.h>
#include <string>
enum matrix_type {
	MODEL,VIEW,PERS
};
class Shader
{
	unsigned int m_prog_id;
	int m_model_loc, m_view_loc, m_pers_loc;
	bool checkErrors(const unsigned int);
public:
	Shader(){}//default
	Shader(const std::string vertexpath, const std::string fragpath);
	void upload2GPU(matrix_type, const float*);
	void Activate();
	~Shader();
};

