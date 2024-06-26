#include <cairo.h>
#include "element.hpp"
#include "utils.hpp"
#include <ft2build.h>
#include FT_SFNT_NAMES_H
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BBOX_H
#include FT_TYPE1_TABLES_H

// ============================================================================
// Models
// ============================================================================
struct TokenModel
{
    float x;
    float y;
    float width;
    float height;
    std::string token;
};

using LineModel = std::vector<TokenModel>;
using LinesModel = std::vector<LineModel>;

using Tokens = std::vector<std::string>;
using Lines = std::vector<Tokens>;

using LinesBBox = std::vector<std::unordered_map<std::string, float>>;

// ============================================================================
// Elements
// ============================================================================
class Font
{
public:
    Font(char *filename);
    // Destructor to release resources
    ~Font();
    // Copy constructor
    Font(const Font& other) = delete; // Disabling copy operations
    // Copy assignment operator
    Font& operator=(const Font& other) = delete; // Disabling copy operations

    // Move constructor
    Font(Font&& other) noexcept;
    // Move assignment operator
    Font& operator=(Font&& other) noexcept;

    cairo_font_face_t *font;
    FT_Face face; // FreeType face
    FT_Library library; // FreeType library
};

class Text : public Element
{
public:
    Text(std::string &txt, float x, float y, float width, float size, float line_height, float space_size, char *filename, std::string color, float opacity, bool responsive, char *align, int z_index, Font *font_obj);
    Text(const Text &element);
    Text *clone() override { return new Text(*this); }
    // ~Text();

    float get_end() override;
    void on_resize() override;
    void get_attributes(Attributes &attributes, float t) override;
    void draw(cairo_t *cr, float t) override;

    Lines tokenize(std::string txt);
    void animate_words(Timeline &timeline, float delay);
    void animate_lines(Timeline &timeline, float delay);
    void word_at(Attributes &attributes, float t);
    void line_at(Attributes &attributes, float t);
    LinesBBox get_lines_bbox();

private:
    float width;
    float size;
    float line_height;
    float char_height;
    float space_size;
    std::string txt;
    Lines tokens;
    Color color;
    cairo_font_face_t *font;
    LinesModel lines_model;

    std::vector<Timeline> word_timelines;
    float word_delay;
    size_t n_words;

    std::vector<Timeline> line_timelines;
    float line_delay;
    size_t n_lines;
};

LinesModel get_text_model(cairo_t *cr, layout::position align, float x, float y, Lines &lines, float width, float line_height, float space_size);
void adapt_text_to_alignment(
    LinesModel &new_lines,
    layout::position &align,
    std::vector<float> lines_width,
    float x,
    float y,
    float char_height,
    float line_height,
    size_t n_lines);
void show_lines(Lines lines);