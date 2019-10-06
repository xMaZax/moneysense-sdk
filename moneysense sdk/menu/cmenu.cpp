#include "cmenu.hpp"
#include "../hack/backdrop.h"
#include <windows.h>
#include <Lmcons.h>
#include "..\cheats\misc\logs.h"
#include "../loader.h"
IDirect3DDevice9* m_device;
menu::menu() {
	area = rect_t(200, 200, 369, 425);
	text = rect_t(150, 150, 369, 390);
}
void menu::draw_weapons()
{
	rect_t top_area = rect_t(
		area.x - 86,
		area.y + 410,
		area.w + 99,
		45
	);

	auto box = [](int x, int y, int w, int h, Color c) {
		render::get().line(x, y, x, y + h, c);
		render::get().line(x, y + h, x + w + 1, y + h, c);
		render::get().line(x + w, y, x + w, y + h, c);
		render::get().line(x, y, x + w, y, c);
	};

	render::get().rect_filled(top_area.x, top_area.y, top_area.w, top_area.h, Color(21, 21, 21));

	box(top_area.x - 0, top_area.y - 0, top_area.w + 0 * 2, top_area.h + 0 * 2, Color(14, 14, 14));
	//box(top_area.x - 1, top_area.y - 1, top_area.w + 1 * 2, top_area.h + 1 * 2, Color(48, 48, 48));
	//box(top_area.x - 2, top_area.y - 2, top_area.w + 2 * 2, top_area.h + 2 * 2, Color(48, 48, 48));

	//box(top_area.x - 3, top_area.y - 3, top_area.w + 3 * 2, top_area.h + 3 * 2, Color(48, 48, 48));
	//box(top_area.x - 4, top_area.y - 4, top_area.w + 4 * 2, top_area.h + 4 * 2, Color(48, 48, 48));
	//box(top_area.x - 5, top_area.y - 5, top_area.w + 5 * 2, top_area.h + 5 * 2, Color(48, 48, 48));

	//box(top_area.x - 5, top_area.y - 5, top_area.w + 5 * 2, top_area.h + 5 * 2, Color(48, 48, 48));
	//box(top_area.x - 6, top_area.y - 6, top_area.w + 6 * 2, top_area.h + 6 * 2, Color(48, 48, 48));
	//box(top_area.x - 7, top_area.y - 7, top_area.w + 7 * 2, top_area.h + 7 * 2, Color(48, 48, 48));

	//box(top_area.x - 8, top_area.y - 8, top_area.w + 8 * 2, top_area.h + 8 * 2, Color(48, 48, 48));
	//box(top_area.x - 9, top_area.y - 9, top_area.w + 9 * 2, top_area.h + 9 * 2, Color(48, 48, 48));
	//box(top_area.x - 10, top_area.y - 10, top_area.w + 10 * 2, top_area.h + 10 * 2, Color(48, 48, 48));

	//box(top_area.x - 11, top_area.y - 11, top_area.w + 11 * 2, top_area.h + 11 * 2, Color(48, 48, 48));
	//box(top_area.x - 11, top_area.y - 11, top_area.w + 11 * 2, top_area.h + 11 * 2, Color(48, 48, 48));
	//box(top_area.x - 12, top_area.y - 12, top_area.w + 12 * 2, top_area.h + 12 * 2, Color(48, 48, 48));

	render::get().text(fonts[weapons_big], top_area.x + 15, top_area.y + 22, Color::Grey, HFONT_CENTERED_Y, "G P W N g Z");

	POINT mouse; GetCursorPos(&mouse);
	rect_t item_region = rect_t(top_area.x, top_area.y, top_area.w, 19);
	if (item_region.contains_point(mouse)) {
		render::get().text(fonts[weapons_big], top_area.x + 15, top_area.y + 22, Color::White, HFONT_CENTERED_Y, "G P W N g Z");
	}
}

