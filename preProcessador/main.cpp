//
//  main.cpp
//  P1SB
//
//  Created by Eduardo on 12/10/16.
//  Copyright © 2016 Eduardo. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>


using namespace std;

//
struct EquateStruct {
    string nome;
    string valor;
    EquateStruct *next;
} ;

string retorno = "";
void ProcurarLista(EquateStruct *a1, string ArgumentoIF){
    retorno = "";
    if (a1 !=NULL) {
        if (a1->nome == ArgumentoIF)
            retorno = a1->valor;
        else
            ProcurarLista(a1->next, ArgumentoIF);
    }
}


int main(int argc, const char * argv[]) {

    string arquivo_entrada = argv[1];
    string arquivo_saida   = argv[2];

    ifstream ArquivoASM (arquivo_entrada); // input
    ofstream ArquivoPRE (arquivo_saida); // output

    ofstream fileBin ("NumeroDeLinhas.bin", ios::binary);// arq que guarda o numero de linhas da saída em relação ao arq de entrada

    string line;
    bool Descartar2LinhasIF = false;
    bool DescartarLinhaIF = false;

    // Teste se ambos os aquivos foram abertos corretamente
    if (!(ArquivoPRE.is_open() && ArquivoASM.is_open()))
    {
        cout << "Unable to open file";
        return 0;
    }

    EquateStruct *InicioListaEqu;
    EquateStruct *FimListaEqu;
    InicioListaEqu = new EquateStruct;
    InicioListaEqu->valor = "";
    InicioListaEqu->nome = "";
    InicioListaEqu->next = NULL;
    FimListaEqu = InicioListaEqu;

    int ContadorLinhas = 0;
    vector<int> NumeroDasLinhas;

    // Percorrer todas as linhas do arquivo
    while ( getline(ArquivoASM,line) )
    {
        ContadorLinhas++;

        bool LinhaVazia = true; // Flag que indica ao final do FOR se a linha está vazia
        char Caractere = 0;
        char CaractereAnterior;

        // UpperCase all characters
        transform(line.begin(), line.end(),line.begin(), ::toupper);

        for (unsigned i=0; i<line.length(); ++i)
        {
            CaractereAnterior = Caractere;
            Caractere = line.at(i);

            // Trabalhar apenas com SPACE, eliminar \t
            if (Caractere == '\t')
            {
                Caractere = ' ';
                line.at(i) = ' ';
            }

            // detectar comentário
            if (Caractere == ';')
            {
                // cortar linha, retirar comentario
                line = line.substr(0,i);
                break; // sair do for, fim da linha
            }

            // teste de linha vazia
            if (Caractere != ' ' && LinhaVazia)
            {

                line = line.substr(i,line.length()-i); // cortar linha, tirar espaços no inicio
                i = 0; // string foi modificada

                LinhaVazia = false;
            }

            // unificar espaços internos da linha
            if (!LinhaVazia && CaractereAnterior == ' ' && Caractere == ' ')
            {
                line.erase (line.begin()+i); // apagar espaço duplo
                i--;
            }

            // retirar SPACE antes de ':' e ','
            if (!LinhaVazia && CaractereAnterior == ' ' && (Caractere == ':' || Caractere == ','))
            {
                line.erase (line.begin()+i); // apagar espaço antes
                line.at(i-1) = Caractere;
                i--;
            }

            // garantir que tem SPACE depois de ':' e ','
            if (!LinhaVazia && (CaractereAnterior == ':' || CaractereAnterior == ',') && Caractere != ' ')
            {
                line.insert (line.begin()+i, ' '); // adicionar espaço
                line.at(i-1) = CaractereAnterior;
                i++;
            }

        }

        // apagar espaço no final da linha
        if (!LinhaVazia)
            if (line.at(line.length()-1) == ' ')
                line.erase (line.begin()+line.length()-1);

        // Procurar EQU na linha -----------------------------------------------------------------------
        size_t foundEQU = line.find("EQU");//foundEQU = unsigned int com a posição
        EquateStruct *aux;
        bool DescartarLinhaEQU = false;
        if (foundEQU!=string::npos)
        {
            DescartarLinhaEQU = true;
            aux = new EquateStruct;
            aux->valor = line.substr(foundEQU+4, line.length()-1);
            aux->nome = line.substr(0,foundEQU-2);

            // procurar por declaração repetida
            ProcurarLista(InicioListaEqu, aux->nome);
            if(retorno != ""){
                cout << "ERRO semântico:(linha " << ContadorLinhas << ") " << " declaração de EQU repetida.\n";
            }

            if (InicioListaEqu->nome == "")
                InicioListaEqu = aux;
            else
                FimListaEqu->next = aux;
            FimListaEqu = aux;
        }

        // Procurar simbolos definidos com EQU ---------------------------------------------------------
        int PrimeiroChar = 0;
        string palavra;
        for (unsigned i=0; i<line.length(); ++i)
        {
            Caractere = line.at(i);

            if(Caractere == ' '){
                palavra = line.substr(PrimeiroChar, i-PrimeiroChar);
                ProcurarLista(InicioListaEqu, palavra);
                if(retorno != "")
                {
                    if(PrimeiroChar>0)
                        line = line.substr(0, PrimeiroChar) + retorno + line.substr(i, line.length()-i);
                    else
                        line = retorno + line.substr(i, line.length()-i);

                }
                PrimeiroChar = i+1;
            }
            if(i == line.length()-1){
                palavra = line.substr(PrimeiroChar, i+1-PrimeiroChar);
                ProcurarLista(InicioListaEqu, palavra);
                if(retorno != "")
                {
                    if(PrimeiroChar>0)
                        line = line.substr(0, PrimeiroChar) + retorno;
                    else
                        line = retorno;

                }
            }
        }

        // Procurar IF na linha ------------------------------------------------------------------------
        if (!LinhaVazia)
        {
            if (Descartar2LinhasIF)
            {
                DescartarLinhaIF = true;
                Descartar2LinhasIF = false;
            }else
                DescartarLinhaIF = false;
            size_t foundIF = line.find("IF");//foundEQU = unsigned int com a posição
            if (foundIF!=string::npos)
            {
                DescartarLinhaIF = true;
                string ArgumentoIF = line.substr(foundIF+3, line.length()-1);
                if (ArgumentoIF == "0")
                {
                    DescartarLinhaIF = true;
                    Descartar2LinhasIF = true;
                }else if (ArgumentoIF != "1")
                {
                    //Procurar argumento na lista EQU
                    ProcurarLista(InicioListaEqu, ArgumentoIF);
                    string Argumento = retorno;
                    if (Argumento == ""){
                        cout << "ERRO semântico:(linha " << ContadorLinhas << ") " << ArgumentoIF << " não declarado.";
                    }else
                    {
                        if (Argumento == "0")
                        {
                            DescartarLinhaIF = true;
                            Descartar2LinhasIF = true;
                        }else if (Argumento != "1")
                            cout << "ERRO semântico:(linha " << ContadorLinhas << ") " << ArgumentoIF << " possui declaração diferente de '0' ou '1'.";
                    }
                }
            }
        }


        // Escrever linha no arquivo de saida
        if (!LinhaVazia && !DescartarLinhaIF && !DescartarLinhaEQU) // linha não vazia
        {
            ArquivoPRE << line << "\n";
            NumeroDasLinhas.push_back(ContadorLinhas);
            //cout << NumeroDasLinhas.back() << "\n";

            fileBin.write ((char *)&ContadorLinhas, sizeof(ContadorLinhas));
        }
    }


    ArquivoPRE.close();
    ArquivoASM.close();
    fileBin.close();

    return 0;
}
