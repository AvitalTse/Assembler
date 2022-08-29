#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "AssemblerSecond.h"
#include "ConstVal.h"
#include "ConstTab.h"
#include "PreAssembler.h"

 
 
/********************************************************************************************************************************************/
/***************************************************** Welcome to The Assembler Program *****************************************************/
/********************************************************************************************************************************************/

/*
The Assembler program goes through the program and interprets all the macros present in it.
The program receives the number of files the user entered and their names one by one,
the main takes the name of the file and creates an output file using the same name as the input file just different ending
 ".am",then checks using a function if the current file opened successfully for reading and if the output file opened successfully for writing.
The program then activates a function for interperting all the macros present in the input file,
after that closes the files and continues until the last file that was insert in the command line by the user,
A function is then invoked to handle the first pass of the assembler using the name of the file.
@recieves-
argc-stands for argument count.
argv[]-an array of strings (character pointers) representing the individual arguments provided on the command line.

*/

int main(int argc,char *argv[])
{
	FILE *file;
	FILE *out_file;
	int i;
	char out_name[MAX_CHARS];
	
	if(argc>=1)/*checks if there are arguments(files name) in the command line*/
	{
		for(i=1;i<argc;i++)/*runs over all the files that are in the argument line*/
		{
			file=open_file(argv[i],"r");/*activates the function for checking if the file succeeds in opening with the appropriate operand*/
			if(file!=NULL)
			{
				printf("\nThe File %s was opened successfully\n\n",argv[i]);
				/*creates the output file name using the name of the input file*/
				strcpy(out_name,argv[i]);
				strtok(out_name,".");
				strcat(out_name,".am");
				out_file=open_file(out_name,"w");/*activates the function for checking if the output file succeeds in opening with the appropriate operand*/
				if(file!=NULL)
				{
					/*function in PreAssembler.c Line 22*/
					Macro_Spread(file,out_file);/*activates the function for spreading the macros and create the new outpit file*/
					fclose(file);
					fclose(out_file);
					AssemblerFirst(out_name);
				}/*if*/
			}/*if*/
			
		}/*for*/
	}/*if*/
	return 0;
}/*main*/


/*The Assembler First void function takes the original file after the macro spreading and goes over line by line 
of the file and first of all checks if the line is legal , not empty or note or exceeds the maximum, than the function analyzes each line,
 if it is a label, enters the label to the list of symbols with the appropriate counter (DC for data , IC for code).
if it is data directive inserts the binary word to the data words list using the DC for address,
 if it is code directive and not contains a label than it inserts the binary word of the code instruction to the code words list using IC for address. 
@recieves-
out_name-name of the original file.
*/