void menu::render() {

	if (g_cfg.misc.backend)
	{
		int iScreenWidth, iScreenHeight;
		g_csgo.m_engine()->GetScreenSize(iScreenWidth, iScreenHeight);

		static wchar_t texst[256];
		static wchar_t texst1[256];
		const auto charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*abcdefghijklmnopqrstuvwxyz";
		swprintf(texst, L"moneysense");
		static int len = std::wstring(texst).length();
		static bool bSleep = false;
		static int delayTime;
		static int pos = 0;

		if (pos > -1) {
			for (int i = 0; i < len; ++i) {
				if (i >= pos) texst[i] = charset[rand() % strlen(charset) - 1];
			}
		}

		if (!bSleep) {
			pos++;
			bSleep = true;
			delayTime = GetTickCount() + 50;
			if (pos == len) {
				pos = -1;
				delayTime += 2000;
			}
		}
		else if (delayTime < GetTickCount()) {
			bSleep = false;
		}

		draw::backdrop();
		render::get().textgay(texst, iScreenWidth / 2.4f, iScreenHeight / 2, Color(255, 255, 255, 255), fonts[BIGVERMIN], HFONT_OFF_SCREEN);
		//render::get().textgay(texst1, iScreenWidth / 1.5f, iScreenHeight / 2, Color(132, 234, 0, 255), fonts[BIGVERMIN], HFONT_OFF_SCREEN);
		//render::get().text(fonts[BIGVERMIN], iScreenWidth / 2.0f, iScreenHeight / 2, Color(255, 255, 255, 255), HFONT_CENTERED_X, texst);
		//render::get().text(fonts[BIGVERMIN], iScreenWidth / 1.8f, iScreenHeight / 2, Color(132, 234, 0, 255), HFONT_CENTERED_X, texst1);
		render::get().rect_filled(0, 0, 4000, 4000, Color(1, 1, 1, 200));
	}

	if (std::strstr(GetCommandLineA(), "-insecure")) {

		float watermarkRainbowSpeed = 0.005f;
		static float rainbow;
		rainbow += watermarkRainbowSpeed;
		if (rainbow > 1.f) rainbow = 0.f;
		static int u = 0;
		float watermarkScrollSpeed = 2.5f;

		render::get().rect_filled(0, 5 + 40, 126, 24, Color(0, 0, 0, 75));
		render::get().rect_filled(0, 5 + 40, 2, 24, Color(255, 0, 0));

		if (u <= 100) render::get().text(fonts[CTAB], 10, 10 + 6 + 40, Color(255, 255, 255), HFONT_CENTERED_Y, "running in insecure.");
		if (u > 100 && u <= 220) render::get().text(fonts[CTAB], 10, 10 + 6 + 40, Color(255, 0, 0), HFONT_CENTERED_Y, "running in insecure..");
		if (u > 200 && u <= 310) render::get().text(fonts[CTAB], 10, 10 + 6 + 40, Color(255, 255, 255), HFONT_CENTERED_Y, "running in insecure...");
		if (u > 300 && u <= 410)  render::get().text(fonts[CTAB], 10, 10 + 6 + 40, Color(255, 0, 0), HFONT_CENTERED_Y, "running in insecure....");
		u += watermarkScrollSpeed;
		if (u > 410) u = 0;

	}

	if (!g_cfg.misc.anti_untrusted) {

		render::get().rect_filled(0, 5 + 6 + 75, 134, 24, Color(0, 0, 0, 75));
		render::get().rect_filled(0, 5 + 6 + 75, 2, 24, Color(250, 195, 13));

		render::get().text(fonts[CTAB], 10, 10 + 12 + 75, Color(250, 195, 13), HFONT_CENTERED_Y, "please on A-Untrusted");

	}

	auto outline = [&](int offset, Color color)
	{
		rect_t m_pos = rect_t(
			area.x - 75, area.y, area.w + 75, area.h
		);
		auto box = [](int x, int y, int w, int h, Color c)
		{
			render::get().line(x, y, x, y + h, c);
			render::get().line(x, y + h, x + w + 1, y + h, c);
			render::get().line(x + w, y, x + w, y + h, c);
			render::get().line(x, y, x + w, y, c);
		};
		box(m_pos.x - offset, m_pos.y - offset, m_pos.w + offset * 2, m_pos.h + offset * 2, color);
	};

	auto top_shadow = [this]() -> void {
		rect_t shadw_area = rect_t(
			area.x - 75, area.y - 20,
			area.w + 75, 20
		);
	};

	rect_t main_tab_area = rect_t(
		area.x - 75, area.y,
		75, area.h
	);

	rect_t area_all = rect_t(
		area.x - 75,
		area.y,
		area.w,
		area.h
	);

	//renderimage();
	/*if (this->menu_background == nullptr) {
		this->menu_background = new c_sprite();
		this->menu_background->setup(m_device, background_texture_bytes, sizeof(background_texture_bytes), 1000, 1000);

		D3DXCreateSprite(m_device, &menu_texture_sprite);

		int screen_w, screen_h;
		g_csgo.m_engine()->GetScreenSize(screen_w, screen_h);

		menu::get().m_device->CreateTexture(screen_w, screen_h, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &menu_texture, nullptr);
	}*/

	//c_sprite::get().setup(m_device, background_texture_bytes, 1000, area.w, area.h);
	//c_sprite::get().draw(area.x, area.y, 255, 0.f);
	render::get().rect_filled(area_all.x, area_all.y, area_all.w + 85, area_all.h, Color(23, 23, 23));
	for (int io = 0; io < 113; io++)
	{
		render::get().rect(main_tab_area.x + (io * 4), area.y, 1, area.h, Color(18, 18, 18));
	}

	//render::get().gradient(area.x, 1, area.w, 1, Color(51, 132, 255, 255), Color(235, 29, 173, 255), GRADIENT_HORIZONTAL);
	//render::get().gradient(area.x, 2, area.w, 2, Color(51, 132, 255, 255), Color(235, 29, 173, 255), GRADIENT_HORIZONTAL);
	//render::get().rect_filled(area.x, 2, area.w, 2, Color(23, 23, 23, 155));

	for (int i = 0; i < tabs.size(); i++) {
		tabs[i]->paint();
	}
	//outline(0, Color::Black);
	//outline(1, Color(48, 48, 48));
	//outline(2, Color::Black);

	outline(0, Color::Black);
	outline(1, Color(53, 53, 53));
	outline(2, Color(28, 28, 28));
	outline(3, Color(28, 28, 28));
	outline(4, Color(28, 28, 28));
	outline(5, Color(28, 28, 28));
	outline(6, Color(28, 28, 28));
	outline(7, Color(28, 28, 28));
	outline(8, Color(28, 28, 28));
	outline(9, Color(28, 28, 28));
	outline(10, Color(28, 28, 28));
	outline(11, Color(53, 53, 53));
	outline(12, Color::Black);

	auto top_bar = [this]() -> void {
		rect_t top_area = rect_t(
			area.x - 75,
			area.y - 55,
			area.w + 75,
			20
		);

		auto box = [](int x, int y, int w, int h, Color c) {
			render::get().line(x, y, x, y + h, c);
			render::get().line(x, y + h, x + w + 1, y + h, c);
			render::get().line(x + w, y, x + w, y + h, c);
			render::get().line(x, y, x + w, y, c);
		};

		render::get().rect_filled(top_area.x, top_area.y, top_area.w, top_area.h, Color(35, 35, 35));
		render::get().gradient(top_area.x, top_area.y, top_area.w, 3, Color(51, 132, 255, 255), Color(235, 29, 173, 255), GRADIENT_HORIZONTAL);

		box(top_area.x - 0, top_area.y - 0, top_area.w + 0 * 2, top_area.h + 0 * 2, Color::Black);
		box(top_area.x - 1, top_area.y - 1, top_area.w + 1 * 2, top_area.h + 1 * 2, Color(53, 53, 53));
		box(top_area.x - 2, top_area.y - 2, top_area.w + 2 * 2, top_area.h + 2 * 2, Color(28, 28, 28));

		box(top_area.x - 3, top_area.y - 3, top_area.w + 3 * 2, top_area.h + 3 * 2, Color(28, 28, 28));
		box(top_area.x - 4, top_area.y - 4, top_area.w + 4 * 2, top_area.h + 4 * 2, Color(28, 28, 28));
		box(top_area.x - 5, top_area.y - 5, top_area.w + 5 * 2, top_area.h + 5 * 2, Color(28, 28, 28));

		box(top_area.x - 5, top_area.y - 5, top_area.w + 5 * 2, top_area.h + 5 * 2, Color(28, 28, 28));
		box(top_area.x - 6, top_area.y - 6, top_area.w + 6 * 2, top_area.h + 6 * 2, Color(28, 28, 28));
		box(top_area.x - 7, top_area.y - 7, top_area.w + 7 * 2, top_area.h + 7 * 2, Color(28, 28, 28));

		box(top_area.x - 8, top_area.y - 8, top_area.w + 8 * 2, top_area.h + 8 * 2, Color(28, 28, 28));
		box(top_area.x - 9, top_area.y - 9, top_area.w + 9 * 2, top_area.h + 9 * 2, Color(28, 28, 28));
		box(top_area.x - 10, top_area.y - 10, top_area.w + 10 * 2, top_area.h + 10 * 2, Color(28, 28, 28));

		box(top_area.x - 11, top_area.y - 11, top_area.w + 11 * 2, top_area.h + 11 * 2, Color(28, 28, 28));
		box(top_area.x - 11, top_area.y - 11, top_area.w + 11 * 2, top_area.h + 11 * 2, Color(53, 53, 53));
		box(top_area.x - 12, top_area.y - 12, top_area.w + 12 * 2, top_area.h + 12 * 2, Color::Black);

		float speed_text = 0.005f;
		static float rainbow;
		rainbow += speed_text;
		if (rainbow > 1.f) rainbow = 0.f;
		static int u = 0;
		float watermarkScrollSpeed = 2.5f;

		if (u <= 100) render::get().text(fonts[CTAB_MIN], top_area.x + 8, top_area.y + 11, Color::Grey, HFONT_CENTERED_Y, "moneysense beta | " __DATE__);
		if (u > 100 && u <= 220) render::get().text(fonts[CTAB_MIN], top_area.x + 8, top_area.y + 11, Color::Grey, HFONT_CENTERED_Y, "moneysense beta | " __TIME__);
		if (u > 200 && u <= 310) render::get().text(fonts[CTAB_MIN], top_area.x + 8, top_area.y + 11, Color::Grey, HFONT_CENTERED_Y, "moneysense beta | " __DATE__);
		if (u > 300 && u <= 410)  render::get().text(fonts[CTAB_MIN], top_area.x + 8, top_area.y + 11, Color::Grey, HFONT_CENTERED_Y, "moneysense beta | " __TIME__);
		u += watermarkScrollSpeed;
		if (u > 410) u = 0;

		render::get().text(fonts[CTAB_MIN], top_area.x + 41, top_area.y + 11, Color(g_cfg.menu.menu_theme[0]), HFONT_CENTERED_Y, "sense");

		std::stringstream ss;

	    ss << "unnamed | days left:";
		render::get().text(fonts[CTAB_MIN], top_area.x + 332, top_area.y + 11, Color(255, 255, 255), HFONT_CENTERED_Y, ss.str().c_str());
		/*if (!g_cfg.ragebot.enable)
		{
			render::get().text(fonts[CTAB_MIN], top_area.x + 390, top_area.y + 11, Color::Grey, HFONT_CENTERED_Y, "R");
		}

		if (!g_cfg.antiaim.animfix)
		{
			render::get().text(fonts[CTAB_MIN], top_area.x + 405, top_area.y + 11, Color::Grey, HFONT_CENTERED_Y, "AA");
		}

		if (!g_cfg.player.enable)
		{
			render::get().text(fonts[CTAB_MIN], top_area.x + 430, top_area.y + 11, Color::Grey, HFONT_CENTERED_Y, "V");
		}



		if (g_cfg.ragebot.enable)
		{
			render::get().text(fonts[CTAB_MIN], top_area.x + 390, top_area.y + 11, Color(g_cfg.menu.menu_theme[0]), HFONT_CENTERED_Y, "R");
		}

		if (g_cfg.antiaim.animfix)
		{
			render::get().text(fonts[CTAB_MIN], top_area.x + 405, top_area.y + 11, Color(g_cfg.menu.menu_theme[0]), HFONT_CENTERED_Y, "AA");
		}

		if (g_cfg.player.enable)
		{
			render::get().text(fonts[CTAB_MIN], top_area.x + 430, top_area.y + 11, Color(g_cfg.menu.menu_theme[0]), HFONT_CENTERED_Y, "V");
		}*/
	};

	//top_bar();

	//menu::get().draw_weapons();

	if (g_cfg.player.esp_view && g_cfg.player.enable)
	{
		//render::get().rect_filled(550, 190, 215, 100, Color(56, 56, 56)); // title bar

		render::get().rect_filled(550, 200, 235, 330, Color(18, 18, 18)); // background
		render::get().rect(550, 200, 235, 330, Color(43, 43, 43)); // outline

		auto box_esp = [](int x, int y, int w, int h, Color c) {
			render::get().line(x, y, x, y + h, c);
			render::get().line(x, y + h, x + w + 1, y + h, c);
			render::get().line(x + w, y, x + w, y + h, c);
			render::get().line(x, y, x + w, y, c);
		};
		if (g_cfg.player.box && g_cfg.player.enable) {
			box_esp(561, 215, 167, 276, Color(g_cfg.player.box_color));
		}
		if (g_cfg.player.flags[FLAGS_VULNERABLE].enabled && g_cfg.player.enable)
		{
			render::get().text(fonts[AIMWAREFONT], 631, 198, Color(255, 151, 16), HFONT_CENTERED_Y, "::bruteforce::");
		}
		if (g_cfg.player.name && g_cfg.player.enable)
		{
			render::get().text(fonts[AIMWAREFONT], 631, 209, Color(g_cfg.player.name_color), HFONT_CENTERED_Y, "name"); // name esp
		}
		if (g_cfg.player.weapon != 0 && g_cfg.player.enable)
		{
			render::get().text(fonts[WEAPONICONS], 631, 510, Color(g_cfg.player.weapon_color), HFONT_CENTERED_Y, "4"); // weapon icon
		}
		if (g_cfg.player.weapon != 1 && g_cfg.player.enable)
		{
			render::get().text(fonts[AIMWAREFONT], 631, 510, Color(g_cfg.player.weapon_color), HFONT_CENTERED_Y, "weapon"); // weapon text
		}


		if (g_cfg.player.flags[FLAGS_SCOPED].enabled && g_cfg.player.enable) {
			render::get().text(fonts[CTAB], 732, 226, Color(150, 150, 220), HFONT_CENTERED_Y, "Scoped");
		}
		if (g_cfg.player.flags[FLAGS_BALANCE].enabled && g_cfg.player.enable) {
			render::get().text(fonts[CTAB], 732, 226 + 10, Color(96, 230, 19), HFONT_CENTERED_Y, "money");
		}
		if (g_cfg.player.flags[FLAGS_BREAKINGLC].enabled && g_cfg.player.enable) {
			render::get().text(fonts[CTAB], 732, 226 + 10 + 10, Color::Green, HFONT_CENTERED_Y, "LC");
		}
		if (g_cfg.player.flags[FLAGS_RESOLVERMODE].enabled && g_cfg.player.enable) {
			render::get().text(fonts[CTAB], 732, 226 + 10 + 10 + 10, Color(255, 110, 199), HFONT_CENTERED_Y, "Res. Mode");
		}
		if (g_cfg.player.flags[FLAGS_SLOWWALK].enabled && g_cfg.player.enable) {
			render::get().text(fonts[CTAB], 732, 226 + 10 + 10 + 10 + 10, Color(255, 110, 199), HFONT_CENTERED_Y, "slowwalk");
		}


		if (g_cfg.player.ammo == 1 && g_cfg.player.enable)
		{
			render::get().rect_filled(561, 495, 168, 3, Color(g_cfg.player.ammobar_color)); // ammo bar
		}

		if (g_cfg.player.health && g_cfg.player.enable)
		{
			render::get().rect_filled(556, 215, 3, 276, Color::Green); // health bar
		}
	}

	/*auto draw_loader_window = [this]() -> void {

		auto outline_box = [](int x, int y, int w, int h, Color c) {
			render::get().line(x, y, x, y + h, c);
			render::get().line(x, y + h, x + w + 1, y + h, c);
			render::get().line(x + w, y, x + w, y + h, c);
			render::get().line(x, y, x + w, y, c);
		};

		render::get().rect_filled(500, 500, 242, 150, Color(14, 14, 14));
		outline_box(500, 500, 242, 150, Color(26, 26, 26));
		render::get().text(fonts[CTAB_MIN], 506, 511, Color(244, 244, 244), HFONT_CENTERED_NONE, "moneysense");
	};

	draw_loader_window();*/
}

