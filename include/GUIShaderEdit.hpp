#ifndef GUISHADEREDIT_HPP
#define GUISHADEREDIT_HPP
#pragma once

#include "platform.hpp"
//#include "shaderwrap.hpp"
#include "programwrap.hpp"
#include "glshader.hpp"

//https://www.khronos.org/opengl/wiki/Common_Mistakes#The_Object_Oriented_Language_Problem

class programwrap;
class shaderwrap;

//extern std::vector<shaderwrap>shaders_w;
extern std::vector<glshader>shaders_w;
extern std::vector<programwrap>programs_w;

extern void showshaderdlg(bool* p_open);

#endif
