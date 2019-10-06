#include "..\sdk\animation_state.h"
#include "..\autowall\autowall.h"
#include <chrono>
#include <iostream>
#include <time.h>
#include <Windows.h>

Vector NormalizeAnglePlusPlus(Vector angle)
{
	while (angle.x > 89.f)
	{
		angle.x -= 180.f;
	}
	while (angle.x < -89.f)
	{
		angle.x += 180.f;
	}
	if (angle.y > 180)
	{
		angle.y -= (round(angle.y / 360) * 360.f);
	}
	else if (angle.y < -180)
	{
		angle.y += (round(angle.y / 360) * -360.f);
	}
	if ((angle.z > 50) || (angle.z < 50))
	{
		angle.z = 0;
	}
	return angle;
}

void VectorAnglesPlusPlus(const Vector& vecForward, Vector& vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / 3.14159265358979323846f;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / 3.14159265358979323846f;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}

void CorrectMovement_AAA(vec3_t vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
	// side/forward move correction
	float deltaView;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (pCmd->m_viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->m_viewangles.y;
	else
		f2 = pCmd->m_viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	pCmd->m_forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->m_sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

bool local_update;
#define PI 3.14159265358979323846f
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

bool m_bAutomaticDir = false;
int m_iAutoDirection = 0;
static bool jitterr = false;

int m_iJitter = 0;
int m_iJitterIteration = 0;

float m_flCurrentFeetYaw = 0.0f;
float m_flPreviousFeetYaw = 0.0f;
bool next_lby_update()
{
	auto local_player = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = g_csgo.m_globals()->m_curtime;
	local_update = next_lby_update_time;

	auto animstate = local_player->get_animation_state();

	if (!animstate)
		return false;

	if (!(local_player->m_fFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1f)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}
float antiaim::GetMaxFakeDelta()
{
	auto animstate = g_ctx.m_local->get_animation_state();

	float duckammount = animstate->m_fDuckAmount;

	float speedfraction = max(0.f, min(animstate->m_flFeetSpeedForwardsOrSideWays, 1.f));

	float speedfactor = max(0.f, max(1.f, animstate->m_flFeetSpeedUnknownForwardOrSideways));

	float unk1 = ((animstate->m_flStopToFullRunningFraction * -0.3) - 0.2) * speedfraction;

	float unk2 = unk1 + 1.f;

	if (duckammount > 0)

		unk2 += +((duckammount * speedfactor) * (0.5f - unk2));

	return *(float*)((uintptr_t)animstate + 0x334) * unk2;
}

float antiaim::get_max_desync_delta() {
	auto animstate = uintptr_t(g_ctx.m_local->get_animation_state());

	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return unk3;
}

float empty_get_max_desync_delta() {
	auto animstate = uintptr_t(g_ctx.m_local->get_animation_state());

	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return unk3;
}

void antiaim::create_move() {

	manual_aa::get().update();

	auto m_weapon = g_ctx.m_local->m_hActiveWeapon().Get();

	if (!m_weapon) {
		return;
	}
	if (m_weapon->is_non_aim()) {
		return;
	}
	if (g_ctx.get_command()->m_buttons & IN_USE) {
		return;
	}
	vec3_t next_angle = math::calculate_angle(g_ctx.m_local->get_eye_pos(), aimbot::get().target_data.aimspot);
	if (g_ctx.m_local->m_bGunGameImmunity() || g_ctx.m_local->m_fFlags() & FL_FROZEN || freeze_check)
		return;

	else if (m_weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
	{
		if (g_ctx.get_command()->m_buttons & IN_ATTACK2)
			return;

		if (m_weapon->CanFirePostPone() && (g_ctx.get_command()->m_buttons & IN_ATTACK))
			return;
	}
	else if (m_weapon->is_grenade()) {
		if (m_weapon->is_grenade() && m_weapon->m_fThrowTime() > 0.f)
			return;
	}
	else {
		if (m_weapon->get_csweapon_info()->type == WEAPONTYPE_KNIFE && ((g_ctx.get_command()->m_buttons & IN_ATTACK) || (g_ctx.get_command()->m_buttons & IN_ATTACK2))) {
			return;
		}
		else if ((g_ctx.get_command()->m_buttons & IN_ATTACK) && (m_weapon->m_iItemDefinitionIndex() != WEAPON_C4)) {
			if (m_weapon->can_fire())
				return;
		}
	}
	if (m_weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER && ((g_ctx.get_command()->m_buttons & IN_ATTACK) || (g_ctx.get_command()->m_buttons & IN_ATTACK2)))
	{
		g_ctx.get_command()->m_viewangles.x = 89.0f;
	}
	if (g_ctx.m_local->get_move_type() == MOVETYPE_NOCLIP || g_ctx.m_local->get_move_type() == MOVETYPE_LADDER)
		return;

	if (!g_ctx.m_globals.fakelagging)
		g_ctx.send_packet = g_ctx.get_command()->m_command_number % 2;

	g_ctx.get_command()->m_viewangles.x = get_pitch();

	if (!manual_aa::get().run())
		get_real();

	bool index;

	if (!(g_ctx.m_local->m_fFlags() & FL_ONGROUND)) { // Air
		index = g_cfg.antiaim.break_adjustment[2];
	}

	if (g_ctx.m_local->m_vecVelocity().Length() > 0 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) { // Move
		index = g_cfg.antiaim.break_adjustment[1];
	}
	if (g_ctx.m_local->m_vecVelocity().Length() < 1 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) // stand
	{
		index = g_cfg.antiaim.break_adjustment[0];
	}

	if (g_cfg.misc.fakewalk && g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.fakewalk_key) & FL_ONGROUND) // slowwalk
	{
		index = g_cfg.antiaim.break_adjustment[3];
	}


	if (!g_ctx.send_packet && g_cfg.antiaim.lby_breaker) {
		if (m_bBreakBalance && index) {
			bool negative = 119 < 0;

			if (abs(119) > 120) {
				float new_delta = 119 / 2;

				g_ctx.get_command()->m_viewangles.y += negative ? -new_delta : new_delta;
			}
			else {
				float new_delta = 119 - 180;

				g_ctx.get_command()->m_viewangles.y += negative ? -new_delta : new_delta;
			}
		}


		g_ctx.get_command()->m_viewangles.y += g_cfg.antiaim.lby_delta;
	}

	if (next_lby_update() && g_cfg.antiaim.air_desync && !(g_ctx.m_local->m_fFlags() & FL_ONGROUND))
	{
		g_ctx.get_command()->m_viewangles.y += g_ctx.m_globals.desync_bind ? 119.f : -119.f;
	}

	g_ctx.get_command()->m_buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT);
}
float get_fixed_feet_yaw() {

	float current_feet_yaw = *(float*)(g_ctx.m_local->get_animation_state() + 0x80);

	if (current_feet_yaw >= -360)
		current_feet_yaw = min(current_feet_yaw, 360.0);

	return current_feet_yaw;
}

bool antiaim::DesyncRotate(float jitter, int direction, CUserCmd* cmd, ctx_t* ctx)
{
	auto animState = g_ctx.m_local->get_animation_state();
	float feetYaw = DEG2RAD(animState->m_flCurrentFeetYaw);

	float feetSin, feetCos;
	DirectX::XMScalarSinCos(&feetSin, &feetCos, feetYaw);

	float feetSin1, feetCos1;
	DirectX::XMScalarSinCos(&feetSin1, &feetCos1, m_flCurrentFeetYaw);

	float feetSin2, feetCos2;
	DirectX::XMScalarSinCos(&feetSin2, &feetCos2, m_flPreviousFeetYaw);

	m_flPreviousFeetYaw = m_flCurrentFeetYaw;
	m_flCurrentFeetYaw = animState->m_flCurrentFeetYaw;

	float totalRotation = atan2(feetSin1 + feetSin + feetSin2, feetCos1 + feetCos + feetCos2);

	float v16 = math::normalize_yaw(RAD2DEG(totalRotation) - jitter);
	if (direction == 1) {
		if (v16 >= 0.0f) {
			m_iJitterIteration = 1;
			return false;
		}
	}
	else if (v16 <= 0.0f) {
		m_iJitterIteration = 1;
		return false;
	}

	float finalJitter = static_cast<float>(30 * (m_iJitterIteration % 12));
	if (direction == 1)
		cmd->m_viewangles.y -= finalJitter;
	else
		cmd->m_viewangles.y += finalJitter;
	cmd->m_viewangles.y = math::normalize_yaw(cmd->m_viewangles.y);

	ctx->send_packet = true;

	++m_iJitterIteration;
	return true;
}

bool Switch = false;

void antiaim::jitter() // aimware desync
{
	int radiuant;
	int jitterrange;

	if (!(g_ctx.m_local->m_fFlags() & FL_ONGROUND)) { // Air
		radiuant = g_cfg.antiaim.jitter_type[2];
		jitterrange = g_cfg.antiaim.desyncjitterrange[2];
	}

	if (g_ctx.m_local->m_vecVelocity().Length() > 0 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) { // Move
		radiuant = g_cfg.antiaim.jitter_type[1];
		jitterrange = g_cfg.antiaim.desyncjitterrange[1];
	}
	if (g_ctx.m_local->m_vecVelocity().Length() < 1 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) // stand
	{
		radiuant = g_cfg.antiaim.jitter_type[0];
		jitterrange = g_cfg.antiaim.desyncjitterrange[0];
	}

	if (g_cfg.misc.fakewalk && g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.fakewalk_key) & FL_ONGROUND) // slowwalk
	{
		radiuant = g_cfg.antiaim.jitter_type[3];
		jitterrange = g_cfg.antiaim.desyncjitterrange[3];
	}


	static float RealAng = 0;
	static bool jitter;
	RealAng = g_ctx.get_command()->m_viewangles.y;
	if (g_ctx.send_packet)
	{
		float server_time = g_ctx.m_local->m_nTickBase() * g_csgo.m_globals()->m_interval_per_tick;
		float time = TIME_TO_TICKS(server_time);

		while (time >= server_time)
			time = 0.f;

		float idk = rand() % 100;

		jitter = !jitter;
		if (time >= server_time / 2)
		{
			if (idk < 70)
			{
				if (!jitter)
					g_ctx.get_command()->m_viewangles.y = RealAng + jitterrange;

			}
			else
			{
				if (!jitter)
					g_ctx.get_command()->m_viewangles.y = RealAng - jitterrange;

			}
		}
		else
		{
			if (idk < 70)
			{
				if (jitter)
					g_ctx.get_command()->m_viewangles.y = RealAng - jitterrange;
			}
			else
			{
				if (jitter)
					g_ctx.get_command()->m_viewangles.y = RealAng + jitterrange;

			}
		}
	}


}

void antiaim::fix_movement(CUserCmd* cmd)
{

}



void antiaim::balance() // aimware desync
{
	int static_desync;
	g_cfg.antiaim.lby_breaker = true;

	if (!(g_ctx.m_local->m_fFlags() & FL_ONGROUND)) { // Air
		static_desync = g_cfg.antiaim.static_desync_type[2];
	}

	if (g_ctx.m_local->m_vecVelocity().Length() > 0 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) { // Move
		static_desync = g_cfg.antiaim.static_desync_type[1];
	}
	if (g_ctx.m_local->m_vecVelocity().Length() < 1 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) // stand
	{
		static_desync = g_cfg.antiaim.static_desync_type[0];
	}

	if (g_cfg.misc.fakewalk && g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.fakewalk_key) & FL_ONGROUND) // slowwalk
	{
		static_desync = g_cfg.antiaim.static_desync_type[3];
	}


	static bool holding;
	static bool bFlipYaw;
	static bool make_inverting;

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.disable_manualkey) && !holding) {
		bFlipYaw = !bFlipYaw;

		if (g_ctx.m_local->is_alive_local()) {

			if (static_desync == 0)
			{
				g_cfg.antiaim.lby_delta = bFlipYaw ? -140.f : +140.f;
				g_ctx.m_local->get_animation_state()->m_flGoalFeetYaw += bFlipYaw ? 58.f : -58.f;
			}
			else if (static_desync == 1)
			{
				g_cfg.antiaim.lby_delta = bFlipYaw ? -40.f : +40.f;
				g_ctx.m_local->get_animation_state()->m_flGoalFeetYaw += bFlipYaw ? 29.f : -29.f;
			}
			g_ctx.m_globals.current_aa = bFlipYaw;
			g_ctx.m_globals.desync_bind = bFlipYaw;
		}

		holding = true;
	}
	else if (!g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.disable_manualkey) && holding)
		holding = false;
}

