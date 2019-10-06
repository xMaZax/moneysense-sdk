#include "player_esp.h"
#include "..\lagcompensation\lagcompensation.h"
//#include "..\ragebot\resolver_recode.h"
#include "..\..\Hack\ragebot.h"
//#include "..\..\AI-Resolver.h"
#include "..\..\Hack\resolver.h"
#include "../../utils/csgo.hpp" 
#include "..\..\sdk\misc\BeamInfo_t.hpp"
#include "bullet_tracers.h"
#include "../../sdk/structs.hpp"
#include "../../sdk/misc/Studio.hpp"
#include "../../std2017.h"
void playeresp::paint_traverse() {

	if (!g_ctx.available() || !g_cfg.player.enable)
		return;


	auto local_player = reinterpret_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	for (int i = 1; i < g_csgo.m_entitylist()->GetHighestEntityIndex(); i++) {
		auto e = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!e)
			continue;

		if (e->IsDormant())
			continue;

		Box box; if (!util::get_bbox_world(e, box))
			continue;

		if (g_cfg.esp.anti_aim_lines)
			antiaim_lines(e);
	}

	for (int i = 1; i < g_csgo.m_globals()->m_maxclients; i++) {
		auto e = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!e->valid(true, !g_cfg.player.enable))
			continue;

		/*e->m_bSpotted() = true;*/  //crashing //are you slow?, its the offset, off=offyaself
		if (g_cfg.player.enable)
		{
			int idx = e->EntIndex();
			float in = (1.f / 0.2f) * g_csgo.m_globals()->m_frametime;
			float out = (1.f / 2.f) * g_csgo.m_globals()->m_frametime;

			if (!e->IsDormant()) {
				if (esp_alpha_fade[idx] < 1.f)
					esp_alpha_fade[idx] += in;
			}
			else {
				if (esp_alpha_fade[idx] > 0.f)
					esp_alpha_fade[idx] -= out;
			}
			esp_alpha_fade[idx] = (esp_alpha_fade[idx] > 1.f ? 1.f : esp_alpha_fade[idx] < 0.f ? 0.f : esp_alpha_fade[idx]);
		}

		Box box; if (util::get_bbox(e, box)) {
			draw_box(e, box);
			draw_name(e, box);
			draw_health(e, box);
			draw_ammobar(e, box);
			draw_flashbar(e, box);
			draw_flags(e, box);
			draw_weapon(e, box);
			draw_weapon_name(e, box);
			draw_lbybar(e, box);

			if (g_cfg.player.skeleton[SKELETON_NORMAL].enabled) {
				Color color = e->IsDormant() ? Color(130, 130, 130, 130) : Color::White;
				color.SetAlpha(g_cfg.player.alpha  * esp_alpha_fade[e->EntIndex()]);

				matrix3x4_t matrix[128];

				if (e->SetupBones(matrix, 128, 0x100, 0.f))
					draw_skeleton(e, color, matrix);
			}
		}
	}
}

void playeresp::draw_box(player_t* m_entity, Box box) {
	if (!g_cfg.player.box)
		return;

	int
		alpha = g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()],
		outline_alpha = alpha * 0.6f;

	Color
		outline_color{ 0, 0, 0, outline_alpha },
		color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : g_cfg.player.box_color;

	color.SetAlpha(alpha);

	render::get().rect(box.x - 1, box.y - 1, box.w + 2, box.h + 2, outline_color);
	render::get().rect(box.x, box.y, box.w, box.h, color);
	render::get().rect(box.x + 1, box.y + 1, box.w - 2, box.h - 2, outline_color);

}

