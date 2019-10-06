 #include "settings.h"

#include <string>
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <iostream>
#include <fstream>
#include <map>

C_ConfigManager * cfg_manager = new C_ConfigManager();
Config g_cfg;

void C_ConfigManager::setup() {
	
	setup_item(&g_cfg.ragebot.enable, false, "Ragebot.enable");
	setup_item(&g_cfg.ragebot.field_of_view, 360, "Ragebot.fov");
	setup_item(&g_cfg.ragebot.silent_aim, false, "Ragebot.silent_aim");
	setup_item(&g_cfg.ragebot.selection_type, 0, "Ragebot.selection_type");
	setup_item(&g_cfg.ragebot.lag_compensation_type, 0, "Ragebot.lag_compensation_type");
	setup_item(&g_cfg.ragebot.anti_recoil, false, "Ragebot.anti_recoil");
	setup_item(&g_cfg.ragebot.anti_spread, false, "Ragebot.anti_spread");
	setup_item(&g_cfg.ragebot.zeus_bot, false, "Ragebot.zeus_bot");
	setup_item(&g_cfg.ragebot.extrapolation, false, "Ragebot.extrapolation");
	setup_item(&g_cfg.antiaim.freestandjitterrange, 25, "Antiaim.freestandjitterrange");
	setup_item(&g_cfg.ragebot.aimstep, false, "Ragebot.aimstep");
	//	setup_item( &g_cfg.ragebot.antiaim_correction, false, "Ragebot.antiaim_correction" );
	//	setup_item(&g_cfg.ragebot.antiaim_correction2, false, "Ragebot.antiaim_correction2");
	//	setup_item(&g_cfg.ragebot.antiaim_correction3, false, "Ragebot.antiaim_correction3");
	//	setup_item(&g_cfg.ragebot.antiaim_correction3modes, false, "Ragebot.antiaim_correction3modes");
	setup_item(&g_cfg.ragebot.override_key, BUTTON_CODE_NONE, "Ragebot.override_key");
	setup_item(&g_cfg.ragebot.bruteshots, 0, "Ragebot.bruteshots");
	setup_item(&g_cfg.ragebot.lagcomp, false, "Ragebot.lagcomp");
	setup_item(&g_cfg.ragebot.hsonly, false, "Ragebot.hsonly");
	setup_item(&g_cfg.ragebot.knifebot, false, "Ragebot.knifebot");
	setup_item(&g_cfg.ragebot.lagcomp_time, 0.10f, "Ragebot.lagcomp_time");
	setup_item(&g_cfg.ragebot.resolver_experimentalmode, 0, "Ragebot.resolver_experimentalmode");
	//g_cfg.ragebot.lag_compensation_when
/*	setup_item(&g_cfg.ragebot.lag_compensation_when, {
	{ false, "Move" },
	{ false, "Angles" },
	{ false, "Air" },
	{ false, "" },
	{ false, "" }
		},"Ragebot.lag_compensation_when");*/
	for (int i = 0; i < 7; i++) {
		std::string start = "Ragebot.";

		char * names[7] = { "pistols", "deagle", "rifles", "smgs", "snipers", "autos", "shotguns" };
		start += names[i];

		setup_item(&g_cfg.ragebot.autoshoot[i], false, start + ".autoshoot");
		setup_item(&g_cfg.ragebot.autowall[i], false, start + ".autowall");
		setup_item(&g_cfg.ragebot.minimum_damage[i], 1, start + ".minimum_damage");

		setup_item(&g_cfg.ragebot.autoscope, false, start + ".autoscope");

		setup_item(&g_cfg.ragebot.hitscan[i], {
			{ false, "Head" },
			{ false, "Chest" },
			{ false, "Stomach" },
			{ false, "Arms" },
			{ false, "Legs" }
			}, start + ".hitscan");

		setup_item(&g_cfg.ragebot.hitscan_history[i], {
			{ false, "Head" },
			{ false, "Chest" },
			{ false, "Stomach" },
			{ false, "Arms" },
			{ false, "Legs" }
			}, start + ".hitscan_history");

		setup_item(&g_cfg.ragebot.multipoint_hitboxes[i], {
			{ false, "Head" },
			{ false, "Chest" },
			{ false, "Stomach" },
			{ false, "Arms" },
			{ false, "Legs" }
			}, start + ".multipoint");

		setup_item(&g_cfg.ragebot.pointscale[i], 0.3f, start + ".pointscaleother");
		setup_item(&g_cfg.ragebot.pointscalehead[i], 0.3f, start + ".pointscalehead");
		setup_item(&g_cfg.ragebot.pointscaleneck[i], 0.3f, start + ".pointscaleneck");
		setup_item(&g_cfg.ragebot.pointscalechest[i], 0.3f, start + ".pointscalechest");
		setup_item(&g_cfg.ragebot.pointscalestomach[i], 0.3f, start + ".pointscalestomach");
		setup_item(&g_cfg.ragebot.pointscalepelvis[i], 0.3f, start + ".pointscalepelvis");
		setup_item(&g_cfg.ragebot.pointscalearms[i], 0.3f, start + ".pointscalearms");
		setup_item(&g_cfg.ragebot.pointscalelegs[i], 0.3f, start + ".pointscalelegs");
		setup_item(&g_cfg.misc.fakewalk, 0, "Fakewalk");
		setup_item(&g_cfg.misc.fakewalk_key, BUTTON_CODE_NONE, "Fakewalk.key");
		setup_item(&g_cfg.ragebot.hitchance[i], false, start + ".hitchance");
		setup_item(&g_cfg.ragebot.hitchance_amount[i], 0, start + ".hitchance_amount");
		setup_item(&g_cfg.ragebot.baimifhp[i], 0, start + ".baimifhp");
		setup_item(&g_cfg.ragebot.baim_settings[i], {
			{ false, "In Air" },
			{ false, "Not Resolved" },
			{ false, "If Can Kill" },
			{ false, "Breaking Lby" },
			{ false, "Slowwalk " },
			{ false, "Always" }
			}, start + ".baim");

		setup_item(&g_cfg.ragebot.quickstop[i], false, start + ".quickstop");
		setup_item(&g_cfg.ragebot.autostopmovement[i], false, start + ".autostopmovement");
		setup_item(&g_cfg.ragebot.resolver_experimental, false, ".resolverrrrr");
	}

	//
	//
	//
	//
	//
	//
	//						ANTIAIM
	//
	//
	//
	//
	//
	//
	//

	for (int i = 0; i < 3; i++) {
		std::string start = "Antiaim.";

		char * antiaim_modes[3] = { "Still", "Moving", "Air" };

		start += antiaim_modes[i];

		setup_item(&g_cfg.antiaim.antiaim_desync[i], false, start + ".desync");
		setup_item(&g_cfg.antiaim.yaw[i], false, start + ".yaw");
		setup_item(&g_cfg.antiaim.yaw_offset[i], 0, start + ".yaw_offset");
		setup_item(&g_cfg.antiaim.pitch[i], false, start + ".pitch");
		setup_item(&g_cfg.antiaim.break_adjustment[i], false, start + "antiaim.break_adjustment");
		setup_item(&g_cfg.antiaim.desync_mode[i], false, start + ".desync_mode");
		setup_item(&g_cfg.antiaim.base_angle[i], false, start + ".base_angle");
		setup_item(&g_cfg.antiaim.shift_yaw[i], 0, start + ".shift_offset");
		setup_item(&g_cfg.antiaim.jitter_type[i], false, start + ".jit_type");
		setup_item(&g_cfg.antiaim.static_desync_type[i], 0, start + ".jit_type");
		setup_item(&g_cfg.antiaim.desync_delta_swap_st[i], 0, start + "antiaim.desync_time_switch");
		setup_item(&g_cfg.antiaim.desyncjitterrange[i], 0, start + "Ragebot.desyncjitter");
		setup_item(&g_cfg.antiaim.enable_shifts[i], false, start + "aa.shify.yaw");
		setup_item(&g_cfg.antiaim.enable_switching_yaw[i], false, start + "aa.switchy.yaw");
		setup_item(&g_cfg.antiaim.tank_aa[i], false, start + "aa.tanki.yaw");
	}

	setup_item(&g_cfg.antiaim.lby_breaker, false, "antiaim.lby_breaker");
	setup_item(&g_cfg.antiaim.enable, false, "antiaim.enable");
	setup_item(&g_cfg.antiaim.lby_delta, 0, "antiaim.lby_delta");
	setup_item(&g_cfg.antiaim.pitch_flick, false, "antiaim.pitch_flick");
	setup_item(&g_cfg.antiaim.manualaafakes, 0, "antiaim.manualaafakes");
	setup_item(&g_cfg.antiaim.air_desync, 0, "antiaim.airdesync");
	setup_item(&g_cfg.antiaim.breaklagcomp, false, "antiaim.break_lagcomp");
	setup_item(&g_cfg.antiaim.animfix, false, "antiaim.animfix");
	setup_item(&g_cfg.antiaim.right_manualkey, BUTTON_CODE_NONE, "Antiaim.manualkey_right");
	setup_item(&g_cfg.antiaim.use2ndfreestanding, false, "Antiaim.use2ndfreestanding");
	setup_item(&g_cfg.antiaim.left_manualkey, BUTTON_CODE_NONE, "Antiaim.manualkey_left");
	setup_item(&g_cfg.antiaim.back_manualkey, BUTTON_CODE_NONE, "Antiaim.manualkey_back");
	setup_item(&g_cfg.antiaim.disable_manualkey, BUTTON_CODE_NONE, "Antiaim.manualkey_disable");//&g_cfg.antiaim.disable_manualkey
	setup_item(&g_cfg.antiaim.hittinggroundpitch, false, "Antiaim.hittinggroundpitch");
	setup_item(&g_cfg.antiaim.fakelag_mode, 0, "Anitiaim.fakelag_mode");
	setup_item(&g_cfg.antiaim.fakelag_enablers, {
		{ false, "moving" },
		{ false, "air" },
		{ false, "standing" },
		{ false, "shooting" }
		}, "Antiaim.fakelag_enablers");
	setup_item(&g_cfg.antiaim.fakelag_amount, 1, "Antiaim.fakelag_amount");
	setup_item(&g_cfg.misc.ticks_fd, 1, "misc.fakeduck.ticks");
	setup_item(&g_cfg.antiaim.panicjitterhp, 0, "Anitiaim.panicjitterhp");
	setup_item(&g_cfg.antiaim.fl_amtweaponswitch, 1, "Antiaim.fl_amtweaponswitch");
	setup_item(&g_cfg.antiaim.auto_fakelag, false, "Antiaim.auto_fakelag");
	setup_item(&g_cfg.antiaim.fl_spike_on_shoot, false, "Antiaim.fl_spike_on_shoot");
	setup_item(&g_cfg.antiaim.fl_spike_on_jump, false, "Antiaim.fl_spike_on_jump");
	setup_item(&g_cfg.antiaim.fl_spike_on_peek, false, "Antiaim.fl_spike_on_peek");
	setup_item(&g_cfg.antiaim.fakelagonkey, BUTTON_CODE_NONE, "Antiaim.fakelagonkey");
	setup_item(&g_cfg.antiaim.use2ndfreestanding, false, "Antiaim.use2ndfreestanding");
	setup_item(&g_cfg.antiaim.panicjitter, false, "Antiaim.panicjitter");
	setup_item(&g_cfg.antiaim.panicjittermode, 0, "Antiaim.panicjittermode");
	setup_item(&g_cfg.antiaim.panicjitterrange, 0, "Antiaim.panicjitterrange");
	setup_item(&g_cfg.antiaim.jitterbetweensecondbestangle, false, "Antiaim.jitterbetweenbest");
	setup_item(&g_cfg.antiaim.freestanddelay, 1, "Antiaim.jitterbetweenbestdelay");
	setup_item(&g_cfg.antiaim.p2cbreaker, false, "Antiaim.p2cbreaker");
	setup_item(&g_cfg.antiaim.ninesevenlbybreaker, false, "Antiaim.ninesevenlbybreaker");

	//
	//
	//
	//
	//
	//
	//						MISC
	//
	//
	//
	//
	//
	//
	//

	setup_item(&g_cfg.misc.ghostcharmsangle, 0, "Misc.ghostcharmsangle");
	setup_item(&g_cfg.misc.bunnyhop, false, "Misc.autohop");
	setup_item(&g_cfg.misc.watermark, false, "Misc.watermark");
	setup_item(&g_cfg.misc.key_binds, false, "Misc.key_binds");
	setup_item(&g_cfg.misc.airstrafe, false, "Misc.airstrafe");
	setup_item(&g_cfg.misc.status_bar, false, "Misc.statusbar");
	//	setup_item(&g_cfg.misc.fakewalkspeed, 5, "Misc.fakewalkspeed");
	setup_item(&g_cfg.misc.duck_jump, false, "Misc.duckjump");
	setup_item(&g_cfg.misc.large, false, "Misc.large");
	setup_item(&g_cfg.misc.largex, 0, "Misc.largex");
	setup_item(&g_cfg.misc.largey, 0, "Misc.largey");
	setup_item(&g_cfg.misc.largez, 0, "Misc.largez");
	setup_item(&g_cfg.misc.customviewmodel, false, "Misc.customviewmodel");
	setup_item(&g_cfg.misc.CustomViewModelX, 2, "Misc.customviewmodelx");
	setup_item(&g_cfg.misc.CustomViewModelY, 2, "Misc.customviewmodely");
	setup_item(&g_cfg.misc.CustomViewModelZ, -2, "Misc.customviewmodelz");
	setup_item(&g_cfg.misc.clantagprefix, true, "Misc.prefix");
	setup_item(&g_cfg.misc.unlimited_duck, false, "Misc.unlimduck");
	setup_item(&g_cfg.misc.fakeduck, BUTTON_CODE_NONE, "Misc.fakeduck");
	setup_item(&g_cfg.misc.clantagprefixmodes, 0, "Misc.clantagprefixmodes");
	setup_item(&g_cfg.misc.circle_strafe, BUTTON_CODE_NONE, "Misc.circle_strafer");
	setup_item(&g_cfg.misc.thirdperson_toggle, BUTTON_CODE_NONE, "Misc.thirdperson_toggle");
	setup_item(&g_cfg.misc.clantagprefixmodes, 0, "Misc.clantagprefixmodes");
	setup_item(&g_cfg.misc.events_to_log, {
		{ false, "Player hits" },
		{ false, "Items" },
		{ false, "Planting" },
		{ false, "Defusing" }
		}, "Misc.events_to_log");
	setup_item(&g_cfg.misc.exaggerated_ragdolls, false, "Misc.exaggerated_ragdolls");
	setup_item(&g_cfg.misc.exaggerated_ragdolls_multiplier, 10, "Misc.exaggerated_ragdolls_multiplier");
	setup_item(&g_cfg.misc.clantag_spammer, false, "Misc.clantag_spammer");

	setup_item(&g_cfg.misc.ping_spike, false, "Misc.ping_spike");
	setup_item(&g_cfg.misc.ping_spike_value, 0, "Misc.ping_spike_value");
	setup_item(&g_cfg.ragebot.baim_bind, BUTTON_CODE_NONE, "Misc.baim.bind");

	setup_item(&g_cfg.misc.anti_untrusted, true, "Misc.antiuntrusted");

	setup_item(&g_cfg.misc.sniper_crosshair, false, "Esp.sniper_crosshair");

		//
		//
		//
		//
		//
		//
		//						MENU
		//
		//
		//
		//
		//
		//
		//

	setup_item(&g_cfg.menu.menu_bind, KEY_INSERT, "Menu.bind");
	setup_item(&g_cfg.menu.menu_theme[0], Color(104, 210, 42), "Menu.color[ 0 ]");
	setup_item(&g_cfg.menu.menu_theme[1], Color(104, 210, 42), "Menu.color[ 1 ]");

	//
	//
	//
	//
	//
	//
	//						PLAYER
	//
	//
	//
	//
	//
	//
	//

	setup_item(&g_cfg.player.enable, false, "Player.enable");
	setup_item(&g_cfg.player.dormant_players, false, "Player.dormant_players");
	setup_item(&g_cfg.player.alpha, 0, "Player.alpha");

	setup_item(&g_cfg.player.out_pov_arrow, false, "Player.directional_arrow");
	setup_item(&g_cfg.player.out_pov_distance, 300, "Player.directional_arrow_distance");
	setup_item(&g_cfg.player.out_pov_arrow_color, Color(255, 255, 255, 255), "Player.out_pov_arrow_color");

	setup_item(&g_cfg.player.esp_view, false, "Player.esp_preview");
	setup_item(&g_cfg.player.box, false, "Player.box");
	setup_item(&g_cfg.player.box_color, Color(255, 255, 255, 255), "Player.box_color");
	setup_item(&g_cfg.player.health, false, "Player.health");
	setup_item(&g_cfg.player.armorbar, false, "Player.armor");
	setup_item(&g_cfg.player.name, false, "Player.name");
	setup_item(&g_cfg.player.name_color, Color(255, 255, 255, 255), "Player.name_color");
	setup_item(&g_cfg.player.weapon, false, "Player.weapon"); 
	setup_item(&g_cfg.player.weapon_color, Color(255, 255, 255, 255), "Player.weapon_color");
	setup_item(&g_cfg.player.ammo, 0, "Player.ammo");
	setup_item(&g_cfg.player.ammobar_color, Color(255, 255, 255, 255), "Player.ammobar_color");
	setup_item(&g_cfg.player.lby, false, "Player.lby");
	setup_item(&g_cfg.player.lbybar_color, Color(255, 255, 255, 255), "Player.lbybar_color");

	setup_item(&g_cfg.player.flags, {
		{ false, "Scoped" },
		{ false, "Money" },
		{ false, "Armor" },
		{ false, "Vulnerable" },
		{ false, "Lowerbody Timer" },
		{ false, "Resolver Mode" },
		{ false, "Breaking LC" },
		{ false, "Player Debug" },
		{ false, "Slowwalking"},
		{ false, "reloading"}
		}, "Player.flags");

	setup_item(&g_cfg.player.skeleton, {
		{ false, "Normal" },
		{ false, "Backtracked" }
		}, "Player.skeleton");

	setup_item(&g_cfg.player.glow, {
		{ false, "Enemy" },
		{ false, "Teammate" },
		{ false, "Local" }
		}, "Player.glow");
	setup_item(&g_cfg.player.glow_type, 0, "Player.glow_type");
	setup_item(&g_cfg.player.glow_color_enemy, Color(255, 255, 255, 255), "Player.glow_color_enemy");
	setup_item(&g_cfg.player.glow_color_teammate, Color(255, 255, 255, 255), "Player.glow_color_teammate");
	setup_item(&g_cfg.player.glow_color_local, Color(255, 255, 255, 255), "Player.glow_color_local");
	setup_item(&g_cfg.player.glowopacity, 100, "Player.glowopacity");
	setup_item(&g_cfg.player.glowbloom, 100, "Player.glowbloom");
	setup_item(&g_cfg.esp.fake_chams, false, "Player.fakelagchams");

	setup_item(&g_cfg.player.chams, {
		{ false, "Visible" },
		{ false, "Invisible" }
		}, "Player.chams");

	setup_item(&g_cfg.player.chams_color, Color(255, 255, 255, 255), "Player.chams_color");
	setup_item(&g_cfg.player.xqz_color, Color(255, 255, 255, 255), "Player.xqz_color");
	setup_item(&g_cfg.player.chams_opacity, 100, "Player.chams_opacity");
	setup_item(&g_cfg.player.chams_type, 0, "Player.chams_type");
	setup_item(&g_cfg.player.lowerbody_indicator, false, "Player.lowerbody_indicator");

	setup_item(&g_cfg.player.backtrack_chams, false, "Player.backtrack_chams");
	setup_item(&g_cfg.player.backtrack_chams_color, Color(255, 255, 255), "Player.backtrack_chams_color");
	setup_item(&g_cfg.player.backtrack_chams_opacity, 100, "Player.backtrack_chams_opacity");

	setup_item(&g_cfg.esp.gunchamscolor, Color(255, 255, 255), "Esp.gunchamscolor");
	setup_item(&g_cfg.esp.handchamscolor, Color(255, 255, 255), "Esp.handchamscolor");
	setup_item(&g_cfg.esp.fake_chams_color, Color(255, 255, 255), "Esp.fakelagchams");
	setup_item(&g_cfg.esp.espbomb, false, "Esp.bombbz");

	setup_item(&g_cfg.esp.gunchams, 0, "Esp.gunchams");
	setup_item(&g_cfg.esp.handchams, 0, "Esp.handchams");
	//
	//
	//
	//
	//
	//
	//						ESP
	//
	//
	//
	//
	//
	//
	//

	setup_item(&g_cfg.esp.fov, 50, "Esp.fov");
	setup_item(&g_cfg.esp.override_fov_scoped, 0, "Esp.override_fov_scoped");

	setup_item(&g_cfg.esp.grenade_enable, 0, "Esp.grenade_prediction_enable");
	setup_item(&g_cfg.esp.transparency_scoped, 0, "Esp.transparency_scoped");
	setup_item(&g_cfg.esp.transparency_scoped_alpha, 0, "Esp.transparency_scoped_alpha");

	setup_item(&g_cfg.esp.spectator_list, false, "Esp.spectator_list");

	setup_item(&g_cfg.esp.spectator_list_x, 0, "Esp.spectatorlistx");
	setup_item(&g_cfg.esp.spectator_list_y, 0, "Esp.spectatorlisty");

	setup_item(&g_cfg.esp.show_spread, false, "Esp.show_spread");
	setup_item(&g_cfg.esp.show_spread_type, 0, "Esp.show_spread_type");
	setup_item(&g_cfg.esp.show_spread_color, Color(255, 255, 255), "Esp.show_spread_color");
	setup_item(&g_cfg.esp.show_spread_opacity, 0, "Esp.show_spread_opacity");

	setup_item(&g_cfg.esp.crosshair, false, "Esp.crosshair");
	setup_item(&g_cfg.esp.crosshair_color, Color(255, 255, 255), "Esp.crosshair_color");
	setup_item(&g_cfg.menu.removescopecol[0], Color(255, 255, 255), "Esp.RemoveSCopeCol");
	setup_item(&g_cfg.esp.anti_aim_lines, false, "Esp.anti_aim_lines");
	setup_item(&g_cfg.misc.autodefuse, false, "Misc.autodefuse");
	setup_item(&g_cfg.esp.ghost_chams, false, "Esp.ghost_chams");
	setup_item(&g_cfg.esp.ghost_chams_color, Color(255, 0, 0), "Esp.ghost_chams_color");
	setup_item(&g_cfg.esp.local_chams, false, "Esp.local_chams");
	setup_item(&g_cfg.esp.local_chams_color, Color(255, 255, 255), "Esp.local_chams_color");

	setup_item(&g_cfg.esp.bullet_tracer, false, "Esp.bullet_tracer");
	setup_item(&g_cfg.esp.bullet_tracer_color, Color(255, 255, 255), "Esp.bullet_tracer_color");
	setup_item(&g_cfg.esp.bullet_tracer_enemy, false, "Esp.bullet_tracer_enemy");
	setup_item(&g_cfg.esp.bullet_tracer_color_enemy, Color(255, 255, 255), "Esp.bullet_tracer_color_enemy");
	setup_item(&g_cfg.esp.bullet_tracer_duration, 3, "Esp.bullet_tracer_duration");
	setup_item(&g_cfg.esp.bullet_tracer_size, 3, "Esp.bullet_tracer_size");

	setup_item(&g_cfg.esp.dropped_weapons, false, "Esp.dropped_weapons");
	setup_item(&g_cfg.esp.dropped_weapons_color, Color(255, 255, 255), "Esp.dropped_weapons_color");

	setup_item(&g_cfg.esp.projectiles, false, "Esp.projectiles");
	setup_item(&g_cfg.esp.projectiles_color, Color(255, 255, 255), "Esp.projectiles_color");

	setup_item(&g_cfg.esp.skybox, 0, "Esp.skybox");
	setup_item(&g_cfg.esp.fullbright, false, "Esp.fullbright");
	setup_item(&g_cfg.esp.bright, false, "Esp.bright");
	setup_item(&g_cfg.esp.dont_render_teammates, false, "Esp.dont_render_teammates");
	setup_item(&g_cfg.esp.nightmode, false, "Esp.nightmode");
	setup_item(&g_cfg.esp.nightmodeskybox, 0, "Esp.nightmodeskybox");

	setup_item(&g_cfg.esp.hitmarker, {
		{ false, "Static" },
		{ false, "Dynamic" }
		}, "Esp.hitmarker");

	setup_item(&g_cfg.esp.hitsound, false, "Esp.hitsound");
	setup_item(&g_cfg.esp.indicators, {
	{ false, "penetration indicator" },
	{ false, "anti-aims indicator" },
	{ false, "Croked indicator" },
	{ false, "extend indicator" },
	{ false, "lc indicator" },
	{ false, "double tap indicator" },
	{ false, "desync indicator" }
	}, "esp.indicators");
	setup_item(&g_cfg.esp.removescope, false, "Esp.removescope");
	setup_item(&g_cfg.esp.removesmoke, false, "Esp.removesmoke");
	setup_item(&g_cfg.esp.removeflash, false, "Esp.removeflash");
	setup_item(&g_cfg.esp.removerecoil, false, "Esp.removerecoil");
	setup_item(&g_cfg.esp.removezoom, false, "Esp.removezoom");
	setup_item(&g_cfg.esp.removeduckcooldown, false, "Esp.removecooldown");
	setup_item(&g_cfg.esp.removescopesensitivity, false, "Esp.removescopesense");
	setup_item(&g_cfg.esp.zeusrange, false, "Esp.zeusrange");

	//
	//
	//
	//
	//
	//
	//						OTHER
	//
	//
	//
	//
	//
	//
	//
	setup_item(&g_cfg.skins.skinenabled, false, "skins.skinenabled");
	setup_item(&g_cfg.skins.AWPSkin, 0, "skins.AWPSkin");
	setup_item(&g_cfg.skins.SCAR20Skin, 0, "skins.SCAR20Skin");
	setup_item(&g_cfg.skins.G3sg1Skin, 0, "skins.G3sg1Skin");
	setup_item(&g_cfg.skins.SSG08Skin, 0, "skins.SSG08Skin");
	setup_item(&g_cfg.skins.DualSkin, 0, "skins.DualSkin");
	setup_item(&g_cfg.skins.DeagleSkin, 0, "skins.DeagleSkin");
	setup_item(&g_cfg.skins.GlockSkin, 0, "skins.GlockSkin");
	setup_item(&g_cfg.skins.USPSkin, 0, "skins.USPSkin");

	setup_item(&g_cfg.skins.Knife, 0, "skins.Knife");
	setup_item(&g_cfg.skins.knifes, 0, "skins.enableKnifechanger");
	setup_item(&g_cfg.skins.KnifeSkin, 0, "skins.KnifeSkin");
	setup_item(&g_cfg.BuyBot.enable, false, "buybot.enable");
	setup_item(&g_cfg.BuyBot.armorkit, false, "buybot.armorkit");
	setup_item(&g_cfg.BuyBot.weapons, 0, "buybot.weapons");
	setup_item(&g_cfg.BuyBot.nades, {
			{ false, "He-nade" },
			{ false, "Molotov" },
			{ false, "Flash" },
			{ false, "Flash" },
			{ false, "Smoke" },
			{ false, "Decoy" }
		}, "buybot.nades");
	setup_item(&g_cfg.BuyBot.extras, {
			{ false, "Kevlar" },
			{ false, "Defuser" },
			{ false, "Zeus" }
		}, "buybot.extras");
	setup_item(&g_cfg.BuyBot.primary, {
		{ false, "Galilar" },
		{ false, "Famas" },
		{ false, "Ak47" },
		{ false, "M4a1" },
		{ false, "M4a1_silencer" },
		{ false, "Ssg08" },
		{ false, "Aug" },
		{ false, "Sg556" },
		{ false, "Awp" },
		{ false, "Scar20" },
		{ false, "G3sg1" },
		{ false, "Nova" },
		{ false, "Xm1014" },
		{ false, "Mag7" },
		{ false, "M249" },
		{ false, "Negev" },
		{ false, "Mac10" },
		{ false, "Mp9" },
		{ false, "Mp7" },
		{ false, "Ump45" },
		{ false, "P90" },
		{ false, "Bizon" }
		}, "BuyBot.primary");
	setup_item(&g_cfg.BuyBot.secondary, {
		{ false, "Glock" },
		{ false, "P2000" },
		{ false, "Usp_silencer" },
		{ false, "Elite" },
		{ false, "P250" },
		{ false, "Tec9" },
		{ false, "Fiveseven" },
		{ false, "Deagle" }
		}, "BuyBot.secondary");
	setup_item(&g_cfg.BuyBot.zeus, false, "buybot.zeus");
	setup_item(&g_cfg.BuyBot.defuser, false, "buybot.defuser");
	setup_item(&g_cfg.nanwalk.amt, 0, "nanwalk.amt");
	setup_item(&g_cfg.nanwalk.enabled, false, "nanwalk.enabled");
	setup_item(&g_cfg.nanwalk.key, BUTTON_CODE_NONE, "nanwalk.key");
	setup_item(&g_cfg.nanwalk.onlyonground, false, "nanwalk.onlyonground");
	setup_item(&g_cfg.fastwalk.key, BUTTON_CODE_NONE, "fastwalk.key");
	setup_item(&g_cfg.fastwalk.enabled, false, "fastwalk.enabled");
	setup_item(&g_cfg.astrowalk.key, BUTTON_CODE_NONE, "astrowalk.key");
	setup_item(&g_cfg.astrowalk.enabled, false, "astrowalk.enabled");
	setup_item(&g_cfg.lagspike.enabled, false, "lagspike.enabled");
	setup_item(&g_cfg.lagspike.key, BUTTON_CODE_NONE, "lagspike.key");
	setup_item(&g_cfg.gloves.glove, 0, "gloves.glove");
}

