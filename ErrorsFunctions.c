#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "ConstVal.h"
#include "ConstTab.h"
#include "Errors.h"

/***********************************************************************************************************************************/
/**************************************** A file contains Specific error checking functions ****************************************/
/***********************************************************************************************************************************/


/*
A function that checks if the sentence is a data directive , if so the function checks the type of directive and sends
 the type to a functoion that check each type if it has legal operands, and if not returns a number , 
 and for each number the function will print a suitable error messege to the user.
@recieves-
token-pointer to the current place in line , the directive.
sen_dir-the sentence directive type.
line_row- the number of the current line in file.
@returns-
true-if there are errors in the line.
false- if there are no error. 
*/

bool Data_Type(char *token,int sen_dir,int line_row)
{
	int index;
	int op_err;
	
	if(sen_dir==1)
	{
		index=Find_Data_Index(token);
		if(index!=2&&index!=0)
			token=strtok(NULL," , \n\t");
		if(index==2)
			token=strtok(NULL," \n\t");
		if(index==0)
			token=strtok(NULL,"\n");
		if(token==NULL)
		{
			printf("Error in Line %d,Invalid operand .\n",line_row);
			return true;
		}/*if*/
		if(token!=NULL);
		{
			op_err=Check_Index_D(index,token);
			if(op_err==0)
			{
				printf("Error in Line %d,Invalid operand for string directive.\n",line_row);
				return true;
			}/*if*/
			if(op_err==1)
			{
				printf("Error in Line %d,Invalid operand for struct directive ,should be an Integer.\n",line_row);
				return true;
			}/*if*/
			if(op_err==2)
			{
				printf("Error in Line %d,Invalid operand for struct directive ,should be a string.\n",line_row);
				return true;
			}/*if*/	
			if(op_err==3)
			{
				printf("Error in Line %d,Invalid operand for data directive ,should be an Integer.\n",line_row);
				return true;
			}/*if*/	
		}/*if*/
	}/*if*/
	else if(sen_dir==-1)
	{
		printf("Error in Line %d,Invalid line of code\n",line_row);
		return true;
	}/*else if*/
	return false;
	
}/*Data_Errors*/
		
/*
A function that recieves the current type of data directive and for each case of type , 
seperates the operands and checks if the operands are legal .
@recieves-
index-the current type.
token-pointer to the current operand.
@returns-
number of error .
*/

int Check_Index_D(int index,char *token)
{
	int i;
	char temp1[MAX_CHARS];
	char temp[MAX_CHARS];
	
	switch(index)
	{
		case 0:/*string*/
			strcpy(temp,token);
			if(temp[0]!='"'||temp[strlen(temp)-1]!='"')
				return 0;
			token=strtok(token,"\"");
			if(token==NULL)
				return 0;
			if(strcmp(token,temp)==0)
				return 0;
			token=strtok(NULL,"\n\t");
			if(token!=NULL)
				return 0;
		break;
		
		case 1:/*struct*/
			if(token!=NULL)
			{
				strcpy(temp,token);
				if(atoi(temp)==0)
					return 1;
				token=strtok(NULL," \n\t");
				if(token==NULL)
					return 2;
				strcpy(temp,token);
				token=strtok(token,"\"");
				if(strcmp(token,temp)==0)
					return 2;			
				token=strtok(NULL," \n\t");
				if(token!=NULL)
					return 2;
			}/*if*/
		break;
		
		case 2:/*data*/
			strtok(token," \n\t");
			strcpy(temp1,token);
			if(temp1[0]==',')
				return 3;
			if(temp1[strlen(temp1)-1]==',')
				return 3;
			for(i=0;i<strlen(temp1);i++)
			{
				if(temp1[i]==',')
				{
					if(atoi(&temp1[i-1])==0)
						return 3;
					if(atoi(&temp1[i+1])==0&&temp1[i+1]!='-'&&temp1[i+1]!='+')
						return 3;
				}/*if*/
				if(atoi(&temp1[i])!=0&&i!=0&&temp1[i]!='-'&&temp1[i]!='+')
				{
					if(temp1[i-1]!='-'&&temp1[i-1]!='+'&&atoi(&temp1[i-1])==0&&temp1[i-1]!=',')
						return 3;
					if(temp1[i+1]=='.')
						return 3;
				}/*if*/
				if(isalpha(temp1[i])!=0)
					return 3;
			}/*for*/
			
		break;
		
		default:
		break;
	}/*switch*/
	return -1;
}/*Check_Index_D*/

