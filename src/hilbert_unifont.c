#include <cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// Next three functions taken from
// https://en.wikipedia.org/wiki/Hilbert_curve

// rotate/flip a quadrant appropriately
void rot(int n, int *x, int *y, int rx, int ry) {
  if (ry == 0) {
    if (rx == 1) {
      *x = n - 1 - *x;
      *y = n - 1 - *y;
    }

    // Swap x and y
    int t = *x;
    *x = *y;
    *y = t;
  }
}

// convert (x,y) to d
int xy2d(int n, int x, int y) {
  int rx, ry, s, d = 0;
  for (s = n / 2; s > 0; s /= 2) {
    rx = (x & s) > 0;
    ry = (y & s) > 0;
    d += s * s * ((3 * rx) ^ ry);
    rot(n, &x, &y, rx, ry);
  }
  return d;
}

// convert d to (x,y)
void d2xy(int n, int d, int *x, int *y) {
  int rx, ry, s, t = d;
  *x = *y = 0;
  for (s = 1; s < n; s *= 2) {
    rx = 1 & (t / 2);
    ry = 1 & (t ^ rx);
    rot(s, x, y, rx, ry);
    *x += s * rx;
    *y += s * ry;
    t /= 4;
  }
}

// codepoint_convert taken from
// https://stackoverflow.com/questions/38491380/how-to-print-unicode-codepoints-as-characters-in-c

unsigned codepoint_convert(u_int8_t *utf8, u_int32_t codepoint) {
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

  char *fonts[] = {"Unifont",    "Courier",          "Noto Sans Hebrew",
                   "Noto Sans",  "Liberation Serif", "DejaVu Math TeX Gyre",
                   "DejaVu Sans"};

  // This is the index of the font we will be using from the above list.
  int font_index = 0;

  // This is the length in characters of each side of the symbol space.
  int n = 0xff;

  // A font size of 16 allows for perfect representation of Unifont's glyphs.
  int font_size = 16;

  int charWidth = n;
  int charHeight = n;
  int width = font_size * (charWidth + 2);
  int height = font_size * (charHeight + 2);

  cairo_surface_t *surface =
      cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *cr = cairo_create(surface);

  cairo_select_font_face(cr, fonts[font_index], 0, 0);
  cairo_set_font_size(cr, font_size);

  // Draw a black background.
  cairo_set_source_rgb(cr, 0., 0., 0.);
  cairo_move_to(cr, 0, 0);
  cairo_line_to(cr, width, 0);
  cairo_line_to(cr, width, height);
  cairo_line_to(cr, 0, height);
  cairo_fill(cr);

  // Draw each glyph.
  for (int i = 0; i < n * n; i++) {
    RGBColor color = hsv2rgb((float)i / ((float)n * (float)n), .8, 1);
    cairo_set_source_rgb(cr, (float)color.r / 255., (float)color.g / 255.,
                         (float)color.b / 255.);
    int x;
    int y;
    d2xy(n, i, &x, &y);
    u_int8_t utf8[4] = {0};
    char buf[10];
    int len = codepoint_convert(utf8, i);
    sprintf(buf, "%s", utf8);
    if(len){
      cairo_move_to(cr, x * font_size, font_size + font_size * y);
      cairo_show_text(cr, buf);
    }
  }

  // Cleanup
  cairo_surface_write_to_png(surface, "hilbert_unifont.png");
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}
