#include "include.h"
#include "heightfield.h"
#pragma once
GLUquadricObj *solar[3];
GLuint textureMapping[3]; //텍스쳐 맵핑 변수
AUX_RGBImageRec *textureBMPImg;

char *BMPFileName[3] = { "posy.bmp", "backGR.bmp", "background.bmp" };

AUX_RGBImageRec *LoadBMPFile(char *filename) { // 이미지 읽어오기
	FILE *pFile = NULL;
	if (!filename) return NULL;
	pFile = fopen(filename, "r");

	if (pFile) {
		fclose(pFile);
		return auxDIBImageLoad(filename);
	}
	return NULL;
}

void Texture(unsigned int tTexture[], char *strFileName, int ID) {
	textureBMPImg = LoadBMPFile(strFileName);
	glGenTextures(1, &tTexture[ID]);
	glBindTexture(GL_TEXTURE_2D, tTexture[ID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, textureBMPImg->sizeX, textureBMPImg->sizeY, GL_RGB, GL_UNSIGNED_BYTE, textureBMPImg->data);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitMap->sizeX, pBitMap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitMap->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	free(textureBMPImg->data);
	free(textureBMPImg);
	/*
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	*/
}