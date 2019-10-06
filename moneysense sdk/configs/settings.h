#pragma once

#include "..\menu\cmenu.hpp"

#include "..\sdk\misc\Color.hpp"
#include "..\sdk\interfaces\IInputSystem.hpp"
#include <set>
#include <map>
#include "..\valvesdk\json.hpp"
#include <unordered_map>
class Color;
class C_GroupBox;
class C_Tab;
struct MultiDropdownItem_t;

using json = nlohmann::json;

struct skinInfo
{
	int seed = -1;
	int paintkit;
	std::string tagName;
};
class Weapon_Autism
{
public:
	int SkinsWeapon;
	int SkinsKnife;
	int VremennyiWeapon;
	int VremennyiKnife;
	int Stikers1;
	int Stikers2;
	int Stikers3;
	int Stikers4;
	float ChangerWear = 0;
	int ChangerStatTrak = 0;
	char ChangerName[32] = "";
	bool ChangerEnabled;
};
class C_ConfigManager {
	class C_ConfigItem {
	public:
		std::string name;
		void *pointer;
		std::string type;

		C_ConfigItem(std::string name, void *pointer, std::string type) {
			this->name = name;
			this->pointer = pointer;
			this->type = type;
		}
	};

protected:
	std::vector<C_ConfigItem*> items;
private:
	void add_item(void * pointer, const char * name, std::string type);
	void setup_item(int *, int, std::string);
	void setup_item(bool *, bool, std::string);
	void setup_item(float *, float, std::string);
	void setup_item(ButtonCode_t *, ButtonCode_t, std::string);
	void setup_item(Color *, Color, std::string);
	void setup_item(std::vector< bool > *, std::string);
	void setup_item(std::vector< int > *, std::string);
	void setup_item(std::vector<MultiDropdownItem_t> *, std::vector< MultiDropdownItem_t >, std::string);
public:
	C_ConfigManager() { setup(); };

	void setup();
	void save(std::string config);
	void load(std::string config);
	void remove(std::string config);

	std::vector<std::string> files;
	std::vector<std::string> files_lua;
	void config_files();
	void lua_files();
};

extern C_ConfigManager * cfg_manager;

enum {
	FLAGS_SCOPED,
	FLAGS_BALANCE,
	FLAGS_ARMOR,
	FLAGS_VULNERABLE,
	FLAGS_LBY,
	FLAGS_RESOLVERMODE,
	FLAGS_BREAKINGLC,
	FLAGS_DEBUG,
	FLAGS_SLOWWALK,
	FLAGS_RELOADING
};

enum {
	PLAYER_CHAMS_VISIBLE,
	PLAYER_CHAMS_INVISIBLE
};

enum {
	PENETRATION,
	ANTIAIM,
	CROCKED,
	EXTEND,
	LC,
	DOUBLETAP,
	DESYNC
};

enum {
	SKELETON_NORMAL,
	SKELETON_BACKTRACK
};

enum {
	GLOW_ENEMY,
	GLOW_TEAMMATE,
	GLOW_LOCAL
};

enum {
	AAINDICATOR_FIRSTPERSON,
	AAINDICATOR_THIRDPERSON
};

enum {
	REMOVALS_SCOPE,
	REMOVALS_SMOKE,
	REMOVALS_FLASH,
	REMOVALS_RECOIL
};

enum {
	HITMARKER_STATIC,
	HITMARKER_DYNAMIC
};

enum {
	BAIM_INAIR,
	BAIM_NOTRESOLVED,
	BAIM_CANKILL,
	BAIM_BREAKLBY,
	BAIM_SLOWWALK,
	BAIM_ALWAYS
};
enum {
	NADES_HE,
	NADES_MOLLY,
	NADES_FLASH,
	NADES_FLASH2,
	NADES_SMOKE,
	NADES_DECOY
};
/*			{ false, "he-nade" },
			{ false, "molotov" },
			{ false, "flash" },
			{ false, "flash" },
			{ false, "smoke" },
			{ false, "decoy" }*/

