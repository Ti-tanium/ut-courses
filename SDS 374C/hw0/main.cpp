#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initialize(float** arr, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      arr[i][j] = random()/(float) RAND_MAX;
    }
  }
}

void smooth(float** x, float** y, int n, float a, float b, float c) {
  for (int i = 1; i < n - 1; i++) {
    for (int j = 1; j < n - 1; j++) {
      y[i][j] = a * (x[i-1][j-1] + x[i-1][j+1] + x[i+1][j-1] + x[i+1][j+1]) +
                b * (x[i-1][j] + x[i+1][j] + x[i][j-1] + x[i][j+1]) +
                c * x[i][j];
    }
  }
}

void printMatrix(float** x, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      printf("%f ", x[i][j]);
    printf("\n");
  }
}

void count(float** arr, int n, float t, int &cnt) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      if (arr[i][j] < t) cnt++;
  }
}

int main() {
  const float a = 0.05, b = 0.1, c = 0.4;
  const float t = 0.1;
  const long n = 98306;
  // int n = 10;

  clock_t start = clock();
  float** x = (float**)malloc(n * sizeof(float*));
  for (int i = 0; i < n; i++)
    x[i] = (float*)malloc(n * sizeof(float));
  clock_t end = clock();
  float t_alloc_x = (end - start) / (float) CLOCKS_PER_SEC;

  start = clock();
  float** y = (float**)malloc(n * sizeof(float*));
  for (int i = 0; i < n; i++)
    y[i] = (float*)malloc(n * sizeof(float));
  end = clock();
  float t_alloc_y = (end - start) / (float) CLOCKS_PER_SEC;

  start = clock();
  initialize(x, n);
  end = clock();
  float t_init_x = (end - start) / (float) CLOCKS_PER_SEC;

  start = clock();
  smooth(x, y, n, a, b, c);
  end = clock();
  float t_smooth = (end - start) / (float) CLOCKS_PER_SEC;

  int cnt_x = 0, cnt_y = 0;

  start = clock();
  count(x, n, t, cnt_x);
  end = clock();
  float t_count_x = (end - start) / (float) CLOCKS_PER_SEC;

  start = clock();
  count(y, n, t, cnt_y);
  end = clock();
  float t_count_y = (end - start) / (float) CLOCKS_PER_SEC;

  printf("Summarry\n--------\n");
  printf("Number of elements in a row/column        :: %10d\n", n);
  printf("Number of inner elements in a row/column  :: %10d\n", n - 2);
  printf("Total number of elements                  :: %10d\n", n * n);
  printf("Total number of inner elements            :: %10d\n", (n - 2) * (n - 2));
  printf("Memory (GB) used per array                :: %10f\n", n * n * 4 / ((float) 1024 * 1024 * 1024));
  printf("Threshold                                 :: %10f\n", t);
  printf("Smoothing constants (a, b, c)             :: %3f %4f %3f\n", a, b, c);
  printf("Number of elements below threshold (X)    :: %10d\n", cnt_x);
  printf("Fraction of elements below threshold      :: %10f\n", cnt_x / (float) (n * n));
  printf("Number of elements below threshold (Y)    :: %10d\n", cnt_y);
  printf("Fraction of elements below threshold      :: %10f\n", cnt_y / (float) ((n-2)*(n-2)));
  printf("Action          ::      time/s     Time resolution = 1.0E-04\n-------------\n");
  printf("CPU: Alloc-X    ::      %5f\n", t_alloc_x);
  printf("CPU: Alloc-Y    ::      %5f\n", t_alloc_y);
  printf("CPU: Init-X     ::      %5f\n", t_init_x);
  printf("CPU: Smooth     ::      %5f\n", t_smooth);
  printf("CPU: Count-X    ::      %5f\n", t_count_x);
  printf("CPU: Count-Y    ::      %5f\n", t_count_y);
}