/*
float antiaim::get_max_desync_delta() { // зачем это вообщем нужно?

	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	auto animstate = uintptr_t(local_player->get_animation_state());
	Vector yawfeetdelta = local_player->GetBaseEntity()->GetAbsAngles();
	yawfeetdelta.y = local_player->get_animation_state()->m_flGoalFeetYaw - local_player->get_animation_state()->m_flEyeYaw;
	math::normalize_angles(yawfeetdelta);

	Vector an = local_player->GetAbsAngles();
	an.y = local_player->get_animation_state()->m_flCurrentFeetYaw;
	local_player->set_abs_angles(an);
	float flYaw = an.y;

	float v47 = local_player->get_animation_state()->m_flCurrentFeetYaw;
	float pes = 360; float v146 = 360;
	if (v47 >= -360.0)
	{
		pes = fminf(v47, 360);
		v146 = pes;
	}

	float v155 = fmod(yawfeetdelta.y, 360.0);
	if (flYaw <= v146)
	{
		if (v155 <= -180.0)
			yawfeetdelta.y = v155 + 360, 0;
	}
	else if (v155 >= 180.0)
	{
		yawfeetdelta.y = v155 - 360, 0;
	}


	// g_LocalPlayer->GetPlayerAnimState()->m_flCurrentFeetYaw;


	float rate = 180;
	float duckammount = *(float *)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));
	float speedfactor;
	if (speedfraction >= 0.0)
		speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));
	else
		speedfactor = 0.0f;

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	if (duckammount > 0) {

		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	}


	int maxrot = local_player->get_animation_state()->m_flGoalFeetYaw  * unk2;
	int invrot = local_player->get_animation_state()->m_flCurrentFeetYaw  * unk2;

	DWORD xmmword_10BF8550;

	if (yawfeetdelta.y <= maxrot)
	{
		if (invrot > yawfeetdelta.y)
			*(float *)(animstate + 0x80) = +flYaw;
	}
	else
	{
		*(float *)(animstate + 0x80) = flYaw;// prevent these
	}
	Vector gfeet2 = local_player->GetBaseEntity()->GetAbsAngles();
	gfeet2.y = fmod(*(float *)(animstate + 0x80), 360);
	Vector gfeet = local_player->GetBaseEntity()->GetAbsAngles();
	gfeet.y = fmod(*(float *)(animstate + 0x80), 360);
	math::clamp_angles(gfeet);

	if (gfeet2.y > 180, 0)
		gfeet.y = gfeet2.y - 360;
	if (gfeet.y < -180.0)
		gfeet.y = gfeet.y + 360;
	//v60 = *(float *)(v3 + 220);
	float feetg = gfeet.y;
	//g_LocalPlayer->GetPlayerAnimState()->m_flCurrentFeetYaw() = gfeet.yaw;


	return feetg;
}
*/
void antiaim::update_lowerbody_breaker()
{
	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	static bool IsInGame = true;

	if (!g_csgo.m_engine()->IsInGame())
	{
		IsInGame = true;
		if (!g_csgo.m_clientstate()->m_nChokedCommands)
			antiaim::get().update_lowerbody_breaker();
	}


	if (!g_ctx.m_local->m_iHealth())
		return;

	float
		server_time = util::server_time();




	m_bBreakLowerBody = false;
	m_bBreakBalance = false;

	if (m_flNextBodyUpdate < server_time)
	{
		m_flNextBodyUpdate = server_time + 1.1;
		m_bBreakLowerBody = true;
	}

	if (m_flNextBodyUpdate - g_csgo.m_globals()->m_interval_per_tick < server_time)
		m_bBreakBalance = true;
	else
	{
		if (!no_active)
			g_ctx.get_command()->m_viewangles.y = RAD2DEG(bestrotation) - 180;

		last_real = g_ctx.get_command()->m_viewangles.y;
	}


	const bool moving_on_ground = g_ctx.m_local->m_vecVelocity().Length2D() > 0.1;

	if (moving_on_ground)
	{
		m_bBreakLowerBody = true;
		m_bBreakBalance = false;
	}

	if (!(g_ctx.m_local->m_fFlags() & FL_ONGROUND))
	{
		m_bBreakLowerBody = false;
		m_bBreakBalance = false;
	}
}


