#include "hashEncadeado.h"

using namespace std;

HashEncadeado::HashEncadeado(int tamanho){
    this->tamanho = tamanho;
}

void HashEncadeado::setQuantidadeDeAcessos(int n){
    this->quantidadeDeAcessos = n;
}

int HashEncadeado::getQuantidadeDeAcessos(){
    return quantidadeDeAcessos;
}

int HashEncadeado::getTamanho(){
    return tamanho;
}

int HashEncadeado::h1(int chave){
    return chave%this->tamanho;
}

int HashEncadeado::h2(int chave){
    int f = floor(chave/this->tamanho);

    if(chave < this->tamanho || f%this->tamanho == 0){
        return 1;
    }
    return f%this->tamanho;
}

void HashEncadeado::insereRegistro(){
    int deslocamento,posicao,posicaoFinal,posicaoAnterior,contador = 1;
    registro abertura,escrita,leitura,novaLeitura,anterior;

    posicao = -1;

    cin >> escrita.chave >> escrita.idade;
    cin.ignore();

    cin.getline(escrita.nome,20);

    abertura.chave = -1;
    abertura.proximo = -1;

    arquivo = fopen("teste.bin","r+");

    if(arquivo == NULL){
        escrita.proximo = -1;

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
    else{

        arquivo = fopen("teste.bin","r+");

        fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

        fread(&leitura,sizeof(registro),1,arquivo);


        if(leitura.chave == -1){
            escrita.proximo = -1;

            fclose(arquivo);

            arquivo = fopen("teste.bin","rb+");

            fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

            fwrite(&escrita,sizeof(registro),1,arquivo);

            fclose(arquivo);
        }
        else if(leitura.chave == escrita.chave){
            cout << "chave ja existente: " << leitura.chave << endl;
            fclose(arquivo);
        }
        else{
            deslocamento = h1(escrita.chave);
            posicao = (ftell(arquivo) - 1)/sizeof(registro);

            //desloca para comparar se o elemento do endereco atual tem o mesmo mod do endereco que quero inserir
            //para poder ficar cada um em sua lista
            fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

            fread(&leitura,sizeof(registro),1,arquivo);

            //substitui o registro que esta la para poder desloca-lo para outro lugar
            if(h1(escrita.chave) != h1(leitura.chave)){

                //adiciona o registro na ultima posicao vazia do arquivo
                deslocamento = getTamanho() - 1;

                fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

                fread(&novaLeitura,sizeof(registro),1,arquivo);


                //sobe no arquivo ate achar a ultima posicao livre (do final para o incio
                while(novaLeitura.chave != -1){
                    deslocamento--;

                    fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

                    posicaoFinal = ftell(arquivo)/sizeof(registro);

                    fread(&novaLeitura,sizeof(registro),1,arquivo);

                    cout << novaLeitura.chave << " " << posicaoFinal << endl;

                }

                fseek(arquivo,(-1)*sizeof(registro),SEEK_CUR);

                fwrite(&leitura,sizeof(registro),1,arquivo);


                //atualiza ponteiro do anterior
                fseek(arquivo,h1(leitura.chave)*sizeof(registro),SEEK_SET);

                fread(&novaLeitura,sizeof(registro),1,arquivo);

                posicaoAnterior = ftell(arquivo)/sizeof(registro);

                //enquanto nao eh o anterior do registro
                while(novaLeitura.chave != leitura.chave && novaLeitura.proximo != -1){

                    posicaoAnterior = ftell(arquivo)/sizeof(registro);

                    fseek(arquivo,novaLeitura.proximo*sizeof(registro),SEEK_SET);

                    fread(&novaLeitura,sizeof(registro),1,arquivo);
                }

                //atualiza ponteiro do anterior
                fseek(arquivo,posicaoAnterior*sizeof(registro),SEEK_SET);

                fread(&novaLeitura,sizeof(registro),1,arquivo);

                cout << "posicao do anterior: " << posicaoAnterior << endl;
                cout << "ponteiro do anterior: " << posicaoFinal << endl;

                novaLeitura.proximo = posicaoFinal;


                fclose(arquivo);

                arquivo = fopen("teste.bin","r+");

                cout << "chave do anterior da lista: " << novaLeitura.chave << endl;
                cout << "idade: " << novaLeitura.idade << endl;
                cout << "nome: " << novaLeitura.nome << endl;
                cout << "proximo: " << novaLeitura.proximo << endl;

                //atualiza anterior
                fseek(arquivo,posicaoAnterior*sizeof(registro),SEEK_SET);

                fwrite(&novaLeitura,sizeof(registro),1,arquivo);

                cout << "chave do atual da lista: " << escrita.chave << endl;
                cout << "idade: " << escrita.idade << endl;
                cout << "nome: " << escrita.nome << endl;
                cout << "proximo: " << escrita.proximo << endl;

                //atualiza a posicao original do elemento a ser inserido
                fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

                fwrite(&escrita,sizeof(registro),1,arquivo);

                cout << "chave do deslocado da lista: " << leitura.chave << endl;
                cout << "idade: " << leitura.idade << endl;
                cout << "nome: " << leitura.nome << endl;
                cout << "proximo: " << leitura.proximo << endl;

                //atualiza a posicao do deslocado
                fseek(arquivo,posicaoFinal*sizeof(registro),SEEK_SET);

                fwrite(&leitura,sizeof(registro),1,arquivo);

                fclose(arquivo);

            }
            else{
                while(leitura.proximo != -1){

                    fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);

                    fread(&leitura,sizeof(registro),1,arquivo);

                    if(leitura.proximo != -1)
                        deslocamento = leitura.proximo;

                    posicao = (ftell(arquivo) - 1)/sizeof(registro);


                }

                while(leitura.chave != -1 && leitura.chave != escrita.chave){
                    fseek(arquivo,(-1)*sizeof(registro),SEEK_CUR);

                    deslocamento++;

                    if(deslocamento >= getTamanho())
                        deslocamento = 0;

                        fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

                        fread(&leitura,sizeof(registro),1,arquivo);

                }
                if(leitura.chave == -1){

                            fseek(arquivo,posicao*sizeof(registro),SEEK_SET);

                            fread(&abertura,sizeof(registro),1,arquivo);

                            fclose(arquivo);

                            abertura.proximo = deslocamento;

                            //cout << abertura.proximo << endl;
                            //cout << posicao << endl;

                            arquivo = fopen("teste.bin","rb+");

                            abertura.proximo = deslocamento;

                            fseek(arquivo,posicao*sizeof(registro),SEEK_SET);

                            fwrite(&abertura,sizeof(registro),1,arquivo);

                            fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

                            escrita.proximo = -1;

                            fwrite(&escrita,sizeof(registro),1,arquivo);

                            fclose(arquivo);

                }

                else if(leitura.chave == escrita.chave){
                        cout << "chave ja existente: " << leitura.chave << endl;

                        fclose(arquivo);
                }

                fclose(arquivo);
            }

        }
    }
}

