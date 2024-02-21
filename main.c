#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define EPS 0.0000000001
#define INCREASING 1
#define DECREASING 0
#define HEAD 0
#define PREHEAD -1
#define MAXEXP 32

void swap(double* a, double* b)
{
    double buf = *a;
    *a = *b;
    *b = buf;
}

void print_arr(double* a, int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%lf\n", a[i]);
    }
    printf("----------\n");
}


// returns 1 if array is sorted correctly
// and 0 if it is sorted incorrectly
int check_sorted_array(double* a, int n)
{
    for (int i = 1; i < n; ++i)
    {
        if (fabs(a[i - 1]) < fabs(a[i]))
        {
            return 0;
        }
    }

    return 1;
}

double generate_random_double(void)
{
    double random_number;

    double frac = ((double) rand() / RAND_MAX);
    int exp = rand() % MAXEXP;
    random_number = frac * ((double)pow(2, exp));


    // generating random sign of the number
    int random_sign_first = rand();
    int random_sign_second = rand();
    int sign = (random_sign_first > random_sign_second) ? 1 : -1;


    random_number *= sign;

    return random_number;
}


void generate_random_numbers_array(double *a, int n)
{
    for (int i = 0; i < n; ++i)
    {
        a[i] = generate_random_double();
    }
}

// function for generating array
// with only increasing or decreasing
// values due to given parameter
void generate_linear_array(double* a, int n, int parameter)
{
    double random_start = generate_random_double();
    double random_delta = generate_random_double();

    while (fabs(random_delta) < EPS || random_delta < 0)
        random_delta = generate_random_double();

    if (parameter == DECREASING)
        random_delta *= -1;

    for (int i = 0; i < n; ++i)
    {
        a[i] = random_start + random_delta * i;
    }
}

// function that looks at the parameter
// and gives array of needed type
double* generate_array(int parameter, int n)
{
    double* a = calloc(n, sizeof(double));
    switch (parameter)
    {
    case 1:
        generate_linear_array(a, n, INCREASING);
        break;
    case 2:
        generate_linear_array(a, n, DECREASING);
        break;
    case 3:
        generate_random_numbers_array(a, n);
        break;
    default:
        break;
    }
    return a;
}


void shell_sorting(double* a, int n)
{
    int swap_cnt = 0;
    int comp_cnt = 0;

    int step = n;
    while (step >= 1)
    {
        step /= 2;
        int ind;
        for (int i = 0; i < step; i++)
        {
            for (int j = i + step; j < n; j += step)
            {
                ind = j;
                while ((ind - step) >= 0 && ++comp_cnt && (fabs(a[ind]) > fabs(a[ind - step])))
                {
                    swap(&a[ind], &a[ind - step]);
                    swap_cnt++;
                    ind -= step;
                }
            }
        }
    }

    printf("Shell sort completed\n %d comparisons \n %d swaps\n", comp_cnt, swap_cnt);
}


void sift_element_up(double* a, int ind, int* swaps, int* compares)
{
    while (ind > 0 && ++(*compares) && (fabs(a[ind]) < fabs(a[(ind - 1) / 2])))
    {
        swap(&a[ind], &a[(ind - 1)/ 2]);
        ++(*swaps);
        ind = (ind - 1) / 2;
    }
}

void build_heap(double* a, int n, int* swaps, int* compares)
{
    for (int i = 1; i < n; ++i)
    {
        sift_element_up(a, i, swaps, compares);
    }
}

// these two functions are created
// to exclude "magic" numbers from code
// and increase readability
// btw, they give indexes of left and right
// children of an element with given index
// in the heap
int left_child_ind(int k)
{
    return 2 * k + 1;
}

int right_child_ind(int k)
{
    return 2 * k + 2;
}

void sift_head_down(double* a, int n, int* swap_cnt, int* comp_cnt)
{
    int head = PREHEAD;
    int next = HEAD;

    int right_ind;
    int left_ind;

    while (head != next)
    {
        head = next;
        right_ind = right_child_ind(head);
        left_ind = left_child_ind(head);

        if (left_ind < n && ++(*comp_cnt) && (fabs(a[next]) > fabs(a[left_ind])))
        {
            next = left_ind;
        }

        if (right_ind < n && ++(*comp_cnt) && (fabs(a[next]) > fabs(a[right_ind])))
        {
            next = right_ind;
        }

        if (head != next)
        {
            swap(&a[next], &a[head]);
            (*swap_cnt)++;
        }
    }
}

void pyramid_sort(double* a, int n)
{
    int swap_cnt = 0;
    int comp_cnt = 0;

    build_heap(a, n, &swap_cnt, &comp_cnt);

    int sorted = 1;
    for (int i = 0; i < (n - 1); ++i)
    {
        swap(&a[HEAD], &a[n - sorted]);
        swap_cnt++;

        sift_head_down(a, n - sorted, &swap_cnt, &comp_cnt);
        sorted++;
    }

    printf("Pyramid sort completed\n %d comparisons \n %d swaps\n", comp_cnt, swap_cnt);
}


int main(void)
{
    srand(time(NULL));

    int n;
    int generator_parameter;

    printf("Put length of array: ");
    scanf("%d", &n);
    printf("Put generator parameter(1-3): ");
    scanf("%d",  &generator_parameter);

    double* a = generate_array(generator_parameter, n);
    printf("Generated array:\n");
    print_arr(a, n);

    double* a_copy = calloc(n, sizeof(double));
    memcpy(a_copy, a, n * sizeof(double));

    pyramid_sort(a, n);
    print_arr(a, n);
    shell_sorting(a_copy, n);
    print_arr(a_copy, n);

    int shell_sorted_correctly = check_sorted_array(a_copy, n);
    int pyramid_sorted_correctly = check_sorted_array(a, n);

    if (shell_sorted_correctly)
        printf("Shell sort completed correctly\n");
    else
        printf("Shell sort completed incorrectly\n");

    if (pyramid_sorted_correctly)
        printf("Pyramid sort completed correctly\n");
    else
        printf("Pyramid sort completed incorrectly\n");

    free(a);
    free(a_copy);
    return 0;
}