void AssemblerFirst(char out_name[])
{
	FILE *file;
	int r;
	int IC=0;
	int DC=0;
	int line_row=1;
	int index=0;
	int code_or_data;
	char line[CHECK_LEN];
	char save_line1[CHECK_LEN];
	char save_line[MAX_ROW_LENGTH];
	char *token=NULL;
	char label[MAX_CHARS];
	char op1[MAX_CHARS];
	char op2[MAX_CHARS];
	const char *Word;
	bool flag_error;
	struct Label* head = NULL; 
	struct Internal* headI = NULL; 
	struct TheDataBin* headD=NULL;
	struct TheCodeBin* headC=NULL;

	file=open_file(out_name,"r");/*function in SharedFunctions.c Line 162*/
	if(file!=NULL)
	{
		while(fgets(line,CHECK_LEN,file)!=NULL)
		{
			strcpy(save_line1,line);
			/*function in SharedFunctions.c Line 218*//*function in SharedFunctions.c Line 242*//*function in ErrorsMain.c Line 27*/
			if (!Check_Empty_Line(save_line1)&&!Check_Line_Length(save_line1)&&!Error_First(save_line1,line_row,head))
			{
				strcpy(save_line,line);
				token=(strtok(line,":"));
				strcpy(label,token);
				/*function in SharedFunctions.c Line 183*//*function in SharedFunctions.c Line 282*/
				if(Label_Flag(save_line,token)&&Search_Label(head,label)=='1')
				{	
					token=(strtok(NULL," \n\t"));
					if(token!=NULL)
					{
						code_or_data=Check_Sentence(token);/*function in SharedFunctions.c Line 26*/
						if(code_or_data==1)/*data*/
							Label_Insert(&head,DC,label,NO_ENCODE);/*function in AssemblerFirst.c Line 407*/
						else if(code_or_data==0)/*code*/
							Label_Insert(&head,IC,label,REL_ENCODE);/*function in AssemblerFirst.c Line 407*/
					}/*if*/
				}/*if*/
				else/*no flag definition in line*/
				{
					token=(strtok(line," \n\t"));/*points to the where should be directive in line*/
					if(token!=NULL)/*==NULL - illegal line*/
					{
						code_or_data=Check_Sentence(token);/*function in SharedFunctions.c Line 26*/
						if(code_or_data==2)/*extern or entry*/
						{
							headI=entry(token,head,headI);/*function in AssemblerFirst.c Line 759*/
							head=exter(token,head,headI);/*function in AssemblerFirst.c Line 729*/
						}/*if*/
					}/*if*/
				}/*else*/
				if(code_or_data==1)/*data*/
				{
					index=Find_Data_Index(token);/*function in SharedFunctions.c Line 258*/
					if(index==0)/*string*/
					{
						token=strtok(NULL,"\"");
						if(token!=NULL)
						{
							strcpy(op1,token);
							headD=Code_Of_String(token,DC,headD,DATA_BITS);/*function in AssemblerFirst.c Line 533*/
							DC=DC_Update(op1,op2,DC,index);/*function in AssemblerFirst.c Line 446*/
						}/*if*/
					}/*if*/
					else if(index==1)/*struct*/
					{
						token=strtok(NULL,", ");
						if(token!=NULL)
						{
							Word=Code_Of_Num(token,DATA_BITS);/*function in AssemblerFirst.c Line 565*/
							Data_Word_Insert(&headD,DC,Word);/*function in AssemblerFirst.c Line 340*/
							DC++;
							strcpy(op1,token);
							token=strtok(NULL," \"");
							if(token!=NULL)
							{
								headD=Code_Of_String(token,DC,headD,DATA_BITS);/*function in AssemblerFirst.c Line 533*/
								strcpy(op2,token);
								DC=DC_Update(op1,op2,DC,index);/*function in AssemblerFirst.c Line 446*/
							}/*if*/
						}/*if*/
					}/*else if*/
					else if(index==2)/*data*/
					{
						token=strtok(NULL,",");
						if(token!=NULL)
							strcpy(op1,token);
						while(token!=NULL)
						{
							Word=Code_Of_Num(token,DATA_BITS);/*function in AssemblerFirst.c Line 565*/
							Data_Word_Insert(&headD,DC,Word);/*function in AssemblerFirst.c Line 340*/
							DC=DC_Update(op1,op2,DC,index);/*function in AssemblerFirst.c Line 446*/
							token=strtok(NULL,",");
						}/*while*/
					}/*else if*/
				}/*if*/
				else if(code_or_data==0)/*code*/
				{
					index=Find_Code_Index(token);/*function in SharedFunctions.c Line 197*/
					if(index==0||index==1||index==2||index==3||index==6)/*2 operands instructions*/
					{
						token=strtok(NULL,",\n\t ");
						if(token!=NULL)
						{
							strcpy(op1,token);/*first perand*/
							token=strtok(NULL,",\n\t ");
							if(token!=NULL)
							{
								strcpy(op2,token);/*second operand*/
								Word=Case_Of_Code(index,op1,op2);/*function in AssemblerFirst.c Line 212*/
								Code_Word_Insert(&headC,IC,Word);/*function in AssemblerFirst.c Line 355*/
								r=Check_For_Registers(op1,op2);/*function in AssemblerFirst.c Line 178*/
								if(r==2)/*both operands are registers*/
								{
									Word=Check_Code_Op(op1,op2,&IC,CODE_BITS);/*function in AssemblerFirst.c Line 65*/
									Code_Word_Insert(&headC,IC,Word);/*function in AssemblerFirst.c Line 373*/
								}/*if*/
								else if(r==-1)/*the second operand is register*/
								{
									Word=Check_Code_Op(op1,op2,&IC,CODE_BITS);/*function in AssemblerFirst.c Line 65*/
									if(Word!=NULL)
										Code_Word_Insert(&headC,IC,Word);/*function in AssemblerFirst.c Line 373*/
									IC++;
									Word=Code_Of_Registers(op1,op2);/*function in AssemblerFirst.c Line 117*/
									if(Word!=NULL)
										Code_Word_Insert(&headC,IC,Word);/*function in AssemblerFirst.c Line 373*/
								}/*else if*/
								else
								{
									Word=Check_Code_Op(op1,op2,&IC,CODE_BITS);/*function in AssemblerFirst.c Line 65*/
									if(Word!=NULL)
										Code_Word_Insert(&headC,IC,Word);/*function in AssemblerFirst.c Line 373*/					
									Word=Check_Code_Op(op2,op1,&IC,CODE_BITS);/*function in AssemblerFirst.c Line 65*/
									if(Word!=NULL)
										Code_Word_Insert(&headC,IC,Word);/*function in AssemblerFirst.c Line 373*/
								}/*else*/
							}/*if*/
						}/*if*/
					}/*if*/
					else if(index==14||index==15)/*non operand instructions*/
					{
						Word=Case_Of_Code(index,op1,op2);/*function in AssemblerFirst.c Line 212*/
						Code_Word_Insert(&headC,IC,Word);/*function in AssemblerFirst.c Line 373*/
					}/*else if*/
					else/*one operand instructions*/
					{
						token=strtok(NULL,", \n\t");
						if(token!=NULL)
						{
							strcpy(op1,token);
							Word=Case_Of_Code(index,op1,op2);/*function in AssemblerFirst.c Line 212*/
							Code_Word_Insert(&headC,IC,Word);/*function in AssemblerFirst.c Line 373*/
							Word=Check_Code_Op(op1,op2,&IC,CODE_BITS);/*function in AssemblerFirst.c Line 65*/
							if(Word!=NULL)
								Code_Word_Insert(&headC,IC,Word);/*function in AssemblerFirst.c Line 373*/
						}/*if*/	
					}/*else*/
					IC++;	
				}/*else if*/
			}/*if*/
			line_row++;
		}/*while*/
		Update_Label_Table(head,IC+MEM_INT);/*function in AssemblerFirst.c Line 319*/
		fseek(file, 0, SEEK_SET);/*back to the start of file*/
		fclose(file);
		flag_error=AssemblerSecond(out_name,head,headC,headD);/*activate the second pass*/
		if(flag_error==false)
			Create_Entry_File(out_name,headI,head);/*function in AssemblerFirst.c Line 26*/
		Free_Label_List(head);/*function in AssemblerFirst.c Line 782*/
		Free_Internal_List(headI);/*function in AssemblerFirst.c Line 803*/
		free((char*)Word);
	}/*if*/
}/*AssemblerFirst*/


