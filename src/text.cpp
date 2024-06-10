#include "text.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-ft.h>
#include <ft2build.h>
#include FT_SFNT_NAMES_H
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BBOX_H
#include FT_TYPE1_TABLES_H

Font::Font(char *filename)
{
    // create font
    FT_Face face;
    FT_Error status;
    FT_Library value;
    // const char * filename = "OpenSans-Light.ttf";
    status = FT_Init_FreeType(&value);
    if (status != 0)
    {
        fprintf(stderr, "Error %d opening library.\n", status);
        exit(EXIT_FAILURE);
    }
    status = FT_New_Face(value, filename, 0, &face);
    if (status != 0)
    {
        fprintf(stderr, "Error %d opening %s.\n", status, filename);
        exit(EXIT_FAILURE);
    }
    cairo_font_face_t *font_ = cairo_ft_font_face_create_for_ft_face(face, 0);
    this->font = font_;
}

Font::~Font() {
    if (font) {
        cairo_font_face_destroy(font);
        FT_Done_Face(face);
        FT_Done_FreeType(library);
    }
}

Font::Font(Font&& other) noexcept
    : font(other.font), face(other.face), library(other.library) {
    other.font = nullptr;
    // It might not be necessary or possible to "nullify" the FT_Face and FT_Library
    // since they're not pointers. Instead, ensure they're not used in a destructed state.
}

Font& Font::operator=(Font&& other) noexcept {
    if (this != &other) {
        if (font) {
            cairo_font_face_destroy(font);
            FT_Done_Face(face);
            FT_Done_FreeType(library);
        }
        font = other.font;
        face = other.face;
        library = other.library;
        other.font = nullptr;
        // Again, resetting FT_Face and FT_Library might not apply
    }
    return *this;
}

Text::Text(std::string &txt, float x, float y, float width, float size, float line_height, float space_size, char *filename, const std::string color, float opacity, bool responsive, char *align, int z_index, Font *font_obj)
{
    this->txt = txt;
    this->x = x;
    this->y = y;
    this->width = width;
    this->line_height = line_height;
    this->space_size = space_size;
    this->size = size;
    this->color = hex_to_rgb(color);
    this->opacity = opacity;
    this->z_index = z_index;
    this->align = parse_alignment(align);
    this->responsive = responsive;
    this->n_lines = 1;
    this->tokens = this->tokenize(this->txt);

    if (font_obj == nullptr)
    {
        // create font
        FT_Face face;
        FT_Error status;
        FT_Library value;
        // const char * filename = "OpenSans-Light.ttf";
        status = FT_Init_FreeType(&value);
        if (status != 0)
        {
            fprintf(stderr, "Error %d opening library.\n", status);
            exit(EXIT_FAILURE);
        }
        status = FT_New_Face(value, filename, 0, &face);
        if (status != 0)
        {
            fprintf(stderr, "Error %d opening %s.\n", status, filename);
            exit(EXIT_FAILURE);
        }
        cairo_font_face_t *font_ = cairo_ft_font_face_create_for_ft_face(
            face, 0);
        this->font = font_;
    }
    else
    {
        this->font = font_obj->font;
    }
}

Text::Text(const Text &element)
{
    this->copy(element);
    this->width = element.width;
    this->size = element.size;
    this->line_height = element.line_height;
    this->char_height = element.char_height;
    this->space_size = element.space_size;
    this->txt = element.txt;
    this->tokens = element.tokens;
    this->color = element.color;
    this->font = element.font;
    this->lines_model = element.lines_model;
    this->word_timelines = element.word_timelines;
    this->line_timelines = element.line_timelines;
    this->word_delay = element.word_delay;
    this->line_delay = element.line_delay;
    this->n_words = element.n_words;
    this->n_lines = element.n_lines;
}

void Text::get_attributes(Attributes &attributes, float t)
{
    attributes["x"] = this->x;
    attributes["y"] = this->y;
    attributes["width"] = this->width;
    attributes["opacity"] = this->opacity;
    attributes["size"] = this->size;
    this->at(attributes, t);

    // get main attributes
    attributes["x"] = this->get_x(attributes["x"]);
    attributes["y"] = this->get_y(attributes["y"]);
    attributes["width"] = this->get_x(attributes["width"]);
    attributes["size"] = this->get_x(attributes["size"]);
}