float antiaim::at_target() {
	int cur_tar = 0;
	float last_dist = 999999999999.0f;

	for (int i = 0; i < g_csgo.m_globals()->m_maxclients; i++) {
		auto m_entity = static_cast<entity_t*>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!m_entity || m_entity == g_ctx.m_local)
			continue;

		if (!m_entity->is_player())
			continue;

		auto m_player = (player_t*)m_entity;
		if (!m_player->IsDormant() && m_player->is_alive() && m_player->m_iTeamNum() != g_ctx.m_local->m_iTeamNum()) {
			float cur_dist = (m_entity->m_vecOrigin() - g_ctx.m_local->m_vecOrigin()).Length();

			if (!cur_tar || cur_dist < last_dist) {
				cur_tar = i;
				last_dist = cur_dist;
			}
		}
	}

	if (cur_tar) {
		auto m_entity = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(cur_tar));
		if (!m_entity) {
			return g_ctx.get_command()->m_viewangles.y;
		}

		Vector target_angle = math::calculate_angle(g_ctx.m_local->m_vecOrigin(), m_entity->m_vecOrigin());
		return target_angle.y;
	}

	return g_ctx.get_command()->m_viewangles.y;
}

void MakeShiftYaw(CUserCmd* cmd, bool& SendPacket)
{
	int index;

	if (!(g_ctx.m_local->m_fFlags() & FL_ONGROUND)) { // Air
		index = g_cfg.antiaim.shift_yaw[2];
	}

	if (g_ctx.m_local->m_vecVelocity().Length() > 0 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) { // Move
		index = g_cfg.antiaim.shift_yaw[1];
	}
	if (g_ctx.m_local->m_vecVelocity().Length() < 1 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) // stand
	{
		index = g_cfg.antiaim.shift_yaw[0];
	}

	if (g_cfg.misc.fakewalk && g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.fakewalk_key) & FL_ONGROUND) // slowwalk
	{
		index = g_cfg.antiaim.shift_yaw[3];
	}


	if (!SendPacket)
	{
		cmd->m_viewangles.y += index;
	}
	else {
		cmd->m_viewangles.y -= index;
	}

	SendPacket = true;

	cmd->m_viewangles.y += 180.f; // breaker


	/*
	static bool shifting_tickbase = false;
	static bool shifting = false;

	if (!shifting)
	{
		shifting_tickbase = true;
		g_ctx.send_packet = true;
	}
	else
	{
		cmd->m_viewangles.y = math::normalize_yaw(cmd->m_viewangles.y + 180.f);
		if (g_csgo.m_clientstate()->m_nChokedCommands > 0)
		{
			cmd->m_tickcount = INT_MAX;
		}
		if (g_csgo.m_clientstate()->m_nChokedCommands < 6)
		{
			g_ctx.send_packet = false;
		}
		else
		{
			g_ctx.send_packet = true;
			shifting_tickbase = false;
		}
		return;
	}
	*/
}

