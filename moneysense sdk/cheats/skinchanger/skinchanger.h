#pragma once
#include "../../hack/includes.hpp"
class skinchanger : public singleton<skinchanger> {
public:
	bool apply_knife_model(weapon_t* weapon, const char* model);
	bool apply_knife_skin(weapon_t* weapon, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear);
	void run();

	int32_t delta_tick;
	void full_update() {
		delta_tick = -1;
	}
};

class updatechanger : public singleton<updatechanger> {
public:
	void update_skins();
};
