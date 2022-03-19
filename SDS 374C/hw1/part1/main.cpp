#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
// compile with: icc -O2 -qopenmp -xhost main.cpp
// execute with: numactl --cpunodebind=0 --preferred=0 ./a.out
// threads: export OMP_NUM_THREADS=1;

void initialize(float** arr, int n) {
  unsigned seed = (int) time(NULL);
  #pragma omp parallel for
  for (int i = 0; i < n; i++) {
    #pragma omp parallel for private(seed)
    for (int j = 0; j < n; j++) {
      arr[i][j] = rand_r(&seed) / (float) RAND_MAX;
    }
  }
}

void printMatrix(float** arr, int n) {
  printf("--------------\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%f ", arr[i][j]);
    }
    printf("\n");
  }
  printf("--------------\n");
}


void smooth(float** x, float** y, int n, float a, float b, float c) {
  #pragma omp parallel for
  for (int i = 1; i < n - 1; i++) {
    #pragma omp parallel for
    for (int j = 1; j < n - 1; j++) {
      y[i][j] = a * (x[i-1][j-1] + x[i-1][j+1] + x[i+1][j-1] + x[i+1][j+1]) +
                b * (x[i-1][j] + x[i+1][j] + x[i][j-1] + x[i][j+1]) +
                c * x[i][j];
    }
  }
}

void count(float** arr, int n, float t, int &cnt) {
  int num_threads = omp_get_max_threads();
  int count[num_threads]={0};
  #pragma omp parallel for
  for (int i = 1; i < n - 1; i++) {
    #pragma omp parallel for
    for (int j = 1; j < n - 1; j++)
      if (arr[i][j] < t) {
        count[omp_get_thread_num()]++;
      }
  }

  int total_count = 0;
  #pragma omp parallel for reduction(+:total_count)
  for (int i = 0; i < num_threads; i++) {
    total_count += count[i];
  }
  cnt = total_count;
}

int main() {
  const float a = 0.05, b = 0.1, c = 0.4;
  const float t = 0.1;
  const long n = 98306;
  // const long n = 5;
  int cnt_x = 0, cnt_y = 0;
  double start, end;
  double t_alloc_x, t_alloc_y, t_init_x, t_smooth, t_count_x, t_count_y, t_init_x_1;
  float **x;
  float **y;

  // dummy parallel region to prevent overhead from starting the very first parallel region.
  #pragma omp parallel
  {
    printf("This is thread %d\n", omp_get_thread_num());
  }

  // malloc
  start = omp_get_wtime();
  x = (float**)malloc(n * sizeof(float*));
  for (int i = 0; i < n; i++)
    x[i] = (float*)malloc(n * sizeof(float));
  end = omp_get_wtime();
  t_alloc_x = end - start;

  start = omp_get_wtime();
  y = (float**)malloc(n * sizeof(float*));
  for (int i = 0; i < n; i++) {
    y[i] = (float*)malloc(n * sizeof(float));
  }

  // init
  end = omp_get_wtime();
  t_alloc_y = end - start;
  start = omp_get_wtime();
  initialize(x, n);
  end = omp_get_wtime();
  t_init_x = end - start;

  // start = omp_get_wtime();
  // initialize(x, n);
  // end = omp_get_wtime();
  // t_init_x_1 = end - start;
  // printf("CPU: Init-X-1     ::      %5f\n", t_init_x_1);

  // smooth
  start = omp_get_wtime();
  smooth(x, y, n, a, b, c);
  end = omp_get_wtime();
  t_smooth = end - start;

  // count
  start = omp_get_wtime();
  count(x, n, t, cnt_x);
  end = omp_get_wtime();
  t_count_x = end - start;

  start = omp_get_wtime();
  count(y, n, t, cnt_y);
  end = omp_get_wtime();
  t_count_y = end - start;

  printf("Summarry\n--------\n");
  printf("Number of threads                         :: %10d\n", omp_get_max_threads());
  printf("Number of elements in a row/column        :: %10d\n", n);
  printf("Number of inner elements in a row/column  :: %10d\n", n - 2);
  printf("Total number of elements                  :: %10d\n", n * n);
  printf("Total number of inner elements            :: %10d\n", (n - 2) * (n - 2));
  printf("Memory (GB) used per array                :: %10f\n", n * n * 4 / ((float) 1024 * 1024 * 1024));
  printf("Threshold                                 :: %10f\n", t);
  printf("Smoothing constants (a, b, c)             :: %3f %4f %3f\n", a, b, c);
  printf("Number of elements below threshold (X)    :: %10d\n", cnt_x);
  printf("Fraction of elements below threshold      :: %10f\n", cnt_x / (float) ((n-2)*(n-2)));
  printf("Number of elements below threshold (Y)    :: %10d\n", cnt_y);
  printf("Fraction of elements below threshold      :: %10f\n", cnt_y / (float) ((n-2)*(n-2)));
  printf("Action          ::      time/s     Time resolution = 1.0E-04\n-------------\n");
  printf("CPU: Alloc-X    ::      %5f\n", t_alloc_x);
  printf("CPU: Alloc-Y    ::      %5f\n", t_alloc_y);
  printf("CPU: Init-X     ::      %5f\n", t_init_x);
  printf("CPU: Smooth     ::      %5f\n", t_smooth);
  printf("CPU: Count-X    ::      %5f\n", t_count_x);
  printf("CPU: Count-Y    ::      %5f\n", t_count_y);

  free(x);
  free(y);
}

