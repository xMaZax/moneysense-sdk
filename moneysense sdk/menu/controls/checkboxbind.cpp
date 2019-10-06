#include "..\cmenu.hpp"
checkboxbind::checkboxbind(std::string n_label, bool* n_state, ButtonCode_t* n_key, std::vector< C_ControlFlag* > n_flags) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	state = n_state;
	flags = n_flags;
	key = n_key;
	if (n_label != "") {
		height_offset = 14;
		area.h += 14;
	}
	area.h = 9;
	parent->add_control(this);
}
void checkboxbind::draw() {
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

		rect_t n_area = rect_t(
			area.x + 131,
			area.y + 7 + height_offset,
			area.w,
			0
		);

		float
			left_brack_size = render::get().text_width(fonts[TAHOMA12], "["),
			right_brack_size = render::get().text_width(fonts[TAHOMA12], "]");

		std::string to_render = g_csgo.m_inputsys()->ButtonCodeToString("[", *key);
		std::transform(to_render.begin(), to_render.end(), to_render.begin(), ::tolower);

		if (*key <= BUTTON_CODE_NONE) {
			to_render = "[]";
		}

		if (taking_input) {
			to_render = "[...]";
		}

		Color text_color = taking_input ? g_cfg.menu.menu_theme[0] : Color(153, 153, 153);
		render::get().text(fonts[keybind_font], area.x + 131, area.y + 7, text_color, HFONT_CENTERED_Y, to_render.c_str());
	}
}
void checkboxbind::update() {
	if (!taking_input) {
		return;
	}
	parent->focus_control(this);
	for (int i = 0; i < 255; i++) {
		if (menu::get().key_press(i)) {
			*key = g_csgo.m_inputsys()->VirtualKeyToButtonCode(i);
			switch (i) {
			case 1: *key = MOUSE_LEFT; break;
			case 2: *key = MOUSE_RIGHT; break;
			case 4: *key = MOUSE_MIDDLE; break;
			case 5: *key = MOUSE_4; break;
			case 6: *key = MOUSE_5; break;
			}
			if (*key == KEY_ESCAPE) {
				*key = BUTTON_CODE_NONE;
				taking_input = false;
				parent->reset_block(); return;
			}
			taking_input = false;
			parent->reset_block(); return;
		}
	}
}
void checkboxbind::click() {
	*state = !(*state);

	if (!taking_input) {
		taking_input = true;
	}
}