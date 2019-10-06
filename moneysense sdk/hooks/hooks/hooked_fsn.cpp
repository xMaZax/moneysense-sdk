#include "..\hooks.hpp"
#include "..\..\cheats\lagcompensation\lagcompensation.h"
#include "..\..\cheats\visuals\nightmode.h"
#include "..\..\cheats\visuals\other_esp.h"
#include "..\..\cheats\skinchanger\skinchanger.h"
#include "..\..\Hack\resolver.h"
#include "..\..\Hack\Backtracking.h"
#define RandomInt(min, max) (rand() % (max - min + 1) + min)
using FrameStageNotify_t = void(__stdcall*)(ClientFrameStage_t);

vec3_t flb_aim_punch;
vec3_t flb_view_punch;

vec3_t * aim_punch;
vec3_t * view_punch;

void remove_smoke() {
	static auto smokecout = *reinterpret_cast<uint32_t * *>(util::pattern_scan("client_panorama.dll", "A3 ? ? ? ? 57 8B CB") + 1);
	if (g_cfg.esp.removesmoke)
	{
		std::vector<const char*> vistasmoke_mats =
		{
			//	"particle/vistasmokev1/vistasmokev1_fire",
				"particle/vistasmokev1/vistasmokev1_smokegrenade",
				"particle/vistasmokev1/vistasmokev1_emods",
				"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		};

		for (auto matName : vistasmoke_mats)
		{
			IMaterial* mat = g_csgo.m_materialsystem()->FindMaterial(matName, "Other textures");
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		}
		*(int*)(smokecout) = 0;
	}
}

weapon_t * local_weapon;

void thirdperson() {
	static bool
		setting,
		holding,
		in_thirdperson,
		last_dead;

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.thirdperson_toggle) && !holding) {
		setting = !setting;

		if (g_ctx.m_local->is_alive()) {
			g_csgo.m_engine()->ExecuteClientCmd(setting ? "thirdperson" : "firstperson");
			in_thirdperson = setting;
		}
		holding = true;
	}
	else if (!g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.thirdperson_toggle) && holding)
		holding = false;

	if (g_ctx.m_local->is_alive())
		return;

	if (in_thirdperson) {
		g_csgo.m_engine()->ExecuteClientCmd("firstperson");
		g_ctx.m_local->m_iObserverMode() = OBS_MODE_IN_EYE;
		in_thirdperson = false;
	}

	if (!setting && last_dead) {
		g_ctx.m_local->m_iObserverMode() = OBS_MODE_IN_EYE;
		last_dead = false;
	}

	if (setting && !last_dead) {
		g_ctx.m_local->m_iObserverMode() = OBS_MODE_CHASE;
		last_dead = true;
	}
}

void update_spoofer() {
	if (!g_ctx.available()) {
		if (g_csgo.m_cvarspoofer()->ready())
			g_csgo.m_cvarspoofer()->release();

		return;
	}

	if (!g_csgo.m_cvarspoofer()->ready()) {
		g_csgo.m_cvarspoofer()->init();

		g_csgo.m_cvarspoofer()->add("sv_cheats");
		g_csgo.m_cvarspoofer()->add("r_drawspecificstaticprop");

		g_csgo.m_cvarspoofer()->spoof();
	}

	if (g_csgo.m_cvarspoofer()->ready()) {
		g_csgo.m_cvarspoofer()->get("sv_cheats")->SetInt(1);
		//g_csgo.m_cvarspoofer()->get("sv_pure")->SetInt(1);
	}
}