void Text::on_resize()
{
    // create surface
    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_image_surface_create(
        CAIRO_FORMAT_ARGB32, this->screen_width, this->screen_height);
    cr = cairo_create(surface);

    // compute attributes
    Attributes attributes;
    this->get_attributes(attributes, 0);

    // get values
    float x = attributes["x"];
    float y = attributes["y"];
    float width = attributes["width"];
    float size = attributes["size"];

    // select font and parameterize
    cairo_set_font_face(cr, this->font);
    cairo_set_font_size(cr, size);
    cairo_set_source_rgba(cr, 0, 0, 0, opacity);

    // get height of the character A
    cairo_text_extents_t letter_te;
    cairo_text_extents(cr, "A", &letter_te);
    this->char_height = letter_te.height;

    // get spacings
    float space_size = this->space_size * size;
    float line_height = size * this->line_height;

    // // get line model
    // LinesModel txt = get_text_model(cr, this->align, x, y, this->tokens, width, line_height, space_size);
    // // update n_lines
    // this->n_lines = txt.size();
    // this->lines_model = txt;

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void Text::draw(cairo_t *cr, float t)
{
    // compute attributes
    Attributes attributes;
    this->get_attributes(attributes, t);

    // get values
    float x = attributes["x"];
    float y = attributes["y"];
    float width = attributes["width"];
    float size = attributes["size"];

    // select color
    float r = std::get<0>(this->color);
    float g = std::get<1>(this->color);
    float b = std::get<2>(this->color);

    // select font and parameterize
    cairo_set_font_face(cr, this->font);
    cairo_set_font_size(cr, size);
    cairo_set_source_rgba(cr, r, g, b, opacity);

    // get spacings
    float space_size = this->space_size * size;
    float line_height = size * this->line_height;

    // get line model
    LinesModel txt = get_text_model(cr, this->align, x, y, this->tokens, width, line_height, space_size);

    // draw text
    if (this->word_timelines.size() == 0 && this->line_timelines.size() == 0)
    {
        for (auto &line : txt)
        {
            for (auto &token : line)
            {
                cairo_move_to(cr, token.x, token.y);
                cairo_show_text(cr, token.token.c_str());
            }
        }
    }
    else
    {
        size_t line_id = 0;
        size_t word_id = 0;
        for (auto &line : txt)
        {
            Attributes line_attr;
            line_attr["x"] = 0;
            line_attr["y"] = 0;
            line_attr["opacity"] = 1;
            this->line_at(line_attr, t - line_id * this->line_delay);
            float line_x = this->get_x(line_attr["x"]);
            float line_y = this->get_x(line_attr["y"]);
            float line_o = line_attr["opacity"];
            line_id++;

            if (line_o == 0)
                continue;

            for (auto &token : line)
            {
                Attributes word_attr;
                word_attr["x"] = 0;
                word_attr["y"] = 0;
                word_attr["opacity"] = 1;
                this->word_at(word_attr, t - word_id * this->word_delay);
                float word_x = this->get_x(word_attr["x"]);
                float word_y = this->get_x(word_attr["y"]);
                float word_o = word_attr["opacity"];
                word_id++;

                if (word_o == 0)
                    continue;

                cairo_set_source_rgba(cr, r, g, b, opacity * word_o * line_o);
                cairo_move_to(cr, token.x + word_x + line_x, token.y + word_y + line_y);
                cairo_show_text(cr, token.token.c_str());
            }
        }
    }
}

void Text::animate_words(Timeline &timeline, float delay)
{
    this->word_timelines.push_back(timeline);
    this->word_delay = delay;
}

void Text::animate_lines(Timeline &timeline, float delay)
{
    this->line_timelines.push_back(timeline);
    this->line_delay = delay;
}

float Text::get_end()
{
    float end = 0;
    for (auto &timeline : this->timelines)
    {
        float tmp = timeline.get_end();
        if (end < tmp)
            end = tmp;
    }
    for (auto &timeline : this->word_timelines)
    {
        float tmp = timeline.get_end() + this->word_delay * (this->n_words - 1);
        if (end < tmp)
            end = tmp;
    }
    for (auto &timeline : this->line_timelines)
    {
        float tmp = timeline.get_end() + this->line_delay * (this->n_lines - 1);
        if (end < tmp)
            end = tmp;
    }
    return end;
}

Lines Text::tokenize(std::string txt)
{
    Lines lines;
    std::istringstream ss(txt);
    std::string line;
    size_t n_words = 0;
    size_t n_lines = 0;
    while (std::getline(ss, line, '\n'))
    {
        Tokens tokens;
        std::string token;
        std::istringstream is(line);
        while (std::getline(is, token, ' '))
        {
            if (token.length() == 0)
                continue;
            tokens.push_back(token);
            n_words++;
        }
        lines.push_back(tokens);
        n_lines++;
    }
    this->n_words = n_words;
    return lines;
}

LinesModel get_text_model(cairo_t *cr, layout::position align, float x, float y, Lines &lines, float width, float line_height, float space_size)
{
    // get height of the character A
    cairo_text_extents_t letter_te;
    cairo_text_extents(cr, "A", &letter_te);
    float char_height = letter_te.height;

    LinesModel new_lines;
    LineModel tokens;

    size_t n_lines = 0;
    std::vector<float> lines_width;
    for (auto &line : lines)
    {
        std::cout << "newline" << std::endl;
        float line_width = 0;
        tokens.clear();
        for (auto &word : line)
        {
            cairo_text_extents_t word_te;
            cairo_text_extents(cr, word.c_str(), &word_te);

            // define token
            TokenModel token;
            token.width = word_te.width;
            token.height = word_te.height;
            token.token = word;
            std::cout << "token: " << token.token << std::endl;

            if (token.width + space_size + line_width <= width)
            {
                token.x = line_width;
                token.y = n_lines;
                tokens.push_back(token);
                line_width += token.width + space_size;
            }
            else
            {
                new_lines.push_back(tokens);
                tokens.clear();
                lines_width.push_back(line_width - space_size);
                n_lines++;

                token.x = 0;
                token.y = n_lines;
                tokens.push_back(token);
                line_width = token.width + space_size;
            }
        }
        if (tokens.size() != 0)
        {
            new_lines.push_back(tokens);
            lines_width.push_back(line_width - space_size);
            n_lines++;
        }
    }

    // adapt to alignment
    adapt_text_to_alignment(new_lines, align, lines_width, x, y, char_height, line_height, n_lines);
    return new_lines;
}

void adapt_text_to_alignment(
    LinesModel &new_lines,
    layout::position &align,
    std::vector<float> lines_width,
    float x,
    float y,
    float char_height,
    float line_height,
    size_t n_lines)
{
    size_t line_id = 0;
    for (auto &line : new_lines)
    {
        float line_width = lines_width[line_id];
        for (auto &token : line)
        {
            // x-alignment
            switch (align)
            {
            case layout::position::top:
            case layout::position::center:
            case layout::position::bottom:
                token.x += x - line_width / 2;
                break;
            case layout::position::top_left:
            case layout::position::left:
            case layout::position::bottom_left:
                token.x += x;
                break;
            default:
                token.x += x - line_width;
                break;
            }
            // y-alignment
            float half_n_lines = (((float)n_lines - 1) / 2);
            switch (align)
            {
            case layout::position::top:
            case layout::position::top_left:
            case layout::position::top_right:
                token.y = token.y * (char_height + line_height) + y + char_height;
                break;
            case layout::position::left:
            case layout::position::center:
            case layout::position::right:
                token.y = (char_height + line_height) * (token.y - half_n_lines) + y + char_height / 2;
                break;
            default:
                token.y = (token.y - n_lines + 1) * (char_height + line_height) + y - char_height / 2;
                break;
            }
        }
        line_id++;
    }
}

void Text::word_at(Attributes &attributes, float t)
{
    attributes["t"] = t;
    for (auto &timeline : this->word_timelines)
    {
        timeline.at(attributes);
    }
}

void Text::line_at(Attributes &attributes, float t)
{
    attributes["t"] = t;
    for (auto &timeline : this->line_timelines)
    {
        timeline.at(attributes);
    }
}

LinesBBox Text::get_lines_bbox()
{
    LinesBBox lines_bbox;
    for (auto &line : this->lines_model)
    {
        std::unordered_map<std::string, float> line_bbox;
        float line_x = line[0].x;
        float line_y = line[0].y - this->char_height;
        float line_width = line[line.size() - 1].width + line[line.size() - 1].x - line_x;
        size_t token_id = 0;

        line_bbox["x"] = 100 * line_x / this->screen_width;
        line_bbox["y"] = 100 * line_y / this->screen_height;
        line_bbox["width"] = 100 * line_width / this->screen_width;
        line_bbox["height"] = 100 * this->char_height / this->screen_height;
        lines_bbox.push_back(line_bbox);
    }
    return lines_bbox;
}

void show_lines(Lines lines)
{
    for (auto &line : lines)
    {
        std::cout << "[";
        for (auto &token : line)
        {
            std::cout << "<" << token << ">";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}