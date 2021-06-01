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
// DATE    : 8/21/2013 Wed
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "RCFramework.h"

// Simple Drawing Test : Refraction Diamond

#include "../common/DIBImage.h"

#define		OBJECT_TYPE		1		//0 : Cube, 1 : Diamond

unsigned int TextureArray[6];

float g_fSpinY = -10.0f;
float g_fSpinX = -45.0f;

struct Color {
	float r, g, b;
};

struct Pos {
	float x, y, z;
};

struct TexC {
	float u, v;
};

struct Color g_cubeColor[] =
{
	// Quad 0
	{ 1.0f,0.0f,0.0f }, // 0 (unique)
	{ 0.0f,1.0f,0.0f }, // 1 (unique)
	{ 0.0f,0.0f,1.0f }, // 2 (unique)
	{ 1.0f,1.0f,0.0f }, // 3 (unique)

	// Quad 1
	{ 1.0f,0.0f,1.0f }, // 4 (unique)
	{ 0.0f,1.0f,1.0f }, // 5 (unique)
	{ 1.0f,1.0f,1.0f }, // 6 (unique)
	{ 1.0f,0.0f,0.0f }, // 7 (unique)

	// Quad 2
	{ 0.0f,1.0f,1.0f }, // 5 (start repeating here)
	{ 1.0f,1.0f,0.0f }, // 3 (repeat of vertex 3)
	{ 0.0f,0.0f,1.0f }, // 2 (repeat of vertex 2... etc.)
	{ 1.0f,1.0f,1.0f }, // 6

	// Quad 3
	{ 1.0f,0.0f,1.0f }, // 4
	{ 1.0f,0.0f,0.0f }, // 7
	{ 0.0f,1.0f,0.0f }, // 1
	{ 1.0f,0.0f,0.0f }, // 0

	// Quad 4
	{ 1.0f,0.0f,0.0f }, // 7
	{ 1.0f,1.0f,1.0f }, // 6
	{ 0.0f,0.0f,1.0f }, // 2
	{ 0.0f,1.0f,0.0f }, // 1

	// Quad 5
	{ 1.0f,0.0f,1.0f }, // 4
	{ 1.0f,0.0f,0.0f }, // 0
	{ 1.0f,1.0f,0.0f }, // 3
	{ 0.0f,1.0f,1.0f }  // 5
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

RCubyte g_cubeIndices[] =
{
	0, 1, 2, 3, // Quad 0
	4, 5, 6, 7, // Quad 1
	5, 3, 2, 6, // Quad 2
	4, 7, 1, 0, // Quad 3
	7, 6, 2, 1, // Quad 4
	4, 0, 3, 5  // Quad 5
};


struct Color g_cubeColor_indexed[]= {
	{ 1.0f,0.0f,0.0f }, // 0
	{ 0.0f,1.0f,0.0f }, // 1
	{ 0.0f,0.0f,1.0f }, // 2
	{ 1.0f,1.0f,0.0f }, // 3
	{ 1.0f,0.0f,1.0f }, // 4
	{ 0.0f,1.0f,1.0f }, // 5
	{ 1.0f,1.0f,1.0f }, // 6
	{ 1.0f,0.0f,0.0f }, // 7
};

struct Pos g_cubePos_indexed[] = {
	{ -1.0f,-1.0f, 1.0f }, // 0
	{  1.0f,-1.0f, 1.0f }, // 1
	{  1.0f, 1.0f, 1.0f }, // 2
	{ -1.0f, 1.0f, 1.0f }, // 3
	{ -1.0f,-1.0f,-1.0f }, // 4
	{ -1.0f, 1.0f,-1.0f }, // 5
	{  1.0f, 1.0f,-1.0f }, // 6
	{  1.0f,-1.0f,-1.0f }, // 7
};

struct TexC g_cubeTexcoord_indexed[] = {
	{  0.0f, 0.0f }, // 0
	{  1.0f, 0.0f }, // 1
	{  1.0f, 1.0f }, // 2
	{  0.0f, 1.0f }, // 3
	{  0.0f, 0.0f }, // 4
	{  0.0f, 1.0f }, // 5
	{  1.0f, 1.0f }, // 6
	{  1.0f, 0.0f }, // 7
};

struct Pos g_diamondPos[] = {
	{ -0.25f, 0.5f, -0.5f },		//0
	{  0.25f, 0.5f, -0.5f },		//1
	{  0.5f,  0.5f,  0.0f },	//2
	{  0.25f, 0.5f,  0.5f },	//3
	{ -0.25f, 0.5f,  0.5f },		//4
	{ -0.5f,  0.5f,  0.0f },		//5

