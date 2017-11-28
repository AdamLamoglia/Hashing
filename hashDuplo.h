#ifndef HASHDUPLO_H
#define HASHDUPLO_H
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <iomanip>


class HashDuplo
{
    public:
        struct registro{
            int chave;
            int idade;
            char nome[20];
        };

        FILE* arquivo;

        HashDuplo(int tamanho);

        int h1(int chave);
        int h2(int chave);

        void insereRegistro();
        void consultaRegistro();
        void imprimeArquivo();
        void calcularMedia();
        void removeRegistro();

        void setQuantidadeDeAcessos(int n);
        int getTamanho();
        int getQuantidadeDeAcessos();


    private:
        int tamanho;
        double quantidadeDeAcessos;
};

#endif // HASHDUPLO_H
