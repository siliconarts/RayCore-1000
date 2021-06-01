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
// DATE    : 8/6/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "RCFramework.h"

#include <stdio.h>
#include "../common/SceneData.h"

// Scene player
class Tutorial : public RCFramework
{
	char	m_sPlayerTitle[256];
public:
	Tutorial(void) : RCFramework(m_sPlayerTitle, 800, 480){
		strcpy(m_sPlayerTitle, "ScenePlayer");
	}
	virtual ~Tutorial(void){}

	SceneData sceneData;

	void StaticSceneDraw(void){
		rcStaticSceneBegin();
		sceneData.DrawScene();
		rcStaticSceneEnd();
	}

protected:
	virtual BOOL OnIntialize(void){
		if(m_Cmd.argc < 2){
			printf("Usage : ScenePlayer scene_path [screen_width] [screen_height]\n");
			return FALSE;
		}

		rcSceneAllInit();
		{
			sceneData.SetFolderName(m_Cmd.argv[1]);

			if(!sceneData.SceneDataLoad(Width(), Height()))	return FALSE;
			if(!sceneData.KeyAnimationDataLoad()) return FALSE;
			if(!sceneData.BoneAnimationDataLoad()) return FALSE;
			if(!sceneData.MeshAnimationDataLoad()) return FALSE;

			sceneData.InitScene();
			sceneData.InitLights();
			sceneData.InitMaterials();

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
		sceneData.OnKeyDown(cKey);
	}

	virtual void OnMouseEvent(float x, float y, DWORD dwBtn = MOUSE_BTN_NULL){
		sceneData.OnMouseEvent(x, y, dwBtn);
	}
};

Tutorial	g_Tutorial;
