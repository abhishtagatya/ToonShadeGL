#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include <vector>
#include <cmath>

#include "primitive_model.h"

class Torus {
public:

    PrimitiveModel pm;

    Torus(float innerRadius, float outerRadius, int sides, int rings)
        : innerRadius(innerRadius), outerRadius(outerRadius), sides(sides), rings(rings), pm(nullptr, nullptr, 0, 0) {
        // Calculate total number of vertices and indices
        totalVertices = (sides + 1) * (rings + 1);
        totalIndices = sides * rings * 6; // 6 indices per quad

        // Allocate memory for vertices, normals, and indices
        vertices = new float[totalVertices * 3];
        normals = new float[totalVertices * 3];
        indices = new unsigned int[totalIndices];

        generateTorus();

        float* data = combinePositionAndNormal();

        pm = PrimitiveModel(data, indices, totalVertices * 6, totalIndices);
        delete[] data;
    }

    ~Torus() {
        delete[] vertices;
        delete[] normals;
        delete[] indices;
    }

    float* getVertices() const {
        return vertices;
    }

    float* getNormals() const {
        return normals;
    }

    unsigned int* getIndices() const {
        return indices;
    }

    int getTotalVertices() const {
        return totalVertices;
    }

    int getTotalIndices() const {
        return totalIndices;
    }

    // New function to combine position and normal into a single float array
    float* combinePositionAndNormal() const {
        float* combined = new float[totalVertices * 6]; // 3 for position + 3 for normal
        for (int i = 0; i < totalVertices; ++i) {
            combined[i * 6 + 0] = vertices[i * 3 + 0]; // Position x
            combined[i * 6 + 1] = vertices[i * 3 + 1]; // Position y
            combined[i * 6 + 2] = vertices[i * 3 + 2]; // Position z
            combined[i * 6 + 3] = normals[i * 3 + 0];  // Normal x
            combined[i * 6 + 4] = normals[i * 3 + 1];  // Normal y
            combined[i * 6 + 5] = normals[i * 3 + 2];  // Normal z
        }
        return combined;
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

private:
    float innerRadius;
    float outerRadius;
    int sides;
    int rings;
    int totalVertices;
    int totalIndices;
    float* vertices;
    float* normals;
    unsigned int* indices;

    void generateTorus() {
        int index = 0;
        for (int i = 0; i <= rings; ++i) {
            float theta = static_cast<float>(i) / rings * 2.0f * M_PI;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);

            for (int j = 0; j <= sides; ++j) {
                float phi = static_cast<float>(j) / sides * 2.0f * M_PI;
                float cosPhi = cos(phi);
                float sinPhi = sin(phi);

                // Calculate vertex positions
                float x = (outerRadius + innerRadius * cosPhi) * cosTheta;
                float y = (outerRadius + innerRadius * cosPhi) * sinTheta;
                float z = innerRadius * sinPhi;

                // Store vertex position
                vertices[index * 3 + 0] = x;
                vertices[index * 3 + 1] = y;
                vertices[index * 3 + 2] = z;

                // Store normals
                normals[index * 3 + 0] = cosPhi * cosTheta;
                normals[index * 3 + 1] = cosPhi * sinTheta;
                normals[index * 3 + 2] = sinPhi;

                index++;
            }
        }

        // Generate indices
        index = 0;
        for (int i = 0; i < rings; ++i) {
            for (int j = 0; j < sides; ++j) {
                indices[index++] = i * (sides + 1) + j;
                indices[index++] = (i + 1) * (sides + 1) + j;
                indices[index++] = (i + 1) * (sides + 1) + (j + 1);

                indices[index++] = i * (sides + 1) + j;
                indices[index++] = (i + 1) * (sides + 1) + (j + 1);
                indices[index++] = i * (sides + 1) + (j + 1);
            }
        }
    }
};