void playeresp::beams(vec3_t src, Color color) {
	vec3_t end;
	BeamInfo_t info;
	info.m_nType = TE_BEAMPOINTS;
	info.m_pszModelName = "sprites/physbeam.vmt";
	info.m_nModelIndex = -1;
	info.m_flHaloScale = 0.0f;
	info.m_flLife = g_cfg.esp.bullet_tracer_duration; //duration of tracers
	info.m_flWidth = g_cfg.esp.bullet_tracer_size; //start width
	info.m_flEndWidth = g_cfg.esp.bullet_tracer_size; //end width
	info.m_flFadeLength = 0.0f;
	info.m_flAmplitude = 2.0f;
	info.m_flBrightness = 255.f;
	info.m_flSpeed = 0.2f;
	info.m_nStartFrame = 0;
	info.m_flFrameRate = 0.f;
	info.m_flRed = color.r();
	info.m_flGreen = color.g();
	info.m_flBlue = color.b();
	info.m_nSegments = 2;
	info.m_bRenderable = true;
	info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;
	info.m_vecStart = src;
	info.m_vecEnd = end;
	Beam_t* beam = g_csgo.m_viewrenderbeams()->CreateBeamPoints(info);

	if (beam)
		g_csgo.m_viewrenderbeams()->DrawBeam(beam);
}

void playeresp::draw_health(player_t* m_entity, Box box) {

	int alpha = g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()];

	if (!g_cfg.player.health)
		return;

	int player_health = m_entity->m_iHealth() > 100 ? 100 : m_entity->m_iHealth();
	if (player_health) {

		int color[3] = { 0, 0, 0 };
		color[0] = 83; color[1] = 250; color[2] = 90;

		render::get().rect_filled(box.x - 7, box.y - 1, 4, box.h + 2, Color(21, 21, 21, alpha));
		int health_height = player_health * box.h / 100;
		int add_space = box.h - health_height;
		Color hec = Color(color[0], color[1], color[2], alpha);

		for (int i = 0; i < 10; i++)
		render::get().rect(box.x - 6, box.y, 2, box.h, Color(34, 34, 34, alpha));
		render::get().rect_filled(box.x - 6, box.y + add_space, 2, health_height, Color(51, 132, 255, alpha));
		render::get().rect(box.x - 6, box.y + add_space, 2, health_height, hec);

	}
}

