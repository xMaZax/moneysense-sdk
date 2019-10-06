#pragma once
#include "includes.hpp"
#include "includes.hpp"
#include "../sdk\structs.hpp"
#include "../cheats/lagcompensation/lagcompensation.h"

class manual_aa : public singleton< manual_aa > {
private:
	enum {
		SIDE_NONE,
		SIDE_LEFT,
		SIDE_RIGHT,
		SIDE_BACK
	};
	int side = SIDE_NONE;
public:
	void update();
	bool run();
	/*void unhittable_aa_skeet();

	void backwards(CUserCmd* cmd);
	void legit(CUserCmd* cmd);
	void sideways(CUserCmd* cmd);
	void backjitter(CUserCmd* cmd);*/

};

class CCSGOPlayerAnimState;

class antiaim : public singleton< antiaim > {
public:
	bool nade_out(CUserCmd* pUserCmd);
	bool breakinglby;
	void make_desync();
	void balance();
	float GetMaxFakeDelta();
	void create_move();
	float get_max_desync_delta();
	void update_lowerbody_breaker();
	float at_target();
	void freestanding_spin();
	float freestanding();
	bool DesyncRotate(float jitter, int direction, CUserCmd* cmd, ctx_t* ctx);
	void jitter();
	void DesyncAnimations(int type, CGlobalVarsBase* vars, CUserCmd* cmd, ctx_t* ctx);
	float get_pitch();
	void pitchjitter();
	float minimal();
	float balance(int type);
	float get_fake();
	bool antiaim_desync = false;

	float get_real();

	void fix_movement(CUserCmd* cmd);

	bool freeze_check = false;

	void get_unpredicted_velocity();
	player_t* m_e;
	vec3_t unpredicted_velocity;

	bool m_bBreakLowerBody = false;
	float m_flLastLby = 0.0f;
	float m_flNextBodyUpdate = 0.f;
	bool m_bBreakBalance = false;
	float m_flLastMoveTime;
};
class aimbot : public singleton< aimbot > {
private:
	struct target_t {
		float best_distance = 1000000.0f;
		bool backtrack = false;
		int tid = -1;
		vec3_t aimspot;
		float simtime = 0.f;
		tickrecord_t best_record;

		void reset() {
			best_distance = INT_MAX;
			tid = -1;
			simtime = 0.f;
			aimspot.Init();

			backtrack = false;
			best_record;
		}
	};

	struct aimbotconfig_t {
		bool autoshoot;
		bool autorevolver;
		bool autowall;
		bool autoscope;
		bool hitchance;

		int minimum_damage;
		int hitchance_amount;
		int baimdmg;
		std::vector< MultiDropdownItem_t > hitscan;
		std::vector< MultiDropdownItem_t > hitscan_history;
		std::vector< MultiDropdownItem_t > multipoint;
		std::vector< MultiDropdownItem_t > baim;

		float pointscalehead;
		float pointscalepelvis;
		float pointscalestomach;
		float pointscalelegs;
		float pontscalearms;
		float pointscaleneck;
		float pointscalechest;
		float pointscale;
		bool autostop;
		bool quickstop;
		bool autofakewalk;
	};

	weapon_t * local_weapon;
	vec3_t engine_angles;
	float recoil_scale;
public:
	aimbotconfig_t config;
	target_t target_data;
public:
	void create_move();

	void iterate_players();

	void zeus_run();
	void double_tap(CUserCmd* cmd);
	void triggerbot(CUserCmd* cmd);
	
	bool can_zeus(Vector & bestpoint);

	void aim();

	vec3_t best_point(player_t * e, matrix3x4_t bones[128]);

	struct bt_point_return_t {
		tickrecord_t record;
		vec3_t point;
	};

	aimbot::bt_point_return_t best_point_backtrack(player_t * e, player_record_t & player);
	void getpoints(player_t * e, int hitbox_id, matrix3x4_t bones[128], std::vector< vec3_t > & points);

	void quickstop();

	void autostop(player_t * target, bool doinAimbot);

	void owowalk(player_t * target, bool doinAimbot);

	bool hitchance(vec3_t angles, player_t * m_entity, float chance);

	void get_target(player_t * e, float fov, vec3_t location, float simulation_time, bool backtrack, tickrecord_t lag_record);

	bool head_check(player_t * e, int m_fFlags, vec3_t m_vecVelocity);

	std::vector< int > hitboxes_from_vector(player_t * e, int m_fFlags, vec3_t m_vecVelocity, std::vector< MultiDropdownItem_t > arr);

	void update_config();
};
