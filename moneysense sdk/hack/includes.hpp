#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <psapi.h>
#include <sstream>
#include <d3d9.h>
#include <d3dx9.h>
#include "console_hook.h"
#include "../utils\util.hpp"
#include "../utils\csgo.hpp"
#include "../utils\draw_manager.h"
#include "../utils\ctx.hpp"
#include "../utils\netmanager.hpp"
#include "../utils\math.hpp"
#include "../utils\singleton.h"

#include "../hooks\hooks.hpp"

#include "../menu\cmenu.hpp"

#include "../configs\settings.h"

#include "../resources\materials.hpp"
#include "../resources\custom_sounds.hpp"

#include "../sdk\math\Vector.hpp"
#include "../sdk\math\VMatrix.hpp"

#include "../sdk\misc\UtlVector.hpp"
#include "../sdk\misc\EHandle.hpp"
#include "../sdk\misc\CUserCmd.hpp"
#include "../sdk\misc\Color.hpp"
#include "../sdk\misc\KeyValues.hpp"
#include "../sdk\misc\datamap.h"

#include "../sdk\interfaces\IClientEntity.hpp"

#include "../sdk\structs.hpp"

#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "d3dx9.lib") 

extern bool unload;