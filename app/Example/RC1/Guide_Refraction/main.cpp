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

// Programming Guide - Refraction

typedef struct RGBImageRec {
	int sizeX, sizeY;
	unsigned char *data;
} RGBImageRec;

typedef struct bmpBITMAPFILEHEADER{
	WORD   bfType;
	DWORD  bfSize;
	WORD   bfReserved1;
	WORD   bfReserved2;
	DWORD  bfOffBits;
}BITMAPFILEHEADER1;

typedef struct bmpBITMAPINFOHEADER{
	DWORD  biSize;
	DWORD  biWidth;
	DWORD  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD  biCompression;
	DWORD  biSizeImage;
	DWORD  biXPelsPerMeter;
	DWORD  biYPelsPerMeter;
	DWORD  biClrUsed;
	DWORD  biClrImportant;
}BITMAPFILEHEADER2;

RGBImageRec *DIBImageLoad(char* path, int channel) {
	RGBImageRec* pImage=NULL;
	FILE *f=NULL;
	unsigned char *pBuf=NULL;
	int dataSize=0;
	int index=0;
	DWORD x=0;
	DWORD y=0;
	int bpp=0;

	if(channel !=3 && channel !=4) return pImage;

	f = fopen(path, "rb");
	if(f != NULL) {
		BITMAPFILEHEADER1 HD1;
		BITMAPFILEHEADER2 HD2;

		fseek(f,0,SEEK_SET);
		fread(&HD1.bfType, sizeof(WORD), 1, f);
		fread(&HD1.bfSize,sizeof(DWORD), 1, f);
		fseek(f, 10, SEEK_SET);
		fread(&HD1.bfOffBits, sizeof(int),1,  f);

		fread(&HD2.biSize, sizeof(DWORD), 1, f);
		fread(&HD2.biWidth, sizeof(DWORD), 1, f);
		fread(&HD2.biHeight, sizeof(DWORD), 1, f);
		fread(&HD2.biPlanes, sizeof(WORD), 1, f);
		fread(&HD2.biBitCount, sizeof(WORD), 1, f);
		fread(&HD2.biCompression, sizeof(DWORD), 1, f);

		fseek(f,HD1.bfOffBits,SEEK_SET);

		bpp = HD2.biBitCount/8;
		if(bpp == 1 || bpp == channel)
		{
			pBuf = (unsigned char*) malloc(channel);

			pImage = (RGBImageRec*) malloc(sizeof(RGBImageRec));
			pImage->sizeX = HD2.biWidth;
			pImage->sizeY = HD2.biHeight;

			dataSize = HD2.biWidth*HD2.biHeight*channel;
			pImage->data = (unsigned char*) malloc(dataSize);

			for(y=0; y<HD2.biHeight; y++) {
				for(x=0; x<HD2.biWidth; x++) {
					fread(pBuf, bpp, 1, f);
					if(bpp == 1) {
						pBuf[1] = pBuf[2] = pBuf[0];
						if(channel == 4)	pBuf[3] = 0;
					}

					index = (y*HD2.biWidth + x)*channel;
					pImage->data[index]		= pBuf[2];
					pImage->data[index + 1] = pBuf[1];
					pImage->data[index + 2] = pBuf[0];
					if(channel == 4)
						pImage->data[index + 3] = pBuf[3];
				}
			}

			if(pBuf) free(pBuf);
			pBuf = NULL;
		}

		fclose(f);
	}

	return pImage;
}

float g_refractionIndex1 = 1.0f;
float g_refractionIndex2 = 1.0f;

RGBImageRec *g_texture;
RCuint		*g_textureName;

