#include <ncurses.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "texture.h"

enum characters {
	CHAR_FILLEDBLOCK = 0x2588,
	CHAR_THREEQUARTER = 0x2593,
	CHAR_HALFBLOCK = 0x2592,
	CHAR_ONEQUARTER = 0x2591
};

enum colours {
	COLOUR_RED = 1,
	COLOUR_GREEN = 2,
	COLOUR_BLUE = 3,
	COLOUR_CYAN = 4,
	COLOUR_MAGENTA = 5,
	COLOUR_YELLOW = 6,
	COLOUR_WHITE = 7,
	COLOUR_BLACK = 8
};

typedef struct _panel {
	WINDOW* window;
	int width;
	int height;
} PANEL;

typedef struct _char_info {
	unsigned char character;
	struct rgb colour;
} CHAR_INFO;


//PROTOTYPES-------------------------------------
int setup(PANEL* panelptr);
void fillscreen(PANEL* panelptr, short c, int v);
void drawpoint(PANEL* panelptr, int x, int y, short c, int v);
void drawline(PANEL* panelptr, int x1, int y1, int x2, int y2, short c, int v);
void drawtriangle(PANEL* panelptr, int x1, int y1, int x2, int y2, int x3, int y3, short c, int v);
void filltriangle(PANEL* panelptr, int x1, int y1, int x2, int y2, int x3, int y3, short c, int v);
void texturetriangle(PANEL* panelptr, int x1, int y1, float u1, float v1, float w1,
					 int x2, int y2, float u2, float v2, float w2,
					 int x3, int y3, float u3, float v3, float w3,
					 short c, TEXTURE* texture, float* depthbuffer);
int rgb_sample(struct rgb* col);
void delay(int mill);
CHAR_INFO console_greyscale(float k);

//DEFINITIONS------------------------------------
int setup(PANEL* panelptr) {
	struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    panelptr->height = w.ws_row * 2; //x2 because font aspect ratio is 2
	panelptr->width = w.ws_col;

	panelptr->window = newwin(w.ws_row, w.ws_col, 0, 0);

	if(!has_colors()) return 1;
	if(panelptr->window == NULL) return 2;

	curs_set(0); //hide cursor
	noecho();

	start_color();
	use_default_colors();
	init_pair(COLOUR_RED, COLOR_RED, -1);
	init_pair(COLOUR_GREEN, COLOR_GREEN, -1);
	init_pair(COLOUR_BLUE, COLOR_BLUE, -1);
	init_pair(COLOUR_CYAN, COLOR_CYAN, -1);
	init_pair(COLOUR_MAGENTA, COLOR_MAGENTA, -1);
	init_pair(COLOUR_YELLOW, COLOR_YELLOW, -1);
	init_pair(COLOUR_WHITE, COLOR_WHITE, -1);
	init_pair(COLOUR_BLACK, COLOR_BLACK, -1);

	return 0;
}

void fillscreen(PANEL* panelptr, short c, int v) {
	for(int i=0; i<=panelptr->height; i++){
		for(int j=0; j<=panelptr->width; j++){
			mvwprintw(panelptr->window, i, j, "%c", c);
		}
	}
}

void drawpoint(PANEL* panelptr, int x, int y, short c, int v) {
	y=y>>1; //2 because font aspect ratio is 2
    if((x<0||x>panelptr->width-1)||(y<0||y>panelptr->height-1)) return;
    
	//set colour to v
	wattron(panelptr->window, COLOR_PAIR(v));
	mvwprintw(panelptr->window, y, x, "%c", c);
	wattroff(panelptr->window, COLOR_PAIR(v));
}

void drawline(PANEL* panelptr, int x1, int y1, int x2, int y2, short c, int v) {
	int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
	int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	while (x1 != x2 || y1 != y2) {
		drawpoint(panelptr, x1, y1, c, v);
		e2 = err;
		if (e2 > -dx) { err -= dy; x1 += sx; }
		if (e2 < dy) { err += dx; y1 += sy; }
	}
}

void drawtriangle(PANEL* panelptr, int x1, int y1, int x2, int y2, int x3, int y3, short c, int v) {
	drawline(panelptr, x1, y1, x2, y2, c, v);
	drawline(panelptr, x2, y2, x3, y3, c, v);
	drawline(panelptr, x3, y3, x1, y1, c, v);
}

