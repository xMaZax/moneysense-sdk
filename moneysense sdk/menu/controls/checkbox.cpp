#include "..\cmenu.hpp"
C_CheckBox::C_CheckBox(std::string n_label, bool * n_state, std::vector< C_ControlFlag * > n_flags) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	state = n_state;
	flags = n_flags;
	area.h = 9;
	parent->add_control(this);
}
void C_CheckBox::draw() {
	if (menu::get().activate_menu)
	{
		render::get().rect_filled(area.x, area.y, 8, 8, Color{ 62, 62, 62, 255 });

		POINT mouse; GetCursorPos(&mouse);
		rect_t item_region = rect_t(area.x, area.y, 8, 8);

		if (*state) {
			render::get().rect_filled(area.x, area.y, 8, 8, g_cfg.menu.menu_theme[0]);
		}

		render::get().rect(area.x, area.y, 8, 8, Color::Black);

		render::get().text(fonts[TAHOMA12], area.x + 17, area.y + 4, Color::White, HFONT_CENTERED_Y, label.c_str());
	}
}
void C_CheckBox::update() {
}
void C_CheckBox::click() {
	*state = !(*state);
}

/*
	static float rainbow;
	rainbow += 0.0005f;
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