/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2015 Robert Beckebans
Copyright (C) 2014 Carl Kenner

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#pragma hdrstop
#include "precompiled.h"

#include "../framework/Common_local.h"
#include "RenderCommon.h"
#include "Framebuffer.h"


idCVar r_drawEyeColor( "r_drawEyeColor", "0", CVAR_RENDERER | CVAR_BOOL, "Draw a colored box, red = left eye, blue = right eye, grey = non-stereo" );
idCVar r_motionBlur( "r_motionBlur", "0", CVAR_RENDERER | CVAR_INTEGER | CVAR_ARCHIVE, "1 - 5, log2 of the number of motion blur samples" );
idCVar r_forceZPassStencilShadows( "r_forceZPassStencilShadows", "0", CVAR_RENDERER | CVAR_BOOL, "force Z-pass rendering for performance testing" );
idCVar r_useStencilShadowPreload( "r_useStencilShadowPreload", "0", CVAR_RENDERER | CVAR_BOOL, "use stencil shadow preload algorithm instead of Z-fail" );
idCVar r_skipShaderPasses( "r_skipShaderPasses", "0", CVAR_RENDERER | CVAR_BOOL, "" );
idCVar r_skipInteractionFastPath( "r_skipInteractionFastPath", "1", CVAR_RENDERER | CVAR_BOOL, "" );
idCVar r_useLightStencilSelect( "r_useLightStencilSelect", "0", CVAR_RENDERER | CVAR_BOOL, "use stencil select pass" );

//extern idCVar stereoRender_swapEyes;


/*
================
SetVertexParm
================
*/
static ID_INLINE void SetVertexParm( renderParm_t rp, const float* value )
{
	renderProgManager.SetUniformValue( rp, value );
}

/*
================
SetVertexParms
================
*/
static ID_INLINE void SetVertexParms( renderParm_t rp, const float* value, int num )
{
	for( int i = 0; i < num; i++ )
	{
		renderProgManager.SetUniformValue( ( renderParm_t )( rp + i ), value + ( i * 4 ) );
	}
}

/*
================
SetFragmentParm
================
*/
static ID_INLINE void SetFragmentParm( renderParm_t rp, const float* value )
{
	renderProgManager.SetUniformValue( rp, value );
}

/*
====================
PrintState
====================
*/
#if 0
void PrintState( uint64 stateBits, uint64* stencilBits )
{
	if( renderLog.Active() == 0 )
	{
		return;
	}
	
	renderLog.OpenBlock( "GL_State" );
	
	// culling
	renderLog.Printf( "Culling: " );
	switch( stateBits & GLS_CULL_BITS )
	{
		case GLS_CULL_FRONTSIDED:
			renderLog.Printf_NoIndent( "FRONTSIDED -> BACK" );
			break;
		case GLS_CULL_BACKSIDED:
			renderLog.Printf_NoIndent( "BACKSIDED -> FRONT" );
			break;
		case GLS_CULL_TWOSIDED:
			renderLog.Printf_NoIndent( "TWOSIDED" );
			break;
		default:
			renderLog.Printf_NoIndent( "NA" );
			break;
	}
	renderLog.Printf_NoIndent( "\n" );
	
	// polygon mode
	renderLog.Printf( "PolygonMode: %s\n", ( stateBits & GLS_POLYMODE_LINE ) ? "LINE" : "FILL" );
	
	// color mask
	renderLog.Printf( "ColorMask: " );
	renderLog.Printf_NoIndent( ( stateBits & GLS_REDMASK ) ? "_" : "R" );
	renderLog.Printf_NoIndent( ( stateBits & GLS_GREENMASK ) ? "_" : "G" );
	renderLog.Printf_NoIndent( ( stateBits & GLS_BLUEMASK ) ? "_" : "B" );
	renderLog.Printf_NoIndent( ( stateBits & GLS_ALPHAMASK ) ? "_" : "A" );
	renderLog.Printf_NoIndent( "\n" );
	
	// blend
	renderLog.Printf( "Blend: src=" );
	switch( stateBits & GLS_SRCBLEND_BITS )
	{
		case GLS_SRCBLEND_ZERO:
			renderLog.Printf_NoIndent( "ZERO" );
			break;
		case GLS_SRCBLEND_ONE:
			renderLog.Printf_NoIndent( "ONE" );
			break;
		case GLS_SRCBLEND_DST_COLOR:
			renderLog.Printf_NoIndent( "DST_COLOR" );
			break;
		case GLS_SRCBLEND_ONE_MINUS_DST_COLOR:
			renderLog.Printf_NoIndent( "ONE_MINUS_DST_COLOR" );
			break;
		case GLS_SRCBLEND_SRC_ALPHA:
			renderLog.Printf_NoIndent( "SRC_ALPHA" );
			break;
		case GLS_SRCBLEND_ONE_MINUS_SRC_ALPHA:
			renderLog.Printf_NoIndent( "ONE_MINUS_SRC_ALPHA" );
			break;
		case GLS_SRCBLEND_DST_ALPHA:
			renderLog.Printf_NoIndent( "DST_ALPHA" );
			break;
		case GLS_SRCBLEND_ONE_MINUS_DST_ALPHA:
			renderLog.Printf_NoIndent( "ONE_MINUS_DST_ALPHA" );
			break;
		default:
			renderLog.Printf_NoIndent( "NA" );
			break;
	}
	renderLog.Printf_NoIndent( ", dst=" );
	switch( stateBits & GLS_DSTBLEND_BITS )
	{
		case GLS_DSTBLEND_ZERO:
			renderLog.Printf_NoIndent( "ZERO" );
			break;
		case GLS_DSTBLEND_ONE:
			renderLog.Printf_NoIndent( "ONE" );
			break;
		case GLS_DSTBLEND_SRC_COLOR:
			renderLog.Printf_NoIndent( "SRC_COLOR" );
			break;
		case GLS_DSTBLEND_ONE_MINUS_SRC_COLOR:
			renderLog.Printf_NoIndent( "ONE_MINUS_SRC_COLOR" );
			break;
		case GLS_DSTBLEND_SRC_ALPHA:
			renderLog.Printf_NoIndent( "SRC_ALPHA" );
			break;
		case GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA:
			renderLog.Printf_NoIndent( "ONE_MINUS_SRC_ALPHA" );
			break;
		case GLS_DSTBLEND_DST_ALPHA:
			renderLog.Printf_NoIndent( "DST_ALPHA" );
			break;
		case GLS_DSTBLEND_ONE_MINUS_DST_ALPHA:
			renderLog.Printf_NoIndent( "ONE_MINUS_DST_ALPHA" );
			break;
		default:
			renderLog.Printf_NoIndent( "NA" );
	}
	renderLog.Printf_NoIndent( "\n" );
	
	// depth func
	renderLog.Printf( "DepthFunc: " );
	switch( stateBits & GLS_DEPTHFUNC_BITS )
	{
		case GLS_DEPTHFUNC_EQUAL:
			renderLog.Printf_NoIndent( "EQUAL" );
			break;
		case GLS_DEPTHFUNC_ALWAYS:
			renderLog.Printf_NoIndent( "ALWAYS" );
			break;
		case GLS_DEPTHFUNC_LESS:
			renderLog.Printf_NoIndent( "LEQUAL" );
			break;
		case GLS_DEPTHFUNC_GREATER:
			renderLog.Printf_NoIndent( "GEQUAL" );
			break;
		default:
			renderLog.Printf_NoIndent( "NA" );
			break;
	}
	renderLog.Printf_NoIndent( "\n" );
	
	// depth mask
	renderLog.Printf( "DepthWrite: %s\n", ( stateBits & GLS_DEPTHMASK ) ? "FALSE" : "TRUE" );
	
	renderLog.Printf( "DepthBounds: %s\n", ( stateBits & GLS_DEPTH_TEST_MASK ) ? "TRUE" : "FALSE" );
	
	// depth bias
	renderLog.Printf( "DepthBias: %s\n", ( stateBits & GLS_POLYGON_OFFSET ) ? "TRUE" : "FALSE" );
	
	// stencil
	auto printStencil = [&]( stencilFace_t face, uint64 bits, uint64 mask, uint64 ref )
	{
		renderLog.Printf( "Stencil: %s, ", ( bits & ( GLS_STENCIL_FUNC_BITS | GLS_STENCIL_OP_BITS ) ) ? "ON" : "OFF" );
		renderLog.Printf_NoIndent( "Face=" );
		switch( face )
		{
			case STENCIL_FACE_FRONT:
				renderLog.Printf_NoIndent( "FRONT" );
				break;
			case STENCIL_FACE_BACK:
				renderLog.Printf_NoIndent( "BACK" );
				break;
			default:
				renderLog.Printf_NoIndent( "BOTH" );
				break;
		}
		renderLog.Printf_NoIndent( ", Func=" );
		switch( bits & GLS_STENCIL_FUNC_BITS )
		{
			case GLS_STENCIL_FUNC_NEVER:
				renderLog.Printf_NoIndent( "NEVER" );
				break;
			case GLS_STENCIL_FUNC_LESS:
				renderLog.Printf_NoIndent( "LESS" );
				break;
			case GLS_STENCIL_FUNC_EQUAL:
				renderLog.Printf_NoIndent( "EQUAL" );
				break;
			case GLS_STENCIL_FUNC_LEQUAL:
				renderLog.Printf_NoIndent( "LEQUAL" );
				break;
			case GLS_STENCIL_FUNC_GREATER:
				renderLog.Printf_NoIndent( "GREATER" );
				break;
			case GLS_STENCIL_FUNC_NOTEQUAL:
				renderLog.Printf_NoIndent( "NOTEQUAL" );
				break;
			case GLS_STENCIL_FUNC_GEQUAL:
				renderLog.Printf_NoIndent( "GEQUAL" );
				break;
			case GLS_STENCIL_FUNC_ALWAYS:
				renderLog.Printf_NoIndent( "ALWAYS" );
				break;
			default:
				renderLog.Printf_NoIndent( "NA" );
				break;
		}
		renderLog.Printf_NoIndent( ", OpFail=" );
		switch( bits & GLS_STENCIL_OP_FAIL_BITS )
		{
			case GLS_STENCIL_OP_FAIL_KEEP:
				renderLog.Printf_NoIndent( "KEEP" );
				break;
			case GLS_STENCIL_OP_FAIL_ZERO:
				renderLog.Printf_NoIndent( "ZERO" );
				break;
			case GLS_STENCIL_OP_FAIL_REPLACE:
				renderLog.Printf_NoIndent( "REPLACE" );
				break;
			case GLS_STENCIL_OP_FAIL_INCR:
				renderLog.Printf_NoIndent( "INCR" );
				break;
			case GLS_STENCIL_OP_FAIL_DECR:
				renderLog.Printf_NoIndent( "DECR" );
				break;
			case GLS_STENCIL_OP_FAIL_INVERT:
				renderLog.Printf_NoIndent( "INVERT" );
				break;
			case GLS_STENCIL_OP_FAIL_INCR_WRAP:
				renderLog.Printf_NoIndent( "INCR_WRAP" );
				break;
			case GLS_STENCIL_OP_FAIL_DECR_WRAP:
				renderLog.Printf_NoIndent( "DECR_WRAP" );
				break;
			default:
				renderLog.Printf_NoIndent( "NA" );
				break;
		}
		renderLog.Printf_NoIndent( ", ZFail=" );
		switch( bits & GLS_STENCIL_OP_ZFAIL_BITS )
		{
			case GLS_STENCIL_OP_ZFAIL_KEEP:
				renderLog.Printf_NoIndent( "KEEP" );
				break;
			case GLS_STENCIL_OP_ZFAIL_ZERO:
				renderLog.Printf_NoIndent( "ZERO" );
				break;
			case GLS_STENCIL_OP_ZFAIL_REPLACE:
				renderLog.Printf_NoIndent( "REPLACE" );
				break;
			case GLS_STENCIL_OP_ZFAIL_INCR:
				renderLog.Printf_NoIndent( "INCR" );
				break;
			case GLS_STENCIL_OP_ZFAIL_DECR:
				renderLog.Printf_NoIndent( "DECR" );
				break;
			case GLS_STENCIL_OP_ZFAIL_INVERT:
				renderLog.Printf_NoIndent( "INVERT" );
				break;
			case GLS_STENCIL_OP_ZFAIL_INCR_WRAP:
				renderLog.Printf_NoIndent( "INCR_WRAP" );
				break;
			case GLS_STENCIL_OP_ZFAIL_DECR_WRAP:
				renderLog.Printf_NoIndent( "DECR_WRAP" );
				break;
			default:
				renderLog.Printf_NoIndent( "NA" );
				break;
		}
		renderLog.Printf_NoIndent( ", OpPass=" );
		switch( bits & GLS_STENCIL_OP_PASS_BITS )
		{
			case GLS_STENCIL_OP_PASS_KEEP:
				renderLog.Printf_NoIndent( "KEEP" );
				break;
			case GLS_STENCIL_OP_PASS_ZERO:
				renderLog.Printf_NoIndent( "ZERO" );
				break;
			case GLS_STENCIL_OP_PASS_REPLACE:
				renderLog.Printf_NoIndent( "REPLACE" );
				break;
			case GLS_STENCIL_OP_PASS_INCR:
				renderLog.Printf_NoIndent( "INCR" );
				break;
			case GLS_STENCIL_OP_PASS_DECR:
				renderLog.Printf_NoIndent( "DECR" );
				break;
			case GLS_STENCIL_OP_PASS_INVERT:
				renderLog.Printf_NoIndent( "INVERT" );
				break;
			case GLS_STENCIL_OP_PASS_INCR_WRAP:
				renderLog.Printf_NoIndent( "INCR_WRAP" );
				break;
			case GLS_STENCIL_OP_PASS_DECR_WRAP:
				renderLog.Printf_NoIndent( "DECR_WRAP" );
				break;
			default:
				renderLog.Printf_NoIndent( "NA" );
				break;
		}
		renderLog.Printf_NoIndent( ", mask=%llu, ref=%llu\n", mask, ref );
	};
	
	uint32 mask = uint32( ( stateBits & GLS_STENCIL_FUNC_MASK_BITS ) >> GLS_STENCIL_FUNC_MASK_SHIFT );
	uint32 ref = uint32( ( stateBits & GLS_STENCIL_FUNC_REF_BITS ) >> GLS_STENCIL_FUNC_REF_SHIFT );
	if( stateBits & GLS_SEPARATE_STENCIL )
	{
		printStencil( STENCIL_FACE_FRONT, stencilBits[ 0 ], mask, ref );
		printStencil( STENCIL_FACE_BACK, stencilBits[ 1 ], mask, ref );
	}
	else
	{
		printStencil( STENCIL_FACE_NUM, stateBits, mask, ref );
	}
	
	renderLog.CloseBlock();
}
#endif

/*
================
RB_SetMVP
================
*/
void RB_SetMVP( const idRenderMatrix& mvp )
{
	SetVertexParms( RENDERPARM_MVPMATRIX_X, mvp[0], 4 );
}

/*
================
RB_SetMVPWithStereoOffset
================
*/
static void RB_SetMVPWithStereoOffset( const idRenderMatrix& mvp, const float stereoOffset )
{
	idRenderMatrix offset = mvp;
	offset[0][3] += stereoOffset;
	
	SetVertexParms( RENDERPARM_MVPMATRIX_X, offset[0], 4 );
}

static const float zero[4] = { 0, 0, 0, 0 };
static const float one[4] = { 1, 1, 1, 1 };
static const float negOne[4] = { -1, -1, -1, -1 };

/*
================
RB_SetVertexColorParms
================
*/
void RB_SetVertexColorParms( stageVertexColor_t svc )
{
	switch( svc )
	{
		case SVC_IGNORE:
			SetVertexParm( RENDERPARM_VERTEXCOLOR_MODULATE, zero );
			SetVertexParm( RENDERPARM_VERTEXCOLOR_ADD, one );
			break;
		case SVC_MODULATE:
			SetVertexParm( RENDERPARM_VERTEXCOLOR_MODULATE, one );
			SetVertexParm( RENDERPARM_VERTEXCOLOR_ADD, zero );
			break;
		case SVC_INVERSE_MODULATE:
			SetVertexParm( RENDERPARM_VERTEXCOLOR_MODULATE, negOne );
			SetVertexParm( RENDERPARM_VERTEXCOLOR_ADD, one );
			break;
	}
}



/*
======================
RB_GetShaderTextureMatrix
======================
*/
void RB_GetShaderTextureMatrix( const float* shaderRegisters, const textureStage_t* texture, float matrix[16] )
{
	matrix[0 * 4 + 0] = shaderRegisters[ texture->matrix[0][0] ];
	matrix[1 * 4 + 0] = shaderRegisters[ texture->matrix[0][1] ];
	matrix[2 * 4 + 0] = 0.0f;
	matrix[3 * 4 + 0] = shaderRegisters[ texture->matrix[0][2] ];
	
	matrix[0 * 4 + 1] = shaderRegisters[ texture->matrix[1][0] ];
	matrix[1 * 4 + 1] = shaderRegisters[ texture->matrix[1][1] ];
	matrix[2 * 4 + 1] = 0.0f;
	matrix[3 * 4 + 1] = shaderRegisters[ texture->matrix[1][2] ];
	
	// we attempt to keep scrolls from generating incredibly large texture values, but
	// center rotations and center scales can still generate offsets that need to be > 1
	if( matrix[3 * 4 + 0] < -40.0f || matrix[12] > 40.0f )
	{
		matrix[3 * 4 + 0] -= ( int )matrix[3 * 4 + 0];
	}
	if( matrix[13] < -40.0f || matrix[13] > 40.0f )
	{
		matrix[13] -= ( int )matrix[13];
	}
	
	matrix[0 * 4 + 2] = 0.0f;
	matrix[1 * 4 + 2] = 0.0f;
	matrix[2 * 4 + 2] = 1.0f;
	matrix[3 * 4 + 2] = 0.0f;
	
	matrix[0 * 4 + 3] = 0.0f;
	matrix[1 * 4 + 3] = 0.0f;
	matrix[2 * 4 + 3] = 0.0f;
	matrix[3 * 4 + 3] = 1.0f;
}

