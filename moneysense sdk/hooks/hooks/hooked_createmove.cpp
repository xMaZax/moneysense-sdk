#include "..\hooks.hpp"
#include "..\..\cheats\sdk\animation_state.h"
#include "..\..\cheats\visuals\other_esp.h"
#include "..\..\cheats\misc\circlestrafer.h"
#include "..\..\cheats\misc\prediction_system.h"
#include "..\..\cheats\misc\bunnyhop.h"
#include "..\..\cheats\misc\airstrafe.h"
#include "..\..\cheats\lagcompensation\lagcompensation.h"
#include "..\..\cheats\misc\spammers.h"
#include "..\..\cheats\fakewalk\fakewalk.h"
#include "../../cheats/misc/fakelag.h"
#include "..\..\Hack\ragebot.h"
#include "..\..\Hack\resolver.h"
#include "..\..\Hack\PWalk.h"
#include "..\..\Hack\Backtracking.h"
#include "..\..\Hack\extrapolation.h"
#include "..\..\Hack\autodefuse.h"
#include "../../cheats/legit/legitbot.h"
#include "../../knifebot.h"
using CreateMove_t = bool(__thiscall*)(IClientMode *, float, CUserCmd *);

weapon_t* e = nullptr;
bool datagram_setup = false;
bool* bSendPacket_;
bool __stdcall hooks::hooked_createmove(float smt, CUserCmd * m_pcmd) {
	static auto original_fn = clientmode_hook->get_func_address< CreateMove_t >(24);
	if (!m_pcmd->m_command_number || !g_ctx.available() || !g_ctx.m_local)
		return original_fn(g_csgo.m_clientmode(), smt, m_pcmd);

	if (g_cfg.misc.clantag_modes == 1)
		spammers::get().clan_tag();

	if (!g_ctx.m_local->is_alive())
		return original_fn(g_csgo.m_clientmode(), smt, m_pcmd);

	g_ctx.set_command(m_pcmd);

	util::server_time(m_pcmd);
	if (g_cfg.misc.ping_spike && !netchannel_hook) {
		auto netchannel = g_csgo.m_clientstate()->m_NetChannel;
		if (netchannel) {
			netchannel_hook = new vmthook(reinterpret_cast<DWORD**>(netchannel));
			netchannel_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_senddatagram), 46);
		}
	}
	if (g_cfg.misc.sniper_crosshair) {
		static bool o = false;
		if (!o) { o = true;	g_csgo.m_cvar()->FindVar("weapon_debug_spread_show")->SetValue(3); }
	}
	else {
		static bool o = false;
		if (!o) { o = true;	g_csgo.m_cvar()->FindVar("weapon_debug_spread_show")->SetValue(0); }
	}

	legitbot::get().legit_resolver(m_pcmd); //legit resolver xD

	ResolverOvermake::resolver.DoCM();
	autodefuser::get().defuserapplication(m_pcmd);
	animation_state::get().create_move();
	uintptr_t *frame_ptr;
	__asm mov frame_ptr, ebp;
	g_ctx.send_packet = true;
	g_ctx.m_globals.fakelagging = false;
	vec3_t original_angle = m_pcmd->m_viewangles;

	if (g_cfg.legit.enable_legit)
	   legitbot::get().create_move();

	if (g_cfg.ragebot.knifebot) {
		knifebot::get().run();
	}

	g_ctx.unpred_velocity = g_ctx.m_local->m_vecVelocity();

	engineprediction::get().start(); {
		
		fakewalk::get().create_move();

		if (g_cfg.misc.fakeduck)
			fakewalk::get().fake_duck(m_pcmd, bSendPacket_);

		fakelag::get().create_move();

		if (g_cfg.misc.customviewmodel) {
			int viewX = g_cfg.misc.CustomViewModelX;

			ConVar* viewXcvar = g_csgo.m_cvar()->FindVar("viewmodel_offset_x");
			*(int*)((DWORD)&viewXcvar->fnChangeCallback + 0xC) = NULL;
			viewXcvar->SetValue(viewX);

			int viewY = g_cfg.misc.CustomViewModelY;

			ConVar* viewYcvar = g_csgo.m_cvar()->FindVar("viewmodel_offset_y");
			*(int*)((DWORD)&viewYcvar->fnChangeCallback + 0xC) = NULL;
			viewYcvar->SetValue(viewY);

			int viewZ = g_cfg.misc.CustomViewModelZ;

			ConVar* viewZcvar = g_csgo.m_cvar()->FindVar("viewmodel_offset_z");
			*(int*)((DWORD)&viewZcvar->fnChangeCallback + 0xC) = NULL;

			viewZcvar->SetValue(viewZ);
		}
		if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(CMBacktracking::get().current_record->m_nFlags & FL_ONGROUND)) {
			*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
		}
		if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && g_csgo.m_inputsys()->IsButtonDown(KEY_SPACE)) {
			*(bool*)(*(DWORD*)frame_ptr - 0x1C) = false;
		}
		if (!g_ctx.m_local->m_fFlags() & FL_ONGROUND && !g_csgo.m_inputsys()->IsButtonDown(KEY_SPACE)) {
			*(bool*)(*(DWORD*)frame_ptr - 0x1C) = false;
		}
		for (int i = 1; i < 2; i++)
		{
			if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(headPositions[g_ctx.m_local->EntIndex()][i].flags)) {
				*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
			}
		}

		if (g_cfg.misc.key_binds)
		{
			if (g_csgo.m_inputsys()->IsButtonDown(KEY_W))
			{
				render::get().text(fonts[CTAB], 450 - 50, 8, Color(255, 255, 255), HFONT_OFF_SCREEN, "W");
			}

			if (g_csgo.m_inputsys()->IsButtonDown(KEY_A))
			{
				render::get().text(fonts[CTAB], 442 - 50, 19, Color(255, 255, 255), HFONT_OFF_SCREEN, "A");
			}

			if (g_csgo.m_inputsys()->IsButtonDown(KEY_S))
			{
				render::get().text(fonts[CTAB], 452 - 50, 19, Color(255, 255, 255), HFONT_OFF_SCREEN, "S");
			}

			if (g_csgo.m_inputsys()->IsButtonDown(KEY_D))
			{
				render::get().text(fonts[CTAB], 461 - 50, 19, Color(255, 255, 255), HFONT_OFF_SCREEN, "D");
			}

			if (g_csgo.m_inputsys()->IsButtonDown(KEY_SPACE))
			{
				render::get().text(fonts[CTAB], 475 - 50, 8, Color(255, 255, 255), HFONT_OFF_SCREEN, "jump");
			}

			if (g_csgo.m_inputsys()->IsButtonDown(KEY_LCONTROL))
			{
				render::get().text(fonts[CTAB], 475 - 50, 19, Color(255, 255, 255), HFONT_OFF_SCREEN, "duck");
			}
		}

		if (g_cfg.ragebot.extrapolation)
			LinearExtrapolationsrun();

		CMBacktracking::get().StartLagCompensation(g_ctx.m_local);	

		fakewalk::get().slidewalk();

		if (g_cfg.ragebot.autorevolver)
		{
			if (e->m_iItemDefinitionIndex() != WEAPON_REVOLVER)
				return;

			static int use_r8 = 0;
			use_r8++;
			if (use_r8 <= 14.5f) {
				m_pcmd->m_buttons |= IN_ATTACK;
			}
			else {
				use_r8 = 0;

				float flPostponeFireReady = e->GetFireReadyTime();
				if (flPostponeFireReady > 0 && flPostponeFireReady < g_csgo.m_globals()->m_curtime) {
					m_pcmd->m_buttons &= ~IN_ATTACK;
				}
			}
		}

		if (g_cfg.ragebot.doubletap_enable)
		   aimbot::get().double_tap(m_pcmd);

		if (g_cfg.misc.bunnyhop)
			bunnyhop::get().create_move();
		if (g_cfg.misc.airstrafe)
			airstrafe::get().create_move();
		pwalk::get().CreateMovementwithBlackPeopleThrowingCoalIntoMyOven(m_pcmd);
		vec3_t wish_angle = m_pcmd->m_viewangles;
		if (!g_cfg.esp.fakelagchams)
			m_pcmd->m_viewangles = original_angle;
		if (!g_csgo.m_clientstate()->m_nChokedCommands)
		if (g_cfg.antiaim.enable) {
			antiaim::get().update_lowerbody_breaker();
			antiaim::get().create_move();
		}
		if (g_cfg.legit.legit_resolver_active) {
			legitbot::get().legit_resolver(m_pcmd);
		}
		if (g_cfg.ragebot.enable)
			aimbot::get().create_move();
		g_ctx.m_globals.add_shot();

		util::movement_fix(wish_angle, m_pcmd);
	}
	lagcompensation::get().create_move();

	if (g_cfg.misc.unlimited_duck)
	  m_pcmd->m_buttons |= IN_BULLRUSH;

	if (g_cfg.misc.unlimited_duck && g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.fakeduck))
	{
		static int cnt = 0;
		static bool do_ = false;

		if (g_ctx.get_command()->m_buttons) {
			g_ctx.send_packet = false;

			if (cnt % 14 == 0)
				do_ = true;
			else if (cnt % 14 == 6)
				g_ctx.send_packet = true;
			else if (cnt % 14 == 7)
				do_ = false;

			if (do_)
				g_ctx.get_command()->m_buttons |= IN_DUCK;
			else
				g_ctx.get_command()->m_buttons &= ~IN_DUCK;

			cnt++;
		}
		else {
			do_ = false;
			cnt = 0;
		}
	}

	if (m_pcmd->m_buttons & IN_DUCK)
	{
		bool CurrentTickSendPacket, LastTickSendPacket;
		int lasttime, sendpacketsticks, notsendpacketticks, autism1, autism2, ifyoureadthisyouareabadpaster;
		if (g_cfg.antiaim.fakekrautsh || g_cfg.antiaim.fakecourouch) {
			//removed
		}
		// clamp choked commands
	}
	if (g_csgo.m_clientstate()->m_nChokedCommands > 14)
		fridge::ConsolePrint("WARNING: Chocked Commands extended 14! \n");
	if (g_csgo.m_clientstate()->m_nChokedCommands > 14)
		g_ctx.send_packet = true;
	// set send_packet
	*(bool*)(*frame_ptr - 0x1C) = g_ctx.send_packet;
	if (g_ctx.m_local && g_ctx.m_local->is_alive())
	{
		if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(headPositions[g_ctx.m_local->EntIndex()][0].flags & FL_ONGROUND)) {
			if (GetAsyncKeyState(VK_SPACE)) {
				*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
			}
		}
		if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(headPositions[g_ctx.m_local->EntIndex()][2].flags & FL_ONGROUND)) {
			if (GetAsyncKeyState(VK_SPACE)) {
				*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
			}
		}
		if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(headPositions[g_ctx.m_local->EntIndex()][1].flags & FL_ONGROUND)) {
			if (GetAsyncKeyState(VK_SPACE)) {
				*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
			}
		}
	}

	// get angles for thirdperson
	if (!g_cfg.esp.fakelagchams)
	{
		if (g_ctx.send_packet) {
			g_ctx.m_globals.fake_angles = m_pcmd->m_viewangles;
		}
		else
			g_ctx.m_globals.real_angles = m_pcmd->m_viewangles;
	}
	// clamp angles for anti-ut
	if (g_cfg.misc.anti_untrusted)
		math::normalize_angles(m_pcmd->m_viewangles);

	// movement clamp
	if (m_pcmd->m_forwardmove > 450.0f)
		m_pcmd->m_forwardmove = 450.0f;
	else if (m_pcmd->m_forwardmove < -450.0f)
		m_pcmd->m_forwardmove = -450.0f;

	if (m_pcmd->m_sidemove > 450.0f)
		m_pcmd->m_sidemove = 450.0f;
	else if (m_pcmd->m_sidemove < -450.0f)
		m_pcmd->m_sidemove = -450.0f;

	return false;
}