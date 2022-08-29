#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "AssemblerFirst.h"
#include "ConstVal.h"
#include "ConstTab.h"

/********************************************************************************************************************************************/
/**************************************** A file that contains all the functions used for first pass ****************************************/
/********************************************************************************************************************************************/



/*
A void function that recieves the current file name and creates a file of the same name just for the entry labels that are in the file, 
the function takes the entry labels from the Internal list and their addresses from the label list using another function,
converts the address to the language of the CPU,then inserts to the entry file and when reaches the end of the list closes the file.
@recieves-
out_name-name of the current file.
headI-pointer to the Internal list.
headL-pointer to the Labels list.
*/

void Create_Entry_File(char out_name[],struct Internal *headI,struct Label *headL)
{
	int add;
	FILE *file;
	struct Internal *head=headI;
	
	strtok(out_name,".");
	strcat(out_name,".ent");
	if(head!=NULL)
	{
		file=open_file(out_name,"w");
		if(file!=NULL)
		{
			while(head!=NULL)
			{
				fprintf(file,"%s\t",head->name);
				add=Search_Label_Add(headL,head->name);/*saves the address of the label*/
				Add_To_CPU_Lang_Code(file,add);/*sends the address of the label to a function that converts it to 32 bit of the CPU and inserts to the out file*/
				fprintf(file,"\n");
				head=head->next;
			}/*while*/
			fclose(file);
		}/*if*/
	}/*if*/
}/*Create_Entry_File*/

/*
A function that checks the type of a operand of the code instruction and according the type, the function advances the instruction counter,
and also for each operand activate the suitable function for converting the operands to their binary value. than using a pointer for returning
the word at the end of the function. 
@recieves-
op-the current operand to check.
op2-the second operand for activating registers functions in case there is one .
*IC-the instruction counter as pointer for the abillity to change it value.
word_length-the amounts of bits to use for converting to binary.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/

const char *Check_Code_Op(char op[],char op2[],int *IC,int word_length)
{
	int L;
	char op_temp[MAX_CHARS];
	const char *Word=NULL;
	
	strcpy(op_temp,op);
	strtok(op,".");
	if(strcmp(op,op_temp)!=0)/*struct type operand*/
	{
		L=2;/*one place is saved for the struct label's adress and one for the number of struct*/
		strcpy(op_temp,strtok(NULL," \n\t"));/*save the number of the struct*/
		Word=Code_Of_Num(op_temp,word_length);/*convert the number to a word*/
		*IC=*IC+L;/*update the instruction counter*/
		return Word;
	}/*if*/
	strcpy(op_temp,strtok(op,"# \n\t"));
	if(strcmp(op,op_temp)!=0)/*number type operand*/
	{
		L=1;/*one place in memory saved for a number*/
		Word=Code_Of_Num(op_temp,word_length);/*convert the number to a word*/
		*IC=*IC+L;/*update the instruction counter*/
		return Word;
	}/*if*/
	if(Check_For_Registers(op,op)==0)/*label type operand*/
	{
		L=1;/*one place in memory saved for a label*/
		*IC=*IC+L;/*update the instruction counter*/
		return Word;/*will be NULL since address is still unknown*/
	}/*if*/
	Word=Code_Of_Registers(op,op2);/*convert the register to a word*/
	if(Word!=NULL)/*register type operand*/
	{
		L=1;/*one place in memory saved for a register operand*/
		*IC=*IC+L;/*update the instruction counter*/
	}/*if*/
	return Word;
}/*Check_Code_Op*/			
	