/*
======================
RB_LoadShaderTextureMatrix
======================
*/
void RB_LoadShaderTextureMatrix( const float* shaderRegisters, const textureStage_t* texture )
{
	float texS[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	float texT[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
	
	if( texture->hasMatrix )
	{
		float matrix[16];
		RB_GetShaderTextureMatrix( shaderRegisters, texture, matrix );
		texS[0] = matrix[0 * 4 + 0];
		texS[1] = matrix[1 * 4 + 0];
		texS[2] = matrix[2 * 4 + 0];
		texS[3] = matrix[3 * 4 + 0];
		
		texT[0] = matrix[0 * 4 + 1];
		texT[1] = matrix[1 * 4 + 1];
		texT[2] = matrix[2 * 4 + 1];
		texT[3] = matrix[3 * 4 + 1];
		
		RENDERLOG_PRINTF( "Setting Texture Matrix\n" );
		renderLog.Indent();
		RENDERLOG_PRINTF( "Texture Matrix S : %4.3f, %4.3f, %4.3f, %4.3f\n", texS[0], texS[1], texS[2], texS[3] );
		RENDERLOG_PRINTF( "Texture Matrix T : %4.3f, %4.3f, %4.3f, %4.3f\n", texT[0], texT[1], texT[2], texT[3] );
		renderLog.Outdent();
	}
	
	SetVertexParm( RENDERPARM_TEXTUREMATRIX_S, texS );
	SetVertexParm( RENDERPARM_TEXTUREMATRIX_T, texT );
}

/*
=====================
RB_BakeTextureMatrixIntoTexgen
=====================
*/
void RB_BakeTextureMatrixIntoTexgen( idPlane lightProject[3], const float* textureMatrix )
{
	float genMatrix[16];
	float final[16];
	
	genMatrix[0 * 4 + 0] = lightProject[0][0];
	genMatrix[1 * 4 + 0] = lightProject[0][1];
	genMatrix[2 * 4 + 0] = lightProject[0][2];
	genMatrix[3 * 4 + 0] = lightProject[0][3];
	
	genMatrix[0 * 4 + 1] = lightProject[1][0];
	genMatrix[1 * 4 + 1] = lightProject[1][1];
	genMatrix[2 * 4 + 1] = lightProject[1][2];
	genMatrix[3 * 4 + 1] = lightProject[1][3];
	
	genMatrix[0 * 4 + 2] = 0.0f;
	genMatrix[1 * 4 + 2] = 0.0f;
	genMatrix[2 * 4 + 2] = 0.0f;
	genMatrix[3 * 4 + 2] = 0.0f;
	
	genMatrix[0 * 4 + 3] = lightProject[2][0];
	genMatrix[1 * 4 + 3] = lightProject[2][1];
	genMatrix[2 * 4 + 3] = lightProject[2][2];
	genMatrix[3 * 4 + 3] = lightProject[2][3];
	
	R_MatrixMultiply( genMatrix, textureMatrix, final );
	
	lightProject[0][0] = final[0 * 4 + 0];
	lightProject[0][1] = final[1 * 4 + 0];
	lightProject[0][2] = final[2 * 4 + 0];
	lightProject[0][3] = final[3 * 4 + 0];
	
	lightProject[1][0] = final[0 * 4 + 1];
	lightProject[1][1] = final[1 * 4 + 1];
	lightProject[1][2] = final[2 * 4 + 1];
	lightProject[1][3] = final[3 * 4 + 1];
}

/*
======================
idRenderBackend::BindVariableStageImage

Handles generating a cinematic frame if needed
======================
*/
void idRenderBackend::BindVariableStageImage( const textureStage_t* texture, const float* shaderRegisters )
{
	if( texture->cinematic )
	{
//#if defined(USE_VULKAN)
//	
//		// FIXME upload of video image sub resources
//		GL_SelectTexture( 0 );
//		globalImages->defaultImage->Bind();
//#else
		cinData_t cin;
		
		if( r_skipDynamicTextures.GetBool() )
		{
			globalImages->defaultImage->Bind();
			return;
		}
		
		// offset time by shaderParm[7] (FIXME: make the time offset a parameter of the shader?)
		// We make no attempt to optimize for multiple identical cinematics being in view, or
		// for cinematics going at a lower framerate than the renderer.
		cin = texture->cinematic->ImageForTime( viewDef->renderView.time[0] + idMath::Ftoi( 1000.0f * viewDef->renderView.shaderParms[11] ) );
		if( cin.imageY != NULL )
		{
			GL_SelectTexture( 0 );
			cin.imageY->Bind();
		
			GL_SelectTexture( 1 );
			cin.imageCr->Bind();
		
			GL_SelectTexture( 2 );
			cin.imageCb->Bind();
		
			// DG: imageY is only used for bink videos (with libbinkdec), so the bink shader must be used
			if( viewDef->is2Dgui )
			{
				renderProgManager.BindShader_BinkGUI();
			}
			else
			{
				renderProgManager.BindShader_Bink();
			}
		}
		else if( cin.image != NULL )
		{
			// Carl: A single RGB image works better with the FFMPEG BINK codec.
			GL_SelectTexture( 0 );
			cin.image->Bind();
		
			
			if( viewDef->is2Dgui )
			{
				renderProgManager.BindShader_TextureVertexColor_sRGB();
			}
			else
			{
				renderProgManager.BindShader_TextureVertexColor();
			}
			
		}
		else
		{
			globalImages->blackImage->Bind();
			// because the shaders may have already been set - we need to make sure we are not using a bink shader which would
			// display incorrectly.  We may want to get rid of RB_BindVariableStageImage and inline the code so that the
			// SWF GUI case is handled better, too
			renderProgManager.BindShader_TextureVertexColor();
		}
//#endif
	}
	else
	{
		// FIXME: see why image is invalid
		if( texture->image != NULL )
		{
			texture->image->Bind();
		}
	}
}

/*
================
idRenderBackend::PrepareStageTexturing
================
*/
void idRenderBackend::PrepareStageTexturing( const shaderStage_t* pStage,  const drawSurf_t* surf )
{
	float useTexGenParm[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	// set the texture matrix if needed
	RB_LoadShaderTextureMatrix( surf->shaderRegisters, &pStage->texture );
	
	// texgens
	if( pStage->texture.texgen == TG_REFLECT_CUBE )
	{
	
		// see if there is also a bump map specified
		const shaderStage_t* bumpStage = surf->material->GetBumpStage();
		if( bumpStage != NULL )
		{
			// per-pixel reflection mapping with bump mapping
			GL_SelectTexture( 1 );
			bumpStage->texture.image->Bind();
			GL_SelectTexture( 0 );
			
			RENDERLOG_PRINTF( "TexGen: TG_REFLECT_CUBE: Bumpy Environment\n" );
			if( surf->jointCache )
			{
				renderProgManager.BindShader_BumpyEnvironmentSkinned();
			}
			else
			{
				renderProgManager.BindShader_BumpyEnvironment();
			}
		}
		else
		{
			RENDERLOG_PRINTF( "TexGen: TG_REFLECT_CUBE: Environment\n" );
			if( surf->jointCache )
			{
				renderProgManager.BindShader_EnvironmentSkinned();
			}
			else
			{
				renderProgManager.BindShader_Environment();
			}
		}
		
	}
	else if( pStage->texture.texgen == TG_SKYBOX_CUBE )
	{
	
		renderProgManager.BindShader_SkyBox();
		
	}
	else if( pStage->texture.texgen == TG_WOBBLESKY_CUBE )
	{
	
		const int* parms = surf->material->GetTexGenRegisters();
		
		float wobbleDegrees = surf->shaderRegisters[ parms[0] ] * ( idMath::PI / 180.0f );
		float wobbleSpeed = surf->shaderRegisters[ parms[1] ] * ( 2.0f * idMath::PI / 60.0f );
		float rotateSpeed = surf->shaderRegisters[ parms[2] ] * ( 2.0f * idMath::PI / 60.0f );
		
		idVec3 axis[3];
		{
			// very ad-hoc "wobble" transform
			float s, c;
			idMath::SinCos( wobbleSpeed * viewDef->renderView.time[0] * 0.001f, s, c );
			
			float ws, wc;
			idMath::SinCos( wobbleDegrees, ws, wc );
			
			axis[2][0] = ws * c;
			axis[2][1] = ws * s;
			axis[2][2] = wc;
			
			axis[1][0] = -s * s * ws;
			axis[1][2] = -s * ws * ws;
			axis[1][1] = idMath::Sqrt( idMath::Fabs( 1.0f - ( axis[1][0] * axis[1][0] + axis[1][2] * axis[1][2] ) ) );
			
			// make the second vector exactly perpendicular to the first
			axis[1] -= ( axis[2] * axis[1] ) * axis[2];
			axis[1].Normalize();
			
			// construct the third with a cross
			axis[0].Cross( axis[1], axis[2] );
		}
		
		// add the rotate
		float rs, rc;
		idMath::SinCos( rotateSpeed * viewDef->renderView.time[0] * 0.001f, rs, rc );
		
		float transform[12];
		transform[0 * 4 + 0] = axis[0][0] * rc + axis[1][0] * rs;
		transform[0 * 4 + 1] = axis[0][1] * rc + axis[1][1] * rs;
		transform[0 * 4 + 2] = axis[0][2] * rc + axis[1][2] * rs;
		transform[0 * 4 + 3] = 0.0f;
		
		transform[1 * 4 + 0] = axis[1][0] * rc - axis[0][0] * rs;
		transform[1 * 4 + 1] = axis[1][1] * rc - axis[0][1] * rs;
		transform[1 * 4 + 2] = axis[1][2] * rc - axis[0][2] * rs;
		transform[1 * 4 + 3] = 0.0f;
		
		transform[2 * 4 + 0] = axis[2][0];
		transform[2 * 4 + 1] = axis[2][1];
		transform[2 * 4 + 2] = axis[2][2];
		transform[2 * 4 + 3] = 0.0f;
		
		SetVertexParms( RENDERPARM_WOBBLESKY_X, transform, 3 );
		renderProgManager.BindShader_WobbleSky();
		
	}
	else if( ( pStage->texture.texgen == TG_SCREEN ) || ( pStage->texture.texgen == TG_SCREEN2 ) )
	{
	
		useTexGenParm[0] = 1.0f;
		useTexGenParm[1] = 1.0f;
		useTexGenParm[2] = 1.0f;
		useTexGenParm[3] = 1.0f;
		
		float mat[16];
		R_MatrixMultiply( surf->space->modelViewMatrix, viewDef->projectionMatrix, mat );
		
		RENDERLOG_PRINTF( "TexGen : %s\n", ( pStage->texture.texgen == TG_SCREEN ) ? "TG_SCREEN" : "TG_SCREEN2" );
		renderLog.Indent();
		
		float plane[4];
		plane[0] = mat[0 * 4 + 0];
		plane[1] = mat[1 * 4 + 0];
		plane[2] = mat[2 * 4 + 0];
		plane[3] = mat[3 * 4 + 0];
		SetVertexParm( RENDERPARM_TEXGEN_0_S, plane );
		RENDERLOG_PRINTF( "TEXGEN_S = %4.3f, %4.3f, %4.3f, %4.3f\n",  plane[0], plane[1], plane[2], plane[3] );
		
		plane[0] = mat[0 * 4 + 1];
		plane[1] = mat[1 * 4 + 1];
		plane[2] = mat[2 * 4 + 1];
		plane[3] = mat[3 * 4 + 1];
		SetVertexParm( RENDERPARM_TEXGEN_0_T, plane );
		RENDERLOG_PRINTF( "TEXGEN_T = %4.3f, %4.3f, %4.3f, %4.3f\n",  plane[0], plane[1], plane[2], plane[3] );
		
		plane[0] = mat[0 * 4 + 3];
		plane[1] = mat[1 * 4 + 3];
		plane[2] = mat[2 * 4 + 3];
		plane[3] = mat[3 * 4 + 3];
		SetVertexParm( RENDERPARM_TEXGEN_0_Q, plane );
		RENDERLOG_PRINTF( "TEXGEN_Q = %4.3f, %4.3f, %4.3f, %4.3f\n",  plane[0], plane[1], plane[2], plane[3] );
		
		renderLog.Outdent();
		
	}
	else if( pStage->texture.texgen == TG_DIFFUSE_CUBE )
	{
	
		// As far as I can tell, this is never used
		idLib::Warning( "Using Diffuse Cube! Please contact Brian!" );
		
	}
	else if( pStage->texture.texgen == TG_GLASSWARP )
	{
	
		// As far as I can tell, this is never used
		idLib::Warning( "Using GlassWarp! Please contact Brian!" );
	}
	
	SetVertexParm( RENDERPARM_TEXGEN_0_ENABLED, useTexGenParm );
}

/*
================
idRenderBackend::FinishStageTexturing
================
*/
void idRenderBackend::FinishStageTexturing( const shaderStage_t* pStage, const drawSurf_t* surf )
{
	if( pStage->texture.cinematic )
	{
		// unbind the extra bink textures
		GL_SelectTexture( 0 );
	}
	
	if( pStage->texture.texgen == TG_REFLECT_CUBE )
	{
		// see if there is also a bump map specified
		const shaderStage_t* bumpStage = surf->material->GetBumpStage();
		if( bumpStage != NULL )
		{
			// per-pixel reflection mapping with bump mapping
			GL_SelectTexture( 0 );
		}
		else
		{
			// per-pixel reflection mapping without bump mapping
		}
		renderProgManager.Unbind();
	}
}

// RB: moved this up because we need to call this several times for shadow mapping
void idRenderBackend::ResetViewportAndScissorToDefaultCamera( const viewDef_t* _viewDef )
{
	// set the window clipping
	GL_Viewport( _viewDef->viewport.x1,
				 _viewDef->viewport.y1,
				 _viewDef->viewport.x2 + 1 - _viewDef->viewport.x1,
				 _viewDef->viewport.y2 + 1 - _viewDef->viewport.y1 );
				 
	// the scissor may be smaller than the viewport for subviews
	GL_Scissor( viewDef->viewport.x1 + _viewDef->scissor.x1,
				viewDef->viewport.y1 + _viewDef->scissor.y1,
				_viewDef->scissor.x2 + 1 - _viewDef->scissor.x1,
				_viewDef->scissor.y2 + 1 - _viewDef->scissor.y1 );
				
	currentScissor = viewDef->scissor;
}
// RB end

/*
=========================================================================================

DEPTH BUFFER RENDERING

=========================================================================================
*/

/*
==================
idRenderBackend::FillDepthBufferGeneric
==================
*/
void idRenderBackend::FillDepthBufferGeneric( const drawSurf_t* const* drawSurfs, int numDrawSurfs )
{
	for( int i = 0; i < numDrawSurfs; i++ )
	{
		const drawSurf_t* drawSurf = drawSurfs[i];
		const idMaterial* shader = drawSurf->material;
		
		// translucent surfaces don't put anything in the depth buffer and don't
		// test against it, which makes them fail the mirror clip plane operation
		if( shader->Coverage() == MC_TRANSLUCENT )
		{
			continue;
		}
		
		// get the expressions for conditionals / color / texcoords
		const float* regs = drawSurf->shaderRegisters;
		
		// if all stages of a material have been conditioned off, don't do anything
		int stage = 0;
		for( ; stage < shader->GetNumStages(); stage++ )
		{
			const shaderStage_t* pStage = shader->GetStage( stage );
			// check the stage enable condition
			if( regs[ pStage->conditionRegister ] != 0 )
			{
				break;
			}
		}
		if( stage == shader->GetNumStages() )
		{
			continue;
		}
		
		// change the matrix if needed
		if( drawSurf->space != currentSpace )
		{
			RB_SetMVP( drawSurf->space->mvp );
			
			currentSpace = drawSurf->space;
		}
		
		uint64 surfGLState = 0;
		
		// set polygon offset if necessary
		if( shader->TestMaterialFlag( MF_POLYGONOFFSET ) )
		{
			surfGLState |= GLS_POLYGON_OFFSET;
			GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * shader->GetPolygonOffset() );
		}
		
		// subviews will just down-modulate the color buffer
		idVec4 color;
		if( shader->GetSort() == SS_SUBVIEW )
		{
			surfGLState |= GLS_SRCBLEND_DST_COLOR | GLS_DSTBLEND_ZERO | GLS_DEPTHFUNC_LESS;
			color[0] = 1.0f;
			color[1] = 1.0f;
			color[2] = 1.0f;
			color[3] = 1.0f;
		}
		else
		{
			// others just draw black
			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;
			color[3] = 1.0f;
		}
		
		renderLog.OpenBlock( shader->GetName() );
		
		bool drawSolid = false;
		if( shader->Coverage() == MC_OPAQUE )
		{
			drawSolid = true;
		}
		else if( shader->Coverage() == MC_PERFORATED )
		{
			// we may have multiple alpha tested stages
			// if the only alpha tested stages are condition register omitted,
			// draw a normal opaque surface
			bool didDraw = false;
			
			// perforated surfaces may have multiple alpha tested stages
			for( stage = 0; stage < shader->GetNumStages(); stage++ )
			{
				const shaderStage_t* pStage = shader->GetStage( stage );
				
				if( !pStage->hasAlphaTest )
				{
					continue;
				}
				
				// check the stage enable condition
				if( regs[ pStage->conditionRegister ] == 0 )
				{
					continue;
				}
				
				// if we at least tried to draw an alpha tested stage,
				// we won't draw the opaque surface
				didDraw = true;
				
				// set the alpha modulate
				color[3] = regs[ pStage->color.registers[3] ];
				
				// skip the entire stage if alpha would be black
				if( color[3] <= 0.0f )
				{
					continue;
				}
				
				uint64 stageGLState = surfGLState;
				
				// set privatePolygonOffset if necessary
				if( pStage->privatePolygonOffset )
				{
					GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * pStage->privatePolygonOffset );
					stageGLState |= GLS_POLYGON_OFFSET;
				}
				
				GL_Color( color );
				
				GL_State( stageGLState );
				idVec4 alphaTestValue( regs[ pStage->alphaTestRegister ] );
				SetFragmentParm( RENDERPARM_ALPHA_TEST, alphaTestValue.ToFloatPtr() );
				
				if( drawSurf->jointCache )
				{
					renderProgManager.BindShader_TextureVertexColorSkinned();
				}
				else
				{
					renderProgManager.BindShader_TextureVertexColor();
				}
				
				RB_SetVertexColorParms( SVC_IGNORE );
				
				// bind the texture
				GL_SelectTexture( 0 );
				pStage->texture.image->Bind();
				
				// set texture matrix and texGens
				PrepareStageTexturing( pStage, drawSurf );
				
				// must render with less-equal for Z-Cull to work properly
				assert( ( GL_GetCurrentState() & GLS_DEPTHFUNC_BITS ) == GLS_DEPTHFUNC_LESS );
				
				// draw it
				DrawElementsWithCounters( drawSurf );
				
				// clean up
				FinishStageTexturing( pStage, drawSurf );
				
				// unset privatePolygonOffset if necessary
				if( pStage->privatePolygonOffset )
				{
					GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * shader->GetPolygonOffset() );
				}
			}
			
			if( !didDraw )
			{
				drawSolid = true;
			}
		}
		
		// draw the entire surface solid
		if( drawSolid )
		{
			if( shader->GetSort() == SS_SUBVIEW )
			{
				renderProgManager.BindShader_Color();
				GL_Color( color );
				GL_State( surfGLState );
			}
			else
			{
				if( drawSurf->jointCache )
				{
					renderProgManager.BindShader_DepthSkinned();
				}
				else
				{
					renderProgManager.BindShader_Depth();
				}
				GL_State( surfGLState | GLS_ALPHAMASK );
			}
			
			// must render with less-equal for Z-Cull to work properly
			assert( ( GL_GetCurrentState() & GLS_DEPTHFUNC_BITS ) == GLS_DEPTHFUNC_LESS );
			
			// draw it
			DrawElementsWithCounters( drawSurf );
		}
		
		renderLog.CloseBlock();
	}
	
	SetFragmentParm( RENDERPARM_ALPHA_TEST, vec4_zero.ToFloatPtr() );
}

/*
=====================
idRenderBackend::FillDepthBufferFast

Optimized fast path code.

If there are subview surfaces, they must be guarded in the depth buffer to allow
the mirror / subview to show through underneath the current view rendering.

Surfaces with perforated shaders need the full shader setup done, but should be
drawn after the opaque surfaces.

The bulk of the surfaces should be simple opaque geometry that can be drawn very rapidly.

If there are no subview surfaces, we could clear to black and use fast-Z rendering
on the 360.
=====================
*/
void idRenderBackend::FillDepthBufferFast( drawSurf_t** drawSurfs, int numDrawSurfs )
{
	if( numDrawSurfs == 0 )
	{
		return;
	}
	
	// if we are just doing 2D rendering, no need to fill the depth buffer
	if( viewDef->viewEntitys == NULL )
	{
		return;
	}
	
	renderLog.OpenMainBlock( MRB_FILL_DEPTH_BUFFER );
	renderLog.OpenBlock( "RB_FillDepthBufferFast" );
	
	// force MVP change on first surface
	currentSpace = NULL;
	
	// draw all the subview surfaces, which will already be at the start of the sorted list,
	// with the general purpose path
	GL_State( GLS_DEFAULT );
	
	int	surfNum;
	for( surfNum = 0; surfNum < numDrawSurfs; surfNum++ )
	{
		if( drawSurfs[surfNum]->material->GetSort() != SS_SUBVIEW )
		{
			break;
		}
		
		FillDepthBufferGeneric( &drawSurfs[surfNum], 1 );
	}
	
	const drawSurf_t** perforatedSurfaces = ( const drawSurf_t** )_alloca( numDrawSurfs * sizeof( drawSurf_t* ) );
	int numPerforatedSurfaces = 0;
	
	// draw all the opaque surfaces and build up a list of perforated surfaces that
	// we will defer drawing until all opaque surfaces are done
	GL_State( GLS_DEFAULT );
	
	// continue checking past the subview surfaces
	for( ; surfNum < numDrawSurfs; surfNum++ )
	{
		const drawSurf_t* surf = drawSurfs[ surfNum ];
		const idMaterial* shader = surf->material;
		
		// translucent surfaces don't put anything in the depth buffer
		if( shader->Coverage() == MC_TRANSLUCENT )
		{
			continue;
		}
		if( shader->Coverage() == MC_PERFORATED )
		{
			// save for later drawing
			perforatedSurfaces[ numPerforatedSurfaces ] = surf;
			numPerforatedSurfaces++;
			continue;
		}
		
		// set polygon offset?
		
		// set mvp matrix
		if( surf->space != currentSpace )
		{
			RB_SetMVP( surf->space->mvp );
			currentSpace = surf->space;
		}
		
		renderLog.OpenBlock( shader->GetName() );
		
		if( surf->jointCache )
		{
			renderProgManager.BindShader_DepthSkinned();
		}
		else
		{
			renderProgManager.BindShader_Depth();
		}
		
		// must render with less-equal for Z-Cull to work properly
		assert( ( GL_GetCurrentState() & GLS_DEPTHFUNC_BITS ) == GLS_DEPTHFUNC_LESS );
		
		// draw it solid
		DrawElementsWithCounters( surf );
		
		renderLog.CloseBlock();
	}
	
	// draw all perforated surfaces with the general code path
	if( numPerforatedSurfaces > 0 )
	{
		FillDepthBufferGeneric( perforatedSurfaces, numPerforatedSurfaces );
	}
	
	renderLog.CloseBlock();
	renderLog.CloseMainBlock();
}

/*
=========================================================================================

GENERAL INTERACTION RENDERING

=========================================================================================
*/

const int INTERACTION_TEXUNIT_BUMP			= 0;
const int INTERACTION_TEXUNIT_SPECULARMIX	= 1;
const int INTERACTION_TEXUNIT_BASECOLOR		= 2;
const int INTERACTION_TEXUNIT_FALLOFF		= 3;
const int INTERACTION_TEXUNIT_PROJECTION	= 4;
const int INTERACTION_TEXUNIT_SHADOWMAPS	= 5;
const int INTERACTION_TEXUNIT_JITTER		= 6;

/*
==================
idRenderBackend::SetupInteractionStage
==================
*/
void idRenderBackend::SetupInteractionStage( const shaderStage_t* surfaceStage, const float* surfaceRegs, const float lightColor[4],
		idVec4 matrix[2], float color[4] )
{

	if( surfaceStage->texture.hasMatrix )
	{
		matrix[0][0] = surfaceRegs[surfaceStage->texture.matrix[0][0]];
		matrix[0][1] = surfaceRegs[surfaceStage->texture.matrix[0][1]];
		matrix[0][2] = 0.0f;
		matrix[0][3] = surfaceRegs[surfaceStage->texture.matrix[0][2]];
		
		matrix[1][0] = surfaceRegs[surfaceStage->texture.matrix[1][0]];
		matrix[1][1] = surfaceRegs[surfaceStage->texture.matrix[1][1]];
		matrix[1][2] = 0.0f;
		matrix[1][3] = surfaceRegs[surfaceStage->texture.matrix[1][2]];
		
		// we attempt to keep scrolls from generating incredibly large texture values, but
		// center rotations and center scales can still generate offsets that need to be > 1
		if( matrix[0][3] < -40.0f || matrix[0][3] > 40.0f )
		{
			matrix[0][3] -= idMath::Ftoi( matrix[0][3] );
		}
		if( matrix[1][3] < -40.0f || matrix[1][3] > 40.0f )
		{
			matrix[1][3] -= idMath::Ftoi( matrix[1][3] );
		}
	}
	else
	{
		matrix[0][0] = 1.0f;
		matrix[0][1] = 0.0f;
		matrix[0][2] = 0.0f;
		matrix[0][3] = 0.0f;
		
		matrix[1][0] = 0.0f;
		matrix[1][1] = 1.0f;
		matrix[1][2] = 0.0f;
		matrix[1][3] = 0.0f;
	}
	
	if( color != NULL )
	{
		for( int i = 0; i < 4; i++ )
		{
			// clamp here, so cards with a greater range don't look different.
			// we could perform overbrighting like we do for lights, but
			// it doesn't currently look worth it.
			color[i] = idMath::ClampFloat( 0.0f, 1.0f, surfaceRegs[surfaceStage->color.registers[i]] ) * lightColor[i];
		}
	}
}

/*
=================
idRenderBackend::DrawSingleInteraction
=================
*/
void idRenderBackend::DrawSingleInteraction( drawInteraction_t* din )
{
	if( din->bumpImage == NULL )
	{
		// stage wasn't actually an interaction
		return;
	}
	
	if( din->diffuseImage == NULL || r_skipDiffuse.GetBool() )
	{
		// this isn't a YCoCg black, but it doesn't matter, because
		// the diffuseColor will also be 0
		din->diffuseImage = globalImages->blackImage;
	}
	if( din->specularImage == NULL || r_skipSpecular.GetBool() || din->ambientLight )
	{
		din->specularImage = globalImages->blackImage;
	}
	if( r_skipBump.GetBool() )
	{
		din->bumpImage = globalImages->flatNormalMap;
	}
	
	// if we wouldn't draw anything, don't call the Draw function
	const bool diffuseIsBlack = ( din->diffuseImage == globalImages->blackImage )
								|| ( ( din->diffuseColor[0] <= 0 ) && ( din->diffuseColor[1] <= 0 ) && ( din->diffuseColor[2] <= 0 ) );
	const bool specularIsBlack = ( din->specularImage == globalImages->blackImage )
								 || ( ( din->specularColor[0] <= 0 ) && ( din->specularColor[1] <= 0 ) && ( din->specularColor[2] <= 0 ) );
	if( diffuseIsBlack && specularIsBlack )
	{
		return;
	}
	
	// bump matrix
	SetVertexParm( RENDERPARM_BUMPMATRIX_S, din->bumpMatrix[0].ToFloatPtr() );
	SetVertexParm( RENDERPARM_BUMPMATRIX_T, din->bumpMatrix[1].ToFloatPtr() );
	
	// diffuse matrix
	SetVertexParm( RENDERPARM_DIFFUSEMATRIX_S, din->diffuseMatrix[0].ToFloatPtr() );
	SetVertexParm( RENDERPARM_DIFFUSEMATRIX_T, din->diffuseMatrix[1].ToFloatPtr() );
	
	// specular matrix
	SetVertexParm( RENDERPARM_SPECULARMATRIX_S, din->specularMatrix[0].ToFloatPtr() );
	SetVertexParm( RENDERPARM_SPECULARMATRIX_T, din->specularMatrix[1].ToFloatPtr() );
	
	RB_SetVertexColorParms( din->vertexColor );
	
	SetFragmentParm( RENDERPARM_DIFFUSEMODIFIER, din->diffuseColor.ToFloatPtr() );
	SetFragmentParm( RENDERPARM_SPECULARMODIFIER, din->specularColor.ToFloatPtr() );
	
	// texture 0 will be the per-surface bump map
	GL_SelectTexture( INTERACTION_TEXUNIT_BUMP );
	din->bumpImage->Bind();
	
	// texture 1 is the per-surface specular map
	GL_SelectTexture( INTERACTION_TEXUNIT_SPECULARMIX );
	din->specularImage->Bind();
	
	// texture 2 is the per-surface diffuse map
	GL_SelectTexture( INTERACTION_TEXUNIT_BASECOLOR );
	din->diffuseImage->Bind();
	
	DrawElementsWithCounters( din->surf );
}

/*
=================
RB_SetupForFastPathInteractions

These are common for all fast path surfaces
=================
*/
static void RB_SetupForFastPathInteractions( const idVec4& diffuseColor, const idVec4& specularColor )
{
	const idVec4 sMatrix( 1, 0, 0, 0 );
	const idVec4 tMatrix( 0, 1, 0, 0 );
	
	// bump matrix
	SetVertexParm( RENDERPARM_BUMPMATRIX_S, sMatrix.ToFloatPtr() );
	SetVertexParm( RENDERPARM_BUMPMATRIX_T, tMatrix.ToFloatPtr() );
	
	// diffuse matrix
	SetVertexParm( RENDERPARM_DIFFUSEMATRIX_S, sMatrix.ToFloatPtr() );
	SetVertexParm( RENDERPARM_DIFFUSEMATRIX_T, tMatrix.ToFloatPtr() );
	
	// specular matrix
	SetVertexParm( RENDERPARM_SPECULARMATRIX_S, sMatrix.ToFloatPtr() );
	SetVertexParm( RENDERPARM_SPECULARMATRIX_T, tMatrix.ToFloatPtr() );
	
	RB_SetVertexColorParms( SVC_IGNORE );
	
	SetFragmentParm( RENDERPARM_DIFFUSEMODIFIER, diffuseColor.ToFloatPtr() );
	SetFragmentParm( RENDERPARM_SPECULARMODIFIER, specularColor.ToFloatPtr() );
}

