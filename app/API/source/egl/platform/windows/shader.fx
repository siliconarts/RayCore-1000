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
// Project : API
// DATE    : 4/30/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

struct VS_INPUT
{
    float4 Position     : POSITION;	// vertex position 
    float2 TextureUV    : TEXCOORD;	// vertex texture coords 
};

struct VS_OUTPUT
{
    float4 Position   : SV_POSITION;	// vertex position 
    float2 TextureUV  : TEXCOORD;		// vertex texture coords 
};

Texture2D    g_Texture : register( t0 );
SamplerState g_Sampler : register( s0 );

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
	VS_OUTPUT	output	= input;
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_TARGET
{
	return g_Texture.Sample( g_Sampler, input.TextureUV );
}
