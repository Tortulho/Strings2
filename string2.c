#include <stdio.h>
#include <string.h>
#include <ctype.h> //pra funçoes strupp strlow
#include <stdlib.h>
#include <stdbool.h>

//da flush ao stdin
void stdinflush() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); //limpa até o fim da linha (isto aqui tive que ir pesquisar se nao nao saia daqui)
}

long input_num(char stdout_string[]) {
    printf("%s", stdout_string);

    long num;
    scanf("%ld",&num);

    stdinflush();

    return num;
}

long strfnd(char str[], char letra) {

    //so faltava usar assembly diretamente aqui ou usar os registos implicitamente aqui para ficar mais divertido

    char* string = str; //nota: so deixei aqui para parecer ainda mais ridiculo, pq queria deixar string++

    strfnd_loop:

        if (*string == letra) {
            return string - str;
        } else if(*string == '\0') {
            return -1;
        }

        string++; //esta sera a minha maior trapalhada hahahaha

    goto strfnd_loop;
}

long strloc(char str[], char letra, unsigned long idx_loc) {

    //so faltava usar assembly diretamente aqui ou usar os registos implicitamente aqui para ficar mais divertido

    char* string = str + idx_loc; //nota: so deixei aqui para parecer ainda mais ridiculo, pq queria deixar string++ //famoso add em masm

    strfnd_loop:

        if (*string == letra) {
            return string - str;
        } else if(*string == '\0') {
            return -1;
        }

        string++; //esta sera a minha maior trapalhada hahahaha

    goto strfnd_loop;
}

//localiza a letra, devolve o ponteiro da letra encontrada, caso nao encontre devolve NULL
char* strploc(char str[], char letra) {

    //so faltava usar assembly diretamente aqui ou usar os registos implicitamente aqui para ficar mais divertido

    //nota: so deixei aqui para parecer ainda mais ridiculo, pq queria deixar string++ //famoso add em masm

    strfnd_loop:

        if (*str == letra) {
            return str; //tecnicamente da return ao ptr mas fica mais funny assim
        } else if(*str == '\0') {
            return NULL;
        }

        str++; //esta sera a minha maior trapalhada double down hahahaha

    goto strfnd_loop;
}

//string pointer limited locator
char* strplloc(char str[], char letra, char* max_pointer) {

    //so faltava usar assembly diretamente aqui ou usar os registos implicitamente aqui para ficar mais divertido

    //nota: so deixei aqui para parecer ainda mais ridiculo, pq queria deixar string++ //famoso add em masm

    strfnd_loop:

        if (str == max_pointer) {
            return NULL; //ja passou do limite
        } else if (*str == letra) {
            return str; //tecnicamente da return ao ptr mas fica mais funny assim
        } else if(*str == '\0') {
            return NULL;
        }

        str++; //esta sera a minha maior trapalhada double down hahahaha

    goto strfnd_loop;
}

//string char counter
unsigned long strcnt(char string[], char letra) {

    long idx_find = strfnd(string,letra);

    if (idx_find == -1) {
        return 0;
    }

    unsigned long count = 0;

    strcnt_loop:
        //nao precisamente: se quissesse otimizar aqui até poderia ver se o ultimo valor antes do /0 seria diferente
        if (*string != letra) {
            string = strploc(string,letra);
            if (!string) {
                return count;
            }
            string++;
            count++;
            goto strcnt_loop; //famoso continue
        }

        count++;
        string++;

        goto strcnt_loop;
}

//conta a partir dos ponteiros, nota: o limsup é exclusivo e o inf é inclusivo
//str counter limited w/ pointer
unsigned long strlpcnt(char string[], char letra, char* ptr_lim_sup) {

    string = strploc(string,letra);

    //caso nao tenha encontrado da NULL
    if (!string) {
        return 0;
    }

    unsigned long count = 0;

    strcnt_loop:
        if (*string != letra || string == ptr_lim_sup) { //se ja nao for a mesma letra ou ja tiver passado do limite
            return count;
        }

        string++;
        count++;
        goto strcnt_loop; //famoso continue

    
}

//conta com base inicial em idx (convertendo para ponteiros)
//str counter limited w/ idx/relative pos
unsigned long strlcnt(char string[], char letra, unsigned long idx_lim_inf, unsigned long idx_lim_sup) {

    string += idx_lim_inf;
    char* ptr_lim_sup = string + idx_lim_sup;
    
    unsigned long count = strlpcnt(string,letra,ptr_lim_sup);

    return count;
}

