#include "include.h"
#include "skybox.h"
#include "camera.h"
// 카메라 위치
float nx = 0;
float ny = 0;
float nz = 2;

GLUquadricObj *mercury, *venus, *earth, *moon, *mars, *jupiter, *saturn, *uranus, *neptune, *sun;
GLuint texmain[2]; // 메인 로고
GLuint tex[6]; //텍스쳐 맵핑 변수

GLUquadricObj *solar[3];
GLuint textureMapping[3]; //텍스쳐 맵핑 변수
AUX_RGBImageRec *textureBMPImg[3];

Skybox* skybox;	//스카이박스
Camera cam;	//카메라
CCamera objCamera;

char *BMPFileName[3] = { "posy.bmp", "backGR.bmp", "background.bmp" };

AUX_RGBImageRec *LoadBMPFile(char *filename) { // 이미지 읽어오기
	FILE *pFile = NULL;
	if (!filename) return NULL;
	pFile = fopen(filename, "r");

	if (pFile) {
		fclose(pFile);
		return auxDIBImageLoad(filename);
	}
	return NULL;
}

void Texture() {
	glGenTextures(3, &textureMapping[0]);
	for (int i = 0; i < 3; i++) {
		solar[i] = gluNewQuadric();
	//	gluQuadricDrawStyle(solar[i], GLU_FILL);
//		gluQuadricTexture(solar[i], GL_TRUE);
	//	gluQuadricOrientation(solar[i], GLU_INSIDE);
	//	gluQuadricNormals(solar[i], GLU_NONE);
		textureBMPImg[i] = LoadBMPFile(BMPFileName[i]);
		glBindTexture(GL_TEXTURE_2D, textureMapping[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, textureBMPImg[i]->sizeX, textureBMPImg[i]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, textureBMPImg[i]->data);
	}
	int i;
	for (i = 0; i < 3; i++) {
		if (textureBMPImg[i])                            // 텍스처가 존재하는지 확인
		{
			if (textureBMPImg[i]->data)                    // 텍스처 이미지가 존재하는지 확인
			{
				free(textureBMPImg[i]->data);                // 텍스처 이미지 메모리를 해제
			}

			free(textureBMPImg[i]);                        // 이미지 구조체를 해제
		}
	}
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

int LoadGLTextures() { //텍스쳐 맵핑
	int i;

	AUX_RGBImageRec *textureimg[6]; //그림 파일에서 맵핑 소스를 얻기 위해 사용할 함수의 반환값을 저장할 목적
	int status = FALSE;
	memset(textureimg, 0, sizeof(void *) * 1); // 메모리 상의 변수를 0으로 만드는 코드
	//각각의 그림 파일을 실제 읽어 들이는 조건문
	if ((textureimg[0] = LoadBMPFile("negz.bmp")) && (textureimg[1] = LoadBMPFile("posz.bmp"))
		&& (textureimg[2] = LoadBMPFile("posx.bmp")) && (textureimg[3] = LoadBMPFile("negx.bmp"))
		&& (textureimg[4] = LoadBMPFile("negy.bmp")) && (textureimg[5] = LoadBMPFile("posy.bmp"))) {
		glGenTextures(6, &tex[0]); // 첫 번째 인자 : 몇 개의 텍스쳐 맵핑 소스를 생성할 것인지.  두 번째 : 가장 첫 번째 텍스쳐 맵핑 소스 식별자 주소값.
		// 위의 GLuint[] 는 []개의 텍스쳐 사용

		for (i = 0; i<6; i++) {
			glBindTexture(GL_TEXTURE_2D, tex[i]); // 앞으로 설정할 텍스쳐의 식별자를 지정. 첫 번째 : 몇 차원인지. 두 번째: 다음에 설정할 텍스쳐 맵핑 식별자
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 보간법(GL_LINEAR)과 최근점법(GL_NEAREST)
			// MIN은 맵핑 소스가 물체보다 클 경우 , MAG는 그 반대.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 첫 번째: 몇 차원인지.
			glTexImage2D(GL_TEXTURE_2D, 0, 3, textureimg[i]->sizeX, textureimg[i]->sizeY,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureimg[i]->data);
			// 첫번째는 텍스쳐 이미지가 1차원인지 2차원. 두번째 인자는 항상 0-> MidMap. 세번째 인자는 생상 구성 요소의 갯수(RGB라 3)
			// 네번째와 다섯번째는 이미지의 가로 길이와 세로 길이. 여섯번째는 이미지의 경계에 대한 픽셀두께
			// 일곱번째는 텍스쳐 맵핑 소스의 구성 요소(어차피 그림파일 가져오므로 RGB)
			// 여덜번째는 색상 구성 요소들이 갖는 값의 타입을 나타내는데 모든 BMP파일에 있어서 RGB의 세가지 구성요소들은 부호 없는 Byte형이므로 GL_UNSIGNED_BYTE
			// 아홉번째는 텍스쳐 맵핑 소스의 실제 데이타값(그림 데이타)에 대한 포인터

		}
	}
	else
		return FALSE;

	// 할당된 메모리를 해제
	for (i = 0; i<6; i++) {
		if (textureimg[i]) {
			if (textureimg[i]->data)
				free(textureimg[i]->data);
			free(textureimg[i]);
		}
		else
			return FALSE;
	}

	glEnable(GL_TEXTURE_2D); //  2차원 텍스쳐 맵핑을 사용하겠노라고 OpenGL에게 알림
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// GL_MODULATE는 화면상의 픽셀 색과 텍스춰의 픽셀 색을 서로 곱하게 된다
	// GL_DECAL은 화면상의 픽셀 색을 텍스쳐의 픽셀 색으로 대체. 이 경우 빛에 대해서 전혀 텍스쳐가 영향을 받지 않는다
	// GL_BLEND는 화면상의 픽셀 색을 텍스쳐의 픽색 색과 섞고 특정한 색과 결합

	return TRUE;
}

/* 2D 평면에 픽셀맵 입히는 BMP파일 로드 */
GLubyte *LoadBmp(const char *Path, int *Width, int *Height)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER *fh = NULL;
	BITMAPINFOHEADER *ih;
	BYTE *pRaster;

	hFile = CreateFileA(Path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER *)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	int len = FileSize - fh->bfOffBits;
	pRaster = (GLubyte *)malloc(len);
	memcpy(pRaster, (BYTE *)fh + fh->bfOffBits, len);

	// RGB로 순서를 바꾼다.
	for (BYTE *p = pRaster; p < pRaster + len - 3; p += 3) {
		BYTE b = *p;
		*p = *(p + 2);
		*(p + 2) = b;
	}

	ih = (BITMAPINFOHEADER *)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	*Width = ih->biWidth;
	*Height = ih->biHeight;

	free(fh);

	return pRaster;
}

/* 출력하는 함수 */
void DoDisplay()
{
	GLubyte *data;
	int Width = 200;
	int Height = 200;
	glClear(GL_COLOR_BUFFER_BIT);
	data = LoadBmp("background.bmp", &Width, &Height);

	if (data != NULL) {
		glRasterPos2f(-0.5, -0.5);
		glDrawPixels(Width, Height, GL_RGB, GL_UNSIGNED_BYTE, data);
		free(data);
	}
}

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

void setBackground() {
	texture[0] = LoadTexture("background.bmp", 256, 256); //load the texture
	texture[1] = LoadTexture("skybox\front.jpg", 256, 256); //load the texture
	texture[2] = LoadTexture("skybox\left.jpg", 256, 256); //load the texture
	texture[3] = LoadTexture("skybox\right.jpg", 256, 256); //load the texture
	texture[4] = LoadTexture("skybox\bottom.jpg", 256, 256); //load the texture
	texture[5] = LoadTexture("skybox\top.jpg", 256, 256); //load the texture
	glEnable(GL_TEXTURE_2D); //enable 2D texturing
							 //glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
							 //glEnable(GL_TEXTURE_GEN_T);
	//skybox();
	for (int i = 0; i < 6; i++) {
		FreeTexture(texture[i]);
	}
}

GLfloat vertices[][3] = {
	{ -95.0, -95.0,  95.0 },   // 0  
	{ -95.0,  95.0,  95.0 },   // 1 
	{ 95.0,  95.0, 95.0 },   // 2 
	{ 95.0, -95.0, 95.0 },   // 3 
	{ -95.0, -95.0, -95.0 },   // 4 
	{ -95.0, 95.0, -95.0 },   // 5 
	{ 95.0, 95.0, -95.0 },   // 6 
	{ 95.0, -95.0, -95.0 } };  // 7 

GLfloat colors[][3] = {
	{ 1.0, 0.0, 0.0 },      // red 
	{ 1.0, 1.0, 0.0 },      // yellow 
	{ 0.0, 1.0, 0.0 },      // green 
	{ 0.0, 0.0, 1.0 },      // blue 
	{ 1.0, 1.0, 1.0 },      // white 
	{ 1.0, 0.0, 1.0 } };     // magenta 

void polygon(int a, int b, int c, int d)
{
	//glColor3fv(colors[a]);
	glBegin(GL_POLYGON);
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertices[a]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertices[b]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertices[c]);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertices[d]);
	glEnd();

}

