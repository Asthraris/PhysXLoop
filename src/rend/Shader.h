#pragma once
#include <glad/glad.h>
enum matrix_type {
	MODEL,VIEW,PERS
};
class Shader
{
	unsigned int m_prog_id;
	int m_model_loc, m_view_loc, m_pers_loc;
	bool checkErrors(const unsigned int vertex_id, const unsigned int fragment_id);
public:
	Shader(){}//default
	Shader(const char* vertexpath,const char* fragpath);
	void upload2GPU(matrix_type, const float*);
	void Activate();
	~Shader();
};

