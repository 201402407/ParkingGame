#include "include.h"
#include "car.h"

const char *BMPFile[9] = {"cartop.bmp", "carbackup.bmp", "carbackdown.bmp", "carfrontup.bmp",
"carfrontdown.bmp", "carleftup.bmp", "carleftdown.bmp", "carwheel.bmp", "carfrontmiddle.bmp" };

Car::Car(void) {
	//Car::doTexture();
	for (int i = 0; i < 9; i++)
	//	printf("asd");
		textures[i] = _loadTexture(BMPFile[i]);
}
Car::~Car(void) {}


Car::uint Car::_loadTexture(pcStr filename) {
	// aux는 내부적으로 malloc이므로 free 필수
	AUX_RGBImageRec* img = auxDIBImageLoad(filename);
	
	// 텍스쳐 생성
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	
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

void Car::carFront() {
	glEnable(GL_TEXTURE_2D);

	/* 앞면의 밑 */
//	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(820, -250, 0);
//	glRotatef(8, 0, 0, 1);
	glScalef(0.01, 0.3, 0.85);
	//glutSolidCube(600);

	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0); glVertex3f(550, 450, 300);
	glTexCoord2f(1.0, 0.0); glVertex3f(550, -450, 300);
	glTexCoord2f(0.0, 0.0); glVertex3f(550, -450, -300);
	glTexCoord2f(0.0, 1.0); glVertex3f(550, 450, -300);
	glEnd();
	//glutSolidCube(600);
	glPopMatrix();
	
	/* 앞면의 앞 */
	glPushMatrix();
	glTranslatef(1000, -155, 0);
	glRotatef(85, 0, 0, 1);
	glScalef(0.01, 0.4, 0.85);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[8]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0); glVertex3f(400, 1200, 300);
	glTexCoord2f(1.0, 0.0); glVertex3f(400, 450, 300);
	glTexCoord2f(0.0, 0.0); glVertex3f(400, 450, -300);
	glTexCoord2f(0.0, 1.0); glVertex3f(400, 1200, -300);
	glEnd();
	//glutSolidCube(600);
	glPopMatrix();
	/* 앞면의 위 */
	//glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(720, -250, 0);
	glRotatef(40, 0, 0, 1);
	glScalef(0.01, 0.4, 0.85);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.7, 1.0); glVertex3f(400, 1250, 300);
	glTexCoord2f(1.0, 0.0); glVertex3f(400, 450, 300);
	glTexCoord2f(0.0, 0.0); glVertex3f(400, 450, -300);
	glTexCoord2f(0.0, 1.0); glVertex3f(400, 1250, -300);
	glEnd();
	//glutSolidCube(600);
	glPopMatrix();
}

void Car::carBack() {
	
	/* 뒷쪽의 아래*/
	glPushMatrix();
	glTranslatef(-600, -230, 0);
	glScalef(0.01, 0.35, 0.85);
	
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(-0.1, 1.0); glVertex3f(-550, 450, -300);
	glTexCoord2f(0.1, 0.0); glVertex3f(-550, -450, -300);
	glTexCoord2f(1.0, 0.0); glVertex3f(-550, -450, 300);
	glTexCoord2f(0.8, 0.9); glVertex3f(-550, 450, 300);
	glEnd();
	glPopMatrix();

	/* 뒷쪽의 위 */
	glPushMatrix();
	glTranslatef(-660, -210, 0);
	glRotatef(-32, 0, 0, 1);
	glScalef(0.01, 0.3, 0.85);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(-0.1, 1.0); glVertex3f(-400, 1350, -300);
	glTexCoord2f(0.2, 0.0); glVertex3f(-400, 450, -300);
	glTexCoord2f(1.0, 0.0); glVertex3f(-400, 450, 300);
	glTexCoord2f(0.65, 0.9); glVertex3f(-400, 1350, 300);
	glEnd();
	glPopMatrix();

}
void Car::carLeft() {
	glPushMatrix();
	glTranslatef(0, -250, -250);
	glScalef(1.5, 0.3, 0.01);
	glEnable(GL_TEXTURE_2D);
	
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
void Car::carTop() {
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(0, 110, 0);
	glScalef(1.0, 0.01, 0.85);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-450, 1250, 300);
	glTexCoord2f(1.0, 1.0); glVertex3f(400, 1250, 300);
	glTexCoord2f(1.0, 0.0); glVertex3f(400, 1250, -300);
	glTexCoord2f(0.0, 0.0); glVertex3f(-450, 1250, -300);
	glEnd();
	//glutSolidCube(600);
	glPopMatrix();
}
void Car::carRight() {
	
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
void Car::carBottom() {
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(0, -350, 0);
	glScalef(1.5, 0.1, 0.85);
	glutSolidCube(600);
	glPopMatrix();
}

void Car::carWheel(int x, int y, int z) {
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
	carWheel(-300, -450, 200);
	carWheel(300, -450, 200);
	carWheel(-300, -450, -200);
	carWheel(300, -450, -200);
	//border();
}
