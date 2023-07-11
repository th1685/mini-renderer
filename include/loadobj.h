#pragma once
#include "dynarr.h"

//PROTOTYPES-------------------------------------
struct dynarr_triangle loadunitcube();
struct dynarr_triangle loadunitpyramid();
struct dynarr_triangle loadobj(char* filename);

//DEFINITIONS------------------------------------
struct dynarr_triangle loadunitcube() {
	struct dynarr_triangle cubetris;
	dynarr_init_triangle(&cubetris);
	struct triangle cubecoords[12] = {                         //vertex coordinates<--|    |-->texture coordinates
		{0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,     1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f},

		{1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f},

		{1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f},

		{0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f},

		{0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f},

		{1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,     0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,     1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f},
	};
	for (int i = 0; i < 12; i++)
		dynarr_push_back_triangle(&cubetris, cubecoords[i]);
	return cubetris;
}

struct dynarr_triangle loadunitpyramid() {
	struct dynarr_triangle pyramidtris;
	dynarr_init_triangle(&pyramidtris);
	struct triangle pyramidcoords[4] = {
		{0.0f, 0.0f, 0.0f, 1.0f,				1.0f, 0.0f, 0.0f, 1.0f,				 							0.5f, 0.0f, sqrtf(3.0f)/2.0f, 1.0f},

		{0.0f, 0.0f, 0.0f, 1.0f,				0.5f, sinf(acosf(sqrtf(3.0f)/4.0f)), sqrtf(3.0f)/4.0f, 1.0f, 	1.0f, 0.0f, 0.0f, 1.0f},

		{1.0f, 0.0f, 0.0f, 1.0f,				0.5f, sinf(acosf(sqrtf(3.0f)/4.0f)), sqrtf(3.0f)/4.0f, 1.0f,	0.5f, 0.0f, sqrtf(3.0f)/2.0f, 1.0f},

		{0.5f, 0.0f, sqrtf(3.0f)/2.0f, 1.0f,	0.5f, sinf(acosf(sqrtf(3.0f)/4.0f)), sqrtf(3.0f)/4.0f, 1.0f,	0.0f, 0.0f, 0.0f, 1.0f}
	};
	for (int i = 0; i < 4; i++)
		dynarr_push_back_triangle(&pyramidtris, pyramidcoords[i]);
	return pyramidtris;
}

struct dynarr_triangle loadobj(char* filename) {
	//works with obj files including only vertex and texture coordinates
	FILE* fp = fopen(filename, "r");
    if(fp==NULL) {printf("could not open file\n"); struct dynarr_triangle error; return error;}
    
    int linecount = 0;
    dynarr_vec3 veclist; dynarr_init_vec3(&veclist);
    dynarr_vec2 texlist; dynarr_init_vec2(&texlist);
    dynarr_triangle triflist; dynarr_init_triangle(&triflist);
    
    char header[128]; int read = fscanf(fp, "%s", header);
    
    while(read!=EOF) {
        struct vec3 vertex; struct vec2 tex; struct triangle trif;
        int vindx_1, vindx_2, vindx_3, tindx_1, tindx_2, tindx_3;
        
        if(strcmp(header, "v")==0) {
            fscanf(fp, "%f %f %f %f\n", &vertex.x, &vertex.y, &vertex.z, &vertex.w);
            dynarr_push_back_vec3(&veclist, vertex);
        }
        else if(strcmp(header, "vt")==0) {
            fscanf(fp, "%f %f %f\n", &tex.u, &tex.v, &tex.w);
            dynarr_push_back_vec2(&texlist, tex);
        }
        else if(strcmp(header, "f")==0) {
            fscanf(fp, "%d/%d %d/%d %d/%d\n", &vindx_1, &tindx_1, &vindx_2, &tindx_2, &vindx_3, &tindx_3);
            trif.p[0] = veclist.contents[vindx_1-1]; trif.p[1] = veclist.contents[vindx_2-1]; trif.p[2] = veclist.contents[vindx_3-1];
            trif.t[0] = texlist.contents[tindx_1-1]; trif.t[1] = texlist.contents[tindx_2-1]; trif.t[2] = texlist.contents[tindx_3-1];
            dynarr_push_back_triangle(&triflist, trif);
        }
        
        read = fscanf(fp, "%s", header);
    }
    
    fclose(fp);
    return triflist;
}