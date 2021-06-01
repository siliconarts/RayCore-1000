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

// Programming Guide - Earth Example

#include "earth.h"

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

		fread(&HD2.biSize, sizeof(int), 1, f);
		fread(&HD2.biWidth, sizeof(int), 1, f);
		fread(&HD2.biHeight, sizeof(int), 1, f);
		fread(&HD2.biPlanes, sizeof(WORD), 1, f);
		fread(&HD2.biBitCount, sizeof(WORD), 1, f);
		fread(&HD2.biCompression, sizeof(int), 1, f);

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

#define Galaxy       0
#define Earth	     1

unsigned int TextureArray[2];
unsigned int MaterialIDs[2];

float Light_Position[] = {-17.802584f, 0.353599f, 8.770458f, 1};
float Light_Ambient[] = {0, 0, 0};
float Light_Diffuse[] = {1, 1, 1};
float Light_Specular[] = {1, 1, 1};

float   g_turn = 35;

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Programming Guide - Earth Example", 800, 480){}
	virtual ~Tutorial(void){}

	void Load(RGBImageRec **pBitmap){

		pBitmap[0] = (RGBImageRec *)DIBImageLoad("./scenedata/Guide_Earth/Galaxy.bmp", 3);	
		pBitmap[1] = (RGBImageRec *)DIBImageLoad("./scenedata/Guide_Earth/Earth.bmp", 3);			
	}
	
	void CreateTexture(void){
		RGBImageRec *pBitmap[2];
		int i=0;

		for(i=0; i<2; i++) {
			pBitmap[i] = NULL;
		}
		Load(pBitmap);

		rcGenTextures(2, TextureArray);

		rcBindTexture(RC_TEXTURE_2D, TextureArray[Galaxy]);
		rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[Galaxy]->sizeX, pBitmap[Galaxy]->sizeY, 
						0, RC_RGB, RC_UNSIGNED_BYTE, 
						pBitmap[Galaxy]->data);

		rcBindTexture(RC_TEXTURE_2D, TextureArray[Earth]);
		rcTexImage2D(RC_TEXTURE_2D, 0, RC_RGB, pBitmap[Earth]->sizeX, pBitmap[Earth]->sizeY, 
						0, RC_RGB, RC_UNSIGNED_BYTE, 
						pBitmap[Earth]->data);

		for(i=0; i<2; i++) {
			if(pBitmap[i]) {
				if(pBitmap[i]->data)	free(pBitmap[i]->data);
				free(pBitmap[i]);
			}
		}
	}

	void SetMaterial(void){
		float AmbientDiffuse[2][3] = {
			{0.588235f, 0.588235f, 0.588235f}, 
			{0.529412f, 0.529412f, 0.529412f}, 			
		};

		float Specular[2][3] = {
			{0, 0, 0}, 
			{1, 0, 0}, 
		};

		float SpecularLevel[2] = {
			0,
			1,
		};

		float Shininess[2] = {
			2.000000f, 
			3.031433f, 			
		};

		rcEnable(RC_TEXTURE_2D);
				
		rcGenMaterials(1, &MaterialIDs[Galaxy]);
		rcBindMaterial(MaterialIDs[Galaxy]);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[Galaxy]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, AmbientDiffuse[Galaxy]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, Specular[Galaxy]);
		rcMaterialf (RC_FRONT_AND_BACK, RC_SPECULAR_LEVEL, SpecularLevel[Galaxy]);
		rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS, Shininess[Galaxy]);

		rcGenMaterials(1, &MaterialIDs[Earth]);
		rcBindMaterial(MaterialIDs[Earth]);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[Earth]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT_AND_DIFFUSE, AmbientDiffuse[Earth]);
		rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, Specular[Earth]);
		rcMaterialf (RC_FRONT_AND_BACK, RC_SPECULAR_LEVEL, SpecularLevel[Earth]);
		rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS, Shininess[Earth]);
	}

	void StaticSceneDraw(void){
        float galaxyV[12] = {
            -1, -1,  0,
             1, -1,  0,
             1,  1,  0,
            -1,  1,  0
        };

        float galaxyT[8] = {
            0, 0,
            1, 0,
            1, 1,
            0, 1
        };

        rcBindMaterial(MaterialIDs[Galaxy]);
		rcEnable(RC_TEXTURE_2D);
		rcBindTexture(RC_TEXTURE_2D, TextureArray[Galaxy]);
		
        rcStaticSceneBegin();

        rcEnableClientState(RC_VERTEX_ARRAY);
        rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

        rcVertexPointer(3, RC_FLOAT, 0, galaxyV);
        rcTexCoordPointer(2, RC_FLOAT, 0, galaxyT);

		rcMatrixMode( RC_MODELVIEW );
	    rcLoadIdentity();
        rcTranslatef(0, 0, -18.f);
        rcScalef(8, 4.8f, 1);

		rcEnable(RC_USE_TEXTURE_ONLY);
        rcDrawArrays(RC_QUADS, 0, 4);
		rcDisable(RC_USE_TEXTURE_ONLY);

        rcDisableClientState(RC_VERTEX_ARRAY);
        rcDisableClientState(RC_TEXTURE_COORD_ARRAY);

		rcStaticSceneEnd();
	}