void menu::update() {
	poll_keyboard();
	int gradient[3] = {
		g_cfg.menu.menu_theme[0].r() - 20,
		g_cfg.menu.menu_theme[0].g() - 20,
		g_cfg.menu.menu_theme[0].b() - 20
	};
	for (int i = 0; i < 3; i++) {
		if (gradient[i] < 42) {
			gradient[i] = g_cfg.menu.menu_theme[0][i] + 20;
		}
	}
	g_cfg.menu.menu_theme[1] = Color(gradient[0], gradient[1], gradient[2]);
	if (key_press(g_csgo.m_inputsys()->ButtonCodeToVirtualKey(g_cfg.menu.menu_bind))) {
		toggle();
		g_csgo.m_cvar()->FindVar("cl_mouseenable")->SetValue(!active());
		g_csgo.m_inputsys()->EnableInput(!active());
		g_csgo.m_inputsys()->ResetInputState();
	}
	if (!active()) {
		return;
	}
	poll_mouse();
	//if (activate_menu) {
		render();
	//}
	rect_t main_tab_area = rect_t(
		area.x - 95, area.y, 95, area.h
	);
	for (int i = 0; i < tabs.size(); i++) {
		tabs[i]->update();
	}
	if (animating) {
		if (animate_direction == TAB_ANIMATION_DOWN) {
			if ((main_tab_area.y + animation_offset) < animation_destination) {
				animation_offset += 2;
			}
			else {
				animating = false;
			}
		}
		else {
			if ((main_tab_area.y + animation_offset) > animation_destination) {
				animation_offset -= 2;
			}
			else {
				animating = false;
			}
		}
	}
	c_mouse.paint();
}

