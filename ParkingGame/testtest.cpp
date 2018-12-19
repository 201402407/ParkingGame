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
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

// ī�޶� ��ġ
float nx = 0;
float ny = 5750;
float nz = 4400;

bool cameraMove = false; // ī�޶� �̵�
bool isDrag = false;

// �ڵ��� �̵�����
float carLocationX = 0;
float carLocationZ = 0;

float xp, zp = 0.0;
float speed = 0;
float kz = 0;
int rotateCar = 0;
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

/* ���� ���ϴ� �Լ� */
double getRadian(int num) {
	return num * (PI / 180);
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
	glTranslatef(0.0f, -300.0f, 0.0f); // ī�޶� ������ ���� ��ŭ ��ġ�� ���� ��ŭ - �ؼ� Translate ��Ų��.
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(6.0, 5.0,7.0);
	
	terrain->RenderTerrain(-10, 0);//������ �׸���.��ǥ�� �����ִ� ������ ī�޶� ��ġ�� Ÿ�Ϻ��� ��ǥ�� ����ϱ� ���� ppt����
	//hField.Render();
	glPopMatrix();
}

/* ���� ���� �Լ� */
void setBus() {

	glPushMatrix();
	glTranslatef(-500, 40, 2600);
	glTranslatef(carLocationX, 0, 0);
	glTranslatef(0, 0, carLocationZ);
	glRotatef(rotateCar, 0.0, 1.0, 0.0);
	glScalef(0.8, 0.8, 0.8);
	GLfloat ambi_diff[] = { 1.0, 1.0, 1.0, 1.0 }; // ambient �� diffuse
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 }; // ���ݻ� ǥ��
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambi_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMateriali(GL_FRONT, GL_SHININESS, 120);
	bus->drawBus();
	glPopMatrix();
}

/* �⺻ ���� */
int InitLightingGL() {
	/* �⺻ ���� */
	glShadeModel(GL_SMOOTH);
	// ���̹���, �ĸ�����Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/* Lighting */
	GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.0f, 1.0f };
	GLfloat lightPos[] = { 0.0f, 2500.0f, 0.0f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glEnable(GL_LIGHTING); // GL_LIGHTING���� ����Ѵٰ� ����.
	glEnable(GL_LIGHT0); // LIGHT0 ������ ����Ѵٰ� ����.
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2); // set focusing strength
	//glEnable(GL_LIGHT0); // 0������ ����Ѵ�.

	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
							// Really Nice Perspective    Calculations
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return TRUE;
}
void display(void) {
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* VIEW ���� */
	// Perspective -> ����� ���̶�� ����. �� �� �ִ� ���� �� �� �� �ִ� �Ÿ�.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//printf("%.2f, %.2f, %.2f \n", nx, ny, nz);
	gluPerspective(60, 1.0f, 0.1f, 99999); // �ִ� �� ���� -> ����� �ȳ�����

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (cameraMove) {
		gluLookAt(nx, ny, nz, 0, 0, kz, 0, 1, 0);
	}
	else {
		gluLookAt(nx, ny, nz, carLocationX, 0, carLocationZ, 0, 1, 0);
	}
	/* �⺻ ���� */
	InitLightingGL();

	/* ��� ���� */
	setBackGround();

	/* �ٴ� ���� */
	setGround();
	
	/* ���� �� �ӵ� �ʱ�ȭ */
	if (!(keyPressed[UP] || keyPressed[DOWN])) {
		speed = 0;
	}
	
	/* ���� ���� */
	setBus();

	/* ��ֹ� 1 ���� ( �ٷ� ������ �� ) */
	glTranslatef(-10, 40, 2600);
	
	/* �ڵ��� ���� */
	//glTranslatef(0.0, 420.0, 480.0); // �ʱ� ����

	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.4, 0.75, 0.75);
	car->drawCar();
	
	/* �й� ���� */
	if (carLocationZ < -5500 || carLocationZ > 500) { // �յ� ��� ����
		printf("failed! \n");
	}
	if (carLocationX > 4000 || carLocationX < -3000) { // �¿� ��� ����
		printf("failed! \n");
	}
	/* �¸� ���� */
	if(carLocationX > 3600 && carLocationX < 3700) { // �¸����� X��
		if (carLocationZ > 1 && carLocationZ < 150) { // �¸����� Z��
			printf("success! \n");
		}

	}


	printf("%.2f, %.2f \n", carLocationX, carLocationZ);
	
	glutSwapBuffers();
}

void Init(void) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	hField.Create("test2.raw", 1024, 1024);
}

