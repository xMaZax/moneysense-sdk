#include "other_esp.h"
#include "..\autowall\autowall.h"
#include "..\..\Hack\ragebot.h"
#include <chrono>
#include "..\..\Hack\resolver.h"

player_t* e;

struct ModColor
{
	float color[3];
	float alpha;

	void SetColorFloat(float r, float g, float b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}

	void SetColorFloat(float col[3])
	{
		color[0] = col[0];
		color[1] = col[1];
		color[2] = col[2];
	}

	void SetColor(Color col)
	{
		color[0] = col.r() / 255.f;
		color[1] = col.g() / 255.f;
		color[2] = col.b() / 255.f;
	}

	void GetColorFloat(float col[3])
	{
		col[0] = color[0];
		col[1] = color[1];
		col[2] = color[2];
	}

	Color GetColor()
	{
		return Color(color[0], color[1], color[2]);
	}

	ModColor(float r, float g, float b, float alph)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		alpha = alph;
	}

	ModColor(float col[3], float alph)
	{
		color[0] = col[0];
		color[1] = col[1];
		color[2] = col[2];
		alpha = alph;
	}
};

std::vector<impact_info> impacts;
std::vector<hitmarker_info> hitmarkers;
std::unordered_map<MaterialHandle_t, std::pair<ModColor, ModColor>> vguiMaterials;
std::unordered_map<MaterialHandle_t, std::pair<ModColor, ModColor>> worldMaterials;
float r, g, b, a;
bool sprop = false;

void otheresp::baim_indicator() {
	auto local_player = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	int screen_width, screen_height;
	g_csgo.m_engine()->GetScreenSize(screen_width, screen_height);

	static int u = 0;
	float speed = 1.5f;

	render::get().text(fonts[INDICATORFONT], 450 - 250, 15 + 25, Color(255, 255, 255), HFONT_OFF_SCREEN, "BAIM");
	render::get().rect_filled(450 - 250, 34 + 25, 40, 8, Color(0, 0, 0, 155));

	if (u <= 100) render::get().rect_filled(450 - 250, 34 + 25, 5, 8, Color(g_cfg.menu.menu_theme[0]));
	if (u > 100 && u <= 220)render::get().rect_filled(450 - 250, 34 + 25, 10, 8, Color(g_cfg.menu.menu_theme[0]));
	if (u > 200 && u <= 310)render::get().rect_filled(450 - 250, 34 + 25, 15, 8, Color(g_cfg.menu.menu_theme[0]));
	if (u > 300 && u <= 410)render::get().rect_filled(450 - 250, 34 + 25, 20, 8, Color(g_cfg.menu.menu_theme[0]));
	if (u > 420 && u <= 430)render::get().rect_filled(450 - 250, 34 + 25, 25, 8, Color(g_cfg.menu.menu_theme[0]));
	if (u > 440 && u <= 450)render::get().rect_filled(450 - 250, 34 + 25, 30, 8, Color(g_cfg.menu.menu_theme[0]));
	if (u > 460 && u <= 470)render::get().rect_filled(450 - 250, 34 + 25, 40, 8, Color(g_cfg.menu.menu_theme[0]));
	u += speed;
	if (u > 470) u = 100;

	render::get().rect(450 - 250, 34 + 25, 40, 8, Color(0, 0, 0, 255));

	INetChannelInfo* nci = g_csgo.m_engine()->GetNetChannelInfo();


}
void otheresp::desyncindicator() {
	auto local_player = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	int screen_width, screen_height;
	g_csgo.m_engine()->GetScreenSize(screen_width, screen_height);
	if (g_ctx.send_packet) {
		char test[64];
		sprintf_s(test, " desync: %.1f", g_ctx.m_globals.real_angles.y);
		render::get().text(fonts[INDICATORFONT], 5, screen_height / 2 + 62, Color{ 255, 255, 255, 255 }, HFONT_OFF_SCREEN, test);

	}
}