#define MATERIAL_TRIANGLE		0
#define MATERIAL_REFRACTION1	1
#define MATERIAL_REFRACTION2	2
unsigned int g_MaterialArray[3];

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Programming Guide - Refraction", 800, 480){}
	virtual ~Tutorial(void){}

	void GenTexture(void){
		g_texture = (RGBImageRec *)DIBImageLoad("./scenedata/Guide_Refraction/texture0.bmp", 3);	

		if(g_texture){
			g_textureName = new RCuint[1];

			rcGenTextures(1, g_textureName);
			rcBindTexture(RC_TEXTURE_2D, g_textureName[0]);
			rcTextureAlpha(128);
			rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, 
						g_texture[0].sizeX, g_texture[0].sizeY, 0, 
						RC_RGB, RC_UNSIGNED_BYTE, 
						g_texture[0].data);	

			if(g_texture->data)	free(g_texture->data);
			free(g_texture);
		}
	}

	void GenMaterial(void){
		float transmittance = 0.5;
		RCfloat color[3][4]={
			{0.8f, 0.3f, 0.4f, 0.0f},
			{0.2f, 0.4f, 0.8f, 0.0f},
			{0.4f, 0.8f, 0.2f, 0.0f},
		};

		rcGenMaterials(3, g_MaterialArray);

		rcBindMaterial(g_MaterialArray[MATERIAL_TRIANGLE]);	
		rcDisable(RC_TEXTURE_2D);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, &color[MATERIAL_TRIANGLE][0]);

		rcBindMaterial(g_MaterialArray[MATERIAL_REFRACTION1]);
		rcDisable(RC_TEXTURE_2D);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, &color[MATERIAL_REFRACTION1][0]);
		rcMaterialf (RC_FRONT_AND_BACK, RC_TRANSMITTANCE, transmittance);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, g_refractionIndex1);

		rcBindMaterial(g_MaterialArray[MATERIAL_REFRACTION2]);
		rcDisable(RC_TEXTURE_2D);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, &color[MATERIAL_REFRACTION2][0]);
		rcMaterialf (RC_FRONT_AND_BACK, RC_TRANSMITTANCE, transmittance);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, g_refractionIndex2);
	}

	void RenderTriangle(void){
		RCfloat TriangleVertices[3][3];

		TriangleVertices[0][0]=0.0f;		
		TriangleVertices[0][1]=0.707f;		
		TriangleVertices[0][2]=0.0f;

		TriangleVertices[1][0]=-1.0f;		
		TriangleVertices[1][1]=-0.707f;		
		TriangleVertices[1][2]=0.0f;

		TriangleVertices[2][0]=1.0f;		
		TriangleVertices[2][1]=-0.707f;		
		TriangleVertices[2][2]=0.0f;

		rcEnableClientState(RC_VERTEX_ARRAY);

		rcVertexPointer(3, RC_FLOAT, 0, TriangleVertices);

		rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

		float TexIndex[3][2];
		TexIndex[0][0] = 0.5f;	TexIndex[0][1] = 0.f;
		TexIndex[1][0] = 0.f;	TexIndex[1][1] = 1.f;
		TexIndex[2][0] = 1.f;	TexIndex[2][1] = 1.f;
		rcTexCoordPointer(2,RC_FLOAT,0,TexIndex);

		rcMatrixMode(RC_MODELVIEW);
		rcLoadIdentity();

		rcBindMaterial(g_MaterialArray[MATERIAL_TRIANGLE]);
		rcEnable(RC_TEXTURE_2D);
		rcBindTexture(RC_TEXTURE_2D, g_textureName[0]);

		rcPushMatrix();
		rcTranslatef(0,0,-10);
		rcScalef(3,3,3);
		rcDrawArrays(RC_TRIANGLES,0,3);
		rcPopMatrix();

		rcDisableClientState(RC_TEXTURE_COORD_ARRAY);
		rcBindMaterial(0);
		rcBindTexture(RC_TEXTURE_2D, 0);
		rcDisable(RC_TEXTURE_2D);

		rcDisableClientState(RC_VERTEX_ARRAY);
	}

	void RenderRefraction(void){
		RCfloat QuadVertices[4][3];

		QuadVertices[0][0]=2.0f;		
		QuadVertices[0][1]=-2.0f;		
		QuadVertices[0][2]=0.0f;

		QuadVertices[1][0]=2.0f;		
		QuadVertices[1][1]=2.0f;		
		QuadVertices[1][2]=0.0f;

		QuadVertices[2][0]=-2.0f;		
		QuadVertices[2][1]=2.0f;		
		QuadVertices[2][2]=0.0f;

		QuadVertices[3][0]=-2.0f;		
		QuadVertices[3][1]=-2.0f;		
		QuadVertices[3][2]=0.0f;

		rcEnableClientState(RC_VERTEX_ARRAY);

		rcVertexPointer(3, RC_FLOAT, 0, QuadVertices);

		rcMatrixMode(RC_MODELVIEW);
		rcLoadIdentity();

		rcPushMatrix();
		rcTranslatef(0,0,-5);

		rcBindMaterial(g_MaterialArray[MATERIAL_REFRACTION1]);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, g_refractionIndex1);	
		rcDrawArrays(RC_QUADS,0,4);

		rcPopMatrix();

		rcDisableClientState(RC_VERTEX_ARRAY);
	}

	void RefreshRefraction(){
		rcBindMaterial(g_MaterialArray[MATERIAL_REFRACTION1]);
		rcMaterialf (RC_FRONT_AND_BACK, RC_REFRACTION_INDEX, g_refractionIndex1);	
	}

	void StaticSceneDraw(void){
		rcStaticSceneBegin();
		RenderTriangle();
		RenderRefraction();	
		rcStaticSceneEnd();
	}

	void DynamicSceneDraw(void){
		RefreshRefraction();
	}

protected:
	virtual BOOL OnIntialize(void){
		GenTexture();
		GenMaterial();

		rcSceneAllInit();

		{
			rcClearColor(0.4f, 0.3f, 0.2f, 0.0f);			
			rcViewport(0, 0, Width(), Height());

			rcMatrixMode(RC_PROJECTION);
			rcLoadIdentity();

			rcuPerspective(70.0f, (RCfloat)Width() / (RCfloat)Height(), 1.0f, 500.0f);
			rcuLookAt(-3,0,0,0,0,-8,0,1,0);

			StaticSceneDraw();
		}
		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;
		i++;

		DynamicSceneDraw();

		g_refractionIndex1 += 0.1f;
		g_refractionIndex2 -= 0.1f;

		return TRUE;
	}
};

Tutorial	g_Tutorial;