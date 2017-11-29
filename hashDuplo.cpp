#include "hashDuplo.h"

using namespace std;

HashDuplo::HashDuplo(int tamanho){
    this->tamanho = tamanho;
}

void HashDuplo::setQuantidadeDeAcessos(int n){
    this->quantidadeDeAcessos = n;
}

int HashDuplo::getQuantidadeDeAcessos(){
    return quantidadeDeAcessos;
}

int HashDuplo::getTamanho(){
    return tamanho;
}

int HashDuplo::h1(int chave){
    return chave%this->tamanho;
}

int HashDuplo::h2(int chave){
    int f = floor(chave/this->tamanho);

    if(chave < this->tamanho || f%this->tamanho == 0){
        return 1;
    }
    return f%this->tamanho;
}

void HashDuplo::insereRegistro(){
    registro abertura,escrita,leitura;

    cin >> escrita.chave;
    cin.ignore();

    cin.getline(escrita.nome,20);

    cin >> escrita.idade;

    abertura.chave = -1;

    arquivo = fopen("teste.bin","r+");

    //se o arquivo nao contem nenhum registro, abre getTamanho() posicoes nele
    if(arquivo == NULL){

        fclose(arquivo);

        arquivo = fopen("teste.bin","w+");

        for(int i = 0;i < 11; i++){
            fseek(arquivo,i*sizeof(registro),SEEK_SET);
            fwrite(&abertura,sizeof(registro),1,arquivo);
        }

        fclose(arquivo);

        arquivo = fopen("teste.bin","rb+");

        fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

        fwrite(&escrita,sizeof(registro),1,arquivo);

        fclose(arquivo);
    }
    //existem registros no arquivo
    else{

        arquivo = fopen("teste.bin","r+");

        fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

        fread(&leitura,sizeof(registro),1,arquivo);

        //o endereco original da chave esta livre
        if(leitura.chave == -1){

            fclose(arquivo);

            arquivo = fopen("teste.bin","rb+");

            fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

            fwrite(&escrita,sizeof(registro),1,arquivo);

            fclose(arquivo);
        }
        //ja foi inserido um registro com chave igual
        else if(leitura.chave == escrita.chave){
            cout << "chave ja existente: " << leitura.chave << endl;
            fclose(arquivo);
        }
        //o endereco original da chave esta ocupado
        else{

            int deslocamento = h1(escrita.chave) + h2(escrita.chave);

            if(deslocamento >= getTamanho())
                deslocamento -= getTamanho();

            fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);
            fread(&leitura,sizeof(registro),1,arquivo);

            if(leitura.chave == -1){
                fclose(arquivo);

                arquivo = fopen("teste.bin","rb+");

                fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

                fwrite(&escrita,sizeof(registro),1,arquivo);

                fclose(arquivo);

            }
            else if(leitura.chave == escrita.chave){
                cout << "chave ja existente: " << leitura.chave << endl;
                fclose(arquivo);
            }

            else{
                //enquanto houver espacos ocupados, incrementa o deslocamento a partir de h2
                while(leitura.chave != -1 && leitura.chave != escrita.chave){
                    deslocamento += h2(escrita.chave);

                    if(deslocamento >= getTamanho())
                        deslocamento -= getTamanho();

                    fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);
                    fread(&leitura,sizeof(registro),1,arquivo);
                }

                if(leitura.chave == -1){
                        fclose(arquivo);

                        arquivo = fopen("teste.bin","rb+");

                        fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

                        fwrite(&escrita,sizeof(registro),1,arquivo);

                        fclose(arquivo);

                    }
                    else if(leitura.chave == escrita.chave){
                        cout << "chave ja existente: " << leitura.chave << endl;

                        fclose(arquivo);
                    }
            }

        }

    }
}



