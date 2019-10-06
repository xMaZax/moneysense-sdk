#include "..\hooks.hpp"

using FindMDL_t = void(__thiscall*)(void*, char*);

void __fastcall hooks::hooked_findmdl(void* ecx, void* edx, char* FilePath) {
	static auto original_fn = modelcache_hook->get_func_address<FindMDL_t>(10);

	if (!g_cfg.misc.model_changer)
		return original_fn(ecx, FilePath);

	if (strstr(FilePath, "arms"))
		sprintf(FilePath, "models//weapons//t_arms_new.mdl");

	if (strstr(FilePath, "ak47.mdl"))
		sprintf(FilePath, "models//weapons//v_rif_ak47_new.mdl");

	if (strstr(FilePath, "m4a1.mdl"))
		sprintf(FilePath, "models//weapons//v_rif_m4a1_new.mdl");

	if (strstr(FilePath, "awp.mdl"))
		sprintf(FilePath, "models//weapons//eminem//dsr_50//v_dsr_50.mdl");

	if (strstr(FilePath, "v_knife_default_ct.mdl") || strstr(FilePath, "v_knife_default_t.mdl"))
	{
		sprintf(FilePath, "models/weapons/v_minecraft_pickaxe.mdl");
	}

	return original_fn(ecx, FilePath);
}