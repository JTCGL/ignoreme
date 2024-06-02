#pragma once
#include "glres.hpp"
#include <assert.h>

class glshader : public glres {
//class glshader {
public:
	glshader();
	glshader(std::string namein, GLenum stype);
	glshader(std::filesystem::path filepath, std::string namein, GLenum stype);
	~glshader();
	//No copying! Delete copy/assign
	glshader(const glshader&) = delete;
	glshader& operator=(const glshader&) = delete;
	//Only moving
	glshader(glshader&& other) noexcept;
	glshader& operator=(glshader&& other) noexcept;
	
	operator GLuint() const { return id; }//will "cast" the shader to the GLuint type where the shader id is needed, ie, glAttachShader(glshader,...)

	const char* Name() const;
	GLuint Id() const;
	GLenum Type() const;
	void Compile(std::string src) noexcept;

private:
	void Release();
	std::string strname = "";
	GLuint id = 0;
	GLuint shadertype = GL_VERTEX_SHADER;
	std::filesystem::path srcpath="";
};
