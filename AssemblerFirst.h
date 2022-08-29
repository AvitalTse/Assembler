#include <stdbool.h>
#include "ConstVal.h"

/*Definition of the Symbols list.*/

 struct Label 
{
char name[MAX_CHARS];
int address;
char are;

struct Label *next;
};

/*Definition of the Code words list.*/

struct TheCodeBin
{
int address;
const char *word;

struct TheCodeBin *next;
};

/*Definition of Data words list.*/

struct TheDataBin
{
int address;
const char *word;

struct TheDataBin *next;
};

/*Definition of Entry symbols list.*/

struct Internal
{
char name[MAX_CHARS];
int address;

struct Internal *next;
};




/*Definition of functions used by Assembler First Pass.*/

struct Label *exter(char *,struct Label *,struct Internal *);
struct Internal *entry(char *,struct Label *,struct Internal *);
void Create_Entry_File(char [],struct Internal*,struct Label*);
bool AssemblerSecond(char[],struct Label* ,struct TheCodeBin* ,struct TheDataBin *);
const char *Bin_To_String(int [],int);
const char *Num_To_Bin(int ,int ,int );
const char *Chars_To_Bin(int ,int );
char* substring(char *, const char *, int, int );
const char *Code_Of_Num(char *,int);
struct TheDataBin *Code_Of_String(char *,int ,struct TheDataBin* ,int);
void Data_Is_String(char *,int *);
void Data_Is_Struct_Second_Op(char *,int *);
void Data_Is_Struct_First_Op(char *,int *);
void Data_Is_Data(char *,int *);
int DC_Update(char [],char [],int ,int );
void Label_Insert(struct Label** , int ,char [],char );
void Code_Word_Insert(struct TheCodeBin** , int,const char *);
void Data_Word_Insert(struct TheDataBin** , int ,const char *);
void Update_Label_Table(struct Label*,int );
void Insert_Internal(struct Internal **,char []);
int Search_Internal(struct Internal* , char []);
const char *Case_Of_Code(int ,char [],char []);
int Check_For_Registers(char [],char []);
const char *Code_Of_Registers(char [],char  []);
const char *Check_Code_Op(char [],char [],int *,int);
int Search_Label_Add(struct Label*, char[]);
void Free_Label_List(struct Label* );
void Free_Internal_List(struct Internal* );

/*Definition of shared functions used by Assembler First and Second Pass*/

bool Check_Line_Length(char []);
bool Error_First(char [],int,struct Label *);
const char *Num_To_Bin_Label(int ,int ,char );
const char *Bin_To_String_Label(int [],int,char);
void Add_To_CPU_Lang_Code(FILE *,int  );
FILE *open_file(char *, char *);
int Find_Data_Index(char *);
int Find_Code_Index(char *);
int Check_Sentence(char *);
bool Label_Flag(char [],char *);
bool Sentence_Is_EX_EN(char *);
bool Sentence_Is_Data(char *);
bool Sentence_Is_Code(char *);
char Search_Label(struct Label* , char []);
int Check_Mion_Method(char []);
bool Check_Empty_Line(char []);