// 6개의 면을 만든다. 
void createCube(void)
{
	polygon(0, 3, 2, 1);    // front 
	polygon(2, 3, 7, 6);    // right 
	polygon(3, 0, 4, 7);    // bottom 
	polygon(4, 5, 6, 7);    // back 
	polygon(1, 2, 6, 5);    // top 
	polygon(5, 4, 0, 1);    // right 
}

void display() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	//gluLookAt(nx, ny, nz, 0, 0, 0, 0, 1, 0);
	glPushMatrix();
	glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
	glPopMatrix();
	
	skybox->draw(); // 스카이박스 생성

	//createCube();   // 큐브 생성 

	//setBackground();
	//glutSolidSphere(1.2, 100, 20);
	//GLUquadricObj *sphere = gluNewQuadric();
	//gluQuadricTexture(sphere, GL_TRUE);
	//glPushMatrix();
	//glBindTexture(GL_TEXTURE_2D, tex[2]);

	/*
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-55.0f, -55.0f, -55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, -55.0f, -55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[3]); // 뒤
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, -55.0f, 55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-55.0f, -55.0f, 55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[4]); // 아래
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(55.0f, -55.0f, 55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-55.0f, -55.0f, -55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-55.0f, -55.0f, 55.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, -55.0f, -55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[5]); // 위
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, 55.0f, -55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, 55.0f, 55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[1]); //
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-55.0f, -55.0f, 55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-55.0f, -55.0f, -55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[0]); // 왼
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(55.0f, -55.0f, -55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(55.0f, 55.0f, -55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(55.0f, 55.0f, 55.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(55.0f, -55.0f, 55.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, tex[2]); // 정면

	*/
	//glColor3f(0.5, 0.5, 0.5);
	//glTexCoord4f(0.0, 0.0, 1.0, 1.0);
	//glutSolidSphere(1.2, 20, 20);
	//gluSphere(solar[0], 4, 50, 30);
	//DrawGLScene();
	//glPopMatrix();
	//glBindTexture(GL_TEXTURE_2D, texmain[0]);
	//glColor3f(0.5, 0.0, 0.0);
	//DoDisplay();
	//glRectd(5.0, 5.0, -5.0, -5.0);
	// 더블버퍼의 버퍼를 교체하여 화면을 갱신
	glutSwapBuffers();

	glFlush();

}

void pressKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		ny += 0.05;
		break;
	case GLUT_KEY_DOWN:
		ny -= 0.05;
		break;
	case GLUT_KEY_LEFT:
		nx += 0.05;
		break;
	case GLUT_KEY_RIGHT:
		nx -= 0.05;
		break;
	case GLUT_KEY_PAGE_UP:
		nz += 0.05;
		break;
	case GLUT_KEY_PAGE_DOWN:
		nz -= 0.05;
		break;
	}
}

void reshape(int w, int h) {
	float ratio = w / (float)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 10, 100);
}

void idle_func(void)
{
	
	glutPostRedisplay();
}
#define glCheckError() _glCheckError(__FILE__, __LINE__)
const char* glGetErrorString(GLenum error)
{
	switch (error)
	{
	case GL_NO_ERROR:          return "No Error";
	case GL_INVALID_ENUM:      return "Invalid Enum";
	case GL_INVALID_VALUE:     return "Invalid Value";
	case GL_INVALID_OPERATION: return "Invalid Operation";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid Framebuffer Operation";
	case GL_OUT_OF_MEMORY:     return "Out of Memory";
	case GL_STACK_UNDERFLOW:   return "Stack Underflow";
	case GL_STACK_OVERFLOW:    return "";
	case GL_CONTEXT_LOST:      return "Context Lost";
	default:                   return "Unknown Error";
	}
}

void _glCheckError(const char *filename, int line)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("OpenGL Error: %s (%d) [%u] %s\n", filename, line, err, glGetErrorString(err));
}
void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	// 윈도우설정
	glutInitWindowSize(1000, 600);
	glutCreateWindow("Parking-Game!");
	skybox = new Skybox();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 이거해야 가운데 흰점 안생김.
	glColor3f(1.0, 1.0, 1.0);
	LoadGLTextures();
	//DrawGLScene();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glDepthFunc(GL_LESS);
	glutSpecialFunc(pressKey);
	// 이벤트가 없을때 처리 함수를 등록
	glutIdleFunc(idle_func);
	//loadMainGLTextures();
	// in init of main after create window
	/* Auto-generation of texture coordinates */
	GLfloat plane_coef_s[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat plane_coef_t[] = { 0.0, 1.0, 0.0, 1.0 };
	//Texture();

	// 뷰포트와 카메라 설정
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0);
	cam.setShape(60.0f, 64.0f / 48.0f, 0.5f, 1000.0f);

	// 카메라의 초기위치와 회전각
	// 설정을 마친 카메라가 처음에 어디에서 어디를 보고있을지 결정
	cam.slide(0, 100, 0);
	cam.roll(0);
	cam.yaw(180);
	cam.pitch(45);

	// 깊이버퍼, 후면제거활성화
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCheckError();
	glutMainLoop();
}