void playeresp::antiaim_lines(player_t* enitiy)
{
	if (!g_cfg.esp.anti_aim_lines)
		return;

	auto local_player = static_cast<player_t*>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	static auto degrees_to_radians = [](float deg) -> float { return deg * (DirectX::XM_PI / 180.f); };
	auto rotated_position = [](Vector start, const float rotation, const float distance) -> Vector {
		const auto rad = degrees_to_radians(rotation);
		start.x += cosf(rad) * distance;
		start.y += sinf(rad) * distance;

		return start;
	};

	if (!g_ctx.available() || !local_player)
		return;

	static const auto real_color = Color(128, 255, 0, 255);
	static const auto fake_color = Color(245, 92, 108, 255);

	if (!local_player->is_alive())
		return;

		if (g_csgo.m_input()->m_fCameraInThirdPerson)
		{
			constexpr auto distance = 50.f;
			const auto origin = local_player->GetAbsOrigin();
			Vector screen1, screen2;
			if (!g_csgo.m_debugoverlay()->ScreenPosition(origin, screen1))
			{
				if (!g_csgo.m_debugoverlay()->ScreenPosition(rotated_position(origin, g_ctx.m_globals.real_angles.y, distance), screen2)) {
					render::get().line(screen1.x, screen1.y, screen2.x, screen2.y, real_color);
					render::get().text(fonts[CTAB], screen2.x, screen2.y, Color(real_color), HFONT_CENTERED_NONE, "real");
				}
				if (!g_csgo.m_debugoverlay()->ScreenPosition(rotated_position(origin, g_ctx.m_local->m_flLowerBodyYawTarget(), distance), screen2)) {
					render::get().line(screen1.x, screen1.y, screen2.x, screen2.y, fake_color);
					render::get().text(fonts[CTAB], screen2.x, screen2.y, Color(fake_color), HFONT_CENTERED_NONE, "fake");
				}
			}
		}
		else
		{
			auto client_viewangles = Vector();
			auto
				screen_width = render::get().viewport().right,
				screen_height = render::get().viewport().bottom;

			g_csgo.m_engine()->GetViewAngles(client_viewangles);

			constexpr auto radius = 80.f;

			const auto screen_center = Vector2D(screen_width / 2.f, screen_height / 2.f);
			const auto real_rot = degrees_to_radians(client_viewangles.y - g_ctx.m_globals.real_angles.y - 90);

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

void playeresp::draw_flashbar(player_t* m_entity, Box box) {
	
	if (!g_cfg.player.flash_bar)
		return;

    int alpha = g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()];
	float flash_alpha = m_entity->m_flFlashDuration();
	if (flash_alpha < 1.f)
		return;

	float width = box.w;

	Box n_box = {
		box.x + 1,
		box.y + box.h + 3,
		box.w - 1,
		2
	};

	render::get().text(fonts[CTAB], box.x + (box.w / 2), box.y - 30, Color(245, 92, 108, alpha), HFONT_CENTERED_X, "Flashed");
}

void playeresp::draw_ammobar(player_t* m_entity, Box box) {

	if (g_cfg.player.ammo != 1)
		return;

	auto weapon = m_entity->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	if (weapon->is_non_aim())
		return;

	int ammo = weapon->m_iClip1();

	int
		alpha = g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()],
		outline_alpha = alpha * 1.0f,
		inner_back_alpha = alpha * 1.0f;

	Color
		outline_color{ 0, 0, 0, outline_alpha },
		inner_back_color{ 0, 0, 0, inner_back_alpha },
		text_color = m_entity->IsDormant() ? Color(130, 130, 130, alpha) : Color(255, 255, 255, alpha),
		color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : g_cfg.player.ammobar_color;

	color.SetAlpha(alpha);

	Box n_box = {
		box.x + 1,
		box.y + box.h + 3,
		box.w - 1,
		2
	};

	float
		bar_width = ammo * box.w / weapon->get_csweapon_info()->max_clip;

	AnimationLayer animlayer = m_entity->get_animlayers()[1];

	if (animlayer.m_pOwner) {
		auto activity = m_entity->sequence_activity(animlayer.m_nSequence);

		bool reloading = (activity == 967 && animlayer.m_flWeight != 0.0f);

		if (reloading && animlayer.m_flCycle < 0.99)
			bar_width = (animlayer.m_flCycle * box.w) / 1.f;
	}

	render::get().rect_filled(n_box.x - 1, n_box.y - 1, box.w + 2, 4, inner_back_color);
	render::get().rect_filled(n_box.x, n_box.y, bar_width, 2, color);

	if (weapon->m_iClip1() != weapon->get_csweapon_info()->max_clip)
		render::get().text(fonts[AIMWAREFONT],
			n_box.x + bar_width, n_box.y + 1,
			text_color, HFONT_CENTERED_X | HFONT_CENTERED_Y, std::to_string(ammo).c_str()
		);

	if (g_cfg.player.armorbar)
	{
		int alpha = g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()];
		int player_ar = m_entity->armor() > 100 ? 100 : m_entity->armor();
		if (player_ar)
		{
			if (player_ar > 100) {
				player_ar = 100;
			}
			int color[3] = { 0, 120, 250 };

			int health_height = player_ar * box.h / 100;
			int add_space = box.h - health_height;
			Color hec = Color(color[0], color[1], color[2], alpha);
			render::get().rect(box.x - 11, box.y, 2, box.h, Color(21, 21, 21, alpha));
			render::get().rect(box.x - 11, box.y, 2, box.h, Color(3, 3, 3, alpha));
			render::get().rect(box.x - 11, box.y + add_space, 2, health_height, hec);
		}
	}
}

void playeresp::draw_name(player_t* m_entity, Box box) {
	static auto sanitize = [](char * name) -> std::string {
		name[127] = '\0';

		std::string tmp(name);

		if (tmp.length() > 20) {
			tmp.erase(20, (tmp.length() - 20));
			tmp.append("...");
		}

		return tmp;
	};

	if (!g_cfg.player.name)
		return;

	player_info_t player_info;
	if (g_csgo.m_engine()->GetPlayerInfo(m_entity->EntIndex(), &player_info)) {
		std::string name = sanitize(player_info.szName);

		auto color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : g_cfg.player.name_color;
		color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()]);

		std::transform(name.begin(), name.end(), name.begin(), tolower);

		render::get().text(fonts[CTAB], box.x + (box.w / 2), box.y - 13, color, HFONT_CENTERED_X, name.c_str());
	}
}