/*
=============
idRenderBackend::RenderInteractions

With added sorting and trivial path work.
=============
*/
void idRenderBackend::RenderInteractions( const drawSurf_t* surfList, const viewLight_t* vLight, int depthFunc, bool performStencilTest, bool useLightDepthBounds )
{
	if( surfList == NULL )
	{
		return;
	}
	
	// change the scissor if needed, it will be constant across all the surfaces lit by the light
	if( !currentScissor.Equals( vLight->scissorRect ) && r_useScissor.GetBool() )
	{
		GL_Scissor( viewDef->viewport.x1 + vLight->scissorRect.x1,
					viewDef->viewport.y1 + vLight->scissorRect.y1,
					vLight->scissorRect.x2 + 1 - vLight->scissorRect.x1,
					vLight->scissorRect.y2 + 1 - vLight->scissorRect.y1 );
					
		currentScissor = vLight->scissorRect;
	}
	
	// perform setup here that will be constant for all interactions
	if( performStencilTest )
	{
		GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | depthFunc | GLS_STENCIL_FUNC_EQUAL | GLS_STENCIL_MAKE_REF( STENCIL_SHADOW_TEST_VALUE ) | GLS_STENCIL_MAKE_MASK( STENCIL_SHADOW_MASK_VALUE ) );
		
	}
	else
	{
		GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | depthFunc | GLS_STENCIL_FUNC_ALWAYS );
	}
	
	// some rare lights have multiple animating stages, loop over them outside the surface list
	const idMaterial* lightShader = vLight->lightShader;
	const float* lightRegs = vLight->shaderRegisters;
	
	drawInteraction_t inter = {};
	inter.ambientLight = lightShader->IsAmbientLight();
	
	//---------------------------------
	// Split out the complex surfaces from the fast-path surfaces
	// so we can do the fast path ones all in a row.
	// The surfaces should already be sorted by space because they
	// are added single-threaded, and there is only a negligable amount
	// of benefit to trying to sort by materials.
	//---------------------------------
	static const int MAX_INTERACTIONS_PER_LIGHT = 1024;
	static const int MAX_COMPLEX_INTERACTIONS_PER_LIGHT = 256;
	idStaticList< const drawSurf_t*, MAX_INTERACTIONS_PER_LIGHT > allSurfaces;
	idStaticList< const drawSurf_t*, MAX_COMPLEX_INTERACTIONS_PER_LIGHT > complexSurfaces;
	for( const drawSurf_t* walk = surfList; walk != NULL; walk = walk->nextOnLight )
	{
	
		// make sure the triangle culling is done
		if( walk->shadowVolumeState != SHADOWVOLUME_DONE )
		{
			assert( walk->shadowVolumeState == SHADOWVOLUME_UNFINISHED || walk->shadowVolumeState == SHADOWVOLUME_DONE );
			
			uint64 start = Sys_Microseconds();
			while( walk->shadowVolumeState == SHADOWVOLUME_UNFINISHED )
			{
				Sys_Yield();
			}
			uint64 end = Sys_Microseconds();
			
			pc.shadowMicroSec += end - start;
		}
		
		const idMaterial* surfaceShader = walk->material;
		if( surfaceShader->GetFastPathBumpImage() )
		{
			allSurfaces.Append( walk );
		}
		else
		{
			complexSurfaces.Append( walk );
		}
	}
	for( int i = 0; i < complexSurfaces.Num(); i++ )
	{
		allSurfaces.Append( complexSurfaces[i] );
	}
	
	bool lightDepthBoundsDisabled = false;
	
	// RB begin
	if( r_useShadowMapping.GetBool() )
	{
		const static int JITTER_SIZE = 128;
		
		// default high quality
		float jitterSampleScale = 1.0f;
		float shadowMapSamples = r_shadowMapSamples.GetInteger();
		
		// screen power of two correction factor
		float screenCorrectionParm[4];
		screenCorrectionParm[0] = 1.0f / ( JITTER_SIZE * shadowMapSamples ) ;
		screenCorrectionParm[1] = 1.0f / JITTER_SIZE;
		screenCorrectionParm[2] = 1.0f / shadowMapResolutions[vLight->shadowLOD];
		screenCorrectionParm[3] = vLight->parallel ? r_shadowMapSunDepthBiasScale.GetFloat() : r_shadowMapRegularDepthBiasScale.GetFloat();
		SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
		
		float jitterTexScale[4];
		jitterTexScale[0] = r_shadowMapJitterScale.GetFloat() * jitterSampleScale;	// TODO shadow buffer size fraction shadowMapSize / maxShadowMapSize
		jitterTexScale[1] = r_shadowMapJitterScale.GetFloat() * jitterSampleScale;
		jitterTexScale[2] = -r_shadowMapBiasScale.GetFloat();
		jitterTexScale[3] = 0.0f;
		SetFragmentParm( RENDERPARM_JITTERTEXSCALE, jitterTexScale ); // rpJitterTexScale
		
		float jitterTexOffset[4];
		if( r_shadowMapRandomizeJitter.GetBool() )
		{
			jitterTexOffset[0] = ( rand() & 255 ) / 255.0;
			jitterTexOffset[1] = ( rand() & 255 ) / 255.0;
		}
		else
		{
			jitterTexOffset[0] = 0;
			jitterTexOffset[1] = 0;
		}
		jitterTexOffset[2] = 0.0f;
		jitterTexOffset[3] = 0.0f;
		SetFragmentParm( RENDERPARM_JITTERTEXOFFSET, jitterTexOffset ); // rpJitterTexOffset
		
		if( vLight->parallel )
		{
			float cascadeDistances[4];
			cascadeDistances[0] = viewDef->frustumSplitDistances[0];
			cascadeDistances[1] = viewDef->frustumSplitDistances[1];
			cascadeDistances[2] = viewDef->frustumSplitDistances[2];
			cascadeDistances[3] = viewDef->frustumSplitDistances[3];
			SetFragmentParm( RENDERPARM_CASCADEDISTANCES, cascadeDistances ); // rpCascadeDistances
		}
		
	}
	// RB end
	
	float lightScale = /*r_useHDR.GetBool() ? 3.0f :*/ r_lightScale.GetFloat(); //GK: Allow the ldr lightlevels to work also on HDR
	
	for( int lightStageNum = 0; lightStageNum < lightShader->GetNumStages(); lightStageNum++ )
	{
		const shaderStage_t*	lightStage = lightShader->GetStage( lightStageNum );
		
		// ignore stages that fail the condition
		if( !lightRegs[ lightStage->conditionRegister ] )
		{
			continue;
		}
		
		const idVec4 lightColor(
			lightScale * lightRegs[ lightStage->color.registers[0] ],
			lightScale * lightRegs[ lightStage->color.registers[1] ],
			lightScale * lightRegs[ lightStage->color.registers[2] ],
			lightRegs[ lightStage->color.registers[3] ] );
		// apply the world-global overbright and the 2x factor for specular
		const idVec4 diffuseColor = lightColor;
		const idVec4 specularColor = lightColor * 2.0f;
		
		float lightTextureMatrix[16];
		if( lightStage->texture.hasMatrix )
		{
			RB_GetShaderTextureMatrix( lightRegs, &lightStage->texture, lightTextureMatrix );
		}
		
		// texture 1 will be the light falloff texture
		GL_SelectTexture( INTERACTION_TEXUNIT_FALLOFF );
		vLight->falloffImage->Bind();
		
		// texture 2 will be the light projection texture
		GL_SelectTexture( INTERACTION_TEXUNIT_PROJECTION );
		lightStage->texture.image->Bind();
		
		if( r_useShadowMapping.GetBool() )
		{
			// texture 5 will be the shadow maps array
			GL_SelectTexture( INTERACTION_TEXUNIT_SHADOWMAPS );
			globalImages->shadowImage[vLight->shadowLOD]->Bind();
			
			// texture 6 will be the jitter texture for soft shadowing
			GL_SelectTexture( INTERACTION_TEXUNIT_JITTER );
			if( r_shadowMapSamples.GetInteger() == 16 )
			{
				globalImages->jitterImage16->Bind();
			}
			else if( r_shadowMapSamples.GetInteger() == 4 )
			{
				globalImages->jitterImage4->Bind();
			}
			else
			{
				globalImages->jitterImage1->Bind();
			}
		}
		
		// force the light textures to not use anisotropic filtering, which is wasted on them
		// all of the texture sampler parms should be constant for all interactions, only
		// the actual texture image bindings will change
		
		//----------------------------------
		// For all surfaces on this light list, generate an interaction for this light stage
		//----------------------------------
		
		// setup renderparms assuming we will be drawing trivial surfaces first
		RB_SetupForFastPathInteractions( diffuseColor, specularColor );
		
		// even if the space does not change between light stages, each light stage may need a different lightTextureMatrix baked in
		currentSpace = NULL;
		
		for( int sortedSurfNum = 0; sortedSurfNum < allSurfaces.Num(); sortedSurfNum++ )
		{
			const drawSurf_t* const surf = allSurfaces[ sortedSurfNum ];
			
			// select the render prog
			if( lightShader->IsAmbientLight() )
			{
				if( surf->jointCache )
				{
					renderProgManager.BindShader_InteractionAmbientSkinned();
				}
				else
				{
					renderProgManager.BindShader_InteractionAmbient();
				}
			}
			else
			{
				if( r_useShadowMapping.GetBool() && vLight->globalShadows )
				{
					// RB: we have shadow mapping enabled and shadow maps so do a shadow compare
					
					if( vLight->parallel )
					{
						if( surf->jointCache )
						{
							renderProgManager.BindShader_Interaction_ShadowMapping_Parallel_Skinned();
						}
						else
						{
							renderProgManager.BindShader_Interaction_ShadowMapping_Parallel();
						}
					}
					else if( vLight->pointLight )
					{
						if( surf->jointCache )
						{
							renderProgManager.BindShader_Interaction_ShadowMapping_Point_Skinned();
						}
						else
						{
							renderProgManager.BindShader_Interaction_ShadowMapping_Point();
						}
					}
					else
					{
						if( surf->jointCache )
						{
							renderProgManager.BindShader_Interaction_ShadowMapping_Spot_Skinned();
						}
						else
						{
							renderProgManager.BindShader_Interaction_ShadowMapping_Spot();
						}
					}
				}
				else
				{
					if( surf->jointCache )
					{
						renderProgManager.BindShader_InteractionSkinned();
					}
					else
					{
						renderProgManager.BindShader_Interaction();
					}
				}
			}
			
			const idMaterial* surfaceShader = surf->material;
			const float* surfaceRegs = surf->shaderRegisters;
			
			inter.surf = surf;
			
			// change the MVP matrix, view/light origin and light projection vectors if needed
			if( surf->space != currentSpace )
			{
				currentSpace = surf->space;
				
				// turn off the light depth bounds test if this model is rendered with a depth hack
				if( useLightDepthBounds )
				{
					if( !surf->space->weaponDepthHack && surf->space->modelDepthHack == 0.0f )
					{
						if( lightDepthBoundsDisabled )
						{
							GL_DepthBoundsTest( vLight->scissorRect.zmin, vLight->scissorRect.zmax );
							lightDepthBoundsDisabled = false;
						}
					}
					else
					{
						if( !lightDepthBoundsDisabled )
						{
							GL_DepthBoundsTest( 0.0f, 0.0f );
							lightDepthBoundsDisabled = true;
						}
					}
				}
				
				// model-view-projection
				RB_SetMVP( surf->space->mvp );
				
				// RB begin
				idRenderMatrix modelMatrix;
				idRenderMatrix::Transpose( *( idRenderMatrix* )surf->space->modelMatrix, modelMatrix );
				
				SetVertexParms( RENDERPARM_MODELMATRIX_X, modelMatrix[0], 4 );
				
				// for determining the shadow mapping cascades
				idRenderMatrix modelViewMatrix, tmp;
				idRenderMatrix::Transpose( *( idRenderMatrix* )surf->space->modelViewMatrix, modelViewMatrix );
				SetVertexParms( RENDERPARM_MODELVIEWMATRIX_X, modelViewMatrix[0], 4 );
				
				idVec4 globalLightOrigin( vLight->globalLightOrigin.x, vLight->globalLightOrigin.y, vLight->globalLightOrigin.z, 1.0f );
				SetVertexParm( RENDERPARM_GLOBALLIGHTORIGIN, globalLightOrigin.ToFloatPtr() );
				// RB end
				
				// tranform the light/view origin into model local space
				idVec4 localLightOrigin( 0.0f );
				idVec4 localViewOrigin( 1.0f );
				R_GlobalPointToLocal( surf->space->modelMatrix, vLight->globalLightOrigin, localLightOrigin.ToVec3() );
				R_GlobalPointToLocal( surf->space->modelMatrix, viewDef->renderView.vieworg, localViewOrigin.ToVec3() );
				
				// set the local light/view origin
				SetVertexParm( RENDERPARM_LOCALLIGHTORIGIN, localLightOrigin.ToFloatPtr() );
				SetVertexParm( RENDERPARM_LOCALVIEWORIGIN, localViewOrigin.ToFloatPtr() );
				
				// transform the light project into model local space
				idPlane lightProjection[4];
				for( int i = 0; i < 4; i++ )
				{
					R_GlobalPlaneToLocal( surf->space->modelMatrix, vLight->lightProject[i], lightProjection[i] );
				}
				
				// optionally multiply the local light projection by the light texture matrix
				if( lightStage->texture.hasMatrix )
				{
					RB_BakeTextureMatrixIntoTexgen( lightProjection, lightTextureMatrix );
				}
				
				// set the light projection
				SetVertexParm( RENDERPARM_LIGHTPROJECTION_S, lightProjection[0].ToFloatPtr() );
				SetVertexParm( RENDERPARM_LIGHTPROJECTION_T, lightProjection[1].ToFloatPtr() );
				SetVertexParm( RENDERPARM_LIGHTPROJECTION_Q, lightProjection[2].ToFloatPtr() );
				SetVertexParm( RENDERPARM_LIGHTFALLOFF_S, lightProjection[3].ToFloatPtr() );
				
				// RB begin
				if( r_useShadowMapping.GetBool() )
				{
					if( vLight->parallel )
					{
						for( int i = 0; i < ( r_shadowMapSplits.GetInteger() + 1 ); i++ )
						{
							idRenderMatrix modelToShadowMatrix;
							idRenderMatrix::Multiply( shadowV[i], modelMatrix, modelToShadowMatrix );
							
							idRenderMatrix shadowClipMVP;
							idRenderMatrix::Multiply( shadowP[i], modelToShadowMatrix, shadowClipMVP );
							
							idRenderMatrix shadowWindowMVP;
							idRenderMatrix::Multiply( renderMatrix_clipSpaceToWindowSpace, shadowClipMVP, shadowWindowMVP );
							
							SetVertexParms( ( renderParm_t )( RENDERPARM_SHADOW_MATRIX_0_X + i * 4 ), shadowWindowMVP[0], 4 );
						}
					}
					else if( vLight->pointLight )
					{
						for( int i = 0; i < 6; i++ )
						{
							idRenderMatrix modelToShadowMatrix;
							idRenderMatrix::Multiply( shadowV[i], modelMatrix, modelToShadowMatrix );
							
							idRenderMatrix shadowClipMVP;
							idRenderMatrix::Multiply( shadowP[i], modelToShadowMatrix, shadowClipMVP );
							
							idRenderMatrix shadowWindowMVP;
							idRenderMatrix::Multiply( renderMatrix_clipSpaceToWindowSpace, shadowClipMVP, shadowWindowMVP );
							
							SetVertexParms( ( renderParm_t )( RENDERPARM_SHADOW_MATRIX_0_X + i * 4 ), shadowWindowMVP[0], 4 );
						}
					}
					else
					{
						// spot light
						
						idRenderMatrix modelToShadowMatrix;
						idRenderMatrix::Multiply( shadowV[0], modelMatrix, modelToShadowMatrix );
						
						idRenderMatrix shadowClipMVP;
						idRenderMatrix::Multiply( shadowP[0], modelToShadowMatrix, shadowClipMVP );
						
						SetVertexParms( ( renderParm_t )( RENDERPARM_SHADOW_MATRIX_0_X ), shadowClipMVP[0], 4 );
						
					}
				}
				// RB end
			}
			
			// check for the fast path
			if( surfaceShader->GetFastPathBumpImage() && !r_skipInteractionFastPath.GetBool() )
			{
				renderLog.OpenBlock( surf->material->GetName() );
				
				// texture 0 will be the per-surface bump map
				GL_SelectTexture( INTERACTION_TEXUNIT_BUMP );
				surfaceShader->GetFastPathBumpImage()->Bind();
				
				// texture 1 is the per-surface specular map
				GL_SelectTexture( INTERACTION_TEXUNIT_SPECULARMIX );
				surfaceShader->GetFastPathSpecularImage()->Bind();
				
				// texture 2 is the per-surface diffuse map
				GL_SelectTexture( INTERACTION_TEXUNIT_BASECOLOR );
				surfaceShader->GetFastPathDiffuseImage()->Bind();
				
				DrawElementsWithCounters( surf );
				
				renderLog.CloseBlock();
				continue;
			}
			
			renderLog.OpenBlock( surf->material->GetName() );
			
			inter.bumpImage = NULL;
			inter.specularImage = NULL;
			inter.diffuseImage = NULL;
			inter.diffuseColor[0] = inter.diffuseColor[1] = inter.diffuseColor[2] = inter.diffuseColor[3] = 0;
			inter.specularColor[0] = inter.specularColor[1] = inter.specularColor[2] = inter.specularColor[3] = 0;
			
			// go through the individual surface stages
			//
			// This is somewhat arcane because of the old support for video cards that had to render
			// interactions in multiple passes.
			//
			// We also have the very rare case of some materials that have conditional interactions
			// for the "hell writing" that can be shined on them.
			for( int surfaceStageNum = 0; surfaceStageNum < surfaceShader->GetNumStages(); surfaceStageNum++ )
			{
				const shaderStage_t*	surfaceStage = surfaceShader->GetStage( surfaceStageNum );
				
				switch( surfaceStage->lighting )
				{
					case SL_COVERAGE:
					{
						// ignore any coverage stages since they should only be used for the depth fill pass
						// for diffuse stages that use alpha test.
						break;
					}
					case SL_AMBIENT:
					{
						// ignore ambient stages while drawing interactions
						break;
					}
					case SL_BUMP:
					{
						// ignore stage that fails the condition
						if( !surfaceRegs[ surfaceStage->conditionRegister ] )
						{
							break;
						}
						// draw any previous interaction
						if( inter.bumpImage != NULL )
						{
							DrawSingleInteraction( &inter );
						}
						inter.bumpImage = surfaceStage->texture.image;
						inter.diffuseImage = NULL;
						inter.specularImage = NULL;
						SetupInteractionStage( surfaceStage, surfaceRegs, NULL, inter.bumpMatrix, NULL );
						break;
					}
					case SL_DIFFUSE:
					{
						// ignore stage that fails the condition
						if( !surfaceRegs[ surfaceStage->conditionRegister ] )
						{
							break;
						}
						// draw any previous interaction
						if( inter.diffuseImage != NULL )
						{
							DrawSingleInteraction( &inter );
						}
						inter.diffuseImage = surfaceStage->texture.image;
						inter.vertexColor = surfaceStage->vertexColor;
						SetupInteractionStage( surfaceStage, surfaceRegs, diffuseColor.ToFloatPtr(),
											   inter.diffuseMatrix, inter.diffuseColor.ToFloatPtr() );
						break;
					}
					case SL_SPECULAR:
					{
						// ignore stage that fails the condition
						if( !surfaceRegs[ surfaceStage->conditionRegister ] )
						{
							break;
						}
						// draw any previous interaction
						if( inter.specularImage != NULL )
						{
							DrawSingleInteraction( &inter );
						}
						inter.specularImage = surfaceStage->texture.image;
						inter.vertexColor = surfaceStage->vertexColor;
						SetupInteractionStage( surfaceStage, surfaceRegs, specularColor.ToFloatPtr(),
											   inter.specularMatrix, inter.specularColor.ToFloatPtr() );
						break;
					}
				}
			}
			
			// draw the final interaction
			DrawSingleInteraction( &inter );
			
			renderLog.CloseBlock();
		}
	}
	
	if( useLightDepthBounds && lightDepthBoundsDisabled )
	{
		GL_DepthBoundsTest( vLight->scissorRect.zmin, vLight->scissorRect.zmax );
	}
	
	renderProgManager.Unbind();
}

// RB begin

/*
=========================================================================================

AMBIENT PASS RENDERING

=========================================================================================
*/

/*
==================
idRenderBackend::AmbientPass
==================
*/
void idRenderBackend::AmbientPass( const drawSurf_t* const* drawSurfs, int numDrawSurfs, bool fillGbuffer )
{
	if( fillGbuffer )
	{
		if( !r_useSSGI.GetBool() && !r_useSSAO.GetBool() )
		{
			return;
		}
	}
	else
	{
		if( r_forceAmbient.GetFloat() <= 0 || r_skipAmbient.GetBool() )
		{
			return;
		}
	}
	
	if( numDrawSurfs == 0 )
	{
		return;
	}
	
	// if we are just doing 2D rendering, no need to fill the depth buffer
	if( viewDef->viewEntitys == NULL )
	{
		return;
	}
	
	const bool hdrIsActive = ( r_useHDR.GetBool() && globalFramebuffers.hdrFBO != NULL && globalFramebuffers.hdrFBO->IsBound() );
	
	renderLog.OpenMainBlock(MRB_AMBIENT_PASS);
	renderLog.OpenBlock("RB_AmbientPass");

	if( fillGbuffer )
	{
		globalFramebuffers.geometryBufferFBO->Bind();
	
		GL_Clear(true, false, false, 0, 0.0f, 0.0f, 0.0f, 1.0f, false);
	}
	
	
	// RB: not needed
	// GL_StartDepthPass( backEnd.viewDef->scissor );
	
	// force MVP change on first surface
	currentSpace = NULL;
	
	// draw all the subview surfaces, which will already be at the start of the sorted list,
	// with the general purpose path
	GL_State( GLS_DEFAULT );
	
#define BLEND_NORMALS 1
	//if( fillGbuffer )
	{
		GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL );
	}
	/*else
	{
		GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL );
	}*/
	
	GL_Color( colorWhite );
	
	const float lightScale = r_lightScale.GetFloat();
	const idVec4 lightColor = colorWhite * lightScale;
	// apply the world-global overbright and the 2x factor for specular
	const idVec4 diffuseColor = lightColor;
	const idVec4 specularColor = lightColor * 2.0f;
	
	idVec4 ambientColor;
	float ambientBoost = 1.0f;
	ambientBoost += r_useSSAO.GetBool() ? 0.2f : 0.0f;
	ambientBoost *= r_useHDR.GetBool() ? 1.1f : 1.0f;
	ambientColor.x = r_forceAmbient.GetFloat() * ambientBoost;
	ambientColor.y = r_forceAmbient.GetFloat() * ambientBoost;
	ambientColor.z = r_forceAmbient.GetFloat() * ambientBoost;
	ambientColor.w = 1;
	
	renderProgManager.SetRenderParm( RENDERPARM_AMBIENT_COLOR, ambientColor.ToFloatPtr() );
	
	// setup renderparms assuming we will be drawing trivial surfaces first
	RB_SetupForFastPathInteractions( diffuseColor, specularColor );
	
	for( int i = 0; i < numDrawSurfs; i++ )
	{
		const drawSurf_t* drawSurf = drawSurfs[i];
		const idMaterial* surfaceMaterial = drawSurf->material;
		
		// translucent surfaces don't put anything in the depth buffer and don't
		// test against it, which makes them fail the mirror clip plane operation
		if( surfaceMaterial->Coverage() == MC_TRANSLUCENT )
		{
			continue;
		}
		
		// get the expressions for conditionals / color / texcoords
		const float* surfaceRegs = drawSurf->shaderRegisters;
		
		// if all stages of a material have been conditioned off, don't do anything
		int stage = 0;
		for( ; stage < surfaceMaterial->GetNumStages(); stage++ )
		{
			const shaderStage_t* pStage = surfaceMaterial->GetStage( stage );
			// check the stage enable condition
			if( surfaceRegs[ pStage->conditionRegister ] != 0 )
			{
				break;
			}
		}
		if( stage == surfaceMaterial->GetNumStages() )
		{
			continue;
		}
		
		//bool isWorldModel = ( drawSurf->space->entityDef->parms.origin == vec3_origin );
		
		//if( isWorldModel )
		//{
		//	renderProgManager.BindShader_VertexLighting();
		//}
		//else
		{
#if 1
			if( fillGbuffer )
			{
				// fill geometry buffer with normal/roughness information
				if( drawSurf->jointCache )
				{
					renderProgManager.BindShader_SmallGeometryBufferSkinned();
				}
				else
				{
					renderProgManager.BindShader_SmallGeometryBuffer();
				}
			}
			else
#endif
			{
				// draw Quake 4 style ambient
				if( drawSurf->jointCache )
				{
					renderProgManager.BindShader_AmbientLightingSkinned();
				}
				else
				{
					renderProgManager.BindShader_AmbientLighting();
				}
			}
		}
		
		// change the matrix if needed
		if( drawSurf->space != currentSpace )
		{
			currentSpace = drawSurf->space;
			
			RB_SetMVP( drawSurf->space->mvp );
			
			// tranform the view origin into model local space
			idVec4 localViewOrigin( 1.0f );
			R_GlobalPointToLocal( drawSurf->space->modelMatrix, viewDef->renderView.vieworg, localViewOrigin.ToVec3() );
			SetVertexParm( RENDERPARM_LOCALVIEWORIGIN, localViewOrigin.ToFloatPtr() );
			
			//if( !isWorldModel )
			//{
			//	// tranform the light direction into model local space
			//	idVec3 globalLightDirection( 0.0f, 0.0f, -1.0f ); // HACK
			//	idVec4 localLightDirection( 0.0f );
			//	R_GlobalVectorToLocal( drawSurf->space->modelMatrix, globalLightDirection, localLightDirection.ToVec3() );
			//
			//	SetVertexParm( RENDERPARM_LOCALLIGHTORIGIN, localLightDirection.ToFloatPtr() );
			//}
			
			// RB: if we want to store the normals in world space so we need the model -> world matrix
			idRenderMatrix modelMatrix;
			idRenderMatrix::Transpose( *( idRenderMatrix* )drawSurf->space->modelMatrix, modelMatrix );
			
			SetVertexParms( RENDERPARM_MODELMATRIX_X, modelMatrix[0], 4 );
			
			// RB: if we want to store the normals in camera space so we need the model -> camera matrix
			float modelViewMatrixTranspose[16];
			R_MatrixTranspose( drawSurf->space->modelViewMatrix, modelViewMatrixTranspose );
			SetVertexParms( RENDERPARM_MODELVIEWMATRIX_X, modelViewMatrixTranspose, 4 );
		}
		
#if 0
		if( !isWorldModel )
		{
			idVec4 directedColor;
			directedColor.x = drawSurf->space->gridDirectedLight.x;
			directedColor.y = drawSurf->space->gridDirectedLight.y;
			directedColor.z = drawSurf->space->gridDirectedLight.z;
			directedColor.w = 1;
			
			idVec4 ambientColor;
			ambientColor.x = drawSurf->space->gridAmbientLight.x;
			ambientColor.y = drawSurf->space->gridAmbientLight.y;
			ambientColor.z = drawSurf->space->gridAmbientLight.z;
			ambientColor.w = 1;
			
			renderProgManager.SetRenderParm( RENDERPARM_COLOR, directedColor.ToFloatPtr() );
			renderProgManager.SetRenderParm( RENDERPARM_AMBIENT_COLOR, ambientColor.ToFloatPtr() );
		}
#endif
		
		/*
		uint64 surfGLState = 0;
		
		// set polygon offset if necessary
		if( surfaceMaterial->TestMaterialFlag( MF_POLYGONOFFSET ) )
		{
			surfGLState |= GLS_POLYGON_OFFSET;
			GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * surfaceMaterial->GetPolygonOffset() );
		}
		
		// subviews will just down-modulate the color buffer
		idVec4 color;
		if( surfaceMaterial->GetSort() == SS_SUBVIEW )
		{
			surfGLState |= GLS_SRCBLEND_DST_COLOR | GLS_DSTBLEND_ZERO | GLS_DEPTHFUNC_LESS;
			color[0] = 1.0f;
			color[1] = 1.0f;
			color[2] = 1.0f;
			color[3] = 1.0f;
		}
		else
		{
			// others just draw black
		#if 0
			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;
			color[3] = 1.0f;
		#else
			color = colorWhite;
		#endif
		}
		*/
		
		// check for the fast path
		if( surfaceMaterial->GetFastPathBumpImage() && !r_skipInteractionFastPath.GetBool() )
		{
			renderLog.OpenBlock( surfaceMaterial->GetName() );
			
			// texture 0 will be the per-surface bump map
			GL_SelectTexture( INTERACTION_TEXUNIT_BUMP );
			surfaceMaterial->GetFastPathBumpImage()->Bind();
			
			// texture 1 is the per-surface specular map
			GL_SelectTexture( INTERACTION_TEXUNIT_SPECULARMIX );
			surfaceMaterial->GetFastPathSpecularImage()->Bind();
			
			// texture 2 is the per-surface diffuse map
			GL_SelectTexture( INTERACTION_TEXUNIT_BASECOLOR );
			surfaceMaterial->GetFastPathDiffuseImage()->Bind();
			
			DrawElementsWithCounters( drawSurf );
			
			renderLog.CloseBlock();
			continue;
		}
		
		renderLog.OpenBlock( surfaceMaterial->GetName() );
		
		//bool drawSolid = false;
		
		
		// we may have multiple alpha tested stages
		// if the only alpha tested stages are condition register omitted,
		// draw a normal opaque surface
		//bool didDraw = false;
		
		drawInteraction_t inter = {};
		inter.surf = drawSurf;
		inter.bumpImage = NULL;
		inter.specularImage = NULL;
		inter.diffuseImage = NULL;
		
		inter.diffuseColor[0] = inter.diffuseColor[1] = inter.diffuseColor[2] = inter.diffuseColor[3] = 1;
		inter.specularColor[0] = inter.specularColor[1] = inter.specularColor[2] = inter.specularColor[3] = 0;
		
		// perforated surfaces may have multiple alpha tested stages
		for( stage = 0; stage < surfaceMaterial->GetNumStages(); stage++ )
		{
			const shaderStage_t* surfaceStage = surfaceMaterial->GetStage( stage );
			
			switch( surfaceStage->lighting )
			{
				case SL_COVERAGE:
				{
					// ignore any coverage stages since they should only be used for the depth fill pass
					// for diffuse stages that use alpha test.
					break;
				}
				
				case SL_AMBIENT:
				{
					// ignore ambient stages while drawing interactions
					break;
				}
				
				case SL_BUMP:
				{
					// ignore stage that fails the condition
					if( !surfaceRegs[ surfaceStage->conditionRegister ] )
					{
						break;
					}
					// draw any previous interaction
					if( inter.bumpImage != NULL )
					{
#if BLEND_NORMALS
						if (inter.vertexColor == SVC_IGNORE)
						{
							GL_State(GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL);
						}
						else
						{
							// RB: this is a bit hacky: use additive blending to blend the normals
							GL_State(GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL);
						}
#endif
						DrawSingleInteraction( &inter );
					}
					inter.bumpImage = surfaceStage->texture.image;
					inter.diffuseImage = NULL;
					inter.specularImage = NULL;
					SetupInteractionStage( surfaceStage, surfaceRegs, NULL,
										   inter.bumpMatrix, NULL );
					break;
				}
				
				case SL_DIFFUSE:
				{
					// ignore stage that fails the condition
					if( !surfaceRegs[ surfaceStage->conditionRegister ] )
					{
						break;
					}
					
					// draw any previous interaction
					if( inter.diffuseImage != NULL )
					{
#if BLEND_NORMALS
						if (inter.vertexColor == SVC_IGNORE)
						{
							GL_State(GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL);
						}
						else
						{
							// RB: this is a bit hacky: use additive blending to blend the normals
							GL_State(GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL);
						}
#endif
						DrawSingleInteraction( &inter );
					}
					
					inter.diffuseImage = surfaceStage->texture.image;
					inter.vertexColor = surfaceStage->vertexColor;
					SetupInteractionStage( surfaceStage, surfaceRegs, diffuseColor.ToFloatPtr(),
										   inter.diffuseMatrix, inter.diffuseColor.ToFloatPtr() );
					break;
				}
				
				case SL_SPECULAR:
				{
					// ignore stage that fails the condition
					if( !surfaceRegs[ surfaceStage->conditionRegister ] )
					{
						break;
					}
					// draw any previous interaction
					if( inter.specularImage != NULL )
					{
#if BLEND_NORMALS
						if (inter.vertexColor == SVC_IGNORE)
						{
							GL_State(GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL);
						}
						else
						{
							// RB: this is a bit hacky: use additive blending to blend the normals
							GL_State(GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL);
						}
#endif
						DrawSingleInteraction( &inter );
					}
					inter.specularImage = surfaceStage->texture.image;
					inter.vertexColor = surfaceStage->vertexColor;
					SetupInteractionStage( surfaceStage, surfaceRegs, specularColor.ToFloatPtr(),
										   inter.specularMatrix, inter.specularColor.ToFloatPtr() );
					break;
				}
			}
		}
		
		// draw the final interaction
#if BLEND_NORMALS
						if( inter.vertexColor == SVC_IGNORE )
						{
							GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL );
						}
						else
						{
							// RB: this is a bit hacky: use additive blending to blend the normals
							GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | GLS_DEPTHFUNC_EQUAL );
						}