/*
A function that recieves the  the operands. the function only activate when the sentence is from code instruction type, 
and at least one of the operands is a register.
The function is responsible for converting to binary  register type operand. and so to create a new word to the code table.
the functions intialize a string for getting 10 bits as chars then for each case insert the suitable registger code and at last
 the 2 bits saved for the encoding type which in this case will always be absolute.
@recieves-
op1-the first operand if there any.
op2-the second operand if there any.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/

const char *Code_Of_Registers(char op1[],char  op2[])
{
	int i;
	int j=0;
	int num_of_r1=-1;
	int num_of_r2=-1;
	char code_binary[WORD_LEN_STR];
	char *Word=NULL;
	Word= (char *) malloc (sizeof(char) * (WORD_LEN_STR));
	
	for(i=0;i<CODE_BITS;i++)/*check which one of the operands is a register or not*/
	{
		if(strcmp(op1,regi[i])==0)
			num_of_r1=i;
		if(strcmp(op2,regi[i])==0)
			num_of_r2=i;
	}/*for*/
	if(num_of_r1!=-1&&num_of_r2!=-1)/*both operands are register*/
	{
		strcpy(code_binary,regi_bin[num_of_r1]);/*inserts the 4 bits code of the first register number*/
		strcat(code_binary,regi_bin[num_of_r2]);/*concatenate the 4 bits code of the second register number*/
		strcat(code_binary,A_R_E[0]);/*concatenate the encoding type*/
	}/*if*/
	else if(num_of_r1!=-1&&num_of_r2==-1)/*only first operand is register*/
	{
		strcpy(code_binary,regi_bin[num_of_r1]);/*the 4 bits code of the first register number*/
		strcat(code_binary,regi_bin[0]);/*concatenate 4 bits of 0 since we dont have register*/
		strcat(code_binary,A_R_E[0]);/*concatenate the encoding type*/
	}/*else if*/
	else if(num_of_r1==-1&&num_of_r2!=-1)/*only second operand is register*/
	{
		strcpy(code_binary,regi_bin[0]);/*insert 4 bits of 0 since we dont have register*/
		strcat(code_binary,regi_bin[num_of_r2]);/*concatenate the 4 bits code of the second register number*/
		strcat(code_binary,A_R_E[0]);/*concatenate the encoding type*/
	}/*else if*/
	if(num_of_r1!=-1||num_of_r2!=-1)/*if at least one is register*/
	{
		for(i=0;i<WORD_LEN_BITS;i++)
		{
			Word[j] = code_binary[i] ;/*take the whole string and tranfer to the returned const word pointer*/
			j++;
		}/*for*/
		Word[WORD_LEN_BITS] = '\0';
	}/*if*/
	return Word;
}/*Code_Of_Registers*/

/*
A function to check whether the code instucrion has both operands as registers type , or only one and whether the only the 
second operand is register , the functions gives the instruction how should the operand code be coded as binary. 
since each position of the regisgter affects the code in case there is a register as operand.
@recieves-
op1-the first operand.
op2-the second operand.
@returns-
0-in case non of the operands are registers.
1-if only the first operand is a register.
2-if both operands are registers.
-1-if only the second operand is register.
*/

int Check_For_Registers(char op1[],char op2[])
{
	int i;
	int r=0;
	int r2=0;
	
	for(i=0;i<MAX_REGIS;i++)
	{
		if(strcmp(op1,regi[i])==0)
			r++;
		if(strcmp(op2,regi[i])==0)
		{
			r++;
			r2=-1;
		}/*if*/
	}/*for*/
	if(r!=2&&r2==-1)
		return r2;
	return r;
}/*Check_For_Registers*/

/*
A function that recieves the code instruction type, and the operands. the function only activate when the sentence is from code instruction type.
The function is responsible for converting to binary the code instruction. and so to creat a new word to the code table.
the functions intialize a string for getting 10 bits as chars then for each case inserts first the opcode to the string ,
 then concatenate each operand's addressing method code , and at the end the ARE symbol , in this case absolute.
@recieves-
index-the type of code instruction.
op1-the first operand if there any.
op2-the second operand if there any.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/
	
const char *Case_Of_Code(int index,char op1[],char op2[])
{
	int j=0;
	int i;
	char code_binary[WORD_LEN_STR];
	char op1_temp[MAX_CHARS];
	char op2_temp[MAX_CHARS];
	char *Word;
	Word = (char *) malloc (sizeof(char) * (WORD_LEN_STR));
	
	strcpy(op1_temp,op1);
	strcpy(op2_temp,op2);
	if(index==0||index==1||index==2||index==3||index==6)/*2 operands instructions*/
	{
		strcpy(code_binary,code_bin[index]);/*insert the opcode*/
		strcat(code_binary,add_method_bin[Check_Mion_Method(op1_temp)]);/*concatenate the first operand's addressing method*/
		strcat(code_binary,add_method_bin[Check_Mion_Method(op2_temp)]);
		strcat(code_binary,A_R_E[0]);/*concatenate the encoding type*/
	}/*if*/	
	else if(index==14||index==15)/*non operands instructions*/
	{
		strcpy(code_binary,code_bin[index]);/*insert the opcode*/
		strcat(code_binary,add_method_bin[0]);/*concatenate the addressing method of no operand*/
		strcat(code_binary,add_method_bin[0]);/*concatenate the addressing method of no operand*/
		strcat(code_binary,A_R_E[0]);/*concatenate the encoding type*/
	}/*else if*/
	else/*1 operands instructions*/
	{
		strcpy(code_binary,code_bin[index]);/*insert the opcode*/
		strcat(code_binary,add_method_bin[0]);/*concatenate the addressing method of no operand*/
		strcat(code_binary,add_method_bin[Check_Mion_Method(op1_temp)]);/*concatenate the operand's addressing method*/
		strcat(code_binary,A_R_E[0]);/*concatenate the encoding type*/
	}/*else*/		
	for(i=0;i<WORD_LEN_BITS;i++)
	{
		Word[j] = code_binary[i];/*take the whole string and tranfer to the returned const word pointer*/
		j++;
	}/*for*/
	Word[WORD_LEN_BITS] = '\0';
	return Word;
}/*Case_Of_Code*/

