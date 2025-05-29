/*

TUGAS PEMROGRAMAN B KOMPUTASI NUMERIK
Kelompok 6:
- Adhi Rajasa Rafif
- Javana Muhammad Dzaki
- Nugroho Ulil Abshar
- Rivi Yasha

TOPIK:
Analisis Diode Circuit Menggunakan Metode Secant
Implementasi Buku Chapman Part 6

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_PARAMS 10

typedef struct {
    char problemType[50];
    char paramNames[MAX_PARAMS][50];
    double paramValues[MAX_PARAMS];
    int paramCount;
    double x0;
    double x1;
    double tolerance;
    int maxIterations;
} Problem;

double diodeFunction(double v, double* params);
double diodeResistorFunction(double v, double* params);
double secantMethod(double (*f)(double, double*), double x0, double x1, double* params, double tol, int maxIter, int* iterations, double* error, int* converged);
void readCsvFile(const char* filename, Problem* problems, int* problemCount);
void writeCsvFile(const char* filename, Problem* problems, int problemCount, double* solutions, int* iterations, int* converged, double* errors, double* times);

int main() {
    Problem problems[100];
    int problemCount = 0;
    double solutions[100];
    int iterations[100];
    int converged[100];
    double errors[100];
    double times[100];
    
    readCsvFile("data.csv", problems, &problemCount);
    
    printf("Jumlah masalah yang ditemukan: %d\n", problemCount);
    
    for (int i = 0; i < problemCount; i++) {
        clock_t start = clock();
        
        printf("\n>> Secant Solving ke %d: %s\n", i + 1, problems[i].problemType);
        
        if (strcmp(problems[i].problemType, "diode") == 0) {
            solutions[i] = secantMethod(diodeFunction, problems[i].x0, problems[i].x1, 
                                        problems[i].paramValues, problems[i].tolerance, 
                                        problems[i].maxIterations, &iterations[i], 
                                        &errors[i], &converged[i]);
        } 
        else if (strcmp(problems[i].problemType, "diode_resistor") == 0) {
            solutions[i] = secantMethod(diodeResistorFunction, problems[i].x0, problems[i].x1, 
                                        problems[i].paramValues, problems[i].tolerance, 
                                        problems[i].maxIterations, &iterations[i], 
                                        &errors[i], &converged[i]);
        } 
        else {
            printf("Tipe masalah tidak dikenal: %s\n", problems[i].problemType);
            solutions[i] = 0;
            iterations[i] = 0;
            converged[i] = 0;
            errors[i] = 0;
        }
        
        clock_t end = clock();
        times[i] = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC; 
        
        if (converged[i]) {
            printf("Solusi ditemukan: %lf setelah %d iterasi\n", solutions[i], iterations[i]);
            printf("Error: %e\n", errors[i]);
            printf("Waktu eksekusi: %.2lf ms\n", times[i]);
        } else {
            printf("Metode gagal konvergen setelah %d iterasi\n", iterations[i]);
            printf("Nilai terakhir: %lf dengan error: %e\n", solutions[i], errors[i]);
        }
    }
    
    printf("\nWriting to csv\n");
    writeCsvFile("results.csv", problems, problemCount, solutions, iterations, converged, errors, times);
    
    printf("[v] Simulasi done\n");
    return 0;
}

double diodeFunction(double v, double* params) {
    double Is = params[0];   // arus saturation
    double n = params[1];    // ideality 
    double Vt = params[2];   // voltage thermnal
    double I = params[3];    // arus di diode
    
    return Is * (exp(v / (n * Vt)) - 1) - I;
}

double diodeResistorFunction(double v, double* params) {
    double Is = params[0];   // arus saturation
    double n = params[1];    // ideality 
    double Vt = params[2];   // voltage thermal
    double R = params[3];    // resistor satuan ohm
    double Vs = params[4];   // voltage sourcenya
    
    return Is * (exp(v / (n * Vt)) - 1) - (Vs - v) / R;
}

double secantMethod(double (*f)(double, double*), double x0, double x1, double* params, 
                   double tol, int maxIter, int* iterations, double* error, int* converged) {
    double f0 = f(x0, params);
    double f1 = f(x1, params);
    double x2;
    
    *iterations = 0;
    *converged = 0;
    
    while (*iterations < maxIter) {
        if (fabs(f1 - f0) < 1e-15) {
            printf("[!] Pembagian oleh nilai mendekati nol\n");
            break;
        }
        
        x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
        *error = fabs(x2 - x1);
        
        printf("Iterasi %d: x = %.10lf, f(x) = %.10e, error = %.10e\n", 
              *iterations + 1, x2, f(x2, params), *error);
        
        if (*error < tol) {
            *converged = 1;
            *iterations = *iterations + 1;
            return x2;
        }
        
        x0 = x1;
        f0 = f1;
        x1 = x2;
        f1 = f(x2, params);
        
        *iterations = *iterations + 1;
    }
    
    if (*iterations >= maxIter)
        printf("Peringatan: Mencapai batas iterasi maksimum\n");
    
    return x1;
}

void readCsvFile(const char* filename, Problem* problems, int* problemCount) {
    FILE* file = fopen(filename, "r");
    
    char line[MAX_LINE_LENGTH];
    int lineNum = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (lineNum == 0 || line[0] == '/' || strlen(line) <= 1) { 
            lineNum++;
            continue; 
        }
        
        Problem* prob = &problems[*problemCount];
        char* token;
        int paramIndex = 0;
        int tokenIndex = 0;
        
        token = strtok(line, ",");
        while (token != NULL) {
            if (tokenIndex == 0) {
                strcpy(prob->problemType, token);
            } else if (tokenIndex % 2 == 1 && tokenIndex < MAX_PARAMS * 2 + 1) {
                strcpy(prob->paramNames[paramIndex], token);
            } else if (tokenIndex % 2 == 0 && tokenIndex < MAX_PARAMS * 2 + 1) {
                prob->paramValues[paramIndex] = atof(token);
                paramIndex++;
            } else if (tokenIndex == MAX_PARAMS * 2 + 1) {
                prob->x0 = atof(token);
            } else if (tokenIndex == MAX_PARAMS * 2 + 2) {
                prob->x1 = atof(token);
            } else if (tokenIndex == MAX_PARAMS * 2 + 3) {
                prob->tolerance = atof(token);
            } else if (tokenIndex == MAX_PARAMS * 2 + 4) {
                prob->maxIterations = atoi(token);
            }
            
            token = strtok(NULL, ",");
            tokenIndex++;
        }
        
        prob->paramCount = paramIndex;
        (*problemCount)++;
        lineNum++;
    }
    
    fclose(file);
}

void writeCsvFile(const char* filename, Problem* problems, int problemCount, 
                 double* solutions, int* iterations, int* converged, double* errors, double* times) {
    FILE* file = fopen(filename, "w");

    
    fprintf(file, "problem_type,");
    for (int j = 0; j < MAX_PARAMS; j++) {
        fprintf(file, "param%d_name,param%d_value,", j+1, j+1);
    }
    fprintf(file, "x0,x1,tolerance,max_iterations,solution,iterations,converged,error,time_ms\n");
    
    for (int i = 0; i < problemCount; i++) {
        fprintf(file, "%s,", problems[i].problemType);
        
        for (int j = 0; j < MAX_PARAMS; j++) {
            if (j < problems[i].paramCount) {
                fprintf(file, "%s,%g,", problems[i].paramNames[j], problems[i].paramValues[j]);
            } else {
                fprintf(file, ",,");
            }
        }
        
        fprintf(file, "%g,%g,%g,%d,%g,%d,%s,%g,%g\n",
                problems[i].x0, problems[i].x1, problems[i].tolerance, problems[i].maxIterations,
                solutions[i], iterations[i], converged[i] ? "yes" : "no", errors[i], times[i]);
    }
    
    fclose(file);
}