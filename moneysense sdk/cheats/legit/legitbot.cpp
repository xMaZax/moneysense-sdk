#include "legitbot.h"
#include "../../std2017.h"
ClientFrameStage_t is_stage;
player_t* is_local;
int besthitbox(player_t* e, Vector& ang)
{
	return HITBOX_HEAD;
}

void legitbot::create_move()
{
	sync();
	Aim();
}

void legitbot::sync()
{
	auto weapon = g_ctx.m_local->m_hActiveWeapon().Get();
	if (weapon->is_pistol())
	{
		fov = g_cfg.legit.fov_pistols;
		smooth = g_cfg.legit.smooth_pistols;
	}
	else if (weapon->is_smg())
	{
		fov = g_cfg.legit.fov_smg;
		smooth = g_cfg.legit.smooth_smg;
	}
	else if (weapon->is_shotgun())
	{
		fov = g_cfg.legit.fov_shotgun;
		smooth = g_cfg.legit.smooth_shotgun;
	}
	else if (weapon->is_rifle())
	{
		fov = g_cfg.legit.fov_rifle;
		smooth = g_cfg.legit.smooth_rifle;
	}
	else if (weapon->is_sniper())
	{
		fov = g_cfg.legit.fov_sniper;
		smooth = g_cfg.legit.smooth_sniper;
	}
}
player_t* legitbot::GetEnemyToCrosshair(Vector& viewAngles)
{
	g_csgo.m_engine()->GetViewAngles(viewAngles);
	float bestdelta = FLT_MAX;
	player_t* bestEntity = nullptr;
	for (size_t i = 1; i <= 64; i++)
	{
		auto m_local = g_ctx.m_local;
		auto e = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(i));
		if (e && !e->IsDormant() && e->m_iHealth() > 0 && e->m_iTeamNum() != g_ctx.m_local->m_iTeamNum())
		{
			float delta = math::get_fov(g_ctx.m_local->m_vecOrigin() + g_ctx.m_local->m_vecViewOffset(), viewAngles);
			if (delta < bestdelta && delta < fov)
			{
				bestdelta = delta;
				bestEntity = e;
			}
		}
	}
	return bestEntity;
}
bool legitbot::Aim()
{
	auto weapon = g_ctx.m_local->m_hActiveWeapon().Get();
	if (g_cfg.legit.enable_legit && GetAsyncKeyState(VK_LBUTTON) && !weapon->is_non_aim())
	{
		Vector viewAngles;
		g_csgo.m_engine()->GetViewAngles(viewAngles);
		player_t* entity = nullptr;
		player_t* e = legitbot::get().GetEnemyToCrosshair(viewAngles);

		if (e && e != nullptr && entity->point_visible(viewAngles))
		{
			Vector angle = math::calculate_angle(g_ctx.m_local->get_eye_pos(), e->get_eye_pos());
			Vector aim_angle = (angle - g_ctx.m_local->m_aimPunchAngle() * (rcs_ammount / 50)).Clamp();
			legitbot::get().Smooth(viewAngles, aim_angle, smooth / 4);

			if (!g_cfg.legit.silient)
				g_csgo.m_engine()->SetViewAngles(aim_angle);
			else
				g_ctx.get_command()->m_viewangles = aim_angle;

			return true;
		}
	}

	return false;
}
void legitbot::legit_aa(CUserCmd* pCmd)
{
	player_t* pLocal = nullptr;
	bool bSendPacket = nullptr;

	if ((pCmd->m_buttons & IN_USE) || pLocal->get_move_type() == MOVETYPE_LADDER)
		return;

	Vector oldAngle = pCmd->m_viewangles;
	float oldForward = pCmd->m_forwardmove;
	float oldSideMove = pCmd->m_sidemove;

	if (g_cfg.legit.aa_active && !pCmd->m_buttons & IN_ATTACK)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		static bool yFlip;
		if (ChokedPackets < 1)
		{
			bSendPacket = true;
		}
		else
		{
			bSendPacket = false;
			yFlip ? pCmd->m_viewangles.y += 90.f : pCmd->m_viewangles.y -= 90.f;
			ChokedPackets = -1;
		}
		yFlip != yFlip;

	}
}

void legitbot::legit_resolver(CUserCmd* cmd)
{
	typedef unsigned long BRUTEFORSING;
	if (is_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {

		if (g_cfg.legit.legit_resolver_active)
		{
			for (int i = 0; i <= g_csgo.m_globals()->m_maxclients; i++)
			{
				static int v36;
				auto e = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(i));
				auto local_player = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
				auto v3 = local_player;
				if (v3 != e)
				{
					auto animstate = e->get_animation_state();

					float lby_delta = local_player->m_flLowerBodyYawTarget();
					lby_delta = std::remainderf(lby_delta, 360.f);
					lby_delta = std2017::clamp(lby_delta, -60.f, 60.f);

					if (animstate->m_fDuckAmount > 0.0)
					{
						auto v29 = 0.0;
						if (animstate->m_flFeetSpeedUnknownForwardOrSideways < 0.0)
							v29 = 0.0;
						else
							v29 = std::fminf(BRUTEFORSING(animstate->m_flFeetSpeedUnknownForwardOrSideways), 0x3F800000);
					}

					animstate->m_flEyeYaw >= 35.0f;
					lby_delta >= 35.0f;
				}
			}
		}
	}
}

void legitbot::Smooth(Vector& viewAngles, Vector& angle, float smoothvalue)
{
	if (smoothvalue != 0)
	{
		angle = (viewAngles + (angle - viewAngles).Clamp() / smoothvalue).Clamp();
	}
}