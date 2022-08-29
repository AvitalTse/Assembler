#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "ConstVal.h"
#include "ConstTab.h"
#include "AssemblerFirst.h"


/*************************************************************************************************************************************/
/**************************************** A file contains functions for use by multiple files ****************************************/
/*************************************************************************************************************************************/


/*
A function that returns the sentence type using functions that runs throw all the possible types and compare them to the sentence type.
@recieves-
token-the sentence type.
@returns-
0-if sentence is code type.
1-if sentence is data type.
2-default if not code or data , can be extern or entry.
*/

int Check_Sentence(char *token)
{
	if(Sentence_Is_Code(token))
		return 0;
	else if(Sentence_Is_Data(token))
		return 1;
	else if(Sentence_Is_EX_EN(token))
		return 2;
	return -1;
}/*Check_Sentence*/

/*
A bool function that runs all over the possible code types and compare the recieved token to see, if equal return true else false.
@recieves-
token-the sentence type.
@returns-
true-if there is a match.
false-if not code type.
*/
	
bool Sentence_Is_Code(char *token)
{
	int i;
	
	for(i=0;i<MAX_CODE_INS;i++)
	{
		if(strcmp(token,code[i])==0)
			return true;
	}/*for*/
return false;
}/*Sentence_Is_Code*/

/*
A bool function that runs all over the possible data types and compare the recieved token to see, if equal return true else false.
@recieves-
token-the sentence type.
@returns-
true-if there is a match.
false-if not data type.
*/

bool Sentence_Is_Data(char *token)
{
	int i;
	
	for(i=0;i<MAX_DATA_DIR;i++)
	{
		if(strcmp(token,data[i])==0)
			return true;
	}/*for*/
return false;
}/*Sentence_Is_Data*/

/*
A bool functions that checks if the sentence is definition of external or internal label.
@recieves-
token-the type of directive.
@returns-
true-if there is a definition of external or internal label.
false-if not.
*/

bool Sentence_Is_EX_EN(char *token)
{
	int i;
	
	for(i=0;i<EX_EN;i++)
	{
		if(strcmp(token,ex_en[i])==0)
			return true;
	}/*for*/
return false;
}/*Sentence_Is_EX_EN*/	

/*
A functions that returns the addressing method index for getting the binary meaning of it to convert the code instructuon to binary .
The function recieves the operand and for each operand type return the suitable addressing method.
@recieves-
op-the operand.
@returns-
0-if the operand is a number.
1-if the operand is a label.
2-if the operand is a struct.
3-if the operand is a register.
*/

int Check_Mion_Method(char op[])
{
	int num_mion=1;
	int i;
	char op_temp[MAX_CHARS];
	
	if(op!=NULL)
	{
		strcpy(op_temp,op);
		strtok(op,".");/*a struct will always have a "." sign*/
		if(op!=NULL)
		{
			if(strcmp(op,op_temp)!=0)/*a struct*/
			{	
				num_mion=2;
				return num_mion;
			}/*if*/
			op=strtok(op," #\n\t");/*a number will always start with "#" sign*/
			if(op!=NULL)
			{
				if(strcmp(op_temp,op)!=0)/*a number*/
				{	
					num_mion=0;
					return num_mion;
				}/*if*/
				for(i=0;i<MAX_REGIS;i++)
				{
					if(strcmp(op,regi[i])==0)/*compares the operand with all the possible registers*/
					{
						num_mion=3;
						return num_mion;/*a register*/
					}/*if*/
				}/*for*/
			}
		}
	}
return num_mion;/*a label*/
}/*Check_Mion_Method*/
			


/*
A function that opens a file and checks if it opened successfully.
The function recieves the name and operating mode of the file , and checks if the file is opened without returning NULL , if returns NULL prints error message to the user .
@recieves-
The name of the file from type char pointer.
The operating mode of the file from type char pointer.
@return-the opened file.
*/

FILE *open_file(char *name, char *mode)
{
	FILE *fp;
	
	if(!(fp=fopen(name,mode)))/*checks if opening the file returns NULL*/
		printf("\nERROR,File cannot be opened,%s does not exist.\n",name);
	return(fp);
}/*open_file*/

