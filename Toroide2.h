/*
 * 
 *Basado en el código creado por
 *  Created on: 06/09/2016
 *      Author: reynaldo Martell
 */

#ifndef TOROIDE_B_
#define TOROIDE_B_

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "definition.h"

class Toroide2 {
public:
	Toroide2(float r,float R, int slices, int stacks);
	void init();
	void load();
	void render();
	virtual ~Toroide2();
private:
	std::vector<VertexColor> vertexC;

	std::vector<GLuint> index;
	float r;
	float R;
	int slices;
	int stacks;

	GLuint VAO, VBO, EBO;
};

#endif /* TOROIDE_B_ */