void HashEncadeado::consultaRegistro(){

    registro leitura;
    int chave;

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

        while(leitura.proximo != -1){

                fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);

                fread(&leitura,sizeof(registro),1,arquivo);

                if(leitura.chave == chave)
                    break;

        }

        if(leitura.chave == chave){
            cout << "chave: " << leitura.chave << endl;
            cout << leitura.nome << endl;
            cout << leitura.idade << endl;
        }
        else{
            cout << "chave nao encontrada: " << chave << endl;
        }

    }

    fclose(arquivo);
}

void HashEncadeado::imprimeArquivo(){
    registro leitura;

    arquivo = fopen("teste.bin","r+");

    for(int i = 0; i < getTamanho(); i++){
        fseek(arquivo,i*sizeof(registro),SEEK_SET);

        fread(&leitura,sizeof(registro),1,arquivo);

        cout << i << ": ";

        if(leitura.chave <= -1)
            cout << "vazio" << endl;
        else{
            cout << leitura.chave << " " << leitura.nome << " " << leitura.idade;

            if(leitura.proximo == -1)
                cout << " nulo" << endl;
            else
                cout << " " << leitura.proximo << endl;
        }
    }

    fclose(arquivo);
}

void HashEncadeado::calcularMedia(){
    registro leitura,acesso;
    double quantidadeDeRegistros = 0;
    setQuantidadeDeAcessos(0);

    arquivo = fopen("teste.bin","r+");

    for(int i = 0; i < getTamanho(); i++){

        fseek(arquivo,i*sizeof(registro),SEEK_SET);

        fread(&leitura,sizeof(registro),1,arquivo);

        if(leitura.chave != -1){
            fseek(arquivo,h1(leitura.chave)*sizeof(registro),SEEK_SET);

            fread(&acesso,sizeof(registro),1,arquivo);

            //primeiro acesso
            setQuantidadeDeAcessos(getQuantidadeDeAcessos()+1);

            if(leitura.chave != acesso.chave){

                 while(acesso.proximo != -1){
                    setQuantidadeDeAcessos(getQuantidadeDeAcessos()+1);

                    fseek(arquivo,acesso.proximo*sizeof(registro),SEEK_SET);

                    fread(&acesso,sizeof(registro),1,arquivo);

                    if(acesso.chave == leitura.chave)
                        break;
                 }
            }

            quantidadeDeRegistros++;
        }
    }
    cout << fixed;
    cout << setprecision(1) << getQuantidadeDeAcessos()/quantidadeDeRegistros << endl;
}

