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

// Simple Drawing Test : Clear
class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Clear", 800, 480){}
	virtual ~Tutorial(void){}

protected:
	virtual BOOL OnIntialize(void){
		rcSceneAllInit();
		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;
		{	// clearing
			static const RCfloat clearcolor[][3] = {
				{0.f, 0.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
			};
			rcClearColor(clearcolor[i%3][0], clearcolor[i%3][1], clearcolor[i%3][2], 1.f);
		}
		i++;
		return TRUE;
	}
};

Tutorial	g_Tutorial;