void filltriangle(PANEL* panelptr, int x1, int y1, int x2, int y2, int x3, int y3, short c, int v) {
	//sort points in height order - opposite order than normal: y1 is top point on screen, has lowest y value, y2 is middle and y3 is bottom
	if (y1 > y2) {
		int temp = y2; //swap coordinates
		y2 = y1;
		y1 = temp;
		temp = x2;
		x2 = x1;
		x1 = temp;
	}
	if (y1 > y3) {
		int temp = y3;
		y3 = y1;
		y1 = temp;
		temp = x3;
		x3 = x1;
		x1 = temp;
	}
	if (y2 > y3) {
		int temp = y3;
		y3 = y2;
		y2 = temp;
		temp = x3;
		x3 = x2;
		x2 = temp;
	}

	//calculate gradients
	int dy1 = y2 - y1;
	int dx1 = x2 - x1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;

	float dax_step = 0, dbx_step = 0;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);


	if (dy1) { //flat bottom
		for (int i = y1; i <= y2; i++) {
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			if (ax > bx) {
				int temp = bx;
				bx = ax;
				ax = temp;
			}

			drawline(panelptr, ax, i, bx, i, c, v);
		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);


	if (dy1) { //flat top
		for (int i = y2; i <= y3; i++) {
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			if (ax > bx) {
				int temp = bx;
				bx = ax;
				ax = temp;
			}

			drawline(panelptr, ax, i, bx, i, c, v);
		}
	}
}

void texturetriangle(PANEL* panelptr, int x1, int y1, float u1, float v1, float w1,
					 int x2, int y2, float u2, float v2, float w2,
					 int x3, int y3, float u3, float v3, float w3,
					 short c, TEXTURE* texture, float* depthbuffer) {
	//sort points in height order - opposite order than normal: y1 is top point on screen, has lowest y value, y2 is middle and y3 is bottom
	if (y1 > y2) {
		int temp = y2; //swap coordinates
		y2 = y1;
		y1 = temp;
		temp = x2;
		x2 = x1;
		x1 = temp;

		float tempf = u2;
		u2 = u1;
		u1 = tempf;
		tempf = v2;
		v2 = v1;
		v1 = tempf;
		tempf = w2;
		w2 = w1;
		w1 = tempf;
	}
	if (y1 > y3) {
		int temp = y3;
		y3 = y1;
		y1 = temp;
		temp = x3;
		x3 = x1;
		x1 = temp;

		float tempf = u3;
		u3 = u1;
		u1 = tempf;
		tempf = v3;
		v3 = v1;
		v1 = tempf;
		tempf = w3;
		w3 = w1;
		w1 = tempf;
	}
	if (y2 > y3) {
		int temp = y3;
		y3 = y2;
		y2 = temp;
		temp = x3;
		x3 = x2;
		x2 = temp;

		float tempf = u3;
		u3 = u2;
		u2 = tempf;
		tempf = v3;
		v3 = v2;
		v2 = tempf;
		tempf = w3;
		w3 = w2;
		w2 = tempf;
	}

	//calculate gradients
	int dy1 = y2 - y1; float du1 = u2 - u1; float dw1 = w2 - w1;
	int dx1 = x2 - x1; float dv1 = v2 - v1;

	int dy2 = y3 - y1; float du2 = u3 - u1; float dw2 = w3 - w1;
	int dx2 = x3 - x1; float dv2 = v3 - v1;

	float tex_u = 0.0f, tex_v = 0.0f, tex_w = 0.0f;

	float dax_step = 0.0f, dbx_step = 0.0f,
		  du1_step = 0.0f, dv1_step = 0.0f, dw1_step = 0.0f,
		  du2_step = 0.0f, dv2_step = 0.0f, dw2_step = 0.0f;

	if (dy1) dax_step = dx1 / (float)abs(dy1); if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1); if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy2) dbx_step = dx2 / (float)abs(dy2); if (dy2) du2_step = du2 / (float)abs(dy2);
	if (dy2) dv2_step = dv2 / (float)abs(dy2); if (dy2) dw2_step = dw2 / (float)abs(dy2);


	if (dy1) { //flat bottom
		for (int i = y1; i <= y2; i++) {
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u1 + (float)(i - y1) * du1_step; float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step; float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step; float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx) {
				int temp = bx;
				bx = ax;
				ax = temp;

				float tempf = tex_eu;
				tex_eu = tex_su;
				tex_su = tempf;

				tempf = tex_ev;
				tex_ev = tex_sv;
				tex_sv = tempf;

				tempf = tex_ew;
				tex_ew = tex_sw;
				tex_sw = tempf;
			}

			tex_u = tex_su; tex_v = tex_sv; tex_w = tex_sw;
			float tstep = 1.0f/(float)(bx - ax), t = 0;

			for(int j=ax; j<bx; j++){
				tex_u = (1.0f-t)*tex_su+t*tex_eu;
				tex_v = (1.0f-t)*tex_sv+t*tex_ev;
				tex_w = (1.0f-t)*tex_sw+t*tex_ew;

				if(tex_w>depthbuffer[i*panelptr->width + j]) {
					struct rgb col = texture_sample(texture, tex_u/tex_w, tex_v/tex_w);
					int v = rgb_sample(&col);
					drawpoint(panelptr, j, i, c, v);
					depthbuffer[i*panelptr->width + j] = tex_w;
				}

				t+=tstep;
			}
		}
	}

	dy1 = y3 - y2; du1 = u3 - u2; dw1 = w3 - w2;
	dx1 = x3 - x2; dv1 = v3 - v2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	du1_step = 0; dv1_step = 0; dw1_step = 0;
	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy1) { //flat top
		for (int i = y2; i <= y3; i++) {
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u2 + (float)(i - y2) * du1_step; float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step; float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step; float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx) {
				int temp = bx;
				bx = ax;
				ax = temp;

				float tempf = tex_eu;
				tex_eu = tex_su;
				tex_su = tempf;

				tempf = tex_ev;
				tex_ev = tex_sv;
				tex_sv = tempf;

				tempf = tex_ew;
				tex_ew = tex_sw;
				tex_sw = tempf;
			}

			tex_u = tex_su; tex_v = tex_sv; tex_w = tex_sw;
			float tstep = 1.0f/(float)(bx - ax), t = 0;

			for(int j=ax; j<bx; j++){
				tex_u = (1.0f-t)*tex_su+t*tex_eu;
				tex_v = (1.0f-t)*tex_sv+t*tex_ev;
				tex_w = (1.0f-t)*tex_sw+t*tex_ew;

				if(tex_w>depthbuffer[i*panelptr->width + j]) {
					struct rgb col = texture_sample(texture, tex_u/tex_w, tex_v/tex_w);
					int v = rgb_sample(&col);
					drawpoint(panelptr, j, i, c, v);
					depthbuffer[i*panelptr->width + j] = tex_w;
				}

				t+=tstep;
			}
		}
	}
}

