#pragma once

#include "..\..\hack\includes.hpp"
#include "..\..\sdk\structs.hpp"

class legitbot : public singleton< legitbot >
{
public:
	void create_move();

	void legit_resolver(CUserCmd* cmd);
	void legit_aa(CUserCmd* pCmd);
private:
	bool Aim();
	player_t* GetEnemyToCrosshair(Vector& viewAngles);
	void Smooth(Vector& viewAngles, Vector& angle, float smoothvalue);
	void sync();


	float fov;
	float smooth;
	bool rcs_enable;
	float rcs_ammount;
	bool smoke_check;
	bool flash_check;
	bool jump_check;
	bool scope_check;
	bool silient;
	float silient_fov;
};