#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *mnemonicos;
    unsigned char opcodes;
    int has_operand;
} instrucao;

instrucao instrucoes[] = {
    {"NOP", 0x00, 0},
    {"STA", 0x10, 1},
    {"LDA", 0x20, 1},
    {"ADD", 0x30, 1},
    {"OR", 0x40, 1},
    {"AND", 0x50, 1},
    {"NOT", 0x60, 0},
    {"JMP", 0x80, 1},
    {"JN", 0x90, 1},
    {"JZ", 0xA0, 1},
    {"HLT", 0xF0, 0},
};

int busca_instrucao(char *mnem, instrucao *instrucoes) {
    for (int i = 0; i < 12; i++) {
        if (strcasecmp(mnem, instrucoes[i].mnemonicos) == 0) {
            return i;
        }
    }
    return -1;
}

int preprocessarlinha(char *linha) {
    char *comment = strchr(linha, ';');
    if (comment) *comment = '\0';

    int i = 0, j = 0;
    while (isspace(linha[i])) i++;
    j = i;
    while (linha[j] != '\0') j++;
    while (j > i && isspace(linha[j-1])) j--;
    linha[j] = '\0';

    if (i > 0) memmove(linha, linha+i, j-i+1);

    return strlen(linha) > 0;
}

int main() {
    FILE *arquivoinicial, *arquivofinal;
    char linha[100];
    char output_filename[100];
    char input_filename[100];

    printf("Digite o nome do arquivo de entrada (exemplo: input.asm): ");
    scanf("%99s", input_filename);

    arquivoinicial = fopen(input_filename, "r");
    if (arquivoinicial == NULL) {
        printf("Erro ao abrir arquivo de entrada\n");
        return 1;
    }
    
    printf("Digite o nome do arquivo de saída (exemplo: output.mem): ");
    scanf("%99s", output_filename);

    arquivofinal = fopen(output_filename, "wb");
    if (arquivofinal == NULL) {
        printf("Erro ao criar arquivo de saída\n");
        fclose(arquivoinicial);
        return 1;
    }
    
    unsigned char prefixo[] = {0x03, 0x4E, 0x44, 0x52};
    fwrite(prefixo, sizeof(unsigned char), 4, arquivofinal);

    while (fgets(linha, sizeof(linha), arquivoinicial)) {

        if (!preprocessarlinha(linha)) {
            continue;
        }

        char mnem[10];
        unsigned int operando = 0;
        int lidos = sscanf(linha, "%9s %x", mnem, &operando);
        
        int encontrado = busca_instrucao(mnem, instrucoes);
        
        if (encontrado == -1) {
            printf("Erro: Instrução desconhecida '%s'\n", mnem);
            continue;
        }
        
        if (instrucoes[encontrado].has_operand && lidos < 2) {
            printf("Erro: Instrução '%s' requer operando\n", mnem);
            continue;
        }
        
        unsigned char opcode = instrucoes[encontrado].opcodes;
        unsigned char zero = 0x00;
        fwrite(&opcode, 1, 1, arquivofinal);
        fwrite(&zero, 1, 1, arquivofinal);
        
        if (instrucoes[encontrado].has_operand) {
            unsigned char operando_byte = (unsigned char)(operando & 0xFF);
            fwrite(&operando_byte, 1, 1, arquivofinal);
            fwrite(&zero, 1, 1, arquivofinal);
        }

    }
    
    fclose(arquivoinicial);
    fclose(arquivofinal);

    printf("Montagem concluída com sucesso. Arquivo gerado: %s\n", output_filename);
    return 0;
}