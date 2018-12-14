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
	glutSwapBuffers();
}

/*
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q': cam.lookUpwards(); break;
	case 'z': cam.lookDownwards(); break;
	case 'w': cam.slideForward(); break;
	case 's': cam.slideBackward(); break;
	case 'a': cam.strafeLeft(); break;
	case 'd': cam.strafeRight(); break;
	case 'e': exit(0); break;
	default: break;
	}
}
*/

void reshape(int x, int y) {
	//cam.reshape(x, y);
}

void mouseMovement(int x, int y) {
	//cam.mouseMovement(x, y);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("A basic OpenGL Window");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	//glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouseMovement);
	glutMainLoop();
}
