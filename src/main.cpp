#define USE_GLAD

#include <stdio.h>
#ifdef USE_GLAD
#include "glad.h"
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include "triangle_renderer.hpp"

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}

	// We use OpenGL ES 2.0
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// We want at least 8 bits per color
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	SDL_Window *window = SDL_CreateWindow("Hello SDL2 OpenGL ES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return 1;
	}

	// We will not actually need a context created, but we should create one
	SDL_GL_CreateContext(window);

	#ifdef USE_GLAD
		gladLoadGLES2Loader(SDL_GL_GetProcAddress);
	#endif

	SDL_Log("Vendor   : %s", glGetString(GL_VENDOR));
	SDL_Log("Renderer : %s", glGetString(GL_RENDERER));
	SDL_Log("Version  : %s", glGetString(GL_VERSION));
	SDL_Log("GLSL     : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	/*
	GLint numext = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numext);
	SDL_Log("numext     : %d", numext);
	for (auto e = 0; e < numext; e++) {
		SDL_Log("%d:\t%s\n", e, glGetStringi(GL_EXTENSIONS, e));
	}
	*/

	// Initialize triangle renderer
	triangle_init();

	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// Process events so the app doesn't hang
		}
		// Draw triangle
		triangle_render(SDL_GetTicks());
		// Swap buffers
		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}