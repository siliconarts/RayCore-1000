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
#include <stdio.h>

// Simple Drawing Test : Cube
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

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Cube", 800, 480){}
	virtual ~Tutorial(void){}

	void StaticSceneDraw(void){

		rcStaticSceneBegin();

		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, 0.0f, -3.0f );

		rcScalef( 5.0f, 5.0f, 5.0f );

		rcEnableClientState(RC_VERTEX_ARRAY);
		//rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

		float color[6][4]={
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,1,1,0},
			{1,0,1,0},
			{1,1,0,0},
		};

		rcDisable(RC_TEXTURE_2D);

		rcVertexPointer(3, RC_FLOAT, 0, g_cubePos_indexed);
		rcBindMaterial(1);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[0]);
		rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[0] );

		rcBindMaterial(2);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[1]);
		rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[4] );

		rcBindMaterial(3);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[2]);
		rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[8] );

		rcBindMaterial(4);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[3]);
		rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[12] );

		rcBindMaterial(5);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[4]);
		rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[16] );

		rcBindMaterial(6);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[5]);
		rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[20] );

		rcStaticSceneEnd();
	}

	void DynamicSceneDraw(void){
		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, 0.0f, -3.0f );

		rcRotatef( -g_fSpinX, 1.0f, 0.0f, 0.0f );
		rcRotatef( -g_fSpinY, 0.0f, 1.0f, 0.0f );

		rcEnableClientState(RC_VERTEX_ARRAY);

		rcVertexPointer(3, RC_FLOAT, 0, g_cubePos);

		rcBindMaterial(2);
		rcDrawArrays( RC_TRIANGLE_FAN, 4, 4 );

		rcBindMaterial(3);
		rcDrawArrays( RC_TRIANGLE_FAN, 8, 4 );

		rcBindMaterial(4);
		rcDrawArrays( RC_TRIANGLE_FAN, 12, 4 );

		rcBindMaterial(5);
		rcDrawArrays( RC_TRIANGLE_FAN, 16, 4 );

		rcBindMaterial(6);
		rcDrawArrays( RC_TRIANGLE_FAN, 20, 4 );

		rcBindMaterial(1);
		rcDrawArrays( RC_TRIANGLE_FAN, 0, 4 );
	}

protected:
	virtual BOOL OnIntialize(void){
		rcSceneAllInit();
		{
			rcClearColor( 0.73f, 0.2f, 0.23f, 1.0f );			
			rcViewport(0, 0, Width(), Height());

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();
			rcuPerspective( 60.0f, (RCfloat)Width() / (RCfloat)Height(), 0.1f, 100.0f);
			rcuLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

			StaticSceneDraw();
		}
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