/*string pointer limited locate word:
string -> começo da string a analisar (comporta-se como um ponteiro)
word   -> word a analisar (comporta-se como um ponteiro)
ptr_lim-> endereço maximo da memoria que ele pode ler (comporta-se como um ponteiro)
limited-> se for true, limita a procura a palavras entre espaços ou entre linhas

Dada uma string, localiza a palavra entre o elemento correspondente ao ponteiro string
até ao elemento correspondente do ponteiro limite
Retorna o primeiro ponteiro da ocorrência da palavra
*/
char* strpllocwrd(char string[], char word[], char* ptr_lim, bool limited) {

    char found;
    size_t word_len = strlen(word);

    while(string < ptr_lim) {

        found = 1;

        //tenta encontrar a primeira letra
        string = strploc(string,word[0]); //o ptr pula para a proxima posiçao onde pode haver a palavra
        if (!string || string >= ptr_lim) {
            return NULL;
        }   
        char* ptr_inicio = string;

        //loop de ver se é a palavra
        for(unsigned long idx = 0; idx < word_len; idx++) {
            if (*string != word[idx] && *string != '\0') {
                found = -1;
                break;
            }
            string++;
        }

        //bloco de verificaçao se a palavra esta entre espaços livres (caso seja pedido limite)
        if (found == 1) {
            if (limited) {
                
                char* ptr_anterior = ptr_inicio-1;

                char* ptr_depois   = string; // já avançou word_len
                bool inicio_valido = (*ptr_anterior == '\0' || *ptr_anterior == ' ' || *ptr_anterior == '\t' || *ptr_anterior == '\n' || *ptr_anterior == '"');
                bool fim_valido    = (*ptr_depois == '\0'   || *ptr_depois == ' '   || *ptr_depois == '\t'   || *ptr_depois == '\n'   || *ptr_depois == '"'  );

                if (inicio_valido && fim_valido) {
                    return ptr_inicio;
                }
            } else {
                return ptr_inicio;
            }
        }
    }

    return NULL;    
}

/*string pointer locater word
Dado uma string localiza a palavra e retorna o ponteiro do inicio da palavra (util para modificação da palavra)*/
inline char* strplocwrd(char string[], char word[], bool limited) {
    char* ptr_lim = string + strlen(string) + 1;
    return strpllocwrd(string,word,ptr_lim,limited);
}

/*String word locater
Dado uma string localiza a palavra e retorna o idx
*/
inline unsigned long strlocwrd(char string[], char word[], bool limited) {
    return (strplocwrd(string,word,limited) - string);
}

//string counter word
unsigned long strcntwrd(char string[], char word[],bool limited) {

    size_t word_len = strlen(word);
    unsigned int count = 0;

    //ver se tem pelo menos a primeira letra da palavra
    string = strploc(string,word[0]);

    if (!string) {
        return 0;
    }

    loop:

        string = strplocwrd(string,word,limited);
        if (!string) {
            return count;
        }

        string += word_len - 1;

        count++;

    goto loop;
}

char* input(char stdout_string[], size_t limiter) {

    printf("%s",stdout_string);


    //meu proprio input
    char* str_input = calloc(limiter,sizeof(char));

    if (!str_input) {
        return NULL;
    }

    char c = '\b';
    size_t idx = 0;
    while (c != '\n' && idx < (limiter - 1)) {
        c = getchar();
        str_input[idx] = (char) c; 

        if(c == -1) {
            str_input[idx+1] = '\0';
        }

        if(c == '\b' && idx > 0) {
            str_input[idx--] = '\0';
            str_input[idx] = '\0';
            idx--;
            continue;
        }
        
        idx++;
    }

    //str_input[limiter] = '\0';

    //BLOCO DE OTIMIZAÇAO DE ESPAÇO DE MEMORIA
    int input_len = strlen(str_input);
    str_input = realloc(str_input, input_len + 1);

    return str_input;
}

char* input_otm(char stdout_string[]) {

    printf("%s",stdout_string);


    //meu proprio input
    char* str_input = calloc(1024,sizeof(char));

    if (!str_input) {
        return NULL;
    }

    char c = '\b';
    size_t idx = 0;
    while (c != '\n' && idx < (1023)) {
        c = getchar();
        str_input[idx] = (char) c; 

        if(c == -1) { //EOF
            str_input[idx+1] = '\0';
        }

        if(c == '\b' && idx > 0) {
            str_input[idx--] = '\0';
            str_input[idx] = '\0';
            idx--;
            continue;
        }
        
        idx++;
    }

    //str_input[1024] = '\0'; //garantir

    //BLOCO DE OTIMIZAÇAO DE ESPAÇO DE MEMORIA
    int input_len = strlen(str_input);
    str_input = realloc(str_input, input_len + 1);

    return str_input;
}

