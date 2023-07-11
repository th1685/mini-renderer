#pragma once

// #define TEXTUREWIDTH 128
// #define TEXTUREHEIGHT 128

struct rgb {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

typedef struct _texture {
	struct rgb* data;
	int width;
	int height;
} TEXTURE;

//PROTOTYPES-------------------------------------
TEXTURE texture_load(char* filename);
struct rgb texture_sample(TEXTURE* texture, float u, float v);
void texture_delete(TEXTURE* texture);

//DEFINITIONS------------------------------------
TEXTURE texture_load(char* filename) {
	TEXTURE buffer;
	//open file
	FILE* fp = fopen(filename, "rb"); if (!fp) {TEXTURE null; return null;}

	//get file size
	int texturewidth, textureheight;
	fscanf(fp, "P6\n%d %d", &texturewidth, &textureheight);
	buffer.width = texturewidth; buffer.height = textureheight;
	
	int headersize = 16; //header: P6\n128 128\n255\n (file type, width, height, max colour depth - 16 bytes)
    fseek(fp, 0L, SEEK_END);
    long int filesize = ftell(fp) - headersize;

	fseek(fp, headersize-1, SEEK_SET); //start from after header

	//allocate memory
	struct rgb* col_data = (struct rgb*)malloc((size_t)filesize);
	
	//read
    fread(col_data, sizeof(struct rgb), filesize/sizeof(struct rgb), fp);

	buffer.data = col_data;
    
    fclose(fp);
	return buffer;
}

struct rgb texture_sample(TEXTURE* texture, float u, float v) {
	int x = u*(float)texture->width; int y = v*(float)texture->height;
	return texture->data[y*texture->width + x];
}

void texture_delete(TEXTURE* texture) {
	free(texture->data);
}