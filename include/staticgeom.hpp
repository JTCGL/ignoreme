#ifndef _STATICGEOM_H_
#define _STATICGEOM_H_

#include "platform.hpp"
#include "geosphere.h"

GLuint vaoID = 0, vboID = 0, iboID = 0, nboID = 0;
GLint vpos_location = 0, vnorm_location = 0;

void genLitGeosphere(const GLint attrib_vpos_location, const GLint attrib_vnorm_location) {
	vpos_location = attrib_vpos_location;
	vnorm_location = attrib_vnorm_location;

	printf("attr 1 loc %d, attr 2 loc %d\n", attrib_vpos_location, attrib_vnorm_location);

	GLuint glerr = GL_NO_ERROR;
	glerr = glGetError();
	if (glerr != GL_NO_ERROR) {
		//std::cout << "genLitGeosphere PRE ERROR! " << glerr << " \n";
	}

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(GeoSphere001_vertPos)), GeoSphere001_vertPos, GL_STATIC_DRAW);
	//std::cout << "sizeof cube verts " << sizeof(Object001_vertPos) << " bytes\n";

	glEnableVertexAttribArray(vpos_location);
	glerr = glGetError();
	if (glerr != GL_NO_ERROR) {
		//std::cout << "genLitGeosphere::glEnableVertexAttribArray ERROR! " << glerr << " \n";
	}

	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glerr = glGetError();
	if (glerr != GL_NO_ERROR) {
		//std::cout << "genLitGeosphere::glVertexAttribPointer ERROR! " << glerr << " \n";
	}

	glGenBuffers(1, &nboID);
	glBindBuffer(GL_ARRAY_BUFFER, nboID);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(GeoSphere001_vertNormal)), GeoSphere001_vertNormal, GL_STATIC_DRAW);

	glEnableVertexAttribArray(vnorm_location);
	glVertexAttribPointer(vnorm_location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//bind index buffer and fill it in
	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GeoSphere001_TRIINDEXCOUNT * sizeof(unsigned short)), GeoSphere001_triIndices, GL_STATIC_DRAW);

	glBindVertexArray(0); // Disable Vertex Array Object
	glEnableVertexAttribArray(0); // Disable Vertex Attribute

	glerr = glGetError();
	if (glerr != GL_NO_ERROR) {
		//std::cout << "genLitGeosphere error reset " << glerr << " \n";
	}
}

void delLitGeosphere() {
	glDeleteVertexArrays(1, &vaoID);
}

void drawLitGeosphere(const GLuint program) {
	glUseProgram(program);
	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, GeoSphere001_TRIINDEXCOUNT, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);

}

#endif