void otheresp::antiaim_indicator() {
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	static auto degrees_to_radians = [](float deg) -> float { return deg * (DirectX::XM_PI / 180.f); };

	auto rotated_position = [](Vector start, const float rotation, const float distance) -> Vector {
		const auto rad = degrees_to_radians(rotation);
		start.x += cosf(rad) * distance;
		start.y += sinf(rad) * distance;

		return start;
	};

	if (!g_ctx.available() || !local_player)
		return;

	static const auto real_color = Color(153, 51, 255, 255);
	//static const auto fake_color = Color(255, 0, 0, 255);
	//static const auto lby_color = Color(0, 0, 255, 255);

	if (!local_player->is_alive())
		return;

	if (g_cfg.esp.indicators[ANTIAIM].enabled) {
		auto client_viewangles = Vector();
		auto
			screen_width = render::get().viewport().right,
			screen_height = render::get().viewport().bottom;

		g_csgo.m_engine()->GetViewAngles(client_viewangles);

		constexpr auto radius = 80.f;

		const auto screen_center = Vector2D(screen_width / 2.f, screen_height / 2.f);
		const auto real_rot = degrees_to_radians(client_viewangles.y - g_ctx.m_globals.real_angles.y - 90);
		//const auto fake_rot = degrees_to_radians(client_viewangles.y - g_ctx.m_globals.fake_angles.y - 90);
		//const auto lby_rot = degrees_to_radians(client_viewangles.y - g_ctx.m_local->m_flLowerBodyYawTarget() - 90);

		auto draw_arrow = [&](float rot, Color color) -> void {
			auto pos_one = Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius);
			auto pos_two = Vector2D(screen_center.x + cosf(rot + degrees_to_radians(8)) * (radius - 13.f),
				screen_center.y + sinf(rot + degrees_to_radians(8)) * (radius - 13.f));
			auto pos_three = Vector2D(screen_center.x + cosf(rot - degrees_to_radians(8)) * (radius - 13.f),
				screen_center.y + sinf(rot - degrees_to_radians(8)) * (radius - 13.f));

			render::get().triangle(
				Vector2D(pos_one.x, pos_one.y),
				Vector2D(pos_two.x, pos_two.y),
				Vector2D(pos_three.x, pos_three.y),
				color);
		};

		draw_arrow(real_rot, real_color);
	}
}

const wchar_t* to_wchar(const char* string) {
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, string);
	_vsnprintf_s(buf, sizeof(buf), string, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	return wbuf;
}

void otheresp::spectators() {

	if (!g_cfg.esp.spectator_list)
		return;

	if (g_csgo.m_engine()->IsInGame() && g_csgo.m_engine()->IsConnected()) {
		int index = 0;
		int w, h;
		g_csgo.m_engine()->GetScreenSize(w, h);

		render::get().rect(931 + g_cfg.esp.spectator_list_x, 141 + g_cfg.esp.spectator_list_y, 200, 100, Color(0, 0, 0, 155));
		render::get().rect_filled(931 + g_cfg.esp.spectator_list_x, 141 + g_cfg.esp.spectator_list_y, 200, 100, Color(0, 0, 0, 65));
		render::get().rect_filled(937 + g_cfg.esp.spectator_list_x, 147 + g_cfg.esp.spectator_list_y, 188, 88, Color(0, 0, 0, 100));
		render::get().rect_filled(941 + g_cfg.esp.spectator_list_x, 172 + g_cfg.esp.spectator_list_y, 180, 57, Color(12, 12, 12, 255));
		render::get().rect(941 + g_cfg.esp.spectator_list_x, 172 + g_cfg.esp.spectator_list_y, 180, 57, Color(43, 43, 43, 155));
		render::get().rect_filled(941 + g_cfg.esp.spectator_list_x, 161 + g_cfg.esp.spectator_list_y, 180, 2, Color(g_cfg.menu.menu_theme[0]));
		render::get().rect_filled(941 + g_cfg.esp.spectator_list_x, 162 + g_cfg.esp.spectator_list_y, 180, 2, Color(g_cfg.menu.menu_theme[0]));
		render::get().text(fonts[CTAB], 999 + g_cfg.esp.spectator_list_x, 148 + g_cfg.esp.spectator_list_y, Color(255, 255, 255), HFONT_OFF_SCREEN, "spectators");

		for (int i = 1; i < 65; i++) {
			auto p_local = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
			auto p_entity = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(i));
			player_info_t e_info;

			if (p_entity && p_entity != p_local) {
				g_csgo.m_engine()->GetPlayerInfo(i, &e_info);
				if (!p_entity->is_alive() && !p_entity->IsDormant()) {
					auto target = p_entity->m_hObserverTarget();
					if (target) {
						auto p_target = g_csgo.m_entitylist()->GetClientEntityFromHandle(target);
						if (p_target == p_local) {
							int x, y;
							g_csgo.m_surface()->GetTextSize(fonts[ESPFLAG], to_wchar(e_info.szName), x, y);

							std::string player_name = e_info.szName;
							player_info_t p_info;
							g_csgo.m_engine()->GetPlayerInfo(i, &e_info);
							render::get().text(fonts[AIMWAREFONT], 946 + g_cfg.esp.spectator_list_x, 174 + g_cfg.esp.spectator_list_y + (10 * index), Color(255, 255, 255), HFONT_OFF_SCREEN, player_name.c_str());
							index++;
						}
					}
				}
			}
		}
	}
}

