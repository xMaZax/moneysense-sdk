#pragma once

#include "../../hack/includes.hpp"
#include "..\..\sdk\structs.hpp"

class Box;

class playeresp : public singleton< playeresp > {
public:
	float esp_alpha_fade[64];
	void paint_traverse();

	void draw_box(player_t* m_entity, Box box);
	void beams(vec3_t src, Color color);
	void antiaim_lines(player_t * enitiy);
	void draw_health(player_t* m_entity, Box box);
	void draw_ammobar(player_t* m_entity, Box box);
	void draw_flashbar(player_t* m_entity, Box box);
	void draw_name(player_t* m_entity, Box box);
	void draw_weapon(player_t* m_entity, Box box);
	void draw_weapon_name(player_t* m_entity, Box box);
	void draw_flags(player_t * e, Box box);
	void draw_skeleton(player_t * e, Color color, matrix3x4_t matrix[128]);
	void draw_lbybar(player_t * m_entity, Box box);
	void pov_arrow();
};