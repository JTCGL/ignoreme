#pragma once

#define USE_GLAD

#include <stdio.h>
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
#include <SDL2/SDL_opengles2.h>

#include <string>

//app, editor, editor gui widgets

class app {
public:
};

class editor {
public:
};

class dialog {
public:
	dialog() {}
	void doit(bool* p_open) {
		//flags |= ImGuiWindowFlags_NoCollapse;
		flags |= ImGuiWindowFlags_NoResize;
		ImGui::Begin("Hello, class!", p_open, flags);
		ImGui::Text("CLASS TEXT");
		ImVec2 sz = {512,512};
		ImGui::SetWindowSize(sz,0);
		ImGui::End();
	}
private:
	//bool opened = false;
	ImGuiWindowFlags flags = 0;
};

class menu {
public:
};

class widget {
public:
};