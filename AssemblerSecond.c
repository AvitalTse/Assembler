#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "ConstVal.h"
#include "ConstTab.h"
#include "AssemblerSecond.h"


/*********************************************************************************************************************************************/
/**************************************** A file that contains all the functions used for second pass ****************************************/
/*********************************************************************************************************************************************/

/*
A function that checks for any error in the line , if the line is empty or excceeds the maximum , and only if line is 
legal activates a function that searches for any error in the line.
@recieves-
save_line-the current line.
line_row-number of the line.
node-pointer to the Labels list.
cnt_erros-pointer to a counter with an abillity to change its value in case there is an empty or long line.
@returns-
true-if there are any errors in line.
false-if there are no errors in line.
*/

bool Error_Flag(char save_line[],int line_row,struct Label *node,int *cnt_errors)
{
	char line[CHECK_LEN];
	bool flag_error=false;
	bool flag_empty;
	bool flag_length;
	
	flag_empty=Check_Empty_Line(save_line);/*function in SharedFunctions.c Line 212*/
	if(flag_empty==true)
		printf("Warning:Line %d is an Empty or a note line\n",line_row);
	flag_length=Check_Line_Length(save_line);/*function in SharedFunctions.c Line 238*/
	if(flag_length==true)
	{
		printf("Error in Line %d,Illegal line length,maximum is 80 chars.",line_row);
		*cnt_errors=*cnt_errors+1;
	}/*if*/
	strcpy(line,save_line);
	if(flag_empty==false&&flag_length==false)
		flag_error=Error_Second(line,line_row,node);/*function in ErrorsMain.c Line 123*/
	if(flag_error==true)
		*cnt_errors=*cnt_errors+1;
	if (flag_error==false&&flag_empty==false&&flag_length==false)
		return flag_error;
		
	return true;
}/*Error_Flag*/

/*
A function that recieves all the operand of the current code instruction and checks if they are registers , and returns the amount of registers.
@recieves-
op1-the first operand.
op2-the second operand.
@returns-
r-counter of the registers.
*/

int Check_For_Registers_Second(char op1[],char op2[])
{
	int i;
	int r=0;
	
	for(i=0;i<MAX_REGIS;i++)
	{
		if(strcmp(op1,regi[i])==0)
			r++;
		if(strcmp(op2,regi[i])==0)
			r++;
	}/*for*/
return r;
}/*Check_For_Registers_Second*/

/*
A function that activates functions that update the addresses of the code and data words, and recieves from the data update
 function the data counter and sends it as return value.
@recieve-
IC-instruction counter.
headC-pointer to the code words list.
headD-pointer to the data words list.
@returns-
DC-data counter.
*/

int Up_Date_Address(int IC,struct TheCodeBin *headC,struct TheDataBin *headD)
{
	int cnt;
	
	Update_Code_Address(headC);
	cnt=Update_Data_Address(headD,(IC+MEM_INT));
	return cnt;
}/*Up_Date_Address*/

/*
A function that creates a new file using the current file name. the function creates the ob file.
@recieves-
file_name-the name of the current file.
@returns-a pointer the opened ob file.
*/

FILE *Create_Ob_File(char file_name[])
{	
	FILE *ob;
	char ob_file[MAX_CHARS];
	
	strcpy(ob_file,file_name);
	strtok(ob_file,".");
	strcat(ob_file,".ob");
	ob=open_file(ob_file,"w");
	return ob;
}/*Create_Ob_File*/

/*
A void function that recieves the current file name and creates a file of the same name just for the extern labels
 that are in the file, the function takes the extern labels from the External list and their addresses ,
 converts the address to the language of the CPU,then inserts to the extern file and when reaches the end of the list closes the file.
@recieves-
out_name-name of the current file.
headI-pointer to the Internal list.
headL-pointer to the Labels list.
*/

void Create_Extern_File(char out_name[],struct External *headE)
{
	FILE *file;
	struct External *head=headE;
	
	strtok(out_name,".");
	strcat(out_name,".ex");
	if(head!=NULL)
	{
		file=open_file(out_name,"w");
		if(file!=NULL)
		{
			while(head!=NULL)
			{
				fprintf(file,"%s\t",head->name);
				Add_To_CPU_Lang_Code(file,head->address);
				fprintf(file,"\n");
				head=head->next;
			}/*while*/
			fclose(file);
		}/*if*/
	}/*if*/
}/*Create_Extern_File*/

/*
A void function that recieves the ob file and the code words list , and converts each word and address of 
the same word to the CPU language , then insert the new converted word and address to the ob file.
@recieves-
head-pointer to the code words list.
file-the ob file.
*/

