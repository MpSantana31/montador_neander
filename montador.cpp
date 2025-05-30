#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    FILE *arquivoInicial, *arquivoFinal;
    int num = 0;
    char comandos[4];

    //Abrindo o arquivo "teste.txt" - nao to usando o asm por enquanto pq fica mais facil de testar

    arquivoInicial = fopen("teste.txt", "r");
        if(arquivoInicial == NULL){
            printf("Erro");
            return 1;
        }
    
    //Le oq ta escrito no teste.txt e fecha o arquivo
    fscanf(arquivoInicial, "%s", comandos);
    fclose(arquivoInicial);
    
    /*
    //Ainda vo arrumar a parte dos comandos
    if (strcmp(comandos, "LDA") == 0){
        num = 20;
    }
    */
       
    //Abre o arquivo final "final.txt", tbm to usando o txt pra conseguir ler por enquanto
    arquivoFinal = fopen("final.txt", "w");
        if(arquivoFinal == NULL){
            printf("Erro");
            return 1;
        }
    
    //Coloca o prefixo "03 4E 44 52" que inicializa o neander
    unsigned char prefixo[] = {0x03, 0x4E, 0x44, 0x52};
    fwrite(prefixo, sizeof(unsigned char), 4, arquivoFinal);
    
    fprintf(arquivoFinal, "%i", num);
    fclose(arquivoFinal);

    return 0;
}