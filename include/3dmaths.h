#pragma once

//PROTOTYPES-----------------------------------------------
struct matrix4 genprojectionmatrix(float nearplane, float farplane, float fov, float aspectratio, float fovrad);
struct matrix4 genrotationmatrixX(float theta, float bias);
struct matrix4 genrotationmatrixY(float theta, float bias);
struct matrix4 genrotationmatrixZ(float theta, float bias);
struct matrix4 gentranslation(struct vec3* coords);
struct matrix4 matrix4_multiply(struct matrix4* input1, struct matrix4* input2);
struct matrix4 matrix4_scale(struct matrix4* input, float scalar);
struct matrix4 matrix4_genpointat(struct vec3* position, struct vec3* target, struct vec3* up);
struct matrix4 matrix4_genlookat(struct matrix4* pointat);
struct vec3 vec3_multbymatrix4(struct vec3* vector, struct matrix4* matrix);
struct vec3 vec3_add(struct vec3* input1, struct vec3* input2);
struct vec3 vec3_subtract(struct vec3* input1, struct vec3* input2);
struct vec3 vec3_multiply(struct vec3* input1, struct vec3* input2);
struct vec3 vec3_divide(struct vec3* input1, struct vec3* input2);
struct vec3 vec3_scale(struct vec3* input, float scalar);
float vec3_dotproduct(struct vec3* input1, struct vec3* input2);
struct vec3 vec3_crossproduct(struct vec3* input1, struct vec3* input2);
float vec3_length(struct vec3* input);
struct vec3 vec3_normalise(struct vec3* input);
struct vec3 vec3_intersectplane(struct vec3* planepoint, struct vec3* planenormal, struct vec3* linestart, struct vec3* lineend, float* t);
float pointtoplane_dist(struct vec3* planepoint, struct vec3* planenormal, struct vec3* point);
int triangle_planeclip(struct vec3 planepoint, struct vec3 planenormal, struct triangle* input, struct triangle* output1, struct triangle* output2);

//DEFINITIONS----------------------------------------------
struct matrix4 genprojectionmatrix(float nearplane, float farplane,
	float fov, float aspectratio, float fovrad) {
	struct matrix4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.element[i][j] = 0.0f;
		}
	}
	matrix.element[0][0] = aspectratio * fovrad;
	matrix.element[1][1] = fovrad;
	matrix.element[2][2] = farplane / (farplane - nearplane);
	matrix.element[2][3] = 1.0f;
	matrix.element[3][2] = -(farplane * nearplane) / (farplane - nearplane);
	return matrix;
}

struct matrix4 genrotationmatrixX(float theta, float bias) {
	struct matrix4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.element[i][j] = 0.0f;
		}
	}
	matrix.element[0][0] = 1.0f;
	matrix.element[1][1] = cosf(theta * bias);
	matrix.element[1][2] = sinf(theta * bias);
	matrix.element[2][1] = -sinf(theta * bias);
	matrix.element[2][2] = cosf(theta * bias);
	matrix.element[3][3] = 1.0f;
	return matrix;
}

struct matrix4 genrotationmatrixY(float theta, float bias) {
	struct matrix4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.element[i][j] = 0.0f;
		}
	}
	matrix.element[0][0] = cosf(theta * bias);
	matrix.element[0][2] = sinf(theta * bias);
	matrix.element[2][0] = -sinf(theta * bias);
	matrix.element[1][1] = 1.0f;
	matrix.element[2][2] = cosf(theta * bias);
	matrix.element[3][3] = 1.0f;
	return matrix;
}

struct matrix4 genrotationmatrixZ(float theta, float bias) {
	struct matrix4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.element[i][j] = 0.0f;
		}
	}
	matrix.element[0][0] = cosf(theta * bias);
	matrix.element[0][1] = sinf(theta * bias);
	matrix.element[1][0] = -sinf(theta * bias);
	matrix.element[1][1] = cosf(theta * bias);
	matrix.element[2][2] = 1.0f;
	matrix.element[3][3] = 1.0f;
	return matrix;
}

struct matrix4 gentranslation(struct vec3* coords) {
	struct matrix4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.element[i][j] = 0.0f;
		}
	}
	matrix.element[0][0] = 1.0f;
	matrix.element[1][1] = 1.0f;
	matrix.element[2][2] = 1.0f;
	matrix.element[3][3] = 1.0f;
	matrix.element[3][0] = coords->x;
	matrix.element[3][1] = coords->y;
	matrix.element[3][2] = coords->z;
	return matrix;
}