enum {
	EVENTLOG_HIT,
	EVENTLOG_ITEM_PURCHASES,
	EVENTLOG_PLANTING,
	EVENTLOG_DEFUSING
};

enum {
	FAKELAG_MOVING,
	FAKELAG_AIR,
	FAKELAG_STANDING,
	FAKELAG_SHOOTING,
	FAKELAG_UNDUCK,
	FAKELAG_RELOAD,
	FAKELAG_WEAPONSWITCH,
	FAKELAG_ONPEEK,
	FAKELAG_ONKEY
};
/*		{ false, "shooting" },
		{ false, "on unduck" },
		{ false, "on reload" },
		{ false, "on weaponswitch" },
		{ false, "on peek" }*/
struct Config {
	struct Ragebot_t {
		bool enable;
		int field_of_view;
		int selection_type;
		int lag_compensation_type;
		std::vector< MultiDropdownItem_t > lag_compensation_when;
		bool silent_aim;
		bool hsonly;
		bool knifebot;
		bool anti_recoil;
		bool anti_spread;
		bool zeus_bot;
		bool tap;
		bool autorevolver;
		bool extrapolation;
		bool resolver_experimental;
		int resolver_experimentalmode;
		bool resolver_prefferMLBYoverFreestand;
		bool jumpscouting;
		bool aimstep;
		bool antiaim_correction;
		bool antiaim_correction2;
		bool antiaim_correction3;
		bool antiaim_correction3testingmode;
		bool correctlbybreaker;
		bool predictlbyupdate;
		bool freestand;
		//int aa_types;
		//std::vector< char* > aa_type{ "Backwards", "Sideways", "BackJitter", "Legit AA" };
		int antiaim_correction3modes;
		ButtonCode_t override_key;
		bool usedesyncresolvemdoe;
		ButtonCode_t baim_bind;
		int bruteshots;
		bool velocity_prediction;
		bool lagcomp;
		float lagcomp_time;
		int dynamicpointscale;

		bool doubletap_enable;
		int doubletap_ticks;

		std::vector< char * > weapon_names{ "pistols", "deagle", "rifles", "smgs", "snipers", "autos", "shotguns" };


		bool autoshoot[7];
		bool autowall[7];
		int minimum_damage[7];
		bool autoscope;
		std::vector< MultiDropdownItem_t > hitscan[7];
		std::vector< MultiDropdownItem_t > multipoint_hitboxes[7];
		std::vector< MultiDropdownItem_t > hitscan_history[7];
		float pointscalehead[7];
		float pointscalepelvis[7];
		float pointscalelegs[7];
		float pointscalearms[7];
		float pointscalestomach[7];
		float pointscaleneck[7];
		float pointscalechest[7];
		float pointscale[7];
		bool hitchance[7];
		int hitchance_amount[7];
		std::vector< MultiDropdownItem_t > baim_settings[7];
		bool quickstop[7];
		bool autofakewalk[7];
		bool autostopmovement[7];
		int baimifhp[7];
	}ragebot;

	struct Legitbot_t {

		bool enable_legit;
		bool legit_resolver_active;

		float fov_pistols;
		float smooth_pistols;

		bool aa_active;

		float fov_smg;
		float smooth_smg;

		float fov_shotgun;
		float smooth_shotgun;

		float fov_rifle;
		float smooth_rifle;

		float fov_sniper;
		float smooth_sniper;

		bool silient;
		float silient_fov;

	}legit;

	struct AntiAim_t {
		bool enable;

		bool lby_breaker;
		int lby_delta;
		bool air_desync;
		bool pitch_flick;
		bool hittinggroundpitch;
		bool break_adjustment[4];

		int base_angle[4];

		int pitch[4];

		int yaw[4];
		int yaw_offset[4];
		bool desync;

		int fake_yaw;
		int fake_yaw_offset;

		float desync_amout;



		bool enable_edge;

