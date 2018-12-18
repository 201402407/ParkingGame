#include "include.h"
#include "bus.h"

using namespace std;

#define TEX_SIZE 1 // 최대 값.
#define GL_CLAMP_TO_EDGE 0x812F

const char *texFile[6] = {
	"busleft.bmp",
	"busright.bmp",
	"busback.bmp",
	"busfront.bmp",
	"bustop.bmp",
	"busbottom.bmp"
};

Bus::Bus(void) {
	for (int i = 0; i<6; i++)
		textures[i] = loadTexture(texFile[i]);
}
Bus::~Bus(void) {}

void Bus::busRight() {// RIGHT
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glRotatef(180, 0.0, 1.0, 0.0);
	glScalef(4.0, 4.0, 4.0);
	glBindTexture(GL_TEXTURE_2D, textures[1]); // RIGHT
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(-45, -80, 100);
	glTexCoord2f(1.0, 1.0); glVertex3f(-45, 80, 100);
	glTexCoord2f(0.0, 1.0); glVertex3f(-45, 80, -100);
	glTexCoord2f(0.0, 0.0); glVertex3f(-45, -80, -100);
	glEnd();

	glPopMatrix();
}

void Bus::busLeft() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	//glTranslatef(-200, -230, 0);
	//glScalef(3.0, 1.0, 3.0);
	glScalef(4.0, 4.0, 4.0);
	glBindTexture(GL_TEXTURE_2D, textures[0]); // LEFT
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-45, 80, -100);
	glTexCoord2f(0.0, 0.0); glVertex3f(-45, -80, -100);
	glTexCoord2f(1.0, 0.0); glVertex3f(-45, -80, 100);
	glTexCoord2f(1.0, 1.0); glVertex3f(-45, 80, 100);
	glEnd();
	glPopMatrix();

}
void Bus::busBack() {

	glPushMatrix();
	//glTranslatef(0, -250, -250);
	//glScalef(1.5, 0.3, 0.01);
	glEnable(GL_TEXTURE_2D);
	glScalef(4.0, 4.0, 4.0);
	glBindTexture(GL_TEXTURE_2D, textures[2]); // BACK
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-45, -80, 100);
	glTexCoord2f(1.0, 0.0); glVertex3f(45, -80, 100);
	glTexCoord2f(0.0, 1.0); glVertex3f(45, 80, 100);
	glTexCoord2f(-1.0, 1.0); glVertex3f(-45, 80, 100);
	/*
	glTexCoord2f(0.0, 0.0); glVertex3f(-45, -80, 100);
	glTexCoord2f(1.0, 0.0); glVertex3f(45, -80, 100);
	glTexCoord2f(1.0, 1.0); glVertex3f(45, 80, 100);
	glTexCoord2f(0.0, 1.0); glVertex3f(-45, 80, 100);
	*/
	glEnd();
	glPopMatrix();
}
void Bus::busTop() { // UP
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	//glTranslatef(0, 110, 0);
	//glScalef(1.0, 0.01, 0.85);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glScalef(4.0, 4.0, 4.0);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-100, 80, 45);
	glTexCoord2f(1.0, 1.0); glVertex3f(100, 80, 45);
	glTexCoord2f(1.0, 0.0); glVertex3f(100, 80, -45);
	glTexCoord2f(0.0, 0.0); glVertex3f(-100, 80, -45);

	glEnd();
	//glutSolidCube(600);
	glPopMatrix();
}
void Bus::busFront() {

	glPushMatrix();
	//glTranslatef(0, -250, 250);
	//glScalef(1.5, 0.3, 0.01);
	glEnable(GL_TEXTURE_2D);
	glScalef(4.0, 4.0, 4.0);
	glBindTexture(GL_TEXTURE_2D, textures[3]); // FRONT
	glBegin(GL_QUADS);
	glTexCoord2f(-1.0, 0.96); glVertex3f(-45, 80, -100);
	glTexCoord2f(0.0, 0.96); glVertex3f(45, 80, -100);
	glTexCoord2f(1.0, 0.0); glVertex3f(45, -80, -100);
	glTexCoord2f(0.0, 0.0); glVertex3f(-45, -80, -100);
	glEnd();
	//glutSolidCube(600);
	//glColor3f(1, 0, 0);
	//glutSolidCube(600);
	glPopMatrix();
}
void Bus::busBottom() {
	glEnable(GL_TEXTURE_2D);
	//glColor3f(1, 1, 1);
	glPushMatrix();
	//glTranslatef(0, -350, 0);
	//glScalef(1.5, 0.1, 0.85);
	glScalef(4.0, 4.0, 4.0);
	glBindTexture(GL_TEXTURE_2D, textures[5]); // BOTTOM
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-45, -80, -100);
	glTexCoord2f(1.0, 1.0); glVertex3f(45, -80, -100);
	glTexCoord2f(1.0, 0.0); glVertex3f(45, -80, 100);
	glTexCoord2f(0.0, 0.0); glVertex3f(-45, -80, 100);

	glEnd();
	//glutSolidCube(600);
	glPopMatrix();
}

void Bus::drawBus() {
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// skybox의 깊이 정보가 씌여지지 않도록 depth_test를 끔
	//glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	// Skybox는 다른 색상의 영향을 받으면 안되므로 DECAL
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	Bus::busLeft();
	Bus::busFront();
	Bus::busRight();
	Bus::busBack();
	Bus::busTop();
	Bus::busBottom();
	glPopMatrix();
}
Bus::uint Bus::loadTexture(pcStr filename) {
	// aux는 내부적으로 malloc이므로 free 필수
	AUX_RGBImageRec* img = auxDIBImageLoadA(filename);
	printf("ok \n");
	// 텍스쳐 생성
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	printf("ok \n");
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	// 텍스쳐 확장 보간법 선정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);

	std::cout << filename << std::endl;
	// 메모리 할당 해제
	free(img->data);
	free(img);
	return texId;
}