/*
A bool function that returns if the sentence have a label definition or not.
the function compars the token that holds the label incase there is one to the whole line.
if they are equal it means there is no label initialization .
@recieves-
save_line-the whole current line.
token-the alleged label.
@returns-
true-if there is a label.
false-if not.
*/

bool Label_Flag(char save_line[],char *token)
{
	if(strcmp(token,save_line)!=0)
		return true;
return false;
}/*Label_Flag*/

/*
A functions that gets the code instruction type.
@recieves-
token-the code instruction.
@returns-the type of code instruction.
*/

int Find_Code_Index(char *token)
{
	int i;
	
	for(i=0;i<MAX_CODE_INS;i++)
	{
		if(strcmp(token,code[i])==0)
			return i;
	}/*for*/
	return i;
}/*Find_Code_Index*/

/*
A bool function that checks if the line is empty or a note in the input file , if so the function returns true .
@recieves-
line-the current line of the file.
@returns-
true-if the line is note or empty .
false- if not.
*/

bool Check_Empty_Line(char line[])
{
	char save_line[CHECK_LEN];
	
	strcpy(save_line,line);
	if(strtok(save_line," \t\r\n\f\v ")==0)
	{
		return true;
	}/*if*/
	strcpy(save_line,line);
	if(strcmp(strtok(save_line,";"),line)!=0)
		return true;
return false;
}/*Check_Empty_Lines*/

/*
A function that checks if the line contains more chars than the maximum of 80 chars.
@recieves-
line-the current line.
@returns-
true-if the line contains more than 80 chars.
false-if the line is legal.
*/

bool Check_Line_Length(char line[])
{
	if(strlen(line)>MAX_ROW_LENGTH)
	{
		return true;
	}/*if*/
return false;
}/*Check_Line_Length*/
	
/*
A functions that gets the data directive type.
@recieves-
token-the data directive.
@returns-the type of data directive.
*/

int Find_Data_Index(char *token)
{
	int i;
	
	for(i=0;i<MAX_DATA_DIR;i++)
	{
		if(strcmp(token,data[i])==0)
			return i;
	}/*for*/
	return i;
}/*Find_Data_Index*/


/*
A function that checks if the current label is already in the labels list.
The function recieves the label and goes through all the names of the labels in the labels list and checks if the current label is already exists and if it is external.
@recieves-
head-The head of the Internal list.
label-the current label name.
@returns-
'1'- current label is not in list.
'E' - current label is external and already defined.
*/
		
char Search_Label(struct Label* head, char label[])
{
    char new_label='1';
    struct Label* current = head;  
  
    while (current != NULL)
    {
        if (strcmp(current->name,label)==0)
        {
        	if(current->are==EX_ENCODE)
        		 return current->are;
        	else
           		 return current->are;
        }/*if*/
        current = current->next; 
    }/*while*/
    return new_label;
}/*Search_Label*/

/*
A void function that converts the address of the label into the language of the processor and puts it in the output file.
the function recieves the address of the label and using a function converts the number to binary word and then the function 
converts it to the 32 bit language of the CPU .
@recieves-
file-the output file(ob).
ADD-the number to convert.
*/
	
void Add_To_CPU_Lang_Code(FILE *file,int ADD)
{	
	int i;
	int sign_num=0;
	int cpu_bits=CPU_BITS;
	int wordi=0;  
	char word;
	const char *WordADD=Num_To_Bin_Label(ADD,WORD_LEN_BITS,NO_ENCODE);
	
	for(i=0;i<WORD_LEN_BITS;i++)
	{
		cpu_bits--;
		word=WordADD[i];
		wordi=atoi(&word);
		sign_num+=((wordi)*pow(2,cpu_bits));	
		if(cpu_bits==0)
		{
			cpu_bits=CPU_BITS;
			if(sign_num!=0)
				fprintf(file,"%c",CPU_LANG[sign_num]);
			sign_num=0;
		}/*if*/			
	}/*for*/
	fprintf(file,"\t");
}/*Add_To_CPU_Lang_Code*/