char* input_dyn(char stdout_string[]) {
    printf("%s",stdout_string);

    size_t memsize = 256;

    //meu proprio input
    char* str_input = calloc(memsize,1);

    if (!str_input) {
        return NULL;
    }

    int c = '\b';
    size_t idx = 0;

    while(1) {
        //BLOCO DE LEITURA
        c = getchar();

        if (c == EOF) break;
        //BLOCO DE ALOCAÇAO DINAMICA
        //caso chegue ao limite, aumenta em 128bytes
        if (idx >= memsize - 1) {
            size_t new_size = memsize + 128;
            char* temp = realloc(str_input, new_size);
            
            if (!temp) {
                free(str_input);
                return NULL;
            }
            str_input = temp;
            memsize = new_size;
        }

        //BLOCO DE ESCRITURA
        str_input[idx] = (char) c;
        
        //BACKSPACE
        /*
        if(c == '\b' && idx > 0) {
            str_input[idx--] = '\0';
            str_input[idx] = '\0';
            idx--;
            continue;
        }
        */
        
        idx++;

        if (c == '\n') break;  // incluir \n e terminar

        if (idx == 0 && c == EOF) {
        free(str_input);
        return NULL; //sinalizar q nada foi lido
        }
        
    }

    

    //ASSEGURAR O \0
    str_input[idx] = '\0';

    

    //BLOCO DE OTIMIZAÇAO DE ESPAÇO DE MEMORIA
    /* caso nao consiga otimizar ele devolve a string à mesma, só que a ocupar mais tamanho
    */
    char* temp = realloc(str_input, idx + 1);
    if (temp) {str_input = temp;}

    return (char*) str_input;
}

/*
String replace

string -> string que vai ter substituição
word -> string que vai substituir uma secçao
count -> quantas vezes vai substituir, -1 para todas as ocorrencias
limited -> false para qualquer ocorrencia, true para apenas ocorrencias espaçadas
*/
void strcrpl(char str[], char word[], size_t count, bool limited) {
    
    size_t str_len = strlen(str);
    size_t wrd_len = strlen(word);
    char*  sptrlim = str + str_len;
    size_t rplcount = 0;

    while (rplcount < count) {

        //loc wrd
        str = strpllocwrd(str,word,sptrlim,limited);

        //se n loc sai
        if (str == NULL) {
            return;
        }

        // faz o replace
        for (size_t idx = 0; idx < wrd_len; idx++) {
            str[idx] = word[idx];
        }

        rplcount++;
        str += str_len;

    }

    return;

}

inline void strrpl(char str[], char word[], bool limited) {
    strcrpl(str, word, -1, limited);
}

void strupp(char str[]) {
    
    size_t len = strlen(str);
    for (size_t idx = 0; idx < len; idx++) {str[idx] = toupper(str[idx]);}
    return;

}

void strlow(char str[]) {
    
    size_t len = strlen(str);
    for (size_t idx = 0; idx < len; idx++) {str[idx] = tolower(str[idx]);}
    return;

}

inline void strcapitalize(char str[]) {
    *str = toupper(*str);
}

inline void strlowercase(char str[]) {
    *str = tolower(*str);
}

//APENAS PARA STRINGS ESTATICAS
inline void strlstrip(char **str) {

    while (isspace(**str)) {
        (*str)++;
    }

} 

void strrstrip(char *str) {

    char pstr = *str;
    *str += (strlen(str) - 1);

    while (isspace(*str) && pstr <= *str) {
        (*str) = '\0';
    }

} 

//APENAS PARA STRINGS ESTATICAS
inline void strstrip(char **str) {

    strlstrip(str);
    strrstrip(*str);

}

void strswap(char str[]) {
    size_t str_len = strlen(str);

    for (size_t idx = 0; idx < str_len; idx++) {
        if (islower(str[idx])) {
            str[idx] = toupper(str[idx]);
        } else if (isupper(str[idx])) {
            str[idx] = tolower(str[idx]);
        }
    }

}