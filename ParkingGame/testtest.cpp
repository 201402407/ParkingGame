#include "include.h"
#include <fstream>
#include <assert.h>

#include "camera.h"
#include "heightfield.h"
#include "skybox.h"
#include "car.h"
#include "terrain.h"
#pragma comment(lib,"glew32.lib")

// 카메라 위치
float nx = 0;
float ny = 800;
float nz = 1000;

float xpos = 85.078, ypos = 351.594, zpos = 281.033, xrot = 758, yrot = 90, angle = 0.0;
float lastx, lasty;

float bounce;
float cScale = 1.0;

bool keyPressed[256];	//키보드입력상황배열
void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }
//해당배열이 true면 Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }
//해당배열이 false면 Key Up

SwiftHeightField hField;
Skybox* skybox;	//스카이박스
Camera cam;	//카메라
CCamera objCamera;
Terrain* terrain;	//지형
Car* car; // 자동차

void camera(void) {
	int posX = (int)xpos;
	int posZ = (int)zpos;

	//glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);
	glTranslated(-xpos, -ypos, -zpos);
	printf("%.2f , %.2f , %.2f \n", xpos, ypos, zpos);
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
	gluLookAt(nx, ny, nz, 0, 0, 0, 0, 1, 0);
	/* 기본 설정 */
	glShadeModel(GL_SMOOTH);
	// 깊이버퍼, 후면제거활성화
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);

	/* 배경 생성 */
	setBackGround();

	/* 바닥 생성 */
	setGround();
	
	/* 자동차 생성 */
	glTranslatef(0.0, 420.0, 480.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.8, 1.5, 1.5);
	car->drawCar();
	
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

void keyboard(unsigned char key, int x, int y) {

	if (key == 'w')
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos += float(sin(yrotrad)) * cScale;
		zpos -= float(cos(yrotrad)) * cScale;
		ypos -= float(sin(xrotrad));
		bounce += 0.04;
	}

	if (key == 's')
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos -= float(sin(yrotrad)) * cScale;
		zpos += float(cos(yrotrad)) * cScale;
		ypos += float(sin(xrotrad));
		bounce += 4.04;
	}

	if (key == 'd')
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos += float(cos(yrotrad)) * cScale;
		zpos += float(sin(yrotrad)) * cScale;
	}

	if (key == 'a')
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xpos -= float(cos(yrotrad)) * cScale;
		zpos -= float(sin(yrotrad)) * cScale;
	}

}

void Idle() {//해당키가 눌려있는지 지속적으로 검사해 다중입력을 할수 있게 한다
	if (keyPressed['1'])glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])glPolygonMode(GL_FRONT, GL_FILL);
	if (keyPressed['d']) { cam.slide(0.2, 0, 0); }
	if (keyPressed['a']) { cam.slide(-0.2, 0, 0); }
	if (keyPressed['s']) { cam.slide(0, 0, 1.0); }
	if (keyPressed['w']) { cam.slide(0, 0, -1.0); }
	if (cam.eye.y<terrain->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }// 간단한 Colision Detection 지표면 아래로 카메라가 내려갈때는 지표면 위로 유지시킴
	if (keyPressed['i']) { cam.pitch(-0.5); }
	if (keyPressed['k']) { cam.pitch(0.5); }
	if (keyPressed['q']) { cam.yaw(-0.5); }
	if (keyPressed['e']) { cam.yaw(0.5); }
	if (keyPressed['j']) { cam.roll(0.5); }
	if (keyPressed['l']) { cam.roll(-0.5); }
	else { cam.slide(0, 0, 0); } //아무것도 눌리지 않았을때는 이동없다고 보고 계속 모델뷰 행렬을 유지시켜준다.
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
	switch (key) {
	case GLUT_KEY_F1:
		nx += 1;
		//cam.pitch(0.5);
		break;
	case GLUT_KEY_F2:
		nx -= 1;
		//cam.pitch(-0.5);
		break;
	case GLUT_KEY_F3:
		ny += 150;
		//cam.yaw(0.5);
		break;
	case GLUT_KEY_F4:
		ny -= 50;
		//cam.yaw(-0.5);
		printf("F4");
		break;
	case GLUT_KEY_F5:
		nz += 1;
		//cam.roll(0.5);
		break;
	case GLUT_KEY_F6:
		nz -= 1;
		//cam.roll(-0.5);
		break;
	case GLUT_KEY_UP: // 앞으로 이동
		nz += 50;
		break;
	case GLUT_KEY_DOWN: // 뒤로 이동
		nz -= 50;
		break;
	case GLUT_KEY_LEFT: // 왼쪽 이동
		nx += 50;
		break;
	case GLUT_KEY_RIGHT: // 오른쪽 이동
		nx -= 50;
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
	glutDisplayFunc(display);
	glutIdleFunc(Idle);
	glutReshapeFunc(reshape);
	//glutKeyboardFunc(keyboard);
	//glutPassiveMotionFunc(mouseMovement);
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp);

	// 자동차 객체
	car = new Car();
	skybox = new Skybox();
	// Terrain, Skybox객체
	terrain = new Terrain("map2.raw", "down.bmp", 1024, 1024);
	// 뷰포트와 카메라 설정 (카메라 설정 함수 내부에 이미 PERSPECTIVE와 MODELVIEW 설정을 할 수 있게 했다.)
	/*
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0);
	cam.setShape(120.0f, 64.0f / 48.0f, 0.5f, 2000.0f);
	printf("%.2f, %.2f, %.2f", cam.eye.x, cam.eye.y, cam.eye.z);
	// 카메라의 초기위치와 회전각
	// 설정을 마친 카메라가 처음에 어디에서 어디를 보고있을지 결정
	cam.slide(0, 100, 0);
	cam.roll(0);
	cam.yaw(10);
	cam.pitch(45);
	*/
	glutSpecialFunc(pressKey);

	glutMainLoop();
	return 0;
}