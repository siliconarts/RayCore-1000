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
#include <stdlib.h>
#include <memory.h>
#include "RCFramework.h"

// Simple Drawing Test : Each Function Test With Cube Drawing

//#define FLAG_FILEOPEN
#define IS_REFLECTION		TRUE

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

struct PosNormalTexC {
	Pos xyz;
	Pos normal;
	TexC uv;
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

void TestParse(void){
	FILE *f=NULL;
	FILE *fw=NULL;
	FILE *fm=NULL;
	int dumyI[3];
	int id;
	float wt;
	float* pMt=NULL;
	float dumyf[2];
	PosNormalTexC aData[1024];
	int aDataIDs[1024];
	int idxVertex=0;
	int weightCount=0;
	int frameCount=0;
	float boneWeight[1024][3];			//By Vertex
	float boneMatrix[1024][3][16];		//By Frame

	memset(&aData[0], 0, sizeof(PosNormalTexC)*1024);
	memset(&aDataIDs[0], 0, sizeof(int)*1024);
	memset(&boneWeight[0], 0, sizeof(float)*1024*3);
	memset(&boneMatrix[0], 0, sizeof(float)*1024*3*16);

	//vertices
	f = fopen("C:\\b_KD_prim_pool_leaf60.txt", "r");
	if(f != NULL) {

		while(!feof(f)) {
			if(fscanf(f, "%d %d %d", &dumyI[0], &dumyI[1], &dumyI[2]) != 3) continue;
			for(int k=0; k<3; k++) {
				aDataIDs[idxVertex] = dumyI[k];
				fscanf(f, "%f %f %f", &aData[idxVertex].xyz.x, &aData[idxVertex].xyz.y, &aData[idxVertex].xyz.z);
				fscanf(f, "%f %f %f", &aData[idxVertex].normal.x, &aData[idxVertex].normal.y, &aData[idxVertex].normal.z);
				fscanf(f, "%f %f", &aData[idxVertex].uv.u, &aData[idxVertex].uv.v);				
				idxVertex++;
			}
			fscanf(f, "%f %f", &dumyf[0], &dumyf[1]);
		}
		fclose(f);
	}

	//weight
	f = fopen("C:\\leaf60.txt", "r");
	if(f != NULL) {
		while(!feof(f)) {
			if(fscanf(f, "%d %d", &dumyI[0], &dumyI[1]) != 2) continue;
			for(int k=0; k<dumyI[1]; k++) {
				fscanf(f, "%d %f", &id, &wt);
				boneWeight[weightCount][id] = wt;
			}
			weightCount++;
		}
		fclose(f);
	}

	//Bone 14, 15, 18
	for(int i=0; i<3; i++){
		if(i == 0)		f = fopen("C:\\b_matrix_Bone14.txt", "r");
		else if(i == 1)	f = fopen("C:\\b_matrix_Bone15.txt", "r");
		else if(i == 2)	f = fopen("C:\\b_matrix_Bone18.txt", "r");		

		if(f != NULL) {
			frameCount = 0;
			while(!feof(f)) {
				pMt = &boneMatrix[frameCount][i][0];
				if(fscanf(f, "%d", &dumyI[0]) != 1) continue;
				fscanf(f, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
					&pMt[0], &pMt[1], &pMt[2], &pMt[3], 
					&pMt[4], &pMt[5], &pMt[6], &pMt[7], 
					&pMt[8], &pMt[9], &pMt[10], &pMt[11], 
					&pMt[12], &pMt[13], &pMt[14], &pMt[15]);
				frameCount++;
			}
			fclose(f);
		}
	}

	f = fopen("C:\\resultVertex.txt", "w");
	if(f != NULL) {
		fprintf(f, "struct PosNormalTexC g_leafVertices[] = \n");
		fprintf(f, "{ \n");
		for(int i=0; i<idxVertex; i++){
			fprintf(f, "\t{ { %ff, %ff, %ff }, { %ff, %ff, %ff }, { %ff, %ff } }, \n", 
				aData[i].xyz.x, aData[i].xyz.y, aData[i].xyz.z,
				aData[i].normal.x, aData[i].normal.y, aData[i].normal.z,
				aData[i].uv.u, aData[i].uv.v);
		}
		fprintf(f, "}; \n");

		fclose(f);
	}

	f = fopen("C:\\resultIndex.txt", "w");
	if(f != NULL) {
		fprintf(f, "RCubyte g_leafIndices[] = \n");
		fprintf(f, "{ \n");
		for(int i=0; i<idxVertex; i+=3){
			fprintf(f, "\t%d, %d, %d, \n", i, i + 1, i + 2);
		}
		fprintf(f, "}; \n");

		fclose(f);
	}

	f = fopen("C:\\resultWeight.txt", "w");
	if(f != NULL) {
		fprintf(f, "float g_leafWeight[] = \n");
		fprintf(f, "{ \n");
		for(int i=0; i<idxVertex; i++){
			fprintf(f, "\t%ff, %ff, %ff, \n", boneWeight[aDataIDs[i]][0], boneWeight[aDataIDs[i]][1], boneWeight[aDataIDs[i]][2]);
		}
		fprintf(f, "}; \n");

		fclose(f);
	}

	f = fopen("C:\\resultMatrixIndices.txt", "w");
	if(f != NULL) {
		fprintf(f, "RCubyte g_leafMatrixIndices[] = \n");
		fprintf(f, "{ \n");
		for(int i=0; i<idxVertex; i++){
			fprintf(f, "\t%d, %d, %d, \n", 0, 1, 2);
		}
		fprintf(f, "}; \n");

		fclose(f);
	}

	f = fopen("C:\\resultMatrix.txt", "w");
	if(f != NULL) {
		fprintf(f, "MatrixP g_leafMaxrix[] = \n");
		fprintf(f, "{ \n");
		for(int i=0; i<frameCount; i++){
			for(int k=0; k<3; k++){
				fprintf(f, "\t{%ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff}, \n", 
					boneMatrix[i][k][0], boneMatrix[i][k][1], boneMatrix[i][k][2], boneMatrix[i][k][3], 
					boneMatrix[i][k][4], boneMatrix[i][k][5], boneMatrix[i][k][6], boneMatrix[i][k][7], 
					boneMatrix[i][k][8], boneMatrix[i][k][9], boneMatrix[i][k][10], boneMatrix[i][k][11], 
					boneMatrix[i][k][12], boneMatrix[i][k][13], boneMatrix[i][k][14], boneMatrix[i][k][15]);
			}
			if(i < frameCount - 1) fprintf(f, " \n");
		}
		fprintf(f, "}; \n");

		fclose(f);
	}
}

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("TestFunction", 800, 480){}
	virtual ~Tutorial(void){}

