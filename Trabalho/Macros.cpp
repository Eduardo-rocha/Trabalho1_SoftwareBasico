//
//  Montador.cpp
//  P1SB
//
//  Created by Eduardo on 13/10/16.
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

struct Argumento {
    string nome;
    string nomeUso;
    Argumento *next;
} ;

struct Linha {
    string nome;
    int numeroDaLinha;
    Linha *next;
} ;

struct Macro {
    string nome;
    Argumento *a1;
    Linha *l1;
    Macro *next;
} ;

void InserirArgumento(Argumento *a1, string Palavra){
    if (a1->next !=NULL) {
        InserirArgumento(a1->next, Palavra);
    }else{
        Argumento *arg;
        arg = new Argumento;
        arg->nome = Palavra;
        arg->next = NULL;
        a1->next = arg;
    }
}

void InserirLinha(Linha *a1, string Palavra, int numero){
    if (a1->next !=NULL) {
        InserirLinha(a1->next, Palavra, numero);
    }else{
        Linha *linha;
        linha = new Linha;
        linha->nome = Palavra;
        linha->numeroDaLinha = numero;
        linha->next = NULL;
        a1->next = linha;
    }
}

Macro *RetornarMacro;
bool ProcurarMacroJaDef(string line, Macro *InicioMacro){
    RetornarMacro = NULL;
    Macro *m1 = InicioMacro;
    string Palavra;
    while(m1 != NULL){
        Palavra = line.substr(0,line.find_first_of(" "));
        if(m1->nome == Palavra){
            RetornarMacro = m1;
            return true;
        }
        m1 = m1->next;
    }
    
    return false;
}

int main(int argc, const char * argv[]) {
    
    string arquivo_entrada = argv[1];
    string arquivo_saida   = argv[2];
    
    ifstream ArquivoPRE (arquivo_entrada); // input
    ofstream ArquivoMCR (arquivo_saida); // output
    
    ifstream fileBinLeitura ("NumeroDeLinhas.bin", ios::binary);
    ofstream fileBinSaida ("NumeroDeLinhasM.bin", ios::binary);// arq que guarda o numero de linhas da saída em relação ao arq de entrada
    
    string line;
    
    // Teste se ambos os aquivos foram abertos corretamente
    if (!(ArquivoPRE.is_open() && ArquivoMCR.is_open()))
    {
        cout << "Unable to open file";
        return 0;
    }
    
    Macro *InicioMacro = NULL;
    Macro *FimMacro = NULL;
    
    int ContadorLinhas = 0;
    vector<int> NumeroDasLinhas;
    
    char Caractere;
    
    // Percorrer todas as linhas do arquivo
    while ( getline(ArquivoPRE,line) )
    {
        fileBinLeitura.read ((char *)&ContadorLinhas, sizeof(ContadorLinhas));
        
        size_t foundMacro = line.find("MACRO");//foundMACRO = unsigned int com a posição
        Macro *aux;
        if (foundMacro!=string::npos)
        {
            
            aux = new Macro;
            aux->nome = line.substr(0,foundMacro-2);
            aux->l1 = NULL;
            aux->a1 = NULL;
            aux->next = NULL;
            
            //Procurar declaraç˜åo repetida
            if(ProcurarMacroJaDef(aux->nome, InicioMacro)){
                cout << "ERRO semântico:(linha " << ContadorLinhas << ") " << " declaração de MACRO repetida.\n";
            }
            
            
            if(InicioMacro == NULL){
                InicioMacro = aux;
                FimMacro = aux;
            }else{
                FimMacro->next = aux;
                FimMacro = aux;
            }
            
            
            
            // Tem argumentos?
            if (foundMacro != line.length()-5)
            {
                int PrimeiroChar = foundMacro+6;
                string palavra;
                for (unsigned i=foundMacro+6; i<line.length(); ++i)
                {
                    Caractere = line.at(i);
                    
                    if(Caractere == ','){
                        palavra = line.substr(PrimeiroChar, i-PrimeiroChar);
                        PrimeiroChar = i+2;
                        if(aux->a1 != NULL){
                            InserirArgumento(aux->a1, palavra);
                        }else{
                            Argumento *arg;
                            arg = new Argumento;
                            arg->nome = palavra;
                            arg->next = NULL;
                            aux->a1 = arg;
                        }
                    }
                    if(i == line.length()-1){
                        palavra = line.substr(PrimeiroChar, i+1-PrimeiroChar);
                        if(aux->a1 != NULL){
                            InserirArgumento(aux->a1, palavra);
                        }else{
                            Argumento *arg;
                            arg = new Argumento;
                            arg->nome = palavra;
                            arg->next = NULL;
                            aux->a1 = arg;
                        }
                    }
                    
                    
                    
                }
                
            }
            
            // Guardar linhas
            while ( getline(ArquivoPRE,line) )
            {
                fileBinLeitura.read ((char *)&ContadorLinhas, sizeof(ContadorLinhas));
                size_t foundEndMacro = line.find("ENDMACRO");//foundENDMACRO = unsigned int com a posição
                if (foundEndMacro==string::npos)
                {
                    if(aux->l1 != NULL){
                        InserirLinha(aux->l1, line, ContadorLinhas);
                    }else{
                        Linha *linha;
                        linha = new Linha;
                        linha->nome = line;
                        linha->numeroDaLinha = ContadorLinhas;
                        linha->next = NULL;
                        aux->l1 = linha;
                    }
                }else{
                    break;// achou ENDMACRO
                }
            }
            
        }else if(ProcurarMacroJaDef(line, InicioMacro))
        {
            size_t inicio = 0, fim = 0;
            Argumento *aux = RetornarMacro->a1;
            while(aux != NULL){
                inicio = line.find_first_of(" ", inicio+1)+1;
                fim = line.find_first_of(",", inicio+1)-1;
                if(fim == std::string::npos)
                    fim = line.length()-1;
                aux->nomeUso = line.substr(inicio, fim-inicio+1);
                aux = aux->next;
            }
            
            Linha *auxl = RetornarMacro->l1;
            while(auxl != NULL){
                aux = RetornarMacro->a1;
                line = auxl->nome;
                while(aux != NULL){
                    size_t foundArgu = line.find(aux->nome);//foundENDMACRO = unsigned int com a posição
                    while (foundArgu!=string::npos)
                    {
                        line = line.substr(0, foundArgu) + aux->nomeUso + line.substr(foundArgu+aux->nome.length(), line.length()-foundArgu+aux->nome.length());
                        foundArgu = line.find(aux->nome);//foundENDMACRO = unsigned int com a posição
                    }
                    aux = aux->next;
                }
                
                
                ArquivoMCR << line << "\n";
                fileBinSaida.write ((char *)&auxl->numeroDaLinha, sizeof(auxl->numeroDaLinha));
                //cout << auxl->numeroDaLinha << "\n";
                
                auxl = auxl->next;
            }
            
            
        }else
        {
            // add line
            ArquivoMCR << line << "\n";
            fileBinSaida.write ((char *)&ContadorLinhas, sizeof(ContadorLinhas));
            //cout << ContadorLinhas << "\n";
        }
        
        
    }
    
    return  0;
}






