void Word_To_CPU_Lang_Code(struct TheCodeBin * head,FILE *file)
{	
	int i;
	int sign_num=0;
	int cpu_bits=CPU_BITS;
	int wordi=0;
	char word;
	const char *Word;
	struct TheCodeBin* current = head; 
	 
	while (current != NULL)
	{
		Word=Address_To_CPU_Lang_Code(current);/*function to convert the address of the word and inserts it to the ob file.*/
		for(i=0;i<WORD_LEN_BITS;i++)
		{
			cpu_bits--;
			word=Word[i];
			wordi=atoi(&word);
			sign_num+=((wordi)*pow(2,cpu_bits));
			if(cpu_bits==0)
			{
				cpu_bits=CPU_BITS;
				fprintf(file,"%c",CPU_LANG[sign_num]);
				sign_num=0;
			}/*if*/
		}/*for*/
		fprintf(file,"\t");
		for(i=0;i<WORD_LEN_BITS;i++)
		{
			cpu_bits--;
			word=current->word[i];
			wordi=atoi(&word);
			sign_num+=((wordi)*pow(2,cpu_bits));
			if(cpu_bits==0)
			{
				cpu_bits=CPU_BITS;
				fprintf(file,"%c",CPU_LANG[sign_num]);
				sign_num=0;
			}/*if*/
			
		}/*for*/
	fprintf(file,"\n");
	current=current->next;
	}/*while*/
}/*Word_To_CPU_Lang_Code*/

/*
A void function that recieves the ob file and the data words list , and converts each word and address of the 
same word to the CPU language , then insert the new converted word and address to the ob file, the data words inserted after the code words.
@recieves-
head-pointer to the data words list.
file-the ob file.
*/

void Word_To_CPU_Lang_Data(struct TheDataBin * head,FILE *file)
{	
	int i;
	int sign_num=0;
	int cpu_bits=CPU_BITS;
	int wordi=0;
	char word;
	const char *Word;
	struct TheDataBin* current = head;  
	
	while (current != NULL)
	{
		Word=Address_To_CPU_Lang_Data(current);/*function to convert the address of the word and inserts it to the ob file.*/
		for(i=0;i<WORD_LEN_BITS;i++)
		{
			cpu_bits--;
			word=Word[i];
			wordi=atoi(&word);
			sign_num+=((wordi)*pow(2,cpu_bits));
			if(cpu_bits==0)
			{
				cpu_bits=CPU_BITS;
				fprintf(file,"%c",CPU_LANG[sign_num]);
				sign_num=0;
			}/*if*/	
		}/*for*/
		fprintf(file,"\t");
		for(i=0;i<WORD_LEN_BITS;i++)
		{
			cpu_bits--;
			word=current->word[i];
			wordi=atoi(&word);
			sign_num+=((wordi)*pow(2,cpu_bits));
			if(cpu_bits==0)
			{
				cpu_bits=CPU_BITS;
				fprintf(file,"%c",CPU_LANG[sign_num]);
				sign_num=0;
			}/*if*/
		}/*for*/
	fprintf(file,"\n");
	 current=current->next;
	}/*while*/
}/*Word_To_CPU_Lang_Data*/

/*
A function that recieves current code word and sends its address to a function that converts it to a binary word,
then returns the converted word .
@recieves-
current-pointer to a specific node of the code list.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/

const char *Address_To_CPU_Lang_Code(struct TheCodeBin *current)
 {
 	const char *Word;
	Word=Num_To_Bin_Label(current->address,WORD_LEN_BITS,NO_ENCODE);
	return Word;
}/*Address_To_CPU_Lang_Code*/

/*
A function that recieves current data word and sends its address to a function that converts it to a binary word, 
then returns the converted word .
@recieves-
current-pointer to a specific node of the data list.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/

const char *Address_To_CPU_Lang_Data(struct TheDataBin *current)
 {
 	const char *Word;
 	
	Word=Num_To_Bin_Label(current->address,WORD_LEN_BITS,NO_ENCODE);
	return Word;
}/*Address_To_CPU_Lang_Data*/

/*
A void function that saves the code's word and address into the appropriate place in terms of numbering the addresses
of the binary code list.
The function is activated only when there is a label opernd in code instruction,which means there is a new code word.
@recieves-
head-The current pointer to head of the the code words list.
address-The Instruction counter.
Word-a mutable pointer to an immutable string of the word display .
*/

void Code_Label_Word_Insert(struct TheCodeBin** head, int address,const char *Word)
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
      if(*head==NULL)/*if the list is empty*/
      {
            *head = newNode;/*current word will be first in the list*/
            return;
      }/*if*/
      temp= *head;
      while(temp->address<(address-1))/*move till the current before place of the list to insert the new word*/
            temp = temp->next;
      if(temp->next==NULL)
      {
      	newNode->next=NULL;
      	temp->next=newNode;
      }/*if*/
      else
      {
      	newNode->next=temp->next; /*insert the word to the to the appropriate place for the word in the list*/
      	temp->next=newNode;
      }/*else*/
}/*Code_Label_Word_Insert*/
				