/*The Assembler Second function takes the original file after the macro spreading and goes over line by 
line of the file and first of all checks if the line is legal , not empty or note or exceeds the maximum,
than the function analyzes each line,the function in this pass takes all the operands that have not beeing
 insert to the code table and using the labels table the function convert each label operand or struct that uses 
 a label to a binary word and inserts it into the code table in the suitable place. 
@recieves-
file_name-name of the original file.
node-pointer to the symbols table.
headC-pointer to the code words table.
headD-pointer to the data words table.
@returns-
true-if there are no errors in the file which means it can create outgoing files.
false - if there are errors in file that have not found in the first pass.
*/

bool AssemblerSecond(char file_name[],struct Label* node,struct TheCodeBin* headC,struct TheDataBin *headD)
{	
	FILE *file;
	FILE *ob;
	int IC=0;
	int DC=0;
	int index;
	int line_row=1;
	int code_or_data; 
	int op1_l;
	int op2_l;
	int ex_flag=0;
	int cnt_errors=0;
	char line[CHECK_LEN];
	char save_line1[CHECK_LEN];
	char save_line[MAX_ROW_LENGTH];
	char *op1=NULL;
	char *op2=NULL;
	char *token;
	const char *Word;
	bool flag_error;
	struct External *headE=NULL;
	
	file=open_file(file_name,"r");/*function in SharedFunctions.c Line 162*/
	if(file!=NULL)
	{
		while(fgets(line,CHECK_LEN,file)!=NULL)
		{
			strcpy(save_line1,line);
			flag_error=Error_Flag(save_line1,line_row,node,&cnt_errors);/*function in AssemblerSecond.c Line 28 */
			if (flag_error==false)
			{
				strcpy(save_line,line);
				token=(strtok(line,":"));
				if(Label_Flag(save_line,token))/*function in SharedFunctions.c Line 183*/
				{
					token=strtok(NULL," \n\t");
					if(token!=NULL)
						code_or_data=Check_Sentence(token);/*function in SharedFunctions.c Line 26*/
				}/*if*/
				else
				{
					token=strtok(line," \n\t");
					if(token!=NULL)
						code_or_data=Check_Sentence(token);/*function in SharedFunctions.c Line 26*/
				}/*else*/
				if(code_or_data==0)/*code*/
				{
					index=Find_Code_Index(token);/*function in SharedFunctions.c Line 197*/
					IC++;
					if(index==0||index==1||index==2||index==3||index==6)
					{
						op1=strtok(NULL,",\n\t ");
						op2=strtok(NULL,", \n\t");
						if(op1!=NULL&&op2!=NULL)
						{
							if(Check_For_Registers_Second(op1,op2)==2)/*function in AssemblerSecond.c Line 64*/
								IC++;
							else
							{
								ex_flag=0;
								op1_l=Check_Code_Op_Second(op1,&IC);/*function in AssemblerSecond.c Line 346*/
								if(op1_l==1||op1_l==2)
								{
									Word=Search_Label_Second(node,op1,&ex_flag);/*function in AssemblerSecond.c Line 389*/
									Code_Label_Word_Insert(&headC,IC,Word);/*function in AssemblerSecond.c Line 302*/
									if(ex_flag==1)
										Insert_External(&headE,op1,IC);/*function in AssemblerSecond.c Line 492*/
									IC+=op1_l;
								}/*if*/
								ex_flag=0;
								op2_l=Check_Code_Op_Second(op2,&IC);/*function in AssemblerSecond.c Line 346*/
								if(op2_l==1||op2_l==2)
								{
									Word=Search_Label_Second(node,op2,&ex_flag);/*function in AssemblerSecond.c Line 389*/
									Code_Label_Word_Insert(&headC,IC,Word);/*function in AssemblerSecond.c Line 302*/
									if(ex_flag==1)
										Insert_External(&headE,op2,IC);/*function in AssemblerSecond.c Line 492*/
									IC+=op2_l;
								}/*if*/
							  }/*else*/
					     	  }/*if*/
					}/*if*/
					else if(index==4||index==5||index==7||index==8||index==9||index==10||index==11||index==12||index==13)
					{
						token=strtok(NULL,",\n\t ");
						if(token!=NULL)
						{
							strcpy(op1,token);
							ex_flag=0;
							op1_l=Check_Code_Op_Second(op1,&IC);/*function in AssemblerSecond.c Line 346*/
							if(op1_l==1||op1_l==2)
							{
								Word=Search_Label_Second(node,op1,&ex_flag);/*function in AssemblerSecond.c Line 389*/
								Code_Label_Word_Insert(&headC,IC,Word);/*function in AssemblerSecond.c Line 302*/
								if(ex_flag==1)
									Insert_External(&headE,op1,IC);/*function in AssemblerSecond.c Line 492*/
								IC+=op1_l;
							}/*if*/
						}/*if*/
					}/*else if*/				
				}/*if*/
				else if(code_or_data==2)
				{			
					if(strcmp(token,ex_en[1])==0)	
					{
						token=strtok(NULL," \t\n");
						if(token!=NULL)
							Search_Label_Entry(node,token);/*function in AssemblerSecond.c Line 425*/
					}/*if*/
				}/*else if*/
			}/*if*/
			line_row++;
		}/*while*/
		if(cnt_errors==0)
		{
			DC=Up_Date_Address(IC,headC,headD);/*function in AssemblerSecond.c Line 90*/
			ob=Create_Ob_File(file_name);/*function in AssemblerSecond.c Line 106*/
			if(ob!=NULL)
			{
				Add_To_CPU_Lang_Code(ob,IC);/*function in SharedFunctions.c Line 310*/
				Add_To_CPU_Lang_Code(ob,DC);/*function in SharedFunctions.c Line 310*/
				fputs("\n",ob);
				Word_To_CPU_Lang_Code(headC,ob);/*function in AssemblerSecond.c Line 160*/
				Word_To_CPU_Lang_Data(headD,ob);/*function in AssemblerSecond.c Line 214*/
				fclose(ob);
			}/*if*/
			Create_Extern_File(file_name,headE);/*function in AssemblerSecond.c Line 128*/
		}/*if*/
		fclose(file);
		Free_Code_List(headC);/*function in AssemblerSecond.c Line 639*/
		Free_Data_List(headD);/*function in AssemblerSecond.c Line 660*/
		Free_External_List(headE);/*function in AssemblerSecond.c Line 681*/
		free((char*)Word);
	}/*if*/
	if(cnt_errors==0)
		return false;
	return true;
}/*AssemblerSecond*/

