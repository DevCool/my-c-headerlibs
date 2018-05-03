#ifndef PRS_BITMAP_H
#define PRS_BITMAP_H

#define MAX_SHADES 10

#include <stdio.h>
#include <stdlib.h>

typedef struct BITMAP_FILE_HEADER {
	unsigned short header;
	unsigned int size;
	unsigned short res1;
	unsigned short res2;
	unsigned int offset;
} BITMAP_HEADER;

typedef struct BITMAP_INFO_HEADER {
	unsigned int size;
	unsigned int width;
	unsigned int height;
	unsigned short col_planes;
	unsigned short bpp;
	unsigned int compression;
	unsigned int image_size;
	unsigned int h_res;
	unsigned int v_res;
	unsigned int num_cols;
	unsigned int num_imp;
} BITMAP_INFO;

typedef struct BITMAP_FILE {
	BITMAP_HEADER header;
	BITMAP_INFO info;
	unsigned char *data;
} BITMAP_FILE;

void destroy_BMP(BITMAP_FILE *bmp)
{
	if (bmp) {
		if (bmp->data)
			free(bmp->data);
		free(bmp);
	}
}

BITMAP_FILE *load_BMP(const char *filename)
{
	BITMAP_FILE *bmp;
	FILE *fp;

	if ((fp = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "Error opening BMP file.\n");
		return NULL;
	}
	bmp = (BITMAP_FILE*)malloc(sizeof(BITMAP_FILE));
	if (bmp) {
		/* BMP HEADER */
		fread(&bmp->header.header, 2, 1, fp);
		fread(&bmp->header.size, 4, 1, fp);
		fread(&bmp->header.res1, 2, 1, fp);
		fread(&bmp->header.res2, 2, 1, fp);
		fread(&bmp->header.offset, 4, 1, fp);

		/* BMP HEADER INFO */
		fread(&bmp->info.size, 4, 1, fp);
		fread(&bmp->info.width, 4, 1, fp);
		fread(&bmp->info.height, 4, 1, fp);
		fread(&bmp->info.col_planes, 2, 1, fp);
		fread(&bmp->info.bpp, 2, 1, fp);
		fread(&bmp->info.compression, 4, 1, fp);
		fread(&bmp->info.image_size, 4, 1, fp);
		fread(&bmp->info.h_res, 4, 1, fp);
		fread(&bmp->info.v_res, 4, 1, fp);
		fread(&bmp->info.num_cols, 4, 1, fp);
		fread(&bmp->info.num_imp, 4, 1, fp);

		if (bmp->header.header == 0x4D42) {
			fseek(fp, bmp->header.offset, SEEK_SET);
			bmp->data = malloc(sizeof(unsigned char)*
					bmp->header.size);
			if (bmp->data) {
				fread(bmp->data, 1, bmp->header.size, fp);
				printf("Valid image loaded.\n");
				fclose(fp);
				return bmp;
			} else {
				printf("Cannot alloc for image data.\n");
				fclose(fp);
			}
		} else {
			printf("Image is invalid.\n");
			fclose(fp);
		}
	}
	destroy_BMP(bmp);
	return NULL;
}

void display_info_BMP(BITMAP_FILE *bmp)
{
	if (bmp) {
		printf("***INFO BELOW ***\n"
			"***********************************\n"
			"BITMAP HEADER            : %u\n"
			"BITMAP SIZE              : %u\n"
			"BITMAP OFFSET            : %u\n"
			"BITMAP WIDTH             : %u\n"
			"BITMAP HEIGHT            : %u\n"
			"BITMAP BITS PER PIXEL    : %u\n"
			"***********************************\n",
			bmp->header.header, bmp->header.size,
			bmp->header.offset, bmp->info.width,
			bmp->info.height, bmp->info.bpp);
/*		printf("*** Data Below ***\n"
			"***********************************\n"
			"%s\n"
			"***********************************\n",
			bmp->data); */
	}
}

/* BMP_to_asciiart:  convert image data to ascii and print */
void BMP_to_asciiart(BITMAP_FILE *bmp)
{
	char shades[MAX_SHADES] = {'#','$','O','=','+','|','-','^','.',' '};
	int average_color;
	int rowsize;
	int x, y;

	if (!bmp)
		return;

	/* get proper row size */
	rowsize = bmp->info.width*3;

	/* loop through converting average color to ascii */
	for (y = bmp->info.height-1; y >= 0; y--) {
		for (x = 0; x < rowsize; x++) {
			average_color = (bmp->data[x+y*rowsize] +
					bmp->data[x+1+y*rowsize] +
					bmp->data[x+2+y*rowsize]) / 3;

			/* convert to a shade */
			average_color /= (256/MAX_SHADES);
			if (average_color >= MAX_SHADES)
				average_color = MAX_SHADES-1;

			/* write char to stdout */
			putchar(shades[average_color]);
		}
		putchar('\n');
	}
}

#endif