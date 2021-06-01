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
// DATE    : 7/26/2013 Fri
// Author  : Sang Gon, Kim (sgkim@siliconarts.co.kr)
//
//=======================================================================

#include "RCFramework.h"

// Programming Guide - Cornell Box

struct Color3 {
	float r, g, b;
};

struct Pos3 {
	float x, y, z;
};

struct TexC2 {
	float u, v;
};

Pos3 Left_Wall[] = {
	{55.28f, 0.f, 0.f}, {54.96f, 0.f, 55.92f}, {55.60f, 54.88f, 55.92f},
	{55.28f, 0.f, 0.f}, {55.60f, 54.88f, 55.92f}, {55.60f, 54.88f, 0.f}
};

Pos3 Right_Wall[] = {
	{0.f, 54.88f, 0.f}, {0.f, 54.88f, 55.92f}, {0.f, 0.f, 55.92f},
	{0.f, 54.88f, 0.f}, {0.f, 0.f, 55.92f}, {0.f, 0.f, 0.f}
};

Pos3 Floor[] = {
	{55.28f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 55.92f},
	{55.28f, 0.f, 0.f}, {0.f, 0.f, 55.92f}, {54.96f, 0.0f, 55.92f}
};

Pos3 Ceiling[] = {
	{55.60f, 54.88f, 0.f}, {55.60f, 54.88f, 55.92f}, {0.f, 54.88f, 55.92f},
	{55.60f, 54.88f, 0.f}, {0.f, 54.88f, 55.92f}, {0.f, 54.88f, 0.f}
};

Pos3 Back_Wall[] = {
	{0.f, 54.88f, 55.92f}, {55.60f, 54.88f, 55.92f}, {54.96f, 0.f, 55.92f},
	{0.f, 54.88f, 55.92f}, {54.96f, 0.f, 55.92f}, {0.f, 0.f, 55.92f}
};

Pos3 Short_Block[] = {
	{13.00f, 16.50f, 6.50f}, {8.20f, 16.50f, 22.50f}, {24.00f, 16.50f, 27.20f},
	{13.00f, 16.50f, 6.50f}, {24.00f, 16.50f, 27.20f}, {29.00f, 16.50f, 11.40f},
	{29.00f, 0.0f, 11.40f}, {29.00f, 16.50f, 11.40f}, {24.00f, 16.50f, 27.20f},
	{29.00f, 0.0f, 11.40f}, {24.00f, 16.50f, 27.20f}, {24.00f, 0.f, 27.20f},
	{13.00f, 0.0f, 6.50f}, {13.00f, 16.50f, 6.50f}, {29.00f, 16.50f, 11.40f},
	{13.00f, 0.0f, 6.50f}, {29.00f, 16.50f, 11.40f}, {29.00f, 0.f, 11.40f},
	{8.20f, 0.0f, 22.50f}, {8.20f, 16.50f, 22.50f}, {13.00f, 16.50f, 6.50f},
	{8.20f, 0.0f, 22.50f}, {13.00f, 16.50f, 6.50f}, {13.00f, 0.f, 6.50f},
	{24.00f, 0.0f, 27.20f}, {24.00f, 16.50f, 27.20f}, {8.20f, 16.50f, 22.50f},
	{24.00f, 0.0f, 27.20f}, {8.20f, 16.50f, 22.50f}, {8.20f, 0.f, 22.50f}
};

Pos3 Tall_Block[] = {
	{42.30f, 33.00f, 24.70f}, {26.50f, 33.00f, 29.60f}, {31.40f, 33.00f, 45.60f},
	{42.30f, 33.00f, 24.70f}, {31.40f, 33.00f, 45.60f}, {47.20f, 33.00f, 40.60f},
	{42.30f, 0.f, 24.70f}, {42.30f, 33.00f, 24.70f}, {47.20f, 33.00f, 40.60f},
	{42.30f, 0.f, 24.70f}, {47.20f, 33.00f, 40.60f}, {47.20f, 0.f, 40.60f},
	{47.20f, 0.f, 40.60f}, {47.20f, 33.00f, 40.60f}, {31.40f, 33.00f, 45.60f},
	{47.20f, 0.f, 40.60f}, {31.40f, 33.00f, 45.60f}, {31.40f, 0.f, 45.60f},
	{31.40f, 0.f, 45.60f}, {31.40f, 33.00f, 45.60f}, {26.50f, 33.00f, 29.60f},
	{31.40f, 0.f, 45.60f}, {26.50f, 33.00f, 29.60f}, {26.50f, 0.f, 29.60f},
	{26.50f, 0.f, 29.60f}, {26.50f, 33.00f, 29.60f}, {42.30f, 33.00f, 24.70f},
	{26.50f, 0.f, 29.60f}, {42.30f, 33.00f, 24.70f}, {42.30f, 0.f, 24.70f}
};

Color3 White = {1, 1, 1};
Color3 Red = {0.67f, 0.025f, 0.025f};
Color3 Green = {0.025f, 0.236f, 0.025f};
Color3 White1 = {0.9f, 1.0f, 0.9f};
Color3 White2 = {0.9f, 0.9f, 1.0f};
Color3 LightColor = {1, 0.67f, 0.21f};

float Light_Ambient[] = {0.05f, 0.05f, 0.05f};
float Light_Diffuse[] = {0.2f, 0.2f, 0.2f};
float Light_Position1[] = {21.3f, 54.87f, 33.2f};
float Light_Position2[] = {34.3f, 54.87f, 22.7f};
float Light_Position3[] = {21.3f, 54.87f, 22.7f};
float Light_Position4[] = {34.3f, 54.87f, 33.2f};