		ButtonCode_t left_manualkey;
		ButtonCode_t right_manualkey;
		ButtonCode_t back_manualkey;
		ButtonCode_t disable_manualkey;
		int manualaafakes;
		bool render_manual;
		bool fakecourouch;
		bool fakekrautsh;
		bool usenewfakelag;
		int fakelag_mode;
		std::vector< MultiDropdownItem_t > fakelag_enablers;
		int fakelag_amount;
		int fakelag_amountstanding;
		int fakelag_amountmoving;
		int fakelag_amountinair;
		int fakelag_amountother;//shoot
		bool fakelagchams;
		/*
			FAKELAG_MOVING,
			FAKELAG_AIR,
			FAKELAG_STANDING,
			FAKELAG_SHOOTING,
			FAKELAG_UNDUCK,
			FAKELAG_RELOAD,
			FAKELAG_WEAPONSWITCH,
			FAKELAG_ONPEEK
		*/
		int fl_amtonpeek;
		int fl_amtunduck;
		int fl_amtreload;
		int fl_amtweaponswitch;
		int fl_amtonkey;
		bool auto_fakelag;
		bool fl_spike_on_shoot;
		bool fl_spike_on_jump;
		bool fl_spike_on_peek;
		bool desync_melly;
		bool animfix;
		bool p2cbreaker;
		bool ninesevenlbybreaker;
		bool antilby;
		ButtonCode_t fakelagonkey;
		ButtonCode_t desync_swap_r;
		ButtonCode_t desync_swap_l;
		int desync_delta_swap_st[4];
		int desync_delta_swap_mv;

		int static_desync_type[4];

		bool breaklagcomp;

		ButtonCode_t desync_r;
		ButtonCode_t desync_l;

		bool lag_comp_enable;
		bool fakelag_onpeek;
		int fakelag_onpeek_amt;
		bool use2ndfreestanding;
		bool jitterbetweensecondbestangle;
		int freestanddelay;
		int freestandjitterrange;
		int desyncjitterrange[4];
		bool freestanddormant;
		bool panicjitter;
		int jitter_type[4];
		bool enable_shifts[4];
		bool antishoot;
		bool enable_switching_yaw[4];
		int desync_mode[4];
		int shift_yaw[4];
		int panicjitterrange;
		bool tank_aa[4];
		int panicjitterhp;
		int panicjittermode;
		ButtonCode_t flip_desync;
		ButtonCode_t forcefakelag;
		bool antiaim_desync[4];
		int max_desync[4];
		int max_desync_inverted[4];
	}antiaim;

	struct Player_t {
		bool enable;
		bool dormant_players;
		int alpha;

		bool out_pov_arrow;
		int out_pov_distance;
		Color out_pov_arrow_color;

		bool box;
		bool flash_bar;
		bool check_sdk;
		Color box_color;
		bool health;
		bool name;
		bool esp_view;
		Color name_color;
		int weapon;
		Color weapon_color;
		int ammo;
		bool armorbar;
		Color ammobar_color;
		bool lby;
		Color lbybar_color;
		std::vector< MultiDropdownItem_t > flags;
		std::vector< MultiDropdownItem_t > skeleton;

		std::vector< MultiDropdownItem_t > chams;
		Color chams_color;
		Color xqz_color;
		int chams_opacity;
		bool lowerbody_indicator;
		int chams_type;

		bool backtrack_chams;
		Color backtrack_chams_color;
		int backtrack_chams_opacity;

		std::vector< MultiDropdownItem_t > glow;
		int glow_type;
		Color glow_color_enemy;
		Color glow_color_teammate;
		Color glow_color_local;
		int glowopacity;
		int glowbloom;
	}player;

	struct Visuals_t {
		int fov;
		bool override_fov_scoped;

		bool transparency_scoped;
		bool grenade_enable;
		int transparency_scoped_alpha;

		bool spectator_list;
		bool espbomb;
		bool show_spread;
		bool check_hit;
		Color show_spread_color;
		bool anti_aim_lines;
		int show_spread_type;
		int hand_chams;
		int show_spread_opacity;
		int spectator_list_x;
		int spectator_list_y;

