//============================================================================
// Author      : Adam Lamoglia
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstdlib>

using namespace std;

struct element{
    int key;
    int *next;
};

int h1(int c,int m){
    return c%m;
}


int main() {
    FILE* file;
    string name;
    int n,x,e,index;
    int c = 0;

    getline(cin,name);

    cin >> n;

    file = fopen(name.c_str(),"w+");

    for(int i = 0;i < n; i++){
        fseek(file,i*sizeof(int),SEEK_SET);
        fwrite(&c,sizeof(int),1,file);
    }

    fclose(file);


    for(int i = 0; i < n; i++){
        cin >> x;

        file = fopen(name.c_str(),"rb+");


        fseek(file,h1(x,n)*sizeof(int),SEEK_SET);

        fread(&e,sizeof(int),1,file);

        if(e == 0){
            fseek(file,h1(x,n)*sizeof(int),SEEK_SET);

            fwrite(&x,sizeof(int),1,file);

        }
        else{
            index = h1(x,n)+1;
            fseek(file,index*sizeof(int),SEEK_SET);

            fread(&e,sizeof(int),1,file);

            while(e != 0){
                index++;

                if(index == n)
                    index = 0;

                fseek(file,index*sizeof(int),SEEK_SET);

                fread(&e,sizeof(int),1,file);

            }

            fseek(file,index*sizeof(int),SEEK_SET);

            fwrite(&x,sizeof(int),1,file);
        }

        fclose(file);

    }
    cout << endl;

    file = fopen(name.c_str(),"r+");

    for(int i = 0;i < n;i++){
        fseek(file,i*sizeof(int),SEEK_SET);
        fread(&x,sizeof(int),1,file);
        cout << x << endl;
    }

    fclose(file);

    cin >> x;

    file = fopen(name.c_str(),"r+");

    fseek(file,h1(x,n)*sizeof(int),SEEK_SET);

    fread(&e,sizeof(int),1,file);

    if(e == x){
        cout << "O elemento " << x << " esta no indice " << h1(x,n) << endl;
        fclose(file);
        exit(1);
    }

    index = h1(x,n) + 1;

    while(index != h1(x,n)){
        fseek(file,index*sizeof(int),SEEK_SET);

        fread(&e,sizeof(int),1,file);

        if(e == x){
           cout << "O elemento " << x << " esta no indice " << index << endl;
            fclose(file);
            exit(1);
        }

        index++;
    }

    cout << "O elemento " << x << " nao esta contido no arquivo" << endl;
}