namespace camera{
	float location[]    = {27.8f, 27.3f,-80.0f};
	float direction[]   = {0, 0, 1};
	float up[]          = {0, 1, 0};
	float right[]       = {-1, 0, 0};

	float focal = 0.0035f;
}


float g_fSpinZ = 0.0f;
float g_fSpinY = 0.0f;
float aniy = 0;
bool  dir = true;

bool Light = true;

RCuint MaterialID_ShortBlock;
RCuint MaterialID_TallBlock;

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Programming Guide - Cornell Box", 800, 480){}
	virtual ~Tutorial(void){}

protected:
	virtual BOOL OnIntialize(void){
		RCuint  materialID;

		rcDepthBounce(14);

		rcSceneAllInit();

		{
			rcClearColor( 0.0f, 0.2f, 0.4f, 1.0f );

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();

			rcFrustum(-1, 1, -0.6f, 0.6f, 1.7f, 1000);

			rcuLookAt(27.8f, 27.3f, -80.0f, 27.8f, 27.3f, 0, 0, 1, 0);

			rcStaticSceneBegin();
			rcEnableClientState(RC_VERTEX_ARRAY);

			rcVertexPointer(3, RC_FLOAT, 0, Left_Wall);
			rcGenMaterials(1, &materialID);
			rcBindMaterial(materialID);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, &Red.r);
			rcDrawArrays( RC_TRIANGLES, 0, 6 );

			rcVertexPointer(3, RC_FLOAT, 0, Right_Wall);
			rcGenMaterials(1, &materialID);
			rcBindMaterial(materialID);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, &Green.r);
			rcDrawArrays( RC_TRIANGLES, 0, 6 );

			rcVertexPointer(3, RC_FLOAT, 0, Floor);
			rcGenMaterials(1, &materialID);
			rcBindMaterial(materialID);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, &White.r);
			rcDrawArrays( RC_TRIANGLES, 0, 6 );

			rcVertexPointer(3, RC_FLOAT, 0, Ceiling);
			rcGenMaterials(1, &materialID);
			rcBindMaterial(materialID);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, &White.r);
			rcDrawArrays( RC_TRIANGLES, 0, 6 );

			rcVertexPointer(3, RC_FLOAT, 0, Back_Wall);
			rcGenMaterials(1, &materialID);
			rcBindMaterial(materialID);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, &White.r);
			rcDrawArrays( RC_TRIANGLES, 0, 6 );

			rcDisableClientState(RC_VERTEX_ARRAY);
			rcStaticSceneEnd();

			rcGenMaterials(1, &MaterialID_ShortBlock);
			rcGenMaterials(1, &MaterialID_TallBlock);
		}

		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;
		i++;

		{
			rcClear( RC_COLOR_BUFFER_BIT | RC_DEPTH_BUFFER_BIT );

			rcMatrixMode( RC_MODELVIEW );
			rcLoadIdentity();

			rcRotatef( -g_fSpinY, 1.0f, 0.0f, 0.0f );
			rcRotatef( -g_fSpinZ, 0.0f, 1.0f, 0.0f );

			///////
			if (Light){
				rcEnable(RC_LIGHTING);
				rcLightfv(RC_LIGHT0,RC_AMBIENT,Light_Ambient);
				rcLightfv(RC_LIGHT0,RC_DIFFUSE,Light_Diffuse);					
				rcLightfv(RC_LIGHT0,RC_POSITION,Light_Position1);
				rcEnable(RC_LIGHT0);

				rcLightfv(RC_LIGHT1, RC_AMBIENT,Light_Ambient);
				rcLightfv(RC_LIGHT1, RC_DIFFUSE,Light_Diffuse);					
				rcLightfv(RC_LIGHT1, RC_POSITION,Light_Position2);
				rcEnable(RC_LIGHT1);

				rcLightfv(RC_LIGHT2, RC_AMBIENT,Light_Ambient);
				rcLightfv(RC_LIGHT2, RC_DIFFUSE,Light_Diffuse);					
				rcLightfv(RC_LIGHT2, RC_POSITION,Light_Position3);
				rcEnable(RC_LIGHT2);

				rcLightfv(RC_LIGHT3, RC_AMBIENT,Light_Ambient);
				rcLightfv(RC_LIGHT3, RC_DIFFUSE, Light_Diffuse);					
				rcLightfv(RC_LIGHT3, RC_POSITION, Light_Position4);
				rcEnable(RC_LIGHT3);
			} else 
				rcDisable(RC_LIGHTING);

			rcEnableClientState(RC_VERTEX_ARRAY);

			rcPushMatrix();
			{
				if (dir)
					aniy += 0.1f;
				else
					aniy -= 0.1f;

				if (aniy < 0) {
					aniy = 0;
					dir = true;
				} else if (aniy > 10) {
					aniy = 10;
					dir = false;
				}
				rcTranslatef(0, aniy, 0);
			}

			rcVertexPointer(3, RC_FLOAT, 0, Short_Block);
			rcBindMaterial(MaterialID_ShortBlock);
			rcColor4f(White1.r, White1.g, White1.b, 0);
			rcDrawArrays( RC_TRIANGLES, 0, 30 );
			rcPopMatrix();

			rcVertexPointer(3, RC_FLOAT, 0, Tall_Block);
			rcBindMaterial(MaterialID_TallBlock);
			rcColor4f(White2.r, White2.g, White2.b, 0);
			rcDrawArrays( RC_TRIANGLES, 0, 30 );

			rcDisableClientState(RC_VERTEX_ARRAY);
		}

		return TRUE;
	}
};

Tutorial	g_Tutorial;