		bool crosshair;
		Color crosshair_color;
		int skybox;

		bool ghost_chams;
		bool pulsating_chams;
		bool fake_chams;
		Color ghost_chams_color;
		Color pulse_color;
		Color fake_chams_color;
		bool fakelagchams;
		bool local_chams;
		Color local_chams_color;

		bool bullet_tracer;
		bool bullet_tracer_enemy;
		int bullet_tracer_duration;
		int bullet_tracer_size;
		Color bullet_tracer_color;
		Color bullet_tracer_color_enemy;
		bool dropped_weapons;
		Color dropped_weapons_color;

		bool projectiles;
		Color projectiles_color;

		bool dont_render_teammates;
		bool nightmode;
		bool hitsound;
		int nightmodeskybox;
		std::vector< MultiDropdownItem_t > hitmarker;

		//std::vector< MultiDropdownItem_t > removals;
		bool removescope;
		bool removesmoke;
		bool removeflash;
		bool removerecoil;
		bool removeduckcooldown;
		int handchams;
		std::vector< MultiDropdownItem_t > indicators;
		Color handchamscolor;
		int gunchams;
		Color gunchamscolor;
		bool removescopesensitivity;
		bool removezoom;
		bool bright;
		bool fullbright;
		bool footstep;
		bool zeusrange;
	}esp;

	struct Misc_t {
		bool bunnyhop;
		int watermark;
		bool airstrafe;
		int status_bar;
		bool duck_jump;
		bool key_binds;
		bool debugmultip;
		ButtonCode_t circle_strafe;
		ButtonCode_t thirdperson_toggle;
		std::vector< MultiDropdownItem_t > events_to_log;
		bool model_changer;
		bool autodefuse;
		bool customviewmodel;
		float CustomViewModelX;
		float CustomViewModelY;
		float CustomViewModelZ;
		bool large;
		int largex;
		int largey;
		int largez;
		bool clantag_spammer;
		int clantag_modes;
		std::string customclantagyo;
		bool customclantagenable;
		bool clantagprefix;
		int clantagprefixmodes;
		std::string customclantag;
		bool sniper_crosshair;

		bool exaggerated_ragdolls;
		int exaggerated_ragdolls_multiplier;
		int ghostcharmsangle;
		bool ping_spike;
		int ping_spike_value;
		int ticks_fd;
		int fakewalkmodes;
		int fakewalkspeed;
		bool fakewalk;
		ButtonCode_t fakewalk_key;

		bool backend = true;
		ButtonCode_t fakeduck;
		bool unlimited_duck;
		ButtonCode_t slidewalk_key;
		bool anti_untrusted;
		bool RainbowBar;
		float rainbowspeed;
		bool RainbowBarraight;
		float rainbowspeedraight;
		std::vector< MultiDropdownItem_t > buybot;
		bool carterrr = true;
	}misc;
	struct
	{
		bool skinchanger_enable = false;

		int knife_model = 0;
		int knife_skin = 0;
		int changer_skin = 0;
		int knife_wear = 0;

