
void pp_colorintensity_set_value(r, g, b){
	
	pp_mtl_colorintensity->skill1 = floatv(r);
	pp_mtl_colorintensity->skill2 = floatv(g);
	pp_mtl_colorintensity->skill3 = floatv(b);
}

VIEW *pp_add(MATERIAL *Material, VIEW *View, BMAP *bmap){
	
	TempView = View;
	while(TempView->stage != NULL){ TempView = TempView->stage; }
	
	TempView->stage = view_create(0);
	set(TempView->stage, PROCESS_TARGET);
	TempView = TempView->stage;
	
	TempView->material = Material;
	if(bmap != NULL){ TempView->bmap = bmap; }
	return(TempView);
}

int pp_remove(MATERIAL *Material, VIEW *View, VIEW *StageView){
	
	TempView = View;
	while(TempView->material != Material && ((StageView == NULL) + (TempView->stage != NULL)) != 0){
		
		View = TempView;
		TempView = TempView->stage;		
		if(TempView == NULL){ return(true); }
	}
	
	View->stage = TempView->stage;
	View->bmap = TempView->bmap;
	TempView->bmap = NULL;
	ptr_remove(TempView);
	return(false);
}

void pp_remove_all(VIEW *View){
	
	if(View->stage == NULL){ return; }
	TempView = View->stage;
	View->stage = NULL;
	View->bmap = NULL;
	
	while(TempView->stage != NULL){
		
		View = TempView;
		TempView = View->stage;		
		View->stage = NULL;		
		View->bmap = NULL;		
		ptr_remove(View);
	}
}

VIEW *pp_last_view(VIEW *View){
	
	TempView = View;
	while(TempView->stage != NULL){
		
		TempView = TempView->stage; 
	}
	return(TempView);
}

void pipeline_adjust(){
	
	pp_mtl_adjust->skill1 = floatv(pp_blur);
	pp_mtl_adjust->skill2 = floatd(pp_saturation, 100);
	pp_mtl_adjust->skill3 = floatd(pp_sepia, 100);
	pp_mtl_adjust->skill4 = floatd(pp_darken, 100);
	pp_mtl_adjust->skill5 = floatd(pp_contrast, 100);
	pp_mtl_sharpen->skill1 = floatv(pp_sharpen_strength);
	
	mtl_world->skill1 = floatv(mtl_brightness);
	mtl_object->skill1 = floatv(mtl_brightness);
	mtl_fade->skill1 = floatv(mtl_brightness);
	mtl_translucent->skill1 = floatv(mtl_brightness);
	mtl_anim_sprite->skill1 = floatv(mtl_brightness);
}

void pipeline_start(){
	
	pp_add(pp_mtl_adjust, camera, NULL);
	pp_add(pp_mtl_sharpen, camera, NULL);
	
	pipeline_adjust();
}

void pipeline_update(){
	
	pp_underwater_is_on = 0;
	pp_remove_all(camera);
	pipeline_start();
}

void pipeline_resolution(){
	
	var mode = video_mode;
	
	while(1){
		
		if(!key_shift){ mode++; }
		else{ mode--; }
		mode = cycle(mode, 6, 12);
		if(video_switch(mode, 0, 0)){ break; }
	}
	
	// update pipeline
	pipeline_update();
}