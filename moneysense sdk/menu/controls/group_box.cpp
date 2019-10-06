#include "..\cmenu.hpp"

struct rect_x {
	int x, y, w, h;

	rect_x() { }

	rect_x(int n_x, int n_y, int n_w, int n_h) {
		x = n_x;
		y = n_y;
		w = n_w;
		h = n_h;
	}

	bool contains_point(POINT pnt) {
		return
			pnt.x > x &&
			pnt.y > y &&
			pnt.x < x + w &&
			pnt.y < y + h;
	}
};

C_GroupBox::C_GroupBox(GroupRow row, int y, std::string n_name, int sub_tab_count) {
	parent = g_cfg.menu.last_tab;
	int x = 8;
	if (row == GROUP_RIGHT) x += 183;
	area = rect_t(x, y, 160, menu::get().area.h - 20);
	name = n_name;
	if (sub_tab_count != 0) {
	for (int i = 0; i < sub_tab_count; i++)
	subdata.sub_tabs.push_back(i);
	subdata.selected_sub = 0;
	last_pos = 23;
	}
	parent_sub = g_cfg.menu.tab_sub;
	if (parent_sub > -1) {
	area.y += 40;
	area.h -= 40;
	}
	parent->add_group(this);
	g_cfg.menu.last_group = this;
	g_cfg.menu.group_sub = -1;
}

void C_GroupBox::render() {
	if (menu::get().activate_menu)
	{
		render::get().rect_filled(area.x, area.y, area.w + 10, area.h, Color(18, 18, 18));
		render::get().rect(area.x, area.y, area.w + 10, area.h, Color(25, 25, 25));
		render::get().rect(area.x - 1, area.y - 1, area.w + 12, area.h + 2, Color::Black);
		render::get().text(fonts[CTAB], area.x + 5, area.y - 1, Color::White, HFONT_CENTERED_Y, name.c_str()); // text ch1ld
	}
}

