#include "..\cmenu.hpp"
C_Button::C_Button(std::string n_label, std::function< void() > n_func) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	func = n_func;
	area.h = 20;
	parent->add_control(this);
}
/*void C_Button::draw() {
	static float rainbow;
	rainbow += g_cfg.misc.rainbowspeed * 0.0001;
	if (rainbow > 1.f)
		rainbow = 0.f;
	render::get().rect_filled(area.x, area.y, area.w, 20, Color{ 26, 26, 26, 255 });
	POINT mouse; GetCursorPos(&mouse);
	rect_t item_region = rect_t(area.x, area.y, area.w, 20);
	if (item_region.contains_point(mouse)) {
		render::get().rect_filled(area.x, area.y, area.w, 20, Color{ 18, 18, 18, 255 });
	}
	render::get().rect(area.x, area.y, area.w, 20, Color::Black);
	if (g_cfg.misc.RainbowBar)
	{
		render::get().text(fonts[TAHOMA12], area.x + 10, area.y + 10, Color::FromHSB(rainbow, 1.f, 1.f), HFONT_CENTERED_Y, label.c_str());
	}
	else
	{
		render::get().text(fonts[TAHOMA12], area.x + 10, area.y + 10, Color::Purple, HFONT_CENTERED_Y, label.c_str());
	}
}*/

void C_Button::draw() {
	POINT mouse; GetCursorPos(&mouse);
	rect_t item_region = rect_t(area.x, area.y, area.w, 20);

	if (item_region.contains_point(mouse)) {
		render::get().gradient(area.x, area.y, area.w, 20, Color{ 24, 24, 24, 255 }, Color(30, 30, 30, 255), GRADIENT_VERTICAL);
	}

	render::get().rect(area.x, area.y, area.w, 20, Color(25, 25, 25, 255));
	render::get().rect(area.x - 1, area.y - 1, area.w + 2, 22, Color(12, 12, 12, 255));

	render::get().text(fonts[CTAB], area.x + 10, area.y + 10, Color::White, HFONT_CENTERED_Y, label.c_str());
}

void C_Button::update() {

}
void C_Button::click() {
	func(); {
		render::get().rect(area.x - 1, area.y - 1, area.w + 2, 22, Color(0, 0, 0, 255));
	}
}

/*
		static float rainbow;
		rainbow += g_cfg.misc.rainbowspeed * 0.0001;
		if (rainbow > 1.f)
			rainbow = 0.f;
		 Color::FromHSB(rainbow, 1.f, 1.f)
				if (g_cfg.misc.RainbowBar)
				{
				}
		else
		{
		}
		*/