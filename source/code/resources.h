#ifndef _RESOURCES_H_
#define _RESOURCES_H_

// bounding boxes
STRING *bbox_cct_mdl = "bbox_npc.mdl";
STRING *bbox_casing_mdl = "bbox_casing.mdl";
STRING *bbox_barrel_mdl = "bbox_barrel.mdl";

// particles
BMAP *particle_png = "particle.png";
BMAP *bubbles_tga = "bubbles.tga";

// casings sprites
STRING *shell_fly_tga = "shell_fly+8.tga";
STRING *shell_floor_tga = "shell_floor+3.tga";
STRING *casing_fly_tga = "casing_fly+8.tga";
STRING *casing_floor_tga = "casing_floor+3.tga";

// weapon sprites
STRING *world_saw_tga = "w_saw.tga";
STRING *view_saw_tga = "v_saw+4.tga";
STRING *world_pistol_tga = "w_pistol.tga";
STRING *view_pistol_tga = "v_pistol+4.tga";
STRING *world_shotgun_tga = "w_shotgun.tga";
STRING *view_shotgun_tga = "v_shotgun+8.tga";
STRING *world_supershotgun_tga = "w_sshotgun.tga";
STRING *view_supershotgun_tga = "v_sshotgun+12.tga";
STRING *world_chaingun_tga = "w_chaingun.tga";
STRING *view_chaingun_tga = "v_chaingun+4.tga";
STRING *world_rocketlauncher_tga = "w_rocketlauncher.tga";
STRING *view_rocketlauncher_tga = "v_rocketlauncher+6.tga";
STRING *world_lasergun_tga = "w_lasergun.tga";
STRING *view_lasergun_tga = "v_lasergun+4.tga";

// items
STRING *item_ammo_big_tga = "item_ammo_big.tga";
STRING *item_ammo_small_tga = "item_ammo_small.tga";
STRING *item_armor_big_tga = "item_armor_big+2.tga";
STRING *item_armor_small_tga = "item_armor_small+4.tga";
STRING *item_health_big_tga = "item_health_big.tga";
STRING *item_health_small_tga = "item_health_small+4.tga";
STRING *item_key_red_tga = "item_key_red+2.tga";
STRING *item_key_blue_tga = "item_key_blue+2.tga";
STRING *item_key_yellow_tga = "item_key_yellow+2.tga";
STRING *item_pistol_ammo_tga = "item_pistol_ammo.tga";
STRING *item_rocket_big_tga = "item_rocket_big.tga";
STRING *item_rocket_small_tga = "item_rocket_small.tga";
STRING *item_shells_big_tga = "item_shells_big.tga";
STRING *item_shells_small_tga = "item_shells_small.tga";
STRING *item_suit_tga = "item_suit.tga";

// weapon fx sprites
STRING *rocket_flying_tga = "rocket_flying+8.tga";
STRING *bullet_impact_tga = "bullet_impact+4.tga";
STRING *blood_impact_tga = "blood_impact+4.tga";

// fx
STRING *water_splash_small_tga = "splash_small+16.tga";
STRING *water_splash_medium_tga = "splash_medium+16.tga";
STRING *water_splash_big_tga = "splash_big+16.tga";
STRING *water_ripple_tga = "water_ripple+16.tga";
STRING *explo_particle_sprite_tga = "explo_sprite+4.tga";
STRING *explo_underwater_particle_sprite_tga = "explo_underwater_sprite+4.tga";

// this explosion sprites, are used for 'non particle' effects
STRING *explo_barrel_tga = "explo_barrel+16.tga";
STRING *explo_big_tga = "explo_big+16.tga";
STRING *explo_underwater_tga = "explo_underwater+16.tga";

// props
STRING *props_barrel_tga = "props_barrel+2.tga";

// fx sounds
SOUND *water_hard_impact_01_ogg = "water_hard_impact_01.ogg";
SOUND *water_hard_impact_02_ogg = "water_hard_impact_02.ogg";
SOUND *water_hard_impact_03_ogg = "water_hard_impact_03.ogg";
SOUND *water_small_impact_01_ogg = "water_small_impact_01.ogg";
SOUND *water_small_impact_02_ogg = "water_small_impact_02.ogg";
SOUND *water_small_impact_03_ogg = "water_small_impact_03.ogg";

// weapon sounds
SOUND *weapon_draw_ogg = "wp_draw.ogg";
SOUND *weapon_holster_ogg = "wp_holster.ogg";
SOUND *weapon_dry_shoot_ogg = "wp_dry_shoot.ogg";
SOUND *weapon_saw_up_ogg = "wp_saw_up.ogg";
SOUND *weapon_saw_idle_ogg = "wp_saw_idle.ogg";
SOUND *weapon_saw_hit_01_ogg = "wp_saw_hit_01.ogg";
SOUND *weapon_saw_hit_02_ogg = "wp_saw_hit_02.ogg";
SOUND *weapon_saw_hit_03_ogg = "wp_saw_hit_03.ogg";
SOUND *weapon_saw_hit_body_01_ogg = "wp_saw_body_01.ogg";
SOUND *weapon_saw_hit_body_02_ogg = "wp_saw_body_02.ogg";
SOUND *weapon_saw_hit_body_03_ogg = "wp_saw_body_03.ogg";
SOUND *weapon_saw_full_ogg = "wp_saw_full.ogg";
SOUND *weapon_pistol_shoot_ogg = "wp_pistol_shoot.ogg";
SOUND *weapon_chaingun_shoot_ogg = "wp_chaingun_shoot.ogg";
SOUND *weapon_shotgun_shoot_ogg = "wp_shotgun_shoot.ogg";
SOUND *weapon_sshotgun_shoot_ogg = "wp_supershotgun_shoot.ogg";
SOUND *weapon_rocketlauncher_shoot_ogg = "wp_rocketlauncher_shoot.ogg";
SOUND *weapon_lasergun_shoot_ogg = "wp_laser_shoot.ogg";

