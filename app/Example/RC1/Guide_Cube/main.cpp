//=======================================================================
//
// This confidential and proprietary software may be used only as
// authorized by a licensing agreement from Siliconarts Inc.
//
//   (C) Copyright 2010 ~ 2013  Siliconarts Inc. ALL RIGHTS RESERVED
//
// The entire notice above must be reproduced on all authorized
// copies and copies may only be made to the extent permitted
// by a licensing agreement from Siliconarts.
//
// Project : Example
// DATE    : 6/4/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "RCFramework.h"

// Programming Guide - Cube Object

typedef struct RGBImageRec {
	int sizeX, sizeY;
	unsigned char *data;
} RGBImageRec;

typedef struct bmpBITMAPFILEHEADER{
	WORD   bfType;
	DWORD  bfSize;
	WORD   bfReserved1;
	WORD   bfReserved2;
	DWORD  bfOffBits;
}BITMAPFILEHEADER1;

typedef struct bmpBITMAPINFOHEADER{
	DWORD  biSize;
	DWORD  biWidth;
	DWORD  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD  biCompression;
	DWORD  biSizeImage;
	DWORD  biXPelsPerMeter;
	DWORD  biYPelsPerMeter;
	DWORD  biClrUsed;
	DWORD  biClrImportant;
}BITMAPFILEHEADER2;

RGBImageRec *DIBImageLoad(char* path, int channel) {
	RGBImageRec* pImage=NULL;
	FILE *f=NULL;
	unsigned char *pBuf=NULL;
	int dataSize=0;
	int index=0;
	DWORD x=0;
	DWORD y=0;
	int bpp=0;

	if(channel !=3 && channel !=4) return pImage;

	f = fopen(path, "rb");
	if(f != NULL) {
		BITMAPFILEHEADER1 HD1;
		BITMAPFILEHEADER2 HD2;

		fseek(f,0,SEEK_SET);
		fread(&HD1.bfType, sizeof(WORD), 1, f);
		fread(&HD1.bfSize,sizeof(DWORD), 1, f);
		fseek(f, 10, SEEK_SET);
		fread(&HD1.bfOffBits, sizeof(int),1,  f);

		fread(&HD2.biSize, sizeof(int), 1, f);
		fread(&HD2.biWidth, sizeof(int), 1, f);
		fread(&HD2.biHeight, sizeof(int), 1, f);
		fread(&HD2.biPlanes, sizeof(WORD), 1, f);
		fread(&HD2.biBitCount, sizeof(WORD), 1, f);
		fread(&HD2.biCompression, sizeof(int), 1, f);

		fseek(f,HD1.bfOffBits,SEEK_SET);

		bpp = HD2.biBitCount/8;
		if(bpp == 1 || bpp == channel)
		{
			pBuf = (unsigned char*) malloc(channel);

			pImage = (RGBImageRec*) malloc(sizeof(RGBImageRec));
			pImage->sizeX = HD2.biWidth;
			pImage->sizeY = HD2.biHeight;

			dataSize = HD2.biWidth*HD2.biHeight*channel;
			pImage->data = (unsigned char*) malloc(dataSize);

			for(y=0; y<HD2.biHeight; y++) {
				for(x=0; x<HD2.biWidth; x++) {
					fread(pBuf, bpp, 1, f);
					if(bpp == 1) {
						pBuf[1] = pBuf[2] = pBuf[0];
						if(channel == 4)	pBuf[3] = 0;
					}

					index = (y*HD2.biWidth + x)*channel;
					pImage->data[index]		= pBuf[2];
					pImage->data[index + 1] = pBuf[1];
					pImage->data[index + 2] = pBuf[0];
					if(channel == 4)
						pImage->data[index + 3] = pBuf[3];
				}
			}

			if(pBuf) free(pBuf);
			pBuf = NULL;
		}

		fclose(f);
	}

	return pImage;
}

#define Blender         0
#define Coffeematic     1
#define Mug             2
#define PieTin          3
#define StandMixer      4
#define Toaster         5

unsigned int TextureArray[6];
unsigned int MaterialIDs[6];

float g_fSpinZ = -30.0f;
float g_fSpinY = -10.0f;

struct Color {
	float r, g, b;
};

struct Pos {
	float x, y, z;
};

struct TexC {
	float u, v;
};

struct Pos g_cubePos[] =
{
	// Quad 0
	{ -1.0f,-1.0f, 1.0f }, // 0 (unique)
	{  1.0f,-1.0f, 1.0f }, // 1 (unique)
	{  1.0f, 1.0f, 1.0f }, // 2 (unique)
	{ -1.0f, 1.0f, 1.0f }, // 3 (unique)

	// Quad 1
	{ -1.0f,-1.0f,-1.0f }, // 4 (unique)
	{ -1.0f, 1.0f,-1.0f }, // 5 (unique)
	{  1.0f, 1.0f,-1.0f }, // 6 (unique)
	{  1.0f,-1.0f,-1.0f }, // 7 (unique)