/*
A function that checks the type of a operand of the code instruction and according the type, the function advances the instruction counter,
and also for label or struct that contains a label operand. 
@recieves-
*op-pointer to the current operand to check.
*IC-the instruction counter as pointer for the abillity to change it value.
@returns-
0-if no label or struct.
1-if label operand.
2-if struct opernad.
*/
		
int Check_Code_Op_Second(char *op,int *IC)
{
	int L;
	char op_temp[MAX_CHARS];
	
	strcpy(op_temp,op);
	strtok(op,".");
	if(strcmp(op,op_temp)!=0)/*struct type operand*/
	{
		return 2;	
	}/*if*/
	strcpy(op_temp,strtok(op,"# \n\t"));
	if(strcmp(op,op_temp)!=0)/*number type operand*/
	{
		L=1;/*one place in memory saved for a number*/
		*IC=*IC+L;/*update the instruction counter*/
		return 0;
	}/*if*/
	if(Check_For_Registers_Second(op,op)==0)/*label type operand*/
	{
		return 1;
	}/*if*/
	else/*register type operand*/
	{
		L=1;/*one place in memory saved for a register operand*/
		*IC=*IC+L;/*update the instruction counter*/
		return 0;
	}/*if*/
}/*Check_Code_Op_Second*/			



/*
A function that searches the label and activates a function for converting the current label address in case
it is external or relocatable , sends to the function the suitable encode type.
@recieves-
head-pointer to The head of the Labels list.
label-the current label name.
ex_flag-pointer to the flag that changes if the label is external.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/

char const *Search_Label_Second(struct Label* head, char label[],int *ex_flag)
{
    const char *Word=NULL;
    struct Label* current = head;  
    
    while (current != NULL)
    {
        if (strcmp(current->name,label)==0)
        {
        	if(current->are==EX_ENCODE)
        	{
        		*ex_flag=1;
        		Word=Num_To_Bin_Label(0,CODE_BITS,current->are);
        		return Word;
        	}/*if*/
        	else
        	{
        		current->are=REL_ENCODE;
        		Word=Num_To_Bin_Label(current->address,CODE_BITS,current->are);
        		return Word;
        	}/*else*/
        }/*if*/
        current = current->next; 
    }/*while*/
    return Word;
}/*Search_Label_Second*/

/*
A void function that updates the encode type of the labels in the labels list , in case the labe is not external , 
the label is Internal which means it is relocatable , so the labels that are with no encode type needs to change to 
relocatable for than be able to convert the labels to binary code currectly by their encode type.
@recieves-
head-pointer to the labels list.
label-the label name.
*/

void Search_Label_Entry(struct Label* head, char label[])
{
    struct Label* current = head;  
    
    while (current != NULL)
    {
        if (strcmp(current->name,label)==0)
        {
        	if(current->are!=EX_ENCODE)
        	current->are=REL_ENCODE;
        }/*if*/
        current = current->next; 
    }/*while*/
}/*Search_Label_Entry*/

/*
A void function that updates the address of each word in the code instructions list.since it was defined that one hundred 
addresses are occupied in memory, we will start the code instruction list in the 100 address, so we will add to each address 100 
until we reach the end of the list .
@recieves-
head-pointer to the code words list. 
*/

void Update_Code_Address(struct TheCodeBin* head)
{
	int IC=0;
	struct TheCodeBin* node=head;
	while(node!=NULL||IC==MIN_LINES)
	{
		node->address+=MEM_INT;
		node = node->next;
		IC++;
	}/*while*/
}/*Update_Code_Address*/

/*
A function that updates the address of each word in the data directive list.since it was defined that the data words will be
 after the code words, we will start the data directive list in the IC+100 address, so we will add to each address IC+100 until
  we reach the end of the list .
@recieves-
head-pointer to the data words list.
cnt-IC+100. 
@returns-
DC-directive counter.
*/
int Update_Data_Address(struct TheDataBin* head,int cnt)
{
	int DC=0;
	struct TheDataBin* node=head;
	while(node!=NULL||DC==MAX_MEM)
	{
		node->address+=cnt;
		node = node->next;
		DC++;
	}/*while*/
	return DC;
}/*Update_Data_Address*/

/*
A void function that saves the external label into the end of the entry labels list.
The function is activated only when there is a extern directive,which means there is a new extern label.
@recieves-
head-The current pointer to head of the the external list.
label-the extern label.
IC-the address of the Label.
*/

