#include "Shader.hpp"

#include <fstream>
#include <sstream>

#include <iostream>
bool Shader::checkErrors(const unsigned int shader_id)
{
	{
		int result;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
			char* error = (char*)malloc(length * sizeof(char));
			glGetShaderInfoLog(shader_id, length, &length, error);
			std::cout << "GLSL-ERROR >" << error << "\n";
			return true;
		}
	}
	return false;
}
Shader::Shader(const std::string vertexpath, const std::string fragpath)
{
	try {
		unsigned int vert_id, frag_id;

		std::ifstream V_file_stream(vertexpath);
		if (!V_file_stream)throw "AMAN > vert File cant opened [maybe not exixt wrong path]\n";

		//ek string ko stram karne layak container bana te hai
		std::stringstream vertCache;
		//fir us container me rdbuf ke Ascii conversion karke storekarte hai
		vertCache << V_file_stream.rdbuf();
		//fir woh opened file ko close kardete hai
		V_file_stream.close();
		//joh temp string container se hum copy karte hai to new std::string var (Usable)
		std::string vertSource = vertCache.str();
		const char* vertstr = vertSource.c_str();



		std::ifstream F_file_stream(fragpath);
		if (!F_file_stream)throw "AMAN > frag File cant opened [maybe not exixt wrong path]\n";

		std::stringstream fragCache;
		fragCache << F_file_stream.rdbuf();
		F_file_stream.close();
		std::string fragSource = fragCache.str();
		const char* fragstr = fragSource.c_str();

		vert_id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert_id, 1, &vertstr, nullptr);
		glCompileShader(vert_id);

		frag_id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_id, 1, &fragstr, nullptr);
		glCompileShader(frag_id);

		if (checkErrors(vert_id))throw "AMAN > VERTEX-Shader not Compiled Successfully !\n";
		if (checkErrors(frag_id))throw "AMAN > FRAGMENT-Shader not Compiled Successfully !\n";


		m_prog_id = glCreateProgram();
		glAttachShader(m_prog_id, vert_id);
		glAttachShader(m_prog_id, frag_id);
		glLinkProgram(m_prog_id);

		glDeleteShader(vert_id);
		glDeleteShader(frag_id);


		try {
			m_model_loc = glGetUniformLocation(m_prog_id, "ModelMatrix");
			if (m_model_loc == -1)throw "SHADER > Model mat defination not found\n";

			m_view_loc = glGetUniformLocation(m_prog_id, "ViewMatrix");
			if (m_view_loc == -1)throw "SHADER > View mat defination not found\n";

			m_pers_loc = glGetUniformLocation(m_prog_id, "ProjMatrix");
			if (m_pers_loc == -1)throw "SHADER > Perspective mat defination not found\n";
		}
		catch (std::string Uerr) {
			std::cout << Uerr;
		}



		glUseProgram(0);
		std::cout << "[DEBUG]=> Shader Loaded !\n";
	}
	catch (const char* Error) {
		std::cout << Error;
		//some handling logic
	}
	
}

void Shader::upload2GPU(matrix_type type,const float* Value)
{
	switch (type)
	{
	case MODEL:
		glUniformMatrix4fv(m_model_loc, 1, GL_FALSE, Value);
		break;
	case VIEW:
		glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, Value);
		break;
	case PERS:
		glUniformMatrix4fv(m_pers_loc, 1, GL_FALSE, Value);
		break;
	default:
		std::cout << "AMAN > wrong type to upload matrix to GPU\n";
		break;
	}
}

void Shader::Activate()
{
	glUseProgram(m_prog_id);
}

Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(m_prog_id);
}