static auto perform_rotation = [=](Vector o_angle, float offset, float rot_deg, float rot_speed, bool center_rotation) {
	Vector ang = o_angle;
	ang += offset - (center_rotation ? rot_deg / 2.0f : rot_deg) + std::fmodf(g_csgo.m_globals()->m_curtime * 222.49223595f * rot_speed, rot_deg);
	return  ang.y;
};
float randnum(int Min, int Max)
{
	return ((rand() % (Max - Min)) + Min);
}
float antiaim::get_pitch() {
	int	index;

	if (!(g_ctx.m_local->m_fFlags() & FL_ONGROUND)) { // Air
		index = g_cfg.antiaim.pitch[2];
	}

	if (g_ctx.m_local->m_vecVelocity().Length() > 0 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) { // Move
		index = g_cfg.antiaim.pitch[1];
	}
	if (g_ctx.m_local->m_vecVelocity().Length() < 1 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) // stand
	{
		index = g_cfg.antiaim.pitch[0];
	}

	if (g_cfg.misc.fakewalk && g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.fakewalk_key) & FL_ONGROUND) // slowwalk
	{
		index = g_cfg.antiaim.pitch[3];
	}

	float pitch = g_ctx.get_command()->m_viewangles.x;

	if (index)
	{
		switch (index) {
		case 1:
			pitch = 89.0f;
			break;
		case 2:
			pitch = -89.0f;
			break;
		case 3:
			static int AdaptiveDown = 1;
			static int Down = 2;
			static int AdaptiveUp = 3;
			static int Up = 4;
			static int JitterStep;
			if (JitterStep != 0)
			{
				if (JitterStep == AdaptiveDown)
				{
					pitch = 75.0f;
					JitterStep = randnum(2, 4);
				}
				else if (JitterStep == AdaptiveUp)
				{
					pitch = -35.0f;
					JitterStep = randnum(1, 2);
				}
				else if (JitterStep == Down)
				{
					pitch = 88.0f;
					JitterStep = randnum(1, 4);
				}
				else if (JitterStep == Up)
				{
					pitch = -47.0f;
					JitterStep = randnum(2, 3);
				}
			}
			else
			{
				JitterStep = randnum(1, 4);
			}
			break;
		}
	}
	return pitch;
}



float quick_normalize(float degree, const float min, const float max) {
	while (degree < min)
		degree += max - min;
	while (degree > max)
		degree -= max - min;

	return degree;
}



enum
{
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	DEVIL
};

void balance_max()
{

}

void balance_min()
{

}

void trigger_switch()
{
	static bool sFlip = false;
	if ((g_ctx.get_command()->m_tickcount % 32) > 11)
		sFlip = true;
	else
		sFlip = false;

	g_ctx.get_command()->m_viewangles.y += math::random_float(0, 10);

	if (next_lby_update() && !g_ctx.send_packet)
		g_ctx.get_command()->m_viewangles.y -= 100;
	else
		g_ctx.get_command()->m_viewangles.y += sFlip ? -89 : 89;
}

void trigger_yaw_max()
{
	if (!g_ctx.send_packet)
	{
		g_ctx.get_command()->m_viewangles.y += 180.f;
	}
	else
	{
		g_ctx.get_command()->m_viewangles.y += 180.f + empty_get_max_desync_delta();
	}
}

void trigger_yaw_min()
{

}

