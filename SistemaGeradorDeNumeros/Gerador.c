#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Gera um arquivo de texto com 'quantidade' numeros inteiros aleatorios, */
/* um por linha, entre 0 e 999999.                                       */
void gerarArquivo(const char *nomeArquivo, int quantidade) {
    FILE *f = fopen(nomeArquivo, "w");
    if (f == NULL) {
        fprintf(stderr, "Erro ao criar o arquivo '%s'.\n", nomeArquivo);
        exit(1);
    }
    for (int i = 0; i < quantidade; i++) {
        fprintf(f, "%d\n", rand() % 1000000);
    }
    fclose(f);
    printf("Criado: %s (%d numeros)\n", nomeArquivo, quantidade);
}

int main(void) {
    srand((unsigned int) time(NULL));

    gerarArquivo("dados_externos_1000.txt", 1000);
    gerarArquivo("dados_externos_10000.txt", 10000);
    gerarArquivo("dados_externos_50000.txt", 50000);
    gerarArquivo("dados_externos_100000.txt", 100000);

    printf("\nPronto. Os 4 arquivos foram criados na mesma pasta onde este programa rodou.\n");
    return 0;
}