/*
A function that recieves the current type of code instruction and for each case of type , 
seperates the operands and checks if the operands are legal .
@recieves-
index-the current type.
token-pointer to the current operand.
@returns-
number of error .
*/

int Check_Index_C(int index,char *token,int line_row,struct Label* head)
{
	int i;
	int op_type1;
	int op_type2;
	char temp[MAX_CHARS];
	char temp_op[MAX_CHARS];
	char temp_op1[MAX_CHARS];
	char temp_op2[MAX_CHARS];
	char *t;
	bool flag1=false;
	bool flag2=false;
	
	switch(index)
	{
		case 14:case 15:
			if(strtok(NULL,"\n\t ")!=NULL)
				return 0;
		break;
		
		case 13:case 12:case 11:case 10:case 9:case 8:case 7:case 5:case 4:
			token=strtok(NULL," \n\t");
			if(token==NULL)
				return 1;
			if(token!=NULL)
			{
				strcpy(temp,token);
				if(temp[strlen(temp)]==','||temp[strlen(temp)]=='+'||temp[strlen(temp)]=='-')
					return 2;
				for(i=0;i<strlen(temp);i++)
				{
					if(temp[i]=='.')
						return 2;
					if(temp[i]=='#')
					{
						if(temp[i+1]!='+'&&temp[i+1]!='-'&&atoi(&temp[i+1])==0)
							return 2;
					}/*if*/
				}/*for*/
				token=strtok(NULL," ,\t\n");
				if(token!=NULL)
				{
					if(strcmp(token,temp)!=0)
						return 1;
				}/*if*/
				else
				{
					strcpy(temp_op,temp);
					op_type1=Check_Mion_Method(temp);
					flag1=Check_Op_Type(op_type1,temp_op,line_row,head);
					if(index!=12)
					{
						if(op_type1==0)
							return 2;
					}/*if*/	
				}/*else*/
			}/*if*/
		break;
		
		case 0:case 1:case 2:case 3:case 6:
			token=strtok(NULL," \t\n");
			if(token==NULL)
				return 3;
			else
			{
				strcpy(temp_op,token);
				token=strtok(NULL," ,\t\n");
				if(token==NULL)
				{
					strcpy(temp_op1,temp_op);
					strtok(temp_op1,",");
					if(temp_op1==NULL||strcmp(temp_op1,temp_op)==0)
						return 3;
					else
					{
						strcpy(temp_op,temp_op1);
						strcpy(temp_op1,strtok(NULL," ,\t\n"));
						if(temp_op1==NULL)
							return 3;
						else
							strcpy(temp,temp_op1);
					}/*else*/
				}/*if*/
				else
				{
					strcpy(temp,token);
				}/*else*/
					t=strtok(temp_op,",");
					if(t==NULL)
						return 6;
					strcpy(temp_op1,t);
					strcpy(temp_op,t);
					t=strtok(temp,",");
					if(t==NULL)
						return 6;
					strcpy(temp_op2,t);
					strcpy(temp,t);
					if(!isalpha(temp_op[0])&&temp_op[0]!='#')
						return 6;
					if(!isalpha(temp[0])&&temp[0]!='#')
						return 6;
					if(!isalpha(temp_op[strlen(temp_op)-1])&&atoi(&temp_op[strlen(temp_op)-1])==0)
					{
						if(!isalpha(temp[0])&&temp[0]!='#')
							return 6;
					}/*if*/
					token=strtok(NULL," \t\n");
					if(token!=NULL)
							return 3;
					else
					{
						
						op_type1=Check_Mion_Method(temp_op);
						op_type2=Check_Mion_Method(temp);
						flag1=Check_Op_Type(op_type1,temp_op1,line_row,head);
						flag2=Check_Op_Type(op_type2,temp_op2,line_row,head);
						if(index==6)
						if(op_type1==0||op_type1==3)
									return 4;
						if(index!=1)
							if(op_type2==0)
								return 5;
					}/*else*/		
				}/*else*/
		break;
		
	}/*switch*/
	if(flag1||flag2)
		return 6; 
return -1;
}/*Check_Index_C*/

