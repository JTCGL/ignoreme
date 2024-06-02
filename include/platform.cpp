#include "platform.hpp"

char lastglerrstr[glerrstrcharcnt] = { 0 };

//separate teh enums into categoris, separate functions; error enums, those used for resource creation/deletion etc
std::string unknownenum="";
constexpr const char* gl_enum_to_char(const GLenum val) {
	switch (val) {
		case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
		case GL_FLOAT_MAT4: return "GL_FLOAT_MAT4";
		case GL_FLOAT_VEC4: return "GL_FLOAT_VEC4";
		case GL_FLOAT_VEC3: return "GL_FLOAT_VEC3";
		default: {
			unknownenum = std::to_string(val);
			return unknownenum.c_str();
		}
	}
	return nullptr;
}

void CHECKGLERR(void) {
#ifdef SHOWGLERRS
	GLuint glerr = glGetError();
	switch (glerr) {
		//snprintf(lastglerrstr, 1, "");
	case GL_OUT_OF_MEMORY: snprintf(lastglerrstr, glerrstrcharcnt, "%s\n", gl_enum_to_char(GL_OUT_OF_MEMORY)); break;
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