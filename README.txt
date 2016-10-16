Grupo:

Eduardo Moura Cirilo Rocha	12/0116014
William Motta			09/50050
William Jun Yamada		12/0138549

Foi utilizado o gcc (Ubuntu 4.9.4-2ubuntu1~14.04.1) 4.9.4 e o g++

Para compilar:

$ g++ -std=c++0x PreProcessador.cpp -o preproc

$ g++ -std=c++0x Macros.cpp -o macro

$ gcc main.c

Para usar basta usar:

$./a.out -flag nome_do_arquivo_alvo nome_do_arquivo_saida

Onde as flags possíveis são:

-p: para gerar apenas o arquivo pré-procesado
-m: para gerar o arquivo com as macros resolvidas
-o: para gerar o arquivo final

