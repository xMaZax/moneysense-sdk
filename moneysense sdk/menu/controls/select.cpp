#include "../cmenu.hpp"
#include <math.h>
#include <algorithm>
#define gg 19
C_Selector::C_Selector(std::string n_label, int* n_selected, std::vector< std::string > n_items, std::vector< C_ControlFlag* > n_flags) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	selected = n_selected;
	items = n_items;
	using_pointer = false;
	flags = n_flags;
	area.h = gg;
	if (n_label != "") {
		height_offset = 14;
		area.h += 14;
	}
	parent->add_control(this);
}
C_Selector::C_Selector(std::string n_label, int* n_selected, std::vector< std::string >* n_items, std::vector< C_ControlFlag* > n_flags) {
	parent = g_cfg.menu.last_group;
	label = n_label;
	selected = n_selected;
	items_pointer = n_items;
	using_pointer = true;
	flags = n_flags;
	area.h = gg;
	if (n_label != "") {
		height_offset = 14;
		area.h += 14;
	}
	parent->add_control(this);
}

void C_Selector::draw() {
	rect_t n_area = rect_t(
		area.x,
		area.y + height_offset,
		area.w,
		gg
	);
	POINT mouse; GetCursorPos(&mouse);
	rect_t item_region = rect_t(n_area.x, n_area.y, n_area.w, gg);
	n_area.y -= 14;
	n_area.h -= gg;
	bool select_text1 = true;
	bool select_text2 = true;
	bool select_text3 = true;
	render::get().rect_filled(n_area.x, n_area.y, area.w, items.size() * gg, Color{ 18, 18, 18, 255 });
	for (int i = 0; i < items.size(); i++) {
		auto& current = items.at(i);
		rect_t item_region = rect_t(n_area.x, n_area.y + (gg * i), n_area.w, gg);
		if (item_region.contains_point(mouse)) {
			render::get().rect_filled(n_area.x, n_area.y + (gg * i), n_area.w, gg, Color{ 26, 26, 26, 255 });
		}
		if (i == *selected) {
			render::get().text(fonts[CTAB], area.x + 10, n_area.y + (gg * i) + 10, g_cfg.menu.menu_theme[0], HFONT_CENTERED_Y, current.c_str());
		}
		else {
			if (select_text1)
			{
				if (!item_region.contains_point(mouse))
				{
					render::get().text(fonts[TAHOMA12], area.x + 10, n_area.y + (gg * i) + 10, Color(153, 153, 153), HFONT_CENTERED_Y, current.c_str());
				}
			}
			if (select_text3)
			{
				if (item_region.contains_point(mouse))
				{
					render::get().text(fonts[CTAB], area.x + 10, n_area.y + (gg * i) + 10, Color(255, 255, 255), HFONT_CENTERED_Y, current.c_str());
				}
			}
		}
	}
	render::get().rect(n_area.x, n_area.y, area.w, items.size() * gg, Color::Black);
}
void C_Selector::update() {
	if (using_pointer) items = *items_pointer;
	if (!open || !items.size()) return;
	POINT mouse; GetCursorPos(&mouse);
	parent->focus_control(this);
	for (int i = 0; i < items.size(); i++) {
		rect_t item_region = rect_t(area.x, area.y + gg + (height_offset)+(i * gg), area.w, gg);
		if (menu::get().key_press(VK_LBUTTON) && item_region.contains_point(mouse)) {
			open = false;
			*selected = i;
			parent->reset_block();
		}
	}
	rect_t full_area = rect_t(area.x, area.y + height_offset, area.w, area.h + (items.size() * gg));
	if (menu::get().key_press(VK_LBUTTON) && !full_area.contains_point(mouse)) {
		open = false;
		parent->reset_block();
	}
}
void C_Selector::click() {
	if (open) parent->reset_block();
	open = !open;
}