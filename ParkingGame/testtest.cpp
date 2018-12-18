#include "include.h"
#include <fstream>
#include <assert.h>

#include "bus.h"
#include "camera.h"
#include "heightfield.h"
#include "skybox.h"
#include "car.h"
#include "terrain.h"
#pragma comment(lib,"glew32.lib")

#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'

// ī�޶� ��ġ
float nx = 0;
float ny = 800;
float nz = 1000;
bool cameraMove = false; // ī�޶� �̵�

// �ڵ��� �̵�����
float carLocationX = 0;
float carLocationZ = 0;
float rotateCar = 0;
bool isLeft = false;
bool isRight = false;
bool isFront = false;
bool isBack = false;
bool carMove = false;

float xpos = 85.078, ypos = 351.594, zpos = 281.033, xrot = 758, yrot = 90, angle = 0.0;
float lastx, lasty;

float bounce;
float cScale = 1.0;

bool keyPressed[256];	//Ű�����Է»�Ȳ�迭
void myKeyboard(unsigned char key, int x, int y) {
	printf("keyboard go \n");
	keyPressed[key] = true;
}
//�ش�迭�� true�� Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }
//�ش�迭�� false�� Key Up

SwiftHeightField hField;
Skybox* skybox;	//��ī�̹ڽ�
Camera cam;	//ī�޶�
CCamera objCamera;
Terrain* terrain;	//����
Car* car; // �ڵ���
Bus* bus; // ����

void camera(void) {
	int posX = (int)xpos;
	int posZ = (int)zpos;

	//glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);
	glTranslated(-xpos, -ypos, -zpos);
	printf("%.2f , %.2f , %.2f \n", xpos, ypos, zpos);
}

/* ���ȭ�� SkyBox ���� �Լ� */
void setBackGround() {
	glPushMatrix();
	
	glRotatef(90, 0.0, 1.0, 0.0);
	//glScalef(2.0f, 2.0f, 2.0f);
	skybox->draw(); // ��ī�̹ڽ� ����
					//hField.Render();
	glPopMatrix();
}

/* �ٴ� ���� �Լ� */
void setGround() {
	glPushMatrix();
	glTranslatef(-450.0f, -300.0f, 0.0); // ī�޶� ������ ���� ��ŭ ��ġ�� ���� ��ŭ - �ؼ� Translate ��Ų��.
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(2.0, 1.0, 2.0);
	terrain->RenderTerrain(cam.eye.x, cam.eye.z);//������ �׸���.��ǥ�� �����ִ� ������ ī�޶� ��ġ�� Ÿ�Ϻ��� ��ǥ�� ����ϱ� ���� ppt����
	//hField.Render();
	glPopMatrix();
}

void display(void) {
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* VIEW ���� */
	// Perspective -> ����� ���̶�� ����. �� �� �ִ� ���� �� �� �� �ִ� �Ÿ�.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//printf("%.2f, %.2f, %.2f \n", nx, ny, nz);
	gluPerspective(120, 1.0f, 0.1f, 99999); // �ִ� �� ���� -> ����� �ȳ�����

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (cameraMove) {
		gluLookAt(nx, ny, nz, nx, ny, 0, 0, 1, 0);
	}
	else {
		gluLookAt(nx, ny, nz, 0, 0, 0, 0, 1, 0);
	}
	/* �⺻ ���� */
	glShadeModel(GL_SMOOTH);
	// ���̹���, �ĸ�����Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);

	/* ��� ���� */
	setBackGround();

	/* �ٴ� ���� */
	setGround();
	
	/* ���� ���� */
	glTranslatef(carLocationX, 0, 0);
	glTranslatef(0, 0, carLocationZ);
	glRotatef(rotateCar, 0.0, 1.0, 0.0);
	bus->drawBus();
	/* �ڵ��� ���� */
	
	glTranslatef(0.0, 420.0, 480.0); // �ʱ� ����
	glTranslatef(carLocationX, 0, 0);
	glTranslatef(0, 0, carLocationZ);
	
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(rotateCar, 0.0, 1.0, 0.0);
	glScalef(0.8, 1.5, 1.5);
	//car->drawCar();
	
	glutSwapBuffers();
}

void Init(void) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	hField.Create("test2.raw", 1024, 1024);
}

void mouseMovement(int x, int y) {
	int diffx = x - lastx;
	int diffy = y - lasty;
	lastx = x;
	lasty = y;
	xrot += (float)diffy;
	yrot += (float)diffx;
}

/* ���콺 Ŭ�� �̺�Ʈ */
/*
void MyMouseClick(GLint Button, GLint State, GLint x, GLint y)
{
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN)
	{
		TopLeftX = x;
		TopLeftY = y;
	}
}
*/


/* ���콺 �巡�� �� ī�޶� ������ */
void pressMouse(int x, int y) {
	printf("mouse %d is x�̰� %d �� y��. \n", x, y);
	glutPostRedisplay();
}

/* ���콺 ��ũ�� �� �� ī�޶� ���� �� �ƿ� */
void mouse(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			printf("click! \n");
			break;
		case 3: // ���콺 �پ�
			cameraMove = true;
			printf("wheel up! \n");
			nx += 10;
			ny += 10;
			break;
		case 4: // ���콺 �� �ٿ�
			printf("wheel down! \n");
			cameraMove = true;
			nx -= 10;
			ny -= 10;
			break;
		}
	}
	glutPostRedisplay();
}


