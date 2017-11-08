#pragma once

#include "sdk/sdk.hpp"

namespace draw
{
	void circle(Vector2D pos, float points, float radius, Color col);
	void filled_circle(Vector2D pos, float points, float radius, Color col);
	void circle3D(Vector pos, float points, float radius, Color col);
	void filled_rectangle(int x0, int y0, int x1, int y1, Color col);
	void filled_rectangle(Vector2D start_pos, Vector2D end_pos, Color col);
	void outlined_rectangle(int x0, int y0, int x1, int y1, Color col);
	void outlined_rectangle(Vector2D start_pos, Vector2D end_pos, Color col);
	void line(int x0, int y0, int x1, int y1, Color col);
	void line(Vector2D start_pos, Vector2D end_pos, Color col);
	void poly_line(int* px, int* py, int num_vertices, Color col);
	void poly_line(Vertex_t* vertices, int num_vertices, Color col);
	void textured_polygon(int num_vertices, Vertex_t* vertices, Color col);
	void text(int x, int y, const char* text, HFont font, Color col);
	void text(Vector2D pos, const char* text, HFont font, Color col);
	Vector2D get_text_size(const char* text, HFont font);
	HFont create_font(const char* font_name, int size, int flag = static_cast<int>(FONTFLAG_DROPSHADOW));
}
