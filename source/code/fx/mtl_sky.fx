float4 vecViewPos;
float4 vecFog;
float4 vecFogColor;
float4 vecSkill1;

float4x4 matWorld;
float4x4 matWorldViewProj;

texture mtlSkin1;
samplerCUBE mtlSkin1SamplerCUBE = sampler_state { Texture = <mtlSkin1>; Mipfilter = NONE; Minfilter = NONE; Magfilter = NONE; };

void SkyVS (
in float4 inPos: POSITION,
out float4 outPos: POSITION,
out float4 outWorld: TEXCOORD1 )
{
	outPos = mul( inPos, matWorldViewProj );
	outWorld = mul( inPos, matWorld );
}


float4 SkyPS (
in float4 inWorld: TEXCOORD1 ) : COLOR0
{
	float3 Dir = normalize ( inWorld.xyz - vecViewPos.xyz );
	float4 Color0 = texCUBE ( mtlSkin1SamplerCUBE, Dir );
	Color0.a = 1.0f;
	
	float Fog = 1.0f - pow ( Dir.y, vecSkill1.x );
	Color0.rgb = lerp ( Color0.rgb, vecFogColor.rgb, Fog );
	
	return Color0;
}



technique Sky
{
	pass p0
	{
		CullMode = None;
		
		VertexShader = compile vs_3_0 SkyVS();
		PixelShader  = compile ps_3_0 SkyPS();
	}

}