#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <vector>

const int MAX_N = 5000;
const char* fileName = "d16.txt";
// const char* fileName = "d100.txt";
// const char* fileName = "d500.txt";
// const char* fileName = "d1000.txt";
// const char* fileName = "d5000.txt";


int original[MAX_N + 1];
int arr[MAX_N + 1];
int N = 0;

int compareInts(const void* a, const void* b)
{
    int x = *(int*)a;
    int y = *(int*)b;
    return (x > y) - (x < y);
}



// Начало qSort
void sortWithQsort(int* arrK, int n)
{
    qsort(arrK + 1, n, sizeof(int), compareInts);
}
// конец qSort



void setAbsL(std::vector<int>& links, int s, int p)
{
    links[s] = links[s] < 0 ? -p : p; // "|" по Кнуту
}



// Начало сортировки слиянием списков
void sortListMerge(int* arrK, int n)
{
    if (n < 2)
        return;

    std::vector<int> links(n + 2, 0);
    std::vector<int> temp(n + 1, 0);

    int s, t, p, q;

    links[0] = 1;
    links[n + 1] = 2;

    for (int i = 1; i <= n - 2; i++)
        links[i] = -(i + 2);

    while (true)
    {
        s = 0;
        t = n + 1;
        p = links[s];
        q = links[t];

        if (q == 0)
            break;

        do
        {
            do
            {
                if (arrK[p] <= arrK[q])
                {
                    setAbsL(links, s, p);
                    s = p;
                    p = links[p];
                }
                else
                {
                    setAbsL(links, s, q);
                    s = q;
                    q = links[q];
                }
            } while (p > 0 && q > 0);

            if (p <= 0)
            {
                links[s] = q;
                s = t;

                do
                {
                    t = q;
                    q = links[q];
                } while (q > 0);
            }
            else
            {
                links[s] = p;
                s = t;

                do
                {
                    t = p;
                    p = links[p];
                } while (p > 0);
            }

            p = -p;
            q = -q;

        } while (q != 0);

        setAbsL(links, s, p);
        setAbsL(links, t, 0);
    }

    p = links[0];

    for (int i = 1; i <= n; i++)
    {
        temp[i] = arrK[p];
        p = links[p];
    }

    for (int i = 1; i <= n; i++)
        arrK[i] = temp[i];
}
// Конец сортировки слиянием списков



// Начало пузырька
void sortBubble(int* arrK, int n)
{
    int bound = n;

    while (bound > 1)
    {
        int lastSwap = 0;

        for (int i = 1; i < bound; i++)
        {
            if (arrK[i] > arrK[i + 1])
            {
                int temp = arrK[i];
                arrK[i] = arrK[i + 1];
                arrK[i + 1] = temp;
                lastSwap = i;
            }
        }

        bound = lastSwap;
    }
}
// Конец пузырька



// Общая функкция для всех сортировок
void runSort(
    const char* name,
    void (*sortFunction)(int*, int)
)
{
    for (int i = 1; i <= N; i++)
        arr[i] = original[i];

    auto start = std::chrono::steady_clock::now();
    sortFunction(arr, N);
    auto end = std::chrono::steady_clock::now();

    long long time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        ).count();

    for (int i = 1; i < N; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            printf("%s: error\n", name);
            return;
        }
    }

    printf("%s: %lld microseconds\n", name, time);
}



int main()
{
    FILE* file = fopen(fileName, "r");

    if (!file)
    {
        printf("Cannot open file\n");
        return 1;
    }

    while (N < MAX_N && fscanf(file, "%d", &original[N + 1]) == 1)
        N++;

    fclose(file);

    if (N == 0)
    {
        printf("No data\n");
        return 1;
    }

    printf("Read %d numbers\n", N);

    runSort("qsort", sortWithQsort);
    runSort("list merge", sortListMerge);
    runSort("bubble", sortBubble);

    return 0;
}