void playeresp::draw_weapon(player_t* m_entity, Box box) {
	player_info_t player_info;
	g_csgo.m_engine()->GetPlayerInfo(m_entity->EntIndex(), &player_info);

	if (!g_cfg.player.weapon != 0)
		return;

	int bottom_pos = 0;

	auto weapon = m_entity->m_hActiveWeapon().Get();

	int _y;

	if (!weapon)
		return;

	bool drawing_ammo_bar = g_cfg.player.ammo == 1;
	bool drawing_lby_bar = g_cfg.player.lby;

	if (weapon->is_non_aim() || !g_cfg.player.lby || player_info.fakeplayer || !g_cfg.ragebot.antiaim_correction)
		drawing_lby_bar = false;

	if (!drawing_lby_bar)
		_y = box.y + box.h + (drawing_ammo_bar ? 10 : 6);
	else
		_y = box.y + box.h + (drawing_ammo_bar ? 10 : 6);

	if (weapon->is_non_aim())
		drawing_ammo_bar = false;

	auto color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : g_cfg.player.weapon_color;
	color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()]);

	//render::get().text(fonts[SUBTABWEAPONS], box.x + (box.w / 2), box.y + box.h, color, HFONT_CENTERED_X, weapon->get_name());
	render::get().text(fonts[SUBTABWEAPONS], box.x + (box.w / 2), _y + 5, color, HFONT_CENTERED_X, weapon->get_name());
}

void playeresp::draw_weapon_name(player_t* m_entity, Box box) {
	player_info_t player_info;
	g_csgo.m_engine()->GetPlayerInfo(m_entity->EntIndex(), &player_info);

	if (!g_cfg.player.weapon != 1)
		return;

	int bottom_pos = 0;

	auto weapon = m_entity->m_hActiveWeapon().Get();

	int _y;

	if (!weapon)
		return;

	bool drawing_ammo_bar = g_cfg.player.ammo == 1;
	bool drawing_lby_bar = g_cfg.player.lby;

	if (weapon->is_non_aim() || !g_cfg.player.lby || player_info.fakeplayer || !g_cfg.ragebot.antiaim_correction)
		drawing_lby_bar = false;

	if (!drawing_lby_bar)
		_y = box.y + box.h + (drawing_ammo_bar ? 10 : 6);
	else
		_y = box.y + box.h + (drawing_ammo_bar ? 10 : 6);

	if (weapon->is_non_aim())
		drawing_ammo_bar = false;

	auto color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : g_cfg.player.weapon_color;
	color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()]);

	render::get().text(fonts[AIMWAREFONT], box.x + (box.w / 2), _y, color, HFONT_CENTERED_X, weapon->get_weapons_name()); //name weapon
}