	float aVertex[10240];
	float aNormal[10240];
	unsigned short aIndex[10240];

	void FileLoad(void){
		FILE *f=NULL;
		float vertexf[3], normalf[3], texturef[2], dumyf[2];
		int idx=0;
		int triCount=0;

		memset(&aVertex[0], 0, sizeof(float)*10240);
		memset(&aNormal[0], 0, sizeof(float)*10240);
		memset(&aIndex[0], 0, sizeof(unsigned short)*10240);

		f = fopen("./scenedata/TestFunction/f_KD_prim_pool.txt", "r");
		if(f != NULL) {

			while(!feof(f)) {
				//Trianle Info.
				for(int k=0; k<3; k++) {
					fscanf(f, "%f %f %f", &vertexf[0], &vertexf[1], &vertexf[2]);
					fscanf(f, "%f %f %f", &normalf[0], &normalf[1], &normalf[2]);
					fscanf(f, "%f %f", &texturef[0], &texturef[1]);
					for(int n=0; n<3; n++) {
						aVertex[idx] = vertexf[n];
						aNormal[idx] = normalf[n];
						aIndex[idx] = idx;
						idx++;
					}
				}
				triCount++;
				fscanf(f, "%f %f", &dumyf[0], &dumyf[1]);
			}
			fclose(f);
		}
	}

	void LightOnOff(int iFrame){
#ifndef FLAG_FILEOPEN
		switch(iFrame)
		{
		case 1:
			rcDisable(RC_LIGHT0);
			break;
		case 2:
			rcEnable(RC_LIGHT0);
			rcDisable(RC_LIGHT1);
			break;
		case 3:
			rcEnable(RC_LIGHT1);
			rcDisable(RC_LIGHT2);
			break;
		case 4:
			rcEnable(RC_LIGHT2);
			rcDisable(RC_LIGHTING);
			break;
		case 5:
			rcEnable(RC_LIGHTING);
			break;
		}
#endif
	}

	void StaticSceneDraw(void){
		rcStaticSceneBegin();

		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, 0.0f, -3.0f );
		rcScalef( 15.0f, 15.0f, 15.0f );

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

#ifndef FLAG_FILEOPEN
		//rcBindMaterial(1);
		//rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[0]);
		//rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[0] );

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
#else
		rcBindMaterial(1);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[0]);
		rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[4] );

		rcBindMaterial(2);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, color[1]);
		rcDrawElements( RC_TRIANGLE_FAN, 4, RC_UNSIGNED_BYTE, &g_cubeIndices[20] );