void otheresp::status_bar()
{
	if (g_cfg.misc.status_bar == 0)
	{

	}
	if (g_cfg.misc.status_bar == 1)
	{

		render::get().rect_filled(15, 190, 55, 15, Color(0, 0, 0, 155));

		if (g_cfg.ragebot.enable)
		{
			render::get().rect_filled(17, 192, 2, 11, Color(255, 139, 23));
		}

		render::get().rect_filled(15 + 57, 190, 55, 15, Color(0, 0, 0, 155));

		if (g_cfg.player.enable)
		{
			render::get().rect_filled(17 + 57, 192, 2, 11, Color(255, 139, 23));
		}

		render::get().rect_filled(15 + 114, 190, 55, 15, Color(0, 0, 0, 155));

		if (g_cfg.misc.bunnyhop)
		{
			render::get().rect_filled(17 + 114, 192, 2, 11, Color(255, 139, 23));
		}


		if (!g_cfg.ragebot.enable)
		{
			render::get().rect_filled(17, 192, 2, 11, Color(124, 124, 124));
		}
		if (!g_cfg.player.enable)
		{
			render::get().rect_filled(17 + 57, 192, 2, 11, Color(124, 124, 124));
		}
		if (!g_cfg.misc.bunnyhop)
		{
			render::get().rect_filled(17 + 114, 192, 2, 11, Color(124, 124, 124));
		}

		render::get().text(fonts[CTAB], 22, 190, Color(255, 255, 255), HFONT_OFF_SCREEN, "AIM");
		render::get().text(fonts[CTAB], 22 + 58, 190, Color(255, 255, 255), HFONT_OFF_SCREEN, "ESP");
		render::get().text(fonts[CTAB], 22 + 58 + 58, 190, Color(255, 255, 255), HFONT_OFF_SCREEN, "BHOP");

	}

	if (g_cfg.misc.status_bar == 2)
	{

		if (g_csgo.m_engine()->IsConnected())
		{
			int Width, Height;
			g_csgo.m_engine()->GetScreenSize(Width, Height);

			g_csgo.m_surface()->DrawSetColor(0, 0, 0, 155);
			g_csgo.m_surface()->rect_failed(Width - 25, 10, Width - 2, 28);
			g_csgo.m_surface()->rect_failed(Width - 50, 10, Width - 27, 28);
			g_csgo.m_surface()->rect_failed(Width - 75, 10, Width - 52, 28);

			if (g_cfg.player.enable)
			{
				g_csgo.m_surface()->DrawSetColor(255, 171, 0, 255);
				g_csgo.m_surface()->rect_failed(Width - 22, 13, Width - 20, 25);
			}

			if (g_cfg.misc.bunnyhop)
			{
				g_csgo.m_surface()->DrawSetColor(255, 171, 0, 255);
				g_csgo.m_surface()->rect_failed(Width - 47, 13, Width - 45, 25);
			}

			if (g_cfg.ragebot.enable)
			{
				g_csgo.m_surface()->DrawSetColor(255, 171, 0, 255);
				g_csgo.m_surface()->rect_failed(Width - 72, 13, Width - 70, 25);
			}


			if (!g_cfg.player.enable)
			{
				g_csgo.m_surface()->DrawSetColor(164, 164, 164, 255);
				g_csgo.m_surface()->rect_failed(Width - 22, 13, Width - 20, 25);
			}

			if (!g_cfg.misc.bunnyhop)
			{
				g_csgo.m_surface()->DrawSetColor(164, 164, 164, 255);
				g_csgo.m_surface()->rect_failed(Width - 47, 13, Width - 45, 25);
			}

			if (!g_cfg.ragebot.enable)
			{
				g_csgo.m_surface()->DrawSetColor(164, 164, 164, 255);
				g_csgo.m_surface()->rect_failed(Width - 72, 13, Width - 70, 25);
			}

			render::get().text(fonts[CTAB], Width - 16, 12, Color(255, 255, 255), HFONT_OFF_SCREEN, "V");
			render::get().text(fonts[CTAB], Width - 41, 12, Color(255, 255, 255), HFONT_OFF_SCREEN, "B");
			render::get().text(fonts[CTAB], Width - 66, 12, Color(255, 255, 255), HFONT_OFF_SCREEN, "A");
		}
	}
}
std::string GetTimeString()
{
	time_t current_time;
	struct tm* time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}

