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

    strfnd_loop:

        if (*str == letra) {
            return str;
        } else if(*str == '\0') {
            return NULL;
        }

        str++;

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
    char *ptrcmp = string;
    char *ptr_anterior = string; //para ter dado util

    while(string < ptr_lim) {

        found = 1;

        //tenta encontrar a primeira letra
        string = strploc(string,word[0]); //o ptr pula para a proxima posiçao onde pode haver a palavra

        if (!string || string >= ptr_lim) {
            return NULL;
        }   

        //printf("DEBUG: CHAR FOUND: %c\n",*string);

        char* ptr_inicio = string;

        //loop de ver se é a palavra
        for(unsigned long idx = 0; idx < word_len; idx++) { //TODO
            if (*string != word[idx]) {
                //printf("ERRO - COMPARANDO %p, caracter: %c\n",string,*string);
                found = -1;
                break;
            }
            //printf(" -- C-BUSCA -- %p - char \'%c\'\n",string,*string);
            string++;
        }

        //printf("calc %ld\n",(string - ptr_inicio));

        //bloco de verificaçao se a palavra esta entre espaços livres (caso seja pedido limite)
        if (found == 1) {
            if (limited) {
                
                if (ptr_inicio == ptrcmp) {ptr_anterior = ptr_inicio - 1;} else {*ptr_anterior = '\0';}
                

                char* ptr_depois   = string; // já avançou word_len
                bool inicio_valido = (*ptr_anterior == '\0' || *ptr_anterior == ' ' || *ptr_anterior == '\t' || *ptr_anterior == '\n' || *ptr_anterior == '"');
                bool fim_valido    = (*ptr_depois == '\0'   || *ptr_depois == ' '   || *ptr_depois == '\t'   || *ptr_depois == '\n'   || *ptr_depois == '"'  );

                if (inicio_valido && fim_valido) {
                    return ptr_inicio;
                }
            } else {
                //printf("RETURN VALIDO");
                return ptr_inicio;
            }
        }
    }

    return NULL;    
}

/*string pointer locater word
Dado uma string localiza a palavra e retorna o ponteiro do inicio da palavra (util para modificação da palavra)*/
char* strplocwrd(char string[], char word[], bool limited) {
    char* ptr_lim = string + strlen(string) + 1;
    //printf("DEBUG . STRPLOCWRD %p\n",ptr_lim);
    return strpllocwrd(string,word,ptr_lim,limited);
}