#endif

		rcStaticSceneEnd();
	}

	void SetMaterial(void){
		if(IS_REFLECTION) {
			//Material Setting
			RCfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
			RCfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
			RCfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
			float reflect = 0.0f;
			float transmit = 0.0f;
			float reflect_init[] = {0.0f, 0.0f, 0.0f};
			float refract_index = 1.0f;

			ambient[0] = ambient[1] = ambient[2] = 0.2f;
			diffuse[0] = diffuse[1] = diffuse[2] = 0.8f;
			specular[0] = specular[1] = specular[2] = 1.0f;
			reflect = 0.5f;
			transmit = 0.5f;
			refract_index = 0.8f;

			rcBindMaterial(7);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT, ambient);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, diffuse);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specular);
			rcMaterialf (RC_FRONT_AND_BACK, RC_REFLECTION, reflect);
			rcMaterialf (RC_FRONT_AND_BACK, RC_TRANSMITTANCE, transmit);
			rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, refract_index);
		}
	}

	void DynamicSceneDraw(){
		RCfloat mat[16];
		
#ifdef FLAG_FILEOPEN
		float shininess = 15.0f;
		float diffuseColor[4] = {0.929524f, 0.796542f, 0.178823f, 1};
		float specularColor[4] = {1.00000f, 0.980392f, 0.549020f, 1.0f};
		float reflect_init = 0.0f;
		float refract_index = 1.0f;

		// set specular and shiniess using rcMaterial (gold-yellow)
		rcBindMaterial(7);
		rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS, shininess); // range 0 ~ 128
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specularColor);				
		if(!IS_REFLECTION) {
			rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, diffuseColor);

			rcMaterialf (RC_FRONT_AND_BACK, RC_REFLECTION, reflect_init);
			rcMaterialf (RC_FRONT_AND_BACK, RC_TRANSMITTANCE, reflect_init);
			rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, refract_index);
		}

		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, 0.4f, -2.0f );

		rcScalef(1.5f, -1.5f, 1.5f);
		rcRotatef( -g_fSpinX, 1.0f, 0.0f, 0.0f );
		rcRotatef( -g_fSpinY, 0.0f, 1.0f, 0.0f );

		rcEnableClientState(RC_NORMAL_ARRAY);
		rcEnableClientState(RC_VERTEX_ARRAY);

		rcNormalPointer(RC_FLOAT, 0, aNormal);
		rcVertexPointer(3, RC_FLOAT, 0, aVertex);

		for(int a=0; a<triCount; a++) {
			rcDrawElements(RC_TRIANGLES, 3, RC_UNSIGNED_SHORT, &aIndex[3*a]);
		}
#else		
		rcMatrixMode( RC_MODELVIEW );
		rcLoadIdentity();
		rcTranslatef( 0.0f, 0.0f, -3.0f );
		rcRotatef( -g_fSpinX, 1.0f, 0.0f, 0.0f );
		rcRotatef( -g_fSpinY, 0.0f, 1.0f, 0.0f );

		rcEnableClientState(RC_VERTEX_ARRAY);

		rcVertexPointer(3, RC_FLOAT, 0, g_cubePos);

		if(!IS_REFLECTION)	rcBindMaterial(2);
		rcDrawArrays( RC_TRIANGLE_FAN, 4, 4 );

		if(!IS_REFLECTION)	rcBindMaterial(3);
		rcDrawArrays( RC_TRIANGLE_FAN, 8, 4 );

		if(!IS_REFLECTION)	rcBindMaterial(4);
		rcDrawArrays( RC_TRIANGLE_FAN, 12, 4 );

		if(!IS_REFLECTION)	rcBindMaterial(5);
		rcDrawArrays( RC_TRIANGLE_FAN, 16, 4 );

		if(!IS_REFLECTION)	rcBindMaterial(6);
		rcDrawArrays( RC_TRIANGLE_FAN, 20, 4 );

		if(!IS_REFLECTION)	rcBindMaterial(1);
		rcDrawArrays( RC_TRIANGLE_FAN, 0, 4 );

		rcPushMatrix();
		rcGetFloatv(RC_MODELVIEW_MATRIX, mat);
		rcPopMatrix();

		rcBindBuffer(RC_ARRAY_BUFFER, 0);
