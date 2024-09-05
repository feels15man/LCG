#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ull unsigned long long

#define chi_sections 16 //кол-во отрезков
#define chi_run 1000 //кол-во запусков
#define chi_amount 10000//кол-во чисел в запуске

#define series_sections 16 //кол-во отрезков
#define series_run 1000 //кол-во запусков
#define series_amount 100000//кол-во чисел в запуске

ull x;
ull a = 42949677;//256349,32139997781
ull c = 1;
ull m = 4294967296; //2 ^ 32

short power(void) {
    ull s = a - 1;
    for (int i = 2; i <= 65536; i++) {
        s *= a - 1;
        if (!(s % m)) return i;
    }
    return -1;
}

ull lcg_next() {
    x = (x * a + c) % m;
    return x;
}

void lcg_init(ull t) {
    x = t;
}

void chi_square() {
    double percents[] = {0, 5.229, 7.261, 11.04, 14.34, 18.25, 25, 30.58, 10000};
    ull borders[chi_sections + 1];
    ull range = (ull) pow(2, 32.0 - log2(chi_sections));
    borders[0] = 0;
    for (int i = 1; i <= chi_sections; i++) {
        borders[i] = range * i;
        printf("%d - i  Граница от %llu до %llu\n", i, borders[i - 1], borders[i]);
    }

    int p[8] = {0};
    double np = chi_amount * (1.0 / chi_sections);
//    printf("np = %lg\n", np);
    for (int test = 0; test < chi_run; test++) {
        int counter[chi_sections] = {0};

        for (int j = 0; j < chi_amount; j++) {
            int k = 0;
            lcg_next();
//            printf("x = %llu\n", x);
            while (x > borders[k]) k++;
            counter[k - 1]++;
        }
//        for (int i = 0; i < chi_sections; i++) {
//            printf("Counter[%d] - %d\n", i, counter[i]);
//        }
        double v = 0;

        for (int j = 0; j < chi_sections; j++) v += ((double) counter[j] - np) * ((double) counter[j] - np) / np;
//        printf("V%d = %.25lg\n", test + 1, v);

        int k = 0;
        while (v > percents[k]) k++;
        p[k - 1]++;
    }
//    for (int i = 1; i < 8; i++) printf("P[%d] = %d\n", i, p[i]);
    printf("Количество значений < 1%% = %d\nКоличетсво значений > 99%% = %d\nВсего недостаточно случайных: %d\n\n",
           p[0], p[7], p[0] + p[7]);
    printf("Количество значений 1%% < v < 5%% = %d\nКоличетсво значений 95%% < v < 99%% = %d\nВсего подозрительных: %d\n\n",
           p[1], p[6], p[1] + p[6]);
    printf("Всего удовлитворительных значений 10%% < v < 90%% = %d\n", p[2] + p[3] + p[4] + p[5]);
}

void series() {
    printf("Считаю много чисел...\n");
    int d = series_sections;
    ull u = series_sections * series_sections - 1;
    double v;
    double np = series_amount * (1.0 / d / d);
    double xp[] = {-2.33, -1.64, -.674, 0, .674, 1.64, 2.33};
    int res[8] = {0}, counter[series_sections][series_sections], q, r;
    double percents[9];
    percents[0] = 0;
    percents[8] = 100000;
    for (int i = 1; i < 8; i++) {
        percents[i] = (double) u + pow(2.0 * u, .5) * xp[i - 1] + 2.0 / 3 * pow(xp[i - 1], 2) - 2.0 / 3 +
                      pow((double) u, -.5);
    }
//    printf("np = %lg\n", np);
    //вывод посчитанных констант для v
//    for (int i = 0; i < 9; ++i) {
//        printf("%.10lg\n", percents[i]);
//    }
    for (int run = 0; run < series_run; ++run) {
        v = 0;
        //обнулить сounter
        for (int i = 0; i < series_sections; ++i) {
            for (int j = 0; j < series_sections; ++j) {
                counter[i][j] = 0;
            }
        }
        for (int i = 0; i < series_amount; ++i) {
            lcg_next();
            q = roundf(series_sections * x / (m - 1));
            lcg_next();
            r = roundf(series_sections * x / (m - 1));
            counter[q][r]++;
        }

//        for (int i = 0; i < series_sections; ++i) {
//            for (int j = 0; j < series_sections; ++j) {
//                printf("%d ", counter[i][j]);
//            }
//            printf("\n");
//        }

        for (int i = 0; i < series_sections; i++) {
            for (int j = 0; j < series_sections; j++) {
                v += ((double) counter[i][j] - np) * ((double) counter[i][j] - np) / np;
            }
        }
        int k = 0;
        while (v > percents[k]) k++;
        res[k - 1]++;
//        printf("V%d = %.100lg\n", run + 1, v);
//        break;
    }
//    for (int i = 1; i < 8; i++) printf("res[%d] = %d\n", i, res[i]);
//    system("cls");
    system("clear");
    printf("Количество значений < 1%% = %d\nКоличетсво значений > 99%% = %d\nВсего недостаточно случайных: %d\n\n",
           res[0], res[7], res[0] + res[7]);
    printf("Количество значений 1%% < v < 5%% = %d\nКоличетсво значений 95%% < v < 99%% = %d\nВсего подозрительных: %d\n\n",
           res[1], res[6], res[1] + res[6]);
    printf("Всего удовлитворительных значений 10%% < v < 90%% = %d\n", res[2] + res[3] + res[4] + res[5]);
}

void period() {
    printf("Считаю период, одну секундочку...\n");
    ull count = 1;
    ull start = x;
    while (lcg_next() != start) count++;
    //system("cls");
    system("clear");
    printf("Период составляет %llu\n", count);
}

void check_params(){
    printf("x = %llu\na = %llu\nc = %llu\nm = 2 ^ 32 = %llu", x, a, c, m);
}

int main() {
    // y = (ax + c) % m
    lcg_init(time(NULL));
//    printf("x = %llu\n", x);
    char q;


    while (1) {
//        system("cls");
        system("clear");
        printf("Выберите тест:\n1 - Просмотр параметров ЛКГ\n2 - Хи-квадрат критерий\n3 - Критерий серий\n4 - Подсчет мощности\n5 - Проверка периода\n0 - Для выхода\n");
        scanf(" %c", &q);
        system("clear");
        switch (q) {
            case '1':{
                check_params();
                break;
            }
            case '2': {
                chi_square();
                break;
            }
            case '3': {
                series();
                break;
            }
            case '4': {
                printf("\n--------------------\nМощность ЛКГ s = %d\n--------------------\n", power());
                break;
            }
            case '5': {
                period();
                break;
            }
            case '0':
                exit(0);
            default: {
                printf("Вы ввели неверное число\n");
            }
        }
        printf("\n-------------------------------------\nНажмите enter, чтобы продолжить\n");
        getchar();
        getchar();
    }
    return 0;
}