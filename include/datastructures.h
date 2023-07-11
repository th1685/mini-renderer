#pragma once

//STRUCTURES-----------------------------------------------
struct keystate {
	bool depressed;
	bool held;
	bool released;
};

struct vec2 {
	float u;
	float v;
	float w;
};

struct vec3 {
	float x;
	float y;
	float z;
	float w;
};

struct matrix4 {
	float element[4][4];
};

struct triangle {
	struct vec3 p[3];
	struct vec2 t[3];
	CHAR_INFO info;
	TEXTURE* texture;
};

struct entity {
	struct dynarr_triangle* mesh;
	struct vec3 worldcoords;
	struct vec3 velocity;
	struct matrix4 position;
	float scale;
	long id;
	long type;
	TEXTURE* texture;
};

struct scene {
	struct dynarr_entity* entlist;
};

// struct rgb {
// 	unsigned char r;
// 	unsigned char g;
// 	unsigned char b;
// };