/*
A function that checks if the current label is already in the entry labels list.
The function recieves the label and goes through all the names of the labels in the entry labels list and checks if
 the current label is already exists,if so, the function prints an error message.
@recieves-
head-The head of the Internal list.
label-the current label name.
@returns-
0 - current label already in list.
1 - current label is not in the list.
*/

int Search_Internal(struct Internal* head, char label[])
{
    struct Internal* current = head;  
  
    while (current != NULL)
    {
        if (strcmp(current->name,label)==0)
            return 0;
        current = current->next;
    }/*while*/
    return 1;
}/*Search_Internal*/

/*
A void function that saves the entry label into the end of the entry labels list.
The function is activated only when there is a entry directive,which means there is a new entry label.
@recieves-
head-The current pointer to head of the the list.
label-the entry label.
*/

void Insert_Internal(struct Internal **head,char label[])
{
	struct Internal* newNode = (struct Internal*) malloc(sizeof(struct Internal));
	struct Internal* temp;
	
	if(newNode==NULL)
	{
		printf("out of memory space\n");
		return ;
	}/*if*/
	strcpy(newNode->name,label);
	newNode->next=NULL;
	if(*head==NULL)/*if the list is empty*/
	{
		*head = newNode;/*current entry will be first in the list*/
		return;
    	}/*if*/
	temp= *head;
      while(temp->next!=NULL)/*move till the end of the list to insert the new entry*/
		temp = temp->next;
      temp->next = newNode;/*insert the entry to the end of the list*/
}/*Insert_Internal*/

/*
A void function that recieves the list of the labels and update the addresses of each label ,
if it is a type code label then adds 100 to the address num (since 100 places in memrory are already in use) and since 
the data words need to be after the code words , the function update the addresses of the data type labels by adding the number of code words and 100.
@recieves-
node-pointer to the head of the label list.
cnt-100+the number of code words.
*/
	
void Update_Label_Table(struct Label* node,int cnt)
{
	while(node!=NULL)
	{
  	  	if(node->are==NO_ENCODE)
      		node->address+=cnt;
   		else if(node->are==REL_ENCODE)
   			node->address+=MEM_INT;
       	node = node->next;
       }/*while*/
}/*Update_Label_Table*/

/*
A void function that saves the data's word and address into the end of the binary data list.
The function is activated only when there is a data directive,which means there is a new data word.
@recieves-
head-The current pointer to head of the the list.
address-The address.
Word-a mutable pointer to an immutable string of the word display .
*/

void Data_Word_Insert(struct TheDataBin** head, int address,const char *Word)
{
      struct TheDataBin* newNode = (struct TheDataBin*) malloc(sizeof(struct TheDataBin));
      struct TheDataBin* temp;
      
 	if(newNode==NULL)
	{
		printf("out of memory space\n");
		return ;
	}/*if*/
	newNode->word=Word;
	newNode->address=address;
	newNode->next=NULL;
      if(*head==NULL)/*if the list is empty*/
      {
            *head = newNode;/*current word will be first in the list*/
            return;
      }/*if*/
      temp= *head;
      while(temp->next!=NULL)/*move till the end of the list to insert the new word*/
            temp = temp->next;
      temp->next = newNode;/*insert the word to the end of the list*/
}/*Data_Word_Insert*/

