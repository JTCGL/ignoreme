#pragma once
#include "platform.hpp"

class shaderwrap;

class programwrap {
private:
	void Release() {
		SDL_Log("%s %s id:%d\n", __FUNCTION__, programname.c_str(), programid);
		glDeleteProgram(programid);
		programid = 0;
	}
public:
	programwrap() {
		SDL_Log(__FUNCTION__);
		programid = glCreateProgram();
		//glLinkProgram();
		//glAttachShader(programid);
	}
	programwrap(std::string namein) : programname(namein) {
		SDL_Log(__FUNCTION__);
		programid = glCreateProgram();
		/*
		glLinkProgram(programid);
		CHECKGLERR();
		SDL_Log("%s, %s\n", __FUNCTION__, lastglerrstr);
		*/
	}
	~programwrap() {
		SDL_Log("%s\n", __FUNCTION__);
		Release();
	}
	programwrap(const programwrap&) = delete;
	programwrap& operator=(const programwrap&) = delete;

	//move
	programwrap(programwrap&& other) noexcept : programname(other.programname), programid(other.programid)
	{
		//programname += ("moved");
		other.programid = 0; //Use the "null" texture for the old object.
		SDL_Log("%s %s\n", __FUNCTION__, programname.c_str());
	}
	//move assign
	programwrap& operator=(programwrap&& other) noexcept
	{
		//ALWAYS check for self-assignment.
		if (this != &other) {
			Release();
			//shaderid is now 0.
			std::swap(programid, other.programid);
			std::swap(programname, other.programname);
			SDL_Log("%s %s\n", __FUNCTION__, programname.c_str());
			//Release();
		}
		return *this;
	}

	//std::string Name() const { return programname; }
	//const char* Name() const { return programname.c_str(); }
	const char* Name() const { return programname.c_str(); }
	GLuint Id() const { return programid; }

	void RemoveShader() {}//Placeholder - call this when a shader is removed from the program, and handle the shader index update!

	std::string programname = "defprg";
	GLuint programid = 0;
	GLint vsIndex = -1, fsindex = -1;//indices into shaders_w, -1 for unassigned, as vector indices start at zero!
};