		int paint_kit_vector_index_knife = 0;
		int paint_kit_index_knife = 0;
		int paint_kit_vector_index_usp = 0;
		int paint_kit_index_usp = 0;
		int paint_kit_vector_index_p2000 = 0;
		int paint_kit_index_p2000 = 0;
		int paint_kit_vector_index_glock = 0;
		int paint_kit_index_glock = 0;
		int paint_kit_vector_index_p250 = 0;
		int paint_kit_index_p250 = 0;
		int paint_kit_vector_index_fiveseven = 0;
		int paint_kit_index_fiveseven = 0;
		int paint_kit_vector_index_tec = 0;
		int paint_kit_index_tec = 0;
		int paint_kit_vector_index_cz = 0;
		int paint_kit_index_cz = 0;
		int paint_kit_vector_index_duals = 0;
		int paint_kit_index_duals = 0;
		int paint_kit_vector_index_deagle = 0;
		int paint_kit_index_deagle = 0;
		int paint_kit_vector_index_revolver = 0;
		int paint_kit_index_revolver = 0;
		int paint_kit_vector_index_famas = 0;
		int paint_kit_index_famas = 0;
		int paint_kit_vector_index_galil = 0;
		int paint_kit_index_galil = 0;
		int paint_kit_vector_index_m4a4 = 0;
		int paint_kit_index_m4a4 = 0;
		int paint_kit_vector_index_m4a1 = 0;
		int paint_kit_index_m4a1 = 0;
		int paint_kit_vector_index_ak47 = 0;
		int paint_kit_index_ak47 = 0;
		int paint_kit_vector_index_sg553 = 0;
		int paint_kit_index_sg553 = 0;
		int paint_kit_vector_index_aug = 0;
		int paint_kit_index_aug = 0;
		int paint_kit_vector_index_ssg08 = 0;
		int paint_kit_index_ssg08 = 0;
		int paint_kit_vector_index_awp = 0;
		int paint_kit_index_awp = 0;
		int paint_kit_vector_index_scar = 0;
		int paint_kit_index_scar = 0;
		int paint_kit_vector_index_g3sg1 = 0;
		int paint_kit_index_g3sg1 = 0;

	} Skinchanger;
	struct BuyBot_t {
		bool enable;
		int weapons;
		bool armorkit;
		bool zeus;
		bool defuser;
		std::vector< MultiDropdownItem_t > nades;
		std::vector< MultiDropdownItem_t > extras;
		std::vector< MultiDropdownItem_t > primary;
		std::vector< MultiDropdownItem_t > secondary;
	}BuyBot;
	struct NaNWalk_t {
		bool enabled;
		bool onlyonground;
		int amt;
		int jitter;
		ButtonCode_t key;
	}nanwalk;
	struct FastWalk_t {
		bool enabled;
		ButtonCode_t key;
		int speedmode;
		int speed;
	}fastwalk;
	struct LagSpike_t {
		bool enabled;
		ButtonCode_t key;
	}lagspike;
	struct astrowalk_t {
		bool enabled;
		ButtonCode_t key;
	}astrowalk;

	struct Skins_t {
		Weapon_Autism weapons[520];
		bool skinenabled = true;
		int Knife = 4;
		int gloves;
		int skingloves;
		float glovewear;
		bool glovesenabled;
		bool rankchanger;
		int KnifeSkin = 2;
		int AK47Skin = 1;
		int GalilSkin;
		int M4A1SSkin;
		int M4A4Skin;
		int AUGSkin;
		int FAMASSkin;
		int AWPSkin;
		int SSG08Skin;
		bool fixshit;
		bool knifes = true;
		int SCAR20Skin;
		int P90Skin;
		int Mp7Skin;
		int NovaSkin;
		int UMP45Skin;
		int GlockSkin;
		int SawedSkin;
		int USPSkin;
		int MagSkin;
		int XmSkin;
		int DeagleSkin;
		int DualSkin;
		int FiveSkin;
		int RevolverSkin;
		int Mac10Skin;
		int tec9Skin;
		int Cz75Skin;
		int NegevSkin;
		int M249Skin;
		int Mp9Skin;
		int P2000Skin;
		int BizonSkin;
		int Sg553Skin;
		int P250Skin;
		int G3sg1Skin;
	}skins;
	struct Gloves_t {
		int glove;
		std::string gloveid;
		bool apply;
	}gloves;

	struct Menu_t {
		Color menu_theme[2];
		Color removescopecol[2];
		ButtonCode_t menu_bind;
		bool newmenu;

		C_GroupBox * last_group;
		C_Tab * last_tab;
		int group_sub;
		int esp_x;
		int esp_y;
		int tab_sub;
	}menu;

	int select_func = 0;
	std::string weapon_select;

	int selected_config = 0;
	std::string new_config_name;
	bool old_pressed_keys[256];
};

extern Config g_cfg;