/*
A void function that saves the code's word and address into the end of the binary code list.
The function is activated only when there is a code instruction,which means there is a new code word.
@recieves-
head-The current pointer to head of the the list.
address-The address.
Word-a mutable pointer to an immutable string of the word display .
*/

void Code_Word_Insert(struct TheCodeBin** head, int address,const char *Word)
{
      struct TheCodeBin* newNode = (struct TheCodeBin*) malloc(sizeof(struct TheCodeBin));
      struct TheCodeBin* temp;
      
 	if(newNode==NULL)
	{
		printf("out of memory space\n");
		return ;
	}/*if*/
	newNode->word=Word;
	newNode->address=address;
	newNode->next=NULL;
      if(*head==NULL)/*if the list is empty*/
      {
            *head = newNode;/*current word will be first in the list*/
            return;
      }/*if*/
      temp= *head;
      while(temp->next!=NULL)/*move till the end of the list to insert the new word*/
            temp = temp->next;
      temp->next = newNode;/*insert the word to the end of the list*/
}/*Code_Word_Insert*/

/*
A void function that saves the label's name and address into the end of the label list.
The function is activated only when flag is 1 ,which means there is a label initialization .
@recieves-
head-The current pointer to head of the the list.
cnt-The address.
label-The label name.
are-Letters indicating the type of encoding, if there any.
*/

void Label_Insert(struct Label** head, int cnt,char label[],char are)
{
      struct Label* newNode = (struct Label*) malloc(sizeof(struct Label));
      struct Label* temp;

      if(newNode==NULL)
	{
		printf("out of memory space\n");
		return ;
	}/*if*/
	strcpy(newNode->name,label);
	if(are!=EX_ENCODE)/*external type have no address*/
		newNode->address=cnt;
	newNode->are=are;
	newNode->next=NULL;
      if(*head==NULL)/*if the list is empty*/
      {
            *head = newNode;/*current label will be first in the list*/
            return;
      }/*if*/
      temp= *head;
      while(temp->next!=NULL)
            temp = temp->next;
      temp->next = newNode;
}/*Label_Insert*/

/*
A functions that checks the operands of a data directive and for each case of data directive , 
updating the data counter using the suitable function for each case, since each data dirfective takes different amount of memory.
 and returns the update data counter.
@recieves-
op1-the first operand.
op2-the second operand.
DC-the data counter.
index-the data directive type.
@returns-
the updated data counter.
*/
	
int DC_Update(char op1[],char op2[],int DC,int index)
{
	switch(index)
	{
		case 0:
			Data_Is_String(op1,&DC);	
		break;
		
		case 1:
			Data_Is_Struct_First_Op(op1,&DC);
			Data_Is_Struct_Second_Op(op2,&DC);
		break;
		
		case 2:
			Data_Is_Data(op1,&DC);
		break;
		
		default:
		return 0;
	}/*switch*/
	return DC;	
}/*DC_Update*/

/*
A void function for updating the data counter in case of a data in a directive sentence which will always be a num.
@recieved-
token-the num.
DC-the data counter pointer to be able to change it directly in the function.
*/

void Data_Is_Data(char *token,int *DC)
{
	*DC=*DC+1;			   
}/*Data_Is_Data*/

/*
A void function for updating the data counter in case of a struct's first operand in a directive sentence which will always be a num.
@recieved-
token-the num.
DC-the data counter pointer to be able to change it directly in the function.
*/

void Data_Is_Struct_First_Op(char *token,int *DC)
{
	if(token!=NULL)
		*DC=*DC+1;		
}/*Data_Is_Struct_First_Op*/

/*
A void function for updating the data counter in case of a struct's second operand in a directive sentence which will always be a string.
@recieved-
token-the string.
DC-the data counter pointer to be able to change it directly in the function.
*/

void Data_Is_Struct_Second_Op(char *token,int *DC)
{
	if(token!=NULL)
		*DC=*DC+strlen(token)+1;
}/*Data_Is_Struct_Second_Op*/

/*
A void function for updating the data counter in case of a string in a directive sentence.
@recieved-
token-the string.
DC-the data counter pointer to be able to change it directly in the function.
*/

void Data_Is_String(char *token,int *DC)
{ 
	if(token!=NULL)
		*DC=*DC+strlen(token)+1;	
}/*Data_Is_String*/