#endif
		DrawSingleInteraction( &inter );
		
		renderLog.CloseBlock();
	}
	
	SetFragmentParm( RENDERPARM_ALPHA_TEST, vec4_zero.ToFloatPtr() );
	
	renderLog.CloseBlock();
	renderLog.CloseMainBlock();
	
	GL_SelectTexture( 0 );
	
	if( fillGbuffer )
	{
		if( hdrIsActive )
		{
			globalFramebuffers.hdrFBO->Bind();
		}
		else
		{
			Framebuffer::Unbind();
		}
	}
	
	renderProgManager.Unbind();
}

// RB end

/*
==============================================================================================

STENCIL SHADOW RENDERING

==============================================================================================
*/

/*
=====================
idRenderBackend::StencilShadowPass

The stencil buffer should have been set to 128 on any surfaces that might receive shadows.
=====================
*/
void idRenderBackend::StencilShadowPass( const drawSurf_t* drawSurfs, const viewLight_t* vLight )
{
	if( r_skipShadows.GetBool() )
	{
		return;
	}
	
	if( drawSurfs == NULL )
	{
		return;
	}
	
	RENDERLOG_PRINTF( "---------- RB_StencilShadowPass ----------\n" );
	
	renderProgManager.BindShader_Shadow();
	
	GL_SelectTexture( 0 );
	
	uint64 glState = 0;
	
	// for visualizing the shadows
	if( r_showShadows.GetInteger() )
	{
		// set the debug shadow color
		SetFragmentParm( RENDERPARM_COLOR, colorMagenta.ToFloatPtr() );
		if( r_showShadows.GetInteger() == 2 )
		{
			// draw filled in
			glState = GLS_DEPTHMASK | GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHFUNC_LESS;
		}
		else
		{
			// draw as lines, filling the depth buffer
			glState = GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_POLYMODE_LINE | GLS_DEPTHFUNC_ALWAYS;
		}
	}
	else
	{
		// don't write to the color or depth buffer, just the stencil buffer
		glState = GLS_DEPTHMASK | GLS_COLORMASK | GLS_ALPHAMASK | GLS_DEPTHFUNC_LESS;
	}
	
	GL_PolygonOffset( r_shadowPolygonFactor.GetFloat(), -r_shadowPolygonOffset.GetFloat() );
	
	// the actual stencil func will be set in the draw code, but we need to make sure it isn't
	// disabled here, and that the value will get reset for the interactions without looking
	// like a no-change-required
	
	// Two Sided Stencil reduces two draw calls to one for slightly faster shadows
	GL_State(
		glState |
		GLS_STENCIL_OP_FAIL_KEEP |
		GLS_STENCIL_OP_ZFAIL_KEEP |
		GLS_STENCIL_OP_PASS_INCR |
		GLS_STENCIL_MAKE_REF( STENCIL_SHADOW_TEST_VALUE ) |
		GLS_STENCIL_MAKE_MASK( STENCIL_SHADOW_MASK_VALUE ) |
		GLS_POLYGON_OFFSET |
		GLS_CULL_TWOSIDED );
		
	// process the chain of shadows with the current rendering state
	currentSpace = NULL;
	
	for( const drawSurf_t* drawSurf = drawSurfs; drawSurf != NULL; drawSurf = drawSurf->nextOnLight )
	{
		if( drawSurf->scissorRect.IsEmpty() )
		{
			continue;	// !@# FIXME: find out why this is sometimes being hit!
			// temporarily jump over the scissor and draw so the gl error callback doesn't get hit
		}
		
		// make sure the shadow volume is done
		if( drawSurf->shadowVolumeState != SHADOWVOLUME_DONE )
		{
			assert( drawSurf->shadowVolumeState == SHADOWVOLUME_UNFINISHED || drawSurf->shadowVolumeState == SHADOWVOLUME_DONE );
			
			uint64 start = Sys_Microseconds();
			while( drawSurf->shadowVolumeState == SHADOWVOLUME_UNFINISHED )
			{
				Sys_Yield();
			}
			uint64 end = Sys_Microseconds();
			
			pc.shadowMicroSec += end - start;
		}
		
		if( drawSurf->numIndexes == 0 )
		{
			continue;	// a job may have created an empty shadow volume
		}
		
		if( !currentScissor.Equals( drawSurf->scissorRect ) && r_useScissor.GetBool() )
		{
			// change the scissor
			GL_Scissor( viewDef->viewport.x1 + drawSurf->scissorRect.x1,
						viewDef->viewport.y1 + drawSurf->scissorRect.y1,
						drawSurf->scissorRect.x2 + 1 - drawSurf->scissorRect.x1,
						drawSurf->scissorRect.y2 + 1 - drawSurf->scissorRect.y1 );
						
			currentScissor = drawSurf->scissorRect;
		}
		
		if( drawSurf->space != currentSpace )
		{
			// change the matrix
			RB_SetMVP( drawSurf->space->mvp );
			
			// set the local light position to allow the vertex program to project the shadow volume end cap to infinity
			idVec4 localLight( 0.0f );
			R_GlobalPointToLocal( drawSurf->space->modelMatrix, vLight->globalLightOrigin, localLight.ToVec3() );
			SetVertexParm( RENDERPARM_LOCALLIGHTORIGIN, localLight.ToFloatPtr() );
			
			currentSpace = drawSurf->space;
		}
		
		if( r_showShadows.GetInteger() == 0 )
		{
			if( drawSurf->jointCache )
			{
				renderProgManager.BindShader_ShadowSkinned();
			}
			else
			{
				renderProgManager.BindShader_Shadow();
			}
		}
		else
		{
			if( drawSurf->jointCache )
			{
				renderProgManager.BindShader_ShadowDebugSkinned();
			}
			else
			{
				renderProgManager.BindShader_ShadowDebug();
			}
		}
		
		// set depth bounds per shadow
		if( r_useShadowDepthBounds.GetBool() )
		{
			GL_DepthBoundsTest( drawSurf->scissorRect.zmin, drawSurf->scissorRect.zmax );
		}
		
		// Determine whether or not the shadow volume needs to be rendered with Z-pass or
		// Z-fail. It is worthwhile to spend significant resources to reduce the number of
		// cases where shadow volumes need to be rendered with Z-fail because Z-fail
		// rendering can be significantly slower even on today's hardware. For instance,
		// on NVIDIA hardware Z-fail rendering causes the Z-Cull to be used in reverse:
		// Z-near becomes Z-far (trivial accept becomes trivial reject). Using the Z-Cull
		// in reverse is far less efficient because the Z-Cull only stores Z-near per 16x16
		// pixels while the Z-far is stored per 4x2 pixels. (The Z-near coallesce buffer
		// which has 4x4 granularity is only used when updating the depth which is not the
		// case for shadow volumes.) Note that it is also important to NOT use a Z-Cull
		// reconstruct because that would clear the Z-near of the Z-Cull which results in
		// no trivial rejection for Z-fail stencil shadow rendering.
		
		const bool renderZPass = ( drawSurf->renderZFail == 0 ) || r_forceZPassStencilShadows.GetBool();
		
		
#if !defined(USE_VULKAN)
		if( renderZPass )
		{
			// Z-pass
			glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR );
			glStencilOpSeparate( GL_BACK, GL_KEEP, GL_KEEP, GL_DECR );
		}
		else if( r_useStencilShadowPreload.GetBool() )
		{
			// preload + Z-pass
			glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_DECR, GL_DECR );
			glStencilOpSeparate( GL_BACK, GL_KEEP, GL_INCR, GL_INCR );
		}
		else
		{
			// Z-fail
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR, GL_KEEP);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR, GL_KEEP);
		}
#endif
		
		DrawStencilShadowPass( drawSurf, renderZPass );
	}
	
	// cleanup the shadow specific rendering state
	
	GL_State( glStateBits & ~( GLS_CULL_MASK ) | GLS_CULL_FRONTSIDED );
	
	// reset depth bounds
	if( r_useShadowDepthBounds.GetBool() )
	{
		if( r_useLightDepthBounds.GetBool() )
		{
			GL_DepthBoundsTest( vLight->scissorRect.zmin, vLight->scissorRect.zmax );
		}
		else
		{
			GL_DepthBoundsTest( 0.0f, 0.0f );
		}
	}
}

/*
==================
idRenderBackend::StencilSelectLight

Deform the zeroOneCubeModel to exactly cover the light volume. Render the deformed cube model to the stencil buffer in
such a way that only fragments that are directly visible and contained within the volume will be written creating a
mask to be used by the following stencil shadow and draw interaction passes.
==================
*/
void idRenderBackend::StencilSelectLight( const viewLight_t* vLight )
{
	renderLog.OpenBlock( "Stencil Select" );
	
	// enable the light scissor
	if( !currentScissor.Equals( vLight->scissorRect ) && r_useScissor.GetBool() )
	{
		GL_Scissor( viewDef->viewport.x1 + vLight->scissorRect.x1,
					viewDef->viewport.y1 + vLight->scissorRect.y1,
					vLight->scissorRect.x2 + 1 - vLight->scissorRect.x1,
					vLight->scissorRect.y2 + 1 - vLight->scissorRect.y1 );
					
		currentScissor = vLight->scissorRect;
	}
	
	// clear stencil buffer to 0 (not drawable)
	uint64 glStateMinusStencil = GL_GetCurrentStateMinusStencil();
	GL_State( glStateMinusStencil | GLS_STENCIL_FUNC_ALWAYS | GLS_STENCIL_MAKE_REF( STENCIL_SHADOW_TEST_VALUE ) | GLS_STENCIL_MAKE_MASK( STENCIL_SHADOW_MASK_VALUE ) );	// make sure stencil mask passes for the clear
	GL_Clear( false, false, true, 0, 0.0f, 0.0f, 0.0f, 0.0f, false );	// clear to 0 for stencil select
	
	// set the depthbounds
	GL_DepthBoundsTest( vLight->scissorRect.zmin, vLight->scissorRect.zmax );
	
	
	GL_State(
		GLS_COLORMASK |
		GLS_ALPHAMASK |
		GLS_CULL_TWOSIDED |
		GLS_DEPTHMASK |
		GLS_DEPTHFUNC_LESS |
		GLS_STENCIL_FUNC_ALWAYS |
		GLS_STENCIL_MAKE_REF( STENCIL_SHADOW_TEST_VALUE ) |
		GLS_STENCIL_MAKE_MASK( STENCIL_SHADOW_MASK_VALUE ) );
		
	renderProgManager.BindShader_Depth();
	
	// set the matrix for deforming the 'zeroOneCubeModel' into the frustum to exactly cover the light volume
	idRenderMatrix invProjectMVPMatrix;
	idRenderMatrix::Multiply( viewDef->worldSpace.mvp, vLight->inverseBaseLightProject, invProjectMVPMatrix );
	RB_SetMVP( invProjectMVPMatrix );
	
#if !defined(USE_VULKAN)
	// two-sided stencil test
	glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_REPLACE, GL_ZERO );
	glStencilOpSeparate( GL_BACK, GL_KEEP, GL_ZERO, GL_REPLACE );
#endif
	
	DrawElementsWithCounters( &zeroOneCubeSurface );
	
	// reset stencil state
	GL_State( glStateBits & ~( GLS_CULL_MASK ) | GLS_CULL_FRONTSIDED );
	
	renderProgManager.Unbind();
	
	
	// unset the depthbounds
	GL_DepthBoundsTest( 0.0f, 0.0f );
	
	renderLog.CloseBlock();
}

/*
==============================================================================================

SHADOW MAPS RENDERING

==============================================================================================
*/

/*
same as D3DXMatrixOrthoOffCenterRH

http://msdn.microsoft.com/en-us/library/bb205348(VS.85).aspx
*/
static void MatrixOrthogonalProjectionRH( float m[16], float left, float right, float bottom, float top, float zNear, float zFar )
{
	m[0] = 2 / ( right - left );
	m[4] = 0;
	m[8] = 0;
	m[12] = ( left + right ) / ( left - right );
	m[1] = 0;
	m[5] = 2 / ( top - bottom );
	m[9] = 0;
	m[13] = ( top + bottom ) / ( bottom - top );
	m[2] = 0;
	m[6] = 0;
	m[10] = 1 / ( zNear - zFar );
	m[14] = zNear / ( zNear - zFar );
	m[3] = 0;
	m[7] = 0;
	m[11] = 0;
	m[15] = 1;
}

void MatrixCrop( float m[16], const idVec3 mins, const idVec3 maxs )
{
	float			scaleX, scaleY, scaleZ;
	float			offsetX, offsetY, offsetZ;
	
	scaleX = 2.0f / ( maxs[0] - mins[0] );
	scaleY = 2.0f / ( maxs[1] - mins[1] );
	
	offsetX = -0.5f * ( maxs[0] + mins[0] ) * scaleX;
	offsetY = -0.5f * ( maxs[1] + mins[1] ) * scaleY;
	
	scaleZ = 1.0f / ( maxs[2] - mins[2] );
	offsetZ = -mins[2] * scaleZ;
	
	m[ 0] = scaleX;
	m[ 4] = 0;
	m[ 8] = 0;
	m[12] = offsetX;
	m[ 1] = 0;
	m[ 5] = scaleY;
	m[ 9] = 0;
	m[13] = offsetY;
	m[ 2] = 0;
	m[ 6] = 0;
	m[10] = scaleZ;
	m[14] = offsetZ;
	m[ 3] = 0;
	m[ 7] = 0;
	m[11] = 0;
	m[15] = 1;
}

void MatrixLookAtRH( float m[16], const idVec3& eye, const idVec3& dir, const idVec3& up )
{
	idVec3 dirN;
	idVec3 upN;
	idVec3 sideN;
	
	sideN = dir.Cross( up );
	sideN.Normalize();
	
	upN = sideN.Cross( dir );
	upN.Normalize();
	
	dirN = dir;
	dirN.Normalize();
	
	m[ 0] = sideN[0];
	m[ 4] = sideN[1];
	m[ 8] = sideN[2];
	m[12] = -( sideN * eye );
	m[ 1] = upN[0];
	m[ 5] = upN[1];
	m[ 9] = upN[2];
	m[13] = -( upN * eye );
	m[ 2] = -dirN[0];
	m[ 6] = -dirN[1];
	m[10] = -dirN[2];
	m[14] = ( dirN * eye );
	m[ 3] = 0;
	m[ 7] = 0;
	m[11] = 0;
	m[15] = 1;
}