void __stdcall hooks::hooked_fsn(ClientFrameStage_t stage) {
	static auto original_fn = client_hook->get_func_address<FrameStageNotify_t>(37);

	if (!g_csgo.m_engine()->IsConnected() || !g_csgo.m_engine()->IsInGame())
		return original_fn(stage);;
	auto clientstate = *(uintptr_t *)(g_csgo.m_clientstate());
	if (!clientstate || !g_ctx.available()) {
		if (netchannel_hook) {
			netchannel_hook->clear_class_base();
			delete netchannel_hook;
			netchannel_hook = nullptr;
		}
	}
	else if (!g_csgo.m_clientstate()->m_NetChannel && netchannel_hook) {
		netchannel_hook->clear_class_base();
		delete netchannel_hook;
		netchannel_hook = nullptr;
	}

	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!g_ctx.available() || !local_player) {
		nightmode::get().clear_stored_materials();

		lagcompensation::get().clear_sequence();

		return original_fn(stage);;
	}
	for (int i = 0; i < g_csgo.m_globals()->m_maxclients; i++)
	{
		auto m_entity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (i == g_csgo.m_engine()->GetLocalPlayer())
			continue;

		player_t* pCurEntity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
		if (!pCurEntity)
			continue;

		CTickRecord trans = CTickRecord(pCurEntity);
		CBacktracking::get().ClearRecord(pCurEntity);
		CBacktracking::get().SaveTemporaryRecord(pCurEntity, trans);
		//fridge::ConsolePrint("Cleared Backtrack Record \n");
		*(int*)((uintptr_t)pCurEntity + 0xA30) = g_csgo.m_globals()->m_framecount;
		*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;
	}
	g_ctx.m_local = local_player;

	aim_punch = nullptr;
	view_punch = nullptr;

	flb_aim_punch = vec3_t{ };
	flb_view_punch = vec3_t{ };

	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		/* skinchanger::get().run(); */
		lagcompensation::get().update_sequence();
		if (g_cfg.ragebot.lagcomp)
		{
			CMBacktracking::get().StartLagCompensation(local_player);
			CMBacktracking::get().FrameUpdatePostEntityThink();
		}
	}
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END) {
		if (g_cfg.ragebot.lagcomp)
		{
			CMBacktracking::get().FrameUpdatePostEntityThink();
			CMBacktracking::get().FinishLagCompensation(local_player);
		}
	}
	if (stage == FRAME_NET_UPDATE_END)
	{
		if (g_cfg.ragebot.lagcomp)
		{
			CMBacktracking::get().FrameUpdatePostEntityThink();
		}
	}
	if (stage == FRAME_RENDER_START) {
		update_spoofer();

		thirdperson();

		/* removals *///you're chatting shit theres more in post you fag		//fak off men
		remove_smoke();

		if (g_cfg.esp.removeflash && g_ctx.m_local->m_flFlashDuration() > 0.0f)
			g_ctx.m_local->m_flFlashDuration() = 0.0f;

		if (*(bool*)g_csgo.m_postprocessing() != g_cfg.esp.removescope)
			*(bool*)g_csgo.m_postprocessing() = g_cfg.esp.removescope;

		if (g_cfg.esp.removerecoil) {
			aim_punch = &g_ctx.m_local->m_aimPunchAngle();
			view_punch = &g_ctx.m_local->m_viewPunchAngle();

			flb_aim_punch = *aim_punch;
			flb_view_punch = *view_punch;

			*aim_punch = vec3_t(0, 0, 0);
			*view_punch = vec3_t(0, 0, 0);
		}

		for (int i = 1; i <= g_csgo.m_entitylist()->GetHighestEntityIndex(); i++) {
			auto e = static_cast<entity_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

			if (!e)
				continue;

			if (e->IsDormant())
				continue;

			if (e->GetClientClass()->m_ClassID == CCSRagdoll) {
				auto ragdoll = (ragdoll_t*)e;

				if (g_cfg.misc.exaggerated_ragdolls) {
					vec3_t
						new_force = ragdoll->m_vecForce() * g_cfg.misc.exaggerated_ragdolls_multiplier,
						new_velocity = ragdoll->m_vecRagdollVelocity() * g_cfg.misc.exaggerated_ragdolls_multiplier;

					ragdoll->m_vecForce() = new_force;
					ragdoll->m_vecRagdollVelocity() = new_velocity;
				}
			}
		}

		if (g_ctx.m_local->is_alive()) {
			if (g_csgo.m_input()->m_fCameraInThirdPerson)
				*(Vector*)((uintptr_t)g_ctx.m_local + 0x31D8) = g_ctx.m_globals.real_angles;
		}
	}

	if (stage == FRAME_RENDER_END) {
		if (g_cfg.esp.nightmode)
			nightmode::get().apply();
		else
			nightmode::get().remove();
		if (g_cfg.ragebot.lagcomp)
			CMBacktracking::get().FrameUpdatePostEntityThink();
	}
	lagcompensation::get().fsn(stage);
	ResolverOvermake::resolver.DoFSN(stage);
	original_fn(stage);

	if (g_ctx.m_local && g_ctx.m_local->is_alive()) {
		if (g_cfg.esp.removerecoil && (aim_punch && view_punch)) {
			*aim_punch = flb_aim_punch;
			*view_punch = flb_view_punch;
		}
	}
}