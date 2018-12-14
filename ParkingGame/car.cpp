#include "include.h"
#include "car.h"


Car::Car(void) {
	
}
Car::~Car(void) {}

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
void carRight() {
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(0, -250, -250);
	glScalef(1.5, 0.3, 0.01);
	glutSolidCube(600);
	glColor3f(1, 0, 0);
	glutSolidCube(600);
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
void carLeft() {
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(0, -250, 250);
	glScalef(1.5, 0.3, 0.01);
	glutSolidCube(600);
	glColor3f(1, 0, 0);
	glutSolidCube(600);
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
	wheel(-300, -400, 200);
	wheel(300, -400, 200);
	wheel(-300, -400, -200);
	wheel(300, -400, -200);
	border();
}