// casing sounds
SOUND *shell_hit_01_ogg = "wp_shell_hit_01.ogg";
SOUND *shell_hit_02_ogg = "wp_shell_hit_02.ogg";
SOUND *shell_hit_03_ogg = "wp_shell_hit_03.ogg";
SOUND *casing_hit_01_ogg = "wp_casing_hit_01.ogg";
SOUND *casing_hit_02_ogg = "wp_casing_hit_02.ogg";
SOUND *casing_hit_03_ogg = "wp_casing_hit_03.ogg";

// explosion sounds
SOUND *explo_01_ogg = "explosion_01.ogg";
SOUND *explo_02_ogg = "explosion_02.ogg";
SOUND *explo_03_ogg = "explosion_03.ogg";
SOUND *explo_underwater_01_ogg = "underwater_explo_01.ogg";
SOUND *explo_underwater_02_ogg = "underwater_explo_02.ogg";
SOUND *explo_underwater_03_ogg = "underwater_explo_03.ogg";
SOUND *explo_water_01_ogg = "water_explosion_01.ogg";
SOUND *explo_water_02_ogg = "water_explosion_02.ogg";
SOUND *explo_water_03_ogg = "water_explosion_03.ogg";

// pickup sounds
SOUND *pickup_weapon_ogg = "pickup_weapon.ogg";
SOUND *pickup_small_item_ogg = "pickup_small_item.ogg";
SOUND *pickup_item_ogg = "pickup_item.ogg";
SOUND *pickup_key_ogg = "pickup_key.ogg";

// water sounds
SOUND *water_out_ogg = "water_out.ogg";
SOUND *water_in_ogg = "water_in.ogg";
SOUND *water_in_slow_ogg = "water_in_slow.ogg";
SOUND *underwater_ogg = "underwater.ogg";

// player's sounds
SOUND *player_death_ogg = "player_death.ogg";
SOUND *player_x_death_ogg = "player_x_death.ogg";
SOUND *player_death_to_water_ogg = "player_death_to_water.ogg";
SOUND *player_land_ogg = "player_land.ogg";
SOUND *player_no_use_ogg = "player_no_use.ogg";
SOUND *player_pain_01_ogg = "player_pain_01.ogg";
SOUND *player_pain_02_ogg = "player_pain_02.ogg";
SOUND *player_underwater_pain_01_ogg = "player_pain_underwater_01.ogg";
SOUND *player_underwater_pain_02_ogg = "player_pain_underwater_02.ogg";
SOUND *player_smashed_ogg = "player_smashed.ogg";
SOUND *player_swim_01_ogg = "player_swim_01.ogg";
SOUND *player_swim_02_ogg = "player_swim_02.ogg";
SOUND *player_gasp_01_ogg = "player_gasp_01.ogg";
SOUND *player_gasp_02_ogg = "player_gasp_02.ogg";
SOUND *player_drown_01_ogg = "player_drown_01.ogg";
SOUND *player_drown_02_ogg = "player_drown_02.ogg";

// props sounds
SOUND *switch_ogg = "switch.ogg";
SOUND *switch_next_level_ogg = "switch_next_level.ogg";
SOUND *elevator_start_ogg = "elevator_start.ogg";
SOUND *elevator_stop_ogg = "elevator_stop.ogg";
SOUND *elevator_mid_ogg = "elevator_mid.ogg";
SOUND *door_01_open_ogg = "door_open_01.ogg";
SOUND *door_01_close_ogg = "door_close_01.ogg";
SOUND *door_02_open_ogg = "door_open_02.ogg";
SOUND *door_02_close_ogg = "door_close_02.ogg";
SOUND *platform_start_ogg = "platform_start.ogg";
SOUND *platform_stop_ogg = "platform_stop.ogg";
SOUND *platform_mid_ogg = "platform_mid.ogg";
SOUND *secret_wall_start_ogg = "secret_wall_start.ogg";
SOUND *secret_wall_stop_ogg = "secret_wall_stop.ogg";
SOUND *secret_zone_found_ogg = "secret_zone_found.ogg";
SOUND *teleport_ogg = "teleport.ogg";

// weapon fx sounds
SOUND *bullet_ric_01_ogg = "wp_ric_01.ogg";
SOUND *bullet_ric_02_ogg = "wp_ric_02.ogg";
SOUND *bullet_ric_03_ogg = "wp_ric_03.ogg";
SOUND *bullet_hit_body_01_ogg = "wp_hit_body_01.ogg";
SOUND *bullet_hit_body_02_ogg = "wp_hit_body_02.ogg";
SOUND *bullet_hit_body_03_ogg = "wp_hit_body_03.ogg";

#endif