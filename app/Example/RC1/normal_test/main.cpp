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
// DATE    : 7/18/2013 Thu
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
	Tutorial(void) : RCFramework("normal_test", 800, 480){}
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

		char foldername[] = "./scenedata/normal_test/";

		m_static_On = FALSE;

		rcDepthBounce(5);
		
		rcSceneAllInit();

		{
			sceneData.SetFolderName(foldername);

			if(!sceneData.SceneDataLoad(Width(), Height()))
				return FALSE;

// 			if(!sceneData.KeyAnimationDataLoad())
// 				return FALSE;
// 
// 			if(!sceneData.BoneAnimationDataLoad())
// 				return FALSE;
// 
// 			if(!sceneData.MeshAnimationDataLoad())
// 				return FALSE;

//			printf("[Init] Set\n");
			sceneData.InitScene();
			sceneData.InitLights();
			sceneData.InitMaterials();

			if(m_static_On)
				StaticSceneDraw();
		}

		return TRUE;
	}

	virtual BOOL OnDraw(void){
//		printf("[OnDraw] \n");
		sceneData.DrawScene();

		return TRUE;
	}

	virtual void OnKeyDown(char cKey){
		switch(cKey){
		case '1':	//Select Material : Orgel
			sceneData.SelectMaterial(30);
			break;
		case '2':	//Select Material : Violet Jewel
			sceneData.SelectMaterial(28);
			break;
		case '3':	//Select Material : Green Jewel
			sceneData.SelectMaterial(27);
			break;
		case '4':	//Select Material : Red Jewel
			sceneData.SelectMaterial(29);
			break;
		case '5':	//Select Material : Glass Bead
			sceneData.SelectMaterial(26);
			break;
		default:
			sceneData.OnKeyDown(cKey);
			break;
		}
	}
};

Tutorial	g_Tutorial;
