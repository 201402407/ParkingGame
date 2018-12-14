#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LIST            "list.txt" //바꿀 bitmap파일 목록 cmd -> dir/b > list.txt 
#define INBITMAP        "in\\"     //bitmap파일들의 경로 
#define OUTROW          "out\\"    //변환된 raw파일의 출력 경로 
#define CFILE           "c\\"      //.c파일들이 들어갈 디렉토리 경로 

#define LIST_WIDTH_HEIGHT   "result.txt" //파일목록 + width + height 정보가 저장 

#define WORD    unsigned short       //2byte
#define DWORD   unsigned int         //4byte
#define LONG    unsigned int         //4byte
#define BYTE    unsigned char        //1byte

#define WIDTHBYTES(bits)    (((bits)+31)/32*4)  //영상의 가로줄은 4바이트의 배수
//width가 늘어나면 쓸대 없는 값 저장 됨.. bitmap에서
//raw로 바꿀때는 늘어난 width는 제외 하고 저장해야 함.. 

//비트맵 파일에 대한 정보(파일 헤드) 
typedef struct tagBITMAPFILEHEAD {
	WORD    bfType;         //"BM"이라는 값을 저장함
	DWORD   bfSize;         //byte단위로 전체 파일 크기
	WORD    bfReserved1;    //예약된 변수
	WORD    bfReserved2;    //예약된 변수
	DWORD   bfOffBits;      //영상 데이터 위치까지의 거리
}   BITMAPFILEHEADER;
//영상 자체에 대한 정보(영상 헤드)
typedef struct tagBITMAPINFOHEADER {
	DWORD   biSize;             //영상 헤드의 크기 
	LONG    biWidth;            //픽셀 단위로 영상의 폭 
	LONG    biHeight;           //영상의 높이 
	WORD    biplanes;           //비트 플레인 수(항상 1)
	WORD    biBitCount;         //피셀당 비트수(컬러, 흑백 구별) 여기서는 24만 처리
	DWORD   biCompression;      //압축 유무
	DWORD   biSizeImage;        //영상의 크기 
	LONG    biXPelsPerMeter;    //가로 해상도 
	LONG    biYPelsPerMeter;    //세로 해상도
	DWORD   biClrUsed;          //실제 사용 색상수
	DWORD   biClrImportant;     //중요한 색상 인덱스 
} BITMAPINFOHEADER;
//팔레트(인덱스에 의한 컬러값을 저장하기 위한 구조체)
//256칼라모드 영상은 팔레트 배열 크기 256, 트루 칼라의 경우 인덱스 저장이 아니라 직접 저장 
//그러므로 여기서는 트루칼라만 다루므로 이 구조체는 사용하지 않는다.
typedef struct tagRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbreserved1;   //예약된 변수 
}   RGBQUAD;

