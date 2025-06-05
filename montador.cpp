#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char mnemon[4];
    unsigned char opcode;
    int memoria; 
} instrucoes;

instrucoes comandos[] = {
    {"NOP", 0x00, 0},
    {"STA", 0x10, 1},
    {"LDA", 0x20, 1},
    {"ADD", 0x30, 1},
    {"OR", 0x40, 1},
    {"AND", 0x50, 1},
    {"NOT", 0x60, 0},
    {"JMP", 0x70, 1},
    {"JN", 0x80, 1},
    {"JZ", 0x90, 1},
    {"HLT", 0xF0, 0},
};

int main(){
    FILE *arqInicial, *arqFinal;
    char linhas[255], mnemon[4];
    int ler, memoria, i;

    arqInicial = fopen("input.asm", "r");
    arqFinal = fopen("output.mem", "wb");

    char prefixo[] = {0x03, 0x4E, 0x44, 0x52};
    fwrite(prefixo, sizeof(char), 4, arqFinal);

    while(fgets(linhas, sizeof(linhas), arqInicial)){     
        for(i=0; i<255; i++) {
            if(strcmp(mnemon, comandos[i].mnemon) == 0) {
                fputc(comandos[i].opcode, arqFinal);

                if(comandos[i].memoria) {
                    fputc((char)memoria, arqFinal);
                }
                break;
            }
        }
    }
    
    //ainda esta dando erro no alocamento de memoria

    fclose(arqInicial);
    fclose(arqFinal);

    printf("Arquivo output.mem gerado!");
    return 0;
}