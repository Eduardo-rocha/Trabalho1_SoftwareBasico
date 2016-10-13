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

using namespace std;

//
struct EquateStruct {
    string nome;
    string valor;
    EquateStruct *next;
} ;


int main(int argc, const char * argv[]) {
    
    ifstream ArquivoASM ("input.txt"); // input
    ofstream ArquivoPRE ("output.txt"); // output
    string line;
    
    // Teste se ambos os aquivos foram abertos corretamente
    if (!(ArquivoPRE.is_open() && ArquivoASM.is_open()))
    {
        cout << "Unable to open file";
        return 0;
    }
    
    // Percorrer todas as linhas do arquivo
    while ( getline(ArquivoASM,line) )
    {
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
        size_t foundEQU = line.find("EQU");//foundEQU = unsig int com a posição
        if (foundEQU!=string::npos)
        {
            //Procurar Label
            
            //Procurar Valor
            
            //Salvar
        }
        
        // Procurar IF na linha ------------------------------------------------------------------------
        
        
        
        
        
        
        
        
        // Escrever linha no arquivo de saida
        if (!LinhaVazia) // linha não vazia
            ArquivoPRE << line << "\n";
    }
    
    
    
    
    
    
    
    
    ArquivoPRE.close();
    ArquivoASM.close();
    
    return 0;
}