#include "include.h"

#include "include.h"

#include <windows.h>
#include <stdio.h>
#include <glut.h>

GLuint texture[6]; //the array for our texture

GLfloat angle = 0.0;

GLuint LoadTexture(const char * filename, int width, int height) {
	GLuint texture;
	unsigned char * data;
	FILE* file;

	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	return texture;
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void skybox(void) {
	float x = 0;
	float y = 0;
	float z = 0;
	float width = 500;
	float height = 500;
	float length = 500;
	// Bind the BACK texture of the sky map to the BACK side of the cube
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Center the skybox
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();
	//glBindTexture( GL_TEXTURE_CUBE_MAP, texture[0] ); //bind the texture
	//glRotatef( angle, 1.0f, 1.0f, 1.0f );
	//glutSolidSphere(2, 40, 40);
}

void setBackground() {
	texture[0] = LoadTexture("background.bmp", 256, 256); //load the texture
	texture[1] = LoadTexture("skybox\front.jpg", 256, 256); //load the texture
	texture[2] = LoadTexture("skybox\left.jpg", 256, 256); //load the texture
	texture[3] = LoadTexture("skybox\right.jpg", 256, 256); //load the texture
	texture[4] = LoadTexture("skybox\bottom.jpg", 256, 256); //load the texture
	texture[5] = LoadTexture("skybox\top.jpg", 256, 256); //load the texture
	glEnable(GL_TEXTURE_2D); //enable 2D texturing
							 glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
							 glEnable(GL_TEXTURE_GEN_T);
	skybox();
	for (int i = 0; i < 6; i++) {
		FreeTexture(texture[i]);
	}
}
/*
void display(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt (20.0, 20.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	texture[0] = LoadTexture("Back.bmp", 256, 256); //load the texture
	texture[1] = LoadTexture("Front.bmp", 256, 256); //load the texture
	texture[2] = LoadTexture("Left.bmp", 256, 256); //load the texture
	texture[3] = LoadTexture("Right.bmp", 256, 256); //load the texture
	texture[4] = LoadTexture("Bottom.bmp", 256, 256); //load the texture
	texture[5] = LoadTexture("Top.bmp", 256, 256); //load the texture
	glEnable(GL_TEXTURE_2D); //enable 2D texturing
							 //glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
							 //glEnable(GL_TEXTURE_GEN_T);
	skybox();
	for (int i = 0; i < 6; i++) {
		FreeTexture(texture[i]);
	}
	glutSwapBuffers()
}
*/