/*
=====================
idRenderBackend::ShadowMapPass
=====================
*/
void idRenderBackend::ShadowMapPass( const drawSurf_t* drawSurfs, const viewLight_t* vLight, int side )
{
	if( r_skipShadows.GetBool() )
	{
		return;
	}
	
	if( drawSurfs == NULL )
	{
		return;
	}
	
	RENDERLOG_PRINTF( "---------- RB_ShadowMapPass( side = %i ) ----------\n", side );
	
	renderProgManager.BindShader_Depth();
	
	GL_SelectTexture( 0 );
	
	uint64 glState = 0;
	
	// the actual stencil func will be set in the draw code, but we need to make sure it isn't
	// disabled here, and that the value will get reset for the interactions without looking
	// like a no-change-required
	GL_State( glState | GLS_POLYGON_OFFSET );
	
	switch( r_shadowMapOccluderFacing.GetInteger() )
	{
		case 0:
			GL_State( glStateBits & ~( GLS_CULL_MASK ) | GLS_CULL_FRONTSIDED );
			GL_PolygonOffset( r_shadowMapPolygonFactor.GetFloat(), r_shadowMapPolygonOffset.GetFloat() );
			break;
			
		case 1:
			GL_State( glStateBits & ~( GLS_CULL_MASK ) | GLS_CULL_BACKSIDED );
			GL_PolygonOffset( -r_shadowMapPolygonFactor.GetFloat(), -r_shadowMapPolygonOffset.GetFloat() );
			break;
			
		default:
			GL_State( glStateBits & ~( GLS_CULL_MASK ) | GLS_CULL_TWOSIDED );
			GL_PolygonOffset( r_shadowMapPolygonFactor.GetFloat(), r_shadowMapPolygonOffset.GetFloat() );
			break;
	}
	
	idRenderMatrix lightProjectionRenderMatrix;
	idRenderMatrix lightViewRenderMatrix;
	
	
	if( vLight->parallel && side >= 0 )
	{
		assert( side >= 0 && side < 6 );
		
		// original light direction is from surface to light origin
		idVec3 lightDir = -vLight->lightCenter;
		if( lightDir.Normalize() == 0.0f )
		{
			lightDir[2] = -1.0f;
		}
		
		idMat3 rotation = lightDir.ToMat3();
		//idAngles angles = lightDir.ToAngles();
		//idMat3 rotation = angles.ToMat3();
		
		const idVec3 viewDir = viewDef->renderView.viewaxis[0];
		const idVec3 viewPos = viewDef->renderView.vieworg;
		
#if 1
		idRenderMatrix::CreateViewMatrix( viewDef->renderView.vieworg, rotation, lightViewRenderMatrix );
#else
		float lightViewMatrix[16];
		MatrixLookAtRH( lightViewMatrix, viewPos, lightDir, viewDir );
		idRenderMatrix::Transpose( *( idRenderMatrix* )lightViewMatrix, lightViewRenderMatrix );
#endif
		
		idBounds lightBounds;
		lightBounds.Clear();
		
		ALIGNTYPE16 frustumCorners_t corners;
		idRenderMatrix::GetFrustumCorners( corners, vLight->inverseBaseLightProject, bounds_zeroOneCube );
		
		idVec4 point, transf;
		for( int j = 0; j < 8; j++ )
		{
			point[0] = corners.x[j];
			point[1] = corners.y[j];
			point[2] = corners.z[j];
			point[3] = 1;
			
			lightViewRenderMatrix.TransformPoint( point, transf );
			transf[0] /= transf[3];
			transf[1] /= transf[3];
			transf[2] /= transf[3];
			
			lightBounds.AddPoint( transf.ToVec3() );
		}
		
		float lightProjectionMatrix[16];
		MatrixOrthogonalProjectionRH( lightProjectionMatrix, lightBounds[0][0], lightBounds[1][0], lightBounds[0][1], lightBounds[1][1], -lightBounds[1][2], -lightBounds[0][2] );
		idRenderMatrix::Transpose( *( idRenderMatrix* )lightProjectionMatrix, lightProjectionRenderMatrix );
		
		
		// 	'frustumMVP' goes from global space -> camera local space -> camera projective space
		// invert the MVP projection so we can deform zero-to-one cubes into the frustum pyramid shape and calculate global bounds
		
		idRenderMatrix splitFrustumInverse;
		if( !idRenderMatrix::Inverse( viewDef->frustumMVPs[FRUSTUM_CASCADE1 + side], splitFrustumInverse ) )
		{
			idLib::Warning( "splitFrustumMVP invert failed" );
		}
		
		// splitFrustumCorners in global space
		ALIGNTYPE16 frustumCorners_t splitFrustumCorners;
		idRenderMatrix::GetFrustumCorners( splitFrustumCorners, splitFrustumInverse, bounds_unitCube );
		
#if 0
		idBounds splitFrustumBounds;
		splitFrustumBounds.Clear();
		for( int j = 0; j < 8; j++ )
		{
			point[0] = splitFrustumCorners.x[j];
			point[1] = splitFrustumCorners.y[j];
			point[2] = splitFrustumCorners.z[j];
			
			splitFrustumBounds.AddPoint( point.ToVec3() );
		}
		
		idVec3 center = splitFrustumBounds.GetCenter();
		float radius = splitFrustumBounds.GetRadius( center );
		
		//ALIGNTYPE16 frustumCorners_t splitFrustumCorners;
		splitFrustumBounds[0] = idVec3( -radius, -radius, -radius );
		splitFrustumBounds[1] = idVec3( radius, radius, radius );
		splitFrustumBounds.TranslateSelf( viewPos );
		idVec3 splitFrustumCorners2[8];
		splitFrustumBounds.ToPoints( splitFrustumCorners2 );
		
		for( int j = 0; j < 8; j++ )
		{
			splitFrustumCorners.x[j] = splitFrustumCorners2[j].x;
			splitFrustumCorners.y[j] = splitFrustumCorners2[j].y;
			splitFrustumCorners.z[j] = splitFrustumCorners2[j].z;
		}
#endif
		
		
		idRenderMatrix lightViewProjectionRenderMatrix;
		idRenderMatrix::Multiply( lightProjectionRenderMatrix, lightViewRenderMatrix, lightViewProjectionRenderMatrix );
		
		// find the bounding box of the current split in the light's clip space
		idBounds cropBounds;
		cropBounds.Clear();
		for( int j = 0; j < 8; j++ )
		{
			point[0] = splitFrustumCorners.x[j];
			point[1] = splitFrustumCorners.y[j];
			point[2] = splitFrustumCorners.z[j];
			point[3] = 1;
			
			lightViewRenderMatrix.TransformPoint( point, transf );
			transf[0] /= transf[3];
			transf[1] /= transf[3];
			transf[2] /= transf[3];
			
			cropBounds.AddPoint( transf.ToVec3() );
		}
		
		// don't let the frustum AABB be bigger than the light AABB
		if( cropBounds[0][0] < lightBounds[0][0] )
		{
			cropBounds[0][0] = lightBounds[0][0];
		}
		
		if( cropBounds[0][1] < lightBounds[0][1] )
		{
			cropBounds[0][1] = lightBounds[0][1];
		}
		
		if( cropBounds[1][0] > lightBounds[1][0] )
		{
			cropBounds[1][0] = lightBounds[1][0];
		}
		
		if( cropBounds[1][1] > lightBounds[1][1] )
		{
			cropBounds[1][1] = lightBounds[1][1];
		}
		
		cropBounds[0][2] = lightBounds[0][2];
		cropBounds[1][2] = lightBounds[1][2];
		
		//float cropMatrix[16];
		//MatrixCrop(cropMatrix, cropBounds[0], cropBounds[1]);
		
		//idRenderMatrix cropRenderMatrix;
		//idRenderMatrix::Transpose( *( idRenderMatrix* )cropMatrix, cropRenderMatrix );
		
		//idRenderMatrix tmp = lightProjectionRenderMatrix;
		//idRenderMatrix::Multiply( cropRenderMatrix, tmp, lightProjectionRenderMatrix );
		
		MatrixOrthogonalProjectionRH( lightProjectionMatrix, cropBounds[0][0], cropBounds[1][0], cropBounds[0][1], cropBounds[1][1], -cropBounds[1][2], -cropBounds[0][2] );
		idRenderMatrix::Transpose( *( idRenderMatrix* )lightProjectionMatrix, lightProjectionRenderMatrix );
		
		shadowV[side] = lightViewRenderMatrix;
		shadowP[side] = lightProjectionRenderMatrix;
	}
	else if( vLight->pointLight && side >= 0 )
	{
		assert( side >= 0 && side < 6 );
		
		// FIXME OPTIMIZE no memset
		
		float	viewMatrix[16];
		
		idVec3	vec;
		idVec3	origin = vLight->globalLightOrigin;
		
		// side of a point light
		memset( viewMatrix, 0, sizeof( viewMatrix ) );
		switch( side )
		{
			case 0:
				viewMatrix[0] = 1;
				viewMatrix[9] = 1;
				viewMatrix[6] = -1;
				break;
			case 1:
				viewMatrix[0] = -1;
				viewMatrix[9] = -1;
				viewMatrix[6] = -1;
				break;
			case 2:
				viewMatrix[4] = 1;
				viewMatrix[1] = -1;
				viewMatrix[10] = 1;
				break;
			case 3:
				viewMatrix[4] = -1;
				viewMatrix[1] = -1;
				viewMatrix[10] = -1;
				break;
			case 4:
				viewMatrix[8] = 1;
				viewMatrix[1] = -1;
				viewMatrix[6] = -1;
				break;
			case 5:
				viewMatrix[8] = -1;
				viewMatrix[1] = 1;
				viewMatrix[6] = -1;
				break;
		}
		
		viewMatrix[12] = -origin[0] * viewMatrix[0] + -origin[1] * viewMatrix[4] + -origin[2] * viewMatrix[8];
		viewMatrix[13] = -origin[0] * viewMatrix[1] + -origin[1] * viewMatrix[5] + -origin[2] * viewMatrix[9];
		viewMatrix[14] = -origin[0] * viewMatrix[2] + -origin[1] * viewMatrix[6] + -origin[2] * viewMatrix[10];
		
		viewMatrix[3] = 0;
		viewMatrix[7] = 0;
		viewMatrix[11] = 0;
		viewMatrix[15] = 1;
		
		// from world space to light origin, looking down the X axis
		float	unflippedLightViewMatrix[16];
		
		// from world space to OpenGL view space, looking down the negative Z axis
		float	lightViewMatrix[16];
		
		static float	s_flipMatrix[16] =
		{
			// convert from our coordinate system (looking down X)
			// to OpenGL's coordinate system (looking down -Z)
			0, 0, -1, 0,
			-1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 1
		};
		
		memcpy( unflippedLightViewMatrix, viewMatrix, sizeof( unflippedLightViewMatrix ) );
		R_MatrixMultiply( viewMatrix, s_flipMatrix, lightViewMatrix );
		
		idRenderMatrix::Transpose( *( idRenderMatrix* )lightViewMatrix, lightViewRenderMatrix );
		
		// set up 90 degree projection matrix
		const float zNear = 4;
		const float	fov = r_shadowMapFrustumFOV.GetFloat();
		
		float ymax = zNear * tan( fov * idMath::PI / 360.0f );
		float ymin = -ymax;
		
		float xmax = zNear * tan( fov * idMath::PI / 360.0f );
		float xmin = -xmax;
		
		const float width = xmax - xmin;
		const float height = ymax - ymin;
		
		// from OpenGL view space to OpenGL NDC ( -1 : 1 in XYZ )
		float lightProjectionMatrix[16];
		
		lightProjectionMatrix[0 * 4 + 0] = 2.0f * zNear / width;
		lightProjectionMatrix[1 * 4 + 0] = 0.0f;
		lightProjectionMatrix[2 * 4 + 0] = ( xmax + xmin ) / width;	// normally 0
		lightProjectionMatrix[3 * 4 + 0] = 0.0f;
		
		lightProjectionMatrix[0 * 4 + 1] = 0.0f;
		lightProjectionMatrix[1 * 4 + 1] = 2.0f * zNear / height;
		lightProjectionMatrix[2 * 4 + 1] = ( ymax + ymin ) / height;	// normally 0
		lightProjectionMatrix[3 * 4 + 1] = 0.0f;
		
		// this is the far-plane-at-infinity formulation, and
		// crunches the Z range slightly so w=0 vertexes do not
		// rasterize right at the wraparound point
		lightProjectionMatrix[0 * 4 + 2] = 0.0f;
		lightProjectionMatrix[1 * 4 + 2] = 0.0f;
		lightProjectionMatrix[2 * 4 + 2] = -0.999f; // adjust value to prevent imprecision issues
		lightProjectionMatrix[3 * 4 + 2] = -2.0f * zNear;
		
		lightProjectionMatrix[0 * 4 + 3] = 0.0f;
		lightProjectionMatrix[1 * 4 + 3] = 0.0f;
		lightProjectionMatrix[2 * 4 + 3] = -1.0f;
		lightProjectionMatrix[3 * 4 + 3] = 0.0f;
		
		idRenderMatrix::Transpose( *( idRenderMatrix* )lightProjectionMatrix, lightProjectionRenderMatrix );
		
		shadowV[side] = lightViewRenderMatrix;
		shadowP[side] = lightProjectionRenderMatrix;
	}
	else
	{
		lightViewRenderMatrix.Identity();
		lightProjectionRenderMatrix = vLight->baseLightProject;
		
		shadowV[0] = lightViewRenderMatrix;
		shadowP[0] = lightProjectionRenderMatrix;
	}
	
	
	// FIXME
#if !defined(USE_VULKAN)
	globalFramebuffers.shadowFBO[vLight->shadowLOD]->Bind();
	
	if( side < 0 )
	{
		globalFramebuffers.shadowFBO[vLight->shadowLOD]->AttachImageDepthLayer( globalImages->shadowImage[vLight->shadowLOD], 0 );
	}
	else
	{
		globalFramebuffers.shadowFBO[vLight->shadowLOD]->AttachImageDepthLayer( globalImages->shadowImage[vLight->shadowLOD], side );
	}
	
	globalFramebuffers.shadowFBO[vLight->shadowLOD]->Check();
	
	GL_ViewportAndScissor( 0, 0, shadowMapResolutions[vLight->shadowLOD], shadowMapResolutions[vLight->shadowLOD] );
	
	
	glClear( GL_DEPTH_BUFFER_BIT );
#endif
	
	// process the chain of shadows with the current rendering state
	currentSpace = NULL;
	
	for( const drawSurf_t* drawSurf = drawSurfs; drawSurf != NULL; drawSurf = drawSurf->nextOnLight )
	{
	
#if 1
		// make sure the shadow occluder geometry is done
		if( drawSurf->shadowVolumeState != SHADOWVOLUME_DONE )
		{
			assert( drawSurf->shadowVolumeState == SHADOWVOLUME_UNFINISHED || drawSurf->shadowVolumeState == SHADOWVOLUME_DONE );
			
			uint64 start = Sys_Microseconds();
			while( drawSurf->shadowVolumeState == SHADOWVOLUME_UNFINISHED )
			{
				Sys_Yield();
			}
			uint64 end = Sys_Microseconds();
			
			pc.shadowMicroSec += end - start;
		}
#endif
		
		if( drawSurf->numIndexes == 0 )
		{
			continue;	// a job may have created an empty shadow geometry
		}
		
		if( drawSurf->space != currentSpace )
		{
			idRenderMatrix modelRenderMatrix;
			idRenderMatrix::Transpose( *( idRenderMatrix* )drawSurf->space->modelMatrix, modelRenderMatrix );
			
			idRenderMatrix modelToLightRenderMatrix;
			idRenderMatrix::Multiply( lightViewRenderMatrix, modelRenderMatrix, modelToLightRenderMatrix );
			
			idRenderMatrix clipMVP;
			idRenderMatrix::Multiply( lightProjectionRenderMatrix, modelToLightRenderMatrix, clipMVP );
			
			if( vLight->parallel )
			{
				idRenderMatrix MVP;
				idRenderMatrix::Multiply( renderMatrix_clipSpaceToWindowSpace, clipMVP, MVP );
				
				RB_SetMVP( clipMVP );
			}
			else if( side < 0 )
			{
				// from OpenGL view space to OpenGL NDC ( -1 : 1 in XYZ )
				idRenderMatrix MVP;
				idRenderMatrix::Multiply( renderMatrix_windowSpaceToClipSpace, clipMVP, MVP );
				
				RB_SetMVP( MVP );
			}
			else
			{
				RB_SetMVP( clipMVP );
			}
			
			// set the local light position to allow the vertex program to project the shadow volume end cap to infinity
			/*
			idVec4 localLight( 0.0f );
			R_GlobalPointToLocal( drawSurf->space->modelMatrix, vLight->globalLightOrigin, localLight.ToVec3() );
			SetVertexParm( RENDERPARM_LOCALLIGHTORIGIN, localLight.ToFloatPtr() );
			*/
			
			currentSpace = drawSurf->space;
		}
		
		bool didDraw = false;
		
		const idMaterial* shader = drawSurf->material;
		
		// get the expressions for conditionals / color / texcoords
		const float* regs = drawSurf->shaderRegisters;
		idVec4 color( 0, 0, 0, 1 );
		
		uint64 surfGLState = 0;
		
		// set polygon offset if necessary
		if( shader && shader->TestMaterialFlag( MF_POLYGONOFFSET ) )
		{
			surfGLState |= GLS_POLYGON_OFFSET;
			GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * shader->GetPolygonOffset() );
		}
		
#if 1
		if( shader && shader->Coverage() == MC_PERFORATED )
		{
			// perforated surfaces may have multiple alpha tested stages
			for( int stage = 0; stage < shader->GetNumStages(); stage++ )
			{
				const shaderStage_t* pStage = shader->GetStage( stage );
				
				if( !pStage->hasAlphaTest )
				{
					continue;
				}
				
				// check the stage enable condition
				if( regs[ pStage->conditionRegister ] == 0 )
				{
					continue;
				}
				
				// if we at least tried to draw an alpha tested stage,
				// we won't draw the opaque surface
				didDraw = true;
				
				// set the alpha modulate
				color[3] = regs[ pStage->color.registers[3] ];
				
				// skip the entire stage if alpha would be black
				if( color[3] <= 0.0f )
				{
					continue;
				}
				
				uint64 stageGLState = surfGLState;
				
				// set privatePolygonOffset if necessary
				if( pStage->privatePolygonOffset )
				{
					GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * pStage->privatePolygonOffset );
					stageGLState |= GLS_POLYGON_OFFSET;
				}
				
				GL_Color( color );
				
				GL_State( stageGLState );
				idVec4 alphaTestValue( regs[ pStage->alphaTestRegister ] );
				SetFragmentParm( RENDERPARM_ALPHA_TEST, alphaTestValue.ToFloatPtr() );
				
				if( drawSurf->jointCache )
				{
					renderProgManager.BindShader_TextureVertexColorSkinned();
				}
				else
				{
					renderProgManager.BindShader_TextureVertexColor();
				}
				
				RB_SetVertexColorParms( SVC_IGNORE );
				
				// bind the texture
				GL_SelectTexture( 0 );
				pStage->texture.image->Bind();
				
				// set texture matrix and texGens
				PrepareStageTexturing( pStage, drawSurf );
				
				// must render with less-equal for Z-Cull to work properly
				assert( ( GL_GetCurrentState() & GLS_DEPTHFUNC_BITS ) == GLS_DEPTHFUNC_LESS );
				
				// draw it
				DrawElementsWithCounters( drawSurf );
				
				// clean up
				FinishStageTexturing( pStage, drawSurf );
				
				// unset privatePolygonOffset if necessary
				if( pStage->privatePolygonOffset )
				{
					GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * shader->GetPolygonOffset() );
				}
			}
		}
#endif
		
		if( !didDraw )
		{
			if( drawSurf->jointCache )
			{
				renderProgManager.BindShader_DepthSkinned();
			}
			else
			{
				renderProgManager.BindShader_Depth();
			}
			
			DrawElementsWithCounters( drawSurf );
		}
	}
	
	// cleanup the shadow specific rendering state
	if( r_useHDR.GetBool() /*&& !viewDef->is2Dgui*/ )
	{
		globalFramebuffers.hdrFBO->Bind();
	}
	else
	{
		Framebuffer::Unbind();
	}
	renderProgManager.Unbind();
	
	GL_State( GLS_DEFAULT );
	
	SetFragmentParm( RENDERPARM_ALPHA_TEST, vec4_zero.ToFloatPtr() );
}

/*
==============================================================================================

DRAW INTERACTIONS

==============================================================================================
*/
/*
==================
idRenderBackend::DrawInteractions
==================
*/
void idRenderBackend::DrawInteractions( const viewDef_t* _viewDef )
{
	if( r_skipInteractions.GetBool() )
	{
		return;
	}
	
	renderLog.OpenMainBlock( MRB_DRAW_INTERACTIONS );
	renderLog.OpenBlock( "RB_DrawInteractions" );
	
	GL_SelectTexture( 0 );
	
	
	const bool useLightDepthBounds = r_useLightDepthBounds.GetBool() && !r_useShadowMapping.GetBool();
	
	//
	// for each light, perform shadowing and adding
	//
	for( const viewLight_t* vLight = viewDef->viewLights; vLight != NULL; vLight = vLight->next )
	{
		// do fogging later
		if( vLight->lightShader->IsFogLight() )
		{
			continue;
		}
		if( vLight->lightShader->IsBlendLight() )
		{
			continue;
		}
		
		if( vLight->localInteractions == NULL && vLight->globalInteractions == NULL && vLight->translucentInteractions == NULL )
		{
			continue;
		}
		
		const idMaterial* lightShader = vLight->lightShader;
		renderLog.OpenBlock( lightShader->GetName() );
		
		// set the depth bounds for the whole light
		if( useLightDepthBounds )
		{
			GL_DepthBoundsTest( vLight->scissorRect.zmin, vLight->scissorRect.zmax );
		}
		
		// RB: shadow mapping
		if( r_useShadowMapping.GetBool() )
		{
			int	side, sideStop;
			
			if( vLight->parallel )
			{
				side = 0;
				sideStop = r_shadowMapSplits.GetInteger() + 1;
			}
			else if( vLight->pointLight )
			{
				if( r_shadowMapSingleSide.GetInteger() != -1 )
				{
					side = r_shadowMapSingleSide.GetInteger();
					sideStop = side + 1;
				}
				else
				{
					side = 0;
					sideStop = 6;
				}
			}
			else
			{
				side = -1;
				sideStop = 0;
			}
			
			for( ; side < sideStop ; side++ )
			{
				ShadowMapPass( vLight->globalShadows, vLight, side );
			}
			
			// go back from light view to default camera view
			ResetViewportAndScissorToDefaultCamera( _viewDef );
			
			if( vLight->localInteractions != NULL )
			{
				renderLog.OpenBlock( "Local Light Interactions" );
				RenderInteractions( vLight->localInteractions, vLight, GLS_DEPTHFUNC_EQUAL, false, useLightDepthBounds );
				renderLog.CloseBlock();
			}
			
			if( vLight->globalInteractions != NULL )
			{
				renderLog.OpenBlock( "Global Light Interactions" );
				RenderInteractions( vLight->globalInteractions, vLight, GLS_DEPTHFUNC_EQUAL, false, useLightDepthBounds );
				renderLog.CloseBlock();
			}
		}
		else
		{
			// only need to clear the stencil buffer and perform stencil testing if there are shadows
			const bool performStencilTest = ( vLight->globalShadows != NULL || vLight->localShadows != NULL ) && !r_useShadowMapping.GetBool();
			
			// mirror flips the sense of the stencil select, and I don't want to risk accidentally breaking it
			// in the normal case, so simply disable the stencil select in the mirror case
			const bool useLightStencilSelect = ( r_useLightStencilSelect.GetBool() && viewDef->isMirror == false );
			
			if( performStencilTest )
			{
				if( useLightStencilSelect )
				{
					// write a stencil mask for the visible light bounds to hi-stencil
					StencilSelectLight( vLight );
				}
				else
				{
					// always clear whole S-Cull tiles
					idScreenRect rect;
					rect.x1 = ( vLight->scissorRect.x1 +  0 ) & ~15;
					rect.y1 = ( vLight->scissorRect.y1 +  0 ) & ~15;
					rect.x2 = ( vLight->scissorRect.x2 + 15 ) & ~15;
					rect.y2 = ( vLight->scissorRect.y2 + 15 ) & ~15;
					
					if( !currentScissor.Equals( rect ) && r_useScissor.GetBool() )
					{
						GL_Scissor( viewDef->viewport.x1 + rect.x1,
									viewDef->viewport.y1 + rect.y1,
									rect.x2 + 1 - rect.x1,
									rect.y2 + 1 - rect.y1 );
									
						currentScissor = rect;
					}
					GL_State( GLS_DEFAULT );	// make sure stencil mask passes for the clear
					GL_Clear( false, false, true, STENCIL_SHADOW_TEST_VALUE, 0.0f, 0.0f, 0.0f, 0.0f, false );
				}
			}
			
			if( vLight->globalShadows != NULL )
			{
				renderLog.OpenBlock( "Global Light Shadows" );
				StencilShadowPass( vLight->globalShadows, vLight );
				renderLog.CloseBlock();
			}
			
			if( vLight->localInteractions != NULL )
			{
				renderLog.OpenBlock( "Local Light Interactions" );
				RenderInteractions( vLight->localInteractions, vLight, GLS_DEPTHFUNC_EQUAL, performStencilTest, useLightDepthBounds );
				renderLog.CloseBlock();
			}
			
			if( vLight->localShadows != NULL )
			{
				renderLog.OpenBlock( "Local Light Shadows" );
				StencilShadowPass( vLight->localShadows, vLight );
				renderLog.CloseBlock();
			}
			
			if( vLight->globalInteractions != NULL )
			{
				renderLog.OpenBlock( "Global Light Interactions" );
				RenderInteractions( vLight->globalInteractions, vLight, GLS_DEPTHFUNC_EQUAL, performStencilTest, useLightDepthBounds );
				renderLog.CloseBlock();
			}
		}
		// RB end
		
		if( vLight->translucentInteractions != NULL && !r_skipTranslucent.GetBool() )
		{
			renderLog.OpenBlock( "Translucent Interactions" );
			
			// Disable the depth bounds test because translucent surfaces don't work with
			// the depth bounds tests since they did not write depth during the depth pass.
			if( useLightDepthBounds )
			{
				GL_DepthBoundsTest( 0.0f, 0.0f );
			}
			
			// The depth buffer wasn't filled in for translucent surfaces, so they
			// can never be constrained to perforated surfaces with the depthfunc equal.
			
			// Translucent surfaces do not receive shadows. This is a case where a
			// shadow buffer solution would work but stencil shadows do not because
			// stencil shadows only affect surfaces that contribute to the view depth
			// buffer and translucent surfaces do not contribute to the view depth buffer.
			
			RenderInteractions( vLight->translucentInteractions, vLight, GLS_DEPTHFUNC_LESS, false, false );
			
			renderLog.CloseBlock();
		}
		
		renderLog.CloseBlock();
	}
	
	// disable stencil shadow test
	GL_State( GLS_DEFAULT );
	
	// unbind texture units
	GL_SelectTexture( 0 );
	
	// reset depth bounds
	if( useLightDepthBounds )
	{
		GL_DepthBoundsTest( 0.0f, 0.0f );
	}
	
	renderLog.CloseBlock();
	renderLog.CloseMainBlock();
}

/*
=============================================================================================

NON-INTERACTION SHADER PASSES

=============================================================================================
*/

