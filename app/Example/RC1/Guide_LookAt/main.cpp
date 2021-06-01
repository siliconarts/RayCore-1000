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

// Programming Guide - LookAt

struct Pos {
	float x, y, z;
};

RCubyte g_cubeIndices[] = {
	0, 1, 2,
};

struct Pos g_TrianglePos[] = {
	{  0.0f, 1.0f, 0.0f }, // 0
	{ -1.0f, -1.0f, 0.0f }, // 1
	{  1.0f, -1.0f, 0.0f }, // 2
};

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Programming Guide - LookAt", 800, 480){}
	virtual ~Tutorial(void){}

	void StaticSceneDraw(void){
		rcStaticSceneBegin();

		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, 0.0f, -3.0f );

		rcEnableClientState(RC_VERTEX_ARRAY);

		float color[4]={1,1,0,0};

		rcDisable(RC_TEXTURE_2D);

		rcVertexPointer(3, RC_FLOAT, 0, g_TrianglePos);
		rcBindMaterial(1);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color);
		rcDrawElements( RC_TRIANGLE_FAN, 3, RC_UNSIGNED_BYTE, g_cubeIndices );

		rcStaticSceneEnd();
	}

	void DynamicSceneDraw(void){}

protected:
	virtual BOOL OnIntialize(void){
		rcSceneAllInit();

		{
			rcClearColor( 0.73f, 0.2f, 0.23f, 1.0f );

			rcViewport(0, 0, 800, 480);
			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();
			rcuPerspective( 60.0f, 800.0f / 480.0f, 0.1f, 100.0f);
			rcuLookAt(0.3f, 0, 1, 0, 0, 0, 0, 1, 0);

			StaticSceneDraw();
		}
		
		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;
		i++;
		
		DynamicSceneDraw();
		
		return TRUE;
	}
};

Tutorial	g_Tutorial;