float antiaim::get_real() {

	int	index;
	int	base_angle;
	int	offset;
	int switchers_desync_mode;
	bool desync_enable;
	int delta_swap;
	bool shifts_enable;
	bool switch_yaw_enable;
	bool tank_aa;
	vec3_t oldAngle = g_ctx.get_command()->m_viewangles;
	float oldForward = g_ctx.get_command()->m_forwardmove;
	float oldSideMove = g_ctx.get_command()->m_sidemove;


	if (!(g_ctx.m_local->m_fFlags() & FL_ONGROUND)) { // Air
		index = g_cfg.antiaim.yaw[2];
		offset = g_cfg.antiaim.yaw_offset[2];
		base_angle = g_cfg.antiaim.base_angle[2];
		switchers_desync_mode = g_cfg.antiaim.desync_mode[2];
		desync_enable = g_cfg.antiaim.antiaim_desync[2];
		delta_swap = g_cfg.antiaim.desync_delta_swap_st[2];
		shifts_enable = g_cfg.antiaim.enable_shifts[2];
		switch_yaw_enable = g_cfg.antiaim.enable_switching_yaw[2];
		tank_aa = g_cfg.antiaim.tank_aa[2];
	}

	if (g_ctx.m_local->m_vecVelocity().Length() > 0 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) { // Move
		index = g_cfg.antiaim.yaw[1];
		offset = g_cfg.antiaim.yaw_offset[1];
		base_angle = g_cfg.antiaim.base_angle[1];
		switchers_desync_mode = g_cfg.antiaim.desync_mode[1];
		desync_enable = g_cfg.antiaim.antiaim_desync[1];
		delta_swap = g_cfg.antiaim.desync_delta_swap_st[1];
		shifts_enable = g_cfg.antiaim.enable_shifts[1];
		switch_yaw_enable = g_cfg.antiaim.enable_switching_yaw[1];
		tank_aa = g_cfg.antiaim.tank_aa[1];
	}
	if (g_ctx.m_local->m_vecVelocity().Length() < 1 && g_ctx.m_local->m_fFlags() & FL_ONGROUND) // stand
	{
		index = g_cfg.antiaim.yaw[0];
		offset = g_cfg.antiaim.yaw_offset[0];
		base_angle = g_cfg.antiaim.base_angle[0];
		switchers_desync_mode = g_cfg.antiaim.desync_mode[0];
		desync_enable = g_cfg.antiaim.antiaim_desync[0];
		delta_swap = g_cfg.antiaim.desync_delta_swap_st[0];
		shifts_enable = g_cfg.antiaim.enable_shifts[0];
		switch_yaw_enable = g_cfg.antiaim.enable_switching_yaw[0];
		tank_aa = g_cfg.antiaim.tank_aa[0];
	}

	if (g_cfg.misc.fakewalk && g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.fakewalk_key) & FL_ONGROUND) // slowwalk
	{
		index = g_cfg.antiaim.yaw[3];
		base_angle = g_cfg.antiaim.base_angle[3];
		offset = g_cfg.antiaim.yaw_offset[3];
		switchers_desync_mode = g_cfg.antiaim.desync_mode[3];
		desync_enable = g_cfg.antiaim.antiaim_desync[3];
		delta_swap = g_cfg.antiaim.desync_delta_swap_st[3];
		shifts_enable = g_cfg.antiaim.enable_shifts[3];
		switch_yaw_enable = g_cfg.antiaim.enable_switching_yaw[3];
		tank_aa = g_cfg.antiaim.tank_aa[3];
	}

	float yaw = g_ctx.get_command()->m_viewangles.y;

	if (index) {

		if (base_angle == 1) {
			vec3_t angles; g_csgo.m_engine()->GetViewAngles(angles);
			g_ctx.get_command()->m_viewangles.y = angles.y;
		}

		if (base_angle == 2)
			g_ctx.get_command()->m_viewangles.y = at_target();

		switch (index) {
		case 1:
			g_ctx.get_command()->m_viewangles.y += 180;
			break;
		case 2:
			g_ctx.get_command()->m_viewangles.y += g_csgo.m_globals()->m_curtime * 420.0f;
			break;
		case 3:
			g_ctx.get_command()->m_viewangles.y += 180 + (math::random_float(-50, 50));
			break;
		case 4:
			g_ctx.get_command()->m_viewangles.y = g_ctx.m_local->m_flLowerBodyYawTarget();
			break;
		case 5: {
			g_ctx.get_command()->m_viewangles.y = g_ctx.m_local->m_flLowerBodyYawTarget() + g_cfg.antiaim.lby_delta;
			break;
		}
		case 6:
			g_ctx.get_command()->m_viewangles.y = freestanding();
			break;
		case 7:
			g_ctx.get_command()->m_viewangles.y += 180 + (math::random_float(0, 15) - 25);
			break;
		}

		g_ctx.get_command()->m_viewangles.y += offset;
	}

	if (shifts_enable)
	{
		MakeShiftYaw(g_ctx.get_command(), g_ctx.send_packet);
	}

	if (switch_yaw_enable)
	{
		static bool sFlip = false;
		if ((g_ctx.get_command()->m_tickcount % 32) > 11)
			sFlip = true;
		else
			sFlip = false;

		if (next_lby_update() && !g_ctx.send_packet)
			g_ctx.get_command()->m_viewangles.y -= 100;
		else
			g_ctx.get_command()->m_viewangles.y += sFlip ? -delta_swap : delta_swap;
	}

	if (tank_aa)
	{
		if (g_ctx.get_command()->m_command_number % 3)
		{
			g_ctx.get_command()->m_viewangles.y -= GetMaxFakeDelta();
		}
		else
		{
			g_ctx.get_command()->m_viewangles.y += GetMaxFakeDelta();
		}
	}

	if (desync_enable)
	{
		if (g_ctx.send_packet)
		{
			switch (switchers_desync_mode)
			{
			case 0: // мем 1
			{
				jitter();
				break;
			}
			case 1: // мем 2
			{
				balance();
				break;
			}
			case 2: // такие дела соси хуй быдло
			{
				static bool sFlip = false;
				if ((g_ctx.get_command()->m_tickcount % 32) > 11)
					sFlip = true;
				else
					sFlip = false;

				if (next_lby_update() && !g_ctx.send_packet)
					g_ctx.get_command()->m_viewangles.y -= 100;
				else
					g_ctx.get_command()->m_viewangles.y += sFlip ? -delta_swap : delta_swap;
				break;

			}
			}
		}
	}

	return yaw;
}

