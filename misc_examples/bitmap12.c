#include "../common/bitmap.h"

int main(void)
{
	BITMAP_FILE *bmp;

	bmp = create_BMP("test.bmp", 64, 64, 24);
	if (check_BMP(bmp))
		return 1;
	clear_BMP(bmp, 0, 0, 0);
	draw_hcircle_BMP(bmp, -10, 20, 20, 255, 0, 0);
	write_BMP(bmp, 0);
	if (check_BMP(bmp))
		return 1;
	destroy_BMP(bmp);
	return 0;
}
