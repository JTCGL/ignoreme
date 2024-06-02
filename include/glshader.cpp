#include "glshader.hpp"

glshader::glshader() : glshader("def", GL_VERTEX_SHADER) {
	//delegates!
}

glshader::glshader(std::string namein, GLenum stype) : strname(namein), id(0), shadertype(stype) {
	id = glCreateShader(shadertype);
	SDL_Log("%s %s\n", __FUNCTION__, strname.c_str());
}

glshader::glshader(std::filesystem::path filepath, std::string namein, GLenum stype) : glshader(namein,stype)
{
	srcpath = filepath;
}

glshader::~glshader() {
	strname += "killed";
	SDL_Log("%s %s\n", __FUNCTION__, strname.c_str());
	SDL_Log("GL id: %d(pre-release)\n", id);
	Release();
}

glshader::glshader(glshader&& other) noexcept : strname(other.strname), shadertype(other.shadertype), id(other.id)
{
	strname += ("moved");
	other.id = 0; //Use the "null" texture for the old object.
	SDL_Log("%s %s\n", __FUNCTION__, strname.c_str());
}

//glshader temp = other;
glshader& glshader::operator=(glshader&& other) noexcept
{
	//ALWAYS check for self-assignment.
	if (this != &other) {
		Release();
		//shaderid is now 0.
		std::swap(id, other.id);
		std::swap(strname, other.strname);
		std::swap(shadertype, other.shadertype);
		SDL_Log("%s %s\n", __FUNCTION__, strname.c_str());
		//Release();
	}
	return *this;
}

const char* glshader::Name() const {
	return strname.c_str();
}

GLuint glshader::Id() const {
	return id;
}

GLuint glshader::Type() const {
	return shadertype;
}

void shadercompile(const GLuint id, const std::string& src) {
	SDL_Log("%s", __FUNCTION__);
	GLsizei logsize;
	static char log[1024] = { "" };
	const char* c_str = src.c_str();
	GLint loglen = 0;
	glShaderSource(id, 1, &c_str, NULL);
	glCompileShader(id);
	glGetShaderInfoLog(id, 1024, NULL, log);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &loglen);
	if (log[0] == '\0')
	//if(loglen != 0)
	{
		sprintf(log, "Empty shader info log, reported: %d", loglen);
		SDL_Log("%s", log);
	}
	SDL_Log("%s", log);
}

void glshader::Compile(std::string src) noexcept {
	shadercompile(id,src);
	/*
	GLsizei logsize;
	static char log[1024] = { "" };
	const char* c_str = src.c_str();
	GLint loglen = 0;
	glShaderSource(id, 1, &c_str, NULL);
	glCompileShader(id);
	glGetShaderInfoLog(id, 1024, NULL, log);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &loglen);
	if (log[0] == '\0') {
		sprintf(log, "Empty shader info log, reported: %d", loglen);
		SDL_Log("%s", log);
	}
	*/
}

void glshader::Release() {
	SDL_Log("%s %s id:%d\n", __FUNCTION__, strname.c_str(), id);
	glDeleteShader(id);
	id = 0;
}