void HashDuplo::consultaRegistro(){
    int chave;
    registro leitura;

    cin >> chave;

    arquivo = fopen("teste.bin","r+");

    fseek(arquivo,h1(chave)*sizeof(registro),SEEK_SET);

    fread(&leitura,sizeof(registro),1,arquivo);


    if(leitura.chave == chave){

        cout << "chave: " << leitura.chave << endl;
        cout << leitura.nome << endl;
        cout << leitura.idade << endl;

    }
    else{
        int deslocamento = h1(chave) + h2(chave);

        if(deslocamento >= getTamanho())
            deslocamento-=getTamanho();

        while(leitura.chave != chave && deslocamento != h1(chave)){

            fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

            fread(&leitura,sizeof(registro),1,arquivo);

            deslocamento += h2(chave);

            if(deslocamento >= getTamanho())
                deslocamento-=getTamanho();
        }

        if(leitura.chave == chave){

            cout << "chave: " << leitura.chave << endl;
            cout << leitura.nome << endl;
            cout << leitura.idade << endl;
        }
        else if(deslocamento == h1(chave))
            cout << "chave nao encontrada: " << chave << endl;

    }

    fclose(arquivo);
}



void HashDuplo::imprimeArquivo(){
    registro leitura;

    arquivo = fopen("teste.bin","r+");

    for(int i = 0; i < getTamanho(); i++){
        fseek(arquivo,i*sizeof(registro),SEEK_SET);

        fread(&leitura,sizeof(registro),1,arquivo);

        cout << i << ": ";

        if(leitura.chave == -1)
            cout << "vazio" << endl;
        else
            cout << leitura.chave << " " << leitura.nome << " " << leitura.idade << endl;
    }
}



void HashDuplo::calcularMedia(){
    registro leitura,acesso;
    double quantidadeDeRegistros = 0;
    setQuantidadeDeAcessos(0);

    arquivo = fopen("teste.bin","r+");

    //percorre o arquivo para calcular a quantidade de acessos de consulta cada registro
    for(int i = 0; i < getTamanho(); i++){

        fseek(arquivo,i*sizeof(registro),SEEK_SET);

        fread(&leitura,sizeof(registro),1,arquivo);

        //se chave != -1, existe um registro no endereco i
        if(leitura.chave != -1){
            fseek(arquivo,h1(leitura.chave)*sizeof(registro),SEEK_SET);

            fread(&acesso,sizeof(registro),1,arquivo);

            //primeiro acesso
            setQuantidadeDeAcessos(getQuantidadeDeAcessos()+1);

            //registro nao esta no endereco natural
            if(leitura.chave != acesso.chave){
                int deslocamento = h1(leitura.chave) + h2(leitura.chave);

                if(deslocamento >= getTamanho())
                    deslocamento-=getTamanho();

                //enquanto elemento nao for encontrado, incrementa a quantidade de acessos
                while(leitura.chave != acesso.chave){

                    fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

                    fread(&acesso,sizeof(registro),1,arquivo);
                    setQuantidadeDeAcessos(getQuantidadeDeAcessos()+1);

                    deslocamento += h2(leitura.chave);

                    if(deslocamento >= getTamanho())
                        deslocamento-=getTamanho();
                }
            }

            quantidadeDeRegistros++;
        }

    }
    cout << fixed;
    cout << setprecision(1) << getQuantidadeDeAcessos()/quantidadeDeRegistros << endl;
}

void HashDuplo::removeRegistro(){
    int chave;
    registro leitura;

    cin >> chave;

    arquivo = fopen("teste.bin","r+");

    fseek(arquivo,h1(chave)*sizeof(registro),SEEK_SET);

    fread(&leitura,sizeof(registro),1,arquivo);

    //registro esta no endereco original
    if(leitura.chave == chave){

        leitura.chave = -1;

        fseek(arquivo,h1(chave)*sizeof(registro),SEEK_SET);

        fwrite(&leitura,sizeof(registro),1,arquivo);


    }
    else{
        int deslocamento = h1(chave) + h2(chave);

        if(deslocamento >= getTamanho())
            deslocamento-=getTamanho();

        fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

        fread(&leitura,sizeof(registro),1,arquivo);

        while(leitura.chave != chave && deslocamento != h1(chave)){

            fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

            fread(&leitura,sizeof(registro),1,arquivo);

            deslocamento += h2(chave);

            if(deslocamento >= getTamanho())
                deslocamento-=getTamanho();
        }

        if(leitura.chave == chave){

            leitura.chave = -1;

            fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

            fwrite(&leitura,sizeof(registro),1,arquivo);
        }
        else if(deslocamento == h1(chave))
            cout << "chave nao encontrada: " << chave << endl;

    }

    fclose(arquivo);
}


