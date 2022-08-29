#include "ConstVal.h"


/*Defining linked list for storing the macros*/

struct Macro {
char name[MAX_CHARS];
char *str;

struct Macro *next;
};

/*Defining the functions for recognition from the macro_spread function*/
FILE *open_file(char *, char *);
void Macro_Spread(FILE *,FILE* );
void Free_Macro_List(struct Macro *);
int Flag_Macro(char []);
void Macro_Insert(FILE *,struct Macro** ,char [],char []);
int Search_Macro(struct Macro* , char [],FILE *);
void AssemblerFirst(char []);