void otheresp::check_hits()
{
	if (!g_cfg.esp.check_hit)
		return;

	int index = 0;
	int w, h;
	g_csgo.m_engine()->GetScreenSize(w, h);

	for (int i = 1; i < 65; i++) {
		auto p_local = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
		auto p_entity = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(i));
		player_info_t e_info;

		if (p_entity && p_entity != p_local) {
			g_csgo.m_engine()->GetPlayerInfo(i, &e_info);
			if (!p_entity->is_alive() && !p_entity->IsDormant()) {
				auto target = p_entity->m_hObserverTarget();
				if (target) {
					auto p_target = g_csgo.m_entitylist()->GetClientEntityFromHandle(target);
					if (p_target == p_local) {
						int x, y;
						g_csgo.m_surface()->GetTextSize(fonts[ESPFLAG], to_wchar(e_info.szName), x, y);

						std::string player_name = e_info.szName;
						player_info_t p_info;
						g_csgo.m_engine()->GetPlayerInfo(i, &e_info);
						render::get().text(fonts[AIMWAREFONT], 29 + 45 - 6, 208 + (15 * index), Color(255, 255, 255), HFONT_OFF_SCREEN, player_name.c_str());
						index++;
					}
				}
			}
		}
	}

	render::get().rect_filled(27, 208, 232 + 35, 123, Color(0, 0, 0, 80));
	render::get().rect_filled(27, 208, 232 + 35, 14, Color(g_cfg.menu.menu_theme[0]));

	render::get().text(fonts[CTAB], 29, 208, Color(255, 255, 255), HFONT_OFF_SCREEN, "ID");
	render::get().text(fonts[CTAB], 29 + 45, 208, Color(255, 255, 255), HFONT_OFF_SCREEN, "PLAYER");
	render::get().text(fonts[CTAB], 29 + 125, 208, Color(255, 255, 255), HFONT_OFF_SCREEN, "DMG");
	render::get().text(fonts[CTAB], 29 + 165, 208, Color(255, 255, 255), HFONT_OFF_SCREEN, "CHANCE");
	render::get().text(fonts[CTAB], 29 + 230, 208, Color(255, 255, 255), HFONT_OFF_SCREEN, "PITCH");

	render::get().text(fonts[CTAB], 29, 208 + 15, Color(255, 255, 255), HFONT_OFF_SCREEN, "1");
	render::get().text(fonts[CTAB], 29, 208 + 15 + 15, Color(255, 255, 255), HFONT_OFF_SCREEN, "2");
	render::get().text(fonts[CTAB], 29, 208 + 15 + 15 + 15, Color(255, 255, 255), HFONT_OFF_SCREEN, "3");
	render::get().text(fonts[CTAB], 29, 208 + 15 + 15 + 15 + 15, Color(255, 255, 255), HFONT_OFF_SCREEN, "4");
	render::get().text(fonts[CTAB], 29, 208 + 15 + 15 + 15 + 15 + 15, Color(255, 255, 255), HFONT_OFF_SCREEN, "5");
	render::get().text(fonts[CTAB], 29, 208 + 15 + 15 + 15 + 15 + 15 + 15, Color(255, 255, 255), HFONT_OFF_SCREEN, "6");
	render::get().text(fonts[CTAB], 29, 208 + 15 + 15 + 15 + 15 + 15 + 15 + 15, Color(255, 255, 255), HFONT_OFF_SCREEN, "7");
}

std::string get_time()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);
	return buf;

}

void otheresp::watermark() {


	if (g_cfg.misc.watermark == 0)
	{

	}

	if (g_cfg.misc.watermark == 1)
	{
		int w_s, h_s;
		int x, y, w, h;
		int w0, h0;

		g_csgo.m_engine()->GetScreenSize(w_s, h_s);
		w = w_s / 2; h = h_s / 2;

		x = 5 + 5, y = 10, w = 197, h = 20;

		auto watermark = [](int x, int y, int w, int h, Color c) {
			render::get().line(x, y, x, y + h, c);
			render::get().line(x, y + h, x + w + 1, y + h, c);
			render::get().line(x + w, y, x + w, y + h, c);
			render::get().line(x, y, x + w, y, c);
		};

		static int u = 0;
		float watermarkScrollSpeed = 1.5f;

		render::get().rect_filled(x, y, w, h, Color(11, 11, 11));
		watermark(x - 0, y - 0, w + 0 * 2, h + 0 * 2, Color::Black);
		watermark(x - 1, y - 1, w + 1 * 2, h + 1 * 2, Color(53, 53, 53));
		watermark(x - 2, y - 2, w + 2 * 2, h + 2 * 2, Color(28, 28, 28));
		watermark(x - 3, y - 3, w + 3 * 2, h + 3 * 2, Color(28, 28, 28));
		watermark(x - 4, y - 4, w + 4 * 2, h + 4 * 2, Color(28, 28, 28));
		watermark(x - 5, y - 5, w + 5 * 2, h + 5 * 2, Color(53, 53, 53));
		watermark(x - 6, y - 6, w + 6 * 2, h + 6 * 2, Color::Black);

		render::get().text(fonts[CTAB], 7 + 5, 14, Color(255, 255, 255, 255), HFONT_OFF_SCREEN, "  money          | by carterrr | ");
		render::get().text(fonts[CTAB], 153, 14, Color(255, 255, 255, 255), HFONT_OFF_SCREEN, get_time().c_str());
		render::get().text(fonts[CTAB], 44 + 7, 14, Color(g_cfg.menu.menu_theme[0]), HFONT_OFF_SCREEN, "sense");
	}


	if (g_cfg.misc.watermark == 2)
	{

		int w, h;
		g_csgo.m_engine()->GetScreenSize(w, h);

		char username[UNLEN + 1];
		DWORD username_len = UNLEN + 1;
		GetUserName(username, &username_len);
		std::stringstream ss;

		ss << "moneysense | " << GetUserName << " | " __DATE__ << "";

		render::get().rect_filled(w - 162, 4, 161, 20, Color(155, 5, 2, 180));
		render::get().text(fonts[AIMWAREFONT], w - 167 + 8, 7, Color(255, 255, 255), HFONT_OFF_SCREEN, ss.str().c_str());
		//render::get().text(fonts[AIMWAREFONT], w - 187 + 92, 5, Color(255, 255, 255), HFONT_OFF_SCREEN, GetUserName);

	}
}

