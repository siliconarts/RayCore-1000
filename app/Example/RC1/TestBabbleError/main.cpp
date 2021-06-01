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

// Test - Babble Error

#include "../common/DIBImage.h"

unsigned int textureID;

float g_fMoveY = 0.0f;

//float eye[] = {0.00f, 3.0339072f, -0.030242039f};
//float center[] = {0.00f, 3.0504291f, -1.0301056f};
//float up[] = {0.00f, 0.99986351f, 0.0152194f};
//float lightPos[]={-0.002f, 3.0339072f, -0.030242039f, 0.0f};

float eye[] = {0.00f, 0.0f, 1.0f};
float center[] = {0.00f, 0.0f, 0.0f};
float up[] = {0.00f, 1.0f, 0.0f};
float lightPos[]={-0.002f, 0.0f, 1.0f, 0.0f};

struct Pos {
	float x, y, z;
};

struct TexC {
	float u, v;
};

//struct Pos g_objectPos[] =
//{
//	{  0.002f, 16.570074f, -0.022758564f },	//0
//	{  0.002f, 16.572073f, -0.022725521f },	//1
//	{ -0.002f, 16.572073f, -0.022725521f },	//2
//	{ -0.002f, 16.570074f, -0.022758564f },	//3
//};

//struct TexC g_objectTexs[] =
//{
//	{  1.0f, 1.0f },	//0
//	{  1.0f, 0.0f },	//1
//	{  0.0f, 0.0f },	//2
//	{  0.0f, 1.0f },	//3
//};

struct Pos g_objectPos[] =
{
	{ -4.0f, 0.0f, 0.0f },	//3
	{  4.0f, 0.0f, 0.0f },	//0
	{  4.0f, 4.0f, 0.0f },	//1
	{ -4.0f, 4.0f, 0.0f },	//2	
};

struct TexC g_objectTexs[] =
{
	{  0.0f, 0.0f },	//2	
	{  1.0f, 0.0f },	//1
	{  1.0f, 1.0f },	//0	
	{  0.0f, 1.0f },	//3	
};

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Test - Babble Error", 480, 800){}
	virtual ~Tutorial(void){}

protected:
	virtual BOOL OnIntialize(void){
		rcSceneAllInit();

		{
			rcClearColor(1.0f, 1.0f, 1.0f, 1.0f);

			rcViewport(0, 0, Width(), Height());

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();
			//rcuPerspective( 60.0f, (RCfloat)Width() / (RCfloat)Height(), 0.00001f, 100.0f);
			rcuPerspective( 60.0f, (RCfloat)Width() / (RCfloat)Height(), 0.1f, 100.0f);

			rcuLookAt(eye[0], eye[1], eye[2], 
				center[0], center[1], center[2], 
				up[0], up[1], up[2]);
						
			RCfloat ambient[]={0.4f, 0.4f, 0.4f, 0.0f};
			RCfloat diffuse[]={0.5f, 0.5f, 0.5f, 0.0f};
			RCfloat specular[]={0.8f, 0.8f, 0.8f, 0.0f};
			
			rcEnable(RC_LIGHTING);
			rcEnable(RC_LIGHT0);

			rcLightfv(RC_LIGHT0, RC_AMBIENT, ambient);
			rcLightfv(RC_LIGHT0, RC_DIFFUSE, diffuse);
			rcLightfv(RC_LIGHT0, RC_SPECULAR, specular);
			rcLightfv(RC_LIGHT0, RC_POSITION, lightPos);

			RGBImageRec *pBitmap = (RGBImageRec *)DIBImageLoad("./scenedata/TestBabbleError/congratulations.bmp", 4);	
			
			rcGenTextures(1, &textureID);

			rcBindTexture(RC_TEXTURE_2D, textureID);
			rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGBA, pBitmap->sizeX, pBitmap->sizeY, 0, RC_RGBA, RC_UNSIGNED_BYTE, pBitmap->data);	
			rcTexParameteri(RC_TEXTURE_2D,RC_TEXTURE_MIN_FILTER,RC_LINEAR_MIPMAP_NEAREST);
			rcTexParameteri(RC_TEXTURE_2D,RC_TEXTURE_MAG_FILTER,RC_LINEAR_MIPMAP_NEAREST);

			if(pBitmap->data)	free(pBitmap->data);
			free(pBitmap);

			RCfloat transmittance=1.0f;
			RCfloat color[]={1.0f, 1.0f, 0.0f, 0.0f};
			
			rcBindMaterial(1);
			rcEnable(RC_TEXTURE_2D);			
			rcBindTexture(RC_TEXTURE_2D, textureID);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color);
			rcMaterialf (RC_FRONT_AND_BACK, RC_TRANSMITTANCE, transmittance);			

			rcHint(RC_RENDERING_HINT, RC_NICEST);
		}

		return TRUE;
	}
	
	void DynamicSceneDraw(void){
		rcMatrixMode( RC_PROJECTION );
		rcLoadIdentity();
		rcuLookAt(eye[0], eye[1]+g_fMoveY, eye[2], 
			center[0], center[1]+g_fMoveY, center[2], 
			up[0], up[1], up[2]);

		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, g_fMoveY-2.0f, -5.0f );
				
		rcLightfv(RC_LIGHT0, RC_POSITION, lightPos);
		
		rcEnableClientState(RC_VERTEX_ARRAY);
		rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

		rcVertexPointer(3, RC_FLOAT, 0, g_objectPos);
		rcTexCoordPointer(2, RC_FLOAT, 0, g_objectTexs);
		
		rcEnable(RC_USE_TRANSMITTANCE_SHADOW);
		rcEnable(RC_USE_TEXTURE_ALPHA_SHADOW);
		rcDrawArrays( RC_TRIANGLE_FAN, 0, 4 );
	}

	virtual BOOL OnDraw(void){
		static int i = 0;

		DynamicSceneDraw();
		
		g_fMoveY += 200.0f;
		
		i++;
		return TRUE;
	}
};

Tutorial	g_Tutorial;

