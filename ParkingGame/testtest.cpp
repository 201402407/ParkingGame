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

// 카메라 위치
float nx = 0;
float ny = 5750;
float nz = 4400;

bool cameraMove = false; // 카메라 이동
bool isDrag = false;

// 자동차 이동관련
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


bool keyPressed[256];	//키보드입력상황배열
void myKeyboard(unsigned char key, int x, int y) {
	keyPressed[key] = true;
}
//해당배열이 true면 Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }
//해당배열이 false면 Key Up

SwiftHeightField hField;
Skybox* skybox;	//스카이박스
Camera cam;	//카메라
CCamera objCamera;
Terrain* terrain;	//지형
Car* car; // 자동차
Bus* bus; // 버스

/* 라디안 구하는 함수 */
double getRadian(int num) {
	return num * (PI / 180);
}

/* 배경화면 SkyBox 생성 함수 */
void setBackGround() {
	glPushMatrix();
	
	glRotatef(90, 0.0, 1.0, 0.0);
	//glScalef(2.0f, 2.0f, 2.0f);
	skybox->draw(); // 스카이박스 생성
					//hField.Render();
	glPopMatrix();
}

/* 바닥 생성 함수 */
void setGround() {
	glPushMatrix();
	glTranslatef(0.0f, -300.0f, 0.0f); // 카메라 시점을 줄인 만큼 위치도 같은 만큼 - 해서 Translate 시킨다.
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(6.0, 5.0,7.0);
	
	terrain->RenderTerrain(-10, 0);//지형을 그린다.좌표를 보내주는 이유는 카메라가 위치한 타일블럭의 좌표를 계산하기 위해 ppt참조
	//hField.Render();
	glPopMatrix();
}

/* 버스 생성 함수 */
void setBus() {

	glPushMatrix();
	glTranslatef(-500, 40, 2600);
	glTranslatef(carLocationX, 0, 0);
	glTranslatef(0, 0, carLocationZ);
	glRotatef(rotateCar, 0.0, 1.0, 0.0);
	glScalef(0.8, 0.8, 0.8);
	GLfloat ambi_diff[] = { 1.0, 1.0, 1.0, 1.0 }; // ambient 와 diffuse
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 }; // 정반사 표면
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambi_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMateriali(GL_FRONT, GL_SHININESS, 120);
	bus->drawBus();
	glPopMatrix();
}

/* 기본 설정 */
int InitLightingGL() {
	/* 기본 설정 */
	glShadeModel(GL_SMOOTH);
	// 깊이버퍼, 후면제거활성화
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/* Lighting */
	GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.0f, 1.0f };
	GLfloat lightPos[] = { 0.0f, 2500.0f, 0.0f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glEnable(GL_LIGHTING); // GL_LIGHTING으로 사용한다고 설정.
	glEnable(GL_LIGHT0); // LIGHT0 종류를 사용한다고 설정.
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2); // set focusing strength
	//glEnable(GL_LIGHT0); // 0번빛을 사용한다.

	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
							// Really Nice Perspective    Calculations
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return TRUE;
}
void display(void) {
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* VIEW 설정 */
	// Perspective -> 사람의 눈이라고 가정. 볼 수 있는 각도 및 볼 수 있는 거리.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//printf("%.2f, %.2f, %.2f \n", nx, ny, nz);
	gluPerspective(60, 1.0f, 0.1f, 99999); // 최대 값 설정 -> 배경이 안끝나게

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (cameraMove) {
		gluLookAt(nx, ny, nz, 0, 0, kz, 0, 1, 0);
	}
	else {
		gluLookAt(nx, ny, nz, carLocationX, 0, carLocationZ, 0, 1, 0);
	}
	/* 기본 설정 */
	InitLightingGL();

	/* 배경 생성 */
	setBackGround();

	/* 바닥 생성 */
	setGround();
	
	/* 정지 시 속도 초기화 */
	if (!(keyPressed[UP] || keyPressed[DOWN])) {
		speed = 0;
	}
	
	/* 버스 생성 */
	setBus();

	/* 장애물 1 생성 ( 바로 오른쪽 길 ) */
	glTranslatef(-10, 40, 2600);
	
	/* 자동차 생성 */
	//glTranslatef(0.0, 420.0, 480.0); // 초기 세팅

	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.4, 0.75, 0.75);
	car->drawCar();
	
	/* 패배 요인 */
	if (carLocationZ < -5500 || carLocationZ > 500) { // 앞뒤 벗어난 기준
		printf("failed! \n");
	}
	if (carLocationX > 4000 || carLocationX < -3000) { // 좌우 벗어난 기준
		printf("failed! \n");
	}
	/* 승리 요인 */
	if(carLocationX > 3600 && carLocationX < 3700) { // 승리조건 X축
		if (carLocationZ > 1 && carLocationZ < 150) { // 승리조건 Z축
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

/* 마우스 이동 */
void notPress(int x, int y) {
	isDrag = false;
}
/* 마우스 드래그 시 카메라 움직임 */
void pressMouse(int x, int y) {
	// 지금 찍은 드래그 좌표
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
	
	// 이전 좌표에 대입
	lastx = x;
	lasty = y;
	
	glutPostRedisplay();
}

/* 마우스 스크롤 휠 시 카메라 줌인 줌 아웃 */
void mouse(int button, int state, int x, int y) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			printf("click! \n");
			break;
		case GLUT_WHEEL_UP: // 마우스 휠업
			cameraMove = true;
			printf("wheel up! \n");
			nx += 100;
			ny += 100;
			break;
		case GLUT_WHEEL_DOWN: // 마우스 휠 다운
			printf("wheel down! \n");
			cameraMove = true;
			nx -= 100;
			ny -= 100;
			break;
		}
	glutPostRedisplay();
}


