#include "cmenu.hpp"
#include "..\cheats\misc\logs.h"
#include <cctype>
#include <shlobj.h>
#include <shlwapi.h>
#include <algorithm>
#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include "../cheats/skinchanger/skinchanger.h"
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(util::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(util::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}
void updatechanger::update_skins() {
	static auto fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(util::pattern_scan("client_panorama.dll", ("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C")));

	auto element = FindHudElement<std::uintptr_t*>(("CCSGO_HudWeaponSelection"));

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = fn(hud_weapons, i);

	skinchanger::get().full_update();
}
void KnifeApplyCallbk()
{
	static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(util::FindSignature("client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C"));
	auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");
	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
	if (hud_weapons == nullptr) return;
	if (!*hud_weapons->get_weapon_count()) return;
	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++) i = clear_hud_weapon_icon_fn(hud_weapons, i);
	typedef void(*ForceUpdate) (void);
}
int lua_follder;
std::string get_config_dir() {
	std::string folder;
	static TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
		folder = std::string(path) + "\\moneysense\\";
	}
	CreateDirectory(folder.c_str(), NULL);
	return folder;
}
#define BUILDSTAMP ( __DATE__ )
void load_config() {
	if (!cfg_manager->files.size()) { return; }
	eventlogs::get().add("Loaded " + cfg_manager->files[g_cfg.selected_config], Color::White);
	cfg_manager->load(cfg_manager->files[g_cfg.selected_config]);
}
void save_config() {
	if (!cfg_manager->files.size()) { return; }
	eventlogs::get().add("Saved " + cfg_manager->files[g_cfg.selected_config], Color::White);
	cfg_manager->save(cfg_manager->files[g_cfg.selected_config]);
	cfg_manager->config_files();
}
void remove_config() {
	if (!cfg_manager->files.size()) { return; }
	cfg_manager->remove(cfg_manager->files[g_cfg.selected_config]);
	cfg_manager->config_files();
	if (g_cfg.selected_config > cfg_manager->files.size() - 1) {
		g_cfg.selected_config = cfg_manager->files.size() - 1;
	}
}
void add_config() {
	if (g_cfg.new_config_name.find(".cc") == -1) g_cfg.new_config_name += ".cc";
	cfg_manager->save(g_cfg.new_config_name.c_str());
	cfg_manager->config_files();
	g_cfg.new_config_name = "";
	g_cfg.selected_config = cfg_manager->files.size() - 1;
}
void unload_cheat() {
	unload = true;
}
bool Contains(const std::string &word, const std::string &sentence) {
	if (word == "" || sentence == "") return true;
	return sentence.find(word) != std::string::npos;
}
std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::tolower);
	return str;
}
void setup_main_menu() {

	static auto set_sub = [](int sub) -> void {
		g_cfg.menu.group_sub = sub;
	};
	static auto set_tabsub = [](int sub) -> void {
		g_cfg.menu.tab_sub = sub;
	};
	auto legit_tab = new C_Tab(&menu::get(), 0, "e", "C", {

		C_Tab::subtab_t("G", 0, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("I", 1, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("e", 2, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("W", 3, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("Z", 4, fonts[SUBTABWEAPONS]),

	}); {
	set_tabsub(0);
		auto legit_aimbot = new C_GroupBox(GROUP_LEFT, 8, ""); {
			auto aim_assistasd = new C_CheckBox("Enable", &g_cfg.legit.enable_legit);
			auto fovsad = new C_SliderFloat("Fov", &g_cfg.legit.fov_pistols, 0, 180, "°");
			auto smoothingasd = new C_SliderFloat("Smooth", &g_cfg.legit.smooth_pistols, 0, 15, "°");
		}
			auto legit_aasdasdasimbot = new C_GroupBox(GROUP_RIGHT, 8, ""); {
			auto aim_asdasdsistasd = new C_CheckBox("Scope Check", &g_cfg.legit.enable_legit);
			auto aim_asdsaassistasd = new C_CheckBox("Smoke Check", &g_cfg.legit.enable_legit);
			auto aim_assidsdstasd = new C_CheckBox("Silent", &g_cfg.legit.enable_legit);
		}
	set_tabsub(1);
	auto legit_aimbot1 = new C_GroupBox(GROUP_LEFT, 8, ""); {
		auto aim_assistasd = new C_CheckBox("Enable", &g_cfg.legit.enable_legit);
		auto fovsad = new C_SliderFloat("Fov", &g_cfg.legit.fov_smg, 0, 180, "°");
		auto smoothingasd = new C_SliderFloat("Smooth", &g_cfg.legit.smooth_smg, 0, 15, "°");
	}
				auto legiasdasdtss_aasimbot = new C_GroupBox(GROUP_RIGHT, 8, ""); {
			auto aim_asdasdsistasd = new C_CheckBox("Scope Check", &g_cfg.legit.enable_legit);
			auto aim_asdsaassistasd = new C_CheckBox("Smoke Check", &g_cfg.legit.enable_legit);
			auto aim_assidsdstasd = new C_CheckBox("Silent", &g_cfg.legit.enable_legit);
		}
	set_tabsub(2);
	auto legit_aimbot1e = new C_GroupBox(GROUP_LEFT, 8, ""); {
		auto aim_assistasd = new C_CheckBox("Enable", &g_cfg.legit.enable_legit);
		auto fovasd = new C_SliderFloat("Fov", &g_cfg.legit.fov_shotgun, 0, 180, "°");
		auto smoothingsda = new C_SliderFloat("Smooth", &g_cfg.legit.smooth_shotgun, 0, 15, "°");
	}
				auto legiasdastss_aasimbot = new C_GroupBox(GROUP_RIGHT, 8, ""); {
			auto aim_asdasdsistasd = new C_CheckBox("Scope Check", &g_cfg.legit.enable_legit);
			auto aim_asdsaassistasd = new C_CheckBox("Smoke Check", &g_cfg.legit.enable_legit);
			auto aim_assidsdstasd = new C_CheckBox("Silent", &g_cfg.legit.enable_legit);
		}
	set_tabsub(3);
	auto legit_aimbot1easd = new C_GroupBox(GROUP_LEFT, 8, ""); {
		auto aim_assistasdasd = new C_CheckBox("Enable", &g_cfg.legit.enable_legit);
		auto fovasdasd = new C_SliderFloat("Fov", &g_cfg.legit.fov_rifle, 0, 180, "°");
		auto smoothingsdaads = new C_SliderFloat("Smooth", &g_cfg.legit.smooth_rifle, 0, 15, "°");
	}
				auto legitsadsd_aasimssbot = new C_GroupBox(GROUP_RIGHT, 8, ""); {
			auto aim_asdasdsistasd = new C_CheckBox("Scope Check", &g_cfg.legit.enable_legit);
			auto aim_asdsaassistasd = new C_CheckBox("Smoke Check", &g_cfg.legit.enable_legit);
			auto aim_assidsdstasd = new C_CheckBox("Silent", &g_cfg.legit.enable_legit);
		}
	set_tabsub(4);
	auto legit_aimbot1easdasd = new C_GroupBox(GROUP_LEFT, 8, ""); {
		auto aim_assiasdstasdasd = new C_CheckBox("Enable", &g_cfg.legit.enable_legit);
		auto fovasdasadsd = new C_SliderFloat("Fov", &g_cfg.legit.fov_sniper, 0, 180, "°");
		auto smoothinasdgsdaads = new C_SliderFloat("Smooth", &g_cfg.legit.smooth_sniper, 0, 15, "°");
	}
			auto legit_asdassasimbot = new C_GroupBox(GROUP_RIGHT, 8, ""); {
			auto aim_asdasdsistasd = new C_CheckBox("Scope Check", &g_cfg.legit.enable_legit);
			auto aim_asdsaassistasd = new C_CheckBox("Smoke Check", &g_cfg.legit.enable_legit);
			auto aim_assidsdstasd = new C_CheckBox("Silent", &g_cfg.legit.enable_legit);
		}
	}
	auto rage_tab = new C_Tab(&menu::get(), 1, "A", "A", {


		C_Tab::subtab_t("G", 0, fonts[AIMTABICON]),
		C_Tab::subtab_t("E", 1, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("A", 2, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("W", 3, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("O", 4, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("a", 5, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("Y", 6, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("c", 7, fonts[SUBTABWEAPONS]),
		C_Tab::subtab_t("4", 8, fonts[SUBTABWEAPONS])

		}); {
		set_tabsub(0);
		auto rage = new C_GroupBox(GROUP_LEFT, 8, "Aimbot"); {
			auto enable = new C_CheckBox("enable", &g_cfg.ragebot.enable);
			auto fov = new C_SliderInt("field of view", &g_cfg.ragebot.field_of_view, 0, 360, "°");
			auto selection_type = new C_Dropdown("selection type", &g_cfg.ragebot.selection_type, { "Field Of View", "Lowest Health", "Closest Distance" });
			auto silent_aim = new C_CheckBox("silent", &g_cfg.ragebot.silent_aim);
			auto onetapp = new globalcheckbox("head check", &g_cfg.ragebot.hsonly);
			auto anti_recoil = new C_CheckBox("remove recoil", &g_cfg.ragebot.anti_recoil);
			auto auto_revolver = new C_CheckBox("auto revolver", &g_cfg.ragebot.autorevolver);
			auto autoscope = new C_CheckBox("auto scope", &g_cfg.ragebot.autoscope);
			new C_CheckBox("knifebot", &g_cfg.ragebot.knifebot);
			auto extrapolation = new C_CheckBox("extrapolation", &g_cfg.ragebot.extrapolation);
			auto bind_key = new C_KeyBind("baim key", &g_cfg.ragebot.baim_bind);

			auto zeus_bot = new globalcheckbox("zeus bot", &g_cfg.ragebot.zeus_bot);
			auto autoshoot_zeus = new C_CheckBox("fire", &g_cfg.ragebot.autoshoot[2]);
			auto minimum_damage_zeus = new C_SliderInt("min. damage zeus", &g_cfg.ragebot.minimum_damage[2], 1, 100, "");
			auto hitchance_zeus = new C_CheckBox("zeus hitchance", &g_cfg.ragebot.hitchance[2]);
			auto hitchance_amount_zeus = new C_SliderInt("", &g_cfg.ragebot.hitchance_amount[2], 0, 100, "%");
			auto hitscan_zeus = new C_MultiDropdown("hitbox", &g_cfg.ragebot.hitscan[2]);
		}
		auto rage2 = new C_GroupBox(GROUP_RIGHT, 8, "More.."); {
			auto legit_resolver = new globalcheckbox("legit resolver", &g_cfg.legit.legit_resolver_active);
			new C_CheckBox("resolver", &g_cfg.ragebot.resolver_experimental);
			new C_Dropdown("resolver mode", &g_cfg.ragebot.resolver_experimentalmode, { "type 1", "type 2", "type 3 (exclusive)", "desync resolver" });
			new C_CheckBox("adaptive resolver", &g_cfg.ragebot.usedesyncresolvemdoe);
			//	new C_KeyBind("Slidewalk Key", &g_cfg.misc.slidewalk_key);
			auto override_key = new C_KeyBind("override resolver", &g_cfg.ragebot.override_key);
			//auto aimstep = new C_CheckBox("Aim Step", &g_cfg.ragebot.aimstep);
			//auto lagcomp = new C_CheckBox("Lag Compensation", &g_cfg.ragebot.lagcomp);
			//auto lag_compensation_type = new C_Dropdown("Compensation Type", &g_cfg.ragebot.lag_compensation_type, { "Refine Shot", "Prediction" });
			new checkboxbind("fake duck", &g_cfg.misc.unlimited_duck, &g_cfg.misc.fakeduck);
			auto shsaah = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto sahsahas = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto hsahashah = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto hashh434 = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto hsahdsaashah = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto hashhdsa434 = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto asdsad = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto asd = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto haha = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto ddhdfh = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto ashahasawerthhah = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto ashsajgwahahah = new C_TextSeperator("", SEPERATOR_NORMAL);
			auto ashahasjgjgahah = new C_TextSeperator("", SEPERATOR_NORMAL);
			new slidercheckbox("double tap", &g_cfg.ragebot.doubletap_enable, &g_cfg.ragebot.doubletap_ticks, 0, 7);
		}
		for (int i = 0; i < 7; i++) {
			set_tabsub(i + 1);
			auto weapon = new C_GroupBox(GROUP_LEFT, 8, "Weapons Settings", 2); {
				for (int s = 0; s < 2; s++) {
					set_sub(s);
					if (s == 0)
					{
						auto autoshoot = new C_CheckBox("auto fire", &g_cfg.ragebot.autoshoot[i]);
						auto autowall = new C_CheckBox("auto wall", &g_cfg.ragebot.autowall[i]);
						auto minimum_damage = new C_SliderInt("", &g_cfg.ragebot.minimum_damage[i], 1, 100, "");
						auto hitchance = new C_CheckBox("hitchance", &g_cfg.ragebot.hitchance[i]);
						auto hitchance_amount = new C_SliderInt("", &g_cfg.ragebot.hitchance_amount[i], 0, 100, "%");
						auto hitscan = new C_MultiDropdown("hitboxes", &g_cfg.ragebot.hitscan[i]);
						auto hitscan_history = new C_MultiDropdown("hitbox history", &g_cfg.ragebot.hitscan_history[i]);
						auto multipoint = new C_MultiDropdown("multipoint", &g_cfg.ragebot.multipoint_hitboxes[i]);
					}
					else if (s == 1)
					{
						new C_SliderFloat("head", &g_cfg.ragebot.pointscalehead[i], 0, 0.3, "");
						new C_SliderFloat("neck", &g_cfg.ragebot.pointscaleneck[i], 0, 0.3, "");
						new C_SliderFloat("chest", &g_cfg.ragebot.pointscalechest[i], 0, 0.3, "");
						new C_SliderFloat("stomach", &g_cfg.ragebot.pointscalestomach[i], 0, 0.3, "");
						new C_SliderFloat("pelvis", &g_cfg.ragebot.pointscalepelvis[i], 0, 0.3, "");
						new C_SliderFloat("arms", &g_cfg.ragebot.pointscalearms[i], 0, 0.3, "");
						new C_SliderFloat("legs", &g_cfg.ragebot.pointscalelegs[i], 0, 0.3, "");
						new C_SliderFloat("other", &g_cfg.ragebot.pointscale[i], 0, 0.3, "");
					}
				}
			}
			auto weapon2 = new C_GroupBox(GROUP_RIGHT, 8, "Other Aim"); {
				new C_MultiDropdown("body", &g_cfg.ragebot.baim_settings[i]);
				new C_CheckBox("quick Stop", &g_cfg.ragebot.quickstop[i]);
				new C_CheckBox("auto Stop", &g_cfg.ragebot.autostopmovement[i]);
				new C_CheckBox("fakewalk", &g_cfg.misc.fakewalk);
				new C_KeyBind("fakewalk key", &g_cfg.misc.fakewalk_key);
			}
		}
		//set_sub(10);
		//auto knifebbb0t = new C_GroupBox(GROUP_LEFT, 8, "KnifeBot", 2); {
		//}
	}
	auto antiaim_tab = new C_Tab(&menu::get(), 2, "B", "B"); {
		auto antiaim = new C_GroupBox(GROUP_LEFT, 8, "AntiAims", 3); {
			char * type[4] = { "Standing", "Moving", "In Air", "Edge" };
			for (int i = 0; i < 3; i++) {
				set_sub(i);
				auto asdadasda = new C_TextSeperator("", SEPERATOR_BOLD);
				new C_CheckBox("enable", &g_cfg.antiaim.enable);
				auto antiaim_type = new C_TextSeperator(type[i], SEPERATOR_BOLD);
				auto base_angle = new C_Dropdown("Base Angle", &g_cfg.antiaim.base_angle[i], { "Static", "Crosshair", "At Targets" });
				std::vector< std::string > pitchx{ "Off", "Down", "Up", "Jitter" };
				auto pitch = new C_Dropdown("Pitch", &g_cfg.antiaim.pitch[i], pitchx);
				std::vector< std::string > reals{ "Off", "Offset", "Rotate", "Jitter", "LBY", "LBY + Delta", "Auto Direction","180z" };
				auto yaw = new C_Dropdown("Real Yaw", &g_cfg.antiaim.yaw[i], reals);
				auto yaw_offset = new C_SliderInt("", &g_cfg.antiaim.yaw_offset[i], -180.f, 180.f, "°");
				auto balance_breaker = new C_CheckBox("balance breaker", &g_cfg.antiaim.break_adjustment[i]);
				auto desync = new C_CheckBox("desync", &g_cfg.antiaim.antiaim_desync[i]);
				auto desync_mode = new C_Dropdown("", &g_cfg.antiaim.desync_mode[i], { "Jitter", "Static" });
				auto animation_fix = new globalcheckbox("epic animfix", &g_cfg.antiaim.animfix);
			}
		}
		auto antiaim_2 = new C_GroupBox(GROUP_RIGHT, 8, "Other", 3); {
			char * type2[3] = { "Manual", "Fakelag", "Other" };
			for (int i = 0; i < 3; i++) {
				set_sub(i);
				auto antiaim_page2 = new C_TextSeperator(type2[i], SEPERATOR_BOLD);
				if (i == 0)
				{
					auto manualkey_left = new C_KeyBind("Manual Key Left", &g_cfg.antiaim.left_manualkey);
					auto manualkey_right = new C_KeyBind("Manual Key Right", &g_cfg.antiaim.right_manualkey);
					auto manualkey_back = new C_KeyBind("Manual key Back", &g_cfg.antiaim.back_manualkey);
					auto manualaafakes = new C_Dropdown("Manual AntiAim Fakes", &g_cfg.antiaim.manualaafakes, { "Jitter around Reals", "Jitter between max delta", "Spin around Reals" });
					//auto unhit_aa_type = new C_Dropdown("unhittable aa", &g_cfg.ragebot.aa_types, { "Off", "Backwards", "Sideways", "BackJitter", "LBY Breaker", "Slow Spin" });
					new C_CheckBox("P2C breaker", &g_cfg.antiaim.p2cbreaker);
					new C_CheckBox("break 979", &g_cfg.antiaim.ninesevenlbybreaker);
					new globalcheckbox("air desync", &g_cfg.antiaim.air_desync);
				}
				else if (i == 1)
				{
					auto fakelags_sep = new C_TextSeperator("fakelags", SEPERATOR_NORMAL);
					auto fake_chams = new C_CheckBox("fakelag chams", &g_cfg.esp.fake_chams,
						{ new C_ColorSelector(&g_cfg.esp.fake_chams_color) });
					auto fakelag_enablers = new C_MultiDropdown("fakelag", &g_cfg.antiaim.fakelag_enablers);
					auto fakelag_mode = new C_Dropdown("", &g_cfg.antiaim.fakelag_mode, { "Factor", "Dynamic", "Maximum" });
					auto fakelag_amount = new C_SliderInt("", &g_cfg.antiaim.fakelag_amount, 1, 14, "");
				}
				else if (i == 2)
				{
					auto enable_shift_yaw = new C_CheckBox("enable shift aa", &g_cfg.antiaim.enable_shifts[i]);
					auto slider_shift = new C_SliderInt("shift radius", &g_cfg.antiaim.shift_yaw[i], 0.f, 90.f, "°");
					auto Enable_swich_yaw = new C_CheckBox("enable switch", &g_cfg.antiaim.enable_switching_yaw[i]);
					auto slider_switch_time = new C_SliderInt("desync range", &g_cfg.antiaim.desync_delta_swap_st[i], 0.f, 180.f, "");
					auto slider_desync_jitter = new C_SliderInt("jitter desync range", &g_cfg.antiaim.desyncjitterrange[i], 0.f, 58.f, "");
					auto static_desync_modew = new C_Dropdown("static desync", &g_cfg.antiaim.static_desync_type[i], { "Max", "Still" });
					auto enable_tank_aa = new C_CheckBox("tank aa", &g_cfg.antiaim.tank_aa[i]);
				}
			}
		}
	}
	auto esp_tab = new C_Tab(&menu::get(), 3, "D", "D", {
		C_Tab::subtab_t("Player", 0, fonts[TABFONT]),
		C_Tab::subtab_t("ESP", 1, fonts[TABFONT]),
		C_Tab::subtab_t("Other visuals", 2, fonts[TABFONT])
		}); {
		set_tabsub(0);
		auto player = new C_GroupBox(GROUP_LEFT, 8, "Visual Player"); {
			auto enable = new C_CheckBox("enable", &g_cfg.player.enable);
			auto alpha = new C_SliderInt("alpha", &g_cfg.player.alpha, 0, 255, "");
			auto box = new C_CheckBox("box", &g_cfg.player.box,
				{ new C_ColorSelector(&g_cfg.player.box_color) });
			auto health = new C_CheckBox("health", &g_cfg.player.health);
			auto armor = new C_CheckBox("armor bar", &g_cfg.player.armorbar);
			auto flash_bar = new C_CheckBox("flash bar", &g_cfg.player.flash_bar);
			auto name = new C_CheckBox("name esp", &g_cfg.player.name,
				{ new C_ColorSelector(&g_cfg.player.name_color) });
			auto weaponselect = new C_Dropdown("weapon", &g_cfg.player.weapon, { "Text", "Icon" });
			auto weapon_сolor = new C_TextSeperator("Weapon Color Bar", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.weapon_color) });
			auto ammo = new C_Dropdown("ammo", &g_cfg.player.ammo, { "Off", "Bar" });
			auto ammobar_color = new C_TextSeperator("Ammo Color Bar", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.ammobar_color) });
			auto lby_bar = new C_CheckBox("LBY bar", &g_cfg.player.lby,
				{ new C_ColorSelector(&g_cfg.player.lbybar_color) });
			auto flags = new C_MultiDropdown("flags", &g_cfg.player.flags);
			auto skeleton = new C_MultiDropdown("skeleton", &g_cfg.player.skeleton);
		}
		auto player_2 = new C_GroupBox(GROUP_RIGHT, 8, "Other Esp"); {
			auto glow = new C_MultiDropdown("glow", &g_cfg.player.glow);
			auto glow_color_enemy = new C_TextSeperator("Enemy Color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.glow_color_enemy) });
			auto glow_color_teammate = new C_TextSeperator("Teammate Color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.glow_color_teammate) });
			auto glow_color_local = new C_TextSeperator("Local Color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.glow_color_local) });
			auto glow_opacity = new C_SliderInt("", &g_cfg.player.glowopacity, 1, 100, "%%");
			auto glow_bloom = new C_SliderInt("", &g_cfg.player.glowbloom, 1, 100, "%%");
			auto glow_type = new C_Dropdown("", &g_cfg.player.glow_type, { "Outline Outer", "Cover", "Outline Inner" });
			auto chams = new C_MultiDropdown("Chams", &g_cfg.player.chams);
			auto chams_color_visible = new C_TextSeperator("Visible Color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.chams_color) });
			auto chams_color_invisible = new C_TextSeperator("Invisible Color", SEPERATOR_NORMAL,
				{ new C_ColorSelector(&g_cfg.player.xqz_color) });
			auto chams_opacity = new C_SliderInt("", &g_cfg.player.chams_opacity, 0, 100, "%%");
			auto chams_type = new C_Dropdown("", &g_cfg.player.chams_type, { "Regular", "Flat", "Cologne", "Metallic" });
			auto backtrack_chams = new C_CheckBox("backtrack model", &g_cfg.player.backtrack_chams,
				{ new C_ColorSelector(&g_cfg.player.backtrack_chams_color) });
			auto backtrack_chams_opacity = new C_SliderInt("", &g_cfg.player.backtrack_chams_opacity, 0, 100, "%%");
			auto esp_view = new C_CheckBox("esp preview", &g_cfg.player.esp_view);
		}
		set_tabsub(1);
		auto esp = new C_GroupBox(GROUP_LEFT, 8, "World Esp"); {
			auto fov = new C_SliderInt("override fov", &g_cfg.esp.fov, 0, 89, "°");
			auto override_fov_scoped = new C_CheckBox("override fov when scoped", &g_cfg.esp.override_fov_scoped);
			auto transparency_scoped = new C_CheckBox("transparency when scoped", &g_cfg.esp.transparency_scoped);
			auto transparency_scoped_alpha = new C_SliderInt("", &g_cfg.esp.transparency_scoped_alpha, 0, 50, "%%");
			auto spectator_list = new C_CheckBox("spectators", &g_cfg.esp.spectator_list);
			auto spectator_x = new C_SliderInt("spectator X", &g_cfg.esp.spectator_list_x, 0, 2000, "X");
			auto spectator_y = new C_SliderInt("spectator Y", &g_cfg.esp.spectator_list_y, 0, 2000, "Y");
			auto espbomba = new C_CheckBox("esp bomb", &g_cfg.esp.espbomb);
			//auto show_spread = new C_CheckBox("Draw Spread", &g_cfg.esp.show_spread,
			//	{ new C_ColorSelector(&g_cfg.esp.show_spread_color) });
			//auto show_spread_type = new C_Dropdown("", &g_cfg.esp.show_spread_type, { "Regular", "Rainbow", "Seed" });
			auto indicators = new C_MultiDropdown("indicators", &g_cfg.esp.indicators);
			auto safdsafa = new C_TextSeperator("others", SEPERATOR_NORMAL);
			auto check_hits = new C_CheckBox("check damage", &g_cfg.esp.check_hit);

		}
		auto esp_2 = new C_GroupBox(GROUP_RIGHT, 8, "Other Esp"); {
			auto dropped_weapon = new C_CheckBox("dropped weapons", &g_cfg.esp.dropped_weapons,
				{ new C_ColorSelector(&g_cfg.esp.dropped_weapons_color) });
			auto projectiles = new C_CheckBox("dropped grenades", &g_cfg.esp.projectiles,
				{ new C_ColorSelector(&g_cfg.esp.projectiles_color) });
			auto dont_render_teammates = new C_CheckBox("invisible teammates", &g_cfg.esp.dont_render_teammates);
			auto nightmode = new C_CheckBox("nightmode", &g_cfg.esp.nightmode);
			auto hitmarker = new C_MultiDropdown("hitmarker", &g_cfg.esp.hitmarker);
			auto hitsound = new C_CheckBox("hitsound", &g_cfg.esp.hitsound);
			auto thirdperson = new C_KeyBind("thirdperson key", &g_cfg.misc.thirdperson_toggle);
			auto events_to_log = new C_MultiDropdown("event log", &g_cfg.misc.events_to_log);
			auto crosshair = new C_CheckBox("crosshair", &g_cfg.esp.crosshair);
			auto sniper_crosshair = new C_CheckBox("sniper crosshair", &g_cfg.misc.sniper_crosshair);
			new C_CheckBox("kill feed", &g_cfg.esp.bright);
			new C_CheckBox("full bright", &g_cfg.esp.fullbright);
			auto bullet_tracer = new C_CheckBox("tracers", &g_cfg.esp.bullet_tracer,
				{ new C_ColorSelector(&g_cfg.esp.bullet_tracer_color) });
			//auto grenade_pred = new C_CheckBox("Grenade Prediction", &g_cfg.esp.grenade_enable);
			auto antiaimslinea = new C_CheckBox("aa lines", &g_cfg.esp.anti_aim_lines);
		}
		set_tabsub(2);
		auto owo = new C_GroupBox(GROUP_LEFT, 8, "Removals"); {
			auto removescope = new C_CheckBox("remove scope", &g_cfg.esp.removescope, { new C_ColorSelector(&g_cfg.menu.removescopecol[0]) });
			auto removescopesense = new C_CheckBox("remove scope sensitivity", &g_cfg.esp.removescopesensitivity);
			auto removezoom = new C_CheckBox("remove zoom effect", &g_cfg.esp.removezoom);
			auto removesmoke = new C_CheckBox("remove smokes", &g_cfg.esp.removesmoke);
			auto removeflash = new C_CheckBox("remove flashbang effect", &g_cfg.esp.removeflash);
			auto removerecoil = new C_CheckBox("remove visual recoil", &g_cfg.esp.removerecoil);
		}
		auto owo2 = new C_GroupBox(GROUP_RIGHT, 8, "Others");
		{
			auto bullet_tracer_enemy = new C_CheckBox("enemy tracers", &g_cfg.esp.bullet_tracer_enemy,
				{ new C_ColorSelector(&g_cfg.esp.bullet_tracer_color_enemy) });
			auto bullet_tracerlifespan = new C_SliderInt("tracer time", &g_cfg.esp.bullet_tracer_duration, 1, 10, "");
			auto bullet_tracersize = new C_SliderInt("tracer size", &g_cfg.esp.bullet_tracer_size, 1, 10, "");
			auto zeusrange = new C_CheckBox("zeus range", &g_cfg.esp.zeusrange);
			new C_Dropdown("Gun Chams", &g_cfg.esp.gunchams, { "off", "simple", "wireframe", "golden", "glass", "rainbow" , "test" },
				{ new C_ColorSelector(&g_cfg.esp.gunchamscolor) });
			auto hand_chams_selector = new C_Dropdown("hand chams", &g_cfg.esp.handchams, { "Off", "Crystal Clear", "Plastic", "Glass" });
			auto isafoidsaasfoas = new C_TextSeperator("player chams", SEPERATOR_BOLD);
			auto ghost_chams = new C_CheckBox("ghost chams", &g_cfg.esp.ghost_chams,
				{ new C_ColorSelector(&g_cfg.esp.ghost_chams_color) });
			auto local_chams = new C_CheckBox("local chams", &g_cfg.esp.local_chams,
				{ new C_ColorSelector(&g_cfg.esp.local_chams_color) });
		}
	}
	auto misc_tab = new C_Tab(&menu::get(), 4, "G", "G", {
		C_Tab::subtab_t("Misc", 0, fonts[TABFONT]),
		C_Tab::subtab_t("Settings", 1, fonts[TABFONT])
	}); {
	set_tabsub(0);
		auto miscellaneous = new C_GroupBox(GROUP_LEFT, 8, "Misc"); {
			new C_Dropdown("watermark", &g_cfg.misc.watermark, { "off", "gamesense", "default" });
			new C_Dropdown("status bar", &g_cfg.misc.status_bar, { "off", "type 1", "type 2" });
			auto bunnyhop = new C_CheckBox("bunnyhop", &g_cfg.misc.bunnyhop);
			auto show_key = new C_CheckBox("show keys", &g_cfg.misc.key_binds);
			auto airstrafe = new C_CheckBox("auto strafe", &g_cfg.misc.airstrafe);
			auto circle_stafer = new C_KeyBind("circle strafe", &g_cfg.misc.circle_strafe);
			new C_Dropdown("clantag", &g_cfg.misc.clantag_modes, { "off", "moneysense" });
			auto clantag_changerprefix2 = new C_Dropdown("clantag prefix", &g_cfg.misc.clantagprefixmodes, { "off", "先輩", "$", "♕" , "♿", "180 名", "☭" });
		}
		auto miscellaneous_2 = new C_GroupBox(GROUP_RIGHT, 8, "Other Misc"); {
			auto exaggerated_ragdoll = new slidercheckbox("exaggerated ragdolls", &g_cfg.misc.exaggerated_ragdolls, &g_cfg.misc.exaggerated_ragdolls_multiplier, 2, 100);
			auto ping_spike = new slidercheckbox("fake latency", &g_cfg.misc.ping_spike, &g_cfg.misc.ping_spike_value, -10, 999);
			auto buybot_separator = new C_TextSeperator("buybot", SEPERATOR_BOLD);
			auto buybot = new C_CheckBox("enable", &g_cfg.BuyBot.enable);
			auto buybot_primary = new C_MultiDropdown("primary", &g_cfg.BuyBot.primary);
			auto buybot_secondary = new C_MultiDropdown("secondary", &g_cfg.BuyBot.secondary);
			auto buybot_nades = new C_MultiDropdown("grenades", &g_cfg.BuyBot.nades);
			auto buybot_extras = new C_MultiDropdown("extras", &g_cfg.BuyBot.extras);
		}
		set_tabsub(1);
		auto config = new C_GroupBox(GROUP_LEFT, 8, "Settings"); {
			new C_TextSeperator("menu color", SEPERATOR_NORMAL, { new C_ColorSelector(&g_cfg.menu.menu_theme[0]) });
			new C_KeyBind("menu key", &g_cfg.menu.menu_bind);
			new C_CheckBox("backdrop", &g_cfg.misc.backend);
			new C_CheckBox("anti unstrusted", &g_cfg.misc.anti_untrusted);
			new C_CheckBox("auto defuse", &g_cfg.misc.autodefuse);
			new C_CheckBox("custom viewmodel", &g_cfg.misc.customviewmodel);
			new C_SliderFloat("> X", &g_cfg.misc.CustomViewModelX, -10, 10, "");
			new C_SliderFloat("> Y", &g_cfg.misc.CustomViewModelY, -10, 10, "");
			new C_SliderFloat("> Z", &g_cfg.misc.CustomViewModelZ, -10, 10, "");
			//new C_Button("Select Weapons", []() { menu::get().draw_tab_weapons; });
		}
		auto config_2 = new C_GroupBox(GROUP_RIGHT, 8, "Configs"); {
			auto config_dropdown = new C_Dropdown("", &g_cfg.selected_config, &cfg_manager->files);
			auto function_seperator = new C_TextSeperator("Configuration Settings", SEPERATOR_BOLD);
			auto load = new C_Button("load config", []() { load_config(); });
			auto save = new C_Button("save config", []() { save_config(); });
			auto remove = new C_Button("delete config", []() { remove_config(); });
			new C_TextField("configuration name", &g_cfg.new_config_name);
			auto add = new C_Button("add new config", []() { add_config(); });
			new C_Button("unload cheat", []() { unload_cheat(); });
			new C_Button("rage quit", []() { exit(1); });
		}
	}
	auto skinchanger_tab = new C_Tab(&menu::get(), 5, "H", "H", {
		C_Tab::subtab_t("W", 0, fonts[WEAPONICONS]),
		C_Tab::subtab_t("4", 1, fonts[WEAPONICONS])
		}); {
		set_tabsub(0);
		auto knifechanger = new C_GroupBox(GROUP_LEFT, 12, "Skin Changer");
		{
			auto skinson = new C_CheckBox("Enable Skinchanger", &g_cfg.skins.skinenabled);
			auto awpns = new C_Dropdown("Awp skins", &g_cfg.skins.AWPSkin,
				{ "none",
				"Asiimov",
				"Dragon Lore",
				"Fever Dream",
				"Medusa",
				"HyperBeast",
				"Boom",
				"Lightning Strike",
				"Pink DDpat",
				"Corticera",
				"Redline",
				"Manowar",
				"Graphite",
				"Electric Hive",
				"Sun in Leo",
				"Hyper Beast",
				"Pit viper",
				"Phobos",
				"Elite Build",
				"Worm God",
				"Oni Taiji",
				"Fever Dream" });
			auto scoutns = new C_Dropdown("SSG08 skins", &g_cfg.skins.SSG08Skin,
				{ "none",
				"Lichen Dashed",
				"Dark Water",
				"Blue Spruce",
				"Sand Dune",
				"Palm",
				"Mayan Dreams",
				"Blood in the Water",
				"Tropical Storm",
				"Acid Fade",
				"Slashed",
				"Detour",
				"Abyss",
				"Big Iron",
				"Necropos",
				"Ghost Crusader",
				"Dragonfire" });
			auto scarns = new C_Dropdown("Scar skins", &g_cfg.skins.SCAR20Skin,
				{ "none",
				"Splash Jam",
				"Storm",
				"Contractor",
				"Carbon Fiber",
				"Sand Mesh",
				"Palm",
				"Crimson Web",
				"Cardiac",
				"Army Sheen",
				"Cyrex",
				"Grotto",
				"Bloodsport" });
			auto glockns = new C_Dropdown("Glock skins", &g_cfg.skins.GlockSkin,
				{ "none",
				"Fade",
				"Dragon Tattoo",
				"Twilight Galaxy",
				"Wasteland Rebel",
				"Water Elemental",
				"Off World",
				"Weasel",
				"Royal Legion",
				"Grinder",
				"Steel Disruption",
				"Brass",
				"Ironwork",
				"Bunsen Burner",
				"Reactor" });
			auto G3sg1ns = new C_Dropdown("G3sg1 skins", &g_cfg.skins.G3sg1Skin,
				{ "none",
				"Hunter",
				"The Executioner",
				"Terrace",
				"Neon Kimono",
				"Orange Kimono",
				"Predator",
				"Chronos"
				});
			auto dualsns = new C_Dropdown("Duals skins", &g_cfg.skins.DualSkin,
				{ "none",
				"Panther",
				"Dualing Dragons",
				"Cobra Strike",
				"Royal Consorts",
				"Duelist" });
			auto deaglens = new C_Dropdown("Deagle skins", &g_cfg.skins.DeagleSkin,
				{ "none",
				"Blaze",
				"Kumicho Dragon",
				"Oxide Blaze",
				"Golden Koi",
				"Cobalt Disruption",
				"Directive" });
			auto uspns = new C_Dropdown("USP skins", &g_cfg.skins.USPSkin,
				{ "none",
				"Neo-Noir",
				"Cyrex",
				"Orion",
				"Kill Confirmed",
				"Overgrowth",
				"Caiman",
				"Serum",
				"Guardian",
				"Road Rash" });
		}
		auto knifechanger2222222 = new C_GroupBox(GROUP_RIGHT, 12, "Skins");
		{
			auto ak = new C_Dropdown("AK47 skins", &g_cfg.skins.AK47Skin,
				{ "none",
				"Fire serpent",
				"Fuel Injector",
				"Bloodsport",
				"Vulcan",
				"Case hardened",
				"Hydroponic",
				"Aquamarine Revenge",
				"Frontside Misty",
				"Point Disarray",
				"Neon Revolution",
				"Red laminate",
				"Redline",
				"Jaguar",
				"Jetset",
				"Wasteland rebel",
				"The Empress",
				"Elite Build" });
			auto P2000 = new C_Dropdown("P2000 skins", &g_cfg.skins.P2000Skin,
				{ "none",
				"Handgun",
				"Fade",
				"Corticera",
				"Ocean Foam",
				"Fire Elemental",
				"Asterion",
				"Pathfinder",
				"Imperial",
				"Oceanic",
				"Imperial Dragon",
				});
			auto uspns = new C_Dropdown("Revolver skins", &g_cfg.skins.RevolverSkin,
				{ "none",
				"Llama Cannon",
				"Fade",
				"Crimson Web",
				"Reboot",
				"Nitro",
				"Bone Mask"
				});
			auto forceupdate = new C_Button("Full Update", []() { updatechanger::get().update_skins(); });
			set_tabsub(1);
			auto knifechanger2 = new C_GroupBox(GROUP_LEFT, 12, "KnifeChanger");
			{
				auto knif = new C_Dropdown("Knife Model", &g_cfg.skins.Knife, { "Default",
					"Bayonet",
					"Flip Knife",
					"Gut Knife",
					"Karambit",
					"M9 Bayonet",
					"Huntsman Knife",
					"Butterfly Knife",
					"Falchion Knife",
					"Shadow Daggers",
					"Bowie Knife",
					"Navaja Knife",
					"Stiletto Knife",
					"Ursus Knife",
					"Talon Knife" });
				auto knifskin = new C_Dropdown("Knife Skin", &g_cfg.skins.KnifeSkin, { "none",
					"Crimson Web",
					"Bone Mask",
					"Fade",
					"Night",
					"Blue Steel",
					"Stained",
					"Case Hardened",
					"Slaughter",
					"Safari Mesh",
					"Boreal Forest",
					"Ultraviolet",
					"Urban Masked",
					"Scorched",
					"Rust Coat",
					"Tiger Tooth",
					"Damascus Steel",
					"Damascus Steel",
					"Marble Fade",
					"Rust Coat",
					"Doppler Ruby",
					"Doppler Sapphire",
					"Doppler Blackpearl",
					"Doppler Phase 1",
					"Doppler Phase 2",
					"Doppler Phase 3",
					"Doppler Phase 4",
					"Gamma Doppler Phase 1",
					"Gamma Doppler Phase 2",
					"Gamma Doppler Phase 3",
					"Gamma Doppler Phase 4",
					"Gamma Doppler Emerald",
					"Lore",
					"Black Laminate",
					"Autotronic",
					"Freehand" });
				auto forceupdate = new C_Button("Full Update", []() { updatechanger::get().update_skins(); });
			}
			auto knifechanger3 = new C_GroupBox(GROUP_RIGHT, 12, "GloveChanger(soon)");
			{
				auto forceupdate = new C_Button("Full Update", []() { updatechanger::get().update_skins(); });
			}
		}
	}
}