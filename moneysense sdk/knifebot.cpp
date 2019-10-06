#include "knifebot.h"
using namespace std;

float GetDamage(Vector Angle)
{
	return 0;
}

void knifebot::run()
{
	auto weapon = g_ctx.m_local->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	if (!weapon->is_knife())
		return;

	auto LocalPlayer = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	auto trace_line = [](Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, player_t* ignore, int collisionGroup, trace_t* ptr)
	{
		Ray_t ray;

		CTraceFilter filter;
		filter.pSkip = ignore;

		ray.Init(vecAbsStart, vecAbsEnd);

		g_csgo.m_trace()->TraceRay(ray, mask, &filter, ptr);

	};
	player_t* m_pEnt = nullptr;
	for (int y = 0; y <= 360; y += 360.f / 6.f) {
		for (int x = -89; x <= 89; x += 179.f / 6.f) {
			Vector ang = Vector(x, y, 0);
			Vector dir;
			trace_t trace;
			math::angle_vectors(ang, dir);
			trace_line(g_ctx.m_local->get_eye_pos(), g_ctx.m_local->get_eye_pos() + (dir * 64), MASK_SHOT, LocalPlayer, 0, &trace);

			if (m_pEnt == nullptr)
				continue;

			if (m_pEnt == LocalPlayer)
				continue;

			if (!m_pEnt->is_alive())
				continue;

			if (!(m_pEnt->m_iHealth() > 0))
				continue;

			if (m_pEnt->m_iTeamNum() == m_pEnt->m_iTeamNum())
				continue;

			if (m_pEnt->IsDormant())
				continue;

			player_info_t info;

			if (!(g_csgo.m_engine()->GetPlayerInfo(m_pEnt->EntIndex(), &info)))
				continue;

			if (GetDamage(Vector(x, y, 0)) != 0)
			{
				g_ctx.get_command()->m_viewangles = Vector(x, y, 0);
				g_ctx.get_command()->m_buttons |= IN_ATTACK2;
			}
			return;
		}
	}
}