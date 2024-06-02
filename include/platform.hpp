#pragma once

//#include <stdio.h>
#include <cinttypes>
#include <cstdio>
#include <string>
#include <vector>
//#include <filesystem>

#ifdef USE_GLAD
#include "glad.h"
#endif

#define IMGUI_IMPL_OPENGL_ES3
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

//#include <SDL.h>
//#include <SDL_opengles2.h>

//#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengles2.h>

#include "editorgui.hpp"

/*
#define DEBUG
#ifdef DEBUG
#define SHOWGLERRS
#endif
*/

#define SHOWGLERRS
//constexpr bool showglerrs = true;
constexpr unsigned char glerrstrcharcnt = 128;
extern char lastglerrstr[glerrstrcharcnt];
extern void CHECKGLERR(void);

constexpr const char* gl_enum_to_char(const GLenum val);

/*
inline void CHECKGLERR(void) {
#ifdef SHOWGLERRS
	GLuint glerr = glGetError();
	switch (glerr) {
		//snprintf(lastglerrstr, 1, "");
		case GL_OUT_OF_MEMORY: snprintf(lastglerrstr, glerrstrcharcnt,"GL_OUT_OF_MEMORY\n"); break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: snprintf(lastglerrstr, glerrstrcharcnt, "GL_INVALID_FRAMEBUFFER_OPERATION\n"); break;
		case GL_INVALID_OPERATION: snprintf(lastglerrstr, glerrstrcharcnt, "GL_INVALID_OPERATION\n"); break;
		case GL_INVALID_VALUE: snprintf(lastglerrstr, glerrstrcharcnt, "GL_INVALID_VALUE\n"); break;
		case GL_INVALID_ENUM: snprintf(lastglerrstr, glerrstrcharcnt, "GL_INVALID_ENUM\n"); break;
		case GL_NO_ERROR: snprintf(lastglerrstr, glerrstrcharcnt, "GL_NO_ERROR\n"); break;
		default: break;
	}
	printf("%s\n", lastglerrstr);
#endif
}
*/
/*
calling glGetError by itself will return the last error, and reset it to GL_NO_ERROR for future calls.
Something - i don't know what - is leaving GL_INVALID_ENUM up after SDL and ImGUI are initialized... they work, but thats disconcerting
something in "ImGui_ImplSDL2_ProcessEvent" - is triggering it, and I need a clean slate before i issue any gl calls
*/
inline void CLEARGLERR() {
	GLuint clearerr = glGetError();
}
