#pragma once
#include "platform.hpp"

class shaderwrap {
private:
	void Release() {
		SDL_Log("%s %s id:%d\n", __FUNCTION__, shadername.c_str(), shaderid);
		glDeleteShader(shaderid);
		shaderid = 0;
	}
public:

	//shaderwrap() { SDL_Log("%s %s\n", __FUNCTION__, shadername.c_str()); }
	shaderwrap(std::string namein, GLenum stype) :shadername(namein), shaderid(0), shadertype(stype) {
		shaderid = glCreateShader(shadertype);
		SDL_Log("%s %s\n", __FUNCTION__, shadername.c_str());
	}
	~shaderwrap() {
		shadername += "killed";
		SDL_Log("%s %s\n", __FUNCTION__, shadername.c_str());
		SDL_Log("GL id: %d(pre-release)\n", shaderid);
		Release();
	}

	shaderwrap(const shaderwrap&) = delete;
	shaderwrap& operator=(const shaderwrap&) = delete;

	//move
	shaderwrap(shaderwrap&& other) : shadername(other.shadername), shadertype(other.shadertype), shaderid(other.shaderid)
	{
		shadername += ("moved");
		other.shaderid = 0; //Use the "null" texture for the old object.
		SDL_Log("%s %s\n", __FUNCTION__, shadername.c_str());
	}
	//move assign
	shaderwrap& operator=(shaderwrap&& other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other) {
			Release();
			//shaderid is now 0.
			std::swap(shaderid, other.shaderid);
			std::swap(shadername, other.shadername);
			std::swap(shadertype, other.shadertype);
			SDL_Log("%s %s\n", __FUNCTION__, shadername.c_str());
			//Release();
		}
		return *this;
	}

	std::string shadername = "defctornam";//ie, "Vertex color", "Shadow Gen vertex" etc
	GLuint shaderid = 0;//storing the gl handle directly...
	GLuint shadertype = GL_VERTEX_SHADER;
	void CreateGLObject() {
		shaderid = glCreateShader(shadertype);
		if (!shaderid) { CHECKGLERR(); }
	}
	void DeleteGLObject() {
		GLint delStatus = 0;
		glGetShaderiv(shaderid, GL_DELETE_STATUS, &delStatus);
		if (!delStatus)	glDeleteShader(shaderid);
	}
	const char* Name() const { return shadername.c_str(); }
};