/*
A function that breaks a string from data instruction to chars , gets the ascii value of each char and then activate
 nesting functions for getting the binary representation of each char and then activate a function to save the total string words. 

@recieves-
token-The token that represents the number as string.
DC-The current avaliable adress in the data list.
headD-The pointer to the current object in the data list.
word_length-The number of bits for displaying the number.
@returns-
headD-The pointer to the current object in the data list.
*/

struct TheDataBin *Code_Of_String(char *token,int DC,struct TheDataBin* headD,int word_length)
{
	int i=0;
	int str_as_int ;
	char str[strlen(token)+1];
	const char *Word;
	
	strcpy(str,token);
	for(i=0;i<strlen(str);i++)/*run over the entire string*/
	{
		str_as_int=(int) str[i];/*cast char type to int*/
		Word=Chars_To_Bin(str_as_int,word_length);/*activate function to get the binary representation of char*/
		Data_Word_Insert(&headD,DC,Word);/*insert the char's word into the data list*/
		DC++;/*go to the next avaliable address*/
	}/*for*/
	Word=Chars_To_Bin(0,word_length);/*activate function to get the binary representation of an end of string*/
	Data_Word_Insert(&headD,DC,Word);/*insert the char's word into the data list*/
	return headD;		
}/*Code_Of_String*/

/*
A function that converts the string that represents the number contained in the command operand to a int type, 
using the "atoi" function to later be able to change the number from decimal to binary.
The function also checks if the number is negative or positive in order to instruct the following function how to encode the number.
The function then activates nesting functions for getting the binary representation of the number.
@recieves-
token-The token that represents the number as string.
word_length-The number of bits for displaying the number.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/

const char *Code_Of_Num(char *token,int word_length)
{
	int num;
	int neg_num_flag=0;
	char num_str[strlen(token)+1];
	char num_str_pos[strlen(num_str)-1];
	char num_str_neg[strlen(num_str)-1];
	const char *Word;
	
	strcpy(num_str,token);
	if(num_str[0]==NEG)/*means the number is negative*/
	{
		neg_num_flag=1;
		substring(num_str_neg,num_str,1,(strlen(num_str_neg)-1));/*save the number as unsigned*/
		num=atoi(num_str_neg);/*change string to int*/
	}/*if*/
	else if(num_str[0]==POS)
	{
		substring(num_str_pos,num_str,1,(strlen(num_str_pos)-1));/*save the number as unsigned*/
		num=atoi(num_str_pos);/*change string to int*/
	}/*else if*/
	else
	{
		num=atoi(num_str);/*change string to int*/
	}/*else*/
	
	Word=Num_To_Bin(num,neg_num_flag,word_length);/*activates a function that sets the binary representation of the number*/
	return Word;
}/*Code_Of_Num*/

/*
An auxiliary function that returns the substring of the source string starting at the position specified in the third
 argument and the length specified in the fourth argument of the function.
*/
	
char* substring(char *destination, const char *source, int beg, int n)
{
    strncpy(destination, (source + beg), n);
    return destination;
}/*substring*/

/*
A function that recieves an ascii number of char from a string and converts it to binary code .
The function recieves also the word length which represents the instruction type ,which in this case will be data,
 so the numbers are displayed as 10 bits .
@recieves-
n-The number to convert.
word_length-The number of bits for displaying the number.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/

const char *Chars_To_Bin(int n,int word_length)
{
 	int bin_num_arr[word_length];
 	int i;
 	const char *Word;
 	
	for(i=0;i<word_length;i++)    
	{    
		bin_num_arr[i]=n%2;  
		if(n==0)
			bin_num_arr[i]=0;  
		else  
			n=n/2;  
	}/*for*/       
   	Word=Bin_To_String(bin_num_arr,word_length);/*activate function for converting the binary array to string*/ 
   	return Word;
}/*Chars_To_Bin*/

/*
A function that recieves a number and converts it to binary code .
The function recieves also the word length which represents the instruction type ,
in case of data, the numbers are displayed as 10 bits , in case of code they are displayed as 8 bit with
2 last bits for encoding type representation.
The function converts the negative numbers using the 2's complement method.
@recieves-
n-The number to convert.
neg_num_flag-A flag that tells if the number is negative or not.
word_length-The number of bits for displaying the number.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/

const char *Num_To_Bin(int n,int neg_num_flag,int word_length)
{
 	int bin_num_arr[word_length];
 	int i;
 	const char *Word;
 	
	for(i=0;i<word_length;i++)    
	{    
		bin_num_arr[i]=n%2;  
		if(n==0)
			bin_num_arr[i]=0;  
		else  
		n=n/2;  
	}/*for*/   
	if(neg_num_flag==1)
	{
		i=0;
		while(i<word_length)
		{
			if(bin_num_arr[i]==0)
				i++;
			else
			{		
				for(++i;i<word_length;i++)
				{
					if(bin_num_arr[i]==0)
						bin_num_arr[i]=1;
					else
						bin_num_arr[i]=0;
				}/*for*/
			}/*else*/
		}/*while*/
	
	}/*if*/
   	Word=Bin_To_String(bin_num_arr,word_length);/*activate function for converting the binary array to string*/ 
   	return Word;  
}/*Num_To_Bin*/