void otheresp::penetration_reticle() {
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!local_player->is_alive())
		return;

	auto draw_xhair = [](float size, Color color) -> void {
		int x_1, x_2, y_1, y_2;

		int
			screen_w = render::get().viewport().right / 2,
			screen_h = render::get().viewport().bottom / 2;

		x_1 = screen_w - (size / 4);
		x_2 = screen_w + (size / 4) + 1;

		y_1 = screen_h - (size / 4);
		y_2 = screen_h + (size / 4) + 1;

		render::get().rect_filled(screen_w - 1, screen_h - 1, 3, 3, color);
	};

	if (!g_cfg.esp.indicators[PENETRATION].enabled || !local_player->is_alive())
		return;

	auto weapon = local_player->m_hActiveWeapon().Get();
	if (!weapon)
		return;

	if (weapon->is_non_aim())
		return;

	auto weapon_info = weapon->get_csweapon_info();
	if (!weapon_info)
		return;

	vec3_t view_angles;
	g_csgo.m_engine()->GetViewAngles(view_angles);

	vec3_t direction;
	math::angle_vectors(view_angles, direction);

	vec3_t
		start = local_player->get_eye_pos(),
		end = start + (direction * 8192.f);
	int
		screen_ww = render::get().viewport().right / 2,
		screen_hh = render::get().viewport().bottom / 2;
	Color color = Color(255, 0, 0);
	if (autowall::get().calculate_return_info(start, end, local_player).m_did_penetrate_wall) {
		color = Color(163, 225, 21);
	}		//ender::get().text(fonts[INDICATORFONT], 5, screen_height - 450, culuryes, HFONT_OFF_SCREEN, "PING");
	//autowall::get().calculate_return_info(latency_based_eye_pos, left_eye_pos, g_ctx.m_local, e, 1).m_damage;
	draw_xhair(2, color);
}
int BreakLagCumpensation()
{
	Vector velocity = g_ctx.m_local->m_vecVelocity();
	velocity.z = 0;
	float speed = velocity.Length();
	if (speed > 0.f)
	{
		auto distance_per_tick = speed * g_csgo.m_globals()->m_interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
}
enum {
	IND_DRAWSTATUS,
	IND_LBY,
	IND_LC,
	IND_HEAD,
	IND_AAMODE,
	IND_NEGATIVEFLICK,
	IND_PING,
	IND_localinfo
};

player_t* m_entity;
void otheresp::lby_indicator() {
	auto local_player = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	auto weapon = local_player->m_hActiveWeapon().Get();
	int w = 0, h = 0;
	g_csgo.m_engine()->GetScreenSize(w, h);

	static auto draw_bar = [](int x, int y, float val, float max, const wchar_t* name) {
		if (val > max)
			val = max;


		render::get().draw_wide_string(true, fonts[INDICATORFONT], x + 125 / 2, y - 8, Color(255, 255, 255, 255), const_cast<wchar_t*>(name));
		render::get().rect_filled(x, y, 125, 15, Color(50, 50, 50, 75));
		render::get().rect_filled(x, y, 125 * (val / max), 15, Color(g_cfg.menu.menu_theme[0]));
		render::get().rect(12, 575, 38 * (val / max), 6, Color(0, 0, 0, 75));
	};

	static auto minimal_bar = [](int x, int y, float val, float max, const wchar_t* name) {
		if (val > max)
			val = max;

		render::get().draw_wide_string(true, fonts[INDICATORFONT], 20, 555, Color(255, 255, 255, 255), const_cast<wchar_t*>(name));	
		render::get().rect_filled(12, 575, 38 * (val / max), 6, Color(0, 0, 0, 75));
		render::get().rect_filled(12, 575, 38 * (val / max), 6, Color(0, 255, 0, 155));
	};
	static auto last_fake_origin = Vector();
	static auto last_origin = Vector();

	if (!g_csgo.m_clientstate()->m_nChokedCommands) {
		last_fake_origin = last_origin;
		last_origin = local_player->m_vecOrigin();
	}
	auto nci = g_csgo.m_engine()->GetNetChannelInfo();

	if (!nci)
		return;
	if (local_player->is_valid_player() && local_player->is_alive()) {
		if (g_cfg.esp.indicators[CROCKED].enabled)
			draw_bar(10, h / 2 - 100, g_csgo.m_clientstate()->m_nChokedCommands, 15.0f, L"CHOKE");

		if (g_cfg.esp.indicators[EXTEND].enabled)
			draw_bar(10, h / 2, 1000.0f, 400.0f, L"EXTEND");

		if (g_cfg.esp.indicators[LC].enabled) {
			draw_bar(10, h / 2 + 100, last_fake_origin.DistToSqr(last_origin), 15.0f, L"LC");
		}

		if (g_cfg.esp.indicators[DOUBLETAP].enabled && g_cfg.ragebot.doubletap_enable)
		{
			render::get().text(fonts[INDICATORFONT], 20, 555, Color(255, 255, 255), HFONT_OFF_SCREEN, "DT");
			render::get().rect_filled(12, 575, 38, 6, Color(0, 0, 0, 75));

			static int u = 0;
			float speed = 1.5f;
			if (u <= 100) render::get().rect_filled(12, 575, 25, 6, Color(0, 255, 0, 155));
			if (u > 100 && u <= 220)render::get().rect_filled(12, 575, 12, 6, Color(0, 255, 0, 155));
			if (u > 200 && u <= 310)render::get().rect_filled(12, 575, 15, 6, Color(0, 255, 0, 155));
			if (u > 300 && u <= 410)render::get().rect_filled(12, 575, 21, 6, Color(0, 255, 0, 155));
			if (u > 420 && u <= 430)render::get().rect_filled(12, 575, 24, 6, Color(0, 255, 0, 155));
			if (u > 440 && u <= 450)render::get().rect_filled(12, 575, 31, 6, Color(0, 255, 0, 155));
			if (u > 460 && u <= 470)render::get().rect_filled(12, 575, 38, 6, Color(0, 255, 0, 155));
			u += speed;
			if (u > 470) u = 100;
			render::get().rect(12, 575, 38, 6, Color(0, 0, 0, 155));
			//minimal_bar(20, 555, (weapon->is_weapon()) * 1000.0f, 45.0f, L"DT");
		}
	}
	else if (local_player && g_csgo.m_engine()->IsInGame() && g_csgo.m_engine()->IsConnected()) {
		if (g_cfg.esp.indicators[CROCKED].enabled)
			draw_bar(15, h / 2 - 150, 0.0f, 1.0f, L"CHOKE");

		if (g_cfg.esp.indicators[EXTEND].enabled)
			draw_bar(15, h / 2, 0.0f, 2.0f, L"EXTEND");

		if (g_cfg.esp.indicators[LC].enabled)
			draw_bar(15, h / 2 + 150, 0.0f, 2.0f, L"LC");

		if (g_cfg.esp.indicators[DOUBLETAP].enabled && g_cfg.ragebot.doubletap_enable)
		{
			render::get().text(fonts[INDICATORFONT], 20, 555, Color(255, 255, 255), HFONT_OFF_SCREEN, "DT");
			render::get().rect_filled(12, 575, 38, 6, Color(0, 0, 0, 75));

			static int u = 0;
			float speed = 1.5f;
			if (u <= 100) render::get().rect_filled(12, 575, 25, 6, Color(0, 255, 0, 155));
			if (u > 100 && u <= 220)render::get().rect_filled(12, 575, 12, 6, Color(0, 255, 0, 155));
			if (u > 200 && u <= 310)render::get().rect_filled(12, 575, 15, 6, Color(0, 255, 0, 155));
			if (u > 300 && u <= 410)render::get().rect_filled(12, 575, 21, 6, Color(0, 255, 0, 155));
			if (u > 420 && u <= 430)render::get().rect_filled(12, 575, 24, 6, Color(0, 255, 0, 155));
			if (u > 440 && u <= 450)render::get().rect_filled(12, 575, 31, 6, Color(0, 255, 0, 155));
			if (u > 460 && u <= 470)render::get().rect_filled(12, 575, 38, 6, Color(0, 255, 0, 155));
			u += speed;
			if (u > 470) u = 100;
			render::get().rect(12, 575, 38, 6, Color(0, 0, 0, 155));
			//minimal_bar(20, 555, (weapon->is_weapon()) * 1000.0f, 45.0f, L"DT");
		}
	}

}

player_t* get_player(int userid) {
	int i = g_csgo.m_engine()->GetPlayerForUserID(userid);
	return static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
}

void otheresp::hitmarker_event(IGameEvent * event) {
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!strcmp(event->GetName(), "player_hurt")) {
		auto attacker = event->GetInt("attacker");
		auto victim = get_player(event->GetInt("userid"));

		if (g_csgo.m_engine()->GetPlayerForUserID(attacker) == g_csgo.m_engine()->GetLocalPlayer())
		{
			hurt_time = g_csgo.m_globals()->m_curtime;
			impact_info best_impact;
			float best_impact_distance = -1;
			float time = g_csgo.m_globals()->m_curtime;

			for (int i = 0; i < impacts.size(); i++) {
				auto iter = impacts[i];
				if (time > iter.time + 1.f) {
					impacts.erase(impacts.begin() + i);
					continue;
				}
				vec3_t position = vec3_t(iter.x, iter.y, iter.z);
				vec3_t enemy_pos = victim->m_vecOrigin();
				float distance = position.DistTo(enemy_pos);
				if (distance < best_impact_distance || best_impact_distance == -1) {
					best_impact_distance = distance;
					best_impact = iter;
				}
			}

			if (best_impact_distance == -1)
				return;

			hitmarker_info info;
			info.impact = best_impact;
			info.alpha = 255;
			hitmarkers.push_back(info);
		}
	}

	if (strstr(event->GetName(), "bullet_impact"))
	{
		auto entity = get_player(event->GetInt("userid"));
		if (!entity || entity != local_player)
			return;

		impact_info info;
		info.x = event->GetFloat("x");
		info.y = event->GetFloat("y");
		info.z = event->GetFloat("z");

		info.time = g_csgo.m_globals()->m_curtime;

		impacts.push_back(info);
	}
}