#endif
	}

protected:
	virtual BOOL OnIntialize(void){
		//ASE Parsing File Converting.....
		//TestParse();

		rcSceneAllInit();

		{
#ifndef FLAG_FILEOPEN
			rcClearColor( 0.73f, 0.2f, 0.23f, 1.0f );

			rcViewport(0, 0, Width(), Height());
			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();

			rcuPerspective( 120.0f, (RCfloat)Width() / (RCfloat)Height(), 0.1f, 100.0f);
			rcuLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

			//Light Setting		
			RCfloat lightPos[] = { 0.0f, 0.0f, 4.0f, 1.0f };	
			RCfloat spotdir[] = { 0.0f, 0.0f, -1.0f };
			RCfloat ambientLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
			RCfloat diffuseLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
			RCfloat specularLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
			RCfloat specref[] = { -2.0f, 2.0f, 2.0f, 1.0f };
			RCfloat spotexponent=1.5f;
			RCfloat incone=10.f;
			RCfloat outcone=35.f;
			RCboolean isEnabled=false;

			RCfloat ambientLight2[] = { 0.25f, 0.25f, 0.25f, 1.0f };
			RCfloat diffuseLight2[] = { 0.3f, 0.3f, 0.3f, 1.0f };
			RCfloat specularLight2[] = { 0.9f, 0.9f, 0.9f, 1.0f };
			RCfloat dlPos[] = { 1.0f, 0.0f, 4.0f, 0.0f };
			RCfloat dlDir[] = { -1.0f, -1.0f, -1.0f };

			RCfloat ambientLight3[] = { 0.25f, 0.25f, 0.25f, 1.0f };
			RCfloat diffuseLight3[] = { 0.3f, 0.3f, 0.3f, 1.0f };
			RCfloat specularLight3[] = { 0.9f, 0.9f, 0.9f, 1.0f };
			RCfloat plPos[] = { 1.0f, 1.0f, 0.0f, 1.0f };	
			RCfloat plDir[] = { 0.0f, 0.0f, 0.0f };

			rcEnable(RC_LIGHTING);
			rcEnable(RC_LIGHT0);
			rcEnable(RC_LIGHT1);
			rcEnable(RC_LIGHT2);

			//Spot Light
			rcLightfv(RC_LIGHT0, RC_AMBIENT, ambientLight);
			rcLightfv(RC_LIGHT0, RC_DIFFUSE, diffuseLight);
			rcLightfv(RC_LIGHT0, RC_POSITION, lightPos);
			rcLightfv(RC_LIGHT0, RC_SPECULAR, specularLight);
			rcLightfv(RC_LIGHT0, RC_SPOT_DIRECTION, spotdir);
			rcLightf(RC_LIGHT0, RC_SPOT_INNER_CONE, incone);
			rcLightf(RC_LIGHT0, RC_SPOT_OUTER_CONE, outcone);
			//rcLightf(RC_LIGHT0, RC_SPOT_CUTOFF, outcone);
			rcLightf(RC_LIGHT0, RC_SPOT_EXPONENT, spotexponent);
			//rcLightf(RC_LIGHT0, RC_START_ATTENUATION, 1.f);
			//rcLightf(RC_LIGHT0, RC_END_ATTENUATION, 25.f);
			//rcLightf(RC_LIGHT0, RC_CONSTANT_ATTENUATION, 0.1f);
			//rcLightf(RC_LIGHT0, RC_LINEAR_ATTENUATION, 0.05f);

			//Directional Light
			rcLightfv(RC_LIGHT1, RC_AMBIENT, ambientLight2);
			rcLightfv(RC_LIGHT1, RC_DIFFUSE, diffuseLight2);
			rcLightfv(RC_LIGHT1, RC_POSITION, dlPos);
			rcLightfv(RC_LIGHT1, RC_SPOT_DIRECTION, dlDir);

			//Point Light
			rcLightfv(RC_LIGHT2, RC_AMBIENT, ambientLight3);
			rcLightfv(RC_LIGHT2, RC_DIFFUSE, diffuseLight3);
			rcLightfv(RC_LIGHT2, RC_POSITION, plPos);
			rcLightfv(RC_LIGHT2, RC_SPOT_DIRECTION, plDir);

			isEnabled = rcIsEnabled(RC_LIGHTING);
			isEnabled = rcIsEnabled(RC_LIGHT0);
			isEnabled = rcIsEnabled(RC_LIGHT1);
			isEnabled = rcIsEnabled(RC_LIGHT2);
			isEnabled = rcIsEnabled(RC_LIGHT3);

			rcBindMaterial(6);
			rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specref);

			//
			RCfloat amblg[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			RCfloat diflg[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			RCfloat pos[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			RCfloat spr[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			RCfloat sprf[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			RCfloat spd[] = { 0.0f, 0.0f, 0.0f };
			RCfloat ctf = 0.0f;

			rcGetLightfv(RC_LIGHT0, RC_AMBIENT, amblg);
			rcGetLightfv(RC_LIGHT0, RC_DIFFUSE, diflg);
			rcGetLightfv(RC_LIGHT0, RC_POSITION, pos);
			rcGetLightfv(RC_LIGHT0, RC_SPECULAR, spr);
			rcGetLightfv(RC_LIGHT0, RC_SPOT_DIRECTION, spd);
			rcGetLightfv(RC_LIGHT0, RC_SPOT_CUTOFF, &ctf);

			rcGetMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, sprf);

			const RCubyte *pByte=NULL;
			pByte = rcGetString(RC_VENDOR);
			printf("%s \n", pByte);
			pByte = rcGetString(RC_RENDERER);
			printf("%s \n", pByte);
			pByte = rcGetString(RC_VERSION);
			printf("%s \n", pByte);
			pByte = rcGetString(RC_EXTENSIONS);
			printf("%s \n", pByte);

			RCfloat mat[16];
			RCint depth;
			rcGetIntegerv(RC_MODELVIEW_STACK_DEPTH, &depth);
			rcGetIntegerv(RC_MAX_MODELVIEW_STACK_DEPTH, &depth);

			rcPushMatrix();		
			rcGetFloatv(RC_PROJECTION_MATRIX, mat);
			rcPopMatrix();

#else
			RCfloat position[] = {3.475039, -0.100748, 7.712855, 1};
			//RCfloat spotdir[] = { -3.475039, 0.100748, -7.712855 };
			RCfloat spotdir[] = { 0, 0, -10 };
			RCfloat ambient[] = {1.0f, 0.0f, 0.0f, 0.0f};
			RCfloat diffuse[] = {0.0f, 1.0f, 1.0f, 1.0f};
			RCfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
			RCfloat attnstart=1.0f;
			RCfloat attend=25.0f;	
			RCfloat cutoff = 0.15f;

			rcClearColor( 0.0f, 0.0f, 0.0f, 1.0f );			
			rcViewport(0, 0, Width(), Height());

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();

			rcuPerspective( 60.0f, (RCfloat)Width() / (RCfloat)Height(), 0.1f, 100.0f);
			rcuLookAt(7.347149, 3.256426, 3.826157, -0.420733, -0.100748, -1.354843, 9.699358, 92.254364, 76.426018);

			rcEnable(RC_LIGHTING);
			rcLightfv(RC_LIGHT0, RC_POSITION, position);
			rcLightfv(RC_LIGHT0, RC_AMBIENT, ambient);
			rcLightfv(RC_LIGHT0, RC_DIFFUSE, diffuse);
			rcLightfv(RC_LIGHT0, RC_SPECULAR, specular);
			rcLightfv(RC_LIGHT0, RC_SPOT_DIRECTION, spotdir);
			rcLightf(RC_LIGHT0, RC_SPOT_INNER_CONE, cos(10.f/PI/2.0f));
			rcLightf(RC_LIGHT0, RC_SPOT_OUTER_CONE, cos(60.f/PI/2.0f));
			rcLightf(RC_LIGHT0, RC_SPOT_CUTOFF, cutoff);
			rcLightf(RC_LIGHT0, RC_START_ATTENUATION, attnstart);
			rcLightf(RC_LIGHT0, RC_END_ATTENUATION, attend);
			rcLightf(RC_LIGHT0, RC_CONSTANT_ATTENUATION, 0.1f);
			rcLightf(RC_LIGHT0, RC_LINEAR_ATTENUATION, 0.05f);
			rcEnable(RC_LIGHT0);
#endif
			StaticSceneDraw();
		}

#ifdef FLAG_FILEOPEN
		FileLoad();
#endif

		SetMaterial();
		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;
		
		LightOnOff(i % 6);
		DynamicSceneDraw();

#ifdef FLAG_FILEOPEN
		g_fSpinX += 10.f;
		g_fSpinY += 10.f;
#else
		g_fSpinX += 15.f;
		g_fSpinY += 15.f;
#endif
		i++;

		return TRUE;
	}
};

Tutorial	g_Tutorial;