struct matrix4 genidentity() {
	struct matrix4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.element[i][j] = 0.0f;
		}
		matrix.element[i][i] = 1.0f;
	}
	return matrix;
}

struct matrix4 genscalematrix(float scalar) {
	struct matrix4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.element[i][j] = 0.0f;
		}
		matrix.element[i][i] = scalar;
	}
	return matrix;
}

struct matrix4 matrix4_multiply(struct matrix4* input1, struct matrix4* input2) {
	struct matrix4 matrix;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			matrix.element[row][col] = input1->element[row][0] * input2->element[0][col] +
									   input1->element[row][1] * input2->element[1][col] +
									   input1->element[row][2] * input2->element[2][col] +
									   input1->element[row][3] * input2->element[3][col];
		}
	}
	return matrix;
}

struct matrix4 matrix4_scale(struct matrix4* input, float scalar) {
	struct matrix4 out;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			out.element[i][j] = input->element[i][j] * scalar;
		}
	}
	return out;
}

struct matrix4 matrix4_genpointat(struct vec3* position, struct vec3* target, struct vec3* up) {
	//matrix to make objects look at things
	//calculate forward direction
	struct vec3 forward = vec3_subtract(target, position);
	forward = vec3_normalise(&forward);

	//calculate object's up direction
	struct vec3 temp = vec3_scale(&forward, vec3_dotproduct(up, &forward)); //how much does the up vector project to the forward vector
	struct vec3 object_up = vec3_subtract(up, &temp); //adjust object's up vector based on the projection
	object_up = vec3_normalise(&object_up);

	//calculate object's right direction
	struct vec3 right = vec3_crossproduct(&forward, &object_up);
	right = vec3_normalise(&right);

	//construct point at matrix
	struct matrix4 matrix;
	matrix.element[0][0] = right.x;		matrix.element[0][1] = right.y;		matrix.element[0][2] = right.z;		matrix.element[0][3] = 0.0f;
	matrix.element[1][0] = object_up.x; matrix.element[1][1] = object_up.y; matrix.element[1][2] = object_up.z; matrix.element[1][3] = 0.0f;
	matrix.element[2][0] = forward.x;	matrix.element[2][1] = forward.y;	matrix.element[2][2] = forward.z;	matrix.element[2][3] = 0.0f;
	matrix.element[3][0] = position->x; matrix.element[3][1] = position->y; matrix.element[3][2] = position->z; matrix.element[3][3] = 1.0f;
	return matrix;
}

struct matrix4 matrix4_genlookat(struct matrix4* pointat) {
	//invert point at matrix to tranform world around camera
	struct matrix4 matrix;
	matrix.element[0][0] = pointat->element[0][0]; matrix.element[0][1] = pointat->element[1][0]; matrix.element[0][2] = pointat->element[2][0]; matrix.element[0][3] = 0.0f;
	matrix.element[1][0] = pointat->element[0][1]; matrix.element[1][1] = pointat->element[1][1]; matrix.element[1][2] = pointat->element[2][1]; matrix.element[1][3] = 0.0f;
	matrix.element[2][0] = pointat->element[0][2]; matrix.element[2][1] = pointat->element[1][2]; matrix.element[2][2] = pointat->element[2][2]; matrix.element[2][3] = 0.0f;
	matrix.element[3][0] = -(pointat->element[3][0] * matrix.element[0][0] + pointat->element[3][1] * matrix.element[1][0] + pointat->element[3][2] * matrix.element[2][0]);
	matrix.element[3][1] = -(pointat->element[3][0] * matrix.element[0][1] + pointat->element[3][1] * matrix.element[1][1] + pointat->element[3][2] * matrix.element[2][1]);
	matrix.element[3][2] = -(pointat->element[3][0] * matrix.element[0][2] + pointat->element[3][1] * matrix.element[1][2] + pointat->element[3][2] * matrix.element[2][2]);
	matrix.element[3][3] = 1.0f;
	return matrix;
}

