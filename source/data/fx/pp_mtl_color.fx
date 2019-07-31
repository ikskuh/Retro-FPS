// changes the colorintensity depending on vecValues1[0][1][2]
texture TargetMap;

float4 vecSkill1;

sampler postTex = sampler_state {
	
   texture = (TargetMap);
   MinFilter = linear;
   MagFilter = linear;
   MipFilter = linear;
   AddressU = Clamp;
   AddressV = Clamp;
   
};

float4 Colorintensity_PS( float2 Tex : TEXCOORD0 ) : COLOR0 {
	
	float4 Color;

	Color = tex2D(postTex, Tex.xy);

	Color.r = Color.r*vecSkill1.r;
	Color.g = Color.g*vecSkill1.g;
	Color.b = Color.b*vecSkill1.b;
	Color.a = vecSkill1.a;

	return(Color);
	
}

technique tech_00 {
	
	pass pass_00 {
		
		VertexShader = null;
		PixelShader = compile ps_2_0 Colorintensity_PS();
		
	}
	
}
