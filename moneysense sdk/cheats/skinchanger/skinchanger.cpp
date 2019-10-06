#include "skinchanger.h"
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF

inline bool skinchanger::apply_knife_model(weapon_t* weapon, const char* model) {
	auto local_player = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	auto viewmodel = reinterpret_cast<viewmodel_t*>(g_csgo.m_entitylist()->GetClientEntityFromHandle(local_player->m_hViewModel()));

	if (!viewmodel)
		return false;

	auto h_view_model_weapon = viewmodel->is_hWeapon();
	auto view_model_weapon = reinterpret_cast<weapon_t*>(g_csgo.m_entitylist()->GetClientEntityFromHandle(h_view_model_weapon));

	if (view_model_weapon != weapon)
		return false;

	viewmodel->m_nModelIndex() = g_csgo.m_modelinfo()->GetModelIndex(model);

	return true;
}

inline bool skinchanger::apply_knife_skin(weapon_t* weapon, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) {
	weapon->m_iItemDefinitionIndex() = item_definition_index;
	weapon->m_nFallbackPaintKit() = paint_kit;
	weapon->m_nModelIndex() = model_index;
	weapon->m_iEntityQuality() = entity_quality;
	weapon->m_flFallbackWear() = fallback_wear;

	return true;
}
template<class T>
static T* find_hud_element(const char* name) {
	static auto fn = *reinterpret_cast<DWORD * *>(util::pattern_scan("client_panorama.dll", ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(util::pattern_scan("client_panorama.dll", ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
	return (T*)find_hud_element(fn, name);
}

void skinchanger::run() {
	if (!g_csgo.m_engine()->IsInGame() && !g_csgo.m_engine()->IsConnected())
		return;

	if (!g_cfg.Skinchanger.skinchanger_enable)
		return;

	auto local_player = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	auto active_weapon = local_player->m_hActiveWeapon();

	auto model_bayonet = "models/weapons/v_knife_bayonet.mdl";
	auto model_m9 = "models/weapons/v_knife_m9_bay.mdl";
	auto model_karambit = "models/weapons/v_knife_karam.mdl";
	auto model_bowie = "models/weapons/v_knife_survival_bowie.mdl";
	auto model_butterfly = "models/weapons/v_knife_butterfly.mdl";
	auto model_falchion = "models/weapons/v_knife_falchion_advanced.mdl";
	auto model_flip = "models/weapons/v_knife_flip.mdl";
	auto model_gut = "models/weapons/v_knife_gut.mdl";
	auto model_huntsman = "models/weapons/v_knife_tactical.mdl";
	auto model_shadow_daggers = "models/weapons/v_knife_push.mdl";
	auto model_navaja = "models/weapons/v_knife_gypsy_jackknife.mdl";
	auto model_stiletto = "models/weapons/v_knife_stiletto.mdl";
	auto model_talon = "models/weapons/v_knife_widowmaker.mdl";
	auto model_ursus = "models/weapons/v_knife_ursus.mdl";

	auto index_bayonet = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	auto index_m9 = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	auto index_karambit = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_karam.mdl");
	auto index_bowie = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	auto index_butterfly = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	auto index_falchion = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	auto index_flip = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_flip.mdl");
	auto index_gut = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_gut.mdl");
	auto index_huntsman = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	auto index_shadow_daggers = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_push.mdl");
	auto index_navaja = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
	auto index_stiletto = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
	auto index_talon = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
	auto index_ursus = g_csgo.m_modelinfo()->GetModelIndex("models/weapons/v_knife_ursus.mdl");

	auto my_weapons = local_player->m_hMyWeapons();
	for (size_t i = 0; my_weapons[i] != INVALID_EHANDLE_INDEX; i++) {
		auto weapon = reinterpret_cast<weapon_t*>(g_csgo.m_entitylist()->GetClientEntityFromHandle(my_weapons[i]));

		//knife conditions
		float wear = 0.f;
		switch (g_cfg.Skinchanger.knife_wear) {
		case 0:
			wear = 0.001f;
			break;
		case 1:
			wear = 0.07f;
			break;
		case 2:
			wear = 0.15f;
			break;
		case 3:
			wear = 0.38f;
			break;
		case 4:
			wear = 0.45f;
			break;
		}

		//apply knife model
		if (active_weapon->GetClientClass()->m_ClassID == ClassId::CKnife) {
			switch (g_cfg.Skinchanger.knife_model) {
			case 0:
				break;
			case 1:
				apply_knife_model(weapon, model_bayonet);
				break;
			case 2:
				apply_knife_model(weapon, model_m9);
				break;
			case 3:
				apply_knife_model(weapon, model_karambit);
				break;
			case 4:
				apply_knife_model(weapon, model_bowie);
				break;
			case 5:
				apply_knife_model(weapon, model_butterfly);
				break;
			case 6:
				apply_knife_model(weapon, model_falchion);
				break;
			case 7:
				apply_knife_model(weapon, model_flip);
				break;
			case 8:
				apply_knife_model(weapon, model_gut);
				break;
			case 9:
				apply_knife_model(weapon, model_huntsman);
				break;
			case 10:
				apply_knife_model(weapon, model_shadow_daggers);
				break;
			case 11:
				apply_knife_model(weapon, model_navaja);
				break;
			case 12:
				apply_knife_model(weapon, model_stiletto);
				break;
			case 13:
				apply_knife_model(weapon, model_talon);
				break;
			case 14:
				apply_knife_model(weapon, model_ursus);
				break;
			}
		}

		if (active_weapon->GetClientClass()->m_ClassID == ClassId::CKnife) {
			switch (g_cfg.Skinchanger.knife_model) {
			case 0:
				break;
			case 1:
				apply_knife_skin(weapon, WEAPON_KNIFE_BAYONET, g_cfg.Skinchanger.paint_kit_index_knife, index_bayonet, 3, wear);
				break;
			case 2:
				apply_knife_skin(weapon, WEAPON_KNIFE_M9_BAYONET, g_cfg.Skinchanger.paint_kit_index_knife, index_m9, 3, wear);
				break;
			case 3:
				apply_knife_skin(weapon, WEAPON_KNIFE_KARAMBIT, g_cfg.Skinchanger.paint_kit_index_knife, index_karambit, 3, wear);
				break;
			case 4:
				apply_knife_skin(weapon, WEAPON_KNIFE_SURVIVAL_BOWIE, g_cfg.Skinchanger.paint_kit_index_knife, index_bowie, 3, wear);
				break;
			case 5:
				apply_knife_skin(weapon, WEAPON_KNIFE_BUTTERFLY, g_cfg.Skinchanger.paint_kit_index_knife, index_butterfly, 3, wear);
				break;
			case 6:
				apply_knife_skin(weapon, WEAPON_KNIFE_FALCHION, g_cfg.Skinchanger.paint_kit_index_knife, index_falchion, 3, wear);
				break;
			case 7:
				apply_knife_skin(weapon, WEAPON_KNIFE_FLIP, g_cfg.Skinchanger.paint_kit_index_knife, index_flip, 3, wear);
				break;
			case 8:
				apply_knife_skin(weapon, WEAPON_KNIFE_GUT, g_cfg.Skinchanger.paint_kit_index_knife, index_gut, 3, wear);
				break;
			case 9:
				apply_knife_skin(weapon, WEAPON_KNIFE_TACTICAL, g_cfg.Skinchanger.paint_kit_index_knife, index_huntsman, 3, wear);
				break;
			case 10:
				apply_knife_skin(weapon, WEAPON_KNIFE_PUSH, g_cfg.Skinchanger.paint_kit_index_knife, index_shadow_daggers, 3, wear);
				break;
			case 11:
				apply_knife_skin(weapon, WEAPON_KNIFE_GYPSY_JACKKNIFE, g_cfg.Skinchanger.paint_kit_index_knife, index_navaja, 3, wear);
				break;
			case 12:
				apply_knife_skin(weapon, WEAPON_KNIFE_STILETTO, g_cfg.Skinchanger.paint_kit_index_knife, index_stiletto, 3, wear);
				break;
			case 13:
				apply_knife_skin(weapon, WEAPON_KNIFE_WIDOWMAKER, g_cfg.Skinchanger.paint_kit_index_knife, index_talon, 3, wear);
				break;
			case 14:
				apply_knife_skin(weapon, WEAPON_KNIFE_URSUS, g_cfg.Skinchanger.paint_kit_index_knife, index_ursus, 3, wear);
				break;
			}
		}

		//weapons
		switch (weapon->m_iItemDefinitionIndex()) {
		case WEAPON_USPS:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_usp;
			break;
		case WEAPON_P2000:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_p2000;
			break;
		case WEAPON_GLOCK:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_glock;
			break;
		case WEAPON_P250:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_p250;
			break;
		case WEAPON_FIVESEVEN:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_fiveseven;
			break;
		case WEAPON_TEC9:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_tec;
			break;
		case WEAPON_CZ75:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_cz;
			break;
		case WEAPON_ELITE:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_duals;
			break;
		case WEAPON_DEAGLE:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_deagle;
			break;
		case WEAPON_REVOLVER:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_revolver;
			break;
		case WEAPON_FAMAS:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_famas;
			break;
		case WEAPON_GALIL:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_galil;
			break;
		case WEAPON_M4A1S:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_m4a4;
			break;
		case WEAPON_M4A4:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_m4a1;
			break;
		case WEAPON_AK47:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_ak47;
			break;
		case WEAPON_SG553:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_sg553;
			break;
		case WEAPON_AUG:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_aug;
			break;
		case WEAPON_SSG08:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_ssg08;
			break;
		case WEAPON_AWP:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_awp;
			break;
		case WEAPON_SCAR20:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_scar;
			break;
		case WEAPON_G3SG1:
			weapon->m_nFallbackPaintKit() = g_cfg.Skinchanger.paint_kit_index_g3sg1;
			break;
		}

		weapon->m_OriginalOwnerXuidLow() = 0;
		weapon->m_OriginalOwnerXuidHigh() = 0;
		weapon->m_nFallbackSeed() = 661;
		weapon->m_iItemIDHigh() = -1;
	}
}