/*
=====================
idRenderBackend::DrawShaderPasses

Draw non-light dependent passes

If we are rendering Guis, the drawSurf_t::sort value is a depth offset that can
be multiplied by guiEye for polarity and screenSeparation for scale.
=====================
*/
int idRenderBackend::DrawShaderPasses( const drawSurf_t* const* const drawSurfs, const int numDrawSurfs,
									   const float guiStereoScreenOffset, const int stereoEye )
{
	// only obey skipAmbient if we are rendering a view
	if( viewDef->viewEntitys && r_skipAmbient.GetBool() )
	{
		return numDrawSurfs;
	}
	
	renderLog.OpenBlock( "RB_DrawShaderPasses" );
	
	GL_SelectTexture( 0 );
	
	currentSpace = ( const viewEntity_t* )1;	// using NULL makes /analyze think surf->space needs to be checked...
	float currentGuiStereoOffset = 0.0f;
	
	int i = 0;
	for( ; i < numDrawSurfs; i++ )
	{
		const drawSurf_t* surf = drawSurfs[i];
		const idMaterial* shader = surf->material;
		
		if( !shader->HasAmbient() )
		{
			continue;
		}
		
		if( shader->IsPortalSky() )
		{
			continue;
		}
		
		// some deforms may disable themselves by setting numIndexes = 0
		if( surf->numIndexes == 0 )
		{
			continue;
		}
		
		if( shader->SuppressInSubview() )
		{
			continue;
		}
		
		if( viewDef->isXraySubview && surf->space->entityDef )
		{
			if( surf->space->entityDef->parms.xrayIndex != 2 )
			{
				continue;
			}
		}
		
		// we need to draw the post process shaders after we have drawn the fog lights
		if( shader->GetSort() >= SS_POST_PROCESS && !currentRenderCopied )
		{
			break;
		}
		
		// if we are rendering a 3D view and the surface's eye index doesn't match
		// the current view's eye index then we skip the surface
		// if the stereoEye value of a surface is 0 then we need to draw it for both eyes.
		const int shaderStereoEye = shader->GetStereoEye();
		const bool isEyeValid = game->GetCVarBool("stereoRender_swapEyes") ? ( shaderStereoEye == stereoEye ) : ( shaderStereoEye != stereoEye );
		if( ( stereoEye != 0 ) && ( shaderStereoEye != 0 ) && ( isEyeValid ) )
		{
			continue;
		}
		
		renderLog.OpenBlock( shader->GetName() );
		
		// determine the stereoDepth offset
		// guiStereoScreenOffset will always be zero for 3D views, so the !=
		// check will never force an update due to the current sort value.
		const float thisGuiStereoOffset = guiStereoScreenOffset * surf->sort;
		
		// change the matrix and other space related vars if needed
		if( surf->space != currentSpace || thisGuiStereoOffset != currentGuiStereoOffset )
		{
			currentSpace = surf->space;
			currentGuiStereoOffset = thisGuiStereoOffset;
			
			const viewEntity_t* space = currentSpace;
			
			if( guiStereoScreenOffset != 0.0f )
			{
				RB_SetMVPWithStereoOffset( space->mvp, currentGuiStereoOffset );
			}
			else
			{
				RB_SetMVP( space->mvp );
			}
			
			// set eye position in local space
			idVec4 localViewOrigin( 1.0f );
			R_GlobalPointToLocal( space->modelMatrix, viewDef->renderView.vieworg, localViewOrigin.ToVec3() );
			SetVertexParm( RENDERPARM_LOCALVIEWORIGIN, localViewOrigin.ToFloatPtr() );
			
			// set model Matrix
			float modelMatrixTranspose[16];
			R_MatrixTranspose( space->modelMatrix, modelMatrixTranspose );
			SetVertexParms( RENDERPARM_MODELMATRIX_X, modelMatrixTranspose, 4 );
			
			// Set ModelView Matrix
			float modelViewMatrixTranspose[16];
			R_MatrixTranspose( space->modelViewMatrix, modelViewMatrixTranspose );
			SetVertexParms( RENDERPARM_MODELVIEWMATRIX_X, modelViewMatrixTranspose, 4 );
		}
		
		// change the scissor if needed
		if( !currentScissor.Equals( surf->scissorRect ) && r_useScissor.GetBool() )
		{
			GL_Scissor( viewDef->viewport.x1 + surf->scissorRect.x1,
						viewDef->viewport.y1 + surf->scissorRect.y1,
						surf->scissorRect.x2 + 1 - surf->scissorRect.x1,
						surf->scissorRect.y2 + 1 - surf->scissorRect.y1 );
						
			currentScissor = surf->scissorRect;
		}
		
		// get the expressions for conditionals / color / texcoords
		const float*	regs = surf->shaderRegisters;
		
		// set face culling appropriately
		uint64 cullMode;
		if( surf->space->isGuiSurface )
		{
			cullMode = GLS_CULL_TWOSIDED;
		}
		else
		{
			switch( shader->GetCullType() )
			{
				case CT_TWO_SIDED:
					cullMode = GLS_CULL_TWOSIDED;
					break;
					
				case CT_BACK_SIDED:
					cullMode = GLS_CULL_BACKSIDED;
					break;
					
				case CT_FRONT_SIDED:
				default:
					cullMode = GLS_CULL_FRONTSIDED;
					break;
			}
		}
		
		uint64 surfGLState = surf->extraGLState | cullMode;
		
		// set polygon offset if necessary
		
		if( shader->TestMaterialFlag( MF_POLYGONOFFSET ) )
		{
			surfGLState = GLS_POLYGON_OFFSET;
			
			// RB: make sure the pipeline of the current shader has dynamic polygon offset enabled
			//renderProgManager.CommitUniforms( surfGLState );
			
#if !defined( USE_VULKAN )
			GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * shader->GetPolygonOffset() );
#endif
		}
		
		for( int stage = 0; stage < shader->GetNumStages(); stage++ )
		{
			const shaderStage_t* pStage = shader->GetStage( stage );
			
			// check the enable condition
			if( regs[ pStage->conditionRegister ] == 0 )
			{
				continue;
			}
			
			// skip the stages involved in lighting
			if( pStage->lighting != SL_AMBIENT )
			{
				continue;
			}
			
			uint64 stageGLState = surfGLState;
			if( ( surfGLState & GLS_OVERRIDE ) == 0 )
			{
				stageGLState |= pStage->drawStateBits;
			}
			
			// skip if the stage is ( GL_ZERO, GL_ONE ), which is used for some alpha masks
			if( ( stageGLState & ( GLS_SRCBLEND_BITS | GLS_DSTBLEND_BITS ) ) == ( GLS_SRCBLEND_ZERO | GLS_DSTBLEND_ONE ) )
			{
				continue;
			}
			
			
			// see if we are a new-style stage
			newShaderStage_t* newStage = pStage->newStage;
			if( newStage != NULL )
			{
				//--------------------------
				//
				// new style stages
				//
				//--------------------------
				if( r_skipNewAmbient.GetBool() )
				{
					continue;
				}
				renderLog.OpenBlock( "New Shader Stage" );
				
				GL_State( stageGLState );
				
				renderProgManager.BindProgram( newStage->glslProgram );
				
				for( int j = 0; j < newStage->numVertexParms; j++ )
				{
					float parm[4];
					parm[0] = regs[ newStage->vertexParms[j][0] ];
					parm[1] = regs[ newStage->vertexParms[j][1] ];
					parm[2] = regs[ newStage->vertexParms[j][2] ];
					parm[3] = regs[ newStage->vertexParms[j][3] ];
					SetVertexParm( ( renderParm_t )( RENDERPARM_USER0 + j ), parm );
				}
				
				// set rpEnableSkinning if the shader has optional support for skinning
				if( surf->jointCache && renderProgManager.ShaderHasOptionalSkinning() )
				{
					const idVec4 skinningParm( 1.0f );
					SetVertexParm( RENDERPARM_ENABLE_SKINNING, skinningParm.ToFloatPtr() );
				}
				
				// bind texture units
				for( int j = 0; j < newStage->numFragmentProgramImages; j++ )
				{
					idImage* image = newStage->fragmentProgramImages[j];
					if( image != NULL )
					{
						GL_SelectTexture( j );
						image->Bind();
					}
				}
				
				// draw it
				DrawElementsWithCounters( surf );
				
				// clear rpEnableSkinning if it was set
				if( surf->jointCache && renderProgManager.ShaderHasOptionalSkinning() )
				{
					const idVec4 skinningParm( 0.0f );
					SetVertexParm( RENDERPARM_ENABLE_SKINNING, skinningParm.ToFloatPtr() );
				}
				
				GL_SelectTexture( 0 );
				renderProgManager.Unbind();
				
				renderLog.CloseBlock();
				continue;
			}
			
			//--------------------------
			//
			// old style stages
			//
			//--------------------------
			
			// set the color
			idVec4 color;
			color[0] = regs[ pStage->color.registers[0] ];
			color[1] = regs[ pStage->color.registers[1] ];
			color[2] = regs[ pStage->color.registers[2] ];
			color[3] = regs[ pStage->color.registers[3] ];
			
			// skip the entire stage if an add would be black
			if( ( stageGLState & ( GLS_SRCBLEND_BITS | GLS_DSTBLEND_BITS ) ) == ( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE )
					&& color[0] <= 0 && color[1] <= 0 && color[2] <= 0 )
			{
				continue;
			}
			
			// skip the entire stage if a blend would be completely transparent
			if( ( stageGLState & ( GLS_SRCBLEND_BITS | GLS_DSTBLEND_BITS ) ) == ( GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA )
					&& color[3] <= 0 )
			{
				continue;
			}
			
			stageVertexColor_t svc = pStage->vertexColor;
			
			renderLog.OpenBlock( "Old Shader Stage" );
			GL_Color( color );
			
			if( surf->space->isGuiSurface )
			{
				// Force gui surfaces to always be SVC_MODULATE
				svc = SVC_MODULATE;
				
				// use special shaders for bink cinematics
				if( pStage->texture.cinematic )
				{
					if( ( stageGLState & GLS_OVERRIDE ) != 0 )
					{
						// This is a hack... Only SWF Guis set GLS_OVERRIDE
						// Old style guis do not, and we don't want them to use the new GUI renederProg
						renderProgManager.BindShader_TextureVertexColor_sRGB();
					}
					else
					{
						renderProgManager.BindShader_TextureVertexColor();
					}
				}
				else
				{
					if( ( stageGLState & GLS_OVERRIDE ) != 0 )
					{
						// This is a hack... Only SWF Guis set GLS_OVERRIDE
						// Old style guis do not, and we don't want them to use the new GUI renderProg
						renderProgManager.BindShader_GUI();
					}
					else
					{
						if( surf->jointCache )
						{
							renderProgManager.BindShader_TextureVertexColorSkinned();
						}
						else
						{
							if( viewDef->is2Dgui && r_useHDR.GetBool())
							{
								// RB: 2D fullscreen drawing like warp or damage blend effects
								renderProgManager.BindShader_TextureVertexColor_sRGB();
							}
							else
							{
								renderProgManager.BindShader_TextureVertexColor();
							}
						}
					}
				}
			}
			else if( ( pStage->texture.texgen == TG_SCREEN ) || ( pStage->texture.texgen == TG_SCREEN2 ) )
			{
				renderProgManager.BindShader_TextureTexGenVertexColor();
			}
			else if( pStage->texture.cinematic )
			{
				renderProgManager.BindShader_Bink();
			}
			else
			{
				if( surf->jointCache )
				{
					renderProgManager.BindShader_TextureVertexColorSkinned();
				}
				else
				{
					renderProgManager.BindShader_TextureVertexColor();
				}
			}
			
			RB_SetVertexColorParms( svc );
			
			// bind the texture
			BindVariableStageImage( &pStage->texture, regs );
			
			if( pStage->privatePolygonOffset )
			{
				stageGLState |= GLS_POLYGON_OFFSET;
			}
			
			// set the state
			GL_State( stageGLState );
			
			// set privatePolygonOffset if necessary
#if defined( USE_VULKAN )
			if( shader->TestMaterialFlag( MF_POLYGONOFFSET ) || pStage->privatePolygonOffset )
			{
				// RB: make sure the pipeline of the current shader has dynamic polygon offset enabled
				renderProgManager.CommitUniforms( stageGLState );
				
				if( shader->TestMaterialFlag( MF_POLYGONOFFSET ) )
				{
					GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * shader->GetPolygonOffset() );
				}
				else
				{
					GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * pStage->privatePolygonOffset );
				}
			}
#else
			if( pStage->privatePolygonOffset )
			{
				GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * pStage->privatePolygonOffset );
			}
#endif
			
			PrepareStageTexturing( pStage, surf );
			
			// draw it
			DrawElementsWithCounters( surf );
			
			FinishStageTexturing( pStage, surf );
			
			// unset privatePolygonOffset if necessary
			if( pStage->privatePolygonOffset )
			{
				GL_PolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * shader->GetPolygonOffset() );
			}
			
			renderLog.CloseBlock();
		}
		
		renderLog.CloseBlock();
	}
	
	GL_Color( 1.0f, 1.0f, 1.0f );
	
	// disable stencil shadow test
	GL_State( GLS_DEFAULT );
	
	GL_SelectTexture( 0 );
	
	renderLog.CloseBlock();
	return i;
}

/*
=============================================================================================

BLEND LIGHT PROJECTION

=============================================================================================
*/

/*
=====================
idRenderBackend::T_BlendLight
=====================
*/
void idRenderBackend::T_BlendLight( const drawSurf_t* drawSurfs, const viewLight_t* vLight )
{
	currentSpace = NULL;
	
	for( const drawSurf_t* drawSurf = drawSurfs; drawSurf != NULL; drawSurf = drawSurf->nextOnLight )
	{
		if( drawSurf->scissorRect.IsEmpty() )
		{
			continue;	// !@# FIXME: find out why this is sometimes being hit!
			// temporarily jump over the scissor and draw so the gl error callback doesn't get hit
		}
		
		if( !currentScissor.Equals( drawSurf->scissorRect ) && r_useScissor.GetBool() )
		{
			// change the scissor
			GL_Scissor( viewDef->viewport.x1 + drawSurf->scissorRect.x1,
						viewDef->viewport.y1 + drawSurf->scissorRect.y1,
						drawSurf->scissorRect.x2 + 1 - drawSurf->scissorRect.x1,
						drawSurf->scissorRect.y2 + 1 - drawSurf->scissorRect.y1 );
						
			currentScissor = drawSurf->scissorRect;
		}
		
		if( drawSurf->space != currentSpace )
		{
			// change the matrix
			RB_SetMVP( drawSurf->space->mvp );
			
			// change the light projection matrix
			idPlane	lightProjectInCurrentSpace[4];
			for( int i = 0; i < 4; i++ )
			{
				R_GlobalPlaneToLocal( drawSurf->space->modelMatrix, vLight->lightProject[i], lightProjectInCurrentSpace[i] );
			}
			
			SetVertexParm( RENDERPARM_TEXGEN_0_S, lightProjectInCurrentSpace[0].ToFloatPtr() );
			SetVertexParm( RENDERPARM_TEXGEN_0_T, lightProjectInCurrentSpace[1].ToFloatPtr() );
			SetVertexParm( RENDERPARM_TEXGEN_0_Q, lightProjectInCurrentSpace[2].ToFloatPtr() );
			SetVertexParm( RENDERPARM_TEXGEN_1_S, lightProjectInCurrentSpace[3].ToFloatPtr() );	// falloff
			
			currentSpace = drawSurf->space;
		}
		
		DrawElementsWithCounters( drawSurf );
	}
}

/*
=====================
idRenderBackend::BlendLight

Dual texture together the falloff and projection texture with a blend
mode to the framebuffer, instead of interacting with the surface texture
=====================
*/
void idRenderBackend::BlendLight( const drawSurf_t* drawSurfs, const drawSurf_t* drawSurfs2, const viewLight_t* vLight )
{
	if( drawSurfs == NULL )
	{
		return;
	}
	if( r_skipBlendLights.GetBool() )
	{
		return;
	}
	renderLog.OpenBlock( vLight->lightShader->GetName() );
	
	const idMaterial* lightShader = vLight->lightShader;
	const float*	 regs = vLight->shaderRegisters;
	
	// texture 1 will get the falloff texture
	GL_SelectTexture( 1 );
	vLight->falloffImage->Bind();
	
	// texture 0 will get the projected texture
	GL_SelectTexture( 0 );
	
	renderProgManager.BindShader_BlendLight();
	
	for( int i = 0; i < lightShader->GetNumStages(); i++ )
	{
		const shaderStage_t*	stage = lightShader->GetStage( i );
		
		if( !regs[ stage->conditionRegister ] )
		{
			continue;
		}
		
		GL_State( GLS_DEPTHMASK | stage->drawStateBits | GLS_DEPTHFUNC_EQUAL );
		
		GL_SelectTexture( 0 );
		stage->texture.image->Bind();
		
		if( stage->texture.hasMatrix )
		{
			RB_LoadShaderTextureMatrix( regs, &stage->texture );
		}
		
		// get the modulate values from the light, including alpha, unlike normal lights
		idVec4 lightColor;
		lightColor[0] = regs[ stage->color.registers[0] ];
		lightColor[1] = regs[ stage->color.registers[1] ];
		lightColor[2] = regs[ stage->color.registers[2] ];
		lightColor[3] = regs[ stage->color.registers[3] ];
		GL_Color( lightColor );
		
		T_BlendLight( drawSurfs, vLight );
		T_BlendLight( drawSurfs2, vLight );
	}
	
	GL_SelectTexture( 0 );
	
	renderProgManager.Unbind();
	renderLog.CloseBlock();
}

/*
=========================================================================================================

FOG LIGHTS

=========================================================================================================
*/

/*
=====================
idRenderBackend::T_BasicFog
=====================
*/
void idRenderBackend::T_BasicFog( const drawSurf_t* drawSurfs, const idPlane fogPlanes[4], const idRenderMatrix* inverseBaseLightProject )
{
	currentSpace = NULL;
	
	for( const drawSurf_t* drawSurf = drawSurfs; drawSurf != NULL; drawSurf = drawSurf->nextOnLight )
	{
		if( drawSurf->scissorRect.IsEmpty() )
		{
			continue;	// !@# FIXME: find out why this is sometimes being hit!
			// temporarily jump over the scissor and draw so the gl error callback doesn't get hit
		}
		
		if( !currentScissor.Equals( drawSurf->scissorRect ) && r_useScissor.GetBool() )
		{
			// change the scissor
			GL_Scissor( viewDef->viewport.x1 + drawSurf->scissorRect.x1,
						viewDef->viewport.y1 + drawSurf->scissorRect.y1,
						drawSurf->scissorRect.x2 + 1 - drawSurf->scissorRect.x1,
						drawSurf->scissorRect.y2 + 1 - drawSurf->scissorRect.y1 );
						
			currentScissor = drawSurf->scissorRect;
		}
		
		if( drawSurf->space != currentSpace )
		{
			idPlane localFogPlanes[4];
			if( inverseBaseLightProject == NULL )
			{
				RB_SetMVP( drawSurf->space->mvp );
				for( int i = 0; i < 4; i++ )
				{
					R_GlobalPlaneToLocal( drawSurf->space->modelMatrix, fogPlanes[i], localFogPlanes[i] );
				}
			}
			else
			{
				idRenderMatrix invProjectMVPMatrix;
				idRenderMatrix::Multiply( viewDef->worldSpace.mvp, *inverseBaseLightProject, invProjectMVPMatrix );
				RB_SetMVP( invProjectMVPMatrix );
				for( int i = 0; i < 4; i++ )
				{
					inverseBaseLightProject->InverseTransformPlane( fogPlanes[i], localFogPlanes[i], false );
				}
			}
			
			SetVertexParm( RENDERPARM_TEXGEN_0_S, localFogPlanes[0].ToFloatPtr() );
			SetVertexParm( RENDERPARM_TEXGEN_0_T, localFogPlanes[1].ToFloatPtr() );
			SetVertexParm( RENDERPARM_TEXGEN_1_T, localFogPlanes[2].ToFloatPtr() );
			SetVertexParm( RENDERPARM_TEXGEN_1_S, localFogPlanes[3].ToFloatPtr() );
			
			currentSpace = ( inverseBaseLightProject == NULL ) ? drawSurf->space : NULL;
		}
		
		if( drawSurf->jointCache )
		{
			renderProgManager.BindShader_FogSkinned();
		}
		else
		{
			renderProgManager.BindShader_Fog();
		}
		
		DrawElementsWithCounters( drawSurf );
	}
}

/*
==================
idRenderBackend::FogPass
==================
*/
void idRenderBackend::FogPass( const drawSurf_t* drawSurfs,  const drawSurf_t* drawSurfs2, const viewLight_t* vLight )
{
	renderLog.OpenBlock( vLight->lightShader->GetName() );
	
	// find the current color and density of the fog
	const idMaterial* lightShader = vLight->lightShader;
	const float* regs = vLight->shaderRegisters;
	// assume fog shaders have only a single stage
	const shaderStage_t* stage = lightShader->GetStage( 0 );
	
	idVec4 lightColor;
	lightColor[0] = regs[ stage->color.registers[0] ];
	lightColor[1] = regs[ stage->color.registers[1] ];
	lightColor[2] = regs[ stage->color.registers[2] ];
	lightColor[3] = regs[ stage->color.registers[3] ];
	
	GL_Color( lightColor );
	
	// calculate the falloff planes
	float a;
	
	// if they left the default value on, set a fog distance of 500
	if( lightColor[3] <= 1.0f )
	{
		a = -0.5f / DEFAULT_FOG_DISTANCE;
	}
	else
	{
		// otherwise, distance = alpha color
		a = -0.5f / lightColor[3];
	}
	
	// texture 0 is the falloff image
	GL_SelectTexture( 0 );
	globalImages->fogImage->Bind();
	
	// texture 1 is the entering plane fade correction
	GL_SelectTexture( 1 );
	globalImages->fogEnterImage->Bind();
	
	// S is based on the view origin
	const float s = vLight->fogPlane.Distance( viewDef->renderView.vieworg );
	
	const float FOG_SCALE = 0.001f;
	
	idPlane fogPlanes[4];
	
	// S-0
	fogPlanes[0][0] = a * viewDef->worldSpace.modelViewMatrix[0 * 4 + 2];
	fogPlanes[0][1] = a * viewDef->worldSpace.modelViewMatrix[1 * 4 + 2];
	fogPlanes[0][2] = a * viewDef->worldSpace.modelViewMatrix[2 * 4 + 2];
	fogPlanes[0][3] = a * viewDef->worldSpace.modelViewMatrix[3 * 4 + 2] + 0.5f;
	
	// T-0
	fogPlanes[1][0] = 0.0f;//a * backEnd.viewDef->worldSpace.modelViewMatrix[0*4+0];
	fogPlanes[1][1] = 0.0f;//a * backEnd.viewDef->worldSpace.modelViewMatrix[1*4+0];
	fogPlanes[1][2] = 0.0f;//a * backEnd.viewDef->worldSpace.modelViewMatrix[2*4+0];
	fogPlanes[1][3] = 0.5f;//a * backEnd.viewDef->worldSpace.modelViewMatrix[3*4+0] + 0.5f;
	
	// T-1 will get a texgen for the fade plane, which is always the "top" plane on unrotated lights
	fogPlanes[2][0] = FOG_SCALE * vLight->fogPlane[0];
	fogPlanes[2][1] = FOG_SCALE * vLight->fogPlane[1];
	fogPlanes[2][2] = FOG_SCALE * vLight->fogPlane[2];
	fogPlanes[2][3] = FOG_SCALE * vLight->fogPlane[3] + FOG_ENTER;
	
	// S-1
	fogPlanes[3][0] = 0.0f;
	fogPlanes[3][1] = 0.0f;
	fogPlanes[3][2] = 0.0f;
	fogPlanes[3][3] = FOG_SCALE * s + FOG_ENTER;
	
	// draw it
	GL_State( GLS_DEPTHMASK | GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA | GLS_DEPTHFUNC_EQUAL );
	T_BasicFog( drawSurfs, fogPlanes, NULL );
	T_BasicFog( drawSurfs2, fogPlanes, NULL );
	
	// the light frustum bounding planes aren't in the depth buffer, so use depthfunc_less instead
	// of depthfunc_equal
	GL_State( GLS_DEPTHMASK | GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA | GLS_DEPTHFUNC_LESS | GLS_CULL_BACKSIDED );
	
	zeroOneCubeSurface.space = &viewDef->worldSpace;
	zeroOneCubeSurface.scissorRect = viewDef->scissor;
	T_BasicFog( &zeroOneCubeSurface, fogPlanes, &vLight->inverseBaseLightProject );
	
	GL_State( glStateBits & ~( GLS_CULL_MASK ) | GLS_CULL_FRONTSIDED );
	
	GL_SelectTexture( 0 );
	
	renderProgManager.Unbind();
	
	renderLog.CloseBlock();
}

/*
==================
idRenderBackend::FogAllLights
==================
*/
void idRenderBackend::FogAllLights()
{
	if( r_skipFogLights.GetBool() || r_showOverDraw.GetInteger() != 0
			|| viewDef->isXraySubview /* don't fog in xray mode*/ )
	{
		return;
	}
	renderLog.OpenMainBlock( MRB_FOG_ALL_LIGHTS );
	renderLog.OpenBlock( "RB_FogAllLights" );
	
	// force fog plane to recalculate
	currentSpace = NULL;
	
	for( viewLight_t* vLight = viewDef->viewLights; vLight != NULL; vLight = vLight->next )
	{
		if( vLight->lightShader->IsFogLight() )
		{
			FogPass( vLight->globalInteractions, vLight->localInteractions, vLight );
		}
		else if( vLight->lightShader->IsBlendLight() )
		{
			BlendLight( vLight->globalInteractions, vLight->localInteractions, vLight );
		}
	}
	
	renderLog.CloseBlock();
	renderLog.CloseMainBlock();
}

// RB begin
void idRenderBackend::CalculateAutomaticExposure()
{
	int				i;
	static float	image[64 * 64 * 4];
	float           curTime;
	float			deltaTime;
	float           luminance;
	float			avgLuminance;
	float			maxLuminance;
	double			sum;
	const idVec3    LUMINANCE_SRGB( 0.2125f, 0.7154f, 0.0721f ); // be careful wether this should be linear RGB or sRGB
	idVec4			color;
	float			newAdaptation;
	float			newMaximum;
	
	if( !r_hdrAutoExposure.GetBool() )
	{
		// no dynamic exposure
		
		hdrKey = r_hdrKey.GetFloat();
		hdrAverageLuminance = r_hdrMinLuminance.GetFloat();
		hdrMaxLuminance = 1;
	}
	else
	{
		curTime = Sys_Milliseconds() / 1000.0f;
		
		// calculate the average scene luminance
		globalFramebuffers.hdr64FBO->Bind();
		
		// FIXME
#if !defined(USE_VULKAN)
		// read back the contents
		glReadPixels( 0, 0, 64, 64, GL_RGBA8, GL_FLOAT, image );
#endif
		
		sum = 0.0f;
		maxLuminance = 0.0f;
		for( i = 0; i < ( 64 * 64 ); i += 4 )
		{
			color[0] = image[i * 4 + 0];
			color[1] = image[i * 4 + 1];
			color[2] = image[i * 4 + 2];
			color[3] = image[i * 4 + 3];
			
			luminance = ( color.x * LUMINANCE_SRGB.x + color.y * LUMINANCE_SRGB.y + color.z * LUMINANCE_SRGB.z ) + 0.0001f;
			if( luminance > maxLuminance )
			{
				maxLuminance = luminance;
			}
			
			//float logLuminance = log( luminance + 1 );
			//if( logLuminance > 0 )
			{
				sum += luminance;
			}
		}
#if 0
		sum /= ( 64.0f * 64.0f );
		avgLuminance = exp( sum );
#else
		avgLuminance = sum / ( 64.0f * 64.0f );
#endif
		
		// the user's adapted luminance level is simulated by closing the gap between
		// adapted luminance and current luminance by 2% every frame, based on a
		// 30 fps rate. This is not an accurate model of human adaptation, which can
		// take longer than half an hour.
		if( hdrTime > curTime )
		{
			hdrTime = curTime;
		}
		
		deltaTime = curTime - hdrTime;
		
		//if(r_hdrMaxLuminance->value)
		{
			hdrAverageLuminance = idMath::ClampFloat( r_hdrMinLuminance.GetFloat(), r_hdrMaxLuminance.GetFloat(), hdrAverageLuminance );
			avgLuminance = idMath::ClampFloat( r_hdrMinLuminance.GetFloat(), r_hdrMaxLuminance.GetFloat(), avgLuminance );
			
			hdrMaxLuminance = idMath::ClampFloat( r_hdrMinLuminance.GetFloat(), r_hdrMaxLuminance.GetFloat(), hdrMaxLuminance );
			maxLuminance = idMath::ClampFloat( r_hdrMinLuminance.GetFloat(), r_hdrMaxLuminance.GetFloat(), maxLuminance );
		}
		
		newAdaptation = hdrAverageLuminance + ( avgLuminance - hdrAverageLuminance ) * ( 1.0f - powf( 0.98f, 30.0f * deltaTime ) );
		newMaximum = hdrMaxLuminance + ( maxLuminance - hdrMaxLuminance ) * ( 1.0f - powf( 0.98f, 30.0f * deltaTime ) );
		
		if( !IsNAN( newAdaptation ) && !IsNAN( newMaximum ) )
		{
#if 1
			hdrAverageLuminance = newAdaptation;
			hdrMaxLuminance = newMaximum;
#else
			hdrAverageLuminance = avgLuminance;
			hdrMaxLuminance = maxLuminance;
#endif
		}
		
		hdrTime = curTime;
		
		// calculate HDR image key
#if 0
		// RB: this never worked :/
		if( r_hdrAutoExposure.GetBool() )
		{
			// calculation from: Perceptual Effects in Real-time Tone Mapping - Krawczyk et al.
			hdrKey = 1.03 - ( 2.0 / ( 2.0 + ( hdrAverageLuminance + 1.0f ) ) );
		}
		else
#endif
		{
			hdrKey = r_hdrKey.GetFloat();
		}
	}
	
	if( r_hdrDebug.GetBool() )
	{
		idLib::Printf( "HDR luminance avg = %f, max = %f, key = %f\n", hdrAverageLuminance, hdrMaxLuminance, hdrKey );
	}
	
	//GL_CheckErrors();
}


void idRenderBackend::Tonemap( const viewDef_t* _viewDef )
{
	RENDERLOG_PRINTF( "---------- RB_Tonemap( avg = %f, max = %f, key = %f, is2Dgui = %i ) ----------\n", hdrAverageLuminance, hdrMaxLuminance, hdrKey, ( int )viewDef->is2Dgui );
	
	//postProcessCommand_t* cmd = ( postProcessCommand_t* )data;
	//const idScreenRect& viewport = cmd->viewDef->viewport;
	//globalImages->currentRenderImage->CopyFramebuffer( viewport.x1, viewport.y1, viewport.GetWidth(), viewport.GetHeight() );
	
	Framebuffer::Unbind();
	
	GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS | GLS_CULL_TWOSIDED );
	
	int screenWidth = renderSystem->GetWidth();
	int screenHeight = renderSystem->GetHeight();
	
	// set the window clipping
	GL_Viewport( 0, 0, screenWidth, screenHeight );
	GL_Scissor( 0, 0, screenWidth, screenHeight );
	
	GL_SelectTexture( 0 );
	
#if defined(USE_HDR_MSAA)
	if( glConfig.multisamples > 0 )
	{
		globalImages->currentRenderHDRImageNoMSAA->Bind();
	}
	else