	// Quad 2
	{ -1.0f, 1.0f,-1.0f }, // 5 (start repeating here)
	{ -1.0f, 1.0f, 1.0f }, // 3 (repeat of vertex 3)
	{  1.0f, 1.0f, 1.0f }, // 2 (repeat of vertex 2... etc.)
	{  1.0f, 1.0f,-1.0f }, // 6

	// Quad 3
	{ -1.0f,-1.0f,-1.0f }, // 4
	{  1.0f,-1.0f,-1.0f }, // 7
	{  1.0f,-1.0f, 1.0f }, // 1
	{ -1.0f,-1.0f, 1.0f }, // 0

	// Quad 4
	{  1.0f,-1.0f,-1.0f }, // 7
	{  1.0f, 1.0f,-1.0f }, // 6
	{  1.0f, 1.0f, 1.0f }, // 2
	{  1.0f,-1.0f, 1.0f }, // 1

	// Quad 5
	{ -1.0f,-1.0f,-1.0f }, // 4
	{ -1.0f,-1.0f, 1.0f }, // 0
	{ -1.0f, 1.0f, 1.0f }, // 3
	{ -1.0f, 1.0f,-1.0f }  // 5
};

struct TexC g_cubeTexs[] =
{
	// Quad 0
	{  0.0f, 0.0f }, // 0 (unique)
	{  1.0f, 0.0f }, // 1 (unique)
	{  1.0f, 1.0f }, // 2 (unique)
	{  0.0f, 1.0f }, // 3 (unique)

	// Quad 1
	{  0.0f, 0.0f }, // 4 (unique)
	{  0.0f, 1.0f }, // 5 (unique)
	{  1.0f, 1.0f }, // 6 (unique)
	{  1.0f, 0.0f }, // 7 (unique)

	// Quad 2
	{  0.0f,  0.0f }, // 5 (start repeating here)
	{  0.0f,  1.0f }, // 3 (repeat of vertex 3)
	{  1.0f,  1.0f }, // 2 (repeat of vertex 2... etc.)
	{  1.0f,  0.0f }, // 6

	// Quad 3
	{  0.0f, 0.0f }, // 4
	{  1.0f, 0.0f }, // 7
	{  1.0f, 1.0f }, // 1
	{  0.0f, 1.0f }, // 0

	// Quad 4
	{  0.0f, 0.0f }, // 7
	{  1.0f, 0.0f }, // 6
	{  1.0f, 1.0f }, // 2
	{  0.0f, 1.0f }, // 1

	// Quad 5
	{  0.0f, 0.0f }, // 4
	{  0.0f, 1.0f }, // 0
	{  1.0f, 1.0f }, // 3
	{  1.0f, 0.0f }  // 5
};

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Programming Guide - Cube Object", 800, 480){}
	virtual ~Tutorial(void){}

	void Load(RGBImageRec **pBitmap){

		pBitmap[0] = (RGBImageRec *)DIBImageLoad("./scenedata/Guide_Cube/Blender.bmp", 3);	
		pBitmap[1] = (RGBImageRec *)DIBImageLoad("./scenedata/Guide_Cube/Coffeematic.bmp", 3);	
		pBitmap[2] = (RGBImageRec *)DIBImageLoad("./scenedata/Guide_Cube/Mug.bmp", 3);	
		pBitmap[3] = (RGBImageRec *)DIBImageLoad("./scenedata/Guide_Cube/PieTin.bmp", 3);	
		pBitmap[4] = (RGBImageRec *)DIBImageLoad("./scenedata/Guide_Cube/StandMixer.bmp", 3);	
		pBitmap[5] = (RGBImageRec *)DIBImageLoad("./scenedata/Guide_Cube/Toaster.bmp", 3);	
	}

	void CreateTexture(void) {
		RGBImageRec *pBitmap[6];
		int i=0;

		for(i=0; i<6; i++) {
			pBitmap[i] = NULL;
		}
		Load(pBitmap);

		rcGenTextures(6, TextureArray);

		rcBindTexture(RC_TEXTURE_2D, TextureArray[Blender]);
		rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[Blender]->sizeX, pBitmap[Blender]->sizeY, 
						0, RC_RGB, RC_UNSIGNED_BYTE, 
						pBitmap[Blender]->data);

		rcBindTexture(RC_TEXTURE_2D, TextureArray[Coffeematic]);
		rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[Coffeematic]->sizeX, pBitmap[Coffeematic]->sizeY, 
						0, RC_RGB, RC_UNSIGNED_BYTE, 
						pBitmap[Coffeematic]->data);

		rcBindTexture(RC_TEXTURE_2D, TextureArray[Mug]);
		rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[Mug]->sizeX, pBitmap[Mug]->sizeY, 
						0, RC_RGB, RC_UNSIGNED_BYTE, 
						pBitmap[Mug]->data);

		rcBindTexture(RC_TEXTURE_2D, TextureArray[PieTin]);
		rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[PieTin]->sizeX, pBitmap[PieTin]->sizeY, 
						0, RC_RGB, RC_UNSIGNED_BYTE, 
						pBitmap[PieTin]->data);

		rcBindTexture(RC_TEXTURE_2D, TextureArray[StandMixer]);
		rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[StandMixer]->sizeX, pBitmap[StandMixer]->sizeY, 
						0, RC_RGB, RC_UNSIGNED_BYTE, 
						pBitmap[StandMixer]->data);

		rcBindTexture(RC_TEXTURE_2D, TextureArray[Toaster]);
		rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[Toaster]->sizeX, pBitmap[Toaster]->sizeY, 
						0, RC_RGB, RC_UNSIGNED_BYTE, 
						pBitmap[Toaster]->data);

		for(i=0; i<6; i++) {
			if(pBitmap[i]) {
				if(pBitmap[i]->data)	free(pBitmap[i]->data);
				free(pBitmap[i]);
			}
		}
	}

	void SetMaterial(void){
		rcEnable(RC_TEXTURE_2D);

		rcGenMaterials(1, &MaterialIDs[Blender]);
		rcBindMaterial(MaterialIDs[Blender]);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[Blender]);

		rcGenMaterials(1, &MaterialIDs[Coffeematic]);
		rcBindMaterial(MaterialIDs[Coffeematic]);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[Coffeematic]);

		rcGenMaterials(1, &MaterialIDs[Mug]);
		rcBindMaterial(MaterialIDs[Mug]);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[Mug]);

		rcGenMaterials(1, &MaterialIDs[PieTin]);
		rcBindMaterial(MaterialIDs[PieTin]);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[PieTin]);

		rcGenMaterials(1, &MaterialIDs[StandMixer]);
		rcBindMaterial(MaterialIDs[StandMixer]);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[StandMixer]);

		rcGenMaterials(1, &MaterialIDs[Toaster]);
		rcBindMaterial(MaterialIDs[Toaster]);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[Toaster]);
	}

