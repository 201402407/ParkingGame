#pragma once
class Bus {
public:
	Bus();
	~Bus();
	void drawBus();
	void drawWall();
	AUX_RGBImageRec *textureImg[6];
private:
#define PI 3.1492
	typedef unsigned int uint;
	typedef const char * pcStr;
	uint textures[8];
	uint texWall;
	static uint loadTexture(pcStr filename);
	void busRight();
	void busLeft();
	void busTop();
	void busFront();
	void busBack();
	void busBottom();

	void wallRight();
	void wallLeft();
	void wallTop();
	void wallFront();
	void wallBack();
	void wallBottom();
};

