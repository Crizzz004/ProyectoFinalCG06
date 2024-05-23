/*
 * 
 *Basado en el código creado por
 *  Created on: 06/09/2016
 *      Author: reynaldo Martell
 */

#ifndef TOROIDE_A_
#define TOROIDE_A_

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "definition.h"

class Toroide {
public:
	Toroide(float ratio, int slices, int stacks);
	void init();
	void load();
	void render();
	virtual ~Toroide();
private:
	std::vector<VertexColor> vertexC;
	std::vector<GLuint> index;
	float ratio;
	int slices;
	int stacks;

	GLuint VAO, VBO, EBO;
};

#endif /* TOROIDE_A_ */
