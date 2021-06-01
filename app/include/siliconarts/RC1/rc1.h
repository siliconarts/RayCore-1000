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
//=======================================================================

#ifndef __OPENRC_H__
#define __OPENRC_H__

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
	#define RC_API			__declspec(dllexport)
	#define RC_APIENTRY		__stdcall
#else //ANDROID_NDK
	#ifdef __cplusplus
		#define RC_API			extern "C"
	#else
		#define RC_API			extern
	#endif
	#define RC_APIENTRY
#endif // _LIBRARY


#ifdef __cplusplus
extern "C" {
#endif

/*
 * This document is licensed under the SGI Free Software B License Version
 * 2.0. For details, see http://oss.sgi.com/projects/FreeB/ .
 */

typedef int              RCfixed;
typedef int              RCclampx;

typedef signed long int  RCintptr;
typedef signed long int  RCsizeiptr;


typedef void                RCvoid;
typedef char                RCchar;
typedef unsigned int        RCenum;
typedef unsigned char       RCboolean;
typedef unsigned int        RCbitfield;
typedef unsigned char       RCbyte;
typedef short               RCshort;
typedef int                 RCint;
typedef int                 RCsizei;
typedef unsigned char       RCubyte;
typedef unsigned short      RCushort;
typedef unsigned int        RCuint;
typedef float               RCfloat;
typedef float               RCclampf;

typedef double              RCdouble;


/*************************************************************/

/* OpenRC ES core versions */
#define RC_VERSION_ES_CM_1_0          1
#define RC_VERSION_ES_CL_1_0          1
#define RC_VERSION_ES_CM_1_1          1
#define RC_VERSION_ES_CL_1_1          1

/* ClearBufferMask */
#define RC_DEPTH_BUFFER_BIT               0x00000100
#define RC_STENCIL_BUFFER_BIT             0x00000400
#define RC_COLOR_BUFFER_BIT               0x00004000

/* Boolean */
#define RC_FALSE                          0
#define RC_TRUE                           1

/* BeginMode */
#define RC_POINTS                         0x0000
#define RC_LINES                          0x0001
#define RC_LINE_LOOP                      0x0002
#define RC_LINE_STRIP                     0x0003
#define RC_TRIANGLES                      0x0004
#define RC_TRIANGLE_STRIP                 0x0005
#define RC_TRIANGLE_FAN                   0x0006
#define RC_QUADS                          0x0007

/* AlphaFunction */
#define RC_NEVER                          0x0200
#define RC_LESS                           0x0201
#define RC_EQUAL                          0x0202
#define RC_LEQUAL                         0x0203
#define RC_GREATER                        0x0204
#define RC_NOTEQUAL                       0x0205
#define RC_GEQUAL                         0x0206
#define RC_ALWAYS                         0x0207

/* BlendingFactorDest */
#define RC_ZERO                           0
#define RC_ONE                            1
#define RC_SRC_COLOR                      0x0300
#define RC_ONE_MINUS_SRC_COLOR            0x0301
#define RC_SRC_ALPHA                      0x0302
#define RC_ONE_MINUS_SRC_ALPHA            0x0303
#define RC_DST_ALPHA                      0x0304
#define RC_ONE_MINUS_DST_ALPHA            0x0305

/* BlendingFactorSrc */
/*      RC_ZERO */
/*      RC_ONE */
#define RC_DST_COLOR                      0x0306
#define RC_ONE_MINUS_DST_COLOR            0x0307
#define RC_SRC_ALPHA_SATURATE             0x0308
/*      RC_SRC_ALPHA */
/*      RC_ONE_MINUS_SRC_ALPHA */
/*      RC_DST_ALPHA */
/*      RC_ONE_MINUS_DST_ALPHA */

/* ClipPlaneName */
#define RC_CLIP_PLANE0                    0x3000
#define RC_CLIP_PLANE1                    0x3001
#define RC_CLIP_PLANE2                    0x3002
#define RC_CLIP_PLANE3                    0x3003
#define RC_CLIP_PLANE4                    0x3004
#define RC_CLIP_PLANE5                    0x3005

/* ColorMaterialFace */
/*      RC_FRONT_AND_BACK */

/* ColorMaterialParameter */
/*      RC_AMBIENT_AND_DIFFUSE */

/* ColorPointerType */
/*      RC_UNSIGNED_BYTE */
/*      RC_FLOAT */
/*      RC_FIXED */

/* CullFaceMode */
#define RC_FRONT                          0x0404
#define RC_BACK                           0x0405
#define RC_FRONT_AND_BACK                 0x0408

/* DepthFunction */
/*      RC_NEVER */
/*      RC_LESS */
/*      RC_EQUAL */
/*      RC_LEQUAL */
/*      RC_GREATER */
/*      RC_NOTEQUAL */
/*      RC_GEQUAL */
/*      RC_ALWAYS */

/* EnableCap */
#define RC_FOG                            0x0B60
#define RC_LIGHTING                       0x0B50
#define RC_TEXTURE_2D                     0x0DE1
#define RC_TEXTURE_2D_NORMAL              0x0DE2
#define RC_CULL_FACE                      0x0B44
#define RC_ALPHA_TEST                     0x0BC0
#define RC_BLEND                          0x0BE2
#define RC_COLOR_LOGIC_OP                 0x0BF2
#define RC_DITHER                         0x0BD0
#define RC_STENCIL_TEST                   0x0B90
#define RC_DEPTH_TEST                     0x0B71
/*      RC_LIGHT0 */
/*      RC_LIGHT1 */
/*      RC_LIGHT2 */
/*      RC_LIGHT3 */
/*      RC_LIGHT4 */
/*      RC_LIGHT5 */
/*      RC_LIGHT6 */
/*      RC_LIGHT7 */
#define RC_POINT_SMOOTH                   0x0B10
#define RC_LINE_SMOOTH                    0x0B20
#define RC_SCISSOR_TEST                   0x0C11
#define RC_COLOR_MATERIAL                 0x0B57
#define RC_NORMALIZE                      0x0BA1
#define RC_RESCALE_NORMAL                 0x803A
#define RC_POLYGON_OFFSET_FILL            0x8037
#define RC_VERTEX_ARRAY                   0x8074
#define RC_NORMAL_ARRAY                   0x8075
#define RC_COLOR_ARRAY                    0x8076
#define RC_TEXTURE_COORD_ARRAY            0x8078
#define RC_MULTISAMPLE                    0x809D
#define RC_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define RC_SAMPLE_ALPHA_TO_ONE            0x809F
#define RC_SAMPLE_COVERAGE                0x80A0
#define RC_USE_SHADOW					  0x80A1
#define RC_USE_COLOR_SHADOW               0x80A2
#define RC_USE_TEXTURE_ALPHA_SHADOW		  0x80A3
#define RC_USE_TEXTURE_ONLY				  0x80A4
#define RC_USE_TRANSMITTANCE_SHADOW		  0x80A5

/* ErrorCode */
#define RC_NO_ERROR                       0
#define RC_INVALID_ENUM                   0x0500
#define RC_INVALID_VALUE                  0x0501
#define RC_INVALID_OPERATION              0x0502
#define RC_STACK_OVERFLOW                 0x0503
#define RC_STACK_UNDERFLOW                0x0504
#define RC_OUT_OF_MEMORY                  0x0505

/* FogMode */
/*      RC_LINEAR */
#define RC_EXP                            0x0800
#define RC_EXP2                           0x0801

/* FogParameter */
#define RC_FOG_DENSITY                    0x0B62
#define RC_FOG_START                      0x0B63
#define RC_FOG_END                        0x0B64
#define RC_FOG_MODE                       0x0B65
#define RC_FOG_COLOR                      0x0B66

/* FrontFaceDirection */
#define RC_CW                             0x0900
#define RC_CCW                            0x0901

/* GetPName */
#define RC_CURRENT_AMBIENT                0x1B00
#define RC_CURRENT_DIFFUSE                0x1B01
#define RC_CURRENT_SPECULAR               0x1B02

#define RC_CURRENT_COLOR                  0x0B00
#define RC_CURRENT_NORMAL                 0x0B02
#define RC_CURRENT_TEXTURE_COORDS         0x0B03
#define RC_POINT_SIZE                     0x0B11
#define RC_POINT_SIZE_MIN                 0x8126
#define RC_POINT_SIZE_MAX                 0x8127
#define RC_POINT_FADE_THRESHOLD_SIZE      0x8128
#define RC_POINT_DISTANCE_ATTENUATION     0x8129
#define RC_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define RC_LINE_WIDTH                     0x0B21
#define RC_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define RC_ALIASED_POINT_SIZE_RANGE       0x846D
#define RC_ALIASED_LINE_WIDTH_RANGE       0x846E
#define RC_CULL_FACE_MODE                 0x0B45
#define RC_FRONT_FACE                     0x0B46
#define RC_SHADE_MODEL                    0x0B54
#define RC_DEPTH_RANGE                    0x0B70
#define RC_DEPTH_WRITEMASK                0x0B72
#define RC_DEPTH_CLEAR_VALUE              0x0B73
#define RC_DEPTH_FUNC                     0x0B74
#define RC_STENCIL_CLEAR_VALUE            0x0B91
#define RC_STENCIL_FUNC                   0x0B92
#define RC_STENCIL_VALUE_MASK             0x0B93
#define RC_STENCIL_FAIL                   0x0B94
#define RC_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define RC_STENCIL_PASS_DEPTH_PASS        0x0B96
#define RC_STENCIL_REF                    0x0B97
#define RC_STENCIL_WRITEMASK              0x0B98
#define RC_MATRIX_MODE                    0x0BA0
#define RC_VIEWPORT                       0x0BA2
#define RC_MODELVIEW_STACK_DEPTH          0x0BA3
#define RC_PROJECTION_STACK_DEPTH         0x0BA4
#define RC_TEXTURE_STACK_DEPTH            0x0BA5
#define RC_MODELVIEW_MATRIX               0x0BA6
#define RC_PROJECTION_MATRIX              0x0BA7
#define RC_TEXTURE_MATRIX                 0x0BA8
#define RC_ALPHA_TEST_FUNC                0x0BC1
#define RC_ALPHA_TEST_REF                 0x0BC2
#define RC_BLEND_DST                      0x0BE0
#define RC_BLEND_SRC                      0x0BE1
#define RC_LOGIC_OP_MODE                  0x0BF0
#define RC_SCISSOR_BOX                    0x0C10
#define RC_SCISSOR_TEST                   0x0C11
#define RC_COLOR_CLEAR_VALUE              0x0C22
#define RC_COLOR_WRITEMASK                0x0C23
#define RC_UNPACK_ALIGNMENT               0x0CF5
#define RC_PACK_ALIGNMENT                 0x0D05
#define RC_MAX_LIGHTS                     0x0D31
#define RC_MAX_CLIP_PLANES                0x0D32
#define RC_MAX_TEXTURE_SIZE               0x0D33
#define RC_MAX_MODELVIEW_STACK_DEPTH      0x0D36
#define RC_MAX_PROJECTION_STACK_DEPTH     0x0D38
#define RC_MAX_TEXTURE_STACK_DEPTH        0x0D39
#define RC_MAX_VIEWPORT_DIMS              0x0D3A
#define RC_MAX_THRESHOLD_LEVELS           0x0D3B
#define RC_MAX_TEXTURE_UNITS              0x84E2
#define RC_SUBPIXEL_BITS                  0x0D50
#define RC_RED_BITS                       0x0D52
#define RC_GREEN_BITS                     0x0D53
#define RC_BLUE_BITS                      0x0D54
#define RC_ALPHA_BITS                     0x0D55
#define RC_DEPTH_BITS                     0x0D56
#define RC_STENCIL_BITS                   0x0D57
#define RC_POLYGON_OFFSET_UNITS           0x2A00
#define RC_POLYGON_OFFSET_FILL            0x8037
#define RC_POLYGON_OFFSET_FACTOR          0x8038
#define RC_MATERIAL_BINDING               0x8068
#define RC_TEXTURE_BINDING_2D             0x8069
#define RC_VERTEX_ARRAY_SIZE              0x807A
#define RC_VERTEX_ARRAY_TYPE              0x807B
#define RC_VERTEX_ARRAY_STRIDE            0x807C
#define RC_NORMAL_ARRAY_TYPE              0x807E
#define RC_NORMAL_ARRAY_STRIDE            0x807F
#define RC_COLOR_ARRAY_SIZE               0x8081
#define RC_COLOR_ARRAY_TYPE               0x8082
#define RC_COLOR_ARRAY_STRIDE             0x8083
#define RC_TEXTURE_COORD_ARRAY_SIZE       0x8088
#define RC_TEXTURE_COORD_ARRAY_TYPE       0x8089
#define RC_TEXTURE_COORD_ARRAY_STRIDE     0x808A
#define RC_VERTEX_ARRAY_POINTER           0x808E
#define RC_NORMAL_ARRAY_POINTER           0x808F
#define RC_COLOR_ARRAY_POINTER            0x8090
#define RC_TEXTURE_COORD_ARRAY_POINTER    0x8092
#define RC_SAMPLE_BUFFERS                 0x80A8
#define RC_SAMPLES                        0x80A9
#define RC_SAMPLE_COVERAGE_VALUE          0x80AA
#define RC_SAMPLE_COVERAGE_INVERT         0x80AB

/* GetTextureParameter */
/*      RC_TEXTURE_MAG_FILTER */
/*      RC_TEXTURE_MIN_FILTER */
/*      RC_TEXTURE_WRAP_S */
/*      RC_TEXTURE_WRAP_T */

#define RC_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define RC_COMPRESSED_TEXTURE_FORMATS     0x86A3

/* HintMode */
#define RC_DONT_CARE                      0x1100
#define RC_FASTEST                        0x1101
#define RC_NICEST                         0x1102
#define RC_FASTEST_AND_NICEST             0x1103
#define RC_THRESHOLD_LEVEL0               0x1118
#define RC_THRESHOLD_LEVEL1               0x1119
#define RC_THRESHOLD_LEVEL2               0x111A
#define RC_THRESHOLD_LEVEL3               0x111B
#define RC_THRESHOLD_LEVEL4               0x111C
#define RC_THRESHOLD_LEVEL5               0x111D
#define RC_THRESHOLD_LEVEL6               0x111E
#define RC_THRESHOLD_LEVEL7               0x111F
#define RC_THRESHOLD_LEVEL8               0x1120
#define RC_THRESHOLD_LEVEL9               0x1121

/* HintTarget */
#define RC_PERSPECTIVE_CORRECTION_HINT    0x0C50
#define RC_POINT_SMOOTH_HINT              0x0C51
#define RC_LINE_SMOOTH_HINT               0x0C52
#define RC_FOG_HINT                       0x0C54
#define RC_MIPMAP_HINT                    0x0C55
#define RC_RENDERING_HINT                 0x0C56
#define RC_RAYBOUNCE_THRESHOLD_HINT       0x0C58
#define RC_GENERATE_MIPMAP_HINT           0x8192

/* LightModelParameter */
#define RC_LIGHT_MODEL_AMBIENT            0x0B53
#define RC_LIGHT_MODEL_TWO_SIDE           0x0B52

/* LightParameter */
#define RC_AMBIENT                        0x1200
#define RC_DIFFUSE                        0x1201
#define RC_SPECULAR                       0x1202
#define RC_POSITION                       0x1203
#define RC_SPOT_DIRECTION                 0x1204
#define RC_SPOT_EXPONENT                  0x1205
#define RC_SPOT_INNER_CONE                0x1206
#define RC_SPOT_OUTER_CONE                0x1207
#define RC_SPOT_CUTOFF                    0x1208
#define RC_ATTENUATION_RANGE              0x1209
#define RC_START_ATTENUATION              0x1210
#define RC_END_ATTENUATION                0x1211
#define RC_CONSTANT_ATTENUATION           0x1212
#define RC_LINEAR_ATTENUATION             0x1213
#define RC_QUADRATIC_ATTENUATION          0x1214

/* DataType */
#define RC_BYTE                           0x1400
#define RC_UNSIGNED_BYTE                  0x1401
#define RC_SHORT                          0x1402
#define RC_UNSIGNED_SHORT                 0x1403
#define RC_FLOAT                          0x1406
#define RC_FIXED                          0x140C

/* LogicOp */
#define RC_CLEAR                          0x1500
#define RC_AND                            0x1501
#define RC_AND_REVERSE                    0x1502
#define RC_COPY                           0x1503
#define RC_AND_INVERTED                   0x1504
#define RC_NOOP                           0x1505
#define RC_XOR                            0x1506
#define RC_OR                             0x1507
#define RC_NOR                            0x1508
#define RC_EQUIV                          0x1509
#define RC_INVERT                         0x150A
#define RC_OR_REVERSE                     0x150B
#define RC_COPY_INVERTED                  0x150C
#define RC_OR_INVERTED                    0x150D
#define RC_NAND                           0x150E
#define RC_SET                            0x150F

/* MaterialFace */
/*      RC_FRONT_AND_BACK */

/* MaterialParameter */
#define RC_EMISSION                       0x1600
#define RC_SHININESS                      0x1601
#define RC_AMBIENT_AND_DIFFUSE            0x1602
#define RC_REFLECTION                     0x1603
#define RC_TRANSMITTANCE                  0x1604
#define RC_REFRACTION_INDEX               0x1605
#define RC_TEXTURE_ALPHA                  0x1606
#define RC_SPECULAR_LEVEL                 0x1607

/*      RC_AMBIENT */
/*      RC_DIFFUSE */
/*      RC_SPECULAR */

/* MatrixMode */
#define RC_MODELVIEW                      0x1700
#define RC_PROJECTION                     0x1701
#define RC_TEXTURE                        0x1702

/* NormalPointerType */
/*      RC_BYTE */
/*      RC_SHORT */
/*      RC_FLOAT */
/*      RC_FIXED */

/* PixelFormat */
#define RC_ALPHA                          0x1906
#define RC_RGB                            0x1907
#define RC_RGBA                           0x1908
#define RC_LUMINANCE                      0x1909
#define RC_LUMINANCE_ALPHA                0x190A

/* PixelStoreParameter */
#define RC_UNPACK_ALIGNMENT               0x0CF5
#define RC_PACK_ALIGNMENT                 0x0D05

/* PixelType */
/*      RC_UNSIGNED_BYTE */
#define RC_UNSIGNED_SHORT_4_4_4_4         0x8033
#define RC_UNSIGNED_SHORT_5_5_5_1         0x8034
#define RC_UNSIGNED_SHORT_5_6_5           0x8363

/* ShadingModel */
#define RC_FLAT                           0x1D00
#define RC_SMOOTH                         0x1D01

/* StencilFunction */
/*      RC_NEVER */
/*      RC_LESS */
/*      RC_EQUAL */
/*      RC_LEQUAL */
/*      RC_GREATER */
/*      RC_NOTEQUAL */
/*      RC_GEQUAL */
/*      RC_ALWAYS */

/* StencilOp */
/*      RC_ZERO */
#define RC_KEEP                           0x1E00
#define RC_REPLACE                        0x1E01
#define RC_INCR                           0x1E02
#define RC_DECR                           0x1E03
/*      RC_INVERT */

/* StringName */
#define RC_VENDOR                         0x1F00
#define RC_RENDERER                       0x1F01
#define RC_VERSION                        0x1F02
#define RC_EXTENSIONS                     0x1F03

/* TexCoordPointerType */
/*      RC_SHORT */
/*      RC_FLOAT */
/*      RC_FIXED */
/*      RC_BYTE */

/* TextureEnvMode */
#define RC_MODULATE                       0x2100
#define RC_DECAL                          0x2101
/*      RC_BLEND */
#define RC_ADD                            0x0104
/*      RC_REPLACE */

/* TextureEnvParameter */
#define RC_TEXTURE_ENV_MODE               0x2200
#define RC_TEXTURE_ENV_COLOR              0x2201

/* TextureEnvTarget */
#define RC_TEXTURE_ENV                    0x2300

/* TextureMagFilter */
#define RC_NEAREST                        0x2600
#define RC_LINEAR                         0x2601

/* TextureMinFilter */
/*      RC_NEAREST */
/*      RC_LINEAR */
#define RC_NEAREST_MIPMAP_NEAREST         0x2700
#define RC_LINEAR_MIPMAP_NEAREST          0x2701
#define RC_NEAREST_MIPMAP_LINEAR          0x2702
#define RC_LINEAR_MIPMAP_LINEAR           0x2703

/* TextureParameterName */
#define RC_TEXTURE_MAG_FILTER             0x2800
#define RC_TEXTURE_MIN_FILTER             0x2801
#define RC_TEXTURE_WRAP_S                 0x2802
#define RC_TEXTURE_WRAP_T                 0x2803
#define RC_GENERATE_MIPMAP                0x8191

/* TextureTarget */
/*      RC_TEXTURE_2D */

/* TextureUnit */
#define RC_TEXTURE0                       0x84C0
#define RC_TEXTURE1                       0x84C1
#define RC_TEXTURE2                       0x84C2
#define RC_TEXTURE3                       0x84C3
#define RC_TEXTURE4                       0x84C4
#define RC_TEXTURE5                       0x84C5
#define RC_TEXTURE6                       0x84C6
#define RC_TEXTURE7                       0x84C7
#define RC_TEXTURE8                       0x84C8
#define RC_TEXTURE9                       0x84C9
#define RC_TEXTURE10                      0x84CA
#define RC_TEXTURE11                      0x84CB
#define RC_TEXTURE12                      0x84CC
#define RC_TEXTURE13                      0x84CD
#define RC_TEXTURE14                      0x84CE
#define RC_TEXTURE15                      0x84CF
#define RC_TEXTURE16                      0x84D0
#define RC_TEXTURE17                      0x84D1
#define RC_TEXTURE18                      0x84D2
#define RC_TEXTURE19                      0x84D3
#define RC_TEXTURE20                      0x84D4
#define RC_TEXTURE21                      0x84D5
#define RC_TEXTURE22                      0x84D6
#define RC_TEXTURE23                      0x84D7
#define RC_TEXTURE24                      0x84D8
#define RC_TEXTURE25                      0x84D9
#define RC_TEXTURE26                      0x84DA
#define RC_TEXTURE27                      0x84DB
#define RC_TEXTURE28                      0x84DC
#define RC_TEXTURE29                      0x84DD
#define RC_TEXTURE30                      0x84DE
#define RC_TEXTURE31                      0x84DF
#define RC_ACTIVE_TEXTURE                 0x84E0
#define RC_CLIENT_ACTIVE_TEXTURE          0x84E1

/* TextureWrapMode */
#define RC_REPEAT                         0x2901
#define RC_MIRRORED_REPEAT                0x8370
#define RC_CLAMP_TO_EDGE                  0x812F

/* VertexPointerType */
/*      RC_SHORT */
/*      RC_FLOAT */
/*      RC_FIXED */
/*      RC_BYTE */

/* LightName */
#define RC_LIGHT0                         0x4000
#define RC_LIGHT1                         0x4001
#define RC_LIGHT2                         0x4002
#define RC_LIGHT3                         0x4003
#define RC_LIGHT4                         0x4004
#define RC_LIGHT5                         0x4005
#define RC_LIGHT6                         0x4006
#define RC_LIGHT7                         0x4007

/* Buffer Objects */
#define RC_ARRAY_BUFFER                   0x8892
#define RC_ELEMENT_ARRAY_BUFFER           0x8893

#define RC_ARRAY_BUFFER_BINDING               0x8894
#define RC_ELEMENT_ARRAY_BUFFER_BINDING       0x8895
#define RC_VERTEX_ARRAY_BUFFER_BINDING        0x8896
#define RC_NORMAL_ARRAY_BUFFER_BINDING        0x8897
#define RC_COLOR_ARRAY_BUFFER_BINDING         0x8898
#define RC_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A

#define RC_STATIC_DRAW                    0x88E4
#define RC_DYNAMIC_DRAW                   0x88E8

#define RC_BUFFER_SIZE                    0x8764
#define RC_BUFFER_USAGE                   0x8765

/* Texture combine + dot3 */
#define RC_SUBTRACT                       0x84E7
#define RC_COMBINE                        0x8570
#define RC_COMBINE_RGB                    0x8571
#define RC_COMBINE_ALPHA                  0x8572
#define RC_RGB_SCALE                      0x8573
#define RC_ADD_SIGNED                     0x8574
#define RC_INTERPOLATE                    0x8575
#define RC_CONSTANT                       0x8576
#define RC_PRIMARY_COLOR                  0x8577
#define RC_PREVIOUS                       0x8578
#define RC_OPERAND0_RGB                   0x8590
#define RC_OPERAND1_RGB                   0x8591
#define RC_OPERAND2_RGB                   0x8592
#define RC_OPERAND0_ALPHA                 0x8598
#define RC_OPERAND1_ALPHA                 0x8599
#define RC_OPERAND2_ALPHA                 0x859A

#define RC_ALPHA_SCALE                    0x0D1C

#define RC_SRC0_RGB                       0x8580
#define RC_SRC1_RGB                       0x8581
#define RC_SRC2_RGB                       0x8582
#define RC_SRC0_ALPHA                     0x8588
#define RC_SRC1_ALPHA                     0x8589
#define RC_SRC2_ALPHA                     0x858A

#define RC_DOT3_RGB                       0x86AE
#define RC_DOT3_RGBA                      0x86AF


#define RC_BYTE                           0x1400
#define RC_UNSIGNED_BYTE                  0x1401
#define RC_SHORT                          0x1402
#define RC_UNSIGNED_SHORT                 0x1403
#define RC_INT                            0x1404
#define RC_UNSIGNED_INT                   0x1405
#define RC_FLOAT                          0x1406
#define RC_2_BYTES                        0x1407
#define RC_3_BYTES                        0x1408
#define RC_4_BYTES                        0x1409
#define RC_DOUBLE                         0x140A

/*------------------------------------------------------------------------*
 * required OES extension tokens
 *------------------------------------------------------------------------*/

/* RC_OES_matrix_palette */
#ifndef RC_OES_matrix_palette
#define RC_MATRIX_PALETTE_OES									0x8840
#define RC_MAX_PALETTE_MATRICES_OES                             0x8842
#define RC_MAX_VERTEX_UNITS_OES                                 0x86A4
#define RC_MATRIX_INDEX_ARRAY_OES                               0x8844
#define RC_WEIGHT_ARRAY_OES                                     0x86AD
#define RC_CURRENT_PALETTE_MATRIX_OES                           0x8843
#define RC_MATRIX_INDEX_ARRAY_SIZE_OES                          0x8846
#define RC_MATRIX_INDEX_ARRAY_TYPE_OES                          0x8847
#define RC_MATRIX_INDEX_ARRAY_STRIDE_OES                        0x8848
#define RC_MATRIX_INDEX_ARRAY_POINTER_OES                       0x8849
#define RC_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES                0x8B9E
#define RC_WEIGHT_ARRAY_SIZE_OES                                0x86AB
#define RC_WEIGHT_ARRAY_TYPE_OES                                0x86A9
#define RC_WEIGHT_ARRAY_STRIDE_OES                              0x86AA
#define RC_WEIGHT_ARRAY_POINTER_OES                             0x86AC
#define RC_WEIGHT_ARRAY_BUFFER_BINDING_OES                      0x889E
#endif

/* OES_read_format */
#ifndef RC_OES_read_format
#define RC_IMPLEMENTATION_COLOR_READ_TYPE_OES                   0x8B9A
#define RC_IMPLEMENTATION_COLOR_READ_FORMAT_OES                 0x8B9B
#endif

/* RC_OES_compressed_paletted_texture */
#ifndef RC_OES_compressed_paletted_texture
#define RC_PALETTE4_RGB8_OES                                    0x8B90
#define RC_PALETTE4_RGBA8_OES                                   0x8B91
#define RC_PALETTE4_R5_G6_B5_OES                                0x8B92
#define RC_PALETTE4_RGBA4_OES                                   0x8B93
#define RC_PALETTE4_RGB5_A1_OES                                 0x8B94
#define RC_PALETTE8_RGB8_OES                                    0x8B95
#define RC_PALETTE8_RGBA8_OES                                   0x8B96
#define RC_PALETTE8_R5_G6_B5_OES                                0x8B97
#define RC_PALETTE8_RGBA4_OES                                   0x8B98
#define RC_PALETTE8_RGB5_A1_OES                                 0x8B99
#endif

/* OES_point_size_array */
#ifndef RC_OES_point_size_array
#define RC_POINT_SIZE_ARRAY_OES                                 0x8B9C
#define RC_POINT_SIZE_ARRAY_TYPE_OES                            0x898A
#define RC_POINT_SIZE_ARRAY_STRIDE_OES                          0x898B
#define RC_POINT_SIZE_ARRAY_POINTER_OES                         0x898C
#define RC_POINT_SIZE_ARRAY_BUFFER_BINDING_OES                  0x8B9F
#endif

/* RC_OES_point_sprite */
#ifndef RC_OES_point_sprite
#define RC_POINT_SPRITE_OES                                     0x8861
#define RC_COORD_REPLACE_OES                                    0x8862
#endif

/*************************************************************/

/* Unavailable only in Common profile */
RC_API void RC_APIENTRY rcAlphaFunc (RCenum func, RCclampf ref);
RC_API void RC_APIENTRY rcBlendFunc (RCenum sfactor, RCenum dfactor);
RC_API void RC_APIENTRY rcClipPlanef (RCenum plane, const RCfloat *equation);
RC_API void RC_APIENTRY rcColorMask (RCboolean red, RCboolean green, RCboolean blue, RCboolean alpha);
RC_API void RC_APIENTRY rcCullFace (RCenum mode);
RC_API void RC_APIENTRY rcDepthRangef (RCclampf zNear, RCclampf zFar);
RC_API void RC_APIENTRY rcDepthMask (RCboolean flag);
RC_API void RC_APIENTRY rcDepthFunc (RCenum func);
RC_API void RC_APIENTRY rcClearDepthf (RCclampf depth);
RC_API void RC_APIENTRY rcFrontFace (RCenum mode);
RC_API void RC_APIENTRY rcScissor (RCint x, RCint y, RCsizei width, RCsizei height);
RC_API void RC_APIENTRY rcPixelStorei (RCenum pname, RCint param);
RC_API void RC_APIENTRY rcLineWidth (RCfloat width);
RC_API void RC_APIENTRY rcLogicOp (RCenum opcode);
RC_API void RC_APIENTRY rcPointSize (RCfloat size);
RC_API void RC_APIENTRY rcTexEnvi (RCenum target, RCenum pname, RCint param);
RC_API void RC_APIENTRY rcTexEnviv (RCenum target, RCenum pname, const RCint *params);
RC_API void RC_APIENTRY rcPolygonOffset (RCfloat factor, RCfloat units);
RC_API void RC_APIENTRY rcStencilMask (RCuint mask);
RC_API void RC_APIENTRY rcStencilOp (RCenum fail, RCenum zfail, RCenum zpass);
RC_API void RC_APIENTRY rcStencilFunc (RCenum func, RCint ref, RCuint mask);
RC_API void RC_APIENTRY rcTexEnvfv (RCenum target, RCenum pname, const RCfloat *params);
RC_API void RC_APIENTRY rcTexEnvf (RCenum target, RCenum pname, RCfloat param);
RC_API void RC_APIENTRY rcCopyTexImage2D (RCenum target, RCint level, RCenum internalformat, RCint x, RCint y, RCsizei width, RCsizei height, RCint border);
RC_API void RC_APIENTRY rcCopyTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCint x, RCint y, RCsizei width, RCsizei height);
RC_API void RC_APIENTRY rcClearStencil (RCint s);
RC_API void RC_APIENTRY rcOrthof (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar);