void C_ConfigManager::add_item(void * pointer, const char * name, std::string type) {
	items.push_back(new C_ConfigItem(std::string(name), pointer, type));
}

void C_ConfigManager::setup_item(int * pointer, int value, std::string name) {
	add_item(pointer, name.c_str(), "int");
	*pointer = value;
}

void C_ConfigManager::setup_item(bool * pointer, bool value, std::string name) {
	add_item(pointer, name.c_str(), "bool");
	*pointer = value;
}

void C_ConfigManager::setup_item(float * pointer, float value, std::string name) {
	add_item(pointer, name.c_str(), "float");
	*pointer = value;
}

void C_ConfigManager::setup_item(ButtonCode_t * pointer, ButtonCode_t value, std::string name) {
	add_item(pointer, name.c_str(), "ButtonCode");
	*pointer = value;
}

void C_ConfigManager::setup_item(Color * pointer, Color value, std::string name) {
	add_item(pointer, name.c_str(), "Color");
	*pointer = value;
}

void C_ConfigManager::setup_item(std::vector< bool > * pointer, std::string name) {
	add_item(pointer, name.c_str(), "vector<int>");
}

void C_ConfigManager::setup_item(std::vector< int > * pointer, std::string name) {
	add_item(pointer, name.c_str(), "vector<int>");
}