int rgb_sample(struct rgb* col) {
    int r = (int)col->r;
    int g = (int)col->g;
    int b = (int)col->b;

    int simrg = abs(r-g);
    int simgb = abs(g-b);
    int simbr = abs(b-r);
    int threshold = 30;
    int out = 0;

    if(simrg<=threshold&&simgb<=threshold&&simbr<=threshold) {
        if(((r+g+b)/3)<128) out = COLOUR_BLACK;
        else out = COLOUR_WHITE;
    }
    else {
        if(simrg<=threshold) {
            if(((r+g)/2)>b) out = COLOUR_YELLOW; else out = COLOUR_BLUE;
        }
        else if(simgb<=threshold) {
            if(((g+b)/2)>r) out = COLOUR_CYAN; else out = COLOUR_RED;
        }
        else if(simbr<=threshold) {
            if(((b+r)/2)>g) out = COLOUR_MAGENTA; else out = COLOUR_GREEN;
        }
        else {
            //return largest r,g,b
            if(r>g) {
                if(r>b) out = COLOUR_RED; else out = COLOUR_BLUE;
            }
            else {
                if(g>b) out = COLOUR_GREEN; else out = COLOUR_BLUE;
            }
        }
    }
    return out;
}

void delay(int mill) {
	clock_t start_time = 1000 * clock() / CLOCKS_PER_SEC;
	while ((1000 * clock() / CLOCKS_PER_SEC) < start_time + mill);
}

CHAR_INFO console_greyscale(float k) {
	CHAR_INFO out; struct rgb white = {255, 255, 255};
	char shades[8] = {'.', '\'', '/', '<', '=', '+', '#', '@'}; //{'.', ':', '-', '=', '+', '*', '%', '#', '@'};
	int index = floor(k*8.0f);
	out.character = shades[index];
	out.colour = white;
	return out;
}