void C_GroupBox::update_subs() {
	if (subdata.sub_tabs.size()) {
		bool even = !(subdata.sub_tabs.size() % 2);
		int size = subdata.sub_tabs.size();
		for (int i = 0; i < subdata.sub_tabs.size(); i++) {
			int index = subdata.sub_tabs[i], position_x = area.x;
			index += 1;
			if (!even) {
				int middle_index = subdata.sub_tabs[(size + 1) / 2];
				position_x += area.w / 2;
				if (index < middle_index) {
					int dta = middle_index - index;
					position_x -= 14 * dta;
				}
				if (index > middle_index) {
					int dta = index - middle_index;
					position_x += 14 * dta;
				}
			}
			else {
				int start_pos = area.x + (area.w / 2) - 8;
				start_pos -= (size / 2) * 14;
				position_x = start_pos + (14 * index);
			}
			index -= 1;

			//render::get().rect(position_x, area.y + 15, 4, 6, Color::Black);
			//render::get().gradient(position_x, area.y + 15, 4, 5, index == subdata.selected_sub ? g_cfg.menu.menu_theme[0] : Color{ 51, 132, 255, 255 }, Color{ 51, 132, 255, 255 }, GRADIENT_HORIZONTAL);
			render::get().text(fonts[CATICON], position_x, area.y + 15, index == subdata.selected_sub ? Color{ 187, 187, 187, 255 } : Color{ 255, 255, 255, 255 }, HFONT_CENTERED_X, "D");

			POINT mouse; GetCursorPos(&mouse);
			if (menu::get().key_press(VK_LBUTTON) && !parent->block_input) {
				if (pow(mouse.x - position_x, 2) + pow(mouse.y - (area.y + 15), 2) < pow(7, 2)) {
					subdata.selected_sub = index;
					reset_block();
				}
			}
		}
	}
}
void C_GroupBox::update(rect_t mparent_area) {
	rect_t fallback_rect = area;
	area = rect_t(
		mparent_area.x + area.x,
		mparent_area.y + area.y,
		area.w,
		area.h
	);
	render();
	update_subs();
	bool reset_positions = false;
	for (int i = 0; i < controls.size(); i++) {
		auto & current = controls.at(i);
		if (current->sub != subdata.selected_sub) continue;
		if (current->override_function) {
			if (*current->override_function != current->last_override_state) {
				current->last_override_state = current->override_function;
				reset_positions = true;
			}
		}
	}
	if (reset_positions == true) reset_control_positions();
	for (int i = controls.size() - 1; i >= 0; i--) {
		auto & current = controls.at(i);
		if (current->sub != subdata.selected_sub) continue;
		rect_t flb_rect = current->area;
		current->area = rect_t(
			area.x + current->area.x,
			area.y + current->area.y,
			current->area.w,
			current->area.h
		);
		if ((current->override_function && *current->override_function) || !current->override_function) {
			if (!parent->block_input || current == parent->overriding_control) current->update(); current->draw();
			if (!parent->block_input || current == parent->overriding_control) {
				POINT mouse; GetCursorPos(&mouse);
				if (current->height_offset != 0) {
					current->area.y += current->height_offset;
					current->area.h -= current->height_offset;
				}
				if (current->area.contains_point(mouse)) {
					if (menu::get().key_press(VK_LBUTTON)) {
						current->click();
					}
				}
			}
			if (current->flags.size()) {
				for (int j = current->flags.size() - 1; j >= 0; j--) {
					auto & flag = current->flags.at(j);
					rect_t flb_flag_rect = flag->area;
					flag->area = rect_t(
						area.x + flag->area.x,
						area.y + flag->area.y,
						flag->area.w,
						flag->area.h
					);
					if (!parent->block_input || flag == parent->overriding_flag) {
						flag->update();
					}
					flag->draw();
					if (!parent->block_input || flag == parent->overriding_flag) {
						POINT mouse; GetCursorPos(&mouse);

						if (flag->area.contains_point(mouse)) {
							if (menu::get().key_press(VK_LBUTTON)) { flag->click(); }
						}
					}
					flag->area = flb_flag_rect;
				}
			}
		}
		current->area = flb_rect;
	}
	if (parent->reset_inputblock && parent->block_input) {
		parent->reset_inputblock = false;
		parent->block_input = false;
		parent->blocked_control_flag = false;
		parent->overriding_flag = nullptr;
		parent->overriding_control = nullptr;
	}
	area = fallback_rect;
}
void C_GroupBox::reset_control_positions() {
	last_pos = 8;
	std::vector< C_Control * > backup_controls = controls;
	controls.clear();
	for (int i = 0; i < backup_controls.size(); i++) {
		auto & current = backup_controls.at(i);
		add_control(current, false);
	}
}
void C_GroupBox::add_control(C_Control * control, bool first) {
	controls.push_back(control);
	if (subdata.sub_tabs.size()) {
		int current = first ? g_cfg.menu.group_sub : control->sub;
		if (subdata.last_sub != current) {
			last_pos = 23;
			subdata.last_sub = current;
		}
	}
	float flags_offset = 0.0f;
	if (control->flags.size()) {
		int next_flag_pos = 8 + area.w - 16;
		for (int i = 0; i < control->flags.size(); i++) {
			auto & flag = control->flags.at(i);
			if (!control->height_offset) flags_offset += flag->area.w + 2;
			next_flag_pos -= flag->area.w;
			flag->area.x = next_flag_pos;
			int flag_y = last_pos;
			if (control->height_offset) {
				flag_y += (control->height_offset / 2) - 4;
			}
			else {
				flag_y += (control->area.h / 2) - 4;
			}
			flag->area.y = flag_y;
			next_flag_pos -= 2;
		}
	}
	control->area = rect_t(8, last_pos, area.w - 16 - flags_offset, control->area.h);
	if (g_cfg.menu.group_sub != -1) control->sub = g_cfg.menu.group_sub;
	last_pos += control->area.h + 4;
}