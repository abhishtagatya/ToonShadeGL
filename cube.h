// cube.h
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "primitive_model.h"

class Cube
{
private:
    // Cube data defined as private members
    int DATA_SIZE = 144;
    int INDICES_SIZE = 36;

    float CUBE_DATA[144] = {
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,

        // Right face
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

         // Top face
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int CUBE_INDICES[36] = {
        0,  1,  2,  2,  3,  0,  // Front face
        8,  9,  10, 10, 11, 8,  // Left face
        12, 13, 14, 14, 15, 12, // Right face
        20, 21, 22, 22, 23, 20,  // Top face
        4,  6,  5,  7,  6,  4,  // Back face
        16, 17, 18, 18, 19, 16, // Bottom face
    };
public:

    PrimitiveModel pm;

    Cube() : pm(CUBE_DATA, CUBE_INDICES, sizeof(CUBE_DATA) / sizeof(CUBE_DATA[0]), sizeof(CUBE_INDICES) / sizeof(CUBE_INDICES[0])) // Use member initializer list
    {
        // Cube constructor
    }

    void Draw(Shader& shader)
    {
        pm.Draw(shader); // Call the draw method of PrimitiveModel
    }

    void DrawBasic()
    {
        pm.DrawBasic(); // Call the draw method of PrimitiveModel
    }

    void Delete()
    {
        pm.Delete();
    }

};
