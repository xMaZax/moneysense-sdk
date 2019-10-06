#pragma once

#include "..\..\Hack\includes.hpp"
#include "..\..\sdk\structs.hpp"

class otheresp : public singleton< otheresp > {
public:
	void antiaim_indicator();
	void watermark();
	void penetration_reticle();
	void lby_indicator();
	void status_bar();
	void check_hits();
	void baim_indicator();
	void desyncindicator();
	void spectators();
	void hitmarker_paint();
	void hitmarkerdynamic_paint();
	void hitmarker_event(IGameEvent * event);
	void spread_crosshair(IDirect3DDevice9*);
	void spread_crosshair();
private:
	float hurt_time;
};

struct impact_info {
	float x, y, z;
	long long time;
};

struct hitmarker_info {
	impact_info impact;
	int alpha;
};

extern std::vector<impact_info> impacts;
extern std::vector<hitmarker_info> hitmarkers;