protected:
	virtual BOOL OnIntialize(void){

		rcDepthBounce(14);

		rcSceneAllInit();

		{
			rcClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
			rcViewport(0, 0, Width(), Height());

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();

			rcuPerspective( 30, (float)Width() /(float)Height(), 10, 10000 );

			rcuLookAt(0,  0,  0, 
                      0,  0, -1, 
                      0,  1,  0);
			
			CreateTexture();
			SetMaterial();

			{
				rcEnable(RC_LIGHTING);
				rcEnable(RC_LIGHT0);
				rcLightfv(RC_LIGHT0,RC_POSITION,Light_Position);
				rcLightfv(RC_LIGHT0,RC_AMBIENT,Light_Ambient);
				rcLightfv(RC_LIGHT0,RC_DIFFUSE,Light_Diffuse);					
				rcLightfv(RC_LIGHT0,RC_SPECULAR,Light_Specular);
			}

			StaticSceneDraw();
		}
		
		return TRUE;
	}
	virtual BOOL OnDraw(void){
		static int i = 0;
		i++;

		{
			rcClear(RC_COLOR_BUFFER_BIT | RC_DEPTH_BUFFER_BIT);

			rcMatrixMode( RC_MODELVIEW );
	        rcLoadIdentity();

			rcPushMatrix();
			rcRotatef(g_turn, 0, 1, 0);
			rcLightfv(RC_LIGHT0, RC_POSITION, Light_Position);
			rcPopMatrix();

			rcTranslatef(0, 0, -7.f);
			rcRotatef(35, 0, 1, 0);
			rcRotatef(15, 1, 0, 0);
			rcRotatef(-90, 0, 0, 1);

			rcEnableClientState(RC_VERTEX_ARRAY);
			rcEnableClientState(RC_NORMAL_ARRAY);
			rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

			rcVertexPointer(3, RC_FLOAT, sizeof(struct Pos), g_EarthVertices);
		    rcNormalPointer(RC_FLOAT, 0, g_EarthNormals);
		    rcTexCoordPointer(2, RC_FLOAT, 0, g_EarthTexCoords);

		    rcBindMaterial(MaterialIDs[Earth]);
			rcDrawArrays( RC_TRIANGLES, 0, sizeof(g_EarthVertices)/sizeof(Pos));

			rcDisableClientState(RC_VERTEX_ARRAY);
            rcDisableClientState(RC_NORMAL_ARRAY);
	        rcDisableClientState(RC_TEXTURE_COORD_ARRAY);

			g_turn += 5;

            if (g_turn == 360)
                g_turn = 0;
		}
		
		return TRUE;
	}
};

Tutorial	g_Tutorial;