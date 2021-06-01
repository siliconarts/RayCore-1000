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
// DATE    : 7/22/2013 Mon
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "RCFramework.h"

#include <stdio.h>
#include "../common/SceneData.h"

// Demo Application - Church
class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Demo - Church", 800, 480){}
	virtual ~Tutorial(void){}

	SceneData sceneData;

	BOOL    m_static_On;

	void StaticSceneDraw(void){

		rcStaticSceneBegin();

		sceneData.DrawScene();

		rcStaticSceneEnd();
	}

protected:
	virtual BOOL OnIntialize(void){

		char foldername[] = "./scenedata/Demo_Church/";

		m_static_On = TRUE;

		SetDepthBounce(5);
		
		rcSceneAllInit();

		{
			sceneData.SetFolderName(foldername);

			if(!sceneData.SceneDataLoadx(Width(), Height()))
				return FALSE;

			if(!sceneData.KeyAnimationDataLoadx())
				return FALSE;

			if(!sceneData.BoneAnimationDataLoadx())
				return FALSE;

			if(!sceneData.MeshAnimationDataLoadx())
				return FALSE;

			sceneData.InitScene();
			sceneData.InitLights();
			sceneData.InitMaterials();

			if(m_static_On)
				StaticSceneDraw();
		}

		return TRUE;
	}

	virtual BOOL OnDraw(void){
		static int i = 0;
		int nSkipFrame = SkipFrame();
		i += nSkipFrame;

		if(i>=sceneData.FrameCount()) i = 0;

		if(sceneData.FrameCount() > 0) {
			sceneData.InitScene(i);
			if(sceneData.LightAniFlag())	sceneData.AnimationLights(i);

			sceneData.KeyAnimationDrawScene(i);
			sceneData.BoneAnimationDrawScene(i);
			sceneData.MeshAnimationDrawScene(i);
		}

		return TRUE;
	}

	virtual void OnKeyDown(char cKey){
		switch(cKey){
		case '1':	//Select Material : Orgel
			sceneData.SelectMaterial(32);
			break;
		case '2':	//Select Material : Violet Jewel
			sceneData.SelectMaterial(29);
			break;
		case '3':	//Select Material : Green Jewel
			sceneData.SelectMaterial(28);
			break;
		case '4':	//Select Material : Red Jewel
			sceneData.SelectMaterial(30);
			break;
		case '5':	//Select Material : Glass Bead
			sceneData.SelectMaterial(26);
			break;
		default:
			sceneData.OnKeyDown(cKey);
			break;
		}
	}

	virtual void OnMouseEvent(float x, float y, DWORD dwBtn = MOUSE_BTN_NULL){
		sceneData.OnMouseEvent(x, y, dwBtn);
	}
};

Tutorial	g_Tutorial;