protected:
	virtual BOOL OnIntialize(void){
		rcSceneAllInit();

		{
			rcClearColor( 0.0f, 0.0f, 0.0f, 1.0f );			
			rcViewport(0, 0, Width(), Height());

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();
			rcFrustumf(-0.9622504483f, 0.9622504483f, -0.577350269f, 0.577350269f, 1, 100);
			rcuLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

			CreateTexture();
			SetMaterial();
		}

		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;
		i++;

		{
			rcMatrixMode( RC_MODELVIEW );
			rcLoadIdentity();
			rcTranslatef( 0.0f, 0.0f, -3.0f );

			rcRotatef( -g_fSpinY, 0.0f, 1.0f, 0.0f );
			rcRotatef( -g_fSpinZ, 0.0f, 0.0f, 1.0f );

			rcEnableClientState(RC_VERTEX_ARRAY);
			rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

			rcVertexPointer(3, RC_FLOAT, 0, g_cubePos);
			rcTexCoordPointer(2, RC_FLOAT, 0, g_cubeTexs);

			rcBindMaterial(MaterialIDs[Blender]);
			rcEnable(RC_TEXTURE_2D);
			rcBindTexture(RC_TEXTURE_2D, TextureArray[Blender]);
			rcDrawArrays( RC_TRIANGLE_FAN, 0, 4 );

			rcBindMaterial(MaterialIDs[Coffeematic]);
			rcEnable(RC_TEXTURE_2D);
			rcBindTexture(RC_TEXTURE_2D, TextureArray[Coffeematic]);
			rcDrawArrays( RC_TRIANGLE_FAN, 4, 4 );

			rcBindMaterial(MaterialIDs[Mug]);
			rcEnable(RC_TEXTURE_2D);
			rcBindTexture(RC_TEXTURE_2D, TextureArray[Mug]);
			rcDrawArrays( RC_TRIANGLE_FAN, 8, 4 );

			rcBindMaterial(MaterialIDs[PieTin]);
			rcEnable(RC_TEXTURE_2D);
			rcBindTexture(RC_TEXTURE_2D, TextureArray[PieTin]);
			rcDrawArrays( RC_TRIANGLE_FAN, 12, 4 );

			rcBindMaterial(MaterialIDs[StandMixer]);
			rcEnable(RC_TEXTURE_2D);
			rcBindTexture(RC_TEXTURE_2D, TextureArray[StandMixer]);
			rcDrawArrays( RC_TRIANGLE_FAN, 16, 4 );

			rcBindMaterial(MaterialIDs[Toaster]);
			rcEnable(RC_TEXTURE_2D);
			rcBindTexture(RC_TEXTURE_2D, TextureArray[Toaster]);
			rcDrawArrays( RC_TRIANGLE_FAN, 20, 4 );
			
			g_fSpinY -= 3.f;
			g_fSpinZ -= 3.f;
		}

		return TRUE;
	}
};

Tutorial	g_Tutorial;
