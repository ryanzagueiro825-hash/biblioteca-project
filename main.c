#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <locale.h>
 typedef struct {
    char titulo[50];
    int ano;
    float preco;
 } Livro;
 void limpaBuffer();
 int tamanho(FILE *arq);
 void cadastrar(FILE *arq);
 void consultar(FILE *arq);
 int main() {
    setlocale(LC_ALL, "Portuguese");
    FILE *arq = fopen("biblioteca.bin", "r+b");
    if (arq == NULL) {
        arq = fopen("biblioteca.bin", "w+b");
        if (arq == NULL) {
            printf("Erro ao criar arquivo!\n");
            return 1;
        }
    }
    int opcao;
    do {
        printf("\n========== BIBLIOTECA PESSOAL ==========\n");
        printf("1 - Cadastrar Livro\n");
        printf("2 - Consultar Livro por Índice\n");
        printf("3 - Ver Quantidade de Registros\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpaBuffer();
        switch (opcao) {
            case 1:
                cadastrar(arq);
                break;
            case 2:
                consultar(arq);
                break;
            case 3:
                printf("Total de registros: %d\n", tamanho(arq));
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    fclose(arq);
    return 0;
 }
 void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
 }
 int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    long bytes = ftell(arq);
    return bytes / sizeof(Livro);
}
 void cadastrar(FILE *arq) {
    Livro L;
    printf("\nTítulo do livro: ");
    fgets(L.titulo, 50, stdin);
    L.titulo[strcspn(L.titulo, "\n")] = '\0';
    printf("Ano de publicação: ");
    scanf("%d", &L.ano);
    limpaBuffer();
    printf("Preço: ");
    scanf("%f", &L.preco);
    limpaBuffer();
    fseek(arq, 0, SEEK_END);
    fwrite(&L, sizeof(Livro), 1, arq);
    printf("Livro cadastrado com sucesso!\n");
 }
 void consultar(FILE *arq) {
    int indice;
    Livro L;
    printf("\nInforme o índice do registro: ");
    scanf("%d", &indice);
    limpaBuffer();
    int total = tamanho(arq);
    if (indice < 0 || indice >= total) {
        printf("Índice inválido! Total de registros: %d\n", total);
        return;
    }
    fseek(arq, indice * sizeof(Livro), SEEK_SET);
    fread(&L, sizeof(Livro), 1, arq);
    printf("\n===== DADOS DO LIVRO =====\n");
    printf("Título: %s\n", L.titulo);
    printf("Ano: %d\n", L.ano);
    printf("Preço: R$ %.2f\n", L.preco);
 }