#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = { 100, 200, 500, 1000, 1500, 2000 };

void fill_increasing(int* t, unsigned int n)
{
    for (int i = 0; i < n; i++)
    {
        t[i] = i;
    }
}

void fill_decreasing(int* t, unsigned int n)
{
    for (int i = 0; i < n; i++)
    {
        t[i] = n-i-1;
    }
}

void fill_vshape(int* t, unsigned int n)
{
    int* start = t;
    int* end;
    end = t+n-1;
    int j = n;

    while (end-start > 1)
    {
        *start = j;
        j = j-1;
        *end = j;
        j = j-1;
        start++;
        end--;
    }
}

void change(int* t, int a, int b)
{
    int temp = t[a];
    t[a] = t[b];
    t[b] = temp;
}

void selection_sort(int* t, unsigned int n)
{
    int pos;

    for (int a = 0; a < n; a++)
    {
        for (int a = 0; a < (n-1); a++)
        {
            pos = a;
            for (int b = a+1; b < n; b++)
            {
                if (t[pos] > t[b])
                    pos = b;
            }
            if (pos != a)
            {
                change(t, a, pos);
            }
        }
    }
}

void insertion_sort(int* t, unsigned int n)
{
    int i, j, temp;

    for (i = 1; i < n; i++)
    {
        temp = t[i];
        j = i-1;

        while (j >= 0 && t[j] > temp)
        {
            t[j+1] = t[j];
            j = j-1;
        }
        t[j+1] = temp;
    }
}

int split(int* t, int a, int b)
{
    int x, i;
    x = t[b];
    i = a-1;

    for (int j = a; j <= b-1; j++)
    {
        if (t[j] < x)
        {
            i++;
            change(t, i, j);
        }
    }
    change(t, i+1, b);

    return i+1;
}

int randomSplit(int* t, int a, int b)
{
    int x = a + rand() % (b - a);
    change(t, x, b);
    return split(t, a, b);
}

void qSort(int* t, int a, int b)
{
    int x;
    if (a < b)
    {
        x = split(t, a, b);
        //x = randomSplit(t, a, b);
        qSort(t, a, x-1);
        qSort(t, x+1, b);
    }
}

void quick_sort(int* t, unsigned int n)
{
    qSort(t, 0, n-1);
}

void heap_sort(int* t, unsigned int n)
{
    // TODO
}

void fill_random(int* t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        t[i] = rand();
    }
}

void is_random(int* t, unsigned int n) {
    return;
}

void is_increasing(int* t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] > t[i - 1]);
    }
}

void is_decreasing(int* t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] < t[i - 1]);
    }
}

void is_vshape(int* t, unsigned int n) {
    int* begin = t;
    int* end = t + n - 1;

    while (end - begin > 1) {
        assert(*begin > * end);
        begin++;
        assert(*end > * begin);
        end--;
    }
}

void is_sorted(int* t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] >= t[i - 1]);
    }
}

void (*fill_functions[])(int*, unsigned int) = { fill_random, fill_increasing, fill_decreasing, fill_vshape };
void (*check_functions[])(int*, unsigned int) = { is_random, is_increasing, is_decreasing, is_vshape };
//void (*sort_functions[])(int*, unsigned int) = { selection_sort, insertion_sort, quick_sort, heap_sort };
void (*sort_functions[])(int*, unsigned int) = { selection_sort, insertion_sort, quick_sort };

char* fill_names[] = { "Random", "Increasing", "Decreasing", "V-Shape" };
char* sort_names[] = { "SelectionSort", "InsertionSort", "QuickSort", "HeapSort" };

int main() {
    for (unsigned int i = 0; i < sizeof(sort_functions) / sizeof(*sort_functions); i++) {
        void (*sort)(int*, unsigned int) = sort_functions[i];

        for (unsigned int j = 0; j < sizeof(fill_functions) / sizeof(*fill_functions); j++) {
            void (*fill)(int*, unsigned int) = fill_functions[j];
            void (*check)(int*, unsigned int) = check_functions[j];

            for (unsigned int k = 0; k < sizeof(ns) / sizeof(*ns); k++) {
                unsigned int n = ns[k];
                int* t = malloc(n * sizeof(*t));

                fill(t, n);
                check(t, n);

                clock_t begin = clock();
                sort(t, n);
                clock_t end = clock();
                is_sorted(t, n);

                printf("%s\t%s\t%u\t%f\n", sort_names[i], fill_names[j], n, (double)(end - begin) / (double)CLOCKS_PER_SEC);
                free(t);
            }
        }
    }
    return 0;
}
