#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "ConstVal.h"
#include "ConstTab.h"
#include "Errors.h"


/*******************************************************************************************************************************************/
/****************************** A file that contains functions to handle errors for the first and second pass ******************************/
/*******************************************************************************************************************************************/


/*
A function that checks if there are errors in each line in the first pass with an emphasis on errors of defining labels and prints suitable error messege.
@recieves-
line-the current line.
line_row-the current line number.
head-pointer to the labels list.
@returns-
false-if there are no errors in the line.
true-if there are errors in the line
*/

bool Error_First(char line[],int line_row,struct Label *head)
{
	int sen_dir;
	int i;
	char *token=NULL;
	char save_line[MAX_ROW_LENGTH];
	char c;
	bool flag; 
	
	strcpy(save_line,line);
	token=save_line;
	strtok(token,":");
	flag=Label_Flag(line,token);
	if(flag)
	{
		c=Search_Label(head,save_line);
		for(i=0;i<strlen(save_line);i++)
		{
			if(isspace(save_line[i])!=0)
				return true;	
		}/*for*/		
		if(Check_Sentence(token)!=-1||Check_regi(token)==-1)
			return true;
		if(strlen(save_line)>MAX_CHARS)	
			return true;
		if(c!='1'&&c!=EX_ENCODE)
		{
			printf("Error in Line %d,The Label was previously defined.\n",line_row);
			return true;
		}/*if*/
		if(c==EX_ENCODE)
			return true;
		if(!isalpha(save_line[0]))
			return true;
		if(token==NULL)
			return true;
		token=strtok(NULL," \t\n");
		sen_dir=Check_Sentence(token);/*check what kind of directive the label have*/
		if(sen_dir==2)
			return true;
	}/*if*/
	else/*not a label*/
	{
	token=strtok(token," \t\n");/*the directive*/
	sen_dir=Check_Sentence(token);
	}/*else*/
	if(sen_dir==2)
	{
		token=strtok(NULL," \t\n");/*the operand*/
		if(token==NULL)
			return true;
		else
		{
			token=strtok(NULL," \t\n");
			if(token!=NULL)
				return true;
		}/*else*/
	}/*if*/
	return false;
}/*Error_First*/

/*
A function that checks if there are errors in each line in the first pass with an emphasis on errors of defining labels and prints suitable error messege.
@recieves-
line-the current line.
line_row-the current line number.
head-pointer to the labels list.
@returns-
false-if there are no errors in the line.
true-if there are errors in the line
*/ 

bool Error_Second(char line[],int line_row,struct Label *head)
{
	int index;
	int op_err;
	int sen_dir;
	char *token;
	char temp[MAX_CHARS];
	char save_line[MAX_ROW_LENGTH];
	char c;
	bool flagD;
	bool flag;
	
	strcpy(save_line,line);
	Error_First_Print(line,line_row,head);
	token=strtok(save_line,":");
	flag=Label_Flag(line,token);
	if(flag)/*if there is label definition*/
		token=strtok(NULL," \t\n");
	else
		token=strtok(save_line," \t\n");/*the directive*/
	sen_dir=Check_Sentence(token);
	if(sen_dir==2)/*extern ot entry*/
	{
		strcpy(temp,token);
		c=Search_Label(head,temp);
		if(strcmp(temp,ex_en[0])==0)
		{
			token=strtok(NULL," \t\n");
			if(token!=NULL)
			{
				strcpy(temp,token);
				if(Check_Sentence(token)!=-1||Check_regi(token)==-1)
				{
					printf("Error in Line %d,Label name can not be a directive or register name.\n",line_row);
					return true;
				}/*if*/
				if(strlen(temp)>MAX_CHARS)
				{
					printf("Error in Line %d,The Label name exceeds the maximum.\n",line_row);
					return true;
				}/*if*/
				if(!isalpha(temp[0]))
				{
					printf("Error in Line %d, illegal name for Label , must start with a letter.\n",line_row);
					return true;
				}/*if*/
				if(c!='1'&&c!=EX_ENCODE)
				{
					printf("Error in Line %d,The Label was already defined.\n",line_row);
					return true;
				}/*if*/
			}/*if*/	
		}/*if*/
		else if(strcmp(temp,ex_en[1])==0)
		{
			token=strtok(NULL," \t\n");
			if(token!=NULL)
			{
				strcpy(temp,token);
				c=Search_Label(head,temp);
				if(c==EX_ENCODE)
				{
					printf("Error in Line %d,The Label wae already defined as External.\n",line_row);
					return true;
				}/*if*/
				else if(c=='1')
				{
					printf("Error in Line %d,Entry can be used only when Label is already defined.\n",line_row);
					return true;
				}/*else if*/
			}/*if*/
		}/*else if*/
	}/*if*/
	else if(sen_dir==0)/*code*/
	{
		index=Find_Code_Index(token);
		op_err=Check_Index_C(index,token,line_row,head);
		if(op_err==0)
		{
			printf("Error in Line %d,This type of instruction cannot contain operands.\n",line_row);
			return true;
		}/*if*/
		if(op_err==1)
		{
			printf("Error in Line %d,This type of instruction cannot contain more or less than one operand.\n",line_row);
			return true;
		}/*if*/
		if(op_err==2)
		{
			printf("Error in Line %d,This type of instruction cannot contain number as operand.\n",line_row);
			return true;
		}/*if*/
		if(op_err==3)
		{
			printf("Error in Line %d,This type of instruction cannot contain more or less than two operand.\n",line_row);
			return true;
		}/*if*/
		if(op_err==4)
		{
			printf("Error in Line %d,This type of instruction cannot contain number or register as first operand.\n",line_row);
			return true;
		}/*if*/
		if(op_err==5)
		{
			printf("Error in Line %d,This type of instruction cannot contain number as second operand.\n",line_row);
			return true;
		}/*if*/
		if(op_err==6)
		{
			printf("Error in Line %d,Illegal type of operand.\n",line_row);
			return true;
		}/*if*/
	
	}/*else if*/
	flagD=Data_Type(token,sen_dir,line_row);
	if(flagD)
		return true;
	return false;
}/*Error_Second*/
