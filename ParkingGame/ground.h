#include "include.h"


class Ground {
private:
	typedef const char* pcStr;
	typedef unsigned int uint;
	typedef unsigned char ubyte;

	uint _texId_ground;//지형텍스쳐와 물텍스쳐의 id
	AUX_RGBImageRec *img;//텍스쳐이미지 객체와 연결되는 포인터
	ubyte* _height; //지형높이 배열과 연결하는 포인터
	GLint _map_width, _map_height; //단위지형의 가로세로크기
	GLuint baseGround; //타일의 인덱스값
	GLubyte minHeight; // 지형데이터의 최소높이
	static ubyte* readRawData(pcStr filename, uint width, uint height); // 지형데이터읽어오기

public:
	Ground(pcStr heightmap, pcStr surfaceTex, GLuint width, GLint height);//지형생성자
	~Ground(void);//소멸자
	void RenderGround(GLfloat, GLfloat);//지형그리기
	void TileGround(GLint);//LoD를 위한 타일링
	GLfloat getHeight(GLfloat, GLfloat);//해당 위치의 높이구하기
	GLint getTileX(GLfloat); // 해당위치의 x타일구하기
	GLint getTileY(GLfloat);// 해당위치의 y타일구하기	
};