#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define NUM_RESTAURANTES 10

int mochila = 0, meta = 0, x = 1, bancada[NUM_RESTAURANTES];
pthread_mutex_t m;

void *Entregador(void *argumento) { 
    if(meta < 50) {
        x = 0;
        pthread_mutex_lock(&m);
        printf("O entregador saiu do ponto A para entregar a comida no ponto B\n\n");
        mochila = 0; 
        pthread_mutex_unlock(&m);
        sleep(15);
        printf("\nO entregador voltou para o ponto A\n\n");
        x = 1;
    }
    else {
        printf("\nRefeiçoes nao entregues: %d\n", mochila);
    }
}

void *Funcionario(void *i) {
    int j;
    pthread_mutex_lock(&m);
    pthread_t entregador_id; 
    for(j = 0; j < NUM_RESTAURANTES; j++) {
        if(bancada[j] == 1) {
            mochila++;
            printf("A comida na bancada do restaurante %d foi colocada na mochila\n", j);
            bancada[j] = 0;
        }
    }
    if(mochila < 10) {
        printf("O funcionario do restaurante %ld colocou a comida na mochila\n", (long)i);
        mochila++;
    }
    else{
        bancada[(long)i] = 1;
    }

    if(mochila == 10 && x == 1){
        meta += 10;
        printf("\nUm dos funcionarios acordou o entregador\n");
        pthread_create(&entregador_id, NULL, Entregador, NULL);
    }
    pthread_mutex_unlock(&m);
}

void *Restaurante(void *i) {

    pthread_t funcionario_id;
    int tempo;
    tempo = rand() % 50;
    sleep(tempo);
    printf("O restaurante %ld finalizou o preparo da comida, tempo de preparo: %d segundos\n", (long)i, tempo);
    if(x == 0) {
        printf("O restaurante %ld colocou a comida na bancada\n", (long)i);
        bancada[(long)i] = 1;
    }
    else {
        bancada[(long)i] = 0;
        pthread_create(&funcionario_id, NULL, Funcionario, (void *)i);
    }
}



int main (int argc, char *argv[]){

    pthread_t restaurantes[NUM_RESTAURANTES];
    int rc, random; 
    long i;


    pthread_mutex_init(&m, NULL);

    while(meta < 50) {
        for(i = 0; i < NUM_RESTAURANTES; i++){
            rc = pthread_create(&restaurantes[i], NULL, Restaurante,  (void *)i);
            if(rc) {
                exit(-1);
            }
        }
        for(i = 0; i < NUM_RESTAURANTES; i++) {
            pthread_join(restaurantes[i], NULL);
        }
    }
    printf("\nOs restaurantes atingiram a meta de %d refeições entregues\n", meta);

}