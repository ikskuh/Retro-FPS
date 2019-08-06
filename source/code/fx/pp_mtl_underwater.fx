
// image received from the previous view stage:
texture TargetMap;

sampler2D smpSource = sampler_state {
	
	texture = <TargetMap>;
	AddressU = Clamp;
	AddressV = Clamp;
	
};

float4 vecTime;

float4 under_water_ps( float2 Tex : TEXCOORD0 ) : COLOR0 {
	
	float4 Color = tex2D(smpSource, Tex + float2(0.0f, 0.002f * sin(vecTime.w * 0.2f + Tex.x * 25.0f)));
	//float4 Color = tex2D(smpSource, Tex + float2(0.0f, 0.002f * sin(vecTime.w * 0.2f + Tex.x * 100.0f)));
	
	return Color;
}

technique under_water {
	
	pass one{
		
		PixelShader = compile ps_2_0 under_water_ps();
		
	}
}
