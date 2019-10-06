#pragma once

#include <vector>
#include "../sdk/math/vector_fix.hpp"
#include "../utils/csgo.hpp"  //#include "sdk/interfaces/ISurface.hpp"
#include "../cheats/visuals/other_esp.h"
#include "../sdk/interfaces/IVEngineClient.hpp"
class Dot;

extern std::vector<Dot*> Dots;

class Dot
{
public:
	Dot(VectorDrop p, VectorDrop v)
	{
		RelativePosition = p;
		Velocity = v;

	}
	virtual void Skope()
	{
		//g_csgo.m_surface()->DrawSetColor(Color(255, 255, 255, 255));
		render::get().text(fonts[AIMTABICON], RelativePosition.x - 2, RelativePosition.y - 2, Color(255, 255, 255, 255), HFONT_CENTERED_X, "A");
		//g_csgo.m_surface()->DrawFilledRect(RelativePosition.x - 2, RelativePosition.y - 2, RelativePosition.x + 2, RelativePosition.y + 2);
		auto t = std::find(Dots.begin(), Dots.end(), this);
		if (t == Dots.end()) return;
		for (auto i = t; i != Dots.end(); i++)
		{
			if ((*i) == this) continue;
			VectorDrop Pos = RelativePosition;
			float Dist = Pos.DistToSqr((*i)->RelativePosition);
			if (Dist < 143)
			{
				VectorDrop Dir = ((*i)->RelativePosition - RelativePosition).NormalizedDrop();
				//g_pSurface->DrawSetColor(Color(69, 140, 230, 255));
				//render::get().line(Pos.x, Pos.y, (Pos + Dir * Dist).x, (Pos + Dir * Dist).y, Color(255, 255, 255, 255));
				//g_csgo.m_surface()->Line(Pos.x, Pos.y, (Pos + Dir * Dist).x, (Pos + Dir * Dist).y, Color(255, 111, 15, 255));

			}
		}
	}
	virtual void Draw()
	{
		g_csgo.m_surface()->DrawSetColor(Color(255, 255, 255, 255));
		g_csgo.m_surface()->DrawFilledRect(RelativePosition.x - 2, RelativePosition.y - 2, RelativePosition.x + 2, RelativePosition.y + 2);
		auto t = std::find(Dots.begin(), Dots.end(), this);
		if (t == Dots.end()) return;
		for (auto i = t; i != Dots.end(); i++)
		{
			if ((*i) == this) continue;
			VectorDrop Pos = RelativePosition;
			float Dist = Pos.DistToDrop((*i)->RelativePosition);
			if (Dist < 128)
			{
				VectorDrop Dir = ((*i)->RelativePosition - RelativePosition).NormalizedDrop();
				//g_pSurface->DrawSetColor(Color(69, 140, 230, 255));
				render::get().line(Pos.x, Pos.y, (Pos + Dir * Dist).x, (Pos + Dir * Dist).y, Color(255, 255, 255, 255));
				//g_csgo.m_surface()->Line(Pos.x, Pos.y, (Pos + Dir * Dist).x, (Pos + Dir * Dist).y, Color(255, 111, 15, 255));

			}
		}
	}

	VectorDrop RelativePosition = VectorDrop(0, 0);
	VectorDrop Velocity;
};

namespace draw
{
	void backdrop();
	void autoskope();
}