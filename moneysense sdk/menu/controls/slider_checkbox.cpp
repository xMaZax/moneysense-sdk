#include "..\cmenu.hpp"
#define gg 9
slidercheckbox::slidercheckbox(std::string n_label, bool* n_state, int* n_value, int n_min, int n_max, std::vector< C_ControlFlag* > n_flags) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	state = n_state;
	flags = n_flags;
	value = n_value;
	min = n_min;
	max = n_max;
	area.h = 9;
	parent->add_control(this);
}
void slidercheckbox::draw() {
	if (menu::get().activate_menu)
	{
		render::get().rect_filled(area.x, area.y, 8, 8, Color{ 62, 62, 62, 255 });

		POINT mouse; GetCursorPos(&mouse);
		rect_t item_region = rect_t(area.x, area.y, 8, 8);

		if (*state) {
			render::get().rect_filled(area.x, area.y, 8, 8, g_cfg.menu.menu_theme[0]);
		}

		render::get().rect(area.x, area.y, 8, 8, Color::Black);

		render::get().text(fonts[TAHOMA12], area.x + 17, area.y + 4, Color(255, 213, 0), HFONT_CENTERED_Y, label.c_str());

		render::get().rect_filled(area.x + 75, area.y, 66, 10, Color{ 62, 62, 62, 255 });
		render::get().rect(area.x + 75, area.y, 66, 10, Color::Black);

		render::get().rect_filled(area.x + 75, area.y, 66, 10, Color{ 62, 62, 62, 255 });
		float ratio = (*value - min) / (max - min);
		float location = ratio * 66;
		render::get().rect_filled(area.x + 75, area.y, location, 10, g_cfg.menu.menu_theme[0]);
		render::get().rect(area.x + 75, area.y, 66, 10, Color::Black);
	}
}
void slidercheckbox::update() {
	if (dragging) {
		if (GetAsyncKeyState(VK_LBUTTON)) {
			POINT mouse;  GetCursorPos(&mouse);
			float
				new_x,
				ratio;
			new_x = mouse.x - area.x;
			if (new_x < 0) { new_x = 0; }
			if (new_x > area.w) { new_x = area.w; }
			ratio = new_x / float(area.w);
			*value = min + (max - min) * ratio;
		}
		else {
			dragging = false;
		}
	}
}
void slidercheckbox::click() {
	*state = !(*state);
	dragging = true;
}