#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class PrimitiveMesh
{
public:
	GLuint vao, vbo, ebo;
	
	PrimitiveMesh() 
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
	}

	void BindVAO() const
	{
		glBindVertexArray(vao);
	}

	void BindVBO() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}

	void BindEBO() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}

	void Link(float* data, unsigned int* indices, size_t dataSize, size_t indicesSize) const
	{
		GLint bufferSize;

		BindVAO();
		BindVBO();
		glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(float), data, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
		std::cout << "Size of the VBO data uploaded: " << bufferSize << " bytes" << std::endl;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		BindEBO();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
		std::cout << "Size of the EBO data uploaded: " << bufferSize << " bytes" << std::endl;
	}

	void Delete() const
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
};