void antiaim::get_unpredicted_velocity() {
	if (!g_ctx.m_local)
		return;

	unpredicted_velocity = g_ctx.m_local->m_vecVelocity();
}
void AngleVectorsANtiAim(const Vector& angles, Vector* forward)//most of this is also defined in resolver so i thought why dont make a resolver out of it
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}
void NormalizeNum(Vector& vIn, Vector& vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}
float fov_ent(Vector ViewOffSet, Vector View, player_t* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = entity->hitbox_position(hitbox);
	AngleVectorsANtiAim(Angles, &Forward);
	VectorSubtract(AimPos, Origin, Delta);
	NormalizeNum(Delta, Delta);
	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / PI));
}
int closest()
{
	int index = -1;
	float lowest_fov = 180.f;

	if (!g_ctx.m_local)
		return -1;

	if (!g_ctx.m_local->is_alive_local())
		return -1;

	Vector local_position = g_ctx.m_local->GetAbsOrigin() + g_ctx.m_local->m_vecViewOffset();
	Vector angles;
	g_csgo.m_engine()->GetViewAngles(angles);
	for (int i = 1; i <= g_csgo.m_globals()->m_maxclients; i++)
	{
		auto entity = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!entity || entity->m_iHealth() <= 0 || entity->m_iTeamNum() == g_ctx.m_local->m_iTeamNum() || entity->IsDormant() || entity == g_ctx.m_local)
			continue;

		float fov = fov_ent(local_position, angles, entity, 0);
		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}
	return index;
}
static bool jitter = false;
void antiaim::freestanding_spin()
{
	float range = g_cfg.antiaim.freestandjitterrange; static int Ticks = 0; if (g_csgo.m_inputsys()->IsButtonDown(ButtonCode_t::MOUSE_LEFT)) return;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	static float hold = 0.f;
	Vector besthead;
	float opposite = 0.f;
	auto leyepos = g_ctx.m_local->m_vecOrigin() + g_ctx.m_local->m_vecViewOffset();
	auto headpos = g_ctx.m_local->hitbox_position(0);
	auto origin = g_ctx.m_local->m_vecOrigin();
	auto checkWallThickness = [&](player_t* pPlayer, Vector newhead) -> float {
		Vector endpos1, endpos2;  Vector eyepos = pPlayer->m_vecVelocity() + pPlayer->m_vecViewOffset();
		Ray_t ray; ray.Init(newhead, eyepos); CTraceFilterSkipTwoEntities filter(pPlayer, g_ctx.m_local);
		trace_t trace1, trace2; g_csgo.m_trace()->TraceRay(ray, MASK_SHOT | MASK_SHOT_BRUSHONLY, &filter, &trace1);
		if (trace1.DidHit()) endpos1 = trace1.endpos; else return 0.f;
		ray.Init(eyepos, newhead); g_csgo.m_trace()->TraceRay(ray, MASK_SHOT | MASK_SHOT_BRUSHONLY, &filter, &trace1);
		if (trace2.DidHit()) endpos2 = trace2.endpos;  float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3; };  int index = closest(); static player_t* entity;
		if (!g_ctx.m_local->is_alive_local()) { hold = 0.f; }  if (index != -1) entity = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(index));
		if (!entity || entity->IsDormant()) { g_ctx.get_command()->m_viewangles.y -= 180.f; return; }  float radius = Vector(headpos - origin).Length2D();
		float server_time = g_ctx.m_local->m_nTickBase() * g_csgo.m_globals()->m_interval_per_tick; double speed = 250;
		double exe = fmod(static_cast<double>(server_time) * speed, range * 2); if (index == -1) { no_active = true; }
		else {
			for (float besthead = 0; besthead < 7; besthead += 0.1)
			{
				Vector newhead(radius * cos(besthead) + leyepos.x, radius * sin(besthead) + leyepos.y, leyepos.z);
				float totalthickness = 0.f;
				no_active = false;
				totalthickness += checkWallThickness(entity, newhead);
				if (totalthickness > highestthickness)
				{
					highestthickness = totalthickness; opposite = besthead - 180; bestrotation = besthead;
				}
			}
		}
		if (no_active)
		{
			g_ctx.get_command()->m_viewangles.y -= (180 - range) + static_cast<float>(exe); return;
		}
		else {
			if (g_ctx.send_packet)
			{
				m_iJitter = !m_iJitter;
				if (m_iJitter)
					g_ctx.get_command()->m_viewangles.y = RAD2DEG(bestrotation) - get_max_desync_delta();
				else g_ctx.get_command()->m_viewangles.y = RAD2DEG(bestrotation) + 50.f;
			}
			else
			{
				g_ctx.get_command()->m_viewangles.y = RAD2DEG(bestrotation - range) + static_cast<float>(exe);
			}
		}
}


