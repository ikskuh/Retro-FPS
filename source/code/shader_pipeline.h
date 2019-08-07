#ifndef _SHADER_PIPELINE_H_
#define _SHADER_PIPELINE_H_

MATERIAL *mtl_world = {

	effect = "mtl_world.fx";
	flags = PASS_SOLID | AUTORELOAD;
}

MATERIAL *mtl_object = {

	ambient_red = 200;
	ambient_green = 200;
	ambient_blue = 200;
	effect = "mtl_object.fx";
	flags = PASS_SOLID | AUTORELOAD;
}

MATERIAL *mtl_fade = {

	effect = "mtl_fade.fx";
	flags = TRANSLUCENT | AUTORELOAD;
}

MATERIAL *mtl_translucent = {

	effect = "mtl_translucent.fx";
	flags = TRANSLUCENT | AUTORELOAD;
}

MATERIAL *mtl_anim_sprite = {

	effect = "mtl_anim_sprite.fx";
	flags = TRANSLUCENT | AUTORELOAD;
}

MATERIAL *mtl_water = {

	effect = "mtl_water.fx";
}

MATERIAL *mtl_z_write = {

	ambient_red = 192;
	ambient_green = 192;
	ambient_blue = 192;
	effect = "

	technique z_write
	{
		pass p0
		{
			ZWriteEnable = True;
			AlphaBlendEnable = True;
			AlphaTestEnable = True;
		}
	}
	technique fallback
	{
		pass p0 {}
	}

	";
}

MATERIAL *mtl_two_side = {

	ambient_red = 192;
	ambient_green = 192;
	ambient_blue = 192;
	effect = "

	technique z_write
	{
		pass p0
		{
			CullMode = NONE;
		}
	}
	technique fallback
	{
		pass p0 {}
	}

	";
}

MATERIAL *pp_mtl_adjust = {

	effect = "pp_mtl_adjust.fx";
	flags = AUTORELOAD;
}

MATERIAL *pp_mtl_sharpen = {

	effect = "pp_mtl_sharpen.fx";
	flags = AUTORELOAD;
}

MATERIAL *pp_mtl_underwater = {

	effect = "pp_mtl_underwater.fx";
	flags = AUTORELOAD;
}

MATERIAL *pp_mtl_colorintensity = {

	effect = "pp_mtl_color.fx";
	flags = AUTORELOAD;
}

void pp_colorintensity_set_value(r, g, b);

VIEW *pp_add(MATERIAL *Material, VIEW *View, BMAP *bmap);

int pp_remove(MATERIAL *Material, VIEW *View, VIEW *StageView);

void pp_remove_all(VIEW *View);

VIEW *pp_last_view(VIEW *View);

void pipeline_adjust();

void pipeline_start();

void pipeline_update();

void pipeline_resolution();

#endif