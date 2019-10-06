#include "world_esp.h"

void worldesp::paint_traverse() {
	if (!g_ctx.available())
		return;

	auto local_player = reinterpret_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	for (int i = 1; i < g_csgo.m_entitylist()->GetHighestEntityIndex(); i++) {
		auto e = static_cast<entity_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!e)
			continue;

		if (e->IsDormant())
			continue;

		Box box; if (!util::get_bbox_world(e, box))
			continue;
	}
	for (int i = 0; i < g_csgo.m_entitylist()->GetHighestEntityIndex(); i++) {
		auto entity = reinterpret_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(i));
		if (entity && entity != local_player) {
			auto client_class = entity->GetClientClass();
			auto model_name = g_csgo.m_modelinfo()->GetModelNamevoid(entity->GetModel());

			if (!entity)
				continue;

			if (entity->IsDormant())
				continue;

			if (client_class->m_ClassID == ClassId::CPlantedC4) {
				if (g_cfg.esp.espbomb)
					bomb_esp((player_t*)entity);
			}

			if (client_class->m_ClassID == ClassId::CC4) {
				if (g_cfg.esp.espbomb)
					drop_bomb((player_t*)entity);
			}

			Box box; //if (!util::get_bbox_world(entity, box))
				//continue;

			if (g_cfg.esp.dropped_weapons)
				weapons((weapon_t*)entity, box);

			if (g_cfg.esp.projectiles)
				grenades((weapon_t*)entity, box);
		}
	}
}
void worldesp::drop_bomb(player_t* entity)
{
	auto client_class = entity->GetClientClass();
	vec3_t bomb_origin, bomb_position;
	bomb_origin = entity->origin();

	if (!math::world_to_screen(bomb_origin, bomb_position))
		return;

	render::get().text(fonts[BOMBICONS], bomb_position.x, bomb_position.y, Color(255, 255, 255), HFONT_CENTERED_X, "o");
}

void worldesp::bomb_esp(player_t* pt_hook)
{
	auto local_player = reinterpret_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	CCSBomb* bomb_search = nullptr;
	vec3_t bomb_origin, bomb_position;
	bomb_origin = pt_hook->origin();
	auto distance_bomb = local_player->get_eye_pos().DistTo(pt_hook->get_eye_pos());

	if (g_csgo.m_inputsys()->IsButtonDown(KEY_E)) {

		if (g_ctx.m_local->m_iTeamNum() != 2)
		{
			if (distance_bomb <= 85.0f)
			{
				render::get().rect_filled(0, 532, 65, 18, Color(0, 0, 0, 155));
				render::get().rect_filled(0, 532, 3, 18, Color(g_cfg.menu.menu_theme[0]));
				render::get().text(fonts[CTAB], 28, 534, Color(g_cfg.menu.menu_theme[0]), HFONT_CENTERED_X, "Defusing");
			}
		}
	}

	if (!math::world_to_screen(bomb_origin, bomb_position))
		return;

	render::get().text(fonts[BOMBICONS], bomb_position.x, bomb_position.y, Color(255, 255, 255), HFONT_CENTERED_X, "o");

	if (distance_bomb <= 895.0f)
		render::get().text(fonts[CTAB], bomb_position.x, bomb_position.y + 20, Color(245, 92, 108), HFONT_CENTERED_X, "dead");

	if (g_csgo.m_inputsys()->IsButtonDown(KEY_E)) {

		if (g_ctx.m_local->m_iTeamNum() != 2)
		{
			if (distance_bomb <= 85.0f) {
				render::get().rect_filled(bomb_position.x - 20, bomb_position.y + 33, 38, 15, Color(0, 0, 0, 155));
				render::get().text(fonts[CTAB], bomb_position.x, bomb_position.y + 33, Color(255, 154, 0), HFONT_CENTERED_X, "defuse");
				render::get().gradient(bomb_position.x - 20, bomb_position.y + 33, 38, 1, Color(51, 132, 255, 255), Color(235, 29, 173, 255), GRADIENT_HORIZONTAL);
			}
		}
	}

}

