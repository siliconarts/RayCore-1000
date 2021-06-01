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

// Programming Guide - Phong Shading

#include "sphere.h"

class Tutorial : public RCFramework
{
public:
	Tutorial(void) : RCFramework("Programming Guide - Phong Shading", 800, 480){}
	virtual ~Tutorial(void){}
	
	RCuint  m_staticMaterial;
	RCuint	m_bindName[4];

protected:
	virtual BOOL OnIntialize(void){
		
		rcSceneAllInit();

		{
			rcDepthBounce(14);

			rcClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
			rcViewport( 0, 0, Width(), Height() );

			rcMatrixMode( RC_PROJECTION );
			rcLoadIdentity();

			rcuPerspective( 30, (float)Width() /(float)Height(), 10, 10000 );

			rcuLookAt(0,  0,  8, 
                      0,  0,  0, 
                      0,  1,  0);

			
            /*
             * Light Setting
             * 
             */

			rcEnable(RC_LIGHTING);

			{
				rcEnable(RC_LIGHT0);

				{
					float pos[4] = {-5, 5, 8, 1 };
					rcLightfv(RC_LIGHT0, RC_POSITION, pos);
				}
				{
					float ambient[4] = { 0, 0, 0, 0 };
					rcLightfv(RC_LIGHT0, RC_AMBIENT, ambient);
				}
				{
					float diffuse[4] = { 0.2f, 0.3f, 0.4f, 0 };
					rcLightfv(RC_LIGHT0, RC_DIFFUSE, diffuse);
				}
				{
					float specular[4] = { 1, 1, 1, 0 };
					rcLightfv(RC_LIGHT0, RC_SPECULAR, specular);
				}
			}


			 /*
             * Material Setting
             * 
             */

			{
				rcGenMaterials(1, &m_bindName[0]);

				rcBindMaterial(m_bindName[0]);

				{
					float ambient[4] = {0, 0, 0, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT, ambient);
				}
				{
					float diffuse[4] = {0, 1, 1, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, diffuse);
				}
				{
					float specular[4] = {1, 0, 0, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specular);
					rcMaterialf (RC_FRONT_AND_BACK, RC_SPECULAR_LEVEL, 1.0);
				}
				{
					float exponent = 10;
					rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS, exponent);
				}
				rcDisable(RC_TEXTURE_2D);
			}

			{
				rcGenMaterials(1, &m_bindName[1]);

				rcBindMaterial(m_bindName[1]);

				{
					float ambient[4] = {0, 0, 0, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT, ambient);
				}
				{
					float diffuse[4] = {1, 0, 1, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, diffuse);
				}
				{
					float specular[4] = {0, 1, 0, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specular);
					rcMaterialf (RC_FRONT_AND_BACK, RC_SPECULAR_LEVEL, 1.0);
				}
				{
					float exponent = 20;
					rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS, exponent);
				}

				rcDisable(RC_TEXTURE_2D);
			}

			{
				rcGenMaterials(1, &m_bindName[2]);

				rcBindMaterial(m_bindName[2]);

				{
					float ambient[4] = {0, 0, 0, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT, ambient);
				}
				{
					float diffuse[4] = {1, 1, 0, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, diffuse);
				}
				{
					float specular[4] = {0, 0, 1, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specular);
					rcMaterialf (RC_FRONT_AND_BACK, RC_SPECULAR_LEVEL, 1.0);
				}
				{
					float exponent = 30;
					rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS, exponent);
				}

				rcDisable(RC_TEXTURE_2D);
			}

			{
				rcGenMaterials(1, &m_bindName[3]);

				rcBindMaterial(m_bindName[3]);
				{
					float ambient[4] = {0, 0, 0, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT, ambient);
				}
				{
					float diffuse[4] = {0, 1, 1, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, diffuse);
				}
				{
					float specular[4] = {0, 0, 0, 0};
					rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specular);
					rcMaterialf (RC_FRONT_AND_BACK, RC_SPECULAR_LEVEL, 0.0);
				}
				{
					float exponent = 20;
					rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS, exponent);
				}

				rcDisable(RC_TEXTURE_2D);
			}

			
            /*
             * Static Scene 
             * 
             */

			{

				float galaxyV[12] = {
                    -1, 0, -1,
                     1, 0, -1,
                     1, 0,  1,
                    -1, 0,  1
                };

                float galaxyT[8] = {
                    0, 0,
                    1, 0,
                    1, 1,
                    0, 1
                };

                float diffuse[3] = {0.6f, 0.8f, 0.5f};

                rcGenMaterials(1, &m_staticMaterial);
                rcBindMaterial(m_staticMaterial);
                        
                {
                    float ambient[4] = {0, 0, 0, 0};
                    rcMaterialfv(RC_FRONT_AND_BACK, RC_AMBIENT, ambient);
                }
                {
                    float diffuse[4] = {1, 1, 0, 0};
                    rcMaterialfv(RC_FRONT_AND_BACK, RC_DIFFUSE, diffuse);
                }
                {
                    float specular[4] = {0, 0, 0, 0};
                    rcMaterialfv(RC_FRONT_AND_BACK, RC_SPECULAR, specular);
                }
                {
                    float exponent = 10;
                    rcMaterialf (RC_FRONT_AND_BACK, RC_SHININESS, exponent);
                }

       			rcDisable(RC_TEXTURE_2D);
     
				rcStaticSceneBegin();

                rcEnableClientState(RC_VERTEX_ARRAY);
                rcEnableClientState(RC_TEXTURE_COORD_ARRAY);

                rcVertexPointer(3, RC_FLOAT, 0, galaxyV);
                rcTexCoordPointer(2, RC_FLOAT, 0, galaxyT);

		        rcMatrixMode( RC_MODELVIEW );
	            rcLoadIdentity();
                rcTranslatef(0.0, -1, -10);
                rcScalef(8, 1, 10);

                rcDrawArrays(RC_QUADS, 0, 4);

                rcDisableClientState(RC_VERTEX_ARRAY);
                rcDisableClientState(RC_TEXTURE_COORD_ARRAY);

		        rcStaticSceneEnd();
            }
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
			rcTranslatef(0.0, 0, -7.0);
			rcEnableClientState(RC_VERTEX_ARRAY);
			rcEnableClientState(RC_NORMAL_ARRAY);

			rcVertexPointer(3, RC_FLOAT, sizeof(struct Pos), g_SphereVertices);
			rcNormalPointer(RC_FLOAT, 0, g_SphereNormals);

			rcPushMatrix();
			rcBindMaterial(m_bindName[0]);
			rcTranslatef(2, 0, 0);
			rcDrawArrays( RC_TRIANGLES, 0, sizeof(g_SphereVertices)/sizeof(Pos) );
			rcPopMatrix();

			rcPushMatrix();
			rcBindMaterial(m_bindName[1]);
			rcTranslatef(-2, 0, 0);
			rcDrawArrays( RC_TRIANGLES, 0, sizeof(g_SphereVertices)/sizeof(Pos) );
			rcPopMatrix();

			rcBindMaterial(m_bindName[2]);
			rcDrawArrays( RC_TRIANGLES, 0, sizeof(g_SphereVertices)/sizeof(Pos) );

			rcDisableClientState(RC_VERTEX_ARRAY);
			rcDisableClientState(RC_NORMAL_ARRAY);
		}

		return TRUE;
	}
};

Tutorial	g_Tutorial;
