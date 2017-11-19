#pragma once

#include "sdk/sdk.hpp"

namespace draw
{
	void circle(const Vector2D& pos, float points, float radius, const Color& col);
	void filled_circle(const Vector2D& pos, float points, float radius, const Color& col);
	void circle3D(const Vector& pos, float points, float radius, const Color& col);
	void filled_rectangle(int x0, int y0, int x1, int y1, const Color& col);
	void filled_rectangle(const Vector2D& start_pos, const Vector2D& end_pos, const Color& col);
	void outlined_rectangle(int x0, int y0, int x1, int y1, const Color& col);
	void outlined_rectangle(const Vector2D& start_pos, const Vector2D& end_pos, const Color& col);
	void line(int x0, int y0, int x1, int y1, const Color& col);
	void line(const Vector2D& start_pos, const Vector2D& end_pos, const Color& col);
	void poly_line(int* px, int* py, int num_vertices, const Color& col);
	void poly_line(Vertex_t* vertices, int num_vertices, const Color& col);
	void textured_polygon(int num_vertices, Vertex_t* vertices, const Color& col);
	void text(int x, int y, const char* text, HFont font, const Color& col);
	void text(const Vector2D& pos, const char* text, HFont font, const Color& col);
	Vector2D get_text_size(const char* text, HFont font);
	HFont create_font(const char* font_name, int size, int flag = int(FONTFLAG_DROPSHADOW));
}