/* Available only in Common profile */
RC_API void RC_APIENTRY rcClearColor (RCclampf red, RCclampf green, RCclampf blue, RCclampf alpha);
RC_API void RC_APIENTRY rcColor4f (RCfloat red, RCfloat green, RCfloat blue, RCfloat alpha);
RC_API void RC_APIENTRY rcFrustum (RCdouble left, RCdouble right, RCdouble bottom, RCdouble top, RCdouble zNear, RCdouble zFar);
RC_API void RC_APIENTRY rcFrustumf (RCfloat left, RCfloat right, RCfloat bottom, RCfloat top, RCfloat zNear, RCfloat zFar);
RC_API void RC_APIENTRY rcGetFloatv (RCenum pname, RCfloat *params);
RC_API void RC_APIENTRY rcGetLightfv (RCenum light, RCenum pname, RCfloat *params);
RC_API void RC_APIENTRY rcGetMaterialfv (RCenum face, RCenum pname, RCfloat *params);
RC_API void RC_APIENTRY rcGetTexParameterfv (RCenum target, RCenum pname, RCfloat *params);
RC_API void RC_APIENTRY rcLightf (RCenum light, RCenum pname, RCfloat param);
RC_API void RC_APIENTRY rcLightfv (RCenum light, RCenum pname, const RCfloat *params);
RC_API void RC_APIENTRY rcLoadMatrixf (const RCfloat *m);
RC_API void RC_APIENTRY rcMaterialf (RCenum face, RCenum pname, RCfloat param);
RC_API void RC_APIENTRY rcMaterialfv (RCenum face, RCenum pname, const RCfloat *params);
RC_API void RC_APIENTRY rcMultMatrixf (const RCfloat *m);
RC_API void RC_APIENTRY rcRotatef (RCfloat angle, RCfloat x, RCfloat y, RCfloat z);
RC_API void RC_APIENTRY rcScalef (RCfloat x, RCfloat y, RCfloat z);
RC_API void RC_APIENTRY rcTexParameterf (RCenum target, RCenum pname, RCfloat param);
RC_API void RC_APIENTRY rcTexParameterfv (RCenum target, RCenum pname, const RCfloat *params);
RC_API void RC_APIENTRY rcTranslatef (RCfloat x, RCfloat y, RCfloat z);

