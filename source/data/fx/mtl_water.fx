//enable: Mirror Fresnel Effect
//help: Water transparency depends on view angle
//id: 1
#define MIRROR_FRESNEL

//enable: Enviroment Fresnel Effect
//help: Water transparency depends on view angle
//id: 11
#define ENVIRO_FRESNEL

//enable: Blinn Shading
//help: Enable for Blinn (better), disable for Phong (faster)
//id: 15
#define BLINN

//enable: Bumpmapping Light Falloff
//help: Enable for light angle modulation at bumpmapped surfaces
//id: 16
#define BUMPFALLOFF

//enable: DX Lighting
//help: Enable for DirectX falloff, disable for 1/r lighting 
//id: 5
//#define DXLIGHTING

struct vertexIn
{
	float4 Pos		: POSITION;
	float4 Ambient: COLOR0;
	float3 Normal:  NORMAL; // expected to be normalized
	float2 Tex1:    TEXCOORD0;
	float2 Tex2:    TEXCOORD1;
	float3 Tangent: TEXCOORD2; // pre-normalized
};

float4x4 matWorldViewProj;
float4 DoTransform(float4 Pos)
{
	return mul(Pos,matWorldViewProj);
}

float4 vecFog;
float4 vecViewPos;
#ifndef MATWORLDVIEW
	float4x4 matWorldView;
	#define MATWORLDVIEW
#endif
#ifndef MATWORLD
	float4x4 matWorld;
	#define MATWORLD
#endif

#ifdef DXFOG
	float DoFog(float4 Pos)
	{
		float3 P = mul(Pos,matWorldView).xyz; // convert vector to view space to get it's depth (.z)
		return saturate((vecFog.y-P.z) * vecFog.z); // apply the linear fog formula
	}
	#else // distance based fog
	float DoFog(float4 Pos)
	{
		float3 P = mul(Pos,matWorld).xyz;
		return 1 - (distance(P,vecViewPos.xyz)-vecFog.x) * vecFog.z;
	}
#endif

#ifndef MATWORLD
	float4x4 matWorld;
	#define MATWORLD
#endif
float4 DoPos(float4 Pos)
{
	return (mul(Pos,matWorld));
}
float3 DoPos(float3 Pos)
{
	return (mul(Pos,(float3x3)matWorld));
}

#ifndef MATWORLD
	float4x4 matWorld;
	#define MATWORLD
#endif

// only rotation and translation => inv(transpose(matWorld)) == matWorld
float3 DoNormal(float3 inNormal)
{
	return normalize(mul(inNormal,(float3x3)matWorld));
}

// reconstruct a compressed normal
float3 CreateNormal(float2 inNormalXY)
{
	float3 n;
	n.xy = inNormalXY * 2 - 1;
	n.z  = sqrt(1.0 - dot(n.xy, n.xy));
	return n;
}

int iLights;
float4 vecLightPos[8];	 // light positions (xyz) and ranges (w)
float4 vecLightColor[8]; // light colors

// calculate the light attenuation factor
float DoLightFactor(float4 Light,float3 Pos)
{
	float fac = 0.f;
	if (Light.w > 0.f) {    
		float LD = length(Light.xyz-Pos)/Light.w;
		#ifdef DXLIGHTING // DX falloff formula
			if (LD < 1.0f)
			fac = saturate(1.f/(0.f + 0.f*LD + 1.5f*LD*LD));	
			#else  // Acknex formula, linear lighting
			if (LD < 1.f)
			fac = saturate(1.f - LD);
		#endif  
	}
	return fac; // get the distance factor
}

// calculate the light attenuation factor on the front side
float DoLightFactorBump(float4 Light,float3 P,float3 N)
{
	#ifdef BUMPFALLOFF
		float3 D = Light.xyz-P; // ray pointing from the light to the surface
		float NdotL = dot(N,normalize(D));   // angle between surface and light ray
		
		if (NdotL >= 0.f) 
		return saturate(NdotL*8)*DoLightFactor(Light,P);
		else
		return 0.f;
		#else
		return DoLightFactor(Light,P);
	#endif
}

float DoLightFactorN(float4 Light,float3 P,float3 N)
{
	float3 D = Light.xyz-P; // ray pointing from the light to the surface
	float NdotL = dot(N,normalize(D));   // angle between surface and light ray
	
	if (NdotL >= 0.f) 
	return 2 * NdotL * DoLightFactor(Light,P);
	else
	return 0.f;
}

float4 DoPointLight(float3 P, float3 N, float4 Light, float4 LightColor)
{
	return LightColor * DoLightFactorN(Light,P,N);
}

float4 DoLight(float3 P, float3 N, int i)
{
	return DoPointLight(P,N,vecLightPos[i],vecLightColor[i]);
}

float4 vecSkill41;

float DoDefault(float vecSkill,float defVal)
{
	if (0 == vecSkill) 
	return defVal;
	else
	return vecSkill;
}

float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float fPower;

float4 DoAmbient()
{
	return (vecAmbient * vecLight) + float4(vecEmissive.xyz*vecColor.xyz,vecLight.w);	
}

float DoSpecular()
{
	return (vecSpecular.x+vecSpecular.y+vecSpecular.z)*0.333;	
}

float4 DoLightmap(float3 Diffuse,float3 Lightmap,float4 Ambient)
{
	return float4(Diffuse+Lightmap*(Diffuse+Ambient.xyz),Ambient.w);
}

float4 DoColor(float3 Diffuse,float4 Ambient)
{
	return float4(Diffuse,Ambient.w) + Ambient;
}

float4 vecTime;

struct turbOut
{
	float4 Pos: POSITION;
	float  Fog:	FOG;
	float4 Color:   COLOR0;
	float2 Tex: TEXCOORD0;
	float2 LM:  TEXCOORD1;
};

turbOut turb_VS (
in float4 inPos: 	POSITION, 
in float3 inNormal:	NORMAL,
in float2 inTex: 	TEXCOORD0,
in float2 inLM: 	TEXCOORD1
)
{
	turbOut Out;
	Out.Pos	= DoTransform(inPos);
	float fCycle = vecTime.w*DoDefault(vecSkill41.y*(0.1/50),0.1);
	float fAmp = DoDefault(vecSkill41.x*(0.005/50),0.005);
	float fMod = 1+DoDefault(vecSkill41.z*(25./50),25);
	Out.Tex.x = inTex.x + fAmp*sin(inTex.y*fMod + fCycle);
	Out.Tex.y = inTex.y - fAmp*sin(inTex.x*fMod + fCycle);
	Out.LM	= inLM;
	Out.Fog	= DoFog(inPos);

	float3 P = DoPos(inPos);
	float3 N = DoNormal(inNormal);

	float3 Color = 0; 
	for (int i=0; i<8; i++)  // Add 8 dynamic lights
	Color += DoLight(P,N,i);

	Out.Color = 0.5*DoAmbient() + 0.5*float4(Color,1)*vecDiffuse;

	return Out;		
}


technique turb
{
	pass one
	{		
		CullMode=none;
		VertexShader = compile vs_2_0 turb_VS();
	}
}

technique fallback { pass one { } }
