/*

Analisis Rangkaian Dioda dengan Metode Secant

Mata Kuliah   : Komputasi Numerik (ENCE604015)
Tugas         : Tugas Pemrograman B
Semester      : Genap 2024/2025

Kelompok 6    :
- Adhi Rajasa Rafif        2306266943      
- Javana Muhammad Dzaki    2306161826
- Nugroho Ulil Abshar      2306229310
- Rivi Yasha Hafizhan      2306250535

Input         : File CSV berisi parameter rangkaian (data.csv)
Output        : File CSV berisi hasil analisis (results.csv)
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
      readCsvFile("./data-io/data.csv", problems, &problemCount);
    
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
            printf("[!] Masalah unknown: %s\n", problems[i].problemType);
            solutions[i] = 0;
            iterations[i] = 0;
            converged[i] = 0;
            errors[i] = 0;
        }
        
        clock_t end = clock();
        times[i] = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC; 
        
        if (converged[i]) {
            printf("Solusi found: %lf setelah %d iterasi\n", solutions[i], iterations[i]);
            printf("Error: %e\n", errors[i]);
            printf("Time eksekusi: %.2lf ms\n", times[i]);
        } else {
            printf("Metode gagal converge setelah %d iterasi\n", iterations[i]);
            printf("Nilai terakhir: %lf, error: %e\n", solutions[i], errors[i]);
        }
    }
      writeCsvFile("./data-io/results.csv", problems, problemCount, solutions, iterations, converged, errors, times);
    
    printf("[v] Simulasi done\n");
    
    return 0;
}

double diodeFunction(double v, double* params) {
    double Is = params[0];
    double n = params[1];
    double Vt = params[2];
    double I = params[3];
    
    double exponent_arg = v / (n * Vt);
    if (exponent_arg > 50.0) {
        exponent_arg = 50.0;
    }
    
    double exp_term = exp(exponent_arg);
    double diode_current = Is * (exp_term - 1);
    
    if (!isfinite(diode_current) || diode_current > 1e6) {
        return (v > 1.0) ? 1e3 : -1e3;
    }
    
    return diode_current - I;
}

double diodeResistorFunction(double v, double* params) {
    double Is = params[0];
    double n = params[1];
    double Vt = params[2];
    double R = params[3];
    double Vs = params[4];
    
    return Is * (exp(v / (n * Vt)) - 1) - (Vs - v) / R;
}

double secantMethod(double (*f)(double, double*), double x0, double x1, double* params, 
                   double tol, int maxIter, int* iterations, double* error, int* converged) {
    double f0, f1, x2, f2, x_prev = 0;
    double damping_factor = 1.0;
    int oscillation_count = 0;
    
    *iterations = 0;
    *converged = 0;
    *error = 0.0;
    
    f0 = f(x0, params);
    f1 = f(x1, params);
    
    printf("Step\tx0\tx1\tx2\tf(x2)\t\n");
    
    do {
        f0 = f(x0, params);
        f1 = f(x1, params);
        
        if (fabs(f1 - f0) < 1e-15) {
            printf("[!] Error, f(x0) and f(x1) terlalu dekat\n");
            break;
        }

        double x2_raw = x1 - (x1 - x0) * f1 / (f1 - f0);
        
        x2 = x1 + damping_factor * (x2_raw - x1);
        
        if (x2 > 1.5) x2 = 1.5;
        if (x2 < 0.2) x2 = 0.2;
        
        if (*iterations > 0 && fabs(x2 - x_prev) < 1e-8) {
            oscillation_count++;
            if (oscillation_count > 3) {
                damping_factor *= 0.5;
                oscillation_count = 0;
                printf("[>] Dumping biar tidak osilasi. Damping ke %f\n", damping_factor);
            }
        } else {
            oscillation_count = 0;
        }
        
        if (fabs(x2 - x1) > 0.3) {
            damping_factor = 0.3;
            x2 = x1 + damping_factor * (x2_raw - x1);
        } else if (damping_factor < 1.0) {
            damping_factor = fmin(1.0, damping_factor * 1.1);
        }
        
        f2 = f(x2, params);
        
        printf("%d\t%lf\t%lf\t%lf\t%e\t%.3f\n", *iterations + 1, x0, x1, x2, f2, damping_factor);

        *error = fabs(x2 - x1);

        if (fabs(f2) < tol || *error < tol * 10) {
            *converged = 1;
            return x2;
        }

        x_prev = x1;
        
        x0 = x1;
        x1 = x2;
        (*iterations)++;

        if (*iterations >= maxIter) {
            printf("[!] Gagal converge melebihi maksimal iterasi", maxIter);
            break;
        }
        
        if (damping_factor < 0.01) {
            printf("[>] Damping terlalu kecil, reset\n");
            damping_factor = 0.5;
        }
        
    } while (fabs(f2) > tol);

    return x2;
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
            } else if (tokenIndex % 2 == 1 && paramIndex < 5) {
                strcpy(prob->paramNames[paramIndex], token);
            } else if (tokenIndex % 2 == 0 && paramIndex < 5 && tokenIndex > 0) {
                prob->paramValues[paramIndex] = atof(token);
                paramIndex++;
            } else if (tokenIndex == 11) {
                prob->x0 = atof(token);
            } else if (tokenIndex == 12) {
                prob->x1 = atof(token);
            } else if (tokenIndex == 13) {
                prob->tolerance = atof(token);
            } else if (tokenIndex == 14) {
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