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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "RCFramework.h"

// Simple Drawing Test : Bone Animation

#include "../common/DIBImage.h"

#define PlaneVertex			0
#define PlaneVertexElement	1
#define LeafVertex			2
#define LeafVertexElement	3
#define MatrixIndexArray	4
#define WeightArray			5
unsigned int BufferArray[6];

#define LeafTexture			0
unsigned int TextureArray[1];

#define PlaneMaterial		0
#define LeafMaterial		1
unsigned int MaterialArray[2];

float g_fSpinZ = 0.0f;


struct Color {
	float r, g, b;
};

struct Pos {
	float x, y, z;
};

struct TexC {
	float u, v;
};

struct PosNormalTexC {
	Pos xyz;
	Pos normal;
	TexC uv;
};

struct MatrixP {
	float m[16];
};

#include "LeafVertex.h"
#include "LeafMatrixPalette.h"

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("BoneAnimation", 800, 480){}
	virtual ~Tutorial(void){}
	
	void Load(RGBImageRec **pBitmap){

		pBitmap[0] = (RGBImageRec *)DIBImageLoad("./scenedata/BoneAnimation/grass04.bmp", 3);	
	}

	void CreateTexture(unsigned int *textureArray) {

		RGBImageRec *pBitmap[1];
		int i=0;

		for(i=0; i<1; i++) {
			pBitmap[i] = NULL;
		}
		Load(pBitmap);

		rcGenTextures(1, textureArray);

		for(i=0; i<1; i++) {
			rcBindTexture(RC_TEXTURE_2D, textureArray[i]);
			rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[i]->sizeX, pBitmap[i]->sizeY, 0, RC_RGB, RC_UNSIGNED_BYTE, pBitmap[i]->data);	
			rcTexParameteri(RC_TEXTURE_2D,RC_TEXTURE_MIN_FILTER,RC_LINEAR_MIPMAP_NEAREST);
			rcTexParameteri(RC_TEXTURE_2D,RC_TEXTURE_MAG_FILTER,RC_LINEAR_MIPMAP_NEAREST);
		}

		for(i=0; i<1; i++) {
			if(pBitmap[i]) {
				if(pBitmap[i]->data)	free(pBitmap[i]->data);
				free(pBitmap[i]);
			}
		}
	}

	void DeleteTexture(void){
		rcDeleteTextures(1, TextureArray);
	}

	void GenBuffer(void){	
		rcGenBuffers(6, BufferArray);

		rcBindBuffer(RC_ARRAY_BUFFER, BufferArray[PlaneVertex]);
		rcBufferData(RC_ARRAY_BUFFER, sizeof(g_planeVertices), g_planeVertices, RC_STATIC_DRAW);
		rcBindBuffer(RC_ARRAY_BUFFER, 0);

		rcBindBuffer(RC_ELEMENT_ARRAY_BUFFER, BufferArray[PlaneVertexElement]);
		rcBufferData(RC_ELEMENT_ARRAY_BUFFER, sizeof(g_planeIndices), g_planeIndices, RC_STATIC_DRAW);
		rcBindBuffer(RC_ELEMENT_ARRAY_BUFFER, 0);

		rcBindBuffer(RC_ARRAY_BUFFER, BufferArray[LeafVertex]);
		rcBufferData(RC_ARRAY_BUFFER, sizeof(g_leafVertices), g_leafVertices, RC_STATIC_DRAW);
		rcBindBuffer(RC_ARRAY_BUFFER, 0);

		rcBindBuffer(RC_ELEMENT_ARRAY_BUFFER, BufferArray[LeafVertexElement]);
		rcBufferData(RC_ELEMENT_ARRAY_BUFFER, sizeof(g_leafIndices), g_leafIndices, RC_STATIC_DRAW);
		rcBindBuffer(RC_ELEMENT_ARRAY_BUFFER, 0);

		rcBindBuffer(RC_ARRAY_BUFFER, BufferArray[MatrixIndexArray]);
		rcBufferData(RC_ARRAY_BUFFER, sizeof(g_leafMatrixIndices), g_leafMatrixIndices, RC_STATIC_DRAW);
		rcBindBuffer(RC_ARRAY_BUFFER, 0);

		rcBindBuffer(RC_ARRAY_BUFFER, BufferArray[WeightArray]);
		rcBufferData(RC_ARRAY_BUFFER, sizeof(g_leafWeight), g_leafWeight, RC_STATIC_DRAW);
		rcBindBuffer(RC_ARRAY_BUFFER, 0);
	}

	void DeleteBuffer(void){	
		rcBindBuffer(RC_ELEMENT_ARRAY_BUFFER, 0);
		rcBindBuffer(RC_ARRAY_BUFFER, 0);
		rcDeleteBuffers(6, BufferArray);
	}

	void GenMaterial(void){	
		rcGenMaterials(2, MaterialArray);

		float material[2][8]={
			{
				0.000000f, 0.000000f, 0.000000f,//ambient & diffuse
				0.900000f, 0.900000f, 0.900000f,//specular
				10.000000f,						//exponent
				1.000000f						//reflectance
			}, 
			{
				1.000000f, 1.000000f, 1.000000f,//ambient & diffuse
				0.360000f, 0.360000f, 0.360000f,//specular
				19.999998f,						//exponent
				0.000000f						//reflectance
			},
		};

		rcBindMaterial(MaterialArray[PlaneMaterial]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, &material[PlaneMaterial][0]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, &material[PlaneMaterial][3]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SHININESS, &material[PlaneMaterial][6]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_REFLECTION, &material[PlaneMaterial][7]);

		rcBindMaterial(MaterialArray[LeafMaterial]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, &material[LeafMaterial][0]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, &material[LeafMaterial][3]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SHININESS, &material[LeafMaterial][6]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_REFLECTION, &material[LeafMaterial][7]);
	}

	void DeleteMaterial(void){	
		rcDeleteMaterials(2, MaterialArray);
	}

	void StaticSceneDraw(void){
		rcStaticSceneBegin();

		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();

		rcDisable(RC_TEXTURE_2D);

		rcEnableClientState(RC_VERTEX_ARRAY);
		rcEnableClientState(RC_NORMAL_ARRAY);

		rcBindBuffer(RC_ARRAY_BUFFER, BufferArray[PlaneVertex]);
		rcBindBuffer(RC_ELEMENT_ARRAY_BUFFER, BufferArray[PlaneVertexElement]);

		rcVertexPointer(3, RC_FLOAT, 32, (RCvoid *)0);
		rcNormalPointer(RC_FLOAT, 32, (RCvoid *)12);

		rcBindMaterial(MaterialArray[PlaneMaterial]);
		rcDrawElements( RC_TRIANGLES, sizeof(g_planeIndices)/sizeof(RCubyte), RC_UNSIGNED_BYTE, (RCvoid *)0);

		rcBindBuffer(RC_ARRAY_BUFFER, 0);
		rcBindBuffer(RC_ELEMENT_ARRAY_BUFFER, 0);

		rcDisableClientState(RC_VERTEX_ARRAY);
		rcDisableClientState(RC_NORMAL_ARRAY);

		rcStaticSceneEnd();
	}

	void SetMaterial(void){
		rcBindMaterial(MaterialArray[LeafMaterial]);
		rcEnable(RC_TEXTURE_2D);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[LeafTexture]);
	}

	void DynamicSceneDraw(int iFrame){
		rcEnable(RC_MATRIX_PALETTE_OES);

		//Set the Matrix Palette
		rcMatrixMode( RC_MATRIX_PALETTE_OES );
		for(int k=0; k<3; k++){
			rcCurrentPaletteMatrixOES(k);
			rcLoadMatrixf(g_leafMaxrix[iFrame*3+k].m);
		}

		//Model View
		rcMatrixMode(RC_MODELVIEW);
		rcLoadIdentity();
		rcRotatef( -g_fSpinZ, 0.0f, 0.0f, 1.0f );g_fSpinZ -= 14.4f;
		rcTranslatef(-0.642648f,-0.379345f, 0.0f);

		rcEnableClientState(RC_WEIGHT_ARRAY_OES);
		rcEnableClientState(RC_MATRIX_INDEX_ARRAY_OES);

		rcEnableClientState(RC_VERTEX_ARRAY);
		rcEnableClientState(RC_NORMAL_ARRAY);
		rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

		//Bone Animation Info
		rcBindBuffer(RC_ARRAY_BUFFER, BufferArray[MatrixIndexArray]);
		rcMatrixIndexPointerOES(3, RC_UNSIGNED_BYTE, 0, (RCvoid *)0);

		rcBindBuffer(RC_ARRAY_BUFFER, BufferArray[WeightArray]);
		rcWeightPointerOES(3, RC_FLOAT, 0, (RCvoid *)0);

		//Leaf
		rcBindBuffer(RC_ARRAY_BUFFER, BufferArray[LeafVertex]);
		rcBindBuffer(RC_ELEMENT_ARRAY_BUFFER, BufferArray[LeafVertexElement]);

		rcVertexPointer(3, RC_FLOAT, 32, (RCvoid *)0);
		rcNormalPointer(RC_FLOAT, 32, (RCvoid *)12);
		rcTexCoordPointer(2, RC_FLOAT, 32, (RCvoid *)24);

		rcDrawElements( RC_TRIANGLES, sizeof(g_leafIndices)/sizeof(RCubyte), RC_UNSIGNED_BYTE, (RCvoid *)0);

		rcBindBuffer(RC_ARRAY_BUFFER, 0);
		rcBindBuffer(RC_ELEMENT_ARRAY_BUFFER, 0);

		rcDisableClientState(RC_VERTEX_ARRAY);
		rcDisableClientState(RC_NORMAL_ARRAY);
		rcDisableClientState(RC_TEXTURE_COORD_ARRAY);

		rcDisableClientState(RC_WEIGHT_ARRAY_OES);
		rcDisableClientState(RC_MATRIX_INDEX_ARRAY_OES);

		rcDisable(RC_MATRIX_PALETTE_OES);
	}