	{ -0.5f,  0.0f, -1.0f },		//6
	{  0.5f,  0.0f, -1.0f },		//7
	{  1.0f,  0.0f,  0.0f },	//8
	{  0.5f,  0.0f,  1.0f },	//9
	{ -0.5f,  0.0f,  1.0f },		//10
	{ -1.0f,  0.0f,  0.0f },		//11

	{  0.0f, -1.0f, 0.0f }		//12
};

RCubyte g_diamondIndices[]= {
	0, 1, 2,
	2, 3, 4,
	4, 5, 2,
	2, 5, 0,

	4, 3, 9,
	9, 10, 4,
	3, 2, 8,
	8, 9, 3,
	2, 1, 7,
	7, 8, 2,
	1, 0, 6,
	6, 7, 1,
	0, 5, 11,
	11, 6, 0,
	5, 4, 10,
	10, 11, 5,

	10, 9, 12,
	9, 8, 12,
	8, 7, 12,
	7, 6, 12,
	6, 11, 12,
	11, 10, 12
};

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("RefractDiamond", 800, 480){}
	virtual ~Tutorial(void){}

	void Load(RGBImageRec **pBitmap){
		pBitmap[0] = (RGBImageRec *)DIBImageLoad("./scenedata/RefractDiamond/WallPaper01.bmp", 3);	
		pBitmap[1] = (RGBImageRec *)DIBImageLoad("./scenedata/RefractDiamond/WallPaper02.bmp", 3);	
		pBitmap[2] = (RGBImageRec *)DIBImageLoad("./scenedata/RefractDiamond/WallPaper03.bmp", 3);	
		pBitmap[3] = (RGBImageRec *)DIBImageLoad("./scenedata/RefractDiamond/WallPaper04.bmp", 3);	
		pBitmap[4] = (RGBImageRec *)DIBImageLoad("./scenedata/RefractDiamond/WallPaper05.bmp", 3);	
		pBitmap[5] = (RGBImageRec *)DIBImageLoad("./scenedata/RefractDiamond/WallPaper06.bmp", 3);	
	}

	void CreateTexture(unsigned int *textureArray) {

		RGBImageRec *pBitmap[6];
		int i=0;

		for(i=0; i<6; i++) {
			pBitmap[i] = NULL;
		}
		Load(pBitmap);

		rcGenTextures(6, textureArray);

		//rcPixelStorei (RC_UNPACK_ALIGNMENT, 1);	//구현되지 않은 함수라 에러 메시지 나옴!

		for(i=0; i<6; i++) {
			rcBindTexture(RC_TEXTURE_2D, textureArray[i]);
			rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[i]->sizeX, pBitmap[i]->sizeY, 0, RC_RGB, RC_UNSIGNED_BYTE, pBitmap[i]->data);	
			rcTexParameteri(RC_TEXTURE_2D,RC_TEXTURE_MIN_FILTER,RC_LINEAR_MIPMAP_NEAREST);
			rcTexParameteri(RC_TEXTURE_2D,RC_TEXTURE_MAG_FILTER,RC_LINEAR_MIPMAP_NEAREST);
		}

