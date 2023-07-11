#pragma once
#include <stdlib.h>
#include <string.h>
#include "datastructures.h"
#define VECDEFAULTCAPACITY 8

//TRIANGLE-------------------------------------------------
typedef struct dynarr_triangle {
	struct triangle* tris;
	int size;
	int capacity;
} dynarr_triangle;

void dynarr_init_triangle(dynarr_triangle* dynarr) {
	dynarr->capacity = VECDEFAULTCAPACITY;
	dynarr->tris = (struct triangle*)malloc(dynarr->capacity * sizeof(struct triangle));
	struct triangle temp = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < dynarr->capacity; i++) {
		dynarr->tris[i] = temp;
	}
	dynarr->size = 0;
}

void dynarr_push_back_triangle(dynarr_triangle* dynarr, struct triangle a) {
	if (dynarr->size >= dynarr->capacity) {
		dynarr->capacity *= 2;
		dynarr->tris = realloc(dynarr->tris, sizeof(struct triangle) * dynarr->capacity);
	}

	dynarr->tris[dynarr->size] = a;
	dynarr->size++;
}

void dynarr_pop_triangle(dynarr_triangle* dynarr) {
	struct triangle temp = { 0, 0, 0 };
	dynarr->tris[dynarr->size - 1] = temp;
	dynarr->size--;
}

void dynarr_shunt_triangle(dynarr_triangle* dynarr) {
	struct triangle temp = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < dynarr->size - 1; i++) {
		dynarr->tris[i] = dynarr->tris[i + 1];
	}
	dynarr->tris[dynarr->size - 1] = temp;
	dynarr->size--;
}

void dynarr_delete_triangle(dynarr_triangle* dynarr) {
	free(dynarr->tris);
}

//ENTITY---------------------------------------------------
typedef struct dynarr_entity {
	struct entity* entities;
	int size;
	int capacity;
} dynarr_entity;

void dynarr_init_entity(dynarr_entity* dynarr) {
	dynarr->capacity = VECDEFAULTCAPACITY;
	dynarr->entities = (struct entity*)malloc(dynarr->capacity * sizeof(struct entity));
	struct entity temp = { NULL, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	for (int i = 0; i < dynarr->capacity; i++) {
		dynarr->entities[i] = temp;
	}
	dynarr->size = 0;
}

void dynarr_push_back_entity(dynarr_entity* dynarr, struct entity a) {
	if (dynarr->size >= dynarr->capacity) {
		dynarr->capacity *= 2;
		dynarr->entities = realloc(dynarr->entities, sizeof(struct entity) * dynarr->capacity);
	}

	dynarr->entities[dynarr->size] = a;
	dynarr->size++;
}

//void vector_pop(vector* vector) {
//
//}

void dynarr_delete_entity(dynarr_entity* dynarr) {
	free(dynarr);
}

//VEC3-----------------------------------------------------
typedef struct dynarr_vec3 {
	struct vec3* contents;
	int size;
	int capacity;
} dynarr_vec3;

void dynarr_init_vec3(dynarr_vec3* dynarr) {
	dynarr->capacity = VECDEFAULTCAPACITY;
	dynarr->contents = (struct vec3*)malloc(dynarr->capacity * sizeof(struct vec3));
	struct vec3 temp = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < dynarr->capacity; i++) {
		dynarr->contents[i] = temp;
	}
	dynarr->size = 0;
}

void dynarr_push_back_vec3(dynarr_vec3* dynarr, struct vec3 a) {
	if (dynarr->size >= dynarr->capacity) {
		dynarr->capacity *= 2;
		dynarr->contents = realloc(dynarr->contents, sizeof(struct vec3) * dynarr->capacity);
	}

	dynarr->contents[dynarr->size] = a;
	dynarr->size++;
}

void dynarr_pop_vec3(dynarr_vec3* dynarr) {
	struct vec3 temp = { 0.0f, 0.0f, 0.0f, 0.0f };
	dynarr->contents[dynarr->size - 1] = temp;
	dynarr->size--;
}

void dynarr_shunt_vec3(dynarr_vec3* dynarr) {
	struct vec3 temp = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < dynarr->size - 1; i++) {
		dynarr->contents[i] = dynarr->contents[i + 1];
	}
	dynarr->contents[dynarr->size - 1] = temp;
	dynarr->size--;
}

void dynarr_delete_vec3(dynarr_vec3* dynarr) {
	free(dynarr->contents);
}

//VEC2-----------------------------------------------------
typedef struct dynarr_vec2 {
	struct vec2* contents;
	int size;
	int capacity;
} dynarr_vec2;

void dynarr_init_vec2(dynarr_vec2* dynarr) {
	dynarr->capacity = VECDEFAULTCAPACITY;
	dynarr->contents = (struct vec2*)malloc(dynarr->capacity * sizeof(struct vec2));
	struct vec2 temp = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < dynarr->capacity; i++) {
		dynarr->contents[i] = temp;
	}
	dynarr->size = 0;
}

void dynarr_push_back_vec2(dynarr_vec2* dynarr, struct vec2 a) {
	if (dynarr->size >= dynarr->capacity) {
		dynarr->capacity *= 2;
		dynarr->contents = realloc(dynarr->contents, sizeof(struct vec2) * dynarr->capacity);
	}

	dynarr->contents[dynarr->size] = a;
	dynarr->size++;
}

void dynarr_pop_vec2(dynarr_vec2* dynarr) {
	struct vec2 temp = { 0.0f, 0.0f, 0.0f };
	dynarr->contents[dynarr->size - 1] = temp;
	dynarr->size--;
}

void dynarr_shunt_vec2(dynarr_vec2* dynarr) {
	struct vec2 temp = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < dynarr->size - 1; i++) {
		dynarr->contents[i] = dynarr->contents[i + 1];
	}
	dynarr->contents[dynarr->size - 1] = temp;
	dynarr->size--;
}

void dynarr_delete_vec2(dynarr_vec2* dynarr) {
	free(dynarr->contents);
}