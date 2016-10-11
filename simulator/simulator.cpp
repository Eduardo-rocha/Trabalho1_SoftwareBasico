/*
 *	William Motta, 0950050
 *		Simulador de Assembly inventado
 *	Compilacao:
 *		g++ simulador.cpp -o simulador
 *	Execucao:
 *		./simulador <arquivo objeto>
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<iostream>
#include<vector>

using namespace std;

unsigned int PC, ACC, OPC, OP1, OP2;
unsigned int STOP;

unsigned int coletarEntrada();

int main(int argc, char* argv[]){
    // Checar entrada
    if(argc<2){
        printf("Necessito do endereco do arquivo objeto. O simulador sera encerrado.");
        return 1;
    }

    // Carregar e decodificar texto
	fstream myfile(argv[1], std::ios_base::in);
	vector<unsigned int> MEM;
	long n = 0;
	while (!myfile.eof()) {
		myfile >> n;
		MEM.push_back(n);
	}
	cout << endl;

    // Simular
    // IF ID EX MEM WB
    PC=0;
    ACC=0;
    STOP=0;
    while(!STOP)
    {
        OPC=MEM[PC];
        switch(OPC){
        case 1: // ADD OP1
            OP1=MEM[PC+1];
            PC=PC+2;
            ACC=ACC+MEM[OP1];
            break;
        case 2:	// SUB OP1
            OP1=MEM[PC+1];
            PC=PC+2;
            ACC=ACC-MEM[OP1];
            break;
        case 3:	// MULT OP1
            OP1=MEM[PC+1];
            PC=PC+2;
            ACC=ACC*MEM[OP1];
            break;
        case 4:	// DIV OP1
            OP1=MEM[PC+1];
            PC=PC+2;
            if(MEM[OP1]==0){
                printf("Divisao por zero, OPC %d OP1 %d ACC %d, encerrando simulacao.", OPC, OP1, ACC);
                STOP=1;
            }
            else
                ACC=ACC/MEM[OP1];
            break;
        case 5: // JMP OP1
            OP1=MEM[PC+1];
            PC=OP1;
            break;
        case 6: // JMPN OP1
            OP1=MEM[PC+1];
            if((char)ACC<0)
                PC=OP1;
            else
                PC=PC+2;
            break;
        case 7:	// JMPP OP1
            OP1=MEM[PC+1];
            if(ACC>0)
                PC=OP1;
            else
                PC=PC+2;
            break;
        case 8:	// JMPZ OP1
            OP1=MEM[PC+1];
            if(ACC==0)
                PC=OP1;
            else
                PC=PC+2;
            break;
        case 9:	// COPY OP1 OP2
            OP1=MEM[PC+1];
            OP2=MEM[PC+2];
            MEM[OP2]=MEM[OP1];
            PC=PC+3;
            break;
        case 10:	//LOAD OP1
            OP1=MEM[PC+1];
            ACC=MEM[OP1];
            PC=PC+2;
            break;
        case 11:	// STORE OP1
            OP1=MEM[PC+1];
            MEM[OP1]=ACC;
            PC=PC+2;
            break;
        case 12:	// INPUT OP1	// TO DO
            OP1=MEM[PC+1];
            MEM[OP1]=coletarEntrada();
            PC=PC+2;
            break;
        case 13:	// OUTPUT OP1
            OP1=MEM[PC+1];
            printf("%d", MEM[OP1]);
            PC=PC+2;
            break;
        case 14:	// STOP
            STOP=1;
            break;
        }
    }
    return 0;
}
unsigned int coletarEntrada(){
    printf("INPUT: ");
    unsigned int input;
    scanf_s("%d",&input);
    return input;
}


