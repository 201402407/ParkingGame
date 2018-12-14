#include "include.h"
#include "ground.h"
#include <Windows.h>
#include <atlbase.h>
#include <iostream>

Ground::Ground(pcStr heightFile, pcStr surfaceTexFile, GLuint width, GLint height) {

	USES_CONVERSION; // 매크로 선언

	_map_width = width;// 맵 가로세로 크기 설정
	_map_height = height;

	img = NULL;// 물과 지형의 비트맵이미지 포인터 초기화
	_texId_ground = 0;//텍스쳐 id값
	_height = 0;

	/* 지형텍스쳐 세팅 및 설정 */
	wchar_t text[50];
	mbstowcs(text, surfaceTexFile, strlen(surfaceTexFile) + 1);
	LPCWSTR LPsurfaceTexFile = text;

	img = auxDIBImageLoad(W2A(LPsurfaceTexFile));//지형텍스쳐이미지 로드
	glGenTextures(1, &_texId_ground);// 텍스쳐 만들기
	glBindTexture(GL_TEXTURE_2D, _texId_ground); //텍스쳐id와 연결
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);//2d텍스쳐이미지설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// 매핑될물체가 작은경우 텍스쳐 선형보간 방법
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// 매핑물체가 클경우 텍스쳐의 확장 방법을 결정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	_height = readRawData(heightFile, width, height);// 파일로부터 가로세로만큼의 높이 데이터를 읽어옴
	
	// 지형의 최소값 구함
	int i;
	for (i = 0, minHeight = 2 ^ 10; i<_map_width*height; i++) {
		if (minHeight>_height[i]) { minHeight = _height[i];
		cout << minHeight << endl;
		}
	}
	cout << "asgag" << endl;
	// 지형 타일링하기
	baseGround = glGenLists(1);// 4단계의 지형 타일, 물 5개의 디스플레이리스트
	glNewList(baseGround, GL_COMPILE);
	TileGround(1);//detail level1 지형
	glEndList();

}

Ground::~Ground() {
	// Ground객체의 소멸자 메모리를 반환한다
	if (_height != 0) { delete[] _height; }
	if (_texId_ground != 0) { glDeleteTextures(1, &_texId_ground); }
	if (img != 0) { free(img->data); free(img); }
}

void Ground::RenderGround(GLfloat x, GLfloat y) {
	int i, j;
	// 타일링한 지형 디스플레이리스트들을 불러오는 함수
	//detail level1 지형
	//카메라가 위치한 곳에 한 블럭만 그린다.
	glPushMatrix();
	glTranslatef(getTileX(x)*(_map_width - 1), 0.0, getTileY(y)*(_map_height - 1));
	glCallList(baseGround);
	glPopMatrix();
}
void Ground::TileGround(GLint levelOfDetail) { //levelOfDetail파라미터를 받아 각 타일을 그리는 함수
	glEnable(GL_TEXTURE_2D); //텍스쳐활성화
	glMatrixMode(GL_MODELVIEW);//모델뷰행렬

	glPushMatrix();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//텍스쳐환경설정
	glBindTexture(GL_TEXTURE_2D, _texId_ground); //지형텍스쳐 id가 가리키는 텍스쳐로 아래 정점의 연결
	float coordX = (float)1.0 / _map_width; // 가로이동단위
	float coordZ = (float)1.0 / _map_height;// 세로이동단위
	for (int z = 0; z<_map_height; z += levelOfDetail) { // x,z축을 기준으로 그림 공간의 바닥에 매팽하기 위해
		glBegin(GL_TRIANGLE_STRIP);// 연속삼각형 그리기로 텍스쳐매핑
		for (int x = 0; x<_map_width; x += levelOfDetail) {
			glTexCoord2f(coordX*x, coordZ*z); // 텍스쳐정점과
			glVertex3f(x, _height[z*_map_height + x], z);// 물체정점 연결
			glTexCoord2f(coordX*x, coordZ*(z + levelOfDetail));// 텍스쳐정점과
			glVertex3f(x, _height[(z + levelOfDetail)*_map_height + x], z + levelOfDetail);// 물체정점 연결
		}
		glEnd();
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

// raw 파일을 읽는 함수
// filename: 높이맵 파일의 경로
// w : 높이맵의 가로 크기
// h : 높이맵의 세로 크기
Ground::ubyte* Ground::readRawData(pcStr filename, uint w, uint h) {
	FILE *f;
	fopen_s(&f, filename, "rb"); // 지형높이데이터 raw파일열기
	if (!f) {//파일이 없을경우
		printf("file not found : %s\n", filename);
		throw std::exception();
	}
	// 헤더가 없는 raw 파일의 경우,"파일의 크기=이미지의 픽셀수"이기
	// 때문에 width*height 크기 만큼의 공간을 할당하면 모든 데이터를 읽어올 수 있음
	ubyte* raw = new ubyte[w*h];// raw의 크기를 잡고
	fread(raw, sizeof(ubyte), w*h, f);// 높이데이터읽어오기
	fclose(f);
	return raw;
}
GLfloat Ground::getHeight(GLfloat _x, GLfloat _y) {
	//텍스쳐이미지의 해당 _x,_y에서 높이 데이터를 가지고 오는 함수
	int x, y;
	if (_x<0) x = _map_width + ((int)_x%_map_width);//텍스쳐이미지범위를 넘어가면 반대편좌표로
	else x = ((int)_x % _map_width);
	if (_y<0) y = _map_height + ((int)_y % _map_height);
	else y = ((int)_y % _map_height);
	// 좌표값을 텍스쳐크기안에 한정시킴
	// 높이 데이터는 1차원배열에 저장되어 있으므로 행렬처리
	return (_height[y*_map_height + x] / 5 + 10);// 해당위치의 높이값 반환
}
GLint Ground::getTileX(GLfloat x) {// 현재카메라가 위치하고 있는 텍스쳐 타일의 블럭좌표를 반환 : x좌표
	if (x>0) return ((int)x) / (int)_map_width;
	else return ((int)x) / (int)_map_width - 1;
}
GLint Ground::getTileY(GLfloat y) {//// 현재카메라가 위치하고 있는 텍스쳐 타일의 블럭좌표를 반환 : y좌표
	if (y>0) return ((int)y) / (int)_map_height;
	else return ((int)y) / (int)_map_height - 1;
}