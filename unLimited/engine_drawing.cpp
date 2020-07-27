#include "engine_drawing.hpp"

#include <vector>

#include "utils.hpp"

static const auto pi = acos(-1);

void draw::circle(const Vector2D& pos, float points, float radius, const Color& col)
{
    auto step = float(pi * 2.f / points);
    
    for (auto a = 0.f; a < pi * 2.f; a += step)
    {
        Vector2D start{ radius * cosf(a) + pos.x, radius * sinf(a) + pos.y };
        Vector2D end{ radius * cosf(a + step) + pos.x, radius * sinf(a + step) + pos.y };
        line(start, end, col);
    }
}

void draw::filled_circle(const Vector2D& pos, float points, float radius, const Color& col)
{
    std::vector<Vertex_t> vertices{};
    auto step = float(pi * 2.f / points);

    for (auto a = 0.f; a < pi * 2.f; a += step)
        vertices.push_back(Vertex_t{ { radius * cosf(a) + pos.x, radius * sinf(a) + pos.y } });

    textured_polygon(int(points), vertices.data(), col);
}

void draw::circle3D(const Vector& pos, float points, float radius, const Color& col)
{
    float step = float(pi * 2.f / points);

    std::vector<Vector> points3d{};

    for (auto a = 0.f; a < pi * 2.f; a += step)
    {
        Vector start{ radius * cosf(a) + pos.x, radius * sinf(a) + pos.y, pos.z };
        Vector end{ radius * cosf(a + step) + pos.x, radius * sinf(a + step) + pos.y, pos.z };

        Vector start2d, end2d;
        if (g_debug_overlay->ScreenPosition(start, start2d) || g_debug_overlay->ScreenPosition(end, end2d))
            return;

        line(Vector2D{ start2d.x, start2d.y }, Vector2D{ end2d.x, end2d.y }, col);
    }
}

void draw::filled_rectangle(int x0, int y0, int x1, int y1, const Color& col)
{
    g_surface->DrawSetColor(col);
    g_surface->DrawFilledRect(x0, y0, x1, y1);
}

void draw::filled_rectangle(const Vector2D& start_pos, const Vector2D& end_pos, const Color& col)
{
    filled_rectangle(int(start_pos.x), int(start_pos.y), int(end_pos.x), int(end_pos.y), col);
}

void draw::outlined_rectangle(int x0, int y0, int x1, int y1, const Color& col)
{
    g_surface->DrawSetColor(col);
    g_surface->DrawOutlinedRect(x0, y0, x1, y1);
}

void draw::outlined_rectangle(const Vector2D& start_pos, const Vector2D& end_pos, const Color& col)
{
    outlined_rectangle(int(start_pos.x), int(start_pos.y), int(end_pos.x), int(end_pos.y), col);
}

void draw::line(int x0, int y0, int x1, int y1, const Color& col)
{
    g_surface->DrawSetColor(col);
    g_surface->DrawLine(x0, y0, x1, y1);
}

void draw::line(const Vector2D& start_pos, const Vector2D& end_pos, const Color& col)
{
    line(int(start_pos.x), int(start_pos.y), int(end_pos.x), int(end_pos.y), col);
}

void draw::poly_line(int* px, int* py, int num_points, const Color& col)
{
    g_surface->DrawSetColor(col);
    g_surface->DrawPolyLine(px, py, num_points);
}

void draw::poly_line(Vertex_t* vertices, int num_vertices, const Color& col)
{
    int* points_x = new int[num_vertices];
    int* points_y = new int[num_vertices];

    for (auto i = 0; i < num_vertices; ++i)
    {
        points_x[i] = int(vertices[i].m_Position.x);
        points_y[i] = int(vertices[i].m_Position.y);
    }

    poly_line(points_x, points_y, num_vertices, col);

    delete[] points_x;
    delete[] points_y;
}

void draw::textured_polygon(int num_vertices, Vertex_t* vertices, const Color& col)
{
    static int texture_id = g_surface->CreateNewTextureID(true);
    static unsigned char buf[4] = { 255, 255, 255, 255 };

    g_surface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
    g_surface->DrawSetColor(col);
    g_surface->DrawSetTexture(texture_id);

    g_surface->DrawTexturedPolygon(num_vertices, vertices);
}

void draw::text(int x, int y, const char* text, HFont font, const Color& col)
{
    std::wstring wstr = utils::to_wstring(text);

    g_surface->DrawSetTextPos(x, y);
    g_surface->DrawSetTextFont(font);
    g_surface->DrawSetTextColor(col.to_int());

    g_surface->DrawPrintText(wstr.c_str(), int(wcslen(wstr.c_str())));
}

void draw::text(const Vector2D& pos, const char* text, HFont font, const Color& col)
{
    draw::text(int(pos.x), int(pos.y), text, font, col);
}

Vector2D draw::get_text_size(const char* text, HFont font)
{
    std::wstring wstr = utils::to_wstring(text);

    int x, y;
    g_surface->GetTextSize(font, wstr.c_str(), x, y);

    return Vector2D{ float(x), float(y) };
}

HFont draw::create_font(const char* font_name, int size, int flag)
{
    HFont new_font = g_surface->CreateFont();

    g_surface->SetFontGlyphSet(new_font, font_name, size, 0, 0, 0, flag);

    return new_font;
}
