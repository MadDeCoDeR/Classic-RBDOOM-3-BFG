/*
   RetroArch's dot Shader Port
   Original Author: Themaister
   License: Public domain
*/

#include "global.inc.hlsl"


struct VS_IN {
   float4 Position: POSITION;
   float2 TexCoord: TEXCOORD0;
   float4 color : COLOR0;
};

struct VS_OUT {
   float4 position : POSITION;
   float2 vTexCoord: TEXCOORD0;
   float4 c00_10: TEXCOORD1;
   float4 c20_01: TEXCOORD2;
   float4 c21_02: TEXCOORD3;
   float4 c12_22: TEXCOORD4;
   float2 c11: TEXCOORD5;
   float2 pixel_no: TEXCOORD6;
   float4 color : COLOR0;

};

void main( VS_IN vertex, out VS_OUT result )
{
   result.position.x = dot4( vertex.Position, rpMVPmatrixX );
	result.position.y = dot4( vertex.Position, rpMVPmatrixY );
	result.position.z = dot4( vertex.Position, rpMVPmatrixZ );
	result.position.w = dot4( vertex.Position, rpMVPmatrixW );

   float dx = rpWindowCoord.x;
   float dy = rpWindowCoord.y;

   result.c00_10 = float4(vertex.TexCoord + float2(-dx, -dy), vertex.TexCoord + float2(0, -dy));
   result.c20_01 = float4(vertex.TexCoord + float2(dx, -dy), vertex.TexCoord + float2(-dx, 0));
   result.c21_02 = float4(vertex.TexCoord + float2(dx, 0), vertex.TexCoord + float2(-dx, dy));
   result.c12_22 = float4(vertex.TexCoord + float2(0, dy), vertex.TexCoord + float2(dx, dy));
   result.c11 = vertex.TexCoord;
   result.pixel_no = vertex.TexCoord * rpWindowCoord.zw;
   result.color = swizzleColor(vertex.color);
}