/*
A function that recieves the word represented as an array of binary numbers and converts it to string.
@recieves-
bin_num_arr-The array of binary numbers.
word_length-The number of bits for displaying the number
@returns-
arrChar-a mutable pointer to an immutable string of the word display .
*/

const char *Bin_To_String(int bin_num_arr[],int word_length)
{
	int i;
	int j=0;
	char *arrChar;
	arrChar = (char *) malloc (sizeof(char) * (WORD_LEN_STR));

	for(i=word_length-1;i>=0;i--) /*Converting the bit order to the correct order as defined when converting to string*/
	{
		arrChar[j] = bin_num_arr[i] + '0';
		j++;
	}/*for*/
	if(word_length==CODE_BITS)/*in case the word is a type of code , the last numbers saved for the encoding type that is Absolute because we are only coding numbers at this passage*/
	{
		arrChar[j] =A_R_E[0][0];
		arrChar[++j]=A_R_E[0][1];
		arrChar[++j] = '\0';
	}/*if*/
	return arrChar;
}/*Bin_To_String*/

/*
A function that recieves a label that is defined in the currrent line as extern ,
 the function checks if the label is not defined as entry and if it is not already defined as label , 
 and only then inserts the label to the labels list with a sign that the label is extern (EX_ENCODE).
@recieves-
token-the directive.(extern or entry)
head-a pointer to the label list.
headI-a pointer to the entry labels list.
@returns-
a new pointer to the updated labels list.
*/

struct Label *exter(char *token,struct Label *head,struct Internal *headI)
{
	char *t;
	char label[MAX_CHARS];
	
	if(strcmp(token,ex_en[0])==0)
	{
		t=strtok(NULL," \t\n");
		if(t!=NULL)
		{
		strcpy(label,t);
		if(Search_Internal(headI,label)==1&&Search_Label(head,label)=='1')
			Label_Insert(&head,0,label,EX_ENCODE);
		}/*if*/
	}/*if*/
	return head;
}/*exter*/

/*
A function that recieves a label that is defined in the currrent line as entry , 
the function checks if the label is not defined as external and if it is not already defined as entry ,
 and only then inserts the label to the entry labels list .
@recieves-
token-the directive.(extern or entry)
head-a pointer to the label list.
headI-a pointer to the entry labels list.
@returns-
a new pointer to the updated Internal list.
*/

struct Internal *entry(char *token,struct Label *head,struct Internal *headI)
{
	char *t;
	char label[MAX_CHARS];
	if(strcmp(token,ex_en[1])==0)
	{
		t=strtok(NULL," \t\n");
		if(t!=NULL)
		{
		strcpy(label,t);
		if(Search_Label(head,label)!=EX_ENCODE&&Search_Internal(headI,label)==1)
			Insert_Internal(&headI,label);
		}/*if*/
	}/*if*/
	return headI;
}/*entry*/

/*
A void function for freeing the memory allocated to the labels list.
@recieves-
head-The pointer to the labels list.
*/

void Free_Label_List(struct Label* head)
{
   struct Label* temp;
   
   if(head!=NULL)
   {
   	while (head != NULL)
   	{
      	 temp = head;
      	 head = head->next;
      	 free(temp);
   	}/*while*/
   }/*if*/
}/*Free_Label_List*/

/*
A void function for freeing the memory allocated to the Internal labels list.
@recieves-
head-The pointer to the entry labels list.
*/

void Free_Internal_List(struct Internal* head)
{
   struct Internal* temp;
   
   if(head!=NULL)
   {
   	while (head != NULL)
   	{
      	 temp = head;
      	 head = head->next;
      	 free(temp);
   	}/*while*/
   }/*if*/
}/*Free_Internal_List*/