void menu::poll_keyboard() {
	std::copy(m_keystate, m_keystate + 255, m_oldstate);
	for (auto n = 0; n < 255; ++n) {
		m_keystate[n] = GetAsyncKeyState(n);
	}
}
void menu::poll_mouse() {
	rect_t top_area = rect_t(
		area.x,
		area.y,
		area.w,
		10
	);
	if (dragging && !GetAsyncKeyState(VK_LBUTTON)) {
		dragging = false;
	}
	if (GetAsyncKeyState(VK_LBUTTON) && top_area.contains_point(c_mouse.cursor)) {
		dragging = true;
	}
	if (dragging) {
		drag_x = c_mouse.cursor.x - area.x;
		drag_y = c_mouse.cursor.y - area.y;
		GetCursorPos(&c_mouse.cursor);
		area.x = c_mouse.cursor.x - drag_x;
		area.y = c_mouse.cursor.y - drag_y;
	}
	else {
		GetCursorPos(&c_mouse.cursor);
	}
}
void menu::Mouse_t::paint() {
	render::get().rect_filled(cursor.x + 1, cursor.y, 1, 17, g_cfg.menu.menu_theme[0]);
	for (int i = 0; i < 11; i++) {
		render::get().rect_filled(cursor.x + 2 + i, cursor.y + 1 + i, 1, 1, g_cfg.menu.menu_theme[0]);
	}
	render::get().rect_filled(cursor.x + 8, cursor.y + 12, 5, 1, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 8, cursor.y + 13, 1, 1, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 9, cursor.y + 14, 1, 2, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 10, cursor.y + 16, 1, 2, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 8, cursor.y + 18, 2, 1, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 7, cursor.y + 16, 1, 2, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 6, cursor.y + 14, 1, 2, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 5, cursor.y + 13, 1, 1, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 4, cursor.y + 14, 1, 1, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 3, cursor.y + 15, 1, 1, g_cfg.menu.menu_theme[0]);
	render::get().rect_filled(cursor.x + 2, cursor.y + 16, 1, 1, g_cfg.menu.menu_theme[0]);
	for (int i = 0; i < 4; i++) {
		render::get().rect_filled(cursor.x + 2 + i, cursor.y + 2 + i, 1, 14 - (i * 2), Color::Grey);
	}
	render::get().rect_filled(cursor.x + 6, cursor.y + 6, 1, 8, Color::Grey);
	render::get().rect_filled(cursor.x + 7, cursor.y + 7, 1, 9, Color::Grey);
	for (int i = 0; i < 4; i++) {
		render::get().rect_filled(cursor.x + 8 + i, cursor.y + 8 + i, 1, 4 - i, Color::Grey);
	}
	render::get().rect_filled(cursor.x + 8, cursor.y + 14, 1, 4, Color::Grey);
	render::get().rect_filled(cursor.x + 9, cursor.y + 16, 1, 2, Color::Grey);
}
