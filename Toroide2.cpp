
/*
* Sphere.cpp
*Basado en el c�digo creado por
*  Created on: 06/09/2016
*      Author: reynaldo Martell
*/
#include "Toroide2.h"


Toroide2::Toroide2(float r,float R, int slices, int stacks) :
	r(r), R(R), slices(slices), stacks(stacks), VAO(0), VBO(0), EBO(0) {
}

Toroide2::~Toroide2() {

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

}



void Toroide2::init() {
	vertexC.resize(((slices + 1) * (stacks + 1)));
	index.resize((slices * stacks + slices) * 6);

	for (int i = 0; i <= stacks; ++i) {
		float V = i / (float)stacks;
		float phi = V * 2 * M_PI;

		for (int j = 0; j <= slices; ++j) {
			float U = j / (float)slices;
			float theta = U * M_PI * 2.0;

			float x = (R + r * cos(phi)) * cos(theta);
			float y = (R + r * cos(phi)) * sin(theta);
			float z = r * sin(phi);

			vertexC[i * (slices + 1) + j].position = glm::vec3(x, y, z);
			vertexC[i * (slices + 1) + j].color = glm::sphericalRand(1.0f);
		}
	}

	for (int i = 0; i < slices * stacks + slices; ++i) {
		index[i * 6] = i;
		index[i * 6 + 1] = i + slices + 1;
		index[i * 6 + 2] = i + slices;
		index[i * 6 + 3] = i + slices + 1;
		index[i * 6 + 4] = i;
		index[i * 6 + 5] = i + 1;
	}
}

void Toroide2::load() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	size_t stride;

	size_t offset1 = 0;
	size_t offset2 = 0;
	size_t offset3 = 0;

		glBufferData(GL_ARRAY_BUFFER, vertexC.size() * sizeof(glm::vec3) * 2,
			vertexC.data(),
			GL_STATIC_DRAW);
		stride = sizeof(vertexC[0]);
		offset1 = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
		index.data(),
		GL_STATIC_DRAW);

	// First attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset1);
	glEnableVertexAttribArray(0);
	// Second attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
		(GLvoid*)offset2);
	glEnableVertexAttribArray(1);
	// Thrid attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset3);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

}

void Toroide2::render() {

	glBindVertexArray(VAO);
	glDrawElements( GL_TRIANGLES, index.size(), GL_UNSIGNED_INT,
			(GLvoid*) (sizeof(GLuint) * 0));
	glBindVertexArray(0);

}