/*A function that checks if the current operand is a register
@recieves-
token-current operand.
@returns-
-1-the operand is register.
0-the operand is not register.

*/

int Check_regi(char *token)
{
	int i;
	
	for(i=0;i<MAX_REGIS;i++)
		if(strcmp(regi[i],token)==0)
			return -1;
	return 0;
}/*Check_Regi*/

/*
A function that if the operand is illegal .the function recieves the type of operand using the addressing method of each operand and the operand itself ,and checks all kinds of error cases that can occur according to each operand and prints a suitable error messege.
@recieves-
op_type-the addressing method number
line_row- the number of the current line in file.
op-the operand.
head-pointer to the labels list.
@returns-
true-if there are errors in the line.
false- if there are no error.
*/

bool Check_Op_Type(int op_type,char op[],int line_row,struct Label *head)
{
	char temp[MAX_CHARS];
	char temp1[MAX_CHARS];
	switch(op_type)
	{
		case 0:
			op=strtok(op,"#");
			op=strtok(op," +-");
			strcpy(temp,op);
			if(strcmp(temp,strtok(op,"."))!=0||atoi(op)==0||op==NULL)
			{
				printf("Error in Line %d,Invalid operand ,should be an Integer.\n",line_row);
				return true;
			}/*if*/
		break;
		
		case 1:
			if(Search_Label(head,op)=='1')
			{
				printf("Error in Line %d,The type of the operand does not exist, label is not defined.\n",line_row);
				return true;
			}/*if*/
		break;
		
		case 2:
			strcpy(temp,strtok(op,"."));
			strcpy(temp1,strtok(NULL," \n\t"));
			if(Search_Label(head,temp)=='1')
			{
				printf("Error in Line %d,The type of the operand does not exist, label is not defined.\n",line_row);
				return true;
			}/*if*/
			if(atoi(temp1)==0||atoi(temp1)>2||atoi(temp1)<1)
			{
				printf("Error in Line %d,struct type can point to the first or second place in the definition of the struct.\n",line_row);
				return true;
			}/*if*/
		break;
		
		default:
		break;	
	}/*switch*/
	return false;
}/*Check_Op_Type*/

/*
A function that checks if there are errors in each line in the second pass with an emphasis on errors of defining labels and prints suitable error messege.
@recieves-
line-the current line.
line_row-the current line number.
head-pointer to the labels list.
@returns-
false-if there are no errors in the line.
true-if there are errors in the line
*/

void Error_First_Print(char line[],int line_row,struct Label *head)
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
			{
				printf("Error in Line %d,Label name can not contain spaces.\n",line_row);
				return;
			}
		}/*for*/		
		if(Check_Sentence(token)!=-1||Check_regi(token)==-1)
		{
			printf("Error in Line %d,Label name can not be a directive or register name.\n",line_row);
			return;
		}
		if(strlen(save_line)>MAX_CHARS)
		{
			printf("Error in Line %d,The Label name exceeds the maximum.\n",line_row);	
			return;
		}
		if(c==EX_ENCODE)
		{
			printf("Error in Line %d,The Label was previously defined as external.\n",line_row);
			return;
		}
		if(!isalpha(save_line[0]))
		{
			printf("Error in Line %d,illegal name for Label , must start with a letter.\n",line_row);
			return;
		}
		if(token==NULL)
		{
			printf("Error in Line %d,Invalid label definition.\n",line_row);
			return;
		}
		token=strtok(NULL," \t\n");
		sen_dir=Check_Sentence(token);/*check what kind of directive the label have*/
		if(sen_dir==2)
		{
			printf("Error in Line %d,A label definition cannot contain a extern or entry directive.\n",line_row);
			return;
		}
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
		{
			printf("Error in Line %d,an extern or entry directive can not be empty.\n",line_row);
			return;
		}
		else
		{
			token=strtok(NULL," \t\n");
			if(token!=NULL)
			{
				printf("Error in Line %d,an extern or entry directive can not have more than one operand.\n",line_row);
				return;
		}
		}/*else*/
	}/*if*/
}/*Error_First_Print*/

