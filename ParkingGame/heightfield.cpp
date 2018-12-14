#include "include.h"
#include "jpeg.h"
#include "heightfield.h"

bool SwiftHeightField::Create(char *hFileName, const int hWidth, const int hHeight) {
	hmHeight = hHeight;
	hmWidth = hWidth;

	FILE *fp;

	fp = fopen(hFileName, "rb");

	fread(hHeightField, 1, hWidth * hHeight, fp);

	fclose(fp);

	Texture(tID, "sand.bmp", 0);

	return true;
}

void SwiftHeightField::Render(void) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tID[0]);
	
	for (int hMapX = 0; hMapX < hmWidth; hMapX++) {
		for (int hMapZ = 0; hMapZ < hmHeight; hMapZ++) {
			//printf("%d , %d , %d \n", hMapX, hHeightField[hMapX][hMapZ], hMapZ);
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f((float)hMapX / hmWidth, (float)hMapZ / hmHeight);
			glVertex3f(hMapX, hHeightField[hMapX][hMapZ], hMapZ);

			glTexCoord2f((float)hMapX / hmWidth, (float)(hMapZ + 1) / hmHeight);
			glVertex3f(hMapX, hHeightField[hMapX][hMapZ + 1], hMapZ + 1);

			glTexCoord2f((float)(hMapX + 1) / hmWidth, (float)hMapZ / hmHeight);
			glVertex3f(hMapX + 1, hHeightField[hMapX + 1][hMapZ], hMapZ);

			glTexCoord2f((float)(hMapX + 1) / hmWidth, (float)(hMapZ + 1) / hmHeight);
			glVertex3f(hMapX + 1, hHeightField[hMapX + 1][hMapZ + 1], hMapZ + 1);
			glEnd();
		}
	}

	glDisable(GL_TEXTURE_2D);
}