void playeresp::draw_flags(player_t * e, Box box) {
	int _x = box.x + box.w + 1, _y = box.y;
	player_info_t player_info;
	g_csgo.m_engine()->GetPlayerInfo(e->EntIndex(), &player_info);

	auto weapon = e->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	auto fakeDuck = [&]() -> bool
	{
		static float storedTick;
		static float crouchedTicks[65];

		if (!e->AnimationState())
			return false;

		float m_flDuckAmount = e->AnimationState()->m_fDuckAmount;
		float m_flDuckSpeed = e->m_bDucking();
		float m_fFlags = e->m_fFlags();

		if (m_flDuckSpeed != 0.f && m_flDuckAmount != 0.f)
		{
			if (m_flDuckSpeed == 8.f && m_flDuckAmount <= 0.9f && m_flDuckAmount > 0.01f)
			{
				if (storedTick != g_csgo.m_globals()->m_tickcount)
				{
					crouchedTicks[e->EntIndex()] = crouchedTicks[e->EntIndex()] + 1;
					storedTick = g_csgo.m_globals()->m_tickcount;
				}
				return (crouchedTicks[e->EntIndex()] >= 5);
			}
			else
				crouchedTicks[e->EntIndex()] = 0;
		}

		return false;
	};

	auto draw_flag = [&](Color color, const char * text, ...) -> void {
		render::get().text(fonts[AIMWAREFONT], _x, _y, color, HFONT_CENTERED_NONE, text);

		_y += 9;
	};

	auto icondraw_flag = [&](Color color, const char* text, ...) -> void {
		render::get().text(fonts[WEAPONICONS], _x, _y, color, HFONT_CENTERED_NONE, text);

		_y += 9;
	};

	auto color = Color(255, 255, 255);
	color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);

	if (g_cfg.player.flags[FLAGS_ARMOR].enabled) {
		bool
			kevlar = e->m_ArmorValue() > 0,
			helmet = e->m_bHasHelmet();

		auto color = Color::White;
		color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);

		if (helmet || kevlar)
			icondraw_flag(color, "q");
	}

	if (player_info.fakeplayer)
		draw_flag(color, "bot");

	if (g_cfg.player.flags[FLAGS_RESOLVERMODE].enabled) {
		auto colorr = Color(255, 100, 75);
		colorr.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);
		auto colorw = Color(255, 225, 225);
		colorw.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);
		auto colorg = Color(25, 225, 25);
		colorg.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);

		if (g_cfg.ragebot.resolver_experimental) {
			if (e->IsDormant())
				return;
			const int resolve_type = ResolverOvermake::resolver.GetPlayerResolveInfo(e).resolve_type;
			draw_flag(ResolverOvermake::resolver.GetResolveColor(resolve_type), ResolverOvermake::resolver.TranslateResolveRecord(resolve_type).c_str());
			if (ResolverOvermake::resolver.GetPlayerResolveInfo(e).is_balance_adjust_triggered)
				draw_flag(Color::Green, "LBY");
		}
	}

	if (g_cfg.player.flags[FLAGS_SCOPED].enabled) {
		auto color = Color(255, 222, 0);
		color.SetAlpha(g_cfg.player.alpha  * esp_alpha_fade[e->EntIndex()]);

		if (e->m_bIsScoped())
			draw_flag(color, "scope");
	}

	if (g_cfg.player.flags[FLAGS_DEBUG].enabled) {
		auto color = Color(255, 150, 220);
		color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);

		if (fakeDuck() && e->m_vecVelocity().Length2D() < 24.f && e->m_fFlags() & FL_ONGROUND)
			draw_flag(color, "fd");
	}

	if (g_cfg.player.flags[FLAGS_BALANCE].enabled) {

		auto color = Color(96, 230, 19);
		color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);

		int get_money = e->m_iAccount();

		draw_flag(color, "$", std::to_string(get_money).c_str());
	}

	if (g_cfg.player.flags[FLAGS_RELOADING].enabled) {

		auto color = Color(96, 230, 19);
		color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);

		player_t* m_entity = nullptr;

		AnimationLayer animlayer = m_entity->get_animlayers()[1];

		if (animlayer.m_pOwner) {
			auto activity = m_entity->sequence_activity(animlayer.m_nSequence);

			bool reloading = (activity == 967 && animlayer.m_flWeight != 0.0f);

			draw_flag(color, "reloading", reloading);
		}
	}

	if (g_cfg.player.flags[FLAGS_VULNERABLE].enabled) {
		if (g_cfg.ragebot.resolver_experimental) {
			const int resolve_type = ResolverOvermake::resolver.GetPlayerResolveInfo(e).resolve_type;
			if (ResolverOvermake::resolver.IsResolved(resolve_type))
				render::get().text(fonts[AIMWAREFONT], box.x + (box.w / 2), box.y - 22, Color(255, 151, 16), HFONT_CENTERED_X, "::bruteforce::");
		}
	}
	if (g_cfg.player.flags[FLAGS_BREAKINGLC].enabled) {
		bool trueex = true;
		if (!e->valid(trueex, trueex))
			return;
		static Vector originrecords[2];
		bool breaking = false;
		Vector curpos = e->m_vecVelocity();
		static bool flip = false;
		originrecords[flip ? 0 : 1] = curpos;
		flip = !flip;

		if (e->abs_origin().LengthSqr() > 4096)
		{
			breaking = true;
		}
		else if ((originrecords[0] - originrecords[1]).LengthSqr() > 4096) {
			breaking = true;
		}
		else
		{
			breaking = false;
		}
	}

	if (g_cfg.player.flags[FLAGS_LBY].enabled) {
		auto & player = lagcompensation::get().players[e->EntIndex()];

		auto color = Color(255, 110, 199);
		color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);

		if (!player_info.fakeplayer && g_cfg.ragebot.antiaim_correction && g_cfg.player.lby) {
			wchar_t buf[80];
			swprintf(buf, L"%.1f", std::fmaxf(g_ctx.m_globals.next_update[e->EntIndex()] - g_csgo.m_globals()->m_curtime, 0.f));
			std::wstring ws(buf);
			std::string str(ws.begin(), ws.end());

			draw_flag(color, str.c_str());
		}
	}
	if (g_cfg.player.flags[FLAGS_DEBUG].enabled) {
		static Vector originrecords[2];
		Vector curpos = e->m_vecVelocity();
		static bool flip = false;
		originrecords[flip ? 0 : 1] = curpos;
		flip = !flip;
		auto color = Color(g_cfg.menu.menu_theme[0]);
		color.SetAlpha(g_cfg.player.alpha * esp_alpha_fade[e->EntIndex()]);
		wchar_t buf[80];
		swprintf(buf, L"Speed: %.1f", e->m_vecVelocity().Length2D());
		std::wstring ws(buf);
		std::string str(ws.begin(), ws.end());
		draw_flag(color, str.c_str());
		wchar_t buf2[80];
		swprintf(buf2, L"Angle Y: %.1f", e->m_angEyeAngles().y);
		std::wstring ws2(buf2);
		std::string str2(ws2.begin(), ws2.end());
		draw_flag(color, str2.c_str());
		wchar_t buf3[80];
		swprintf(buf3, L"Angle X: %.1f", e->m_angEyeAngles().x);
		std::wstring ws3(buf3);
		std::string str3(ws3.begin(), ws3.end());
		draw_flag(color, str3.c_str());
		wchar_t buf4[80];
		swprintf(buf4, L"ABS Origin LengthSqr: %.f", e->abs_origin().LengthSqr());
		std::wstring ws4(buf4);
		std::string str4(ws4.begin(), ws4.end());
		draw_flag(color, str4.c_str());
		wchar_t buf5[80];
		swprintf(buf5, L"Origin LengthSqr Diff: %.f", (originrecords[0] - originrecords[1]).LengthSqr());
		std::wstring ws5(buf5);
		std::string str5(ws5.begin(), ws5.end());
		draw_flag(color, str5.c_str());
		wchar_t buf6[80];
		swprintf(buf6, L"Simulation Time: %.f", e->m_flSimulationTime());
		std::wstring ws6(buf6);
		std::string str6(ws6.begin(), ws6.end());
		draw_flag(color, str6.c_str());
		wchar_t buf7[80];
		const int shotsfired = ResolverOvermake::resolver.GetPlayerResolveInfo(e).shots_fired[ResolverOvermake::resolver.GetResolveTypeIndex(ResolverOvermake::resolver.GetPlayerResolveInfo(e).resolve_type)];
		const int shots_hit = ResolverOvermake::resolver.GetPlayerResolveInfo(e).shots_hit[ResolverOvermake::resolver.GetResolveTypeIndex(ResolverOvermake::resolver.GetPlayerResolveInfo(e).resolve_type)];
		const int shots_missed = shotsfired - shots_hit;
		swprintf(buf7, L"Missed Shots: %.f", shots_missed);
		std::wstring ws7(buf7);
		std::string str7(ws7.begin(), ws7.end());
		draw_flag(color, str7.c_str());
	}//e->abs_origin().LengthSqr() (originrecords[0] - originrecords[1]).LengthSqr() 		const int shots_missed = resolve_record.shots_fired[GetResolveTypeIndex(resolve_record.resolve_type)] - resolve_record.shots_hit[GetResolveTypeIndex(resolve_record.resolve_type)];

	if (g_cfg.player.flags[FLAGS_SLOWWALK].enabled) {
		if (e->m_vecVelocity().Length2D() < 75 && e->m_vecVelocity().Length2D() > 65)
		{
			draw_flag(color, "walk");
		}
	}
}