float antiaim::freestanding() {
	bool change = false;
	float servertime = util::server_time();
	float nextchange = servertime + g_cfg.antiaim.freestanddelay;
	if (nextchange < servertime) {
		nextchange = servertime + g_cfg.antiaim.freestanddelay;
		change = true;
	}
	else
		change = false;

	if (nextchange - g_csgo.m_globals()->m_interval_per_tick < servertime)
	{
		change = true;
	}
	else
		change = false;
	enum {
		back,
		right,
		left,
		forward,
		jitter,
		right2,
		left2,
	};

	vec3_t view_angles;
	g_csgo.m_engine()->GetViewAngles(view_angles);

	static constexpr int damage_tolerance = 30;// make this a variable that the user can enter

	std::vector< player_t* > enemies;

	auto get_target = [&]() -> player_t * {
		player_t* target = nullptr;
		float best_fov = 360.f;

		for (int id = 1; id <= g_csgo.m_globals()->m_maxclients; id++) {
			auto e = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(id));

			if (!e->valid(true, !g_cfg.antiaim.freestanddormant))
				continue;

			float fov = math::get_fov(view_angles, math::calculate_angle(g_ctx.m_local->get_eye_pos(), e->m_vecOrigin()));

			if (fov < best_fov) {
				target = e;
				best_fov = fov;
			}

			enemies.push_back(e);
		}

		return target;
	};

	player_t* e = get_target();
	int ServerTime = (float)g_ctx.m_local->m_nTickBase() * g_csgo.m_globals()->m_interval_per_tick;
	int value = ServerTime % 2;

	//prepare all values for pj
	int jitterrange = 0;//actualy outputangles but nvm
	int jittermode = 0;
	static bool jitterleaver = false;

	bool shouldpanic = false;
	//set values to a value
	if (g_cfg.antiaim.panicjitter)
	{
		jitterleaver = jitterleaver;
		shouldpanic = true;
		if (g_cfg.antiaim.panicjittermode == 0)//away from desync IMPORTANT: never flip angles into desync angles!!
		{
			int desyncangles = g_ctx.m_globals.fake_angles.y;
			int olddesyncangles = 0;
			if (desyncangles != olddesyncangles)
			{
				olddesyncangles = desyncangles;
			}
			if (jitterleaver) {
				jitterrange = RandomInt(-g_cfg.antiaim.panicjitterrange, g_cfg.antiaim.panicjitterrange);
			}
			else
				jitterrange = RandomInt(g_cfg.antiaim.panicjitterrange, -g_cfg.antiaim.panicjitterrange);

			if (jitterrange == olddesyncangles || jitterrange == desyncangles)
			{
				jitterrange = -10;
			}
		}
		else if (g_cfg.antiaim.panicjittermode == 1)//
		{
			int lby = g_ctx.m_local->m_flLowerBodyYawTarget();

			jitterrange = RandomInt((lby - g_cfg.antiaim.panicjitterrange), (lby + g_cfg.antiaim.panicjitterrange));
		}
		else if (g_cfg.antiaim.panicjittermode == 2)
		{
			jitterrange = math::random_float(math::random_float(-g_cfg.antiaim.panicjitterrange, -g_cfg.antiaim.panicjitterrange / 2), math::random_float(g_cfg.antiaim.panicjitterrange, g_cfg.antiaim.panicjitterrange * 2));
		}
	}
	static int Ticks = 120;
	if (!e)
	{
		vec3_t yaw;
		g_csgo.m_engine()->GetViewAngles(yaw);

		Ticks += 4; //change the 4 for the speed of the spin

		if (Ticks > 240)
			Ticks = 120;

		return (yaw.y - Ticks); // 180z using ticks owo
	}

	auto calculate_damage = [&](vec3_t point) -> int {
		int damage = 0;
		for (auto& enemy : enemies)
			damage += autowall::get().calculate_return_info(enemy->get_eye_pos(), point, enemy, g_ctx.m_local, 1).m_damage;

		return damage;
	};

	auto rotate_and_extend = [](vec3_t position, float yaw, float distance) -> vec3_t {
		vec3_t direction;
		math::angle_vectors(vec3_t(0, yaw, 0), direction);

		return position + (direction * distance);
	};

	vec3_t
		head_position = g_ctx.m_local->get_eye_pos(),
		at_target = math::calculate_angle(g_ctx.m_local->m_vecOrigin(), e->m_vecOrigin());

	float angles[8] = {
		at_target.y + 180.f,
		at_target.y - 70.f,
		at_target.y + 70.f,
		at_target.y,
		at_target.y + 180 + (math::random_float(g_cfg.antiaim.freestandjitterrange, -g_cfg.antiaim.freestandjitterrange)),
		at_target.y - 135.f,
		at_target.y + 135.f,
	};

	vec3_t head_positions[7] = {
		rotate_and_extend(head_position, at_target.y + 180.f, 17.f),
		rotate_and_extend(head_position, at_target.y - 70.f, 17.f),
		rotate_and_extend(head_position, at_target.y + 70.f, 17.f),
		rotate_and_extend(head_position, at_target.y, 17.f),
		rotate_and_extend(head_position, at_target.y + 180 + math::random_float(g_cfg.antiaim.freestandjitterrange, -g_cfg.antiaim.freestandjitterrange), 17.f),
		rotate_and_extend(head_position, at_target.y - 135.f, 17.f),
		rotate_and_extend(head_position, at_target.y + 135.f, 17.f)
	};

	int damages[7] = {
		calculate_damage(head_positions[back]),
		calculate_damage(head_positions[right]),
		calculate_damage(head_positions[left]),
		calculate_damage(head_positions[forward]),
		calculate_damage(head_positions[jitter]),
		calculate_damage(head_positions[right2]),
		calculate_damage(head_positions[left2]),
	};
	if (g_ctx.m_globals.missed_shots[e->EntIndex()] == 1 || g_ctx.m_globals.missed_shots[e->EntIndex()] == 3 || g_ctx.m_globals.missed_shots[e->EntIndex()] == 5 || g_ctx.m_globals.missed_shots[e->EntIndex()] > 6)
		change = true;
	else
		change = false;

	if (shouldpanic && g_cfg.antiaim.panicjitterhp > g_ctx.m_local->m_iHealth())
	{
		if (jitterleaver)
			return(at_target.y + 180 + jitterrange);
		else
			return(at_target.y + 180 - jitterrange);
	}

	if (g_cfg.antiaim.jitterbetweensecondbestangle)	//large skeet feature lol		it just confuses some resolvers sometimes		nyani
	{
		if (change)
		{
			if (damages[right2] > damage_tolerance && damages[left2] > damage_tolerance)
			{
				if (damages[forward] > damage_tolerance)
				{
					if (damages[back] > damage_tolerance || damages[jitter] > damage_tolerance)
					{
						if (damages[back] < damage_tolerance) {
							g_ctx.m_globals.headsavenotshure = true; g_ctx.m_globals.headsave = false;
							return angles[jitter];
						}
						else
						{
							if (shouldpanic)
							{
								if (jitterleaver)
									return(at_target.y + 180 + jitterrange);
								else
									return(at_target.y + 180 - jitterrange);
							}
							else
							{
								if (damages[forward] < damage_tolerance) {
									g_ctx.m_globals.headsavenotshure = true; g_ctx.m_globals.headsave = false;
									return angles[forward];
								}
								else {
									return angles[back];
									g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = false;
								}
							}
						}
					}
					else
					{
						g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = true;
						return angles[jitter];
					}
				}
				else {
					g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = false;
					if (shouldpanic)
					{
						if (jitterleaver)
							return(at_target.y + jitterrange);
						else
							return(at_target.y - jitterrange);
					}
					else
					{
						return angles[forward];
					}
				}
			}
		}
		else
		{
			if (damages[right] > damage_tolerance && damages[right] > damage_tolerance)
			{
				if (damages[forward] > damage_tolerance)
				{
					if (damages[back] > damage_tolerance && damages[jitter] > damage_tolerance)
					{
						return angles[jitter];
					}
					else
					{
						if (damages[back] > damage_tolerance)
						{
							if (shouldpanic)
							{
								if (jitterleaver)
									return(angles[jitter] + jitterrange);
								else
									return(angles[jitter] - jitterrange);
							}
							else
							{
								return angles[jitter];
							}
						}
						else {
							return angles[back];
						}
					}
				}
				else {
					if (shouldpanic)
					{
						if (jitterleaver)
							return(at_target.y + jitterrange);
						else
							return(at_target.y - jitterrange);
					}
					else
					{
						return angles[forward];
					}
				}
				g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = false;
			}
		}
	}
	else {
		if (damages[right] > damage_tolerance && damages[left] > damage_tolerance)
		{
			if (shouldpanic)
			{
				if (jitterleaver)
					return(at_target.y + 180 + jitterrange);
				else
					return(at_target.y + 180 - jitterrange);
			}
			else
			{
				return angles[back];
			}
			g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = false;
		}
	}
	//return angles[back];
	if (at_target.x > 15.f)	//just replace the 15 with an float given by user
		return angles[jitter];		//autobackwards here c; large mutiny feature that is useless oof

	if (damages[right] == damages[left]) {
		auto trace_to_end = [](vec3_t start, vec3_t end) -> vec3_t {
			trace_t trace;
			CTraceFilterWorldOnly filter;
			Ray_t ray;

			ray.Init(start, end);
			g_csgo.m_trace()->TraceRay(ray, MASK_ALL, &filter, &trace);
			g_ctx.m_globals.headsavenotshure = true; g_ctx.m_globals.headsave = false;
			return trace.endpos;
		};

		vec3_t
			trace_right_endpos = trace_to_end(head_position, head_positions[right]),
			trace_left_endpos = trace_to_end(head_position, head_positions[left]),
			trace_right_endpos2 = trace_to_end(head_position, head_positions[right2]),
			trace_left_endpos2 = trace_to_end(head_position, head_positions[left2]);

		Ray_t ray;
		trace_t trace;
		CTraceFilterWorldOnly filter;

		ray.Init(trace_right_endpos, e->get_eye_pos());
		g_csgo.m_trace()->TraceRay(ray, MASK_ALL, &filter, &trace);
		float distance_1 = (trace.startpos - trace.endpos).Length();

		ray.Init(trace_left_endpos, e->get_eye_pos());
		g_csgo.m_trace()->TraceRay(ray, MASK_ALL, &filter, &trace);
		float distance_2 = (trace.startpos - trace.endpos).Length();

		ray.Init(trace_right_endpos2, e->get_eye_pos());
		g_csgo.m_trace()->TraceRay(ray, MASK_ALL, &filter, &trace);
		float distance_3 = (trace.startpos - trace.endpos).Length();

		ray.Init(trace_left_endpos2, e->get_eye_pos());
		g_csgo.m_trace()->TraceRay(ray, MASK_ALL, &filter, &trace);
		float distance_4 = (trace.startpos - trace.endpos).Length();
		if (g_cfg.antiaim.jitterbetweensecondbestangle)
		{
			if (change)
			{
				if (fabs(distance_3 - distance_4) > 20.f) {
					g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = true;
					return (distance_3 < distance_4) ? angles[right2] : angles[left2];
				}
				else {
					g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = false;
					return angles[jitter];
				}
			}
			else
			{
				if (fabs(distance_1 - distance_2) > 15.f) {
					g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = true;
					return (distance_1 < distance_2) ? angles[right] : angles[left];
				}
				else {
					g_ctx.m_globals.headsavenotshure = true; g_ctx.m_globals.headsave = false;
					if (shouldpanic)
					{
						if (jitterleaver)
							return(at_target.y + 180 + jitterrange);
						else
							return(at_target.y + 180 - jitterrange);
					}
					else
					{
						return angles[back];
					}
				}
			}
		}
		else {
			if (fabs(distance_1 - distance_2) > 15.f) {
				return (distance_1 < distance_2) ? angles[right] : angles[left];
				g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = true;
			}
			else {
				return angles[back];
				g_ctx.m_globals.headsavenotshure = true; g_ctx.m_globals.headsave = false;
			}
		}
	}
	else {
		g_ctx.m_globals.headsavenotshure = false; g_ctx.m_globals.headsave = true;
		return (damages[right] < damages[left]) ? angles[right] : angles[left];
	}
}