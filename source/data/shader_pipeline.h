#ifndef _SHADER_PIPELINE_H_
	#define _SHADER_PIPELINE_H_
	
	VIEW *TempView;
	
	var pp_underwater_is_on = false; // true - if underwater effect is on, otherwise - false
	
	var pp_blur = 0;
	var pp_saturation = 100;
	var pp_sepia = 5;
	var pp_darken = 8;
	var pp_contrast = 125;
	
	var pp_sharpen_strength = 2.5;
	
	MATERIAL *mtl_water = {
		
		effect = "mtl_water.fx";
		
	}
	
	MATERIAL *mtl_z_write = {
		
		ambient_red = 192;
		ambient_green = 192;
		ambient_blue = 192;
		effect = "
		
		technique z_write {
			pass p0 {
				ZWriteEnable = True;
				AlphaBlendEnable = True;
				AlphaTestEnable = True;
			}
		}
		technique fallback{
			pass p0{ }
		} 
		
		";
		
	}

	MATERIAL *mtl_two_side = {
		
		ambient_red = 192;
		ambient_green = 192;
		ambient_blue = 192;
		effect = "
		
		technique z_write {
			pass p0 {
				CullMode = NONE;
			}
		}
		technique fallback{
			pass p0{ }
		} 
		
		";
		
	}
	
	MATERIAL *pp_mtl_adjust = {
		
		effect = "pp_mtl_adjust.fx";
		flags = AUTORELOAD;
		
	}

	MATERIAL* pp_mtl_sharpen = {
		
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

	#include "shader_pipeline.c"
#endif