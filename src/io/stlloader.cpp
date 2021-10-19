#include "stlloader.hpp"

float* ObjLoader::extractVertexArray(STL_BINARY_FILE_DATA* bfd) {
    printf("[INFO] Extracting vertice array...\n");
    //Get size in MB
    unsigned int size = (*bfd).n_triangles * sizeof(float) * 9;
    float MB = size / 1024.0f / 1024.0f;
    printf("[INFO] Memory needed is %fMB\n", MB);
    float* fA = (float*)malloc(size);
    if (fA == NULL) {
        printf("[CRITICAL] Out of memory!\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("[INFO] Allocated vertex memory successfully.\n");
    }
    //For each triangle (and all vertices), add each vertex in order to the final sequential array
    for (unsigned int i = 0; i < (bfd->n_triangles) * 9; i += 9) {
        STL_BINARY_TRIANGLE_DATA btd = (bfd->triangles[i / 9]);
        fA[i]     = btd.vertex1.x;
        fA[i + 1] = btd.vertex1.y;
        fA[i + 2] = btd.vertex1.z;
        fA[i + 3] = btd.vertex2.x;
        fA[i + 4] = btd.vertex2.y;
        fA[i + 5] = btd.vertex2.z;
        fA[i + 6] = btd.vertex3.x;
        fA[i + 7] = btd.vertex3.y;
        fA[i + 8] = btd.vertex3.z;
    }
    printf("[INFO] Done.\n");
    return fA;
}
vec3 ObjLoader::normalizeVector(vec3 in) {
    //Converts vector to a unit vector
    double d = (in.x * in.x) + (in.y * in.y) + (in.z * in.z);
    vec3 out = {
        in.x/d,
        in.y/d,
        in.z/d
    };
    return out;
}

void ObjLoader::loadNormals(STL_BINARY_FILE_DATA* bfd) {
    printf("[INFO] Loading normals...\n");
    //Get vertex array in a non-indexed, sequential order
    float* vertices = extractVertexArray(bfd);
    unsigned int n_tri = bfd->n_triangles;
    //Load face normals
    for (unsigned int i = 0; i < n_tri; i ++) {
        //Calculate face normal vector from triangle vertices
        vec3 V = bfd->triangles[i].vertex2 - bfd->triangles[i].vertex1;
        vec3 W = bfd->triangles[i].vertex3 - bfd->triangles[i].vertex1;
        double Nx = (V.y * W.z) - (V.z * W.y);
        double Ny = (V.z * W.x) - (V.x * W.z);
        double Nz = (V.x * W.y) - (V.y * W.x);
        vec3 A = normalizeVector(vec3(Nx, Ny, Nz));
        //Assign triangle normal at this index to the just calculated normal
        bfd->triangles[i].normal = A;
    }

    //Load vertex normals. Allocated the minimum amount of possible vertex normals in order to be most efficient.
    //This minimum would be if all triangles have the same points, which is unlikely but possible.
    std::unordered_map<vec3, vec3> vertex_normals;

    //For each vertex
    for (unsigned int i = 0; i < bfd->n_triangles * 3; i++) {
        STL_BINARY_TRIANGLE_DATA btd = bfd->triangles[i / 3];
        vec3 V;
        switch (i % 3) {
        case 0:
            V = btd.vertex1;
            break;
        case 1:
            V = btd.vertex2;
            break;
        case 2:
            V = btd.vertex3;
            break;
        default: break;
        }
        auto it = vertex_normals.find(V);
        if (it == vertex_normals.end()) {
            vertex_normals[V] = vec3(0, 0, 0);
        }
        else {
            it->second += btd.normal;
        }
    }
    bfd->vertex_normals = vertex_normals;
}

STL_BINARY_FILE_DATA* ObjLoader::loadSTL(const char* path) {
    printf("[INFO] Loading STL...\n");
    STL_BINARY_FILE_DATA* fData = new STL_BINARY_FILE_DATA();
    FILE* f;
    if(f=fopen(path,"r")) {
        //pass
    } else {
        f=NULL;
    }
    long size;
    if (f) {
        printf("[INFO] STL successfully opened.\n");
        //Get size of STL file
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fseek(f, 0, SEEK_SET);
        printf("[INFO] STL has size of %ld bytes\n", size);
        unsigned char* buffer = (unsigned char*)malloc(size);
        if (buffer == NULL) {
            fprintf(stderr, "[CRITICAL] Out of memory!\n");
            exit(EXIT_FAILURE);
        }
        fread(buffer, size, 1, f);
        fclose(f);
        printf("[INFO] STL loaded into buffer and closed.\n");
        if (size < 1) {
            fprintf(stderr, "[CRITICAL] File IO error!\n");
            exit(EXIT_FAILURE);
        }
        //Extract file header, first 80 bytes
        for (int i = 0; i < 80; i++) {
            fData->header[i] = buffer[i];
        }
        //Convert header to string and print
        std::string header((char*)fData->header);
        printf("[INFO] STL has header: %s\n", header.c_str());
        
        //Extract number of triangle count
        unsigned long nTc = (unsigned int)buffer[83] << 24 |
            (unsigned int)buffer[82] << 16 |
            (unsigned int)buffer[81] << 8 |
            (unsigned int)buffer[80];
        printf("[INFO] Number of triangles is %i\n", nTc);
        fData->n_triangles = nTc;
        //Allocate memory for triangle struct array
        fData->triangles = (STL_BINARY_TRIANGLE_DATA *)malloc(nTc * sizeof(STL_BINARY_TRIANGLE_DATA));
        if (fData->triangles == NULL) {
            fprintf(stderr, "[CRITICAL] Out of memory!\n");
            exit(EXIT_FAILURE);
        }
        //For every triangle
        printf("[INFO] Decomposing buffer into component data...\n");
        for (int i = 84; i < size; i += 50) {
            STL_BINARY_TRIANGLE_DATA* tData = new STL_BINARY_TRIANGLE_DATA();
            //For every byte in triangle
            for (int j = 0; j < 48; j+=4) {
                //Convert hex to float
                unsigned int num = (buffer[i + j + 0]) | (buffer[i + j + 1] << 8) | (buffer[i + j + 2] << 16) | (buffer[i + j + 3] << 24);
                Converter c;
                float f = (float)c.convertIEEE754toFloat((unsigned long)num, 0);
                //Determine whether x, y or z data is being processed
                int index = (j % 12) / 4;
                //If j<12, we are processing the normal
                if (j < 12) {
                    switch (index) {
                    case 0:
                        tData->normal.x = f;
                        break;
                    case 1:
                        tData->normal.y = f;
                        break;
                    case 2:
                        tData->normal.z = f;
                        break;
                    default: break;
                    }
                }
                //If j<24 and >=12, we are processing the first vertex
                else if (j < 24) {
                    switch (index) {
                    case 0:
                        tData->vertex1.x = f;
                        break;
                    case 1:
                        tData->vertex1.y = f;
                        break;
                    case 2:
                        tData->vertex1.z = f;
                        break;
                    default: break;
                    }
                }
                //If j<36 and >=24, we are processing the second vertex
                else if (j < 36) {
                    switch (index) {
                    case 0:
                        tData->vertex2.x = f;
                        break;
                    case 1:
                        tData->vertex2.y = f;
                        break;
                    case 2:
                        tData->vertex2.z = f;
                        break;
                    default: break;
                    }
                }
                //Else, we are processing the third vertex
                else {
                    switch (index) {
                    case 0:
                        tData->vertex3.x = f;
                        break;
                    case 1:
                        tData->vertex3.y = f;
                        break;
                    case 2:
                        tData->vertex3.z = f;
                        break;
                    default: break;
                    }
                }
            }
            //Set triangle attribute to 0x00 (this value is not used at all, so it does not matter)
            tData->attr = 0x00;
            //Assign this triangle to file
            fData->triangles[(i - 84) / 50] = *tData;
        }
    }
    else {
        printf("[ERROR] File does not exist!\n");
    }
    printf("[INFO] Processing vertex normals...\n");
    printf("[INFO] Done.\n");
    return fData;
}