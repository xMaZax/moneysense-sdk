#pragma once

#include "..\Hack\includes.hpp"
#include "singleton.h"


struct colorc {
	int a, r, g, b;
	int red, green, blue, alpha;

	colorc() = default;
	colorc(int r, int g, int b, int a = 255) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	colorc(uint32_t color) {
		this->a = (color >> 24) & 0xff;
		this->r = (color >> 16) & 0xff;
		this->g = (color >> 8) & 0xff;
		this->b = (color & 0xff);
	}
	colorc from_uint(uint32_t uint) {
		return colorc(uint);
	}
	static colorc Black(int a = 255) { return { 0, 0, 0, a }; }
	static colorc Grey(int  a = 255) { return { 127, 127, 127, a }; }
	static colorc White(int a = 255) { return { 255, 255, 255, a }; }
	static colorc Red(int   a = 255) { return { 255, 0, 0, a }; }
	static colorc Green(int a = 255) { return { 0, 255, 0, a }; }
	static colorc Blue(int  a = 255) { return { 0, 0, 255, a }; }
	D3DCOLOR from_color(colorc col) {
		return D3DCOLOR_ARGB(col.a, col.r, col.g, col.b);
	}
};

enum FontCenteringFlags {
	HFONT_CENTERED_NONE = (1 << 0),
	HFONT_CENTERED_X = (1 << 1),
	HFONT_CENTERED_Y = (1 << 2),
	HFONT_OFF_SCREEN,
};

enum GradientType {
	GRADIENT_HORIZONTAL = 0,
	GRADIENT_VERTICAL
};

enum FONT_INDEX {
	SKOPECROSSHAIR,
	AIMTABICON,
	rolexTEXT,
	rolexBOLD,
	ESPFLAG,
	TAHOMA12,
	AIMWAREFONT,
	IFUCKYOU,
	MENUNAME,
	VERDANA12,
	VERDANA12OUT,
	VERDANA12BOLD,
	VERDANA12BOLDOUT,
	ESPNAME,
	ESPHEALTH,
	WEAPONICONS,
	GRENADEICONS,
	BOMBICONS,
	SUBTABWEAPONS,
	INDICATORFONT,
	INDICATORFONT2,
	TABFONT,
	rolexFONT,
	SLIDERFONT,
	CATICON,
	NYAFONT,
	FONT_MAX,
	BIGVERMIN,
	CTAB_MIN,
	WATERMARK,
	ICONS,
	CTAB,
	keybind_font,
	GROUPBOX,
	weapons_big
};

extern vgui::HFont fonts[FONT_MAX];

template<typename T>
class singleton;

struct CUSTOMVERTEX {
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD color;
};

class render : public singleton< render > {
private:
	float alpha_factor = 1.f;
	LPDIRECT3DDEVICE9 m_device;
	D3DVIEWPORT9      m_viewport;
public:
	bool initalized = false;
	void setup();

	float get_alphafactor();
	void set_alphafactor(float factor);
	void rect(int x, int y, int w, int h, Color color);
	void rect_filled(int x, int y, int w, int h, Color color);
	void setup_states() const;
	void initalize_objects(LPDIRECT3DDEVICE9 m_device);
	void invalidate_objects();
	void draw_text_stdstring(int x, int y, unsigned long font, std::string string, bool text_centered, colorc colour);
	void nonamegey(int x, int y, Color color, DWORD font, std::string Input);
	void textgay(const wchar_t* buf, int pos_x, int pos_y, Color color, vgui::HFont font, DWORD flags);
	void restore_objects(LPDIRECT3DDEVICE9 m_device);
	void circle_dual_colour(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9 * m_device);
	void circle_dual_colour(float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2, IDirect3DDevice9 * m_device);
	void gradient(int x, int y, int w, int h, Color first, Color second, GradientType type);
	void rounded_box(int x, int y, int w, int h, int points, int radius, Color color);
	void circle(int x, int y, int points, int radius, Color color);
	void circle_filled(int x, int y, int points, int radius, Color color);
	void draw_wide_string(bool center, unsigned long font, int x, int y, Color c, wchar_t* str);
	void triangle(Vector2D point_one, Vector2D point_two, Vector2D point_three, Color color);
	void draw_text(float x, float y, const char* msg, Color color);
	void line(int x, int y, int x2, int y2, Color color);
	void text(vgui::HFont font, int x, int y, Color color, DWORD flags, const char* msg, ...);
	void wtext(vgui::HFont font, int x, int y, Color color, DWORD flags, wchar_t* msg);
	float text_width(vgui::HFont font, const char* msg, ...);
	RECT viewport();
};