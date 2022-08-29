#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "ConstVal.h"
#include "ConstTab.h"
#include "AssemblerFirst.h"

/*Definitions of all the functions used by the Errors files.*/

bool Check_Op_Type(int ,char [],int,struct Label*);
int Check_Index_C(int ,char *,int,struct Label*);
int Check_Index_D(int ,char *);
int Check_regi(char *);
bool Error_First(char [],int,struct Label *);
void Error_First_Print(char [],int,struct Label *);
bool Error_Second(char [],int ,struct Label *);
bool Data_Type(char *,int ,int );