struct vec3 vec3_multbymatrix4(struct vec3* vector, struct matrix4* matrix) {
	struct vec3 output;
	output.x = vector->x * matrix->element[0][0] + vector->y * matrix->element[1][0] + vector->z * matrix->element[2][0] + vector->w * matrix->element[3][0];
	output.y = vector->x * matrix->element[0][1] + vector->y * matrix->element[1][1] + vector->z * matrix->element[2][1] + vector->w * matrix->element[3][1];
	output.z = vector->x * matrix->element[0][2] + vector->y * matrix->element[1][2] + vector->z * matrix->element[2][2] + vector->w * matrix->element[3][2];
	output.w = vector->x * matrix->element[0][3] + vector->y * matrix->element[1][3] + vector->z * matrix->element[2][3] + vector->w * matrix->element[3][3];
	return output;
}

struct vec3 vec3_add(struct vec3* input1, struct vec3* input2) {
	struct vec3 out = { input1->x + input2->x, input1->y + input2->y, input1->z + input2->z };
	return out;
}

struct vec3 vec3_subtract(struct vec3* input1, struct vec3* input2) {
	struct vec3 out = { input1->x - input2->x, input1->y - input2->y, input1->z - input2->z };
	return out;
}

struct vec3 vec3_multiply(struct vec3* input1, struct vec3* input2) {
	struct vec3 out = { input1->x * input2->x, input1->y * input2->y, input1->z * input2->z };
	return out;
}

struct vec3 vec3_divide(struct vec3* input1, struct vec3* input2) {
	struct vec3 out = { input1->x / input2->x, input1->y / input2->y, input1->z / input2->z };
	return out;
}

struct vec3 vec3_scale(struct vec3* input, float scalar) {
	struct vec3 out = { input->x * scalar, input->y * scalar, input->z * scalar };
	return out;
}

float vec3_dotproduct(struct vec3* input1, struct vec3* input2) {
	float dp = input1->x * input2->x + input1->y * input2->y + input1->z * input2->z;
	return dp;
}

struct vec3 vec3_crossproduct(struct vec3* input1, struct vec3* input2) {
	struct vec3 out;
	out.x = input1->y * input2->z - input1->z * input2->y;
	out.y = input1->z * input2->x - input1->x * input2->z;
	out.z = input1->x * input2->y - input1->y * input2->x;
	return out;
}

float vec3_length(struct vec3* input) {
	float length = sqrtf(input->x * input->x + input->y * input->y + input->z * input->z);
	return length;
}

struct vec3 vec3_normalise(struct vec3* input) {
	float length = vec3_length(input);
	struct vec3 out = { input->x / length, input->y / length, input->z / length };
	return out;
}

struct vec3 vec3_intersectplane(struct vec3* planepoint, struct vec3* planenormal, struct vec3* linestart, struct vec3* lineend, float* t) {
	*planenormal = vec3_normalise(planenormal);
	float D = -vec3_dotproduct(planenormal, planepoint);
	float AD = vec3_dotproduct(linestart, planenormal);
	float BD = vec3_dotproduct(lineend, planenormal);
	*t = (-D - AD) / (BD - AD);
	struct vec3 line = vec3_subtract(lineend, linestart);
	struct vec3 linetointersect = vec3_scale(&line, *t);
	return vec3_add(linestart, &linetointersect);
}

float pointtoplane_dist(struct vec3* planepoint, struct vec3* planenormal, struct vec3* point) {
	struct vec3 temp = vec3_normalise(point);
	return (planenormal->x * point->x + planenormal->y * point->y + planenormal->z * point->z - vec3_dotproduct(planenormal, planepoint));
}