void Insert_External(struct External **head,char *label,int IC)
{
	struct External* newNode = (struct External*) malloc(sizeof(struct External));
	struct External* temp;
	
	if(newNode==NULL)
	{
		printf("out of memory space\n");
		return ;
	}/*if*/
	strcpy(newNode->name,label);
	newNode->address=IC+MEM_INT;
	newNode->next=NULL;
	if(*head==NULL)/*if the list is empty*/
	{
		*head = newNode;
		return;/*current extern will be first in the list*/
    	}/*if*/
	temp= *head;
      while(temp->next!=NULL)/*move till the end of the list to insert the new extern*/
		temp = temp->next;
      temp->next = newNode;/*insert the extern to the end of the list*/
}/*Insert_External*/

/*
A function that search the label in the labels list to check if the label is defined as an external.
@recieves-
label-the label name.
head-pointer to the labels list.
@returns-
1-if the label is defined as external.
0-if not.
*/

int Search_EX_Label(char label[],struct Label *head)
{
	struct Label* current = head; 
	
	while (current != NULL)
	{
		if (strcmp(current->name,label)==0)
		{
      	  	if(current->are==EX_ENCODE)
      	  		return 1;
        	}/*if*/
        	current = current->next;
	}/*while*/
return 0;
}/*Search_EX_Label*/

/*
A function that returns the address of a label from the label list using the label name.
@recieves-
head-pointer to the label list.
label-the current name of the label.
@returns-
The current label address.
-1-if label is not in the list.
*/

int Search_Label_Add(struct Label* head, char label[])
{
    struct Label* current = head;  
   
    while (current != NULL)
    {
        if (strcmp(current->name,label)==0)
        	return current->address;
        current = current->next; 
    }/*while*/
    return -1;
}/*Search_Label_Add*/

/*
A function that converts the address of the label to a binary number, since the address will always be an unsigned number ,
 this function responsible for converting positive numbers only.
@recieves-
n-the address(num)
word_length-the number of bits to save the number in.
are-encoding type.
@returns-
Word-a mutable pointer to an immutable string of the word display .
*/

const char *Num_To_Bin_Label(int n,int word_length,char are)
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
   	Word=Bin_To_String_Label(bin_num_arr,word_length,are);/*activate function for converting the binary array to string*/ 
   	return Word; 	 
}/*Num_To_Bin_Label*/	

/*
A function that recieves the word represented as an array of binary numbers and converts it to string.
if the label is defined as external the encoding type '10' will be added to the word.
if the label defined as internal , or defined in the code the encoding type '01' will be added to the word.
@recieves-
bin_num_arr-The array of binary numbers.
word_length-The number of bits for displaying the number.
are-the encoding type.
@returns-
arrChar-a mutable pointer to an immutable string of the word display .
*/

const char *Bin_To_String_Label(int bin_num_arr[],int word_length,char are)
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
	if(are==REL_ENCODE)/*in case the word is a type of internal label*/
	{
		arrChar[j] =A_R_E[2][0];
		arrChar[++j]=A_R_E[2][1];
		arrChar[++j] = '\0';
	}/*if*/
	else if(are==EX_ENCODE)/*in case the word is a type of external label*/
	{
		arrChar[j]=A_R_E[1][0];
		arrChar[++j]=A_R_E[1][1];
		arrChar[++j] = '\0';
	}/*else if*/	
	return arrChar;
}/*Bin_To_String_Label*/

/*
A void function for freeing the memory allocated to the code words list.
@recieves-
head-The pointer to the code words list.
*/

void Free_Code_List(struct TheCodeBin* head)
{
   struct TheCodeBin* temp;
   
   if(head!=NULL)
   {
   	while (head != NULL)
   	{
      	 temp = head;
      	 head = head->next;
      	 free(temp);
   	}/*while*/
   }/*if*/
}/*Free_Code_List*/

/*
A void function for freeing the memory allocated to the data words list.
@recieves-
head-The pointer to the data words list.
*/

void Free_Data_List(struct TheDataBin* head)
{
   struct TheDataBin* temp;
   
   if(head!=NULL)
   {
   	while (head != NULL)
   	{
   	    temp = head;
   	    head = head->next;
   	    free(temp);
   	}/*while*/
   }/*if*/
}/*Free_Data_List*/

/*
A void function for freeing the memory allocated to the External labels list.
@recieves-
head-The pointer to the External list.
*/

void Free_External_List(struct External* head)
{
   struct External* temp;
   
   if(head!=NULL)
   {
   	while(head != NULL)
   	{
   	    temp = head;
   	    head = head->next;
   	    free(temp);
   	}/*while*/
   }/*if*/
}/*Free_External_List*/