/*String word locater
Dado uma string localiza a palavra e retorna o idx
*/
unsigned long strlocwrd(char string[], char word[], bool limited) {
    char* ptr = strplocwrd(string,word,limited);
    if (!ptr) {return -1;}
    return (ptr - string);
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
    if(str_input[idx-1] == '\n')  str_input[idx-1] = '\0';
    else str_input[idx] = '\0';
    

    //BLOCO DE OTIMIZAÇAO DE ESPAÇO DE MEMORIA
    /* caso nao consiga otimizar ele devolve a string à mesma, só que a ocupar mais tamanho
    */
    char* temp = realloc(str_input, idx + 1);
    if (temp) {str_input = temp;}

    return (char*) str_input;
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

void strcapitalize(char str[]) { //    *str = toupper(*str);
}

void strlowercase(char str[]) { //    *str = tolower(*str); 
}

//ONLY FOR STATIC STRINGS (can lose the pointer for the main string (if dynamic))
void strlstrip(char **str) { //
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

//ONLY FOR STATIC STRINGS (can lose the pointer for the main string (if dynamic use other pointer to allocated memblock to free whenever u need))
void strstrip(char **str) { //
    strlstrip(str);
    strrstrip(*str);

}

//Swap lowercase chars to uppercase and uppercase to lowercase
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

// ONLY FOR DYNAMIC MEMORY STRINGS
// insert a string at str[start_idx]
void strinsert(char **str, size_t start_idx, const char *str_to_add) {

    //errors (pointers)
    if (!str || !*str || !str_to_add) return;

    size_t mainstrlen    = strlen(*str);       
    size_t strtoaddlen   = strlen(str_to_add);

    if (start_idx > mainstrlen) return;

    // realloc 
    char *ptr = realloc(*str, (mainstrlen + strtoaddlen + 1)*sizeof(char));
    if (!ptr) return; //error return

    *str = ptr; //now the string point to the new string

    // move the rest of the string (little slicing) 
    memmove(
        *str + start_idx + strtoaddlen,               //dest
        *str + start_idx,                             //src
        (mainstrlen - start_idx + 1)*sizeof(char)     //size
    );

    // now the clean insert
    memcpy(*str + start_idx, str_to_add, strtoaddlen);
}

// Remove x (count) strings of removalstr from main string, limited for only whitespaced occurrences 
void strremove(char **mainstr, size_t count, char *removalstr, bool limited) {

    if (!mainstr || !*mainstr || !removalstr) return;

    char *ptr_de_iteracao = *mainstr;
    size_t mainstrlen = strlen(*mainstr);
    size_t rmvstrlen = strlen(removalstr);
    if (rmvstrlen == 0) return;

    char *ptr_lim     = ptr_de_iteracao + mainstrlen; //proteger o \0 final


    while (count > 0) {

        if (ptr_lim <= ptr_de_iteracao) {break;}

        ptr_de_iteracao = strpllocwrd(ptr_de_iteracao, removalstr, ptr_lim, limited);

        if (!ptr_de_iteracao) {break;}

        // move the rest of the string (little slicing) 
        memmove(
        ptr_de_iteracao,                                     //dest -> ptr do elemento da palavra a remover
        ptr_de_iteracao + rmvstrlen,            //src -> ptr depois da palavra a ser removida
        (ptr_lim - (ptr_de_iteracao + rmvstrlen) + 1)       //size -> tamanho do resto da string
        );

        ptr_lim -= rmvstrlen; //limitar a procura

        

        count--;

    }

    mainstrlen = strlen(*mainstr);

    //BLOCO DE OTIMIZAÇAO DE ESPAÇO DE MEMORIA
    /* caso nao consiga otimizar ele devolve a string à mesma, só que a ocupar mais tamanho
    */
    char* temp = realloc(*mainstr, mainstrlen + 1);
    if (temp) {*mainstr = temp;}

}

//o return serve apenas para controlo de erros: -1 para ptr NULL, 0 para erro no realloc, 1 para bem sucedido
//return only for error control: -1 for NULL, 0 for failure on otmization (mem error), 1 for sucesseful operation
char strotm(char **str) {
    if (!*str || !str) return -1;
    char* temp = realloc(*str, strlen(*str) + 1);
    if (temp) {*str = temp; return 1;} else {return 0;}
}

//only used for strpop
static void static_strpop(char *mainstr, size_t start_idx, size_t end_idx) {

    if (!mainstr) return;

    size_t str_len = strlen(mainstr);

    if (start_idx > str_len || end_idx > str_len) {return;}
    if (start_idx > end_idx) {
        size_t temp = start_idx;
        start_idx = end_idx;
        end_idx = temp;
    }

    memmove(
        mainstr + start_idx,
        mainstr + end_idx,
        (str_len - end_idx + 1) * sizeof(char) //size: str[end_idx+1] -- str[str_len]
    );

}

//Pop elements (char) from mainstr and reduce memsize of mainstr
void strpop(char **mainstr, size_t start_idx, size_t end_idx) {

    if (!*mainstr) return;

    static_strpop(*mainstr,start_idx,end_idx);

    char res;

    for (unsigned int counter = 0; counter < 4; counter++) {
        res = strotm(mainstr);
        if (res == 1) {
            return;
        }
    }

    return;

}

//slice string with a pointer
char* strpslice(char *ptr_start, size_t length) {

    if (!ptr_start) return NULL;

    //control
    if (strlen(ptr_start) < length) {return NULL;}

    char *strsliced = calloc(length+1,sizeof(char));
    if (!strsliced) return NULL;

    memcpy(strsliced,ptr_start,length);

    return strsliced;

}

//Return the sliced string that begins on start_idx and end at end_idx
char* strslice(char* str, size_t start_idx, size_t end_idx) {

    if (!str) return NULL;

    size_t str_len = strlen(str);
    
    if (start_idx > str_len || end_idx > str_len) {return NULL;}
    if (start_idx > end_idx) {
        size_t temp = start_idx;
        start_idx = end_idx;
        end_idx = temp;
    }


    return strpslice(str+(start_idx),end_idx-start_idx);

}

void strreplace(char **str, char *string_to_be_replaced, char *string_replacer, size_t start_idx, size_t count) {
    
    if (!str || !*str || !string_replacer || !string_to_be_replaced) {return;}

    size_t str_len = strlen(*str);
    size_t stbr_len = strlen(string_to_be_replaced);
    size_t sr_len = strlen(string_replacer);

    if (str_len <= start_idx) {return;}
    if (stbr_len > str_len) {return;}
    if (stbr_len == 0) {return;}

    size_t idx = start_idx;

    while (count > 0) {
        
        idx = strlocwrd(*str ,string_to_be_replaced,false);


        
        //printf("DEBUG %ld\n STRING",idx);
        
        if (idx == -1) {return;}

        //assume-se que stbr_len < strlen atual
        strpop(str,idx,idx+stbr_len);

        strinsert(str,idx,string_replacer);
        
        //exec pop
        //exec insert
        
        count--;
        idx++; //arranjo de bug
    }

    return;

}