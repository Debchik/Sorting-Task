#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAIL 10000
#define EPS 0.0000000001
#define INCREASING 1
#define DECREASING 0
#define HEAD 0
#define PREHEAD -1

void swap(double* a, double* b)
{
    double buf = *a;
    *a = *b;
    *b = buf;
}

double generate_random_double(void)
{
    int integer_part = rand();
    int fractional_part = rand();
    double random_number;

    // generating random sign of the number
    int random_sign_first = rand();
    int random_sign_second = rand();
    int sign = (random_sign_first > random_sign_second) ? 1 : -1;

    random_number = integer_part + ((double)(fractional_part) / (double)TAIL);
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

    while (abs(random_delta) < EPS || random_delta < 0)
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
            for (int j = i + step; j < n; ++j)
            {
                ind = j;
                while ((ind - step) >= 0 && ++comp_cnt && (abs(a[ind]) > abs(a[ind - step])))
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


void sift_element_up(double* a, int ind)
{
    while (ind > 0 && (abs(a[ind]) < abs(a[ind / 2])))
    {
        swap(&a[ind], &a[ind / 2]);
        ind /= 2;
    }
}

void build_heap(double* a, int n)
{
    for (int i = 1; i < n; ++i)
    {
        sift_element_up(a, i);
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

        if (left_ind < n && ++(*comp_cnt) && (abs(a[next]) > abs(a[left_ind])))
        {
            next = left_ind;
        }

        if (right_ind < n && ++(*comp_cnt) && (abs(a[next]) > abs(a[right_ind])))
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

    build_heap(a, n);

    int sorted = 1;
    for (int i = 0; i < n; ++i)
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
    printf("Put generator parameter: ");
    scanf("%d",  &generator_parameter);

    double* a = generate_array(3, n);
    double* a_copy = calloc(n, sizeof(double));
    memcpy(a_copy, a, n * sizeof(double));

    pyramid_sort(a, n);
    shell_sorting(a_copy, n);

    free(a);
    free(a_copy);
    return 0;
}
