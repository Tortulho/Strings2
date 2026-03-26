#ifndef STRINGS2_H
#define STRINGS2_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

//da flush ao stdin
void stdinflush();

long input_num(char stdout_string[]);

//string char finder
long strfnd(char str[], char letra);

//string char locator
long strloc(char str[], char letra, unsigned long idx_loc);

//string char locator w/pointer
char* strploc(char str[], char letra);

//string char limited locator w/pointer
char* strplloc(char str[], char letra, char* max_pointer);

//string char counter
unsigned long strcnt(char string[], char letra);

//conta a partir dos ponteiros, nota: o limsup é exclusivo e o inf é inclusivo
//str counter limited w/ pointer
unsigned long strlpcnt(char string[], char letra, char* ptr_lim_sup);

//conta com base inicial em idx (convertendo para ponteiros)
//str counter limited w/ idx/relative pos
unsigned long strlcnt(char string[], char letra, unsigned long idx_lim_inf, unsigned long idx_lim_sup);

/*string pointer limited locate word:
string -> começo da string a analisar (comporta-se como um ponteiro)
word   -> word a analisar (comporta-se como um ponteiro)
ptr_lim-> endereço maximo da memoria que ele pode ler (comporta-se como um ponteiro)
limited-> se for true, limita a procura a palavras entre espaços ou entre linhas
*/
char* strpllocwrd(char string[], char word[], char* ptr_lim, bool limited);

//string pointer locater word
char* strplocwrd(char string[], char word[], bool limited);

/*String word locater
Dado uma string localiza a palavra e retorna o idx
*/
unsigned long strlocwrd(char string[], char word[], bool limited);

//string counter word
unsigned long strcntwrd(char string[], char word[],bool limited);

char* input(char stdout_string[], size_t limiter);

char* input_otm(char stdout_string[]);

//input otimizado com alocaçao de memoria dinamica e rearranjo automatico da memoria para otimização
char* input_dyn(char stdout_string[]);

//string replace with count (limiter)
void strcrpl(char str[], char word[], size_t count, bool limited);

//string replace
 void strrpl(char str[], char word[], bool limited);

//string upper
void strupp(char str[]);

//string lower
void strlow(char str[]);

//string upper (first char)
inline void strcapitalize(char str[]);

//string lower (first char)
inline void strlowercase(char str[]);

//APENAS FUNCIONA PARA STRINGS ESTATICOS
//string strip da parte esquerda
inline void strlstrip(char **str);

//string strip da parte direita
void strrstrip(char *str);

//string strip
inline void strstrip(char **str);

//string swap 
//lower to upper and upper to lower
void strswap(char str[]);

#endif