void playeresp::draw_skeleton(player_t * e, Color color, matrix3x4_t matrix[128]) {
	auto get_bone_position = [&](int bone) -> vec3_t {
		return vec3_t(matrix[bone][0][3], matrix[bone][1][3], matrix[bone][2][3]);
	};

	auto hdr = g_csgo.m_modelinfo()->GetStudiomodel(e->GetModel());

	if (!hdr)
		return;

	constexpr int chest_bone = 6;
	vec3_t
		upper_direction = get_bone_position(chest_bone + 1) - get_bone_position(chest_bone),
		breast_bone = get_bone_position(chest_bone) + upper_direction / 2;
	vec3_t child, parent;

	for (int i = 0; i < hdr->numbones; i++) {
		mstudiobone_t * bone = hdr->pBone(i);

		if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) {
			child = vec3_t(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]);
			parent = vec3_t(matrix[bone->parent][0][3], matrix[bone->parent][1][3], matrix[bone->parent][2][3]);

			vec3_t
				delta_child = child - breast_bone,
				delta_parent = parent - breast_bone;

			if ((delta_parent.Length() < 9 && delta_child.Length() < 9))
				parent = breast_bone;

			if (i == chest_bone - 1)
				child = breast_bone;

			if (abs(delta_child.z) < 5 && (delta_parent.Length() < 5 && delta_child.Length() < 5) || i == chest_bone)
				continue;

			vec3_t schild, sparent;
			if (math::world_to_screen(child, schild) && math::world_to_screen(parent, sparent))
				render::get().line(schild.x, schild.y, sparent.x, sparent.y, color);
		}
	}
}