#endif
	{
		globalImages->currentRenderHDRImage->Bind();
	}
	
	GL_SelectTexture( 1 );
	globalImages->heatmap7Image->Bind();
	
	if( r_hdrDebug.GetBool() )
	{
		renderProgManager.BindShader_HDRDebug();
	}
	else
	{
		renderProgManager.BindShader_Tonemap();
	}
	
	float screenCorrectionParm[4];
	if( _viewDef->is2Dgui )
	{
		screenCorrectionParm[0] = 2.0f;
		screenCorrectionParm[1] = 1.0f;
		screenCorrectionParm[2] = 1.0f;
	}
	else
	{
		if( r_hdrAutoExposure.GetBool() )
		{
			float exposureOffset = Lerp( -0.01f, 0.02f, idMath::ClampFloat( 0.0, 1.0, r_exposure.GetFloat() ) );
			
			screenCorrectionParm[0] = hdrKey + exposureOffset;
			screenCorrectionParm[1] = hdrAverageLuminance;
			screenCorrectionParm[2] = hdrMaxLuminance;
			screenCorrectionParm[3] = exposureOffset;
			//screenCorrectionParm[3] = Lerp( -1, 5, idMath::ClampFloat( 0.0, 1.0, r_exposure.GetFloat() ) );
		}
		else
		{
			//float exposureOffset = ( idMath::ClampFloat( 0.0, 1.0, r_exposure.GetFloat() ) * 2.0f - 1.0f ) * 0.01f;
			
			float exposureOffset = Lerp( -0.01f, 0.01f, idMath::ClampFloat( 0.0, 1.0, r_exposure.GetFloat() ) );
			
			screenCorrectionParm[0] = 0.015f + exposureOffset;
			screenCorrectionParm[1] = 0.005f;
			screenCorrectionParm[2] = 1;
			
			// RB: this gives a nice exposure curve in Scilab when using
			// log2( max( 3 + 0..10, 0.001 ) ) as input for exp2
			//float exposureOffset = r_exposure.GetFloat() * 10.0f;
			//screenCorrectionParm[3] = exposureOffset;
		}
	}
	
	SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
	
	// Draw
	DrawElementsWithCounters( &unitSquareSurface );
	
	GL_SelectTexture( 0 );
	renderProgManager.Unbind();
	
	GL_State( GLS_DEFAULT );
}


void idRenderBackend::Bloom( const viewDef_t* _viewDef )
{
	if( _viewDef->is2Dgui || !r_useHDR.GetBool() )
	{
		return;
	}
	
	RENDERLOG_PRINTF( "---------- RB_Bloom( avg = %f, max = %f, key = %f ) ----------\n", hdrAverageLuminance, hdrMaxLuminance, hdrKey );
	
	// BRIGHTPASS
	
	//GL_CheckErrors();
	
	//Framebuffer::Unbind();
	//globalFramebuffers.hdrQuarterFBO->Bind();
	
	// FIXME
#if !defined(USE_VULKAN)
	glClearColor( 0, 0, 0, 1 );
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
#endif

	GL_State( /*GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO |*/ GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS | GLS_CULL_TWOSIDED );
	
	int screenWidth = renderSystem->GetWidth();
	int screenHeight = renderSystem->GetHeight();
	
	// set the window clipping
	GL_Viewport( 0, 0, screenWidth / 4, screenHeight / 4 );
	GL_Scissor( 0, 0, screenWidth / 4, screenHeight / 4 );
	
	globalFramebuffers.bloomRenderFBO[ 0 ]->Bind();
	
	GL_SelectTexture( 0 );
	
	if( r_useHDR.GetBool() )
	{
		globalImages->currentRenderHDRImage->Bind();
		
		renderProgManager.BindShader_Brightpass();
	}
	else
	{
		int x = viewDef->viewport.x1;
		int y = viewDef->viewport.y1;
		int	w = viewDef->viewport.x2 - viewDef->viewport.x1 + 1;
		int	h = viewDef->viewport.y2 - viewDef->viewport.y1 + 1;
		
		RENDERLOG_PRINTF( "Resolve to %i x %i buffer\n", w, h );
		
		// resolve the screen
		globalImages->currentRenderImage->CopyFramebuffer( x, y, w, h );
		
		renderProgManager.BindShader_Brightpass();
	}
	
	float screenCorrectionParm[4];
	screenCorrectionParm[0] = hdrKey;
	screenCorrectionParm[1] = hdrAverageLuminance;
	screenCorrectionParm[2] = hdrMaxLuminance;
	screenCorrectionParm[3] = 1.0f;
	SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
	
	float overbright[4];
	if( r_useHDR.GetBool() )
	{
		if( r_hdrAutoExposure.GetBool() )
		{
			overbright[0] = r_hdrContrastDynamicThreshold.GetFloat();
		}
		else
		{
			overbright[0] = r_hdrContrastStaticThreshold.GetFloat();
		}
		overbright[1] = r_hdrContrastOffset.GetFloat();
		overbright[2] = 0;
		overbright[3] = 0;
	}
	else
	{
		overbright[0] = r_ldrContrastThreshold.GetFloat();
		overbright[1] = r_ldrContrastOffset.GetFloat();
		overbright[2] = 0;
		overbright[3] = 0;
	}
	SetFragmentParm( RENDERPARM_OVERBRIGHT, overbright ); // rpOverbright
	
	// Draw
	DrawElementsWithCounters( &unitSquareSurface );
	
	
	// BLOOM PING PONG rendering
	renderProgManager.BindShader_HDRGlareChromatic();
	
	int j;
	for( j = 0; j < r_hdrGlarePasses.GetInteger(); j++ )
	{
		globalFramebuffers.bloomRenderFBO[( j + 1 ) % 2 ]->Bind();
		
		// FIXME
#if !defined(USE_VULKAN)
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
#endif
		
		globalImages->bloomRenderImage[j % 2]->Bind();
		
		DrawElementsWithCounters( &unitSquareSurface );
	}
	
	// add filtered glare back to main context
	Framebuffer::Unbind();
	
	ResetViewportAndScissorToDefaultCamera( _viewDef );
	
	GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS );
	
	renderProgManager.BindShader_Screen();
	
	globalImages->bloomRenderImage[( j + 1 ) % 2]->Bind();
	
	DrawElementsWithCounters( &unitSquareSurface );
	
	renderProgManager.Unbind();
	
	GL_State( GLS_DEFAULT );
}


void idRenderBackend::DrawScreenSpaceAmbientOcclusion( const viewDef_t* _viewDef )
{
#if !defined(USE_VULKAN)
	if( !_viewDef->viewEntitys || _viewDef->is2Dgui )
	{
		// 3D views only
		return;
	}
	
	if( r_useSSAO.GetInteger() <= 0 )
	{
		return;
	}
	
	// FIXME very expensive to enable this in subviews
	if( _viewDef->isSubview )
	{
		return;
	}
	
	RENDERLOG_PRINTF( "---------- RB_SSAO() ----------\n" );
	
#if 0
	//GL_CheckErrors();
	
	// clear the alpha buffer and draw only the hands + weapon into it so
	// we can avoid blurring them
	glClearColor( 0, 0, 0, 1 );
	GL_State( GLS_COLORMASK | GLS_DEPTHMASK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	GL_Color( 0, 0, 0, 0 );
	
	
	GL_SelectTexture( 0 );
	globalImages->blackImage->Bind();
	backEnd.currentSpace = NULL;
	
	drawSurf_t** drawSurfs = ( drawSurf_t** )&backEnd.viewDef->drawSurfs[0];
	for( int surfNum = 0; surfNum < backEnd.viewDef->numDrawSurfs; surfNum++ )
	{
		const drawSurf_t* surf = drawSurfs[ surfNum ];
		
		if( !surf->space->weaponDepthHack && !surf->space->skipMotionBlur && !surf->material->HasSubview() )
		{
			// Apply motion blur to this object
			continue;
		}
		
		const idMaterial* shader = surf->material;
		if( shader->Coverage() == MC_TRANSLUCENT )
		{
			// muzzle flash, etc
			continue;
		}
		
		// set mvp matrix
		if( surf->space != backEnd.currentSpace )
		{
			RB_SetMVP( surf->space->mvp );
			backEnd.currentSpace = surf->space;
		}
		
		// this could just be a color, but we don't have a skinned color-only prog
		if( surf->jointCache )
		{
			renderProgManager.BindShader_TextureVertexColorSkinned();
		}
		else
		{
			renderProgManager.BindShader_TextureVertexColor();
		}
		
		// draw it solid
		RB_DrawElementsWithCounters( surf );
	}
	GL_State( GLS_DEPTHFUNC_ALWAYS );
	
	// copy off the color buffer and the depth buffer for the motion blur prog
	// we use the viewport dimensions for copying the buffers in case resolution scaling is enabled.
	const idScreenRect& viewport = backEnd.viewDef->viewport;
	globalImages->currentRenderImage->CopyFramebuffer( viewport.x1, viewport.y1, viewport.GetWidth(), viewport.GetHeight() );
	
	// in stereo rendering, each eye needs to get a separate previous frame mvp
	int mvpIndex = ( backEnd.viewDef->renderView.viewEyeBuffer == 1 ) ? 1 : 0;
	
	// derive the matrix to go from current pixels to previous frame pixels
	idRenderMatrix	inverseMVP;
	idRenderMatrix::Inverse( backEnd.viewDef->worldSpace.mvp, inverseMVP );
	
	idRenderMatrix	motionMatrix;
	idRenderMatrix::Multiply( backEnd.prevMVP[mvpIndex], inverseMVP, motionMatrix );
	
	backEnd.prevMVP[mvpIndex] = backEnd.viewDef->worldSpace.mvp;
	
	RB_SetMVP( motionMatrix );
#endif
	
	currentSpace = &viewDef->worldSpace;
	RB_SetMVP( viewDef->worldSpace.mvp );
	
	const bool hdrIsActive = ( r_useHDR.GetBool() && globalFramebuffers.hdrFBO != NULL && globalFramebuffers.hdrFBO->IsBound() );
	
	int screenWidth = renderSystem->GetWidth();
	int screenHeight = renderSystem->GetHeight();
	
	// build hierarchical depth buffer
	if( r_useHierarchicalDepthBuffer.GetBool() )
	{
		renderProgManager.BindShader_AmbientOcclusionMinify();
		
		glClearColor( 0, 0, 0, 1 );
		
		GL_SelectTexture( 0 );
		//globalImages->currentDepthImage->Bind();
		
		for( int i = 0; i < MAX_HIERARCHICAL_ZBUFFERS; i++ )
		{
			int width = globalFramebuffers.csDepthFBO[i]->GetWidth();
			int height = globalFramebuffers.csDepthFBO[i]->GetHeight();
			
			GL_Viewport( 0, 0, width, height );
			GL_Scissor( 0, 0, width, height );
			
			globalFramebuffers.csDepthFBO[i]->Bind();
			
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
			
			if( i == 0 )
			{
				renderProgManager.BindShader_AmbientOcclusionReconstructCSZ();
				
				globalImages->currentDepthImage->Bind();
			}
			else
			{
				renderProgManager.BindShader_AmbientOcclusionMinify();
				
				GL_SelectTexture( 0 );
				globalImages->hierarchicalZbufferImage->Bind();
			}
			
			float jitterTexScale[4];
			jitterTexScale[0] = i - 1;
			jitterTexScale[1] = 0;
			jitterTexScale[2] = 0;
			jitterTexScale[3] = 0;
			SetFragmentParm( RENDERPARM_JITTERTEXSCALE, jitterTexScale ); // rpJitterTexScale
#if 1
			float screenCorrectionParm[4];
			screenCorrectionParm[0] = 1.0f / width;
			screenCorrectionParm[1] = 1.0f / height;
			screenCorrectionParm[2] = width;
			screenCorrectionParm[3] = height;
			SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
#endif
			
			DrawElementsWithCounters( &unitSquareSurface );
		}
	}
	
	// set the window clipping
	GL_Viewport( 0, 0, screenWidth, screenHeight );
	GL_Scissor( 0, 0, screenWidth, screenHeight );
	
	GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS | GLS_CULL_TWOSIDED );
	
	if( r_ssaoFiltering.GetBool() )
	{
		globalFramebuffers.ambientOcclusionFBO[0]->Bind();
		
		glClearColor( 0, 0, 0, 0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
		renderProgManager.BindShader_AmbientOcclusion();
	}
	else
	{
		if( r_ssaoDebug.GetInteger() <= 0 )
		{
			GL_State( GLS_SRCBLEND_DST_COLOR | GLS_DSTBLEND_ZERO | GLS_ALPHAMASK | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS );
		}
		
		if( hdrIsActive )
		{
			globalFramebuffers.hdrFBO->Bind();
		}
		else
		{
			Framebuffer::Unbind();
		}
		
		renderProgManager.BindShader_AmbientOcclusionAndOutput();
	}
	
	float screenCorrectionParm[4];
	screenCorrectionParm[0] = 1.0f / screenWidth;
	screenCorrectionParm[1] = 1.0f / screenHeight;
	screenCorrectionParm[2] = screenWidth;
	screenCorrectionParm[3] = screenHeight;
	SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
	
#if 0
	// RB: set unprojection matrices so we can convert zbuffer values back to camera and world spaces
	idRenderMatrix modelViewMatrix;
	idRenderMatrix::Transpose( *( idRenderMatrix* )backEnd.viewDef->worldSpace.modelViewMatrix, modelViewMatrix );
	idRenderMatrix cameraToWorldMatrix;
	if( !idRenderMatrix::Inverse( modelViewMatrix, cameraToWorldMatrix ) )
	{
		idLib::Warning( "cameraToWorldMatrix invert failed" );
	}
	
	SetVertexParms( RENDERPARM_MODELMATRIX_X, cameraToWorldMatrix[0], 4 );
	//SetVertexParms( RENDERPARM_MODELMATRIX_X, viewDef->unprojectionToWorldRenderMatrix[0], 4 );
#endif
	SetVertexParms( RENDERPARM_MODELMATRIX_X, viewDef->unprojectionToCameraRenderMatrix[0], 4 );
	
	
	float jitterTexOffset[4];
	if( r_shadowMapRandomizeJitter.GetBool() )
	{
		jitterTexOffset[0] = ( rand() & 255 ) / 255.0;
		jitterTexOffset[1] = ( rand() & 255 ) / 255.0;
	}
	else
	{
		jitterTexOffset[0] = 0;
		jitterTexOffset[1] = 0;
	}
	jitterTexOffset[2] = viewDef->renderView.time[0] * 0.001f;
	jitterTexOffset[3] = 0.0f;
	SetFragmentParm( RENDERPARM_JITTERTEXOFFSET, jitterTexOffset ); // rpJitterTexOffset
	
	GL_SelectTexture( 0 );
	globalImages->currentNormalsImage->Bind();
	
	GL_SelectTexture( 1 );
	if( r_useHierarchicalDepthBuffer.GetBool() )
	{
		globalImages->hierarchicalZbufferImage->Bind();
	}
	else
	{
		globalImages->currentDepthImage->Bind();
	}
	
	DrawElementsWithCounters( &unitSquareSurface );
	
	if( r_ssaoFiltering.GetBool() )
	{
		float jitterTexScale[4];
		
		// AO blur X
#if 1
		globalFramebuffers.ambientOcclusionFBO[1]->Bind();
		
		renderProgManager.BindShader_AmbientOcclusionBlur();
		
		// set axis parameter
		jitterTexScale[0] = 1;
		jitterTexScale[1] = 0;
		jitterTexScale[2] = 0;
		jitterTexScale[3] = 0;
		SetFragmentParm( RENDERPARM_JITTERTEXSCALE, jitterTexScale ); // rpJitterTexScale
		
		GL_SelectTexture( 2 );
		globalImages->ambientOcclusionImage[0]->Bind();
		
		DrawElementsWithCounters( &unitSquareSurface );
#endif
		
		// AO blur Y
		if( hdrIsActive )
		{
			globalFramebuffers.hdrFBO->Bind();
		}
		else
		{
			Framebuffer::Unbind();
		}
		
		if( r_ssaoDebug.GetInteger() <= 0 )
		{
			GL_State( GLS_SRCBLEND_DST_COLOR | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS );
		}
		
		renderProgManager.BindShader_AmbientOcclusionBlurAndOutput();
		
		// set axis parameter
		jitterTexScale[0] = 0;
		jitterTexScale[1] = 1;
		jitterTexScale[2] = 0;
		jitterTexScale[3] = 0;
		SetFragmentParm( RENDERPARM_JITTERTEXSCALE, jitterTexScale ); // rpJitterTexScale
		
		GL_SelectTexture( 2 );
		globalImages->ambientOcclusionImage[1]->Bind();
		
		DrawElementsWithCounters( &unitSquareSurface );
	}
	
	renderProgManager.Unbind();
	
	GL_State( GLS_DEFAULT );
	
	//GL_CheckErrors();
#endif
}

void idRenderBackend::DrawScreenSpaceGlobalIllumination( const viewDef_t* _viewDef )
{
#if !defined(USE_VULKAN)
	if( !_viewDef->viewEntitys || _viewDef->is2Dgui )
	{
		// 3D views only
		return;
	}
	
	if( r_useSSGI.GetInteger() <= 0 )
	{
		return;
	}
	
	// FIXME very expensive to enable this in subviews
	if( _viewDef->isSubview )
	{
		return;
	}
	
	RENDERLOG_PRINTF( "---------- RB_SSGI() ----------\n" );
	
	currentSpace = &viewDef->worldSpace;
	RB_SetMVP( viewDef->worldSpace.mvp );
	
	const bool hdrIsActive = ( r_useHDR.GetBool() && globalFramebuffers.hdrFBO != NULL && globalFramebuffers.hdrFBO->IsBound() );
	
	int screenWidth = renderSystem->GetWidth();
	int screenHeight = renderSystem->GetHeight();
	
	// set the window clipping
	GL_Viewport( 0, 0, screenWidth, screenHeight );
	GL_Scissor( 0, 0, screenWidth, screenHeight );
	
	if( !hdrIsActive )
	{
		const idScreenRect& viewport = viewDef->viewport;
		globalImages->currentRenderImage->CopyFramebuffer( viewport.x1, viewport.y1, viewport.GetWidth(), viewport.GetHeight() );
	}
	
	// build hierarchical depth buffer
	if( r_useHierarchicalDepthBuffer.GetBool() )
	{
		renderProgManager.BindShader_AmbientOcclusionMinify();
		
		glClearColor( 0, 0, 0, 1 );
		
		GL_SelectTexture( 0 );
		//globalImages->currentDepthImage->Bind();
		
		for( int i = 0; i < MAX_HIERARCHICAL_ZBUFFERS; i++ )
		{
			int width = globalFramebuffers.csDepthFBO[i]->GetWidth();
			int height = globalFramebuffers.csDepthFBO[i]->GetHeight();
			
			GL_Viewport( 0, 0, width, height );
			GL_Scissor( 0, 0, width, height );
			
			globalFramebuffers.csDepthFBO[i]->Bind();
			
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
			
			if( i == 0 )
			{
				renderProgManager.BindShader_AmbientOcclusionReconstructCSZ();
				
				globalImages->currentDepthImage->Bind();
			}
			else
			{
				renderProgManager.BindShader_AmbientOcclusionMinify();
				
				GL_SelectTexture( 0 );
				globalImages->hierarchicalZbufferImage->Bind();
			}
			
			float jitterTexScale[4];
			jitterTexScale[0] = i - 1;
			jitterTexScale[1] = 0;
			jitterTexScale[2] = 0;
			jitterTexScale[3] = 0;
			SetFragmentParm( RENDERPARM_JITTERTEXSCALE, jitterTexScale ); // rpJitterTexScale
#if 1
			float screenCorrectionParm[4];
			screenCorrectionParm[0] = 1.0f / width;
			screenCorrectionParm[1] = 1.0f / height;
			screenCorrectionParm[2] = width;
			screenCorrectionParm[3] = height;
			SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
#endif
			
			DrawElementsWithCounters( &unitSquareSurface );
		}
	}
	
	// set the window clipping
	GL_Viewport( 0, 0, screenWidth, screenHeight );
	GL_Scissor( 0, 0, screenWidth, screenHeight );
	
	GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS | GLS_CULL_TWOSIDED );
	
	if( r_ssgiFiltering.GetBool() )
	{
		globalFramebuffers.ambientOcclusionFBO[0]->Bind();
		
		// FIXME remove and mix with color from previous frame
		glClearColor( 0, 0, 0, 0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
		renderProgManager.BindShader_DeepGBufferRadiosity();
	}
	else
	{
		if( r_ssgiDebug.GetInteger() > 0 )
		{
			// replace current
			GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS );
		}
		else
		{
			// add result to main color
			GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS );
		}
		
		if( hdrIsActive )
		{
			globalFramebuffers.hdrFBO->Bind();
		}
		else
		{
			Framebuffer::Unbind();
		}
		
		renderProgManager.BindShader_DeepGBufferRadiosity();
	}
	
	float screenCorrectionParm[4];
	screenCorrectionParm[0] = 1.0f / screenWidth;
	screenCorrectionParm[1] = 1.0f / screenHeight;
	screenCorrectionParm[2] = screenWidth;
	screenCorrectionParm[3] = screenHeight;
	SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
	
#if 0
	// RB: set unprojection matrices so we can convert zbuffer values back to camera and world spaces
	idRenderMatrix modelViewMatrix;
	idRenderMatrix::Transpose( *( idRenderMatrix* )backEnd.viewDef->worldSpace.modelViewMatrix, modelViewMatrix );
	idRenderMatrix cameraToWorldMatrix;
	if( !idRenderMatrix::Inverse( modelViewMatrix, cameraToWorldMatrix ) )
	{
		idLib::Warning( "cameraToWorldMatrix invert failed" );
	}
	
	SetVertexParms( RENDERPARM_MODELMATRIX_X, cameraToWorldMatrix[0], 4 );
	//SetVertexParms( RENDERPARM_MODELMATRIX_X, viewDef->unprojectionToWorldRenderMatrix[0], 4 );
#endif
	SetVertexParms( RENDERPARM_MODELMATRIX_X, viewDef->unprojectionToCameraRenderMatrix[0], 4 );
	
	
	float jitterTexOffset[4];
	if( r_shadowMapRandomizeJitter.GetBool() )
	{
		jitterTexOffset[0] = ( rand() & 255 ) / 255.0;
		jitterTexOffset[1] = ( rand() & 255 ) / 255.0;
	}
	else
	{
		jitterTexOffset[0] = 0;
		jitterTexOffset[1] = 0;
	}
	jitterTexOffset[2] = viewDef->renderView.time[0] * 0.001f;
	jitterTexOffset[3] = 0.0f;
	SetFragmentParm( RENDERPARM_JITTERTEXOFFSET, jitterTexOffset ); // rpJitterTexOffset
	
	GL_SelectTexture( 0 );
	globalImages->currentNormalsImage->Bind();
	
	GL_SelectTexture( 1 );
	if( r_useHierarchicalDepthBuffer.GetBool() )
	{
		globalImages->hierarchicalZbufferImage->Bind();
	}
	else
	{
		globalImages->currentDepthImage->Bind();
	}
	
	GL_SelectTexture( 2 );
	if( hdrIsActive )
	{
		globalImages->currentRenderHDRImage->Bind();
	}
	else
	{
		globalImages->currentRenderImage->Bind();
	}
	
	DrawElementsWithCounters( &unitSquareSurface );
	
	if( r_ssgiFiltering.GetBool() )
	{
		float jitterTexScale[4];
		
		// AO blur X
#if 1
		globalFramebuffers.ambientOcclusionFBO[1]->Bind();
		
		renderProgManager.BindShader_DeepGBufferRadiosityBlur();
		
		// set axis parameter
		jitterTexScale[0] = 1;
		jitterTexScale[1] = 0;
		jitterTexScale[2] = 0;
		jitterTexScale[3] = 0;
		SetFragmentParm( RENDERPARM_JITTERTEXSCALE, jitterTexScale ); // rpJitterTexScale
		
		GL_SelectTexture( 2 );
		globalImages->ambientOcclusionImage[0]->Bind();
		
		DrawElementsWithCounters( &unitSquareSurface );
#endif
		
		// AO blur Y
		if( hdrIsActive )
		{
			globalFramebuffers.hdrFBO->Bind();
		}
		else
		{
			Framebuffer::Unbind();
		}
		
		if( r_ssgiDebug.GetInteger() > 0 )
		{
			// replace current
			GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS );
		}
		else
		{
			// add result to main color
			GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS );
		}
		
		renderProgManager.BindShader_DeepGBufferRadiosityBlurAndOutput();
		
		// set axis parameter
		jitterTexScale[0] = 0;
		jitterTexScale[1] = 1;
		jitterTexScale[2] = 0;
		jitterTexScale[3] = 0;
		SetFragmentParm( RENDERPARM_JITTERTEXSCALE, jitterTexScale ); // rpJitterTexScale
		
		GL_SelectTexture( 2 );
		globalImages->ambientOcclusionImage[1]->Bind();
		
		DrawElementsWithCounters( &unitSquareSurface );
	}
	
	renderProgManager.Unbind();
	
	GL_State( GLS_DEFAULT );
	
	//GL_CheckErrors();
#endif
}
// RB end

/*
=========================================================================================================

BACKEND COMMANDS

=========================================================================================================
*/


