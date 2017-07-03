/*
 ============================================================================
 Name        : OptativaRedes.c
 Author      : Wagner Frana
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Calcula o número de Slots necessários
int CalculoSlots(int Gb, int mod){
    //Teste se for BPSK
    if(mod == 0)return ceil(Gb / 12.5);

    //QPSK
    if(mod == 1)return ceil(Gb / 25);

    //8QAM
    if(mod == 2)return ceil(Gb / 37.5);

}

//Função que imprime resultados no arquivo
void imprimeResultadosArquivo(FILE *arq, int modMF, int linka, int linkb){
    if(modMF == 0){
        fprintf(arq, "\nFormato de Modulação Usado nos Links: %d -> %d : BPSK", linka+1, linkb+1);
    }else if(modMF == 1){
        fprintf(arq, "\nFormato de Modulação Usado nos Links: %d -> %d : QPSK", linka+1, linkb+1);
    }else{
        fprintf(arq, "\nFormato de Modulação Usado nos Links: %d -> %d : 8QAM", linka+1, linkb+1);
    }

}



int main(void) {
    FILE *arq;
    int Demandas[4][4];
    int NSlots[4][2];
    int i, j, d, aux, modulacao, modMF, totalNumSlots;
    d=totalNumSlots=0;

    srand(time(NULL));

    printf("Digite o valor do Gbps usado para os links : ");
    scanf("%d", &d);

    printf("Digite o formato de modulação : - 0=BPSK e 1=Multi-MF : ");

    scanf("%d", &modulacao);


    //Prenche matriz de demandas com o valor em Gbps
    for(i=0; i < 4; i++){
        for(j=0; j<4; j++){
            if((i==0 && j==1) || (i==0 && j==3) ||  (i==1 && j==0) || (i==1 && j==2) ||
                    (i==2 && j==1) || (i==2 && j==3) || (i==3 && j==0) || (i==3 && j==2)){
                Demandas[i][j]= d;

            }else{
                Demandas[i][j]=0;
            }
        }
    }

    arq = fopen("saida.txt", "w");
    if(arq == NULL){
        printf("Erro, nao foi possivel abrir o arquivo\n");
        return 0;
    }

    //Percorre as demandas e cálcula o número de slots necessário
    for(i=0; i < 4; i++){
            aux = 0;
            for(j=0; j<4; j++){
                if(Demandas[i][j] != 0){
                    if(modulacao==0){

                        NSlots[i][aux] = CalculoSlots(Demandas[i][j], modulacao);
                        if(i==0 && aux==0)fprintf(arq, "\nFormato de Modulação Utilizado : BPSK\n\n");
                    }else{
                        modMF = rand() % 3;
                        printf("\n\n[%d]", modMF);
                        NSlots[i][aux] = CalculoSlots(Demandas[i][j], modMF);
                        imprimeResultadosArquivo(arq, modMF, i, j);
                    }

                    aux++;
                }
            }

    }

    printf("\n\n\n\n");
    fprintf(arq,"\n\n\n");


    for(i=0; i < 4; i++){
        aux = 0;
        for(j=0; j<4; j++){
            if(Demandas[i][j] != 0){
                printf("\nNúmero Slots %d -> %d : %d", i+1, j+1, NSlots[i][aux]);
                fprintf(arq, "\nNúmero Slots Usados no Link: %d -> %d : %d", i+1, j+1, NSlots[i][aux]);
                totalNumSlots = totalNumSlots + NSlots[i][aux];
                aux++;

            }
        }
    }

    fprintf(arq, "\n\nTotal de Slots Utilizado : %d\n\n", totalNumSlots);

    fclose (arq);




    return EXIT_SUCCESS;
}