/** Ű���� �Է� ��ĵ�ϴ� Idle �Լ�.
* F1~F9 : 59 ~ 67
* F11~F12 : 133~134
* LEFT : 75, RIGHT : 77, UP : 72, DOWN : 80, SPACE : 32
* END : 79, DELETE : 83
*/
void Idle() {//�ش�Ű�� �����ִ��� ���������� �˻��� �����Է��� �Ҽ� �ְ� �Ѵ�
	// �켱 �ʱ�ȭ
	isFront = false;
	isBack = false;
	isLeft = false;
	isRight = false;

	if (keyPressed['1'])	glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])	glPolygonMode(GL_FRONT, GL_FILL);
	/*
	if (keyPressed[UP]) { // UP
		carLocationZ = carLocationZ + 5;	isFront = true;	carMove = true;	printf("up \n");
	}
	if (keyPressed[LEFT]) { // LEFT
		carLocationX = carLocationX + 5;	isLeft = true;	carMove = true;	printf("left \n");
	}
	if (keyPressed[RIGHT]) {  // RIGHT
		carLocationX = carLocationX - 5;	isRight = true; carMove = true;	printf("right \n");
	}
	if (keyPressed[DOWN]) {  // DOWN
		carLocationZ = carLocationZ - 5;	isBack = true; carMove = true;	printf("down \n");
	}
	*/

	if (keyPressed[UP]) { 
		carLocationZ = carLocationZ - 10;	isFront = true;	printf("up \n"); rotateCar = rotateCar + 180;
	}
	if (keyPressed[DOWN]) {
		carLocationZ = carLocationZ + 10;	isBack = true;	printf("down \n");	rotateCar = rotateCar - 180;
	}
	if (keyPressed[LEFT]) {
		carLocationX = carLocationX - 10;	isLeft = true;	printf("left \n");	rotateCar++;
	}
	if (keyPressed[RIGHT]) { 
		carLocationX = carLocationX + 10;	isRight = true;printf("right \n");	rotateCar--;
	}
	//if (cam.eye.y<terrain->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }// ������ Colision Detection ��ǥ�� �Ʒ��� ī�޶� ���������� ��ǥ�� ���� ������Ŵ
	
	else { carMove = false; } //�ƹ��͵� ������ �ʾ������� �̵����ٰ� ���� ��� �𵨺� ����� ���������ش�.
	glutPostRedisplay();// �ٽñ׸���
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void pressKey(int key, int x, int y) {
	if (key == GLUT_KEY_UP)		keyPressed[72] = true;
	if (key == GLUT_KEY_DOWN)	keyPressed[80] = true;
	if (key == GLUT_KEY_LEFT)	keyPressed[75] = true;
	if (key == GLUT_KEY_RIGHT)	keyPressed[77] = true;
	
	
	switch (key) {
	case GLUT_KEY_F1:
		nx += 50;
		printf("F1");
		//cam.pitch(0.5);
		break;
	case GLUT_KEY_F2:
		nx -= 50;
		printf("F2");
		//cam.pitch(-0.5);
		break;
	case GLUT_KEY_F3:
		ny += 50;
		printf("F3");
		//cam.yaw(0.5);
		break;
	case GLUT_KEY_F4:
		ny -= 50;
		//cam.yaw(-0.5);
		printf("F4");
		break;
	case GLUT_KEY_F5:
		nz += 50;
		printf("F5");
		//cam.roll(0.5);
		break;
	case GLUT_KEY_F6:
		printf("F6");
		nz -= 50;
		//cam.roll(-0.5);
		break;
		/*
	case GLUT_KEY_UP: // ������ �̵�
		keyPressed[72] = true;
		//nz += 50;
		break;
	case GLUT_KEY_DOWN: // �ڷ� �̵�
		keyPressed[80] = true;
		
		//nz -= 50;
		break;
	case GLUT_KEY_LEFT: // ���� �̵�
		keyPressed[75] = true;
		//nx += 50;
		break;
	case GLUT_KEY_RIGHT: // ������ �̵�
		keyPressed[77] = true;
		//nx -= 50;
		break;
		*/
	}
	
	glutPostRedisplay();// �ٽñ׸���
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(1200, 700);
	glutCreateWindow("Parking-Game!");
	Init();
	glutIdleFunc(Idle); // ���⼭ Ű �������� Ȯ��
	//glutKeyboardFunc(keyboard);
	//glutPassiveMotionFunc(mouseMovement);
	/* Ư��Ű �Է� */
	glutSpecialFunc(pressKey);
	/* �ƽ�Ű�ڵ�� �޾� ���÷� Ȯ�� */
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp);
	/* ���콺 �巡�� Ȯ�� */
	glutMotionFunc(pressMouse);
	glutMouseFunc(mouse);
	// �ڵ��� ��ü
	car = new Car();
	bus = new Bus();
	skybox = new Skybox();
	// Terrain, Skybox��ü
	terrain = new Terrain("map2.raw", "down.bmp", 1024, 1024);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}