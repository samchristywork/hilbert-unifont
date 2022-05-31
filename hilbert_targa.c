#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// hsv2rgb was taken from
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
  int n = 128 * 2 * 2;

  unsigned char *p = malloc(n * n * 4);
  bzero(p, n * n * 4);

  for (int y = 0; y < n; y++) {
    for (int x = 0; x < n; x++) {
      float value = (float)xy2d(n, x, y) / (n * n);
      RGBColor color = hsv2rgb(value, 1., 1.);
      p[(x + y * n) * 4 + 0] = color.r;
      p[(x + y * n) * 4 + 1] = color.g;
      p[(x + y * n) * 4 + 2] = color.b;
      p[(x + y * n) * 4 + 3] = 255;
    }
  }

  FILE *f = fopen("hilbert_curve.tga", "wb");

  unsigned char header[18];
  bzero(header, sizeof(header));
  header[2] = 2;
  header[12] = 255 & n;
  header[13] = 255 & (n >> 8);
  header[14] = 255 & n;
  header[15] = 255 & (n >> 8);
  header[16] = 32;
  header[17] = 32;
  fwrite(header, sizeof(header), 1, f);
  fwrite(p, n * n * 4, 1, f);
  free(p);
  fclose(f);
}
