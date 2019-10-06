#pragma once

#include "..\..\Hack\includes.hpp"

class fakewalk : public singleton< fakewalk > {
public:
	void create_move();
	void fake_duck(CUserCmd* ccmd, bool* bSendPacket);
	void slidewalk();
};