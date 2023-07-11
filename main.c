#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "include/graphics.h"
#include "include/dynarr.h"
#include "include/datastructures.h"
#include "include/3dmaths.h"
#include "include/loadobj.h"

enum enitity_types {
    CUBE = 0,
    DSDS = 1
};

enum entity_ids {
    CB1 = 0,
    CB2 = 1,
    CB3 = 2
};

int main(void) {
	//SETUP--------------------------------------
    PANEL panel;
    initscr();
    switch(setup(&panel)) {
		case 1:
		endwin();
		printf("setup error: no color support\n");
		return -1;;
		break;

		case 2:
		endwin();
		printf("setup error: could not initialise window\n");
		return -1;
		break;

		default:
		break;
	}
    refresh();

	cbreak(); //asyncronous input
	nodelay(panel.window, TRUE);
	//notimeout(panel.window, TRUE);
	keypad(panel.window, TRUE); //keycodes

    ////TIMING-----------------------------------
	clock_t timepoint1 = clock();
	clock_t timepoint2 = clock();
	clock_t deltaTime = 0;

    //VARIABLES----------------------------------
	////DEPTH BUFFER-----------------------------
	float* depthbuffer = (float*)malloc((size_t)(panel.width*panel.height*sizeof(float)));

    ////SCENE------------------------------------
	dynarr_entity entlist1;
	dynarr_init_entity(&entlist1);
	struct scene scene1 = { &entlist1 };

	////ENTITIES---------------------------------
	//load mesh data
	dynarr_triangle cubetris = loadobj("assets/models/solids/cube.obj"); //loadunitcube();
	dynarr_triangle icotris = loadobj("assets/models/solids/icosahedron.obj");
	dynarr_triangle pyramidtris = loadobj("assets/models/solids/tetrahedron.obj"); //loadunitpyramid();

	//texture data
	TEXTURE texture1 = texture_load("assets/textures/texture1.ppm"),
			texture2 = texture_load("assets/textures/texture2.ppm"),
			texture3 = texture_load("assets/textures/texture3.ppm"),
			ico_tex = texture_load("assets/textures/ico_tex.ppm");

	//initialise cube entity
	struct entity cube1 = {
		NULL, //mesh
		{2.0f, 0.0f, 3.0f}, //coords
		{0, 0, 0} //vel
	};

	cube1.mesh = &cubetris;
	cube1.scale = 1.0f;
	cube1.id = CB1;
	cube1.type = CUBE;
	cube1.texture = &texture1;
	dynarr_push_back_entity(scene1.entlist, cube1); //push cube into scene

	struct entity cube3 = {
		NULL, //mesh
		{0.0f, 0.0f, 3.0f}, //coords
		{0, 0, 0} //vel
	};

	cube3.mesh = &icotris;
	cube3.scale = 1.5f;
	cube3.id = CB3;
	cube3.type = CUBE;
	cube3.texture = &ico_tex;
	dynarr_push_back_entity(scene1.entlist, cube3); //push cube into scene

	struct entity cube2 = {
		NULL, //mesh
		{-2.0f, 0.0f, 3.0f}, //coords
		{0, 0, 0} //vel
	};

	cube2.mesh = &pyramidtris;
	cube2.scale = 1.0f;
	cube2.id = CB2;
	cube2.type = CUBE;
	cube2.texture = &texture3;
	dynarr_push_back_entity(scene1.entlist, cube2); //push cube into scene

    ////PROJECTION VARIABLES---------------------
	float nearplane = 0.1f;
	float farplane = 1000.0f;
	float fov = 100.0f; //degrees
	float aspectratio = (float)panel.height / (float)panel.width;
	float fovrad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.141592654f); //convert to rads
	struct matrix4 projectionmatrix = genprojectionmatrix(nearplane, farplane, fov, aspectratio, fovrad);

	////CLIPPING PLANES--------------------------
	struct vec3 topclip = { 0.0f, (float)panel.height - 1.0f, 0.0f }, topclipnormal = { 0.0f, -1.0f, 0.0f };
	struct vec3 bottomclip = { 0.0f, 0.0f, 0.0f }, bottomclipnormal = { 0.0f, 1.0f, 0.0f };
	struct vec3 rightclip = { 0.0f, 0.0f, 0.0f }, rightclipnormal = { 1.0f, 0.0f, 0.0f };
	struct vec3 leftclip = { (float)panel.width - 1.0f, 0.0f, 0.0f }, leftclipnormal = { -1.0f, 0.0f, 0.0f };

	////SETTINGS---------------------------------
	bool wireframe = false;
	bool mousecapture = false;
    	char running = 1;

	////STANDARD ROTATION ANGLE------------------
	float theta = 0.0f;

	////CAMERA-----------------------------------
	struct vec3 camera = { 0.0f, 0.0f, 0.0f };
	struct vec3 lookdir = { 0.0f, 0.0f, 1.0f };
	struct vec3 up = { 0.0f, 1.0f, 0.0f };

	struct matrix4 cameramatrix;
	struct matrix4 viewmatrix;

	float lookmultiplier = 0.00005f;

	float yaw = 3.141f; float pitch = 3.141f;
	float minpitch = 1.6f, maxpitch = 4.7f;

	////MOVEMENT---------------------------------
	float movemultiplier = 0.0001f;

	////LIGHTING---------------------------------
	struct vec3 global_light_dir = { 0.0f, 0.0f, -1.0f };

    //LOOP---------------------------------------
    while(running){
        //TIMING---------------------------------
		if (deltaTime < 1) {
			timepoint1 = clock();
			delay(1);
			timepoint2 = clock();
			deltaTime = timepoint2 - timepoint1;
		}
		timepoint1 = clock();

        //INPUT----------------------------------
		struct vec3 forward = vec3_scale(&lookdir, movemultiplier * deltaTime);
		struct vec3 strafedir = vec3_crossproduct(&up, &forward); strafedir = vec3_normalise(&strafedir);
		struct vec3 strafe = vec3_scale(&strafedir, movemultiplier * deltaTime);

		int input_c = wgetch(panel.window);
		switch(input_c) {
			//settings
			case 'q':
			running = 0;
			break;

			case 'x':
			wireframe ^= true;
			break;

			//move
			case ' ':
			camera.y += movemultiplier * deltaTime;
			break;

			case 'c':
			camera.y -= movemultiplier * deltaTime;
			break;

			case 'd':
			camera = vec3_add(&camera, &strafe);
			break;

			case 'a':
			camera = vec3_subtract(&camera, &strafe);
			break;

			case 'w':
			camera = vec3_add(&camera, &forward);
			break;

			case 's':
			camera = vec3_subtract(&camera, &forward);
			break;

			//look
			case KEY_LEFT:
			yaw += lookmultiplier * deltaTime;
			break;

			case KEY_RIGHT:
			yaw -= lookmultiplier * deltaTime;
			break;

			case KEY_UP:
			pitch += lookmultiplier * deltaTime;
			break;

			case KEY_DOWN:
			pitch -= lookmultiplier * deltaTime;
			break;
		}

        //UPDATE---------------------------------
		struct matrix4 yawmatrix = genrotationmatrixY(yaw, 1.0f);
		struct matrix4 pitchmatrix = genrotationmatrixX(pitch, 1.0f);

		theta += 0.0000005f * deltaTime;

		struct matrix4 rotationX = genrotationmatrixX(theta, 1.0f),
			rotationY = genrotationmatrixY(theta, 1.0f),
			rotationZ = genrotationmatrixZ(theta, 0.5f);

		//camera update
		up.x = 0.0f, up.y = 1.0f, up.z = 0.0f;
		struct matrix4 camerarotation = matrix4_multiply(&pitchmatrix, &yawmatrix);
		struct vec3 target = { 0.0f, 0.0f, 1.0f };

		lookdir = vec3_multbymatrix4(&target, &camerarotation);
		target = vec3_add(&camera, &lookdir);

		cameramatrix = matrix4_genpointat(&camera, &target, &up);
		viewmatrix = matrix4_genlookat(&cameramatrix);

		////ENTITY TRANSFORMATIONS---------------
		for (int i = 0; i < scene1.entlist->size; i++) { //for all entities
			scene1.entlist->entities[i].position = genidentity(); //initialise position matrix

			switch (scene1.entlist->entities[i].id) { //check entity id and apply specific behaviour
			case CB1:
				//move
				scene1.entlist->entities[i].worldcoords.x = 1.5f * sinf(theta) + 2.0f;
				scene1.entlist->entities[i].worldcoords.y = 1.5f * cosf(theta) + 2.0f;
				//rotate
				scene1.entlist->entities[i].position = matrix4_multiply(&rotationZ, &rotationX);
				//scale
				//scene1.entlist->entities[i].scale = fabs(sinf(theta*2.0f + 5.0f));
				break;
			case CB2:
				//move
				scene1.entlist->entities[i].worldcoords.x = -1.5f * sinf(theta) - 2.0f;
				scene1.entlist->entities[i].worldcoords.y = -1.5f * cosf(theta) - 2.0f;
				//rotate
				scene1.entlist->entities[i].position = matrix4_multiply(&rotationZ, &rotationX);
				//scale
				//scene1.entlist->entities[i].scale = fabs(sinf(theta));
				break;
			case CB3:
				//rotate y axis
				scene1.entlist->entities[i].position = matrix4_multiply(&scene1.entlist->entities[i].position, &rotationY);
				//scale
				scene1.entlist->entities[i].scale = 5.0f*fabs(sinf(theta));
				break;
			default:
				break;
			}

			switch (scene1.entlist->entities[i].type) { //check entity type and apply specific behaviour
			case DSDS:
				scene1.entlist->entities[i].position = matrix4_multiply(&rotationZ, &rotationX);
				break;
			default:
				break;
			}

			//update the position matrix
			struct matrix4 temp;
			temp = gentranslation(&scene1.entlist->entities[i].worldcoords);
			scene1.entlist->entities[i].position = matrix4_multiply(&scene1.entlist->entities[i].position, &temp);
			//scene1.entlist->entities[i].position = matrix4_scale(&scene1.entlist->entities[i].position, scene1.entlist->entities[i].scale);
		}

        //RENDER---------------------------------
        fillscreen(&panel, ' ', COLOUR_BLACK); //clear screen
		memset(depthbuffer, 0, panel.width*panel.height*sizeof(float));

        dynarr_triangle clipped_triangles;
		dynarr_init_triangle(&clipped_triangles);

		//project triangles and clip in worldspace (near clip)
		for (int i = 0; i < scene1.entlist->size; i++) { //for all entities
			for (int j = 0; j < scene1.entlist->entities[i].mesh->size; j++) { //for all tris
				struct entity currentent = scene1.entlist->entities[i];
				struct triangle transformedtriangle, viewedtriangle, projectedtriangle;

				//apply position matrix to triangle
				transformedtriangle.p[0] = vec3_multbymatrix4(&currentent.mesh->tris[j].p[0], &currentent.position);
				transformedtriangle.p[1] = vec3_multbymatrix4(&currentent.mesh->tris[j].p[1], &currentent.position);
				transformedtriangle.p[2] = vec3_multbymatrix4(&currentent.mesh->tris[j].p[2], &currentent.position);
				//and texture coords
				transformedtriangle.t[0] = currentent.mesh->tris[j].t[0];
				transformedtriangle.t[1] = currentent.mesh->tris[j].t[1];
				transformedtriangle.t[2] = currentent.mesh->tris[j].t[2];

				//BACKFACE CULLING-------------------------
				//calculate face normal for backface culling
				struct vec3 normal, line0, line1;
				line0 = vec3_subtract(&transformedtriangle.p[1], &transformedtriangle.p[0]);
				line1 = vec3_subtract(&transformedtriangle.p[2], &transformedtriangle.p[0]);

				normal = vec3_crossproduct(&line0, &line1);
				normal = vec3_normalise(&normal); //normalise

				struct vec3 camray = vec3_subtract(&transformedtriangle.p[0], &camera); //line from camera to triangle

				if (vec3_dotproduct(&normal, &camray) < 0.0f) { //can the camera see the triangle? (backface culling)
					//flat shading
					global_light_dir = vec3_normalise(&global_light_dir); //normalise light vector
					float dotproduct = vec3_dotproduct(&global_light_dir, &normal);
					CHAR_INFO colour = console_greyscale(fabs(dotproduct));
					projectedtriangle.info = colour;

					//convert world space to view space (camera transformation)
					viewedtriangle.p[0] = vec3_multbymatrix4(&transformedtriangle.p[0], &viewmatrix);
					viewedtriangle.p[1] = vec3_multbymatrix4(&transformedtriangle.p[1], &viewmatrix);
					viewedtriangle.p[2] = vec3_multbymatrix4(&transformedtriangle.p[2], &viewmatrix);
					//texture coords
					viewedtriangle.t[0] = transformedtriangle.t[0];
					viewedtriangle.t[1] = transformedtriangle.t[1];
					viewedtriangle.t[2] = transformedtriangle.t[2];

					//clip triangles over near plane for maximum of 2 new triangles
					int clippedcount = 0;
					struct triangle clippedoutput[2];
					struct vec3 nearclip = { 0.0f, 0.0f, nearplane }, nearclipnormal = { 0.0f, 0.0f, 1.0f };
					clippedcount = triangle_planeclip(nearclip, nearclipnormal, &viewedtriangle, &clippedoutput[0], &clippedoutput[1]);

					for (int i = 0; i < clippedcount; i++) {
						for (int point = 0; point < 3; point++) {
							//project triangle
							projectedtriangle.p[point] = vec3_multbymatrix4(&clippedoutput[i].p[point], &projectionmatrix);
							projectedtriangle.t[point] = clippedoutput[i].t[point];
							//project coordinates
							projectedtriangle.t[point].u = projectedtriangle.t[point].u/projectedtriangle.p[point].w;
							projectedtriangle.t[point].v = projectedtriangle.t[point].v/projectedtriangle.p[point].w;
							projectedtriangle.t[point].w = 1.0f/projectedtriangle.p[point].w;

							//normalise projected triangle
							projectedtriangle.p[point] = vec3_scale(&projectedtriangle.p[point], 1.0f / projectedtriangle.p[point].w);

							//fix x and y inversions
							projectedtriangle.p[point].x *= -1.0f;
							projectedtriangle.p[point].y *= -1.0f;

							//transform triangle into screen
							projectedtriangle.p[point].x += 1.0f; projectedtriangle.p[point].y += 1.0f;
							projectedtriangle.p[point].x *= 0.5f * (float)panel.width; projectedtriangle.p[point].y *= 0.5f * (float)panel.height;
							
							//load texture information
							projectedtriangle.texture = currentent.texture;
						}
						dynarr_push_back_triangle(&clipped_triangles, projectedtriangle);
					}
				}
			}
		}

		//clip projected triangles in screenspace
		for (int i = 0; i < clipped_triangles.size; i++) {
			//clip against edge planes
			struct triangle clippedoutput[2];
			dynarr_triangle screenclipped_triangles; dynarr_init_triangle(&screenclipped_triangles);
			dynarr_push_back_triangle(&screenclipped_triangles, clipped_triangles.tris[i]);

			int newtriangles = 1;
			for (int i = 0; i < 4; i++) {
				int addtris = 0;
				while(newtriangles>0) {
					struct triangle temp = screenclipped_triangles.tris[0];
					dynarr_shunt_triangle(&screenclipped_triangles);
					newtriangles--;

					switch (i) {
					case 0:
						addtris = triangle_planeclip(topclip, topclipnormal, &temp, &clippedoutput[0], &clippedoutput[1]);
						break;
					case 1:
						addtris = triangle_planeclip(bottomclip, bottomclipnormal, &temp, &clippedoutput[0], &clippedoutput[1]);
						break;
					case 2:
						addtris = triangle_planeclip(rightclip, rightclipnormal, &temp, &clippedoutput[0], &clippedoutput[1]);
						break;
					case 3:
						addtris = triangle_planeclip(leftclip, leftclipnormal, &temp, &clippedoutput[0], &clippedoutput[1]);
						break;
					}
					for (int j = 0; j < addtris; j++)
						dynarr_push_back_triangle(&screenclipped_triangles, clippedoutput[j]);
				}
				newtriangles = screenclipped_triangles.size;
			}

			for (int i = 0; i < screenclipped_triangles.size; i++) {
				//draw triangle
				if (wireframe) {
					drawtriangle(&panel, screenclipped_triangles.tris[i].p[0].x, screenclipped_triangles.tris[i].p[0].y,
						screenclipped_triangles.tris[i].p[1].x, screenclipped_triangles.tris[i].p[1].y,
						screenclipped_triangles.tris[i].p[2].x, screenclipped_triangles.tris[i].p[2].y,
						'x', COLOUR_WHITE);
				}
				else {
					texturetriangle(&panel, screenclipped_triangles.tris[i].p[0].x, screenclipped_triangles.tris[i].p[0].y,
									screenclipped_triangles.tris[i].t[0].u, screenclipped_triangles.tris[i].t[0].v, screenclipped_triangles.tris[i].t[0].w,
									screenclipped_triangles.tris[i].p[1].x, screenclipped_triangles.tris[i].p[1].y,
									screenclipped_triangles.tris[i].t[1].u, screenclipped_triangles.tris[i].t[1].v, screenclipped_triangles.tris[i].t[1].w,
									screenclipped_triangles.tris[i].p[2].x, screenclipped_triangles.tris[i].p[2].y,
									screenclipped_triangles.tris[i].t[2].u, screenclipped_triangles.tris[i].t[2].v, screenclipped_triangles.tris[i].t[2].w,
									screenclipped_triangles.tris[i].info.character, screenclipped_triangles.tris[i].texture, depthbuffer);
				}
			}
			dynarr_delete_triangle(&screenclipped_triangles); //clear up
		}

		dynarr_delete_triangle(&clipped_triangles); //clear up

		float fps = CLOCKS_PER_SEC/deltaTime; //print fps
		mvwprintw(panel.window, 0, 0, "fps: %3.0f", fps);

        wrefresh(panel.window); //write to window

        //TIMING---------------------------------
		timepoint2 = clock();
		deltaTime = timepoint2 - timepoint1;
    }
    //CLEAN UP-----------------------------------
	free(depthbuffer);
	////FREE ASSETS------------------------------
	dynarr_delete_triangle(&cubetris);
	dynarr_delete_triangle(&icotris);
	dynarr_delete_triangle(&pyramidtris);

	texture_delete(&texture1);
	texture_delete(&texture2);
	texture_delete(&texture3);
	texture_delete(&ico_tex);

	////FREE GRAPHICS RESOURCES------------------
    endwin();

    return 0;
}