void HashEncadeado::removeRegistro(){
    int chave,proximaPosicao,posicaoAtual;
    registro leitura, escrita, anterior;

    cin >> chave;

    arquivo = fopen("teste.bin","r+");

    fseek(arquivo,h1(chave)*sizeof(registro),SEEK_SET);

    fread(&leitura,sizeof(registro),1,arquivo);

    if(leitura.chave == chave){

        //primeiro elemento da lista encadeada
        if(leitura.proximo != -1){

            //desloca ponteiro para proximo elemento da lista
            fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);

            //pega informacao do registro
            fread(&escrita, sizeof(registro),1,arquivo);

            //desloca ponteiro para registro inicial da lista
            fseek(arquivo,h1(chave)*sizeof(registro),SEEK_SET);

            //substitui pelo segundo elemento
            fwrite(&escrita,sizeof(registro),1,arquivo);

            //desloca ponteiro para proximo elemento da lista, para remove-lo
            fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);

            leitura.chave = -1;

            fwrite(&leitura,sizeof(registro),1,arquivo);
        }
        //nao tem lista associada
        else{

            leitura.chave = -1;

            fseek(arquivo,h1(chave)*sizeof(registro),SEEK_SET);

            fwrite(&leitura,sizeof(registro),1,arquivo);
        }


    }

    //pode ser o elemento do meio/final
    else{

        while(leitura.chave != chave && leitura.proximo != -1){

            fseek(arquivo,(-1)*sizeof(registro),SEEK_CUR);

            posicaoAtual = (ftell(arquivo)+1)/sizeof(registro);

            anterior = leitura;

            //desloca ponteiro para proximo elemento da lista
            fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);

            //pega informacao do registro
            fread(&leitura, sizeof(registro),1,arquivo);

            proximaPosicao = leitura.proximo;

        }
        if(leitura.chave == chave){
            leitura.proximo = ftell(arquivo)/sizeof(registro);

            //cout << proximaPosicao << endl;
            //cout << leitura.proximo << endl;

            leitura.chave = -1;

            fclose(arquivo);

            arquivo = fopen("teste.bin","r+");

            cout << "anterior chave: " << anterior.chave << endl;
            cout << "leitura chave: " << leitura.chave << endl;

            //altera ponteiro do anterior para apontar para o proximo
            fseek(arquivo,posicaoAtual*sizeof(registro),SEEK_SET);

            anterior.proximo = proximaPosicao;

            fwrite(&anterior,sizeof(registro),1,arquivo);

            //remove elemento desejado
            fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);

            fwrite(&leitura,sizeof(registro),1,arquivo);
        }
        else{
            cout << "chave nao encontrada: " << chave << endl;
        }
    }

    fclose(arquivo);
}

