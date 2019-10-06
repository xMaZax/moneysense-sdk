#pragma once

#include "..\..\Hack\includes.hpp"
#include "..\..\sdk\structs.hpp"

class weapon_t;
class Box;

class worldesp : public singleton< worldesp > {
public:
	float esp_alpha_fade[64];
	void paint_traverse();

	void bomb_esp(player_t* pt_hook);

	void DrawZeusRange();
	void grenades(weapon_t * m_nade, Box box);
	void drop_bomb(player_t* entity);
	void weapons(weapon_t * m_weapon, Box box);
	player_t * BombCarrier;
};