void otheresp::hitmarker_paint() {
	auto linesize = 8;
	int sizeX, centerX;
	int sizeY, centerY;

	if (!g_ctx.m_local->is_alive())
		hurt_time = 0.f;
	if (!g_csgo.m_engine()->IsInGame() || !g_csgo.m_engine()->IsConnected())
		hurt_time = 0.f;

	if (hurt_time + 0.65f >= g_csgo.m_globals()->m_curtime) {
		g_csgo.m_engine()->GetScreenSize(sizeX, sizeY);
		linesize -= static_cast<int>(static_cast<float>(((hurt_time + 0.65f) - g_csgo.m_globals()->m_curtime) / 1.2) * 45);

		centerX = sizeX / 2;
		centerY = sizeY / 2;

		util::clamp_value(linesize, 0, 8);

		g_csgo.m_surface()->DrawSetColor(255, 255, 255, 255);
		g_csgo.m_surface()->DrawLine(centerX - linesize, centerY - linesize, centerX - (linesize / 4), centerY - (linesize / 4));
		g_csgo.m_surface()->DrawLine(centerX - linesize, centerY + linesize, centerX - (linesize / 4), centerY + (linesize / 4));
		g_csgo.m_surface()->DrawLine(centerX + linesize, centerY + linesize, centerX + (linesize / 4), centerY + (linesize / 4));
		g_csgo.m_surface()->DrawLine(centerX + linesize, centerY - linesize, centerX + (linesize / 4), centerY - (linesize / 4));
	}
}

