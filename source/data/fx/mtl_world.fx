
#define ANGLE_SURFACE_DARKEN 0.5

float4x4 matWorldViewProj;
float4x4 matWorld;
float4 vecViewPos;
float4 vecViewDir;
float4 vecAmbient;
float4 vecFog;
float4 vecFogColor;

float4 vecSkill1;

int iLights;
float4 vecLightColor[8];
float4 vecLightPos[8];

texture entSkin1;
texture entSkin2;

sampler ColorSampler = sampler_state { Texture = <entSkin1>; Mipfilter = NONE; Minfilter = NONE; Magfilter = NONE; };
sampler ShadowSampler = sampler_state { Texture = <entSkin2>; Mipfilter = Linear; Minfilter = Linear; Magfilter = Linear; };


void VS (
in float4 inposition : POSITION,
in float3 innormal : NORMAL,
in float4 intex1 : TEXCOORD0,
in float4 intex2 : TEXCOORD1,
out float4 outposition : POSITION,
out float3 outnormal : TEXCOORD0,
out float4 outtex : TEXCOORD1,
out float4 outworldPos : TEXCOORD2 )
{
	inposition.w = 1.0f;
	outposition = mul ( inposition, matWorldViewProj );
	outnormal = normalize ( mul ( innormal, (float3x3)matWorld ) );
	outtex.xy = intex1.xy;
	outtex.zw = intex2.xy;
	outworldPos = mul ( inposition, matWorld );
}

float4 PS (
float3 normal : TEXCOORD0,
float4 tex : TEXCOORD1,
float4 worldPos : TEXCOORD2 ) : COLOR0
{
	float4 textureColor = tex2D ( ColorSampler, tex.xy );
	
	float4 color = tex2D ( ShadowSampler, tex.zw );
	
	float fDepth = distance ( vecViewPos.xyz, worldPos.xyz );
	
	for ( int i=0; i<iLights; i++ )
	{
		float3 lightDir = normalize ( vecLightPos[i].xyz - worldPos.xyz );
		float Distance = distance ( vecLightPos[i].xyz, worldPos.xyz );
		float Radiance = saturate ( ( vecLightPos[i].w - Distance ) / vecLightPos[i].w );
		color.rgb += vecLightColor[i].rgb * saturate(dot(lightDir,normal)) * Radiance;
	}
	
	color.rgb = saturate ( color.rgb );
	color.rgb = pow ( color.rgb, vecSkill1.x );
	
	///////////////////////////////
	
	float3 vPixelToViewDir = normalize(vecViewPos.xyz - worldPos.xyz); // *** direction vector from the surface to the camera
	
	float dot_result = dot(vPixelToViewDir, normal.xyz); // *** get the angle ( cos(angle) ) between these vectors; both vectors in the dot product have to be normalized (length = 1)
	
	color.rgb *= saturate(1.0 - (1.0 - dot_result) * ANGLE_SURFACE_DARKEN); // *** apply the darkening factor with adjustable intensity; saturate() to prevent negative numbers (and numbers > 1)
	
	///////////////////////////////
	
	
	color.rgb *= textureColor.rgb;
	
	float Fog = saturate ( ( fDepth - vecFog.x ) * vecFog.z );
	color.rgb = lerp ( color.rgb, vecFogColor, Fog );
	
	color.a = 1;
	
	return color;
}

technique
{
	pass pass0
	{
		ZWriteEnable = True;
		AlphaBlendEnable = False;
		AlphaTestEnable = False;
		
		VertexShader = compile vs_3_0 VS(); 
		PixelShader  = compile ps_3_0 PS(); 
	}
}