/* ���콺 �̵� */
void notPress(int x, int y) {
	isDrag = false;
}
/* ���콺 �巡�� �� ī�޶� ������ */
void pressMouse(int x, int y) {
	// ���� ���� �巡�� ��ǥ
	int currentx = x;
	int currenty = y;
	
	if (!isDrag) {
		cout << "DRag" << endl;
		lastx = currentx;
		lasty = currenty;
		isDrag = true;
		return;
	}

	nx = nx - ((currentx - lastx) * 6);
	ny = ny - ((currenty - lasty) * 6);
	
	// ���� ��ǥ�� ����
	lastx = x;
	lasty = y;
	
	glutPostRedisplay();
}

/* ���콺 ��ũ�� �� �� ī�޶� ���� �� �ƿ� */
void mouse(int button, int state, int x, int y) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			printf("click! \n");
			break;
		case GLUT_WHEEL_UP: // ���콺 �پ�
			cameraMove = true;
			printf("wheel up! \n");
			nx += 100;
			ny += 100;
			break;
		case GLUT_WHEEL_DOWN: // ���콺 �� �ٿ�
			printf("wheel down! \n");
			cameraMove = true;
			nx -= 100;
			ny -= 100;
			break;
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

	// ����
	//angle = 3.14159 * (rotateCar / 180.0);


	/* ���ο� ������� */
	/*
	xp = dir_x * cos(angle) + dir_z * sin(angle);
	zp = -dir_x * sin(angle) + dir_z * cos(angle);
	*/

	/* ������� */
	xp = -sin(getRadian(rotateCar));
	zp = -cos(getRadian(rotateCar));

	/* �ݿø� */
	xp = floorf(xp * 1000) / 1000;
	zp = floorf(zp * 1000) / 1000;
	if (keyPressed['1'])	glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])	glPolygonMode(GL_FRONT, GL_FILL);
	if (keyPressed['e'])	// ����
	{
		printf("zoom in! \n");
		kz += 100;
		//ny += 100;
	}

	if (keyPressed['r'])	// �ܾƿ�
	{
		printf("zoom out! \n");
		kz -= 100;
		//ny -= 100;
	}

	if (keyPressed[UP] || keyPressed[DOWN] || keyPressed[LEFT] || keyPressed[RIGHT]) {
		carMove = true;
		//rotateCar = rotateCar % 360;
	}

	if (keyPressed[LEFT]) {
		rotateCar++;

		
		cout <<"rotate : " << rotateCar << endl;
		cout << "xp : " << xp << endl;
		cout << "zp : " << zp << endl;
		//cout << "carLocationX : " << carLocationX << endl;
		//cout << "carLocationZ : " << carLocationZ << endl;
		//		carLocationX = carLocationX - ((float)(tan(rotateCar) * 2));
		isLeft = true;
		
		//printf("left \n");
	}
	if (keyPressed[RIGHT]) {
		rotateCar--;
		cout << "rotate : " << rotateCar << endl;
		cout << "xp : " << xp << endl;
		cout << "zp : " << zp << endl;
		//carLocationX = carLocationX + (rotateCar / 2);
		//carLocationX = carLocationX + ((float)(tan(rotateCar) * 2));
		isRight = true;

		//printf("right \n");
	}

	if (keyPressed[UP]) {  // up�� -
		speed += 0.05;
		printf("%.2f \n", speed);

		/* ���� ���⿡ ���� ��ġ �̵� */
		carLocationX = carLocationX + (xp * speed);
		carLocationZ = carLocationZ + (zp * speed);

		/*
		cout << "rotate : " << rotateCar << endl;
		cout << "xp : " << xp << endl;
		cout << "zp : " << zp << endl;
		cout << "carLocationX : " << carLocationX << endl;
		cout << "carLocationZ : " << carLocationZ << endl;
		*/
	}
	if (keyPressed[DOWN]) {
		speed -= 0.05;
		//printf("%.2f \n", speed);

		/* ���� ���⿡ ���� ��ġ �̵� */
		carLocationX = carLocationX + (xp * speed);
		carLocationZ = carLocationZ + (zp * speed);
		isBack = true;
		
		//printf("down \n");
	}
	
	else { 
		//speed = 0;
		carMove = false;
	} //�ƹ��͵� ������ �ʾ������� �̵����ٰ� ���� ��� �𵨺� ����� ���������ش�.
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
	glutPassiveMotionFunc(notPress);
	glutMotionFunc(pressMouse);
	glutMouseFunc(mouse);
	// �ڵ��� ��ü
	car = new Car();
	bus = new Bus();
	skybox = new Skybox();
	// Terrain, Skybox��ü
	terrain = new Terrain("testtest.raw", "area1.bmp", 1024, 1024);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}