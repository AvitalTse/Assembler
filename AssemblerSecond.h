#include <stdbool.h>
#include "ConstVal.h"
#include "AssemblerFirst.h"

/*Definition of the list of external labels*/

struct External
{
char name[MAX_CHARS];
int address;

struct External *next;
};

/*Definition of functions used by Assembler Second Pass.*/

bool Error_Flag(char [],int ,struct Label *,int *);
void Free_External_List(struct External*);
void Free_Code_List(struct TheCodeBin*);
void Free_Data_List(struct TheDataBin* );
void Create_Extern_File(char [],struct External *);
int Search_EX_Label(char [],struct Label *);
void Insert_External(struct External **,char *,int );
int Up_Date_Address(int,struct TheCodeBin *,struct TheDataBin *);
FILE *Create_Ob_File(char []);
void Word_To_CPU_Lang_Data(struct TheDataBin * ,FILE *);
const char *Address_To_CPU_Lang_Code(struct TheCodeBin *);
void Word_To_CPU_Lang_Code(struct TheCodeBin *,FILE * );
const char *Address_To_CPU_Lang_Data(struct TheDataBin *);
void Code_Label_Word_Insert(struct TheCodeBin**, int ,const char *);
const char *Num_To_Bin_Label(int ,int ,char );
const char *Bin_To_String_Label(int [],int ,char );
int Check_Code_Op_Second(char *,int *);
int Check_For_Registers_Second(char[],char[]);
char const *Search_Label_Second(struct Label*  ,char [],int *);
void Search_Label_Entry(struct Label* , char[]);
void Update_Code_Address(struct TheCodeBin* );
int Update_Data_Address(struct TheDataBin* ,int);
bool Error_Second(char [],int ,struct Label *);




