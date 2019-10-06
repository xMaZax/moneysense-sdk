#include "backdrop.h"
std::vector<Dot*> Dots;

void draw::autoskope()
{
	int width, height;

	g_csgo.m_engine()->GetScreenSize(width, height);

	g_csgo.m_surface()->DrawSetColor(Color(0.f, 0.f, 0.f, 0.70f));
	g_csgo.m_surface()->DrawFilledRect(0, 0, width, height);
	int s = rand() % 24;

	if (s == 0)
		Dots.push_back(new Dot(VectorDrop(rand() % (int)width, -16), VectorDrop((rand() % 7) - 3, rand() % 3 + 1)));
	else if (s == 1)
		Dots.push_back(new Dot(VectorDrop(rand() % (int)width, (int)height + 16), VectorDrop((rand() % 7) - 3, -1 * (rand() % 3 + 1))));

	for (auto i = Dots.begin(); i < Dots.end();)
	{
		if ((*i)->RelativePosition.y < -20 || (*i)->RelativePosition.y > height + 20 || (*i)->RelativePosition.x < -20 || (*i)->RelativePosition.x > width + 20)
		{
			delete (*i);
			i = Dots.erase(i);
		}
		else
		{
			(*i)->RelativePosition = (*i)->RelativePosition + (*i)->Velocity * (0.3);
			i++;
		}
	}
	for (auto i = Dots.begin(); i < Dots.end(); i++)
		(*i)->Skope();
}


void draw::backdrop()
{

	int width, height;

	g_csgo.m_engine()->GetScreenSize(width, height);

	g_csgo.m_surface()->DrawSetColor(Color(0.f, 0.f, 0.f, 0.70f));
	g_csgo.m_surface()->DrawFilledRect(0, 0, width, height);
	int s = rand() % 24;

	if (s == 0)
		Dots.push_back(new Dot(VectorDrop(rand() % (int)width, -16), VectorDrop((rand() % 7) - 3, rand() % 3 + 1)));
	else if (s == 1)
		Dots.push_back(new Dot(VectorDrop(rand() % (int)width, (int)height + 16), VectorDrop((rand() % 7) - 3, -1 * (rand() % 3 + 1))));

	for (auto i = Dots.begin(); i < Dots.end();)
	{
		if ((*i)->RelativePosition.y < -20 || (*i)->RelativePosition.y > height + 20 || (*i)->RelativePosition.x < -20 || (*i)->RelativePosition.x > width + 20)
		{
			delete (*i);
			i = Dots.erase(i);
		}
		else
		{
			(*i)->RelativePosition = (*i)->RelativePosition + (*i)->Velocity * (0.3);
			i++;
		}
	}
	for (auto i = Dots.begin(); i < Dots.end(); i++)
		(*i)->Draw();
}
