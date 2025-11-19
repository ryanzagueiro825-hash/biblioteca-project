Projeto Biblioteca - Código Completo
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
void excluir(FILE *arq);
void gerarRelatorio(FILE *arq);
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
 printf("3 - Excluir Livro por Índice\n");
 printf("4 - Gerar Relatório (.txt)\n");
 printf("5 - Ver Quantidade de Registros\n");
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
 excluir(arq);
 break;
 case 4:
 gerarRelatorio(arq);
 break;
 case 5:
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
void excluir(FILE *arq) {
 int indice, total;
 Livro L;
 total = tamanho(arq);
 printf("\nInforme o índice para excluir: ");
 scanf("%d", &indice);
 limpaBuffer();
 if (indice < 0 || indice >= total) {
printf("Índice inválido!\n");
 return;
 }
 FILE *temp = fopen("temp.bin", "w+b");
 if (!temp) {
 printf("Erro ao criar arquivo temporário!\n");
 return;
 }
 fseek(arq, 0, SEEK_SET);
 for (int i = 0; i < total; i++) {
 fread(&L, sizeof(Livro), 1, arq);
 if (i != indice) {
 fwrite(&L, sizeof(Livro), 1, temp);
 }
 }
 fclose(arq);
 fclose(temp);
 remove("biblioteca.bin");
 rename("temp.bin", "biblioteca.bin");
 arq = fopen("biblioteca.bin", "r+b");
 printf("Livro removido com sucesso!\n");
}
void gerarRelatorio(FILE *arq) {
 FILE *txt = fopen("relatorio.txt", "w");
 if (!txt) {
 printf("Erro ao gerar arquivo de relatório!\n");
 return;
 }
 Livro L;
 int total = tamanho(arq);
 fseek(arq, 0, SEEK_SET);
 fprintf(txt, "===== RELATÓRIO DA BIBLIOTECA =====\n\n");
 for (int i = 0; i < total; i++) {
 fread(&L, sizeof(Livro), 1, arq);
 fprintf(txt, "Registro %d\n", i);
 fprintf(txt, "Título: %s\n", L.titulo);
 fprintf(txt, "Ano: %d\n", L.ano);
 fprintf(txt, "Preço: R$ %.2f\n\n", L.preco);
 }
 fclose(txt);
 printf("Relatório gerado com sucesso: relatorio.txt\n");
}