void C_ConfigManager::setup_item(std::vector< MultiDropdownItem_t > * pointer, std::vector< MultiDropdownItem_t > value, std::string name) {
	add_item(pointer, name.c_str(), "vector<MultiDropdownItem_t>");

	std::vector< MultiDropdownItem_t > array = *pointer;

	pointer->clear();

	for (int i = 0; i < value.size(); i++) {
		pointer->push_back({ value[i].enabled, value[i].name });
	}
}

void C_ConfigManager::save(std::string config) {
	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() ->void {
		static TCHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
			folder = std::string(path) + "\\moneysense\\";
			file = std::string(path) + "\\moneysense\\" + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};
	get_dir();

	std::ofstream ofs;

	ofs.open(file + "", std::ios::out | std::ios::trunc);

	json allJson;

	for (auto it : items) {
		json j;

		j["name"] = it->name;
		j["type"] = it->type;

		if (!it->type.compare("int")) {
			j["value"] = (int)*(int*)it->pointer;
		}
		else if (!it->type.compare("float")) {
			j["value"] = (float)*(float*)it->pointer;
		}
		else if (!it->type.compare("bool")) {
			j["value"] = (bool)*(bool*)it->pointer;
		}
		else if (!it->type.compare("ButtonCode")) {
			j["value"] = (int)*(int*)it->pointer;
		}
		else if (!it->type.compare("Color")) {
			Color c = *(Color*)(it->pointer);

			std::vector<int> a = { c.r(), c.g(), c.b(), c.a() };

			json ja;

			for (auto& i : a) {
				ja.push_back(i);
			}

			j["value"] = ja.dump();
		}
		else if (!it->type.compare("vector<int>")) {
			auto& ptr = *(std::vector<int>*)(it->pointer);

			json ja;

			for (auto& i : ptr)
				ja.push_back(i);

			j["value"] = ja.dump();
		}
		else if (!it->type.compare("vector<MultiDropdownItem_t>")) {
			auto& ptr = *(std::vector<MultiDropdownItem_t>*)(it->pointer);

			std::vector< std::string > a_str;
			for (int i = 0; i < ptr.size(); i++) {
				a_str.push_back(ptr[i].enabled ? "1" : "0");
			}

			json ja;
			for (auto & i : a_str) {
				ja.push_back(i);
			}

			j["value"] = ja.dump();
		}

		allJson.push_back(j);
	}

	std::string data = allJson.dump();

	ofs << std::setw(4) << data << std::endl;

	ofs.close();
}

