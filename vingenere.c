#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>        
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define tamanho_tabula 27

char alfabeto[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','\0'};

// Os textos sao trabalhados em minusculo, sendo assim necessario a transformacao
void transformaMinusculo(char texto[]) {
    for(int i = 0; i < strlen(texto); i++) {
        if(isupper(texto[i])) 
            texto[i] = tolower(texto[i]);
    }
}

// Gera uma chave aleatoria do mesmo tamanho do texto
void gerarChave(int tamanho, char chave[tamanho]) {
    for(int i = 0; i < tamanho; i++) {
        int valor = rand() % 26;
        chave[i] = 'a' + valor;
    }
}

// Le o arquivo e preenche a matriz com a tabula Recta
void preencherTabula(char tabula[tamanho_tabula][tamanho_tabula]) {
    FILE *fp = fopen("tabula.txt", "r");

    char str[tamanho_tabula + 1];

    int i = 0;

    while(fgets(str, tamanho_tabula + 1, fp) != NULL) {
        for(int j = 0; j < tamanho_tabula - 1; j++) {
            if(str[j] != ' ') 
                tabula[i][j] = str[j];

            str[j] = ' ';
        }

        i++;
    }

    fclose(fp);
}

// Procura a posicao no array correspondente ao alfabeto
int procuraPosicao(char letra) {
    for(int i = 0; i < strlen(alfabeto); i++) {
        if(letra == alfabeto[i])
            return i;
    }
}

// Procura a posicao da chave, necessario pois a chave pode ser menor que o texto
int procuraPosicaoChave(char chave[], int contador) {
    char letra;
    int j = 0;
    for(int i = 0; i <= contador; i++) {
        letra = chave[j];
        j++;
        if(j == strlen(chave))
            j = 0;
    }

    return procuraPosicao(letra);
}

// Procura linha e coluna (chave e texto) e seta a saida na posicao equivalente da tabula
void aplicarCriptografia(char textoSaida[], char texto[], char chave[], char tabulaRecta[tamanho_tabula][tamanho_tabula]) {
    for(int i = 0; i < strlen(texto); i++) {
        int linha = procuraPosicaoChave(chave, i);
        int coluna = procuraPosicao(texto[i]);

        textoSaida[i] = tabulaRecta[linha][coluna];
    }
}

// Procura a linha utilizando a chave
// Com essa linha eh feita a procura dentro da tabula
// O indice da tabula eh correspondente a posicao no array do alfabeto
void reverterCriptografia(char textoSaida[], char texto[], char chave[], char tabulaRecta[tamanho_tabula][tamanho_tabula]) {
    int coluna = 0;
    for(int i = 0; i < strlen(texto); i++) {
        int linha = procuraPosicaoChave(chave, i);

        for(int j = 0; j < tamanho_tabula; j++) {
            if(texto[i] == tabulaRecta[linha][j]) {
                coluna = j;
                break;
            }
        }

        textoSaida[i] = alfabeto[coluna];
    }
}

int main(int argc, char *argv[]) {
    // verifica se há pelo menos o texto para criptografar
    if(argc < 2) {
        printf("Entrada invalida. Esse sistema utiliza argumentos por linha de comando\n");
        printf("A entrada deve seguir esse formato: ./trabalho <texto> <chave>, sendo a chave opcional\n");
        exit(1);
    }
    
    // aloca memoria para o array de char equivalente ao texto
    char *texto = calloc(strlen(argv[1]), sizeof(char));
    texto = argv[1];  
    transformaMinusculo(texto);  

    srand(time(NULL));

    // define a chave 
    // se for passado por parametro o valor é equivalente
    // senao gera uma chave aleatoria com o tamanho do texto
    char *chave;
    if(argc > 2) {
        chave = calloc(strlen(argv[2]), sizeof(char));
        chave = argv[2];
        transformaMinusculo(chave);
    } else {
        chave = calloc(strlen(argv[1]), sizeof(char));
        gerarChave(strlen(argv[1]), chave);
    }

    // define a tabula recta e preenche ela com os valores do arquivo
    char tabulaRecta[tamanho_tabula][tamanho_tabula];
    preencherTabula(tabulaRecta);

    // define a variavel e passa todos os argumentos necessarios para a cifragem
    char *cifragem = calloc(strlen(argv[1]), sizeof(char)); 
    aplicarCriptografia(cifragem, texto, chave, tabulaRecta);

    printf("Cifragem: %s\n", cifragem);

    // define a variavel e passa todos os argumentos necessarios para a decifragem
    char *decifragem = calloc(strlen(argv[1]), sizeof(char));
    reverterCriptografia(decifragem, cifragem, chave, tabulaRecta);

    printf("Decifragem: %s\n", decifragem);

    return 0;
}