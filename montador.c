#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
    FILE *arquivoInicial, *arquivoFinal;
    char linha[100];
    char output_filename[] = "output.mem";

    typedef struct {
        char *mnemonicos;
        unsigned char opcodes;
        int has_operand;
    } instrucao;
    
    // Tabela de instruções do Neander

    instrucao instrucoes[] = {
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

    // Abrir arquivo de entrada

    arquivoInicial = fopen("input.asm", "r");
    if (arquivoInicial == NULL) {
        printf("Erro ao abrir arquivo de entrada\n");
        return 1;
    }
    
    // Abrir arquivo de saída em modo binário

    arquivoFinal = fopen(output_filename, "wb");
    if (arquivoFinal == NULL) {
        printf("Erro ao criar arquivo de saída\n");
        fclose(arquivoInicial);
        return 1;
    }
    
    // Escrever cabeçalho fixo

    unsigned char prefixo[] = {0x03, 0x4E, 0x44, 0x52};
    fwrite(prefixo, sizeof(unsigned char), 4, arquivoFinal);

    // Processar cada linha do arquivo de entrada

    while (fgets(linha, sizeof(linha), arquivoInicial)) {

        // Remover comentários (tudo após ';')

        char *comment = strchr(linha, ';');
        if (comment) *comment = '\0';

        // Remover espaços em branco

        int i = 0, j = 0;
        while (isspace(linha[i])) i++;
        
        j = i;
        while (linha[j] != '\0') j++;
        
        while (j > i && isspace(linha[j-1])) j--;
        
        linha[j] = '\0';
        
        if (i > 0) memmove(linha, linha+i, j-i+1);

        // Ignorar linhas vazias

        if (strlen(linha) == 0) continue;

        // Extrair mnemônico e operando

        char mnem[10];
        unsigned int operando = 0;
        int lidos = sscanf(linha, "%9s %x", mnem, &operando);
        
        // Procurar instrução na tabela

        int encontrado = -1;
        for (int k = 0; k < 11; k++) {
            if (strcasecmp(mnem, instrucoes[k].mnemonicos) == 0) {
                encontrado = k;
                break;
            }
        }
        
        if (encontrado == -1) {
            printf("Erro: Instrução desconhecida '%s'\n", mnem);
            continue;
        }
        
        // Verificar se operando é necessário
        if (instrucoes[encontrado].has_operand && lidos < 2) {
            printf("Erro: Instrução '%s' requer operando\n", mnem);
            continue;
        }
        
        // Escrever opcode seguido de 00
        unsigned char opcode = instrucoes[encontrado].opcodes;
        unsigned char zero = 0x00;
        fwrite(&opcode, 1, 1, arquivoFinal);
        fwrite(&zero, 1, 1, arquivoFinal);
        
        // Se tiver operando escreve também
        if (instrucoes[encontrado].has_operand) {
            unsigned char operando_byte = (unsigned char)(operando & 0xFF);
            fwrite(&operando_byte, 1, 1, arquivoFinal);
            fwrite(&zero, 1, 1, arquivoFinal);
        }

    }
    
    // Fechar arquivos
    fclose(arquivoInicial);
    fclose(arquivoFinal);

    printf("Montagem concluída com sucesso. Arquivo gerado: %s\n", output_filename);
    return 0;
}