void C_ConfigManager::load(std::string config) {
	static auto find_item = [](std::vector< C_ConfigItem* > items, std::string name) -> C_ConfigItem* {
		for (int i = 0; i < (int)items.size(); i++) {
			if (items[i]->name.compare(name) == 0)
				return (items[i]);
		}

		return nullptr;
	};

	static auto right_of_delim = [](std::string const& str, std::string const& delim) -> std::string {
		return str.substr(str.find(delim) + delim.size());
	};

	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() ->void {
		static TCHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
			folder = std::string(path) + "\\moneysense\\";
			file = std::string(path) + "\\moneysense\\" + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();

	std::ifstream ifs;
	std::string data;

	std::string path = file + "";

	ifs.open(path);

	json allJson;

	ifs >> allJson;

	for (json::iterator it = allJson.begin(); it != allJson.end(); ++it) {
		json j = *it;

		std::string name = j["name"];
		std::string type = j["type"];

		C_ConfigItem *item = find_item(items, name);

		if (item) {
			if (!type.compare("int")) {
				*(int*)item->pointer = j["value"].get<int>();
			}
			else if (!type.compare("float")) {
				*(float*)item->pointer = j["value"].get<float>();
			}
			else if (!type.compare("bool")) {
				*(bool*)item->pointer = j["value"].get<bool>();
			}
			else if (!type.compare("ButtonCode")) {
				*(int*)item->pointer = j["value"].get<int>();
			}
			else if (!type.compare("Color")) {
				std::vector<int> a;

				json ja = json::parse(j["value"].get<std::string>().c_str());

				for (json::iterator it = ja.begin(); it != ja.end(); ++it)
					a.push_back(*it);

				*(Color*)item->pointer = Color(a[0], a[1], a[2], a[3]);
			}
			else if (!type.compare("vector<int>")) {
				auto ptr = static_cast<std::vector <int>*> (item->pointer);
				ptr->clear();

				json ja = json::parse(j["value"].get<std::string>().c_str());

				for (json::iterator it = ja.begin(); it != ja.end(); ++it)
					ptr->push_back(*it);
			}
			else if (!type.compare("vector<MultiDropdownItem_t>")) {
				auto ptr = static_cast<std::vector<MultiDropdownItem_t>*> (item->pointer);
				for (int i = 0; i < ptr->size(); i++) {
					ptr->at(i).enabled = false;
				}

				json ja = json::parse(j["value"].get<std::string>().c_str());

				std::vector < std::string > option_array;
				for (json::iterator it = ja.begin(); it != ja.end(); ++it) {
					std::string it_converted = *it;
					it_converted.erase(std::remove(it_converted.begin(), it_converted.end(), '"'), it_converted.end());
					option_array.push_back(it_converted);
				}

				for (int i = 0; i < option_array.size(); i++) {
					ptr->at(i).enabled = option_array[i].c_str()[0] == '1' ? true : false;
				}
			}
		}
	}

	ifs.close();
}

void C_ConfigManager::remove(std::string config) {
	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() ->void {
		static TCHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
			folder = std::string(path) + "\\moneysense\\";
			file = std::string(path) + "\\moneysense\\" + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};
	get_dir();

	std::string path = file + "";

	std::remove(path.c_str());
}

void C_ConfigManager::config_files() {
	std::string folder;

	auto get_dir = [&folder]() -> void {
		static TCHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
			folder = std::string(path) + "\\moneysense\\";
		}

		CreateDirectory(folder.c_str(), NULL);
	};
	get_dir();

	files.clear();

	std::string path = folder + "/*.cc";// "/*.*";

	WIN32_FIND_DATA fd;

	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				files.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));

		::FindClose(hFind);
	}
}


void C_ConfigManager::lua_files() {
	std::string folder_lua;

	auto get_dir = [&folder_lua]() -> void {
		static TCHAR path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
			folder_lua = std::string(path) + "\\moneysense\\lua\\";
		}

		CreateDirectory(folder_lua.c_str(), NULL);
	};
	get_dir();

	files_lua.clear();

	std::string path = folder_lua + "/*.lua";

	WIN32_FIND_DATA fd;

	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				files.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));

		::FindClose(hFind);
	}
}