void otheresp::hitmarkerdynamic_paint() {
	float time = g_csgo.m_globals()->m_curtime;

	for (int i = 0; i < hitmarkers.size(); i++) {
		bool expired = time >= hitmarkers.at(i).impact.time + 2.f;

		if (expired)
			hitmarkers.at(i).alpha -= 1;

		if (expired && hitmarkers.at(i).alpha <= 0) {
			hitmarkers.erase(hitmarkers.begin() + i);
			continue;
		}

		Vector pos3D = Vector(hitmarkers.at(i).impact.x, hitmarkers.at(i).impact.y, hitmarkers.at(i).impact.z), pos2D;
		if (!math::world_to_screen(pos3D, pos2D))
			continue;

		auto linesize = 8;

		g_csgo.m_surface()->DrawSetColor(255, 255, 255, hitmarkers.at(i).alpha);
		g_csgo.m_surface()->DrawLine(pos2D.x - linesize, pos2D.y - linesize, pos2D.x - (linesize / 4), pos2D.y - (linesize / 4));
		g_csgo.m_surface()->DrawLine(pos2D.x - linesize, pos2D.y + linesize, pos2D.x - (linesize / 4), pos2D.y + (linesize / 4));
		g_csgo.m_surface()->DrawLine(pos2D.x + linesize, pos2D.y + linesize, pos2D.x + (linesize / 4), pos2D.y + (linesize / 4));
		g_csgo.m_surface()->DrawLine(pos2D.x + linesize, pos2D.y - linesize, pos2D.x + (linesize / 4), pos2D.y - (linesize / 4));
	}
}