void worldesp::DrawZeusRange() {
	if (!g_csgo.m_engine()->IsConnected() || !g_csgo.m_engine()->IsInGame())
		return;

	if (!g_ctx.m_local) return;

	if (!*(bool*)((uintptr_t)g_csgo.m_input() + 0xAD))
		return;

	if (!g_cfg.esp.zeusrange)
		return;

	auto weapon = g_ctx.m_local->m_hActiveWeapon().Get();
	if (weapon->m_iItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_ZEUS)
		return;
#define M_PI 3.14159265358979323846
	float step = M_PI * 2.0 / 1023;
	float rad = weapon->get_csweapon_info()->range;
	Vector origin = g_ctx.m_local->get_eye_pos();

	static double rainbow;

	Vector screenPos;
	static Vector prevScreenPos;

	for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
	{
		Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z);

		trace_t trace;
		CTraceFilter filter;

		filter.pSkip = g_ctx.m_local;


		g_csgo.m_trace()->TraceRay(Ray_t(origin, pos), MASK_SHOT_BRUSHONLY, &filter, &trace);


		if (g_csgo.m_debugoverlay()->ScreenPosition(trace.endpos, screenPos))
			continue;

		if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f&& prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f) {
			rainbow += 0.00001;
			if (rainbow > 1.f)
				rainbow = 0;

			Color color = Color::FromHSB(rainbow, 1.f, 1.f);
			render::get().line(prevScreenPos.x, prevScreenPos.y, screenPos.x, screenPos.y, color);
			//DrawLine(prevScreenPos.x, prevScreenPos.y + 1, screenPos.x, screenPos.y + 1, color);
			//DrawLine(Vector2D(prevScreenPos.x, prevScreenPos.y - 1), Vector2D(screenPos.x, screenPos.y - 1), color);
		}
		prevScreenPos = screenPos;
	}
}

player_t* es;

void worldesp::grenades(weapon_t* m_nade, Box box) {

	if (!m_nade)
		return;

	auto client_class = m_nade->GetClientClass();

	vec3_t grenade_position, grenade_origin;
	grenade_origin = m_nade->origin();

	if (!math::world_to_screen(grenade_origin, grenade_position))
		return;

	if (client_class->m_ClassID == ClassId::CFlashbang) {
		render::get().text(fonts[GRENADEICONS], grenade_position.x, grenade_position.y, Color(255, 255, 255), HFONT_CENTERED_X, "i");
	}
	else if (client_class->m_ClassID == ClassId::CSmokeGrenade) {
		render::get().text(fonts[GRENADEICONS], grenade_position.x, grenade_position.y, Color(255, 255, 255), HFONT_CENTERED_X, "k");
		//auto time = g_csgo.m_globals()->m_interval_per_tick * (g_csgo.m_globals()->m_tickcount - es->smoke_grenade_tick_begin());

		//if (!(18 - time < 0)) {
		//	render::get().rect_filled(grenade_position.x - 18, grenade_position.y + 22, 36, 3, Color(10, 10, 10, 180));
		//	render::get().rect_filled(grenade_position.x - 18, grenade_position.y + 22, time * 2, 3, Color(167, 24, 71, 255));
		//}
	}

	else if (client_class->m_ClassID == ClassId::CIncendiaryGrenade) {
		render::get().text(fonts[GRENADEICONS], grenade_position.x, grenade_position.y, Color(255, 255, 255), HFONT_CENTERED_X, "n");
	}

	else if (client_class->m_ClassID == ClassId::CMolotovGrenade) {
		render::get().text(fonts[GRENADEICONS], grenade_position.x, grenade_position.y, Color(255, 255, 255), HFONT_CENTERED_X, "l");
	}

	else if (client_class->m_ClassID == ClassId::CHEGrenade) {
		render::get().text(fonts[GRENADEICONS], grenade_position.x, grenade_position.y, Color(255, 255, 255), HFONT_CENTERED_X, "j");
	}

	else if (client_class->m_ClassID == ClassId::CDecoyGrenade) {
		render::get().text(fonts[GRENADEICONS], grenade_position.x, grenade_position.y, Color(255, 255, 255), HFONT_CENTERED_X, "m");
	}
}

void worldesp::weapons(weapon_t * e, Box box) {

	auto client_class = e->GetClientClass();
	vec3_t weapon_origin, weapon_position;
	weapon_origin = e->origin();

	if (!math::world_to_screen(weapon_origin, weapon_position))
		return;

	if (e->GetClientClass()->m_ClassID != (int)ClassId::CBaseWeaponWorldModel && ((strstr(e->GetClientClass()->m_pNetworkName, "Weapon")) || e->GetClientClass()->m_ClassID == (int)ClassId::CDEagle || e->GetClientClass()->m_ClassID == (int)ClassId::CAK47))
	{

		char * icon = e->get_name();

		Color color = g_cfg.esp.dropped_weapons_color;
		auto w = render::get().text_width(fonts[SUBTABWEAPONS], icon);
		render::get().text(fonts[SUBTABWEAPONS], weapon_position.x, weapon_position.y, color, HFONT_CENTERED_X, icon);

		//render::get().rect_filled(weapon_position.x - 18, weapon_position.y + 22, 36, 3, Color(10, 10, 10, 180));
		//render::get().rect_filled(weapon_position.x - 18, weapon_position.y + 22, 15, 3, Color(g_cfg.esp.dropped_weapons_color));
		//render::get().rect(weapon_position.x - 18, weapon_position.y + 22, 36, 3, Color(10, 10, 10, 180));
	}

}