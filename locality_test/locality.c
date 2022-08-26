#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

int main() {
  int n = 128;

  FILE *f1 = fopen("data/line1", "wb");
  FILE *f2 = fopen("data/line2", "wb");
  FILE *f3 = fopen("data/line1_avg", "wb");
  FILE *f4 = fopen("data/line2_avg", "wb");
  float avg_1 = 90;
  float avg_2 = 90;
  float factor=.999;

  for (int i = 0; i < n * n; i++) {
    // Measure locality using the traditional Gradient Method
    {
      int x = i % n;
      int y = i / n;
      int dx = n / 2 - x;
      int dy = n / 2 - y;
      float dist = sqrt(dx * dx + dy * dy);
      avg_1=avg_1*factor+dist*(1-factor);
      fprintf(f1, "%f\n", dist);
      fprintf(f3, "%f\n", avg_1);
    }

    // Measure locality using the Hilbert Method
    {
      int x;
      int y;
      d2xy(n, i, &x, &y);
      int dx = n / 2 - x;
      int dy = n / 2 - y;
      float dist = sqrt(dx * dx + dy * dy);
      avg_2=avg_2*factor+dist*(1-factor);
      fprintf(f2, "%f\n", dist);
      fprintf(f4, "%f\n", avg_2);
    }
  }
  fclose(f1);
  fclose(f2);
  fclose(f3);
  fclose(f4);
}