/** 키보드 입력 스캔하는 Idle 함수.
* F1~F9 : 59 ~ 67
* F11~F12 : 133~134
* LEFT : 75, RIGHT : 77, UP : 72, DOWN : 80, SPACE : 32
* END : 79, DELETE : 83
*/
void Idle() {//해당키가 눌려있는지 지속적으로 검사해 다중입력을 할수 있게 한다
	// 우선 초기화
	isFront = false;
	isBack = false;
	isLeft = false;
	isRight = false;

	// 각도
	//angle = 3.14159 * (rotateCar / 180.0);


	/* 새로운 진행방향 */
	/*
	xp = dir_x * cos(angle) + dir_z * sin(angle);
	zp = -dir_x * sin(angle) + dir_z * cos(angle);
	*/

	/* 진행방향 */
	xp = -sin(getRadian(rotateCar));
	zp = -cos(getRadian(rotateCar));

	/* 반올림 */
	xp = floorf(xp * 1000) / 1000;
	zp = floorf(zp * 1000) / 1000;
	if (keyPressed['1'])	glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])	glPolygonMode(GL_FRONT, GL_FILL);
	if (keyPressed['e'])	// 줌인
	{
		printf("zoom in! \n");
		kz += 100;
		//ny += 100;
	}

	if (keyPressed['r'])	// 줌아웃
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

	if (keyPressed[UP]) {  // up은 -
		speed += 0.05;
		printf("%.2f \n", speed);

		/* 진행 방향에 따른 위치 이동 */
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

		/* 진행 방향에 따른 위치 이동 */
		carLocationX = carLocationX + (xp * speed);
		carLocationZ = carLocationZ + (zp * speed);
		isBack = true;
		
		//printf("down \n");
	}
	
	else { 
		//speed = 0;
		carMove = false;
	} //아무것도 눌리지 않았을때는 이동없다고 보고 계속 모델뷰 행렬을 유지시켜준다.
	glutPostRedisplay();// 다시그리기
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
	
	glutPostRedisplay();// 다시그리기
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(1200, 700);
	glutCreateWindow("Parking-Game!");
	Init();
	glutIdleFunc(Idle); // 여기서 키 눌렀는지 확인
	//glutKeyboardFunc(keyboard);
	//glutPassiveMotionFunc(mouseMovement);
	/* 특수키 입력 */
	glutSpecialFunc(pressKey);
	/* 아스키코드로 받아 수시로 확인 */
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp);
	/* 마우스 드래그 확인 */
	glutPassiveMotionFunc(notPress);
	glutMotionFunc(pressMouse);
	glutMouseFunc(mouse);
	// 자동차 객체
	car = new Car();
	bus = new Bus();
	skybox = new Skybox();
	// Terrain, Skybox객체
	terrain = new Terrain("testtest.raw", "area1.bmp", 1024, 1024);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}