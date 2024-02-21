#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void test_func(int, char, long long, void (int *, int));


unsigned long long compare_cnt = 0;
unsigned long long swap_cnt = 0;


#define COMPARE(a, b) (++compare_cnt, abs(a) <= abs(b))


#define SWAP(a, b) { \
    ++swap_cnt;      \
    int tmp = (a);   \
    (a) = (b);       \
    (b) = tmp;       \
}


void shell_sort(int *a, int n) {
    for (int d = n / 2; d > 0; d /= 2) {
        for (int i = d; i < n; ++i) {
            for (int j = 0; i - d * (j + 1) >= 0; ++j) {

                if COMPARE(a[i - d - d * j], a[i - d * j]) {
                    break;
                }

                SWAP(a[i - d * j], a[i - d - d * j])
            }
        }
    }
}


void heapify(int *a, int n, int i) {
    int max = i;

    int left = 2 * i + 1, right = 2 * i + 2;

    if (left < n && COMPARE(a[max], a[left]))
        max = left;

    if (right < n && COMPARE(a[max], a[right]))
        max = right;

    if (max != i) {
        SWAP(a[i], a[max])
        heapify(a, n, max);
    }
}


void heap_sort(int *a, int n) {
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(a, n, i);
    }

    for (int i = n - 1; i >= 0; --i) {
        SWAP(a[0], a[i])
        heapify(a, i, 0);
    }
}


int main(void) {
    int n = 50000;
    char mode = 'r'; ///< mode: 'r' — random, 'b' — best, 'w' — worth
    const long long seed = time(NULL);

    test_func(n, mode, seed, shell_sort);
    test_func(n, mode, seed, heap_sort);

    return 0;
}


int rand_int(void) {
    int n = 0;

    for (int i = 0; i < sizeof(int); ++i) {
        n <<= 8;
        n |= rand() % 0x100;
    }

    return n;
}


int *generate_array(int n, char mode, long long seed) {
    int *arr = malloc(n * sizeof(int));

    switch (mode) {
        case 'b':
            for (int i = 0; i < n; ++i) {
                arr[i] = i;
            }
            break;

        case 'w':
            for (int i = 0; i < n; ++i) {
                arr[i] = n - i;
            }
            break;

        case 'r':
        default:
            srand(seed);

            for (int i = 0; i < n; ++i) {
                arr[i] = rand_int();
            }
    }

    return arr;
}


int is_sorted(int *a, int n) {
    for (int i = 0; i < n - 1; ++i) {
        if (abs(a[i]) > abs(a[i + 1])) {
            return 0;
        }
    }

    return 1;
}


void test_func(int n, char mode, long long seed, void func(int *, int)) {
    int *a = generate_array(n, mode, seed);

    compare_cnt = 0;
    swap_cnt = 0;

    func(a, n);

    if (is_sorted(a, n)) {
        printf("Compares: %llu\nSwaps: %llu\nTotal: %llu\n\n", compare_cnt, swap_cnt, compare_cnt + swap_cnt);
    } else {
        printf("Mistake!\nNot sorted\n\n");
    }

    free(a);
}