int triangle_planeclip(struct vec3 planepoint, struct vec3 planenormal, struct triangle* input, struct triangle* output1, struct triangle* output2) {
	//ensure plane normal is normalised
	planenormal = vec3_normalise(&planenormal);

	//temp storage to classify triangle inside or outside clipping plane
	struct vec3* insidepoints[3]; int insidecount = 0;
	struct vec3* outsidepoints[3]; int outsidecount = 0;
	//texture coords
	struct vec2* insidetex[3]; int insidetexcount = 0;
	struct vec2* outsidetex[3]; int outsidetexcount = 0;
	//number of triangles after clip (return value)
	int out = 0;

	//find shortest distance from point to plane
	float d0 = pointtoplane_dist(&planepoint, &planenormal, &input->p[0]);
	float d1 = pointtoplane_dist(&planepoint, &planenormal, &input->p[1]);
	float d2 = pointtoplane_dist(&planepoint, &planenormal, &input->p[2]);

	//separate the triangle's points into different storage
	if (d0 >= 0) { insidepoints[insidecount++] = &input->p[0]; insidetex[insidetexcount++] = &input->t[0]; }
	else { outsidepoints[outsidecount++] = &input->p[0]; outsidetex[outsidetexcount++] = &input->t[0]; }

	if (d1 >= 0) { insidepoints[insidecount++] = &input->p[1]; insidetex[insidetexcount++] = &input->t[1]; }
	else { outsidepoints[outsidecount++] = &input->p[1]; outsidetex[outsidetexcount++] = &input->t[1]; }

	if (d2 >= 0) { insidepoints[insidecount++] = &input->p[2]; insidetex[insidetexcount++] = &input->t[2]; }
	else { outsidepoints[outsidecount++] = &input->p[2]; outsidetex[outsidetexcount++] = &input->t[2]; }

	//classify triangle based on points inside or outside plane
	float t = 0.0f;
	switch (insidecount) {
	case 0:
		//no points are inside triangle
		out = 0;
		break;
	case 1:
		output1->info = input->info; output1->texture = input->texture;

		//one point is inside plane
		output1->p[0] = *insidepoints[0];
		output1->t[0] = *insidetex[0];

		//need 2 new points where the triangle's lines intersect the plane
		output1->p[1] = vec3_intersectplane(&planepoint, &planenormal, insidepoints[0], outsidepoints[0], &t);
		output1->t[1].u = t * (outsidetex[0]->u - insidetex[0]->u) + insidetex[0]->u; //generate clipped texture coords
		output1->t[1].v = t * (outsidetex[0]->v - insidetex[0]->v) + insidetex[0]->v;
		output1->t[1].w = t * (outsidetex[0]->w - insidetex[0]->w) + insidetex[0]->w;

		output1->p[2] = vec3_intersectplane(&planepoint, &planenormal, insidepoints[0], outsidepoints[1], &t);
		output1->t[2].u = t * (outsidetex[1]->u - insidetex[0]->u) + insidetex[0]->u; //generate clipped texture coords
		output1->t[2].v = t * (outsidetex[1]->v - insidetex[0]->v) + insidetex[0]->v;
		output1->t[2].w = t * (outsidetex[1]->w - insidetex[0]->w) + insidetex[0]->w;

		out = 1;
		break;
	case 2:
		output1->info = input->info; output1->texture = input->texture;
		output2->info = input->info; output2->texture = input->texture;
		
		//two points are inside the plane
		//need 2 new triangles from the quadrilateral
		output1->p[0] = *insidepoints[0]; output1->t[0] = *insidetex[0];
		output1->p[1] = *insidepoints[1]; output1->t[1] = *insidetex[1];
		output1->p[2] = vec3_intersectplane(&planepoint, &planenormal, insidepoints[0], outsidepoints[0], &t);
		output1->t[2].u = t * (outsidetex[0]->u - insidetex[0]->u) + insidetex[0]->u; //generate clipped texture coords
		output1->t[2].v = t * (outsidetex[0]->v - insidetex[0]->v) + insidetex[0]->v;
		output1->t[2].w = t * (outsidetex[0]->w - insidetex[0]->w) + insidetex[0]->w;

		output2->p[0] = *insidepoints[1]; output2->t[0] = *insidetex[1];
		output2->p[1] = output1->p[2]; output2->t[1] = output1->t[2];
		output2->p[2] = vec3_intersectplane(&planepoint, &planenormal, insidepoints[1], outsidepoints[0], &t);
		output2->t[2].u = t * (outsidetex[0]->u - insidetex[1]->u) + insidetex[1]->u; //generate clipped texture coords
		output2->t[2].v = t * (outsidetex[0]->v - insidetex[1]->v) + insidetex[1]->v;
		output2->t[2].w = t * (outsidetex[0]->w - insidetex[1]->w) + insidetex[1]->w;

		out = 2;
		break;
	case 3:
		output1->info = input->info;

		//all three points are inside plane
		//leave triangle as it is
		*output1 = *input;
		out = 1;
		break;
	}
	return out;
}
