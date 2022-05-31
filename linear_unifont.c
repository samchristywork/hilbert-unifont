#include <cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// https://stackoverflow.com/questions/38491380/how-to-print-unicode-codepoints-as-characters-in-c
unsigned convert(u_int8_t *utf8, u_int32_t codepoint) {
  if (codepoint <= 0x7F) {
    utf8[0] = codepoint;
    return 1;
  }
  if (codepoint <= 0x7FF) {
    utf8[0] = 0xC0 | (codepoint >> 6);
    utf8[1] = 0x80 | (codepoint & 0x3F);
    return 2;
  }
  if (codepoint <= 0xFFFF) {
    if (codepoint >= 0xD800 && codepoint <= 0xDFFF) {
      return 0;
    }
    utf8[0] = 0xE0 | (codepoint >> 12);
    utf8[1] = 0x80 | ((codepoint >> 6) & 0x3F);
    utf8[2] = 0x80 | (codepoint & 0x3F);
    return 3;
  }
  if (codepoint <= 0x10FFFF) {
    utf8[0] = 0xF0 | (codepoint >> 18);
    utf8[1] = 0x80 | ((codepoint >> 12) & 0x3F);
    utf8[2] = 0x80 | ((codepoint >> 6) & 0x3F);
    utf8[3] = 0x80 | (codepoint & 0x3F);
    return 4;
  }
  return 0;
}

// hsv2rgb taken from
// https://github.com/Inseckto/HSV-to-RGB/blob/master/HSV2RGB.c

typedef struct RGBColor {
  int r;
  int g;
  int b;
} RGBColor;

RGBColor hsv2rgb(float h, float s, float v) {
  float r, g, b;

  int i = floor(h * 6);
  float f = h * 6 - i;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);

  switch (i % 6) {
  case 0:
    r = v, g = t, b = p;
    break;
  case 1:
    r = q, g = v, b = p;
    break;
  case 2:
    r = p, g = v, b = t;
    break;
  case 3:
    r = p, g = q, b = v;
    break;
  case 4:
    r = t, g = p, b = v;
    break;
  case 5:
    r = v, g = p, b = q;
    break;
  }

  RGBColor color;
  color.r = r * 255;
  color.g = g * 255;
  color.b = b * 255;

  return color;
}

int main() {
  int font_size = 16;
  int width = font_size * (0xff + 2);
  int height = font_size * (0xff + 2);
  cairo_surface_t *surface =
      cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *cr = cairo_create(surface);

  cairo_select_font_face(cr, "Unifont", 0, 0);

  cairo_set_source_rgb(cr, 0., 0., 0.);
  cairo_move_to(cr, 0, 0);
  cairo_line_to(cr, width, 0);
  cairo_line_to(cr, width, height);
  cairo_line_to(cr, 0, height);
  cairo_fill(cr);

  cairo_set_font_size(cr, font_size);

  for (int offset = 0; offset < 0xff; offset++) {
    for (u_int32_t code = 0; code <= 0xff; code++) {
      u_int8_t utf8[4];

      RGBColor color =
          hsv2rgb((float)(code + offset * 0x100) / (0xff * 0xff), 1, 1);
      cairo_set_source_rgb(cr, (float)color.r / 255., (float)color.g / 255.,
                           (float)color.b / 255.);

      unsigned len = convert(utf8, code + offset * 0x100);
      if (len == 0) {
        fprintf(stderr, "Decode failure.\n");
      } else {
        char buf[5] = {0};
        memcpy(buf, utf8, len);
        cairo_move_to(cr, code * font_size, font_size + font_size * offset);
        cairo_show_text(cr, buf);
        cairo_fill(cr);
      }
    }
  }

  cairo_surface_write_to_png(surface, "linear_unifont.png");
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}