/*
====================
RB_ExecuteBackEndCommands

This function will be called syncronously if running without
smp extensions, or asyncronously by another thread.
====================
*/
void idRenderBackend::ExecuteBackEndCommands( const emptyCommand_t* cmds )
{
	// r_debugRenderToTexture
	int c_draw3d = 0;
	int c_draw2d = 0;
	int c_setBuffers = 0;
	int c_copyRenders = 0;
	
	resolutionScale.SetCurrentGPUFrameTime( commonLocal.GetRendererGPUMicroseconds() );
	
	ResizeImages();
	
	renderLog.StartFrame();
	GL_StartFrame();
	
	if( cmds->commandId == RC_NOP && !cmds->next )
	{
		return;
	}
	
	if( renderSystem->GetStereo3DMode() != STEREO3D_OFF )
	{
		StereoRenderExecuteBackEndCommands( cmds );
		renderLog.EndFrame();
		return;
	}
	
	uint64 backEndStartTime = Sys_Microseconds();
	
	// needed for editor rendering
	GL_SetDefaultState();
	
	for( ; cmds != NULL; cmds = ( const emptyCommand_t* )cmds->next )
	{
		switch( cmds->commandId )
		{
			case RC_NOP:
				break;
				
			case RC_DRAW_VIEW_3D:
			case RC_DRAW_VIEW_GUI:
				DrawView( cmds, 0 );
				if( ( ( const drawSurfsCommand_t* )cmds )->viewDef->viewEntitys )
				{
					c_draw3d++;
				}
				else
				{
					c_draw2d++;
				}
				break;
				
			case RC_SET_BUFFER:
				SetBuffer( cmds );
				c_setBuffers++;
				break;
				
			case RC_COPY_RENDER:
				CopyRender( cmds );
				c_copyRenders++;
				break;
				
			case RC_POST_PROCESS:
				PostProcess( cmds );
				break;
				
			default:
				common->Error( "RB_ExecuteBackEndCommands: bad commandId" );
				break;
		}
	}
	
	DrawFlickerBox();
	
	GL_EndFrame();
	
	// stop rendering on this thread
	uint64 backEndFinishTime = Sys_Microseconds();
	pc.totalMicroSec = backEndFinishTime - backEndStartTime;
	
	if( r_debugRenderToTexture.GetInteger() == 1 )
	{
		common->Printf( "3d: %i, 2d: %i, SetBuf: %i, CpyRenders: %i, CpyFrameBuf: %i\n", c_draw3d, c_draw2d, c_setBuffers, c_copyRenders, pc.c_copyFrameBuffer );
		pc.c_copyFrameBuffer = 0;
	}
	
	renderLog.EndFrame();
}


/*
==================
idRenderBackend::DrawViewInternal
==================
*/
void idRenderBackend::DrawViewInternal( const viewDef_t* _viewDef, const int stereoEye )
{
	renderLog.OpenBlock( "RB_DrawViewInternal" );
	
	//-------------------------------------------------
	// guis can wind up referencing purged images that need to be loaded.
	// this used to be in the gui emit code, but now that it can be running
	// in a separate thread, it must not try to load images, so do it here.
	//-------------------------------------------------
	drawSurf_t** drawSurfs = ( drawSurf_t** )&_viewDef->drawSurfs[0];
	const int numDrawSurfs = _viewDef->numDrawSurfs;
	
	for( int i = 0; i < numDrawSurfs; i++ )
	{
		const drawSurf_t* ds = _viewDef->drawSurfs[ i ];
		if( ds->material != NULL )
		{
			const_cast<idMaterial*>( ds->material )->EnsureNotPurged();
		}
	}
	
	//-------------------------------------------------
	// RB_BeginDrawingView
	//
	// Any mirrored or portaled views have already been drawn, so prepare
	// to actually render the visible surfaces for this view
	//
	// clear the z buffer, set the projection matrix, etc
	//-------------------------------------------------
	ResetViewportAndScissorToDefaultCamera( _viewDef );
	
	// ensures that depth writes are enabled for the depth clear
	GL_State( GLS_DEFAULT | GLS_CULL_FRONTSIDED, true );
	
	//GL_CheckErrors();
	
	// RB begin
	bool useHDR = r_useHDR.GetBool() && !_viewDef->is2Dgui;
	
	// Clear the depth buffer and clear the stencil to 128 for stencil shadows as well as gui masking
	GL_Clear( false, true, true, STENCIL_SHADOW_TEST_VALUE, 0.0f, 0.0f, 0.0f, 0.0f, useHDR );
	
	if( useHDR )
	{
		globalFramebuffers.hdrFBO->Bind();
	}
	else
	{
		Framebuffer::Unbind();
	}
	// RB end
	
	//GL_CheckErrors();
	
#if !defined(USE_VULKAN)
	// bind one global Vertex Array Object (VAO)
	glBindVertexArray( glConfig.global_vao );
#endif
	
	//------------------------------------
	// sets variables that can be used by all programs
	//------------------------------------
	{
		//
		// set eye position in global space
		//
		float parm[4];
		parm[0] = viewDef->renderView.vieworg[0];
		parm[1] = viewDef->renderView.vieworg[1];
		parm[2] = viewDef->renderView.vieworg[2];
		parm[3] = 1.0f;
		
		SetVertexParm( RENDERPARM_GLOBALEYEPOS, parm ); // rpGlobalEyePos
		
		// sets overbright to make world brighter
		// This value is baked into the specularScale and diffuseScale values so
		// the interaction programs don't need to perform the extra multiply,
		// but any other renderprogs that want to obey the brightness value
		// can reference this.
		float overbright = r_lightScale.GetFloat() * 0.5f;
		parm[0] = overbright;
		parm[1] = overbright;
		parm[2] = overbright;
		parm[3] = overbright;
		SetFragmentParm( RENDERPARM_OVERBRIGHT, parm );
		
		// Set Projection Matrix
		float projMatrixTranspose[16];
		R_MatrixTranspose( viewDef->projectionMatrix, projMatrixTranspose );
		SetVertexParms( RENDERPARM_PROJMATRIX_X, projMatrixTranspose, 4 );
	}
	
	//-------------------------------------------------
	// fill the depth buffer and clear color buffer to black except on subviews
	//-------------------------------------------------
	FillDepthBufferFast( drawSurfs, numDrawSurfs );
	
	//-------------------------------------------------
	// FIXME, OPTIMIZE: merge this with FillDepthBufferFast like in a light prepass deferred renderer
	//
	// fill the geometric buffer with normals and roughness
	//-------------------------------------------------
	AmbientPass( drawSurfs, numDrawSurfs, true );
	
	//-------------------------------------------------
	// fill the depth buffer and the color buffer with precomputed Q3A style lighting
	//-------------------------------------------------
	AmbientPass( drawSurfs, numDrawSurfs, false );
	
	//-------------------------------------------------
	// main light renderer
	//-------------------------------------------------
	DrawInteractions( _viewDef );
	
	//-------------------------------------------------
	// capture the depth for the motion blur before rendering any post process surfaces that may contribute to the depth
	//-------------------------------------------------
	if( ( r_motionBlur.GetInteger() > 0 ||  r_useSSAO.GetBool() || r_useSSGI.GetBool() ) && !r_useHDR.GetBool() )
	{
		const idScreenRect& viewport = viewDef->viewport;
		globalImages->currentDepthImage->CopyDepthbuffer( viewport.x1, viewport.y1, viewport.GetWidth(), viewport.GetHeight() );
	}
	
	//-------------------------------------------------
	// darken the scene using the screen space ambient occlusion
	//-------------------------------------------------
	DrawScreenSpaceAmbientOcclusion( _viewDef );
	//RB_SSGI( _viewDef );
	
	//-------------------------------------------------
	// now draw any non-light dependent shading passes
	//-------------------------------------------------
	int processed = 0;
	if( !r_skipShaderPasses.GetBool() )
	{
		renderLog.OpenMainBlock( MRB_DRAW_SHADER_PASSES );
		float guiScreenOffset;
		if( _viewDef->viewEntitys != NULL )
		{
			// guiScreenOffset will be 0 in non-gui views
			guiScreenOffset = 0.0f;
		}
		else
		{
			guiScreenOffset = stereoEye * _viewDef->renderView.stereoScreenSeparation;
		}
		processed = DrawShaderPasses( drawSurfs, numDrawSurfs, guiScreenOffset, stereoEye );
		renderLog.CloseMainBlock();
	}
	
	//-------------------------------------------------
	// use direct light and emissive light contributions to add indirect screen space light
	//-------------------------------------------------
	//RB_SSGI( viewDef );
	
	//-------------------------------------------------
	// fog and blend lights, drawn after emissive surfaces
	// so they are properly dimmed down
	//-------------------------------------------------
	FogAllLights();
	
	//-------------------------------------------------
	// now draw any screen warping post-process effects using _currentRender
	//-------------------------------------------------
	if( processed < numDrawSurfs && !r_skipPostProcess.GetBool() )
	{
		int x = viewDef->viewport.x1;
		int y = viewDef->viewport.y1;
		int	w = viewDef->viewport.x2 - viewDef->viewport.x1 + 1;
		int	h = viewDef->viewport.y2 - viewDef->viewport.y1 + 1;
		
		RENDERLOG_PRINTF( "Resolve to %i x %i buffer\n", w, h );
		
		GL_SelectTexture( 0 );
		
		// resolve the screen
		globalImages->currentRenderImage->CopyFramebuffer( x, y, w, h );
		currentRenderCopied = true;
		
		// RENDERPARM_SCREENCORRECTIONFACTOR amd RENDERPARM_WINDOWCOORD overlap
		// diffuseScale and specularScale
		
		// screen power of two correction factor (no longer relevant now)
		float screenCorrectionParm[4];
		screenCorrectionParm[0] = 1.0f;
		screenCorrectionParm[1] = 1.0f;
		screenCorrectionParm[2] = 0.0f;
		screenCorrectionParm[3] = 1.0f;
		SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
		
		// window coord to 0.0 to 1.0 conversion
		float windowCoordParm[4];
		windowCoordParm[0] = 1.0f / w;
		windowCoordParm[1] = 1.0f / h;
		windowCoordParm[2] = 0.0f;
		windowCoordParm[3] = 1.0f;
		SetFragmentParm( RENDERPARM_WINDOWCOORD, windowCoordParm ); // rpWindowCoord
		
		// render the remaining surfaces
		renderLog.OpenMainBlock( MRB_DRAW_SHADER_PASSES_POST );
		DrawShaderPasses( drawSurfs + processed, numDrawSurfs - processed, 0.0f /* definitely not a gui */, stereoEye );
		renderLog.CloseMainBlock();
	}
	
	//-------------------------------------------------
	// render debug tools
	//-------------------------------------------------
	DBG_RenderDebugTools( drawSurfs, numDrawSurfs );
	
#if !defined(USE_VULKAN)
	// RB: convert back from HDR to LDR range
	if( useHDR )
	{
		/*
		int x = backEnd.viewDef->viewport.x1;
		int y = backEnd.viewDef->viewport.y1;
		int	w = backEnd.viewDef->viewport.x2 - backEnd.viewDef->viewport.x1 + 1;
		int	h = backEnd.viewDef->viewport.y2 - backEnd.viewDef->viewport.y1 + 1;
		
		GL_Viewport( viewDef->viewport.x1,
				viewDef->viewport.y1,
				viewDef->viewport.x2 + 1 - viewDef->viewport.x1,
				viewDef->viewport.y2 + 1 - viewDef->viewport.y1 );
		*/
		
		/*
		glBindFramebuffer( GL_READ_FRAMEBUFFER, globalFramebuffers.hdrFBO->GetFramebuffer() );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, globalFramebuffers.hdrQuarterFBO->GetFramebuffer() );
		glBlitFramebuffer( 0, 0, renderSystem->GetWidth(), renderSystem->GetHeight(),
						   0, 0, renderSystem->GetWidth() * 0.25f, renderSystem->GetHeight() * 0.25f,
						   GL_COLOR_BUFFER_BIT,
						   GL_LINEAR );
		*/
		
#if defined(USE_HDR_MSAA)
		if( glConfig.multisamples > 0 )
		{
			glBindFramebuffer( GL_READ_FRAMEBUFFER, globalFramebuffers.hdrFBO->GetFramebuffer() );
			glBindFramebuffer( GL_DRAW_FRAMEBUFFER, globalFramebuffers.hdrNonMSAAFBO->GetFramebuffer() );
			glBlitFramebuffer( 0, 0, renderSystem->GetWidth(), renderSystem->GetHeight(),
							   0, 0, renderSystem->GetWidth(), renderSystem->GetHeight(),
							   GL_COLOR_BUFFER_BIT,
							   GL_LINEAR );
							   
			// TODO resolve to 1x1
			glBindFramebuffer( GL_READ_FRAMEBUFFER_EXT, globalFramebuffers.hdrNonMSAAFBO->GetFramebuffer() );
			glBindFramebuffer( GL_DRAW_FRAMEBUFFER_EXT, globalFramebuffers.hdr64FBO->GetFramebuffer() );
			glBlitFramebuffer( 0, 0, renderSystem->GetWidth(), renderSystem->GetHeight(),
							   0, 0, 64, 64,
							   GL_COLOR_BUFFER_BIT,
							   GL_LINEAR );
		}
		else
#endif
		{
			if (glConfig.directStateAccess) {
				glBlitNamedFramebuffer(globalFramebuffers.hdrFBO->GetFramebuffer(), globalFramebuffers.hdr64FBO->GetFramebuffer(),
					0, 0, renderSystem->GetWidth(), renderSystem->GetHeight(),
					0, 0, 64, 64,
					GL_COLOR_BUFFER_BIT,
					GL_LINEAR);
			}
			else {
				glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, globalFramebuffers.hdrFBO->GetFramebuffer());
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, globalFramebuffers.hdr64FBO->GetFramebuffer());
				glBlitFramebuffer(0, 0, renderSystem->GetWidth(), renderSystem->GetHeight(),
					0, 0, 64, 64,
					GL_COLOR_BUFFER_BIT,
					GL_LINEAR);
			}
		}
		
		CalculateAutomaticExposure();
		
		Tonemap( _viewDef );
	}
	
	Bloom( _viewDef );
#endif
	
	renderLog.CloseBlock();
}

/*
==================
RB_MotionBlur

Experimental feature
==================
*/
void idRenderBackend::MotionBlur()
{
	if( !viewDef->viewEntitys )
	{
		// 3D views only
		return;
	}
	if( r_motionBlur.GetInteger() <= 0 )
	{
		return;
	}
	if( viewDef->isSubview )
	{
		return;
	}
	
	//GL_CheckErrors();
	
	
	// clear the alpha buffer and draw only the hands + weapon into it so
	// we can avoid blurring them
	GL_State( GLS_COLORMASK | GLS_DEPTHMASK );
	
// FIXME
#if !defined(USE_VULKAN)
	glClearColor( 0, 0, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	
#endif
	
	GL_Color( 0, 0, 0, 0 );
	GL_SelectTexture( 0 );
	globalImages->blackImage->Bind();
	currentSpace = NULL;
	
	drawSurf_t** drawSurfs = ( drawSurf_t** )&viewDef->drawSurfs[0];
	for( int surfNum = 0; surfNum < viewDef->numDrawSurfs; surfNum++ )
	{
		const drawSurf_t* surf = drawSurfs[ surfNum ];
		
		if( !surf->space->weaponDepthHack && !surf->space->skipMotionBlur && !surf->material->HasSubview() )
		{
			// Apply motion blur to this object
			continue;
		}
		
		const idMaterial* shader = surf->material;
		if( shader->Coverage() == MC_TRANSLUCENT )
		{
			// muzzle flash, etc
			continue;
		}
		
		// set mvp matrix
		if( surf->space != currentSpace )
		{
			RB_SetMVP( surf->space->mvp );
			currentSpace = surf->space;
		}
		
		// this could just be a color, but we don't have a skinned color-only prog
		if( surf->jointCache )
		{
			renderProgManager.BindShader_TextureVertexColorSkinned();
		}
		else
		{
			renderProgManager.BindShader_TextureVertexColor();
		}
		
		// draw it solid
		DrawElementsWithCounters( surf );
	}
	
	GL_State( GLS_DEPTHFUNC_ALWAYS );
	
	// copy off the color buffer and the depth buffer for the motion blur prog
	// we use the viewport dimensions for copying the buffers in case resolution scaling is enabled.
	const idScreenRect& viewport = viewDef->viewport;
	globalImages->currentRenderImage->CopyFramebuffer( viewport.x1, viewport.y1, viewport.GetWidth(), viewport.GetHeight() );
	
	// in stereo rendering, each eye needs to get a separate previous frame mvp
	int mvpIndex = ( viewDef->renderView.viewEyeBuffer == 1 ) ? 1 : 0;
	
	// derive the matrix to go from current pixels to previous frame pixels
	idRenderMatrix	inverseMVP;
	idRenderMatrix::Inverse( viewDef->worldSpace.mvp, inverseMVP );
	
	idRenderMatrix	motionMatrix;
	idRenderMatrix::Multiply( prevMVP[mvpIndex], inverseMVP, motionMatrix );
	
	prevMVP[mvpIndex] = viewDef->worldSpace.mvp;
	
	RB_SetMVP( motionMatrix );
	
	GL_State( GLS_DEPTHFUNC_ALWAYS | GLS_CULL_TWOSIDED );
	
	renderProgManager.BindShader_MotionBlur();
	
	// let the fragment program know how many samples we are going to use
	idVec4 samples( ( float )( 1 << r_motionBlur.GetInteger() ) );
	SetFragmentParm( RENDERPARM_OVERBRIGHT, samples.ToFloatPtr() );
	
	GL_SelectTexture( 0 );
	globalImages->currentRenderImage->Bind();
	GL_SelectTexture( 1 );
	globalImages->currentDepthImage->Bind();
	
	DrawElementsWithCounters( &unitSquareSurface );
	//GL_CheckErrors();
}

/*
==================
idRenderBackend::DrawView

StereoEye will always be 0 in mono modes, or -1 / 1 in stereo modes.
If the view is a GUI view that is repeated for both eyes, the viewDef.stereoEye value
is 0, so the stereoEye parameter is not always the same as that.
==================
*/
void idRenderBackend::DrawView( const void* data, const int stereoEye )
{
	const drawSurfsCommand_t* cmd = ( const drawSurfsCommand_t* )data;
	
	viewDef = cmd->viewDef;
	
	// we will need to do a new copyTexSubImage of the screen
	// when a SS_POST_PROCESS material is used
	currentRenderCopied = false;
	
	// if there aren't any drawsurfs, do nothing
	if( !viewDef->numDrawSurfs )
	{
		return;
	}
	
	// skip render bypasses everything that has models, assuming
	// them to be 3D views, but leaves 2D rendering visible
	if( r_skipRender.GetBool() && viewDef->viewEntitys )
	{
		return;
	}
	
	// skip render context sets the wgl context to NULL,
	// which should factor out the API cost, under the assumption
	// that all gl calls just return if the context isn't valid
	
	// RB: not really needed
	//if( r_skipRenderContext.GetBool() && backEnd.viewDef->viewEntitys )
	//{
	//	GLimp_DeactivateContext();
	//}
	// RB end
	
	pc.c_surfaces += viewDef->numDrawSurfs;
	
	DBG_ShowOverdraw();
	
	// render the scene
	DrawViewInternal( cmd->viewDef, stereoEye );
	
	MotionBlur();
	
	// restore the context for 2D drawing if we were stubbing it out
	// RB: not really needed
	//if( r_skipRenderContext.GetBool() && backEnd.viewDef->viewEntitys )
	//{
	//	GLimp_ActivateContext();
	//	GL_SetDefaultState();
	//}
	// RB end
	
	// optionally draw a box colored based on the eye number
	if( r_drawEyeColor.GetBool() )
	{
		const idScreenRect& r = viewDef->viewport;
		GL_Scissor( ( r.x1 + r.x2 ) / 2, ( r.y1 + r.y2 ) / 2, 32, 32 );
		switch( stereoEye )
		{
			case -1:
				GL_Clear( true, false, false, 0, 1.0f, 0.0f, 0.0f, 1.0f );
				break;
			case 1:
				GL_Clear( true, false, false, 0, 0.0f, 1.0f, 0.0f, 1.0f );
				break;
			default:
				GL_Clear( true, false, false, 0, 0.5f, 0.5f, 0.5f, 1.0f );
				break;
		}
	}
}

/*
==================
idRenderBackend::CopyRender

Copy part of the current framebuffer to an image
==================
*/
void idRenderBackend::CopyRender( const void* data )
{
	const copyRenderCommand_t* cmd = ( const copyRenderCommand_t* )data;
	
	if( r_skipCopyTexture.GetBool() )
	{
		return;
	}
	
	RENDERLOG_PRINTF( "***************** RB_CopyRender *****************\n" );
	
	if( cmd->image )
	{
		cmd->image->CopyFramebuffer( cmd->x, cmd->y, cmd->imageWidth, cmd->imageHeight );
	}
	
	if( cmd->clearColorAfterCopy )
	{
		GL_Clear( true, false, false, STENCIL_SHADOW_TEST_VALUE, 0, 0, 0, 0 );
	}
}

/*
==================
idRenderBackend::PostProcess

==================
*/
extern idCVar rs_enable;
void idRenderBackend::PostProcess( const void* data )
{
	// only do the post process step if resolution scaling is enabled. Prevents the unnecessary copying of the framebuffer and
	// corresponding full screen quad pass.
	if( rs_enable.GetInteger() == 0 && !r_useFilmicPostProcessEffects.GetBool() && r_antiAliasing.GetInteger() == 0 )
	{
		return;
	}
	
	if( ( r_ssaoDebug.GetInteger() > 0 ) || ( r_ssgiDebug.GetInteger() > 0 ) )
	{
		return;
	}
	
	RENDERLOG_PRINTF( "---------- RB_PostProcess() ----------\n" );
	
// FIXME
#if !defined(USE_VULKAN)

	// resolve the scaled rendering to a temporary texture
	postProcessCommand_t* cmd = ( postProcessCommand_t* )data;
	const idScreenRect& viewport = cmd->viewDef->viewport;
	
	GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_DEPTHMASK | GLS_DEPTHFUNC_ALWAYS |  GLS_CULL_TWOSIDED );
	
	int screenWidth = renderSystem->GetWidth();
	int screenHeight = renderSystem->GetHeight();
	
	// set the window clipping
	GL_Viewport( 0, 0, screenWidth, screenHeight );
	GL_Scissor( 0, 0, screenWidth, screenHeight );
	
	// SMAA
	int aaMode = r_antiAliasing.GetInteger();
	if( aaMode == ANTI_ALIASING_SMAA_1X )
	{
		/*
		 * The shader has three passes, chained together as follows:
		 *
		 *                           |input|------------------�
		 *                              v                     |
		 *                    [ SMAA*EdgeDetection ]          |
		 *                              v                     |
		 *                          |edgesTex|                |
		 *                              v                     |
		 *              [ SMAABlendingWeightCalculation ]     |
		 *                              v                     |
		 *                          |blendTex|                |
		 *                              v                     |
		 *                [ SMAANeighborhoodBlending ] <------�
		 *                              v
		 *                           |output|
		*/
		
		globalImages->smaaInputImage->CopyFramebuffer( 0, 0, screenWidth, screenHeight );
		
		// set SMAA_RT_METRICS = rpScreenCorrectionFactor
		float screenCorrectionParm[4];
		screenCorrectionParm[0] = 1.0f / screenWidth;
		screenCorrectionParm[1] = 1.0f / screenHeight;
		screenCorrectionParm[2] = screenWidth;
		screenCorrectionParm[3] = screenHeight;
		SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
		
		globalFramebuffers.smaaEdgesFBO->Bind();
		
		glClearColor( 0, 0, 0, 0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
		GL_SelectTexture( 0 );
		globalImages->smaaInputImage->Bind();
		
		renderProgManager.BindShader_SMAA_EdgeDetection();
		DrawElementsWithCounters( &unitSquareSurface );
		
#if 1
		//globalImages->smaaEdgesImage->CopyFramebuffer( viewport.x1, viewport.y1, viewport.GetWidth(), viewport.GetHeight() );
		
		globalFramebuffers.smaaBlendFBO->Bind();
		//Framebuffer::Unbind();
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		
		GL_SelectTexture( 0 );
		globalImages->smaaEdgesImage->Bind();
		
		GL_SelectTexture( 1 );
		globalImages->smaaAreaImage->Bind();
		
		GL_SelectTexture( 2 );
		globalImages->smaaSearchImage->Bind();
		
		renderProgManager.BindShader_SMAA_BlendingWeightCalculation();
		DrawElementsWithCounters( &unitSquareSurface );
		
		Framebuffer::Unbind();
#endif
		
#if 1
		//GL_SelectTexture( 0 );
		//globalImages->smaaBlendImage->CopyFramebuffer( viewport.x1, viewport.y1, viewport.GetWidth(), viewport.GetHeight() );
		
		GL_SelectTexture( 0 );
		globalImages->smaaInputImage->Bind();
		
		GL_SelectTexture( 1 );
		globalImages->smaaBlendImage->Bind();
		
		renderProgManager.BindShader_SMAA_NeighborhoodBlending();
		DrawElementsWithCounters( &unitSquareSurface );
#endif
	}
	
#if 1
	if( r_useFilmicPostProcessEffects.GetBool() )
	{
		globalImages->currentRenderImage->CopyFramebuffer( viewport.x1, viewport.y1, viewport.GetWidth(), viewport.GetHeight() );
		
		GL_SelectTexture( 0 );
		globalImages->currentRenderImage->Bind();
		
		GL_SelectTexture( 1 );
		globalImages->grainImage1->Bind();
		
		renderProgManager.BindShader_PostProcess();
		
		const static int GRAIN_SIZE = 128;
		
		// screen power of two correction factor
		float screenCorrectionParm[4];
		screenCorrectionParm[0] = 1.0f / GRAIN_SIZE;
		screenCorrectionParm[1] = 1.0f / GRAIN_SIZE;
		screenCorrectionParm[2] = 1.0f;
		screenCorrectionParm[3] = 1.0f;
		SetFragmentParm( RENDERPARM_SCREENCORRECTIONFACTOR, screenCorrectionParm ); // rpScreenCorrectionFactor
		
		float jitterTexOffset[4];
		if( r_shadowMapRandomizeJitter.GetBool() )
		{
			jitterTexOffset[0] = ( rand() & 255 ) / 255.0;
			jitterTexOffset[1] = ( rand() & 255 ) / 255.0;
		}
		else
		{
			jitterTexOffset[0] = 0;
			jitterTexOffset[1] = 0;
		}
		jitterTexOffset[2] = 0.0f;
		jitterTexOffset[3] = 0.0f;
		SetFragmentParm( RENDERPARM_JITTERTEXOFFSET, jitterTexOffset ); // rpJitterTexOffset
		
		// Draw
		DrawElementsWithCounters( &unitSquareSurface );
	}
#endif
	
	GL_SelectTexture( 0 );
	renderProgManager.Unbind();
	
#endif
	
	renderLog.CloseBlock();
}
