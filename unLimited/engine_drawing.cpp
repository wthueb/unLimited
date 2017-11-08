#include "engine_drawing.hpp"

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "utils.hpp"

void draw::circle(Vector2D pos, float points, float radius, Color col)
{
	auto step = static_cast<float>(M_PI * 2.f / points);

	for (auto a = 0.f; a < M_PI * 2.f; a += step)
	{
		Vector2D start{ radius * cosf(a) + pos.x, radius * sinf(a) + pos.y };
		Vector2D end{ radius * cosf(a + step) + pos.x, radius * sinf(a + step) + pos.y };
		line(start, end, col);
	}
}

void draw::filled_circle(Vector2D pos, float points, float radius, Color col)
{
	std::vector<Vertex_t> vertices;
	auto step = static_cast<float>(M_PI * 2.f / points);

	for (auto a = 0.f; a < M_PI * 2.f; a += step)
		vertices.push_back(Vertex_t{ { radius * cosf(a) + pos.x, radius * sinf(a) + pos.y } });

	textured_polygon(static_cast<int>(points), vertices.data(), col);
}

void draw::circle3D(Vector pos, float points, float radius, Color col)
{
	float step = static_cast<float>(M_PI * 2.0f / points);

	std::vector<Vector> points3d;

	for (auto a = 0.f; a < M_PI * 2.f; a += step)
	{
		Vector start(radius * cosf(a) + pos.x, radius * sinf(a) + pos.y, pos.z);
		Vector end(radius * cosf(a + step) + pos.x, radius * sinf(a + step) + pos.y, pos.z);

		Vector start2d, end2d;
		if (g_debug_overlay->ScreenPosition(start, start2d) || g_debug_overlay->ScreenPosition(end, end2d))
			return;

		line(Vector2D(start2d.x, start2d.y), Vector2D(end2d.x, end2d.y), col);
	}
}

void draw::filled_rectangle(int x0, int y0, int x1, int y1, Color col)
{
	g_surface->DrawSetColor(col);
	g_surface->DrawFilledRect(x0, y0, x1, y1);
}

void draw::filled_rectangle(Vector2D start_pos, Vector2D end_pos, Color col)
{
	filled_rectangle(static_cast<int>(start_pos.x), static_cast<int>(start_pos.y), static_cast<int>(end_pos.x), static_cast<int>(end_pos.y), col);
}

void draw::outlined_rectangle(int x0, int y0, int x1, int y1, Color col)
{
	g_surface->DrawSetColor(col);
	g_surface->DrawOutlinedRect(x0, y0, x1, y1);
}

void draw::outlined_rectangle(Vector2D start_pos, Vector2D end_pos, Color col)
{
	outlined_rectangle(static_cast<int>(start_pos.x), static_cast<int>(start_pos.y), static_cast<int>(end_pos.x), static_cast<int>(end_pos.y), col);
}

void draw::line(int x0, int y0, int x1, int y1, Color col)
{
	g_surface->DrawSetColor(col);
	g_surface->DrawLine(x0, y0, x1, y1);
}

void draw::line(Vector2D start_pos, Vector2D end_pos, Color col)
{
	line(static_cast<int>(start_pos.x), static_cast<int>(start_pos.y), static_cast<int>(end_pos.x), static_cast<int>(end_pos.y), col);
}

void draw::poly_line(int* px, int* py, int num_points, Color col)
{
	g_surface->DrawSetColor(col);
	g_surface->DrawPolyLine(px, py, num_points);
}

void draw::poly_line(Vertex_t* vertices, int num_vertices, Color col)
{
	int* points_x = new int[num_vertices];
	int* points_y = new int[num_vertices];

	for (auto i = 0; i < num_vertices; ++i)
	{
		points_x[i] = static_cast<int>(vertices[i].m_Position.x);
		points_y[i] = static_cast<int>(vertices[i].m_Position.y);
	}

	poly_line(points_x, points_y, num_vertices, col);

	delete[] points_x;
	delete[] points_y;
}

void draw::textured_polygon(int num_vertices, Vertex_t* vertices, Color col)
{
	static int texture_id = g_surface->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };

	g_surface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
	g_surface->DrawSetColor(col);
	g_surface->DrawSetTexture(texture_id);

	g_surface->DrawTexturedPolygon(num_vertices, vertices);
}

void draw::text(int x, int y, const char* text, HFont font, Color col)
{
	std::wstring wstr = utils::to_wstring(text);

	g_surface->DrawSetTextPos(x, y);
	g_surface->DrawSetTextFont(font);
	g_surface->DrawSetTextColor(col);

	g_surface->DrawPrintText(wstr.c_str(), wcslen(wstr.c_str()));
}

void draw::text(Vector2D pos, const char* text, HFont font, Color col)
{
	draw::text(static_cast<int>(pos.x), static_cast<int>(pos.y), text, font, col);
}

Vector2D draw::get_text_size(const char* text, HFont font)
{
	std::wstring wstr = utils::to_wstring(text);

	int x, y;

	g_surface->GetTextSize(font, wstr.c_str(), x, y);

	return Vector2D{ static_cast<float>(x), static_cast<float>(y) };
}

HFont draw::create_font(const char* font_name, int size, int flag)
{
	HFont new_font = g_surface->CreateFont();

	g_surface->SetFontGlyphSet(new_font, font_name, size, 0, 0, 0, flag);

	return new_font;
}