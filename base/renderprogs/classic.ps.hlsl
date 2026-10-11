/*
   RetroArch's dot Shader Port
   Original Author: Themaister
   License: Public domain
*/

#include "global.inc.hlsl"

uniform sampler2D samp0 : register(s0);

const float gamma = 2.4;
const float shine = 0.05;
const float blend = 0.65;

float dist(float2 coord, float2 source)
{
   float2 delta = coord - source;
   return sqrt(dot(delta, delta));
}

float color_bloom(float3 color)
{
   const float3 gray_coeff = float3(0.30, 0.59, 0.11);
   float bright = dot(color, gray_coeff);
   return lerp(1.0 + shine, 1.0 - shine, bright);
}

float3 lookup(float2 pixel_no, float offset_x, float offset_y, float3 color)
{
   float2 offset = float2(offset_x, offset_y);
   float delta = dist(frac(pixel_no), offset + float2(0.5, 0.5));
   return color * exp(-gamma * delta * color_bloom(color));
}

#define TEX(coord) tex2D(samp0, coord).rgb

float4 dot_effect(float4 c00_10, float4 c20_01, float4 c21_02, float4 c12_22, float2 c11, float2 pixel_no)
{
   float3 mid_color = lookup(pixel_no, 0.0, 0.0, TEX(c11));
   float3 color = float3(0.0, 0.0, 0.0);
   color += lookup(pixel_no, -1.0, -1.0, TEX(c00_10.xy));
   color += lookup(pixel_no,  0.0, -1.0, TEX(c00_10.zw));
   color += lookup(pixel_no,  1.0, -1.0, TEX(c20_01.xy));
   color += lookup(pixel_no, -1.0,  0.0, TEX(c20_01.zw));
   color += mid_color;
   color += lookup(pixel_no,  1.0,  0.0, TEX(c21_02.xy));
   color += lookup(pixel_no, -1.0,  1.0, TEX(c21_02.zw));
   color += lookup(pixel_no,  0.0,  1.0, TEX(c12_22.xy));
   color += lookup(pixel_no,  1.0,  1.0, TEX(c12_22.zw));
   float3 out_color = lerp(1.2 * mid_color, color, blend);

   return float4(out_color, 1.0f);
}

struct PS_IN {
   float4 position : VPOS;
   float2 vTexCoord: TEXCOORD0_centroid;
   float4 c00_10: TEXCOORD1;
   float4 c20_01: TEXCOORD2;
   float4 c21_02: TEXCOORD3;
   float4 c12_22: TEXCOORD4;
   float2 c11: TEXCOORD5;
   float2 pixel_no: TEXCOORD6;
   float4 color : COLOR0;
};

struct PS_OUT {
	float4 color : COLOR;
};

void main( PS_IN fragment, out PS_OUT result )
{
	result.color = sRGBAToLinearRGBA(fragment.color * dot_effect(fragment.c00_10, fragment.c20_01, fragment.c21_02, fragment.c12_22, fragment.c11, fragment.pixel_no));
}
