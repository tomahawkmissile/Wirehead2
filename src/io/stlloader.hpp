#ifndef STLLOADER_HPP
#define STLLOADER_HPP

#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include "../util/converter.hpp"

struct vec3 {
    float x,y,z;

    vec3(float x=0f, float y=0f, float z=0f) : x(x), y(y), z(z) {}

    vec3& operator=(const pos& a) {
        x=a.x;
        y=a.y;
        z=a.z;
        return *this;
    }

    vec3& operator-(const vec3& a) const {
        return vec3(a.x-x,a.y-y,a.z-z);
    }
};
struct STL_BINARY_TRIANGLE_DATA {
    vec3 normal;
    vec3 vertex1;
    vec3 vertex2;
    vec3 vertex3;
    unsigned short attr;
};
typedef struct VERTEX_NORMAL_DATA {
    vec3 vertex;
    vec3 normal;
};
typedef struct STL_BINARY_FILE_DATA {
    unsigned char header[80];
    unsigned long n_triangles;
    STL_BINARY_TRIANGLE_DATA* triangles;
    std::unordered_map<vec3, vec3> vertex_normals;
};

class ObjLoader {
public:
    float* extractVertexArray(STL_BINARY_FILE_DATA* bfd);
    vec3 normalizeVector(vec3 in);
    void loadNormals(STL_BINARY_FILE_DATA* bfd);
    STL_BINARY_FILE_DATA* loadSTL(const char* path);
};

#endif