void playeresp::draw_lbybar(player_t* m_entity, Box box) {

	/*player_info_t player_info;
	g_csgo.m_engine()->GetPlayerInfo(m_entity->EntIndex(), &player_info);

	if (!g_cfg.player.lby)
		return;

	int
		alpha = g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()],
		outline_alpha = alpha * 0.7f,
		inner_back_alpha = alpha * 0.6f;

	Color
		outline_color{ 0, 0, 0, outline_alpha },
		inner_back_color{ 0, 0, 0, inner_back_alpha },
		text_color = m_entity->IsDormant() ? Color(130, 130, 130, alpha) : Color(255, 255, 255, alpha),
		color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : g_cfg.player.lbybar_color;

	color.SetAlpha(alpha);

	bool drawing_ammo_bar = g_cfg.player.ammo == 1;

	Box n_box = {
		box.x + 1,
		box.y + box.h + 8,
		box.w - 1,
		2
	};

	if (!drawing_ammo_bar)
		n_box.y -= 5;

	float cycle = g_ctx.m_globals.next_update[m_entity->EntIndex()] - m_entity->m_flSimulationTime();
	float bar_width = (((cycle * (n_box.w / 1.1f))));

	util::clamp_value<float>(g_ctx.m_globals.add_time[m_entity->EntIndex()], 0.f, 1.1f);

	render::get().rect_filled(n_box.x - 1, n_box.y - 1, box.w + 2, 4, inner_back_color);
	render::get().rect_filled(n_box.x, n_box.y, bar_width, 2, color);
	render::get().rect(n_box.x - 1, n_box.y - 1, box.w + 2, 4, Color(0, 0, 0, alpha));*/

	if (!g_cfg.player.lby)
		return;

	int
		alpha = g_cfg.player.alpha * esp_alpha_fade[m_entity->EntIndex()],
		outline_alpha = alpha * 0.7f,
		inner_back_alpha = alpha * 0.6f;

	Color
		outline_color{ 0, 0, 0, outline_alpha },
		inner_back_color{ 0, 0, 0, inner_back_alpha },
		text_color = m_entity->IsDormant() ? Color(130, 130, 130, alpha) : Color(255, 255, 255, alpha),
		color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : g_cfg.player.lbybar_color;

	color.SetAlpha(alpha);

	bool drawing_ammo_bar = g_cfg.player.ammo == 1;

	Box n_box = {
		box.x + 1,
		box.y + box.h + 8,
		box.w - 1,
		2
	};

	if (!drawing_ammo_bar)
		n_box.y -= 5;

	static int u = 0;
	float scroll_speed = 1.5f;
	render::get().rect(n_box.x - 1, n_box.y - 1, box.w + 2, 4, Color(0, 0, 0, alpha));
	render::get().rect_filled(n_box.x - 1, n_box.y - 1, box.w + 2, 4, inner_back_color);

	float bar_width = (n_box.w / 4.0f);
	float bar_width2 = (n_box.w / 11.0f);
	float bar_width3 = (n_box.w / 23.0f);
	float bar_width4 = (n_box.w / 32.0f);	
	float bar_width5 = (n_box.w / 45.0f);
	float bar_width6 = (n_box.w / 56.0f);

	if (u <= 100)render::get().rect_filled(n_box.x, n_box.y + 1, bar_width, 2, color);
	if (u > 100 && u <= 220)render::get().rect_filled(n_box.x, n_box.y, bar_width + 2, 2, color);
	if (u > 200 && u <= 310)render::get().rect_filled(n_box.x, n_box.y, bar_width + 2, 2, color);
	if (u > 300 && u <= 410)render::get().rect_filled(n_box.x, n_box.y, bar_width2 + 2, 2, color);
	if (u > 420 && u <= 430)render::get().rect_filled(n_box.x, n_box.y, bar_width3 + 2, 2, color);
	if (u > 440 && u <= 450)render::get().rect_filled(n_box.x, n_box.y, bar_width4 + 2, 2, color);
	if (u > 460 && u <= 470)render::get().rect_filled(n_box.x, n_box.y, bar_width4 + 2, 2, color);
	if (u > 470 && u <= 480)render::get().rect_filled(n_box.x, n_box.y, bar_width5 + 2, 2, color);
	if (u > 480 && u <= 490)render::get().rect_filled(n_box.x, n_box.y, bar_width6 + 2, 2, color);
	if (u > 500 && u <= 510)render::get().rect_filled(n_box.x, n_box.y, bar_width6 + 2, 2, color);
	u += scroll_speed;
	if (u > 510) u = 100;
}

