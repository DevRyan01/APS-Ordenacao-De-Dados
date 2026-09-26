#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* ---------- Função auxiliar de troca ---------- */
void troca(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* ---------- Selection Sort ---------- */
void selectionSort(int arr[], int tamanho) {
    int i, j, min_index;
    for (i = 0; i < tamanho - 1; i++) {
        min_index = i;
        for (j = i + 1; j < tamanho; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            troca(&arr[i], &arr[min_index]);
        }
    }
}

/* ---------- Heap Sort ---------- */
void heapify(int arr[], int n, int i) {
    int maior = i;
    int filho_esq = 2 * i + 1;
    int filho_dir = 2 * i + 2;

    if (filho_esq < n && arr[filho_esq] > arr[maior])
        maior = filho_esq;
    if (filho_dir < n && arr[filho_dir] > arr[maior])
        maior = filho_dir;

    if (maior != i) {
        troca(&arr[i], &arr[maior]);
        heapify(arr, n, maior);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        troca(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

/* ---------- Merge Sort ---------- */
void merge(int arr[], int first, int mid, int last) {
    int n1 = mid - first + 1;
    int n2 = last - mid;

    int *esq = malloc(n1 * sizeof(int));
    int *dir = malloc(n2 * sizeof(int));
    if (esq == NULL || dir == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria no merge.\n");
        free(esq); free(dir);
        exit(1);
    }

    for (int i = 0; i < n1; i++) esq[i] = arr[first + i];
    for (int j = 0; j < n2; j++) dir[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = first;
    while (i < n1 && j < n2) {
        if (esq[i] <= dir[j]) arr[k++] = esq[i++];
        else arr[k++] = dir[j++];
    }
    while (i < n1) arr[k++] = esq[i++];
    while (j < n2) arr[k++] = dir[j++];

    free(esq);
    free(dir);
}

void mergeSort(int arr[], int first, int last) {
    if (first < last) {
        int mid = (first + last) / 2;
        mergeSort(arr, first, mid);
        mergeSort(arr, mid + 1, last);
        merge(arr, first, mid, last);
    }
}

/* Geração dos três tipos de dado */

/* Aleatório */
void gerarAleatorio(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }
}

/* Ordenado */
void gerarOrdenado(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

/* Semi-ordenado */

void gerarSemiOrdenado(int arr[], int n) {
    gerarOrdenado(arr, n);
    int trocas = n / 10;
    for (int i = 0; i < trocas; i++) {
        int pos1 = rand() % n;
        int pos2 = rand() % n;
        troca(&arr[pos1], &arr[pos2]);
    }
}

/*  Leitura de dados externos */
                                                   */
int lerVetorDeArquivo(const char *nomeArquivo, int arr[], int n) {
    FILE *f = fopen(nomeArquivo, "r");
    if (f == NULL) {
        fprintf(stderr, "Nao foi possivel abrir '%s'. Rode antes o programa gerar_arquivos_externos.c\n", nomeArquivo);
        exit(1);
    }
    int i = 0;
    while (i < n && fscanf(f, "%d", &arr[i]) == 1) {
        i++;
    }
    fclose(f);
    return i;
}

/*  Verificação de corretude */
int estaOrdenado(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) return 0;
    }
    return 1;
}


double raizQuadrada(double x) {
    if (x <= 0.0) return 0.0;
    double chute = x;
    for (int i = 0; i < 40; i++) {
        chute = 0.5 * (chute + x / chute);
    }
    return chute;
}


double calcularDesvioPadrao(double tempos[], int n, double media) {
    if (n <= 1) return 0.0;
    double somaQuadrados = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = tempos[i] - media;
        somaQuadrados += diff * diff;
    }
    return raizQuadrada(somaQuadrados / (n - 1));
}

/*  Estrutura para guardar cada resultado  */
typedef struct {
    int volume;
    char tipoDado[20];
    char algoritmo[20];
    double tempoMedio;
    double tempoMinimo;
    double tempoMaximo;
    double desvioPadrao;
    int ordenadoCorretamente;
} Resultado;


#define REPETICOES 5

int main(void) {
    srand((unsigned int) time(NULL));

    int volumes[] = {1000, 10000, 50000, 100000};
    int numVolumes = 4;

    const char *tiposDado[] = {"Aleatorio", "Ordenado", "Semi-ordenado"};
    int numTipos = 3;

    const char *algoritmos[] = {"Heap Sort", "Merge Sort", "Selection Sort"};
    int numAlgoritmos = 3;

    int totalInternos = numVolumes * numTipos * numAlgoritmos;
    int totalExternos = numVolumes * numAlgoritmos;
    int totalTestes = totalInternos + totalExternos;
    Resultado *resultados = malloc(totalTestes * sizeof(Resultado));
    if (resultados == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        return 1;
    }

    int idx = 0;

    printf("Executando testes, isso pode levar alguns segundos (Selection Sort em 100 mil eh o mais demorado)...\n\n");

    for (int v = 0; v < numVolumes; v++) {
        int n = volumes[v];

        for (int t = 0; t < numTipos; t++) {

            for (int a = 0; a < numAlgoritmos; a++) {

                double tempos[REPETICOES];
                int todasOrdenadas = 1;

                for (int rep = 0; rep < REPETICOES; rep++) {


                    int *dados = malloc(n * sizeof(int));
                    if (dados == NULL) {
                        fprintf(stderr, "Erro de alocacao de memoria.\n");
                        free(resultados);
                        return 1;
                    }

                    if (strcmp(tiposDado[t], "Aleatorio") == 0) {
                        gerarAleatorio(dados, n);
                    } else if (strcmp(tiposDado[t], "Ordenado") == 0) {
                        gerarOrdenado(dados, n);
                    } else {
                        gerarSemiOrdenado(dados, n);
                    }

                    clock_t inicio = clock();

                    if (strcmp(algoritmos[a], "Heap Sort") == 0) {
                        heapSort(dados, n);
                    } else if (strcmp(algoritmos[a], "Merge Sort") == 0) {
                        mergeSort(dados, 0, n - 1);
                    } else {
                        selectionSort(dados, n);
                    }

                    clock_t fim = clock();
                    tempos[rep] = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

                    if (!estaOrdenado(dados, n)) {
                        todasOrdenadas = 0;
                    }

                    free(dados);
                }


                double soma = 0.0;
                double minimo = tempos[0];
                double maximo = tempos[0];
                for (int rep = 0; rep < REPETICOES; rep++) {
                    soma += tempos[rep];
                    if (tempos[rep] < minimo) minimo = tempos[rep];
                    if (tempos[rep] > maximo) maximo = tempos[rep];
                }
                double media = soma / REPETICOES;
                double desvio = calcularDesvioPadrao(tempos, REPETICOES, media);

                resultados[idx].volume = n;
                strncpy(resultados[idx].tipoDado, tiposDado[t], sizeof(resultados[idx].tipoDado) - 1);
                resultados[idx].tipoDado[sizeof(resultados[idx].tipoDado) - 1] = '\0';
                strncpy(resultados[idx].algoritmo, algoritmos[a], sizeof(resultados[idx].algoritmo) - 1);
                resultados[idx].algoritmo[sizeof(resultados[idx].algoritmo) - 1] = '\0';
                resultados[idx].tempoMedio = media;
                resultados[idx].tempoMinimo = minimo;
                resultados[idx].tempoMaximo = maximo;
                resultados[idx].desvioPadrao = desvio;
                resultados[idx].ordenadoCorretamente = todasOrdenadas;

                printf("Volume: %7d | Tipo: %-14s | Algoritmo: %-15s | Media: %10.6f s | Min: %10.6f s | Max: %10.6f s | DesvPad: %10.6f | Ordenado em todas: %s\n",
                       n, tiposDado[t], algoritmos[a], media, minimo, maximo, desvio,
                       todasOrdenadas ? "SIM" : "NAO");

                idx++;
            }
        }
        printf("\n");
    }

    /* Testes com dados EXTERNOS */
    printf("Lendo dados externos de arquivo...\n\n");

    const char *arquivosExternos[] = {
        "dados_externos_1000.txt",
        "dados_externos_10000.txt",
        "dados_externos_50000.txt",
        "dados_externos_100000.txt"
    };

    for (int v = 0; v < numVolumes; v++) {
        int n = volumes[v];

        int *base = malloc(n * sizeof(int));
        if (base == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria.\n");
            free(resultados);
            return 1;
        }

        int lidos = lerVetorDeArquivo(arquivosExternos[v], base, n);
        if (lidos != n) {
            fprintf(stderr, "Aviso: '%s' deveria ter %d valores, mas foram lidos %d.\n",
                    arquivosExternos[v], n, lidos);
        }

        for (int a = 0; a < numAlgoritmos; a++) {

            double tempos[REPETICOES];
            int todasOrdenadas = 1;

            for (int rep = 0; rep < REPETICOES; rep++) {

                int *dados = malloc(n * sizeof(int));
                if (dados == NULL) {
                    fprintf(stderr, "Erro de alocacao de memoria.\n");
                    free(base);
                    free(resultados);
                    return 1;
                }
                memcpy(dados, base, n * sizeof(int));

                clock_t inicio = clock();

                if (strcmp(algoritmos[a], "Heap Sort") == 0) {
                    heapSort(dados, n);
                } else if (strcmp(algoritmos[a], "Merge Sort") == 0) {
                    mergeSort(dados, 0, n - 1);
                } else {
                    selectionSort(dados, n);
                }

                clock_t fim = clock();
                tempos[rep] = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

                if (!estaOrdenado(dados, n)) {
                    todasOrdenadas = 0;
                }

                free(dados);
            }

            double soma = 0.0;
            double minimo = tempos[0];
            double maximo = tempos[0];
            for (int rep = 0; rep < REPETICOES; rep++) {
                soma += tempos[rep];
                if (tempos[rep] < minimo) minimo = tempos[rep];
                if (tempos[rep] > maximo) maximo = tempos[rep];
            }
            double media = soma / REPETICOES;
            double desvio = calcularDesvioPadrao(tempos, REPETICOES, media);

            resultados[idx].volume = n;
            strncpy(resultados[idx].tipoDado, "Externo", sizeof(resultados[idx].tipoDado) - 1);
            resultados[idx].tipoDado[sizeof(resultados[idx].tipoDado) - 1] = '\0';
            strncpy(resultados[idx].algoritmo, algoritmos[a], sizeof(resultados[idx].algoritmo) - 1);
            resultados[idx].algoritmo[sizeof(resultados[idx].algoritmo) - 1] = '\0';
            resultados[idx].tempoMedio = media;
            resultados[idx].tempoMinimo = minimo;
            resultados[idx].tempoMaximo = maximo;
            resultados[idx].desvioPadrao = desvio;
            resultados[idx].ordenadoCorretamente = todasOrdenadas;

            printf("Volume: %7d | Tipo: %-14s | Algoritmo: %-15s | Media: %10.6f s | Min: %10.6f s | Max: %10.6f s | DesvPad: %10.6f | Ordenado em todas: %s\n",
                   n, "Externo", algoritmos[a], media, minimo, maximo, desvio,
                   todasOrdenadas ? "SIM" : "NAO");

            idx++;
        }

        free(base);
    }
    printf("\n");

    /* Salva tudo em CSV */
    FILE *csv = fopen("resultados.csv", "w");
    if (csv != NULL) {
        fprintf(csv, "Volume,TipoDado,Algoritmo,TempoMedioSegundos,TempoMinimoSegundos,TempoMaximoSegundos,DesvioPadrao,OrdenadoCorretamenteEmTodas_%dExecucoes\n", REPETICOES);
        for (int i = 0; i < totalTestes; i++) {
            fprintf(csv, "%d,%s,%s,%.6f,%.6f,%.6f,%.6f,%s\n",
                    resultados[i].volume,
                    resultados[i].tipoDado,
                    resultados[i].algoritmo,
                    resultados[i].tempoMedio,
                    resultados[i].tempoMinimo,
                    resultados[i].tempoMaximo,
                    resultados[i].desvioPadrao,
                    resultados[i].ordenadoCorretamente ? "SIM" : "NAO");
        }
        fclose(csv);
        printf("Resultados salvos em 'resultados.csv'.\n");
    } else {
        fprintf(stderr, "Nao foi possivel gravar o arquivo CSV.\n");
    }


    printf("\n===== TABELAS RESUMO (tempo em segundos, media de %d execucoes) =====\n", REPETICOES);
    for (int v = 0; v < numVolumes; v++) {
        for (int t = 0; t < numTipos; t++) {
            printf("\n--- Volume: %d | Tipo de dado: %s ---\n", volumes[v], tiposDado[t]);
            printf("%-16s %12s %12s %12s %12s\n", "Algoritmo", "Media", "Minimo", "Maximo", "DesvPad");
            for (int i = 0; i < totalTestes; i++) {
                if (resultados[i].volume == volumes[v] && strcmp(resultados[i].tipoDado, tiposDado[t]) == 0) {
                    printf("%-16s %12.6f %12.6f %12.6f %12.6f\n",
                           resultados[i].algoritmo,
                           resultados[i].tempoMedio,
                           resultados[i].tempoMinimo,
                           resultados[i].tempoMaximo,
                           resultados[i].desvioPadrao);
                }
            }
        }

        printf("\n--- Volume: %d | Tipo de dado: Externo ---\n", volumes[v]);
        printf("%-16s %12s %12s %12s %12s\n", "Algoritmo", "Media", "Minimo", "Maximo", "DesvPad");
        for (int i = 0; i < totalTestes; i++) {
            if (resultados[i].volume == volumes[v] && strcmp(resultados[i].tipoDado, "Externo") == 0) {
                printf("%-16s %12.6f %12.6f %12.6f %12.6f\n",
                       resultados[i].algoritmo,
                       resultados[i].tempoMedio,
                       resultados[i].tempoMinimo,
                       resultados[i].tempoMaximo,
                       resultados[i].desvioPadrao);
            }
        }
    }

    free(resultados);
    return 0;
}
