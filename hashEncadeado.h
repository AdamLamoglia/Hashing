#ifndef HASHENCADEADO_H
#define HASHENCADEADO_H
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <iomanip>


class HashEncadeado
{
    public:
        struct registro{
            int chave;
            int idade;
            char nome[20];
            int proximo;
        };

        FILE* arquivo;

        HashEncadeado(int tamanho);

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

#endif // HASHENCADEADO_H
