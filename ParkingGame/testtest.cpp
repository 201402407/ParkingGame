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

#define PI 3.141592

// 카메라 위치
float nx = 0;
float ny = 800;
float nz = 1000;

bool cameraMove = false; // 카메라 이동
bool isDrag = false;

// 자동차 이동관련
float carLocationX = 0;
float carLocationZ = 0;

float xp, zp = 0.0;
float speed = 0;

GLuint rotateCar = 0;
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
	printf("keyboard go \n");
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

void camera(void) {
	int posX = (int)xpos;
	int posZ = (int)zpos;

	//glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);
	glTranslated(-xpos, -ypos, -zpos);
	printf("%.2f , %.2f , %.2f \n", xpos, ypos, zpos);
}

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
	glTranslatef(-450.0f, -300.0f, 0.0); // 카메라 시점을 줄인 만큼 위치도 같은 만큼 - 해서 Translate 시킨다.
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(2.0, 1.0, 2.0);
	terrain->RenderTerrain(cam.eye.x, cam.eye.z);//지형을 그린다.좌표를 보내주는 이유는 카메라가 위치한 타일블럭의 좌표를 계산하기 위해 ppt참조
	//hField.Render();
	glPopMatrix();
}

void display(void) {
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* VIEW 설정 */
	// Perspective -> 사람의 눈이라고 가정. 볼 수 있는 각도 및 볼 수 있는 거리.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//printf("%.2f, %.2f, %.2f \n", nx, ny, nz);
	gluPerspective(120, 1.0f, 0.1f, 99999); // 최대 값 설정 -> 배경이 안끝나게

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (cameraMove) {
		gluLookAt(nx, ny, nz, nx, ny, 0, 0, 1, 0);
	}
	else {
		gluLookAt(nx, ny, nz, 0, 0, 0, 0, 1, 0);
	}
	/* 기본 설정 */
	glShadeModel(GL_SMOOTH);
	// 깊이버퍼, 후면제거활성화
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);

	/* 배경 생성 */
	//setBackGround();

	/* 바닥 생성 */
	setGround();
	
	if (!(keyPressed[UP] || keyPressed[DOWN])) {
		speed = 0;
	}

	/* 버스 생성 */
	glTranslatef(carLocationX, 0, 0);
	glTranslatef(0, 0, carLocationZ);
	glRotatef(rotateCar, 0.0, 1.0, 0.0);
	bus->drawBus();

	/* 자동차 생성 */
	glTranslatef(0.0, 420.0, 480.0); // 초기 세팅
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

/* 마우스 클릭 이벤트 */
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

	nx = nx - (currentx - lastx);
	ny = ny - (currenty - lasty);
	
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
			nx += 10;
			ny += 10;
			break;
		case GLUT_WHEEL_DOWN: // 마우스 휠 다운
			printf("wheel down! \n");
			cameraMove = true;
			nx -= 10;
			ny -= 10;
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
	if (keyPressed[UP] || keyPressed[DOWN] || keyPressed[LEFT] || keyPressed[RIGHT]) {
		carMove = true;
		rotateCar = rotateCar % 360;
	}

	if (keyPressed[LEFT]) {
		rotateCar = rotateCar + 2;

		/*
		cout <<"rotate : " << rotateCar << endl;
		cout << "xp : " << xp << endl;
		cout << "zp : " << zp << endl;
		cout << "carLocationX : " << carLocationX << endl;
		cout << "carLocationZ : " << carLocationZ << endl;
		//		carLocationX = carLocationX - ((float)(tan(rotateCar) * 2));
		isLeft = true;
		*/
		//printf("left \n");
	}
	if (keyPressed[RIGHT]) {
		rotateCar = rotateCar - 2;
		//carLocationX = carLocationX + (rotateCar / 2);
		//carLocationX = carLocationX + ((float)(tan(rotateCar) * 2));
		isRight = true;

		//printf("right \n");
	}

	if (keyPressed[UP]) {  // up은 -
		speed += 1;
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
		//printf("%.2f \n", carLocationZ);

		/*
		printf("%.2f \n", speed);
		if (rotateCar > 90 && rotateCar < 270) {
			carLocationZ = carLocationZ + speed;
		}
		else {
			//carLocationZ = carLocationZ - ((float)(tan(rotateCar) * speed));
			carLocationZ = carLocationZ - speed;
		}
		if(carLocationX != 0)
			carLocationX = carLocationX - ((float)(tan(rotateCar) * speed));
		isFront = true;
		*/
		//printf("up \n");
	}
	if (keyPressed[DOWN]) {
		speed -= 1;
		//printf("%.2f \n", speed);

		/* 진행 방향에 따른 위치 이동 */
		carLocationX = carLocationX + (xp * speed);
		carLocationZ = carLocationZ + (zp * speed);
		/*
		if (rotateCar > 90 && rotateCar < 270) {
			carLocationZ = carLocationZ - speed;
		}
		else {
			carLocationZ = carLocationZ + speed;
		}
		carLocationZ = carLocationZ + 15;
		if (carLocationX != 0) {
			carLocationX = carLocationX + ((float)(tan(rotateCar) * speed));
		}
		*/
		isBack = true;
		
		//printf("down \n");
	}
	//if (cam.eye.y<terrain->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }// 간단한 Colision Detection 지표면 아래로 카메라가 내려갈때는 지표면 위로 유지시킴
	
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
		/*
	case GLUT_KEY_UP: // 앞으로 이동
		keyPressed[72] = true;
		//nz += 50;
		break;
	case GLUT_KEY_DOWN: // 뒤로 이동
		keyPressed[80] = true;
		
		//nz -= 50;
		break;
	case GLUT_KEY_LEFT: // 왼쪽 이동
		keyPressed[75] = true;
		//nx += 50;
		break;
	case GLUT_KEY_RIGHT: // 오른쪽 이동
		keyPressed[77] = true;
		//nx -= 50;
		break;
		*/
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
	terrain = new Terrain("map2.raw", "down.bmp", 1024, 1024);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}