void playeresp::pov_arrow() {
	if (!g_cfg.player.out_pov_arrow)
		return;

	static float alpha = 0.f;
	static int plus_or_minus = false;
	if (alpha <= 0.f || alpha >= 255.f)
		plus_or_minus = !plus_or_minus;

	alpha += plus_or_minus ? (255.f / 0.5f * g_csgo.m_globals()->m_frametime) : -(255.f / 0.5f * g_csgo.m_globals()->m_frametime);
	alpha = math::clamp< float >(alpha, 0.f, 255.f);

	int screen_width, screen_height;
	g_csgo.m_engine()->GetScreenSize(screen_width, screen_height);

	vec3_t client_viewangles;
	g_csgo.m_engine()->GetViewAngles(client_viewangles);

	const float radius = g_cfg.player.out_pov_distance;

	auto draw_arrow = [&](float rotation, Color color, player_t* e, vec3_t poopvec) -> void {
		Vector newangle = math::calculate_angle(Vector(g_ctx.m_local->abs_origin().x, g_ctx.m_local->abs_origin().y, 0), Vector(e->abs_origin().x, e->abs_origin().y, 0));
		math::angle_vectors(Vector(0, 270, 0) - newangle + Vector(0, client_viewangles.y, 0), poopvec);
		auto circlevec = Vector(screen_width / 2, screen_height / 2, 0) + (poopvec * radius);
		render::get().circle_filled(circlevec.x, circlevec.y, 8, 14/*4, 6*/, color);
	};

	for (auto i = 1; i <= g_csgo.m_globals()->m_maxclients; i++) {
		auto e = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!e->valid(true))
			continue;

		vec3_t screen_point;
		g_csgo.m_debugoverlay()->ScreenPosition(e->abs_origin(), screen_point);

		if (screen_point.x < 0 || screen_point.y < 0 || screen_point.x > screen_width || screen_point.y > screen_height) {
			auto angle = math::calculate_angle(g_ctx.m_local->get_eye_pos(), e->abs_origin());
			const auto angle_yaw_rad = DEG2RAD(client_viewangles.y - angle.y);

			Color color = g_cfg.player.out_pov_arrow_color;
			color.SetAlpha(alpha);

			draw_arrow(angle_yaw_rad, color, e, screen_point);
		}
	}
}