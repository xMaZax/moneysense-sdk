#include "materials.hpp"
void materials::setup() {
	std::ofstream("csgo\\materials\\simple_ignorez_reflective.vmt") << R"#("VertexLitGeneric"
{

  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       "env_cubemap"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast"             "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	
	std::ofstream("csgo\\materials\\glowOverlay.vmt") << R"#("VertexLitGeneric" {

    "$additive" "1"
    "$envmap" "models/effects/cube_white"
    "$envmaptint" "[1 1 1]"
    "$envmapfresnel" "1"
    "$envmapfresnelminmaxexp" "[0 1 2]"
    "$alpha" "0.8"
})#";

	std::ofstream("csgo\\materials\\simple_regular_reflective.vmt") << R"#("VertexLitGeneric"
{

  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       "env_cubemap"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast"             "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	static IMaterial * materialMetall = g_csgo.m_materialsystem()->FindMaterial("simple_ignorez_reflective", TEXTURE_GROUP_MODEL);
	static IMaterial * materialMetallnZ = g_csgo.m_materialsystem()->FindMaterial("simple_regular_reflective", TEXTURE_GROUP_MODEL);
	static IMaterial* localplayer_glow = g_csgo.m_materialsystem()->FindMaterial("glowOverlay", TEXTURE_GROUP_MODEL);

	std::ofstream("csgo\\materials\\vitality_rubber.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white"
  "$envmap" "env_cubemap"
  "$normalmapalphaenvmapmask" 1
  "$envmapcontrast" 1
  "$model" 1
  "$selfillum" 1
}
)#";
	std::ofstream("csgo\\materials\\vitality_metallic.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez" "0"
  "$envmap" "env_cubemap"
  "$normalmapalphaenvmapmask" "1"
  "$envmapcontrast"  "1"
  "$nofog" "1"
  "$model" "1"
  "$nocull" "0"
  "$selfillum" "1"
  "$halflambert" "1"
  "$znearer" "0"
  "$flat" "1"
}
)#";
}
void materials::remove() {
	std::remove("csgo\\materials\\vitality_regular.vmt");
	std::remove("csgo\\materials\\vitality_rubber.vmt");
	std::remove("csgo\\materials\\vitality_metallic.vmt");
}