void otheresp::spread_crosshair(IDirect3DDevice9* m_device) {
	if (!g_cfg.esp.show_spread)
		return;

	if (!g_ctx.m_local)
		return;

	if (!g_ctx.m_local->is_alive())
		return;

	if (!g_csgo.m_engine()->IsConnected() && !g_csgo.m_engine()->IsInGame())
		return;

	auto weapon = g_ctx.m_local->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	static float rot = 0.f;

	int w, h;

	g_csgo.m_engine()->GetScreenSize(w, h);

	w /= 2, h /= 2;

	int r, g, b;
	r = g_cfg.esp.show_spread_color.r();
	g = g_cfg.esp.show_spread_color.g();
	b = g_cfg.esp.show_spread_color.b();

	weapon->update_accuracy_penality();

	switch (g_cfg.esp.show_spread_type) {
	case 0:
		render::get().circle_dual_colour(w, h, weapon->get_innacuracy() * 500.0f, 0, 1, 50, D3DCOLOR_RGBA(r, g, b, 100), D3DCOLOR_RGBA(0, 0, 0, 0), m_device);
		break;
	case 1:
		render::get().circle_dual_colour(w, h, weapon->get_innacuracy() * 500.0f, rot, 1, 50, m_device);
		break;
	}

	rot += 0.5f;
	if (rot > 360.f)
		rot = 0.f;
}

void otheresp::spread_crosshair() {
	if (g_cfg.esp.show_spread_type != 2)
		return;

	if (!g_ctx.m_local)
		return;

	if (!g_ctx.m_local->is_alive())
		return;

	if (!g_csgo.m_engine()->IsConnected() && !g_csgo.m_engine()->IsInGame())
		return;

	auto weapon = g_ctx.m_local->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	if (weapon) {
		int screen_w, screen_h;
		g_csgo.m_engine()->GetScreenSize(screen_w, screen_h);
		int cross_x = screen_w / 2, cross_y = screen_h / 2;

		float recoil_step = screen_h / 180;

		cross_x -= (int)(g_ctx.m_local->m_aimPunchAngle().y * recoil_step);
		cross_y += (int)(g_ctx.m_local->m_aimPunchAngle().x * recoil_step);

		weapon->update_accuracy_penality();
		float inaccuracy = weapon->get_innacuracy();
		float spread = weapon->get_spread();

		float cone = inaccuracy * spread;
		cone *= screen_h * 0.7f;
		cone *= 90.f / 180;

		for (int seed{ }; seed < 512; ++seed) {
			math::random_seed(math::random_float(0, 512));

			float	rand_a = math::random_float(0.f, 1.f);
			float	pi_rand_a = math::random_float(0.f, 2.f * DirectX::XM_PI);
			float	rand_b = math::random_float(0.0f, 1.f);
			float	pi_rand_b = math::random_float(0.f, 2.f * DirectX::XM_PI);

			float spread_x = cos(pi_rand_a) * (rand_a * inaccuracy) + cos(pi_rand_b) * (rand_b * spread);
			float spread_y = sin(pi_rand_a) * (rand_a * inaccuracy) + sin(pi_rand_b) * (rand_b * spread);

			float max_x = cos(pi_rand_a) * cone + cos(pi_rand_b) * cone;
			float max_y = sin(pi_rand_a) * cone + sin(pi_rand_b) * cone;

			float step = screen_h / 180 * 90.f;
			int screen_spread_x = (int)(spread_x * step * 0.7f);
			int screen_spread_y = (int)(spread_y * step * 0.7f);

			float percentage = (rand_a * inaccuracy + rand_b * spread) / (inaccuracy + spread);

			render::get().rect(cross_x + screen_spread_x, cross_y + screen_spread_y, 1, 1,
				g_cfg.esp.show_spread_color);
		}
	}
}