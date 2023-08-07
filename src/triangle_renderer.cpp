#include "triangle_renderer.hpp"
#include <math.h>

/*
* GLES2 is defined in CMakeLists, and propogated to the generated project files
*/

#define USE_GLAD //DONT DUPLICATE! Fix this so its only set ***ONCE***!

//#include "glad.h"
#if defined(GLES2)
	#ifdef USE_GLAD
	#include "glad.h"
	#else
	#include <GLES2/gl2.h>
	#endif
#else
#error Wrong GLES version selected (or none)
#endif

// OpenGL ES 2.0 uses shaders
#ifdef GLES2

const char* vs = R"(
attribute vec4 position;
attribute vec4 color;
varying vec4 vcolor;
void main() {
gl_Position = vec4(position.xyz, 1.0);
vcolor = color;
}
)";
const char *VERTEX_SHADER =
	"attribute vec4 position;\n"
	"attribute vec4 color;\n"
	"varying vec4 vcolor;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.xyz, 1.0);\n"
	"vcolor = color;\n"
	"}";

const char *FRAGMENT_SHADER =
	"precision mediump float;\n"
	"varying vec4 vcolor;\n"
	"void main()\n"
	"{\n"
	"gl_FragColor = vec4 (vcolor.xyz, 1.0);\n"
	"}";
#endif

static GLfloat vertexes[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};
static GLfloat colors[12] = {};

void triangle_init()
{
#ifdef GLES2
	// Load shaders
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &VERTEX_SHADER, NULL);
	glCompileShader(vertex);

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &FRAGMENT_SHADER, NULL);
	glCompileShader(fragment);

	// Combine shaders into program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	// Use it
	glUseProgram(program);

	// Point position attribute to vertexes
	GLint position = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 0, vertexes);
	// Point color attribute to colors
	GLint color = glGetAttribLocation(program, "color");
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, colors);
#endif
}

void triangle_render(double timemillis)
{
	#ifndef M_PI
		float M_PI = 3.14f;
	#endif
	// Animate colors
	for (int i = 0; i < 3; i++)
	{
		colors[i * 4 + 0] = sin(i * M_PI * 2 / 3 + timemillis / 1000) / 2 + 0.5f;
		colors[i * 4 + 1] = sin((i + 1) * M_PI * 2 / 3 + timemillis / 1000) / 2 + 0.5f;
		colors[i * 4 + 2] = sin((i + 2) * M_PI * 2 / 3 + timemillis / 1000) / 2 + 0.5f;
		colors[i * 4 + 3] = 1.0f;
	}
	// Clear screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef GLES1
	// No shaders used in OpenGL ES 1.0, use this instead
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexes);
	glColorPointer(4, GL_FLOAT, 0, colors);
#endif
	// Draw triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
}