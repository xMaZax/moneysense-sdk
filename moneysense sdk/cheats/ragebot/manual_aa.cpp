#include "..\..\hack\ragebot.h"

void manual_aa::update() {
	static bool
		holding_left = false,
		holding_right = false,
		holding_back = false;

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.left_manualkey) && !holding_left) {
		if (side == SIDE_LEFT) {
			side = SIDE_NONE;
		}
		else {
			side = SIDE_LEFT;
		}

		holding_left = true;
	}
	else if (holding_left && !g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.left_manualkey)) {
		holding_left = false;
	}

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.right_manualkey) && !holding_right) {
		if (side == SIDE_RIGHT) {
			side = SIDE_NONE;
		}
		else {
			side = SIDE_RIGHT;
		}

		holding_right = true;
	}
	else if (holding_right && !g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.right_manualkey)) {
		holding_right = false;
	}

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.back_manualkey) && !holding_back) {
		if (side == SIDE_BACK) {
			side = SIDE_NONE;
		}
		else {
			side = SIDE_BACK;
		}

		holding_back = true;
	}
	else if (holding_back && !g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.back_manualkey)) {
		holding_back = false;
	}
	// 	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.antiaim.disable_manualkey) && !holding_back)
	// 	{
	// 		int side = SIDE_NONE;
	// 	}
}

bool manual_aa::run() {
	vec3_t engine_angles; g_csgo.m_engine()->GetViewAngles(engine_angles);
	float yaw = engine_angles.y;
	float LBYDelta = g_ctx.get_command()->m_viewangles.y - g_ctx.m_local->m_flLowerBodyYawTarget(); bool breakinglby;
	if (fabsf(LBYDelta) > 35.f || fabsf(LBYDelta) < -35.f)
		breakinglby = true;
	else
		breakinglby = false;
	switch (side) {
	case SIDE_LEFT:
		if (g_ctx.send_packet) {
			switch (g_cfg.antiaim.manualaafakes)
			{
			case 0:
				g_ctx.get_command()->m_viewangles.y = yaw - (90.0f + (math::random_float(0.f, 30.f) - 15.f)); break;
			case 1:
			{
				if (breakinglby)
					g_ctx.get_command()->m_viewangles.y = g_ctx.m_local->m_flLowerBodyYawTarget();
				else
					g_ctx.get_command()->m_viewangles.y = (g_ctx.m_local->m_flLowerBodyYawTarget() + 140 + rand() % 123) - rand() % 20;
			}break;
			case 2:
			{
				g_ctx.get_command()->m_viewangles.y -= (float)(fmod(util::server_time() / 0.5f * 75.0f, 105.0f));
			}break;
			}
		}
		else {
			g_ctx.get_command()->m_viewangles.y = yaw + 90.0f;
		}

		break;
	case SIDE_RIGHT:
		if (g_ctx.send_packet) {
			switch (g_cfg.antiaim.manualaafakes)
			{
			case 0:
				g_ctx.get_command()->m_viewangles.y = yaw + (90.0f + (math::random_float(0.f, 30.f) - 15.f)); break;
			case 1:
			{
				if (breakinglby)
					g_ctx.get_command()->m_viewangles.y = g_ctx.m_local->m_flLowerBodyYawTarget();
				else
					g_ctx.get_command()->m_viewangles.y = (g_ctx.m_local->m_flLowerBodyYawTarget() + 140 + rand() % 123) - rand() % 20;
			}break;
			case 2:
			{
				g_ctx.get_command()->m_viewangles.y += (float)(fmod(util::server_time() / 0.5f * 75.0f, 105.0f));
			}break;
			}
		}
		else {
			g_ctx.get_command()->m_viewangles.y = yaw - 90.0f;
		}

		break;
	case SIDE_BACK:
		if (g_ctx.send_packet) {
			static bool side[2] = { false, true };

			float range = side[1] ? 100 : 80;

			g_ctx.get_command()->m_viewangles.y = yaw + (side[0] ? range : -range);

			side[1] = !side[1];

			if (side[1]) {
				side[0] = !side[0];
			}
		}
		else {
			g_ctx.get_command()->m_viewangles.y = yaw + 180.0f;
		}

		break;
	}

	return side != SIDE_NONE;
}
/*
CUserCmd* cmd;
void manual_aa::unhittable_aa_skeet()
{
	auto local_player = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!local_player)
		return;

	if (local_player->m_iHealth() <= 0)
		return;

	if (g_cfg.ragebot.aa_types == 0)
	{
		return;
	}
	else if (g_cfg.ragebot.aa_types == 1)
	{
		backwards(cmd); 
	}
	else if (g_cfg.ragebot.aa_types == 2)
	{
		sideways(cmd);
	}
	else if (g_cfg.ragebot.aa_types == 3)
	{
		backjitter(cmd);
	}
	else if (g_cfg.ragebot.aa_types == 4)
	{
		legit(cmd);
	}
	else if (g_cfg.ragebot.aa_types == 5)
	{
		if (!g_ctx.send_packet) {
			float server_time = local_player->m_nTickBase() * g_csgo.m_globals()->m_interval_per_tick;
			double rate = 360.0 / 1.618033988749895;
			double yaw = fmod(static_cast<double>(server_time) * rate, 360.0);
			cmd->m_viewangles.y += static_cast<float>(yaw);
		}
	}
}

void manual_aa::backwards(CUserCmd* cmd) {
	if (!g_ctx.send_packet)
		cmd->m_viewangles.y += 180.000000;
}

void manual_aa::legit(CUserCmd* cmd) {
	if (!g_ctx.send_packet)
		cmd->m_viewangles.y += 90;
}

void manual_aa::sideways(CUserCmd* cmd)
{
	if (!g_ctx.send_packet)
		cmd->m_viewangles.y -= 90;
}

void manual_aa::backjitter(CUserCmd* cmd) {
	if (!g_ctx.send_packet)
		cmd->m_viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f));
}*/