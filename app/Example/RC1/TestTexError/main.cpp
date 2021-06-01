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
#include "../common/SceneData.h"

// Test - Texture Error
class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Test - Texture Error", 800, 480){}
	virtual ~Tutorial(void){}

	SceneData sceneData;

	void StaticSceneDraw(void){

		rcStaticSceneBegin();

		//

		rcStaticSceneEnd();
	}

protected:
	virtual BOOL OnIntialize(void){
		
		char foldername[] = "./scenedata/TestTexError/";

		rcDepthBounce(14);

		rcSceneAllInit();

		{
			sceneData.SetFolderName(foldername);

			if(!sceneData.SceneDataLoad(Width(), Height()))
				return FALSE;

			if(!sceneData.KeyAnimationDataLoad())
				return FALSE;

			if(!sceneData.BoneAnimationDataLoad())
				return FALSE;

			if(!sceneData.MeshAnimationDataLoad())
				return FALSE;

			sceneData.InitScene();
			sceneData.InitLights();
			sceneData.InitMaterials();

			StaticSceneDraw();
		}

		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;
		
		rcClearColor( 0.73f, 0.2f, 0.23f, 1.0f );
		sceneData.DrawScene();

		i++;
		return TRUE;
	}
	
	virtual void OnKeyDown(char cKey){
	}
};

Tutorial	g_Tutorial;
