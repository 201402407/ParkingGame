#include "include.h"
#include "car.h"

const char *BMPFile[8] = { "carup.bmp", "carbackup.bmp", "carbackdown.bmp", "carfrontup.bmp", 
"carfrontdown.bmp", "carleftup.bmp", "carleftdown.bmp", "carwheel.bmp", };

Car::Car(void) {
	//Car::doTexture();
	for (int i = 0; i<8; i++)
		textures[i] = Car::_loadTexture(BMPFile[i]);
}
Car::~Car(void) {}


Car::uint Car::_loadTexture(pcStr filename) {
	// aux는 내부적으로 malloc이므로 free 필수
	AUX_RGBImageRec* img = auxDIBImageLoadA(filename);
	// 텍스쳐 생성
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	// 텍스쳐 확장 보간법 선정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// CLAMP_TO_EDGE 파라메터는 EDGE의 색상으로 경계를 확장
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	std::cout << filename << std::endl;
	// 메모리 할당 해제
	free(img->data);
	free(img);
	return texId;
}

void carFront() {
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(-450, -250, 0);
	glScalef(0.01, 0.3, 0.85);
	glutSolidCube(600);
	glColor3f(1, 0, 0);
	glutSolidCube(600);
	glPopMatrix();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(-370, -70, 0);
	glRotatef(-45, 0, 0, 1);
	glScalef(0.01, 0.4, 0.85);
	glutSolidCube(600);
	glPopMatrix();
}

void carBack() {
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(450, -250, 0);
	glScalef(0.01, 0.3, 0.85);
	glutSolidCube(600);
	glPopMatrix();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(380, -80, 0);
	glRotatef(38, 0, 0, 1);
	glScalef(0.01, 0.35, 0.85);
	glutSolidCube(600);
	glColor3f(1, 0, 0);
	glutSolidCube(600);
	glPopMatrix();
}
void Car::carRight() {
	glPushMatrix();
	glTranslatef(0, -250, -250);
	glScalef(1.5, 0.3, 0.01);
	glEnable(GL_TEXTURE_2D);
	/*
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-400, 450, 300);
	glTexCoord2f(1.0, 0.0); glVertex3f(-400, 450, -300);
	glTexCoord2f(1.0, 1.0); glVertex3f(-400, 1000, -200);
	glTexCoord2f(0.0, 1.0); glVertex3f(-400, 1000, 100);
	glEnd();
	*/
	/*
	glTexCoord2f(0.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, TEX_SIZE);
	*/
	
	
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-300, 1250, -400);
	glTexCoord2f(0.7, 1.0); glVertex3f(270, 1250, -400);
	glTexCoord2f(1.0, 0.0); glVertex3f(400, 450, -400);
	glTexCoord2f(0.0, 0.0); glVertex3f(-400, 450, -400);
	glEnd();
	
	
	glBindTexture(GL_TEXTURE_2D, textures[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-400, 450, -400);
	glTexCoord2f(0.9, 1.0); glVertex3f(550, 450, -400);
	glTexCoord2f(1.0, 0.0); glVertex3f(550, -450, -400);
	glTexCoord2f(0.0, 0.0); glVertex3f(-400, -450, -400);
	
	glEnd();
	
	glPopMatrix();
}
void carTop() {
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(15, 20, 0);
	glScalef(1.0, 0.01, 0.85);
	glutSolidCube(600);
	glColor3f(1, 0, 0);
	glutSolidCube(600);
	glPopMatrix();
}
void Car::carLeft() {
	
	glPushMatrix();
	glTranslatef(0, -250, 250);
	glScalef(1.5, 0.3, 0.01);
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-400, 450, -400);
	glTexCoord2f(1.0, 0.0); glVertex3f(400, 450, -400);
	glTexCoord2f(0.7, 1.0); glVertex3f(270, 1250, -400);
	glTexCoord2f(0.0, 1.0); glVertex3f(-300, 1250, -400);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, textures[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-400, -450, -400);
	glTexCoord2f(1.0, 0.0); glVertex3f(550, -450, -400);
	glTexCoord2f(0.9, 1.0); glVertex3f(550, 450, -400);
	glTexCoord2f(0.0, 1.0); glVertex3f(-400, 450, -400);
	glEnd();
	
	//glutSolidCube(600);
	//glColor3f(1, 0, 0);
	//glutSolidCube(600);
	glPopMatrix();
}
void carBottom() {
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(0, -350, 0);
	glScalef(1.5, 0.1, 0.85);
	glutSolidCube(600);
	glPopMatrix();
}

void wheel(int x, int y, int z) {
	float th;
	glColor3f(1, 0, 1);
	int k;
	for (k = 0; k < 50; k += 10) {
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			th = i * PI / 180;
			glVertex3f(x + 70 * cos(th), y + 70 * sin(th), z + k);
		}
	}
	glEnd();
}

void border() {
	glPushMatrix();
	glTranslatef(-50, -160, -250);
	glScalef(0.01, 0.6, 0.01);
	glColor3f(1, 1, 1);
	glutWireCube(600);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50, -160, 250);
	glScalef(0.01, 0.6, 0.01);
	glColor3f(1, 1, 1);
	glutWireCube(600);
	glPopMatrix();

}
/* 차 오브젝트 생성 */
void Car::drawCar() {
	carLeft();
	carRight();
	carFront();
	carBack();
	carTop();
	carBottom();
	wheel(-300, -450, 200);
	wheel(300, -450, 200);
	wheel(-300, -450, -200);
	wheel(300, -450, -200);
	//border();
}
