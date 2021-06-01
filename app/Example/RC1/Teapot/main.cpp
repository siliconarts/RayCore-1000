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

#include <stdio.h>
#include "RCFramework.h"

// Simple Drawing Test : Teapot

#include "teapot.h"

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
	Tutorial(void) : RCFramework("Teapot", 800, 480){}
	virtual ~Tutorial(void){}

	void initLights()
	{
		// set up light colors (ambient, diffuse, specular)
		RCfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
		RCfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
		RCfloat lightKs[] = {1, 1, 1, 1};           // specular light
		//RCfloat lightPos[] = {0.f, -10.5f, 20, 1};	// positional light
		RCfloat lightPos[] = { 0.0f, -0.5f, 1.0f, 1.0f };

		rcEnable(RC_LIGHTING);

		rcLightfv(RC_LIGHT0, RC_AMBIENT, lightKa);
		rcLightfv(RC_LIGHT0, RC_DIFFUSE, lightKd);
		rcLightfv(RC_LIGHT0, RC_SPECULAR, lightKs);

		// position the light
		rcLightfv(RC_LIGHT0, RC_POSITION, lightPos);

		rcEnable(RC_LIGHT0);                        // MUST enable each light source after configuration
	}

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
#if 0
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
#else
		float shininess = 15.0f;
		float diffuseColor[4]	= {0.929524f, 0.796542f, 0.178823f, 1};
		float specularColor[4]	= {1.00000f, 0.980392f, 0.549020f, 1.0f};

		// set specular and shininess using rcMaterial (gold-yellow)
		rcBindMaterial(6);

		rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS, shininess); // range 0 ~ 128
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specularColor);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, diffuseColor);

		rcMaterialf (RC_FRONT_AND_BACK, RC_SPECULAR_LEVEL, 1);

		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, -0.6f, -2.0f );

		rcScalef(0.6f, 0.6f, 0.6f);
		rcRotatef( g_fSpinX+45.f, 1.0f, 0.0f, 0.0f );
		rcRotatef( -g_fSpinY, 0.0f, 1.0f, 0.0f );


		rcEnableClientState(RC_NORMAL_ARRAY);
		rcEnableClientState(RC_VERTEX_ARRAY);


		rcNormalPointer(RC_FLOAT, 0, teapotNormals);
		rcVertexPointer(3, RC_FLOAT, 0, teapotVertices);

		rcDrawElements(RC_TRIANGLE_STRIP, 12, RC_UNSIGNED_SHORT, &teapotIndices[0]);
		rcDrawElements(RC_TRIANGLE_STRIP, 78, RC_UNSIGNED_SHORT, &teapotIndices[12]);
		rcDrawElements(RC_TRIANGLE_STRIP, 35, RC_UNSIGNED_SHORT, &teapotIndices[90]);
		rcDrawElements(RC_TRIANGLE_STRIP, 70, RC_UNSIGNED_SHORT, &teapotIndices[125]);
		rcDrawElements(RC_TRIANGLE_STRIP, 65, RC_UNSIGNED_SHORT, &teapotIndices[195]);
		rcDrawElements(RC_TRIANGLE_STRIP, 37, RC_UNSIGNED_SHORT, &teapotIndices[260]);
		rcDrawElements(RC_TRIANGLE_STRIP, 35, RC_UNSIGNED_SHORT, &teapotIndices[297]);
		rcDrawElements(RC_TRIANGLE_STRIP, 32, RC_UNSIGNED_SHORT, &teapotIndices[332]);
		rcDrawElements(RC_TRIANGLE_STRIP, 56, RC_UNSIGNED_SHORT, &teapotIndices[364]);
		rcDrawElements(RC_TRIANGLE_STRIP, 45, RC_UNSIGNED_SHORT, &teapotIndices[420]);
		rcDrawElements(RC_TRIANGLE_STRIP, 41, RC_UNSIGNED_SHORT, &teapotIndices[465]);
		rcDrawElements(RC_TRIANGLE_STRIP, 37, RC_UNSIGNED_SHORT, &teapotIndices[506]);
		rcDrawElements(RC_TRIANGLE_STRIP, 33, RC_UNSIGNED_SHORT, &teapotIndices[543]);
		rcDrawElements(RC_TRIANGLE_STRIP, 29, RC_UNSIGNED_SHORT, &teapotIndices[576]);
		rcDrawElements(RC_TRIANGLE_STRIP, 25, RC_UNSIGNED_SHORT, &teapotIndices[605]);
		rcDrawElements(RC_TRIANGLE_STRIP, 21, RC_UNSIGNED_SHORT, &teapotIndices[630]);
		rcDrawElements(RC_TRIANGLE_STRIP, 17, RC_UNSIGNED_SHORT, &teapotIndices[651]);
		rcDrawElements(RC_TRIANGLE_STRIP, 13, RC_UNSIGNED_SHORT, &teapotIndices[668]);
		rcDrawElements(RC_TRIANGLE_STRIP, 9, RC_UNSIGNED_SHORT, &teapotIndices[681]);
		rcDrawElements(RC_TRIANGLE_STRIP, 27, RC_UNSIGNED_SHORT, &teapotIndices[690]);
		rcDrawElements(RC_TRIANGLE_STRIP, 16, RC_UNSIGNED_SHORT, &teapotIndices[717]);
		rcDrawElements(RC_TRIANGLE_STRIP, 22, RC_UNSIGNED_SHORT, &teapotIndices[733]);
		rcDrawElements(RC_TRIANGLE_STRIP, 50, RC_UNSIGNED_SHORT, &teapotIndices[755]);
		rcDrawElements(RC_TRIANGLE_STRIP, 42, RC_UNSIGNED_SHORT, &teapotIndices[805]);
		rcDrawElements(RC_TRIANGLE_STRIP, 43, RC_UNSIGNED_SHORT, &teapotIndices[847]);
		rcDrawElements(RC_TRIANGLE_STRIP, 4, RC_UNSIGNED_SHORT, &teapotIndices[890]);
		rcDrawElements(RC_TRIANGLE_STRIP, 143, RC_UNSIGNED_SHORT, &teapotIndices[894]);
		rcDrawElements(RC_TRIANGLE_STRIP, 234, RC_UNSIGNED_SHORT, &teapotIndices[1037]);
		rcDrawElements(RC_TRIANGLE_STRIP, 224, RC_UNSIGNED_SHORT, &teapotIndices[1271]);
		rcDrawElements(RC_TRIANGLE_STRIP, 71, RC_UNSIGNED_SHORT, &teapotIndices[1495]);
		rcDrawElements(RC_TRIANGLE_STRIP, 69, RC_UNSIGNED_SHORT, &teapotIndices[1566]);
		rcDrawElements(RC_TRIANGLE_STRIP, 67, RC_UNSIGNED_SHORT, &teapotIndices[1635]);
		rcDrawElements(RC_TRIANGLE_STRIP, 65, RC_UNSIGNED_SHORT, &teapotIndices[1702]);
		rcDrawElements(RC_TRIANGLE_STRIP, 63, RC_UNSIGNED_SHORT, &teapotIndices[1767]);
		rcDrawElements(RC_TRIANGLE_STRIP, 61, RC_UNSIGNED_SHORT, &teapotIndices[1830]);
		rcDrawElements(RC_TRIANGLE_STRIP, 59, RC_UNSIGNED_SHORT, &teapotIndices[1891]);
		rcDrawElements(RC_TRIANGLE_STRIP, 57, RC_UNSIGNED_SHORT, &teapotIndices[1950]);
		rcDrawElements(RC_TRIANGLE_STRIP, 55, RC_UNSIGNED_SHORT, &teapotIndices[2007]);
		rcDrawElements(RC_TRIANGLE_STRIP, 53, RC_UNSIGNED_SHORT, &teapotIndices[2062]);
		rcDrawElements(RC_TRIANGLE_STRIP, 51, RC_UNSIGNED_SHORT, &teapotIndices[2115]);
		rcDrawElements(RC_TRIANGLES,       3, RC_UNSIGNED_SHORT, &teapotIndices[2166]);
		rcDrawElements(RC_TRIANGLE_STRIP, 50, RC_UNSIGNED_SHORT, &teapotIndices[2169]);
		rcDrawElements(RC_TRIANGLE_STRIP, 48, RC_UNSIGNED_SHORT, &teapotIndices[2219]);
		rcDrawElements(RC_TRIANGLE_STRIP, 46, RC_UNSIGNED_SHORT, &teapotIndices[2267]);
		rcDrawElements(RC_TRIANGLE_STRIP, 44, RC_UNSIGNED_SHORT, &teapotIndices[2313]);
		rcDrawElements(RC_TRIANGLE_STRIP, 42, RC_UNSIGNED_SHORT, &teapotIndices[2357]);
		rcDrawElements(RC_TRIANGLE_STRIP, 40, RC_UNSIGNED_SHORT, &teapotIndices[2399]);
		rcDrawElements(RC_TRIANGLE_STRIP, 38, RC_UNSIGNED_SHORT, &teapotIndices[2439]);
		rcDrawElements(RC_TRIANGLE_STRIP, 36, RC_UNSIGNED_SHORT, &teapotIndices[2477]);
		rcDrawElements(RC_TRIANGLE_STRIP, 34, RC_UNSIGNED_SHORT, &teapotIndices[2513]);
		rcDrawElements(RC_TRIANGLE_STRIP, 32, RC_UNSIGNED_SHORT, &teapotIndices[2547]);
		rcDrawElements(RC_TRIANGLE_STRIP, 30, RC_UNSIGNED_SHORT, &teapotIndices[2579]);
		rcDrawElements(RC_TRIANGLE_STRIP, 28, RC_UNSIGNED_SHORT, &teapotIndices[2609]);
		rcDrawElements(RC_TRIANGLE_STRIP, 26, RC_UNSIGNED_SHORT, &teapotIndices[2637]);
		rcDrawElements(RC_TRIANGLE_STRIP, 24, RC_UNSIGNED_SHORT, &teapotIndices[2663]);
		rcDrawElements(RC_TRIANGLE_STRIP, 22, RC_UNSIGNED_SHORT, &teapotIndices[2687]);
		rcDrawElements(RC_TRIANGLE_STRIP, 20, RC_UNSIGNED_SHORT, &teapotIndices[2709]);
		rcDrawElements(RC_TRIANGLE_STRIP, 18, RC_UNSIGNED_SHORT, &teapotIndices[2729]);
		rcDrawElements(RC_TRIANGLE_STRIP, 16, RC_UNSIGNED_SHORT, &teapotIndices[2747]);
		rcDrawElements(RC_TRIANGLE_STRIP, 14, RC_UNSIGNED_SHORT, &teapotIndices[2763]);
		rcDrawElements(RC_TRIANGLE_STRIP, 12, RC_UNSIGNED_SHORT, &teapotIndices[2777]);
		rcDrawElements(RC_TRIANGLE_STRIP, 10, RC_UNSIGNED_SHORT, &teapotIndices[2789]);
		rcDrawElements(RC_TRIANGLE_STRIP,  8, RC_UNSIGNED_SHORT, &teapotIndices[2799]);
		rcDrawElements(RC_TRIANGLE_STRIP,  6, RC_UNSIGNED_SHORT, &teapotIndices[2807]);
		rcDrawElements(RC_TRIANGLES,       3, RC_UNSIGNED_SHORT, &teapotIndices[2813]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[2816]);
		rcDrawElements(RC_TRIANGLE_STRIP, 200, RC_UNSIGNED_SHORT, &teapotIndices[2819]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3019]);
		rcDrawElements(RC_TRIANGLE_STRIP, 66, RC_UNSIGNED_SHORT, &teapotIndices[3022]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3088]);
		rcDrawElements(RC_TRIANGLE_STRIP, 209, RC_UNSIGNED_SHORT, &teapotIndices[3091]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3300]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3303]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3306]);
		rcDrawElements(RC_TRIANGLE_STRIP, 38, RC_UNSIGNED_SHORT, &teapotIndices[3309]);
		rcDrawElements(RC_TRIANGLE_STRIP, 15, RC_UNSIGNED_SHORT, &teapotIndices[3347]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3362]);
		rcDrawElements(RC_TRIANGLE_STRIP, 26, RC_UNSIGNED_SHORT, &teapotIndices[3365]);
		rcDrawElements(RC_TRIANGLE_STRIP, 9, RC_UNSIGNED_SHORT, &teapotIndices[3391]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3400]);
		rcDrawElements(RC_TRIANGLE_STRIP, 14, RC_UNSIGNED_SHORT, &teapotIndices[3403]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3417]);
		rcDrawElements(RC_TRIANGLE_STRIP, 115, RC_UNSIGNED_SHORT, &teapotIndices[3420]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3535]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3538]);
		rcDrawElements(RC_TRIANGLE_STRIP, 39, RC_UNSIGNED_SHORT, &teapotIndices[3541]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3580]);
		rcDrawElements(RC_TRIANGLE_STRIP, 91, RC_UNSIGNED_SHORT, &teapotIndices[3583]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3674]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3677]);
		rcDrawElements(RC_TRIANGLE_STRIP, 31, RC_UNSIGNED_SHORT, &teapotIndices[3680]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3711]);
		rcDrawElements(RC_TRIANGLE_STRIP, 67, RC_UNSIGNED_SHORT, &teapotIndices[3714]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3781]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3784]);
		rcDrawElements(RC_TRIANGLE_STRIP, 23, RC_UNSIGNED_SHORT, &teapotIndices[3787]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3810]);
		rcDrawElements(RC_TRIANGLE_STRIP, 45, RC_UNSIGNED_SHORT, &teapotIndices[3813]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3858]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3861]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3864]);
		rcDrawElements(RC_TRIANGLE_STRIP, 32, RC_UNSIGNED_SHORT, &teapotIndices[3867]);
		rcDrawElements(RC_TRIANGLE_STRIP, 38, RC_UNSIGNED_SHORT, &teapotIndices[3899]);
		rcDrawElements(RC_TRIANGLE_STRIP, 15, RC_UNSIGNED_SHORT, &teapotIndices[3937]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3952]);
		rcDrawElements(RC_TRIANGLE_STRIP, 26, RC_UNSIGNED_SHORT, &teapotIndices[3955]);
		rcDrawElements(RC_TRIANGLE_STRIP, 9, RC_UNSIGNED_SHORT, &teapotIndices[3981]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[3990]);
		rcDrawElements(RC_TRIANGLE_STRIP, 14, RC_UNSIGNED_SHORT, &teapotIndices[3993]);
		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[4007]);
		rcDrawElements(RC_TRIANGLE_STRIP, 135, RC_UNSIGNED_SHORT, &teapotIndices[4010]);

		rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &teapotIndices[4145]);
		rcDrawElements(RC_TRIANGLE_STRIP,  76, RC_UNSIGNED_SHORT, &teapotIndices[4148]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[4224]);
		rcDrawElements(RC_TRIANGLE_STRIP,  60, RC_UNSIGNED_SHORT, &teapotIndices[4227]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[4287]);
		rcDrawElements(RC_TRIANGLE_STRIP,  23, RC_UNSIGNED_SHORT, &teapotIndices[4290]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[4313]);
		rcDrawElements(RC_TRIANGLE_STRIP,  26, RC_UNSIGNED_SHORT, &teapotIndices[4316]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[4342]);
		rcDrawElements(RC_TRIANGLE_STRIP,   6, RC_UNSIGNED_SHORT, &teapotIndices[4345]);
		rcDrawElements(RC_TRIANGLE_STRIP, 947, RC_UNSIGNED_SHORT, &teapotIndices[4351]);
		rcDrawElements(RC_TRIANGLE_STRIP,  35, RC_UNSIGNED_SHORT, &teapotIndices[5298]);
		rcDrawElements(RC_TRIANGLE_STRIP,  31, RC_UNSIGNED_SHORT, &teapotIndices[5333]);
		rcDrawElements(RC_TRIANGLE_STRIP,  27, RC_UNSIGNED_SHORT, &teapotIndices[5364]);
		rcDrawElements(RC_TRIANGLE_STRIP,  23, RC_UNSIGNED_SHORT, &teapotIndices[5391]);
		rcDrawElements(RC_TRIANGLE_STRIP,  20, RC_UNSIGNED_SHORT, &teapotIndices[5414]);
		rcDrawElements(RC_TRIANGLE_STRIP,  24, RC_UNSIGNED_SHORT, &teapotIndices[5434]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5458]);
		rcDrawElements(RC_TRIANGLE_STRIP,  28, RC_UNSIGNED_SHORT, &teapotIndices[5461]);
		rcDrawElements(RC_TRIANGLE_STRIP,  32, RC_UNSIGNED_SHORT, &teapotIndices[5489]);
		rcDrawElements(RC_TRIANGLE_STRIP,  36, RC_UNSIGNED_SHORT, &teapotIndices[5521]);
		rcDrawElements(RC_TRIANGLE_STRIP,  76, RC_UNSIGNED_SHORT, &teapotIndices[5557]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5633]);
		rcDrawElements(RC_TRIANGLE_STRIP,  67, RC_UNSIGNED_SHORT, &teapotIndices[5636]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5703]);
		rcDrawElements(RC_TRIANGLE_STRIP,  59, RC_UNSIGNED_SHORT, &teapotIndices[5706]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5765]);
		rcDrawElements(RC_TRIANGLE_STRIP,  51, RC_UNSIGNED_SHORT, &teapotIndices[5768]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5819]);
		rcDrawElements(RC_TRIANGLE_STRIP,  43, RC_UNSIGNED_SHORT, &teapotIndices[5822]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5865]);
		rcDrawElements(RC_TRIANGLE_STRIP,  35, RC_UNSIGNED_SHORT, &teapotIndices[5868]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5903]);
		rcDrawElements(RC_TRIANGLE_STRIP,  27, RC_UNSIGNED_SHORT, &teapotIndices[5906]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5933]);
		rcDrawElements(RC_TRIANGLE_STRIP,  19, RC_UNSIGNED_SHORT, &teapotIndices[5936]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5955]);
		rcDrawElements(RC_TRIANGLE_STRIP,  11, RC_UNSIGNED_SHORT, &teapotIndices[5958]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[5969]);
		rcDrawElements(RC_TRIANGLE_STRIP,  30, RC_UNSIGNED_SHORT, &teapotIndices[5972]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[6002]);
		rcDrawElements(RC_TRIANGLE_STRIP,  11, RC_UNSIGNED_SHORT, &teapotIndices[6005]);
		rcDrawElements(RC_TRIANGLE_STRIP,  18, RC_UNSIGNED_SHORT, &teapotIndices[6016]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[6034]);
		rcDrawElements(RC_TRIANGLES,        3, RC_UNSIGNED_SHORT, &teapotIndices[6037]);
		rcDrawElements(RC_TRIANGLE_STRIP,   5, RC_UNSIGNED_SHORT, &teapotIndices[6040]);
		rcDrawElements(RC_TRIANGLE_STRIP, 122, RC_UNSIGNED_SHORT, &teapotIndices[6045]);
		rcDrawElements(RC_TRIANGLE_STRIP,  75, RC_UNSIGNED_SHORT, &teapotIndices[6167]);
		rcDrawElements(RC_TRIANGLE_STRIP,  71, RC_UNSIGNED_SHORT, &teapotIndices[6242]);
		rcDrawElements(RC_TRIANGLE_STRIP,  67, RC_UNSIGNED_SHORT, &teapotIndices[6313]);
		rcDrawElements(RC_TRIANGLE_STRIP,  63, RC_UNSIGNED_SHORT, &teapotIndices[6380]);
		rcDrawElements(RC_TRIANGLE_STRIP,  59, RC_UNSIGNED_SHORT, &teapotIndices[6443]);
		rcDrawElements(RC_TRIANGLE_STRIP,  55, RC_UNSIGNED_SHORT, &teapotIndices[6502]);
		rcDrawElements(RC_TRIANGLE_STRIP,  51, RC_UNSIGNED_SHORT, &teapotIndices[6557]);
		rcDrawElements(RC_TRIANGLE_STRIP,  47, RC_UNSIGNED_SHORT, &teapotIndices[6608]);
		rcDrawElements(RC_TRIANGLE_STRIP,  43, RC_UNSIGNED_SHORT, &teapotIndices[6655]);
		rcDrawElements(RC_TRIANGLE_STRIP,  39, RC_UNSIGNED_SHORT, &teapotIndices[6698]);
		rcDrawElements(RC_TRIANGLE_STRIP,  35, RC_UNSIGNED_SHORT, &teapotIndices[6737]);
		rcDrawElements(RC_TRIANGLE_STRIP,  31, RC_UNSIGNED_SHORT, &teapotIndices[6772]);
		rcDrawElements(RC_TRIANGLE_STRIP,  27, RC_UNSIGNED_SHORT, &teapotIndices[6803]);
		rcDrawElements(RC_TRIANGLE_STRIP,  23, RC_UNSIGNED_SHORT, &teapotIndices[6830]);
		rcDrawElements(RC_TRIANGLE_STRIP,  19, RC_UNSIGNED_SHORT, &teapotIndices[6853]);
		rcDrawElements(RC_TRIANGLE_STRIP,  15, RC_UNSIGNED_SHORT, &teapotIndices[6872]);
		rcDrawElements(RC_TRIANGLE_STRIP,  11, RC_UNSIGNED_SHORT, &teapotIndices[6887]);
		rcDrawElements(RC_TRIANGLE_STRIP,   7, RC_UNSIGNED_SHORT, &teapotIndices[6898]);
#endif
	}

protected:
	virtual BOOL OnIntialize(void){
		rcSceneAllInit();

		{
			rcClearColor( 0.73f, 0.2f, 0.23f, 1.0f );			
			rcViewport(0, 0, Width(), Height());

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();
			rcuPerspective( 75.0f, (RCfloat)Width() / (RCfloat)Height(), 0.1f, 100.0f);
			rcuLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

			StaticSceneDraw();
		}

		initLights();
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