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
    registro abertura,escrita,leitura,novaLeitura,anterior,aux;

    posicao = -1;

    cin >> escrita.chave;
    cin.ignore();

    cin.getline(escrita.nome,20);

    cin >> escrita.idade;

    abertura.chave = -1;
    abertura.proximo = -1;

    arquivo = fopen("teste.bin","rb+");

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
    //existem registros dentro do arquivo
    else{

        //arquivo = fopen("teste.bin","r+");

        fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

        fread(&leitura,sizeof(registro),1,arquivo);

        //o endereco esta livre
        if(leitura.chave == -1){
            escrita.proximo = -1;

            fclose(arquivo);

            arquivo = fopen("teste.bin","rb+");

            fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

            fwrite(&escrita,sizeof(registro),1,arquivo);

            fclose(arquivo);
        }
        //existe um registro com chave igual
        else if(leitura.chave == escrita.chave){
            cout << "chave ja existente: " << leitura.chave << endl;
            fclose(arquivo);
        }
        //existe um registro no endereco original
        else{
            fclose(arquivo);

            arquivo = fopen("teste.bin","rb+");

            //desloca para o ultimo endereco do arquivo, para verificar se esta vazio
            deslocamento = getTamanho() - 1;

            fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);
            fread(&novaLeitura,sizeof(registro),1,arquivo);

            //sobe no arquivo ate encontrar um endereco livre
            while(novaLeitura.chave != -1){
                deslocamento--;
                fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);
                fread(&novaLeitura,sizeof(registro),1,arquivo);
            }

            //se a insercao for na mesma lista, entao precisamos encontrar o ultimo registro da lista
            if(h1(escrita.chave) == h1(leitura.chave)){

                escrita.proximo = -1;

                cout << "deslocamento: " << deslocamento << endl;
                cout << "escrita.chave: " << escrita.chave << endl;
                cout << "escrita.nome: " << escrita.nome << endl;
                cout << "escrita.idade: " << escrita.idade << endl;
                fclose(arquivo);

                arquivo = fopen("teste.bin","rb+");

                //poe registro no fim da lista
                fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);
                fwrite(&escrita,sizeof(registro),1,arquivo);


                fseek(arquivo,h1(leitura.chave)*sizeof(registro),SEEK_SET);
                fread(&leitura,sizeof(registro),1,arquivo);

                posicaoAnterior = (ftell(arquivo)-1)/sizeof(registro);

                cout << "leitura.chave " << leitura.chave << endl;
                cout << "leitura.prox " << leitura.proximo << endl;


                //procura o elemento do fim da lista antiga, para atualizar o proximo
                while(leitura.proximo != -1){
                    posicaoAnterior = leitura.proximo;
                    fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);
                    fread(&leitura,sizeof(registro),1,arquivo);

                    cout << "leitura.chave " << leitura.chave << endl;
                    cout << "leitura.prox " << leitura.proximo << endl;
                }

                leitura.proximo = deslocamento;

                cout << posicaoAnterior << endl;
                cout << deslocamento << endl;

                fseek(arquivo,posicaoAnterior*sizeof(registro),SEEK_SET);

                fwrite(&leitura,sizeof(registro),1,arquivo);


            }
            //nao fazem parte da mesma lista
            else{
                escrita.proximo = -1;

                fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

                fread(&leitura,sizeof(registro),1,arquivo);

                //escreve registro no endereco original
                fseek(arquivo,h1(escrita.chave)*sizeof(registro),SEEK_SET);

                fwrite(&escrita,sizeof(registro),1,arquivo);

                //poe registro no fim da lista
                fseek(arquivo,deslocamento*sizeof(registro),SEEK_SET);

                fwrite(&leitura,sizeof(registro),1,arquivo);

                //atualiza ponteiro do anterior
                fseek(arquivo,h1(leitura.chave)*sizeof(registro),SEEK_SET);

                fread(&leitura,sizeof(registro),1,arquivo);

                posicaoAnterior = (ftell(arquivo)-1)/sizeof(registro);

                while(leitura.proximo != -1){
                    fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);

                    fread(&leitura,sizeof(registro),1,arquivo);

                    if(leitura.proximo == -1)
                        break;

                    posicaoAnterior = (ftell(arquivo)-1)/sizeof(registro);
                }

                cout << posicaoAnterior << endl;
                cout << deslocamento << endl;

                fseek(arquivo,posicaoAnterior*sizeof(registro),SEEK_SET);

                fread(&leitura,sizeof(registro),1,arquivo);

                leitura.proximo = deslocamento;

                fseek(arquivo,posicaoAnterior*sizeof(registro),SEEK_SET);

                fwrite(&leitura,sizeof(registro),1,arquivo);
            }

            fclose(arquivo);
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
    int chave,proximaPosicao,posicaoAtual,posicaoAnterior;
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

        //vai para o inicio da lista
        fseek(arquivo,h1(chave)*sizeof(registro),SEEK_SET);

        fread(&leitura,sizeof(registro),1,arquivo);

        posicaoAtual = (ftell(arquivo)-1)/sizeof(registro);

        //enquanto nao encontrar o registro, percorre a lista
        while(leitura.chave != chave && leitura.proximo != -1){

            fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);

            fread(&leitura,sizeof(registro),1,arquivo);

            posicaoAtual = (ftell(arquivo)-1)/sizeof(registro);

            if(leitura.chave == chave)
                break;

            posicaoAnterior = (ftell(arquivo)-1)/sizeof(registro);

            if(posicaoAnterior == getTamanho())
                posicaoAnterior = getTamanho() - 1;

            //cout << posicaoAtual << endl;
            //cout << posicaoAnterior << endl;
            //cout << leitura.proximo << endl;
        }

        if(leitura.chave == chave){
            cout << posicaoAtual << endl;
            cout << posicaoAnterior << endl;
            cout << leitura.proximo << endl;

            //apaga registro
            leitura.chave = -1;

            fseek(arquivo,posicaoAtual*sizeof(registro),SEEK_SET);

            fwrite(&leitura,sizeof(registro),1,arquivo);

            //aponta anterior para o proximo
            fseek(arquivo,posicaoAnterior*sizeof(registro),SEEK_SET);

            fread(&escrita,sizeof(registro),1,arquivo);

            escrita.proximo = leitura.proximo;

            fseek(arquivo,posicaoAnterior*sizeof(registro),SEEK_SET);

            fwrite(&escrita,sizeof(registro),1,arquivo);
        }
        else{
            cout << "chave nao encontrada: " << chave << endl;
        }


        /*while(leitura.chave != chave && leitura.proximo != -1){

            fseek(arquivo,(-1)*sizeof(registro),SEEK_CUR);

            posicaoAtual = (ftell(arquivo)+1)/sizeof(registro);

            anterior = leitura;

            //desloca ponteiro para proximo elemento da lista
            fseek(arquivo,leitura.proximo*sizeof(registro),SEEK_SET);

            //pega informacao do registro
            fread(&leitura, sizeof(registro),1,arquivo);

            proximaPosicao = leitura.proximo;

        }*/
        /*if(leitura.chave == chave){
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
        }*/
        /*else{
            cout << "chave nao encontrada: " << chave << endl;
        }*/
    }

    fclose(arquivo);
}