		for(i=0; i<6; i++) {
			if(pBitmap[i]) {
				if(pBitmap[i]->data)	free(pBitmap[i]->data);
				free(pBitmap[i]);
			}
		}
	}

	void StaticSceneDraw(void){
		rcStaticSceneBegin();

		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, 0.0f, -3.0f );

		rcScalef( 5.0f, 5.0f, 5.0f );

		rcEnableClientState(RC_VERTEX_ARRAY);
		rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

		float color[6][4]={
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,1,1,0},
			{1,0,1,0},
			{1,1,0,0},
		};

		rcVertexPointer(3, RC_FLOAT, 0, g_cubePos);
		rcTexCoordPointer(2, RC_FLOAT, 0, g_cubeTexs);

		rcBindMaterial(1);
		rcEnable(RC_TEXTURE_2D);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[0]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[0]);	
		rcDrawArrays( RC_TRIANGLE_FAN, 4, 4 );

		rcBindMaterial(2);
		rcEnable(RC_TEXTURE_2D);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[1]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[1]);	
		rcDrawArrays( RC_TRIANGLE_FAN, 8, 4 );

		rcBindMaterial(3);
		rcEnable(RC_TEXTURE_2D);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[2]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[2]);
		rcDrawArrays( RC_TRIANGLE_FAN, 12, 4 );

		rcBindMaterial(4);
		rcEnable(RC_TEXTURE_2D);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[3]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[3]);	
		rcDrawArrays( RC_TRIANGLE_FAN, 16, 4 );

		rcBindMaterial(5);
		rcEnable(RC_TEXTURE_2D);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[4]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[4]);	
		rcDrawArrays( RC_TRIANGLE_FAN, 20, 4 );

		rcBindMaterial(6);
		rcEnable(RC_TEXTURE_2D);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[5]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[5]);	
		rcDrawArrays( RC_TRIANGLE_FAN, 0, 4 );

		rcStaticSceneEnd();

		rcDisable(RC_TEXTURE_2D);
	}

	void SetMaterial(void){
		RCfloat ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
		RCfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
		RCfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
		float reflect = 0.0f;
		float transmit = 0.9f;
		float refract_index = 1.7f;

		rcBindMaterial(7);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT, ambient);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, diffuse);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specular);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFLECTION, reflect);
		rcMaterialf (RC_FRONT_AND_BACK, RC_TRANSMITTANCE, transmit);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, refract_index);
	}

	void DynamicSceneDraw(void){
		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, 0.0f, -3.0f );

		rcRotatef( -g_fSpinX, 1.0f, 0.0f, 0.0f );
		rcRotatef( -g_fSpinY, 0.0f, 1.0f, 0.0f );

		rcEnableClientState(RC_VERTEX_ARRAY);

		if(OBJECT_TYPE == 0) {
			//Cube
			rcVertexPointer(3, RC_FLOAT, 0, g_cubePos_indexed);

			//If you want to use the rcMaterial function, 
			//Don't use the rcColor4f function!
			rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[0] );
			rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[4] );
			rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[8] );
			rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[12] );
			rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[16] );
			rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[20] );

		} else if(OBJECT_TYPE == 1) {
			//Diamond
			rcVertexPointer(3, RC_FLOAT, 0, g_diamondPos);

			//If you want to use the rcMaterial function, 
			//Don't use the rcColor4f function!
			for(int j=0; j<22; j++)
				rcDrawElements( RC_TRIANGLE_STRIP, 3, RC_UNSIGNED_BYTE, &g_diamondIndices[3*j] );
		}
	}

protected:
	virtual BOOL OnIntialize(void){
		rcSceneAllInit();

		{
			rcClearColor( 0.73f, 0.2f, 0.23f, 1.0f );			
			rcViewport(0, 0, Width(), Height());

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();
			rcuPerspective( 80.0f, (RCfloat)Width() / (RCfloat)Height(), 0.1f, 100.0f);
			rcuLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

			CreateTexture(TextureArray);

			StaticSceneDraw();
		}

		SetMaterial();
		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;
		i++;

		DynamicSceneDraw();

		g_fSpinX += 3.f;
		g_fSpinY += 3.f;

		return TRUE;
	}
};

Tutorial	g_Tutorial;