protected:
	virtual BOOL OnIntialize(void){
		RCfloat position[] = {2.114956f, -2.560182f, 7.125879f, 0};
		RCfloat spotdir[] = { 0, 0, 0 };
		RCfloat ambient[] = {1.0f, 1.0f, 1.0f, 0.0f};
		RCfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		RCfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
		RCint maxBoneCount=0;

		rcEnable(RC_LIGHTING);
		rcLightfv(RC_LIGHT0, RC_POSITION, position);
		rcLightfv(RC_LIGHT0, RC_AMBIENT, ambient);
		rcLightfv(RC_LIGHT0, RC_DIFFUSE, diffuse);
		rcLightfv(RC_LIGHT0, RC_SPECULAR, specular);
		rcLightfv(RC_LIGHT0, RC_SPOT_DIRECTION, spotdir);
		rcEnable(RC_LIGHT0);

		rcGetIntegerv(RC_MAX_PALETTE_MATRICES_OES, &maxBoneCount);

		rcDisable(RC_USE_TRANSMITTANCE_SHADOW);
		rcDisable(RC_USE_TEXTURE_ALPHA_SHADOW);
		rcDisable(RC_USE_TEXTURE_ONLY);

		CreateTexture(TextureArray);
		GenBuffer();
		GenMaterial();

		rcSceneAllInit();

		{
			rcClearColor( 0.73f, 0.2f, 0.23f, 1.0f );			
			rcViewport(0, 0, Width(), Height());

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();
			rcuPerspective( 30.0f, (RCfloat)Width() / (RCfloat)Height(), 0.1f, 100.0f);
			rcuLookAt(-0.000001f, -12.859189f, 9.126232f, 0, 0.659648f, 1.180530f, 0, 0.506710f, 0.862117f);

			StaticSceneDraw();
		}

		SetMaterial();
		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;

		DynamicSceneDraw(i%50);
		i++;
		return TRUE;
	}
};

Tutorial	g_Tutorial;