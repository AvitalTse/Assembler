#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PreAssembler.h"
#include "ConstVal.h"


/*****************************************************************************************************************************************/
/*********************************** A file that contains all the functions used for the Pre Assembler ***********************************/
/*****************************************************************************************************************************************/


/*
A void function that recieves a file containing assembly language code,goes through the program and spreads all the macros that exist in it.
The function runs functions on one line of the code each time until reaches the end of the file which will return null,
 and checks if the line is a macro definition or if it is not ,then changes the flag accordingly and handles each case individually.
@recieves-
file-The open file from type FILE.
out_file-The output file from type FILE.
*/

void Macro_Spread(FILE *file,FILE *out_file)
{
	int flag=0;
	char macro_name[MAX_CHARS];
	char line[MAX_ROW_LENGTH];
	char *token=NULL;
	struct Macro* head = NULL;
	
	
	while(fgets(line,MAX_ROW_LENGTH,file)!=NULL)/*get line by line from file until end of file*/
	{
		flag=Flag_Macro(line);/*activates the function to change the flag in case there is a macro*/
		if(flag==1)/*if there is a macro definition in line*/
			{	
				token=strtok(line," \t\n");
				token=strtok(NULL," \n\t");/*cut the macro name from the line*/
				strcpy(macro_name,token);
				Macro_Insert(file,&head,macro_name,line);/*activate the function that saves the macro name and content in the macro list*/
				flag=0;/*macro definition has ended*/
			}/*if*/
			else if(flag==0)/*no macro definition in line*/
			{
			/*activate the function for printing to output the relevant lines in the case of a macro or a normal line*/
				if(head!=NULL)
				{
				if(Search_Macro(head,line,out_file)==0)
					fputs(line,out_file);
				}
				else
					fputs(line,out_file);
			}/*else if*/	
	}/*while*/
	Free_Macro_List(head);	
}/*macro_spread*/

/*
A function that checks if there is a macro in line.
The function recieves the current line and saves it , for cutting the line and still be able to use the whole current line,
then checks if the first word is "macro" which means there is a macro definition ,if so change the flag to 1,if not return the flag as it is.
@recieves-
line-The current line .
@returns-
1-if there is a macro definition.
0-if not.
 */
 	
int Flag_Macro(char line[])
{
	char save_line[MAX_ROW_LENGTH];
	char *token;
	int flag=0;
	
	strcpy(save_line,line);
	token=strtok(save_line," \t\n");/*the first word of the line*/
	if(token!=NULL)
	{
	if(strcmp(token,MACRO)==0)
		flag=1;
	}	
	return flag;
}/*flag_macro*/

/*
A function that checks if the current line is a call for macro(maccro name) .
The function recieves the current line .
The function goes through all the names of the macros in the macro linked list and checks if the current line is a name of a macro,
if so the content(intialization) of the current macro printed to the output file instead of the macro name.
if the current line is not a name of a macro , returns zero.
@recieves-
head-The head of the macro list.
macro_name-The currrent line.
out_file-The output file.
@returns-
0 - current line is not a macro name.
1 - current line is macro name.
*/

int Search_Macro(struct Macro* head, char macro_name[],FILE *out_file)
{
	char macro[MAX_CHARS];
	char *m=NULL;
	struct Macro* current = head; 
		
	if(macro_name!=NULL)
	{
		strcpy(macro,macro_name);
	      m=strtok(macro," \t\n");
		if(m!=NULL&&current!=NULL)
		{
		     while (current != NULL)
		     {	    
		         if(strcmp(current->name,m)==0)
		         {
        			 fputs(current->str,out_file);
        			 return 1;
           		   }/*if*/
        		   current = current->next;
    			}/*while*/
    		}/*if*/
   	 }/*if*/
    return 0;
}/*searchMacro*/

/*
A void function that saves the macro's name and content into the end of the macro list.
The function is activated only when flag is 1 ,which means there is a macro initialization , 
at first the function inserts the name of the macro to a new node in the macro list ,
then concatenates the next lines into the content at the same node .
@recieves-
file-The open file from type FILE.
head-The current pointer to the head of the list.
macro_name-The macro name.
line-The current line.
*/
 
void Macro_Insert(FILE *file,struct Macro** head,char macro_name[],char line[])
{
	int i=0;
	char buffer[BUFFER_SIZE];
	char save_line[MAX_ROW_LENGTH];
	struct Macro* newNode = (struct Macro*) malloc(sizeof(struct Macro));
	struct Macro* temp;
	
	fgets(line,MAX_ROW_LENGTH,file);/*The first line of the macro intialization content*/
	strcpy(save_line,line);
	
	if(newNode==NULL)
	{
		printf("out of memory space\n");
		return ;
	}/*if*/
	strcpy(newNode->name,macro_name);
	
	strcpy(buffer,line);
	fgets(line,MAX_ROW_LENGTH,file);
	strcpy(save_line,line);
	if(strtok(save_line," \n\t")==NULL)
		strcpy(save_line,line);
	else
		strcpy(save_line,strtok(save_line," \t\n"));
	while(strcmp(save_line,ENDMACRO)!=0)/*insert all the macro initialization content into the list*/
	{
		strcat(buffer,line);
		fgets(line,MAX_ROW_LENGTH,file);
		strcpy(save_line,line);
		if(strtok(save_line," \n\t")==NULL)
			strcpy(save_line,line);
		else
			strcpy(save_line,strtok(save_line," \t\n"));
		
	}/*while*/
	newNode->str=malloc(sizeof(char)*strlen(buffer));
	for(i=0;i<strlen(buffer);i++)
		newNode->str[i]=buffer[i];	
	strcpy(buffer,"");
	
	newNode->next=NULL;
	if(*head==NULL)/*if the list is empty*/
	{
		*head = newNode;/*current macro will be first in the list*/
		return;
	}/*if*/
      temp= *head;
      while(temp->next!=NULL)/*move till the end of the list to insert the new macro*/
            temp = temp->next;
      temp->next = newNode;/*insert the macro to the end of the list*/
}/*Macro_Insert*/

/*
A void function for freeing the memory allocated to the Macro list.
@recieves-
head-The pointer to the macro list.
*/

void Free_Macro_List(struct Macro* head)
{
   struct Macro* temp;
   if(head!=NULL)
   {
   	while (head != NULL)
   	 {
   	    temp = head;
   	    head = head->next;
   	    free(temp);
   	 }/*while*/
   }
}/*Free_Macro_List*/

