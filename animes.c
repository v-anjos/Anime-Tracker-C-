#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define BAR 20

#define GREEN "\033[0;32m"
#define CYAN "\033[0;36m"
#define YELLOW "\033[1;33m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

struct Anime{

    char nome[100];
    int temporada;

    int total_ep;
    int ep_assistido;

    int status; // 0 assistindo,1 completo,2 planejado,3 dropado

};

struct Anime animes[MAX];

int qtd=0;

char* statusTexto(int s){

    if(s==0) return "Assistindo";
    if(s==1) return "Completo";
    if(s==2) return "Planejado";
    if(s==3) return "Dropado";

    return "";
}

void salvar(){

    FILE *f=fopen("animes.txt","w");

    if(!f) return;

    for(int i=0;i<qtd;i++){

        fprintf(f,"%s|%d|%d|%d|%d\n",

        animes[i].nome,
        animes[i].temporada,
        animes[i].total_ep,
        animes[i].ep_assistido,
        animes[i].status);
    }

    fclose(f);
}

void carregar(){

    FILE *f=fopen("animes.txt","r");

    if(!f) return;

    while(fscanf(f,"%[^|]|%d|%d|%d|%d\n",

    animes[qtd].nome,
    &animes[qtd].temporada,
    &animes[qtd].total_ep,
    &animes[qtd].ep_assistido,
    &animes[qtd].status)!=EOF){

        qtd++;
    }

    fclose(f);
}

void barra(int assistido,int total){

    float p=(float)assistido/total;

    int filled=p*BAR;

    printf("[");

    for(int i=0;i<filled;i++)
        printf(GREEN "#");

    for(int i=filled;i<BAR;i++)
        printf("-");

    printf(RESET "] %.0f%%",p*100);
}

float progresso(int i){

    return (float)animes[i].ep_assistido/animes[i].total_ep;

}

void cadastrar(){

    printf("\nNome do anime: ");
    scanf(" %[^\n]",animes[qtd].nome);

    printf("Temporada: ");
    scanf("%d",&animes[qtd].temporada);

    printf("Total de episodios: ");
    scanf("%d",&animes[qtd].total_ep);

    printf("\nStatus:\n");
    printf("0 Assistindo\n");
    printf("1 Completo\n");
    printf("2 Planejado\n");
    printf("3 Dropado\n");

    scanf("%d",&animes[qtd].status);

    animes[qtd].ep_assistido=0;

    qtd++;

    salvar();

    printf(GREEN "Anime cadastrado!\n" RESET);
}

void listar(){

    if(qtd==0){

        printf(RED "\nNenhum anime cadastrado\n" RESET);
        return;
    }

    printf(CYAN "\n=========== DASHBOARD ANIME ===========\n\n" RESET);

    for(int i=0;i<qtd;i++){

        int rest=animes[i].total_ep-animes[i].ep_assistido;

        printf(YELLOW "%d - %s S%d [%s] " RESET,

        i,
        animes[i].nome,
        animes[i].temporada,
        statusTexto(animes[i].status));

        barra(animes[i].ep_assistido,
              animes[i].total_ep);

        printf(" (%d/%d)",

        animes[i].ep_assistido,
        animes[i].total_ep);

        printf(" faltam %d ep\n",rest);
    }
}

void listarCategoria(){

    int cat;

    printf("\nCategorias:\n");
    printf("0 Assistindo\n");
    printf("1 Completo\n");
    printf("2 Planejado\n");
    printf("3 Dropado\n");

    printf("\nEscolha: ");
    scanf("%d",&cat);

    printf(CYAN "\n======= %s =======\n\n" RESET,statusTexto(cat));

    for(int i=0;i<qtd;i++){

        if(animes[i].status==cat){

            printf("%s S%d\n",
            animes[i].nome,
            animes[i].temporada);
        }
    }
}

void marcar(){

    listar();

    int id;

    printf("\nEscolha o anime: ");
    scanf("%d",&id);

    if(id<0||id>=qtd){

        printf(RED "ID invalido\n" RESET);
        return;
    }

    printf("Episodio assistido: ");
    scanf("%d",&animes[id].ep_assistido);

    salvar();
}

void buscar(){

    char nome[100];

    printf("\nBuscar anime: ");
    scanf(" %[^\n]",nome);

    for(int i=0;i<qtd;i++){

        if(strstr(animes[i].nome,nome)){

            printf(GREEN "\nEncontrado:\n" RESET);

            printf("%s S%d [%s] ",

            animes[i].nome,
            animes[i].temporada,
            statusTexto(animes[i].status));

            barra(animes[i].ep_assistido,
                  animes[i].total_ep);

            printf("\n");
        }
    }
}

void remover(){

    listar();

    int id;

    printf("\nRemover anime: ");
    scanf("%d",&id);

    if(id<0||id>=qtd) return;

    for(int i=id;i<qtd-1;i++)
        animes[i]=animes[i+1];

    qtd--;

    salvar();

    printf(RED "Anime removido\n" RESET);
}

void ranking(){

    int idx[MAX];

    for(int i=0;i<qtd;i++)
        idx[i]=i;

    for(int i=0;i<qtd;i++){

        for(int j=i+1;j<qtd;j++){

            if(progresso(idx[j])>progresso(idx[i])){

                int tmp=idx[i];
                idx[i]=idx[j];
                idx[j]=tmp;
            }
        }
    }

    printf(CYAN "\n======= RANKING DE PROGRESSO =======\n\n" RESET);

    for(int i=0;i<qtd;i++){

        int k=idx[i];

        printf("%dº %s S%d [%s] ",

        i+1,
        animes[k].nome,
        animes[k].temporada,
        statusTexto(animes[k].status));

        barra(animes[k].ep_assistido,
              animes[k].total_ep);

        printf("\n");
    }
}

int main(){

    int op;

    carregar();

    do{

        printf(CYAN "\n=========== ANIME TRACKER ===========\n" RESET);

        printf("1 Cadastrar anime\n");
        printf("2 Marcar episodio\n");
        printf("3 Dashboard\n");
        printf("4 Buscar anime\n");
        printf("5 Remover anime\n");
        printf("6 Ranking de progresso\n");
        printf("7 Listar por categoria\n");
        printf("0 Sair\n");

        printf("\nEscolha: ");
        scanf("%d",&op);

        switch(op){

            case 1:
                cadastrar();
                break;

            case 2:
                marcar();
                break;

            case 3:
                listar();
                break;

            case 4:
                buscar();
                break;

            case 5:
                remover();
                break;

            case 6:
                ranking();
                break;

            case 7:
                listarCategoria();
                break;

        }

    }while(op!=0);
return 0;
}
