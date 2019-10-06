#include "fakewalk.h"
#include "..\..\Hack\ragebot.h"
#include "..\sdk\animation_state.h"
#define square( x ) ( x * x )
Vector unpredictedVelocity = Vector(0, 0, 0);
inline float FastSqrt(float x) {

	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;

	return *(float*)&i;
}

void fakewalk::create_move() {
	if (g_cfg.misc.fakewalk && g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.fakewalk_key))
	{
		auto get_speed = 31;

		float min_speed = (float)(FastSqrt(square(g_ctx.get_command()->m_forwardmove) + square(g_ctx.get_command()->m_sidemove) + square(g_ctx.get_command()->m_upmove)));
		if (min_speed <= 0.f)
			return;

		if (g_ctx.get_command()->m_buttons & IN_DUCK)
			get_speed *= 2.94117647f;

		if (min_speed <= get_speed)
			return;

		float kys = get_speed / min_speed;
		g_cfg.misc.fakewalk = true;
		g_ctx.get_command()->m_forwardmove *= kys;
		g_ctx.get_command()->m_sidemove *= kys;
		g_ctx.get_command()->m_upmove *= kys;

	}
}
void fakewalk::fake_duck(CUserCmd* ccmd, bool* bSendPacket) {

	static int fakeduckChokeAmount = 0;
	if (GetAsyncKeyState(g_cfg.misc.fakeduck))
	{
		if (fakeduckChokeAmount > 14)
		{
			fakeduckChokeAmount = 0;
			*bSendPacket = true;
		}
		else
			*bSendPacket = false;
		fakeduckChokeAmount++;
		auto choke = *(int*)(uintptr_t(g_csgo.m_clientstate()) + 0x4D28);

		if (g_ctx.m_local->m_fFlags() & FL_ONGROUND)
		{
			if (choke >= 7)
				ccmd->m_buttons |= IN_DUCK;
			else
				ccmd->m_buttons &= ~IN_DUCK;
		}
	}
	else
		fakeduckChokeAmount = 0;
}
void fakewalk::slidewalk() {

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.slidewalk_key))
	{
		if (g_ctx.get_command()->m_forwardmove > 0)
		{
			g_ctx.get_command()->m_buttons |= IN_BACK;
			g_ctx.get_command()->m_buttons &= ~IN_FORWARD;
		}

		if (g_ctx.get_command()->m_forwardmove < 0)
		{
			g_ctx.get_command()->m_buttons |= IN_FORWARD;
			g_ctx.get_command()->m_buttons &= ~IN_BACK;
		}

		if (g_ctx.get_command()->m_sidemove < 0)
		{
			g_ctx.get_command()->m_buttons |= IN_MOVERIGHT;
			g_ctx.get_command()->m_buttons &= ~IN_MOVELEFT;
		}

		if (g_ctx.get_command()->m_sidemove > 0)
		{
			g_ctx.get_command()->m_buttons |= IN_MOVELEFT;
			g_ctx.get_command()->m_buttons &= ~IN_MOVERIGHT;
		}
	}
}