/* Available in both Common and Common-Lite profiles */
RC_API void RC_APIENTRY rcBindBuffer (RCenum target, RCuint buffer);
RC_API void RC_APIENTRY rcBindMaterial (RCuint material);
RC_API void RC_APIENTRY rcBindTexture (RCenum target, RCuint texture);
RC_API void RC_APIENTRY rcBufferData (RCenum target, RCsizeiptr size, const RCvoid *data, RCenum usage);
RC_API void RC_APIENTRY rcBufferSubData (RCenum target, RCintptr offset, RCsizeiptr size, const RCvoid *data);
RC_API void RC_APIENTRY rcClear (RCbitfield mask);
RC_API void RC_APIENTRY rcClearColorx (RCclampx red, RCclampx green, RCclampx blue, RCclampx alpha);
RC_API void RC_APIENTRY rcColor4ub (RCubyte red, RCubyte green, RCubyte blue, RCubyte alpha);
RC_API void RC_APIENTRY rcColor4x (RCfixed red, RCfixed green, RCfixed blue, RCfixed alpha);
RC_API void RC_APIENTRY rcColorPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcDeleteBuffers (RCsizei n, const RCuint *buffers);
RC_API void RC_APIENTRY rcDeleteMaterials (RCsizei n, RCuint *materials);
RC_API void RC_APIENTRY rcDeleteTextures (RCsizei n, const RCuint *textures);
RC_API void RC_APIENTRY rcDisable (RCenum cap);
RC_API void RC_APIENTRY rcDisableClientState (RCenum array);
RC_API void RC_APIENTRY rcDrawArrays (RCenum mode, RCint first, RCsizei count);
RC_API void RC_APIENTRY rcDrawElements (RCenum mode, RCsizei count, RCenum type, const RCvoid *indices);
RC_API void RC_APIENTRY rcEnable (RCenum cap);
RC_API void RC_APIENTRY rcEnableClientState (RCenum array);
RC_API void RC_APIENTRY rcFinish (void);
RC_API void RC_APIENTRY rcFlush (void);
RC_API void RC_APIENTRY rcFrustumx (RCfixed left, RCfixed right, RCfixed bottom, RCfixed top, RCfixed zNear, RCfixed zFar);
RC_API void RC_APIENTRY rcGetBooleanv (RCenum pname, RCboolean *params);
RC_API void RC_APIENTRY rcGetBufferParameteriv (RCenum target, RCenum pname, RCint *params);
RC_API void RC_APIENTRY rcGenBuffers (RCsizei n, RCuint *buffers);
RC_API void RC_APIENTRY rcGenMaterials (RCsizei n, RCuint *materials);
RC_API void RC_APIENTRY rcGenTextures (RCsizei n, RCuint *textures);
RC_API RCenum RC_APIENTRY rcGetError (void);
RC_API void RC_APIENTRY rcGetFixedv (RCenum pname, RCfixed *params);
RC_API void RC_APIENTRY rcGetIntegerv (RCenum pname, RCint *params);
RC_API void RC_APIENTRY rcGetLightxv (RCenum light, RCenum pname, RCfixed *params);
RC_API void RC_APIENTRY rcGetMaterialxv (RCenum face, RCenum pname, RCfixed *params);
RC_API void RC_APIENTRY rcGetPointerv (RCenum pname, RCvoid **params);
RC_API const RCubyte * RC_APIENTRY rcGetString (RCenum name);
RC_API void RC_APIENTRY rcGetTexParameteriv (RCenum target, RCenum pname, RCint *params);
RC_API void RC_APIENTRY rcGetTexParameterxv (RCenum target, RCenum pname, RCfixed *params);
RC_API void RC_APIENTRY rcHint (RCenum target, RCenum mode);
RC_API RCboolean RC_APIENTRY rcIsBuffer (RCuint buffer);
RC_API RCboolean RC_APIENTRY rcIsEnabled (RCenum cap);
RC_API RCboolean RC_APIENTRY rcIsMaterial (RCuint material);
RC_API RCboolean RC_APIENTRY rcIsTexture (RCuint texture);
RC_API void RC_APIENTRY rcLightx (RCenum light, RCenum pname, RCfixed param);
RC_API void RC_APIENTRY rcLightxv (RCenum light, RCenum pname, const RCfixed *params);
RC_API void RC_APIENTRY rcLoadIdentity (void);
RC_API void RC_APIENTRY rcLoadMatrixx (const RCfixed *m);
RC_API void RC_APIENTRY rcMaterialx (RCenum face, RCenum pname, RCfixed param);
RC_API void RC_APIENTRY rcMaterialxv (RCenum face, RCenum pname, const RCfixed *params);
RC_API void RC_APIENTRY rcMatrixMode (RCenum mode);
RC_API void RC_APIENTRY rcMultMatrixx (const RCfixed *m);
RC_API void RC_APIENTRY rcNormalPointer (RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcPopMatrix (void);
RC_API void RC_APIENTRY rcPushMatrix (void);
RC_API void RC_APIENTRY rcReadPixels (RCint x, RCint y, RCsizei width, RCsizei height, RCenum format, RCenum type, RCvoid *pixels);
RC_API void RC_APIENTRY rcRotatex (RCfixed angle, RCfixed x, RCfixed y, RCfixed z);
RC_API void RC_APIENTRY rcScalex (RCfixed x, RCfixed y, RCfixed z);
RC_API void RC_APIENTRY rcTexCoordPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcTexImage2D (RCenum target, RCint level, RCint internalformat, RCsizei width, RCsizei height, RCint border, RCenum format, RCenum type, const RCvoid *pixels);
RC_API void RC_APIENTRY rcTexParameteri (RCenum target, RCenum pname, RCint param);
RC_API void RC_APIENTRY rcTexParameterx (RCenum target, RCenum pname, RCfixed param);
RC_API void RC_APIENTRY rcTexParameteriv (RCenum target, RCenum pname, const RCint *params);
RC_API void RC_APIENTRY rcTexParameterxv (RCenum target, RCenum pname, const RCfixed *params);
RC_API void RC_APIENTRY rcTexSubImage2D (RCenum target, RCint level, RCint xoffset, RCint yoffset, RCsizei width, RCsizei height, RCenum format, RCenum type, const RCvoid *pixels);
RC_API void RC_APIENTRY rcTranslatex (RCfixed x, RCfixed y, RCfixed z);
RC_API void RC_APIENTRY rcVertexPointer (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcViewport (RCint x, RCint y, RCsizei width, RCsizei height);

/*------------------------------------------------------------------------*
 * Required OES extension functions
 *------------------------------------------------------------------------*/

#ifndef RC_OES_matrix_palette
#define RC_OES_matrix_palette 1
RC_API void RC_APIENTRY rcCurrentPaletteMatrixOES (RCuint matrixpaletteindex);
RC_API void RC_APIENTRY rcLoadPaletteFromModelViewMatrixOES (void);
RC_API void RC_APIENTRY rcMatrixIndexPointerOES (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
RC_API void RC_APIENTRY rcWeightPointerOES (RCint size, RCenum type, RCsizei stride, const RCvoid *pointer);
#endif


/*------------------------------------------------------------------------*
 * Extended Functions
 *------------------------------------------------------------------------*/
RC_API void RC_APIENTRY rcuPerspective ( RCfloat fovy, RCfloat aspect, RCfloat zNear, RCfloat zFar);
RC_API void RC_APIENTRY rcuLookAt (RCfloat eyex, RCfloat eyey, RCfloat eyez,
                                   RCfloat centerx, RCfloat centery, RCfloat centerz,
                                   RCfloat upx, RCfloat upy, RCfloat upz);
RC_API void RC_APIENTRY rcSceneAllInit(void);
RC_API void RC_APIENTRY rcStaticSceneBegin(void);
RC_API void RC_APIENTRY rcStaticSceneEnd(void);

RC_API void RC_APIENTRY rcTextureAlpha(RCbyte value);
RC_API void RC_APIENTRY rcDepthBounce (RCuint value);

#ifdef __cplusplus
}
#endif

#endif /* __OPENRC_H__ */
