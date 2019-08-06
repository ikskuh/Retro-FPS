
// image received from the previous view stage:
texture TargetMap;

sampler2D smpSource = sampler_state {
	texture = <TargetMap>;
	AddressU = Clamp;
	AddressV = Clamp;
};

float4 vecViewPort; // contains viewport pixel size in zw components
float4 vecSkill1;	// blurstrength, saturation, sepiastrength, darken
float4 vecSkill5;	// contrast, scratchspeed

float4 OldMoviePS( float2 Tex : TEXCOORD0 ) : COLOR0 {
	
	float4 Color = 0;

	// blur
	float BlurDist = 1;
	for (float i=-BlurDist; i<=BlurDist; i+=BlurDist)
	for (float j=-BlurDist; j<=BlurDist; j+=BlurDist)
	Color += tex2D(smpSource, Tex.xy + float2(i * vecViewPort.z, j * vecViewPort.w) * vecSkill1.x);
	Color.rgb /= 9;	
	
	// sepia
	float Helligkeit = dot(Color.rgb, float3(0.299f, 0.587f, 0.114f));
	Color.rgb = lerp(Helligkeit, Color.rgb, vecSkill1.y);
	Color.rgb += float3(0.98, 0.588, 0) * vecSkill1.z;
	Color.rgb -= vecSkill1.w;
	Color.rgb *= vecSkill5.x;
	
	return Color;
}

technique OldMovie
{
	pass one
	{
		PixelShader = compile ps_2_0 OldMoviePS();
	}
}
