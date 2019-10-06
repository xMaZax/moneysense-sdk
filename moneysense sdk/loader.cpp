#include "loader.h"
std::string label;
std::string label1;
void loader::draw()
{
	auto outline = [&](int offset, Color color)
	{
		rect_t m_pos = rect_t(
			35, 45, 205, 125
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


	render::get().rect_filled(35, 45, 205, 125, Color(18, 18, 18));
	render::get().gradient(35, 45, 205, 3, Color(51, 132, 255, 255), Color(235, 29, 173, 255), GRADIENT_HORIZONTAL);
	render::get().rect_filled(35, 47, 205, 1, Color(0, 0, 0, 200));

	outline(0, Color::Black);
	outline(1, Color(53, 53, 53));
	outline(2, Color(28, 28, 28));
	outline(3, Color(53, 53, 53));
	outline(4, Color::Black);

	POINT mouse; GetCursorPos(&mouse);
	rect_t item_region = rect_t(70, 91 - 15 + 25 + 35, 128, 20);
	rect_t item_region1 = rect_t(70, 91 - 15, 128, 17);
	rect_t item_region2 = rect_t(70, 91 - 15 + 25, 128, 17);
	rect_t item_region23 = rect_t(70, 91 - 15 + 25 + 35, 128, 20);

	render::get().rect_filled(70, 91 - 15, 128, 17, Color(54, 54, 54));

	render::get().rect_filled(70, 91 - 15, 128, 17, Color(54, 54, 54));
	render::get().rect_filled(70, 91 - 15 + 25, 128, 17, Color(54, 54, 54));

	render::get().rect(70, 91 - 15, 128, 17, Color(14, 14, 14));
	render::get().rect(70, 91 - 15 + 25, 128, 17, Color(14, 14, 14));

	if (item_region1.contains_point(mouse)) {
		render::get().rect_filled(70, 91 - 15, 128, 17, Color(65, 65, 65));
	}

	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);
	std::stringstream ss;

	ss << GetUserName;

	render::get().text(fonts[CTAB], 70 + 6, 91 - 15 + 26 - 24, Color(255, 255, 255), HFONT_CENTERED_NONE, ss.str().c_str());
	if (item_region2.contains_point(mouse)) {
		render::get().rect_filled(70, 91 - 15 + 25, 128, 17, Color(65, 65, 65));

		static int u = 0;
		float spid = 2.5f;

		if (u <= 100)render::get().text(fonts[CTAB], 70 + 6, 91 - 15 + 26, Color(255, 255, 255), HFONT_CENTERED_NONE, "*");
		if (u > 100 && u <= 220)render::get().text(fonts[CTAB], 70 + 6, 91 - 15 + 26, Color(255, 255, 255), HFONT_CENTERED_NONE, "**");
		if (u > 200 && u <= 310)render::get().text(fonts[CTAB], 70 + 6, 91 - 15 + 26, Color(255, 255, 255), HFONT_CENTERED_NONE, "***");
		if (u > 300 && u <= 410)render::get().text(fonts[CTAB], 70 + 6, 91 - 15 + 26, Color(255, 255, 255), HFONT_CENTERED_NONE, "*****");
		if (u > 410 && u <= 420)render::get().text(fonts[CTAB], 70 + 6, 91 - 15 + 26, Color(255, 255, 255), HFONT_CENTERED_NONE, "******");
		u += spid;
		if (u > 420) u = 0;
	}

	render::get().rect_filled(70, 91 - 15 + 25 + 35, 128, 20, Color(24, 24, 24)); //button

	render::get().rect(70, 91 - 15 + 25 + 35, 128, 20, Color(14, 14, 14));
	if (item_region.contains_point(mouse)) {
		menu::get().activate_menu = true; // activate hack
		render::get().rect(70, 91 - 15 + 25 + 35, 128, 20, Color(76, 76, 76)); // out mouse button
		g_cfg.misc.carterrr = false; // off loader draw
	}

	render::get().text(fonts[CTAB], 70 + 50, 91 - 15 + 25 + 35 + 4, Color(255, 255, 255), HFONT_CENTERED_NONE, "Login");

	render::get().text(fonts[CTAB], 70 + 30, 91 - 15 + 25 - 50, Color(255, 255, 255), HFONT_CENTERED_NONE, "money");
	render::get().text(fonts[CTAB], 70 + 63, 91 - 15 + 25 - 50, Color(g_cfg.menu.menu_theme[0]), HFONT_CENTERED_NONE, "sense");

}
