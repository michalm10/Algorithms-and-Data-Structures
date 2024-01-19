#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAXLEN 30

typedef struct {
    char s[MAXLEN];
    char n[10];
    int length;
} str;

void swap(str *a, str *b) {
    str t = *a;
    *a = *b;
    *b = t;
}

str* load(FILE *in, int *len) {
    int n = 0, arrLen = 10;
    str *arr = (str*) malloc(10 * sizeof(str));
    while (feof(in) == 0) {
        if (n >= arrLen) {
            arr = realloc(arr, 1.2 * arrLen * sizeof(str));
            arrLen *= 1.2;
        }
        char s[MAXLEN], num[10];
        fscanf(in, "%s", num);
        fscanf(in, "%s", s);
        for (int i = 0; i < strlen(s); i++)
            if (!isalpha(s[i])) {
                memmove((s + i), (s + i + 1), (strlen(s) - i + 1) * sizeof(char));
                s[strlen(s)] = '\0';
                i--;
            }
        strcpy(arr[n].s, s);
        strcpy(arr[n].n, num);
        arr[n].length = strlen(arr[n].s);
        n++;
    }
    *len = n;
    return arr;
}

int partition (str *arr, int p, int r) {
    str x = arr[r];
    int i = p - 1;
    for (int j = p; j <= r; j++)
        if (strcmp(arr[j].s, x.s) <= 0) {
            i++;
            if (i<=r)
            swap(arr + i, arr + j);
        }
    if (i < r)
        return i;
    else
        return i - 1;
}

void quicksort (str *arr, int p, int r) {
    if (p < r) {
        int q = partition(arr, p, r);
        quicksort(arr, p, q);
        quicksort(arr, q + 1, r);
    }
}

str* countingSort(str *arr, int n, int pos) {
    str *B = (str*) malloc(n * sizeof(str));
    int C[26];
    for (int i = 0; i < 26; i++)
        C[i] = 0;
    for (int i = 0; i < n; i++)
        C[pos >= arr[i].length ? 0 : toupper(arr[i].s[pos]) - 'A']++;
    for (int i = 1; i < 26; i++)
        C[i] += C[i - 1];
    for (int i = n - 1; i >= 0; i--) {
        B[C[pos >= arr[i].length ? 0 : toupper(arr[i].s[pos]) - 'A'] - 1] = arr[i];
        C[pos >= arr[i].length ? 0 : toupper(arr[i].s[pos]) - 'A']--;
    }
    free(arr);
    return B;
}

str* radixSort(str *arr, int n) {
    int max = 0;
    for (int i = 0; i < n; i++)
        if (arr[i].length > max) max = arr[i].length;
    for (int i = max - 1; i >= 0; i--)
        arr = countingSort(arr, n, i);
    return arr;
}

int main() {
    int n;
    LARGE_INTEGER start, end, freq;
    FILE *in = fopen("input.txt", "r");
    FILE *out = fopen("output.txt", "w");

    str *arr = load(in, &n);

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    arr = radixSort(arr, n);
    QueryPerformanceCounter(&end);
    double time1 = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    fseek(in, 0, SEEK_SET);
    str *arr2 = load(in, &n);

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    quicksort(arr2, 0, n - 1);
    QueryPerformanceCounter(&end);
    double time2 = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    for (int i = 0; i < n; i++)
        fprintf(out, "%s %s\n", arr[i].n, arr[i].s);

    printf("Radix Sort: %g\nQuick Sort: %g\n", time1, time2);

    fclose(in);
    fclose(out);

    free(arr);
    free(arr2);
    return 0;
}