void Bitmap24ToRaw();
void MakeC(char *filename);
FILE *in, *out, *listout; //비트맵 파일, 바꿀 raw파일, .c목록 파일 
int main()
{
	FILE    *list;
	//FILE *in, *out;
	char file[256], filename[256];
	unsigned int i;

	char line[256];
	list = fopen(LIST, "r");

	listout = fopen(LIST_WIDTH_HEIGHT, "w");

	while (fscanf(list, "%s", line) != EOF) {
		printf("start : %s\n", line);

		strcpy(filename, line);
		for (i = 0; i<strlen(filename); i++)
			if (filename[i] == '.')
				break;
		filename[i] = '\0';  //파일명만 빼내기 

		strcpy(file, INBITMAP);
		strcat(file, line);
		if ((in = fopen(file, "rb")) == NULL) {
			printf("input file(%s) open fail!", file); exit(1);
		}

		strcpy(file, OUTROW);
		strcat(file, filename);
		strcat(file, ".raw");

		printf("out filename : %s \n", file);
		if ((out = fopen(file, "wb")) == NULL) {
			printf("output file(%s) open fail!", file); exit(2);
		}

		fprintf(listout, "%s.c", filename);
		Bitmap24ToRaw();

		fclose(in);
		fclose(out);

		MakeC(line);
	}
	fclose(list);
	fclose(listout);
	return 0;
}
void Bitmap24ToRaw()
{
	BITMAPFILEHEADER hf;    //파일정보 헤드 
	BITMAPINFOHEADER hInfo; //영상정보 헤드
	BYTE *bmp;
	int i, j;

	fread(&hf.bfType, sizeof(WORD), 1, in);
	fread(&hf.bfSize, sizeof(DWORD), 1, in);
	fread(&hf.bfReserved1, sizeof(WORD), 1, in);
	fread(&hf.bfReserved2, sizeof(WORD), 1, in);
	fread(&hf.bfOffBits, sizeof(DWORD), 1, in);
	//printf("%x %lx %x %x %lx",hf.bfType, hf.bfSize, hf.bfReserved1, hf.bfReserved2, hf.bfOffBits);
	if (hf.bfType != 0x4D42) {  //"BM"
		printf("비트맵 파일이 아니다.\n"); exit(1);
	}
	fread(&hInfo.biSize, sizeof(DWORD), 1, in);
	fread(&hInfo.biWidth, sizeof(LONG), 1, in);
	fread(&hInfo.biHeight, sizeof(LONG), 1, in);
	fread(&hInfo.biplanes, sizeof(WORD), 1, in);
	fread(&hInfo.biBitCount, sizeof(WORD), 1, in); //나머지는 안읽음 .. 필요 없음..

	fprintf(listout, "\t%d\t%d\n\n", hInfo.biWidth, hInfo.biHeight);

	int width = WIDTHBYTES(24 * hInfo.biWidth); //bitmap파일의 width바이트 수를 다시 계산 

	bmp = (BYTE *)malloc(width*hInfo.biHeight * 3);

	fseek(in, hf.bfOffBits, SEEK_SET);

	for (i = 0; i<hInfo.biHeight; i++)
		for (j = 0; j<width; j++)
			fread(&bmp[i*width + j], 1, 1, in);

	for (i = hInfo.biHeight - 1; i >= 0; i--)
		for (j = 0; j<hInfo.biWidth * 3; j += 3) {
			fwrite(&bmp[i*width + j + 2], 1, 1, out);
			fwrite(&bmp[i*width + j + 1], 1, 1, out);
			fwrite(&bmp[i*width + j], 1, 1, out);
		}
	delete[] bmp;
	//파일 목록 만들기  
}
void MakeC(char *filename)
{
	FILE *fin, *fout;
	char file[256], array_name[256];
	unsigned int i, j, tmp, size, num = 0;
	unsigned char cr, cg, cb;

	strcpy(file, OUTROW);
	strcat(file, filename);
	for (i = 0; i<strlen(file); i++)
		if (file[i] == '.') break;
	file[i] = '\0';
	strcat(file, ".raw");

	if ((fin = fopen(file, "rb")) == NULL) {
		printf("%s file read fail!!\n", file);
		exit(1);
	}

	strcpy(file, CFILE);
	strcat(file, filename);
	for (i = 0; i<strlen(file); i++)
		if (file[i] == '.') break;
	file[i] = '\0';

	strcat(file, ".c");
	if ((fout = fopen(file, "w")) == NULL) {
		printf("%s file write fail!!\n", file);
		exit(2);
	}

	strcpy(array_name, "img_");
	strcat(array_name, filename);
	for (i = 0; i<strlen(array_name); i++)
		if (array_name[i] == '.') break;
	array_name[i] = '\0';

	fprintf(fout, "/**************************************************************************/\n");
	fprintf(fout, "/* Header file containing C5x generated from BIN2ARAY.EXE                 */\n");
	fprintf(fout, "/* version : 0727_좌표수정                                                */\n");
	fprintf(fout, "/* Made by OpenEyes Communication                                         */\n");
	fprintf(fout, "/**************************************************************************/\n");

	fseek(fin, 0L, SEEK_END);
	size = ftell(fin);
	rewind(fin);

	fprintf(fout, "\nconst unsigned short %s[] = { /* size=%d */\n", array_name, size);

	for (j = 0, i = 1; j<(size / 3); j++, i++)
	{
		fread(&cr, 1, 1, fin);
		fread(&cg, 1, 1, fin);
		fread(&cb, 1, 1, fin);

		//tmp = (c2<<8) | c1;
		//5: 6 : 5
		tmp = ((cr & 0xf8) << 8) | ((cg & 0xfc) << 3) | ((cb & 0xf8) >> 3);
		//fwrite((unsigned short *)&tmp, 1, 2, fout);

		fprintf(fout, "0x%04X, ", tmp);
		num += 2;

		if (i == 16) {
			fprintf(fout, "\n");
			i = 0;
		}
	}
	fprintf(fout, "\n};\n");
	printf("num=%d\n", num);
	fclose(fin);
	fclose(fout);

	printf("\nEnd of make DSP load image table\n");
	printf("**************************************************\n\n");

}