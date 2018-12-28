#include<stdio.h>
#include<string.h>
#define MAX 50 //maximum rows and columns are 50
struct linked
{
	char link[4];
};
struct symbolTable
{
	short int status;//indicate present or not the char
	short int startRow;
	short int startCol;
	short int endRow;
	short int endCol;
	short int type;//what is the type of character
	short int length;//no of linked character
	struct linked connect;//var of linked structure
}symbol[38];//38 because 26 alpha(A-Z) 10 digits(0-9) 1 dash(-) and 1 astrisk(*)

//function declaration
int input(void);                //take input
void tableAnalysis(void);        //maintain the information about symbols
int findPosition(int,int);          //it is like hashing find the appropriate position of given symbol
void tableSymbolType(void);      //to find the type of symbol means is it madeup of single or multiple squares 
/*symbol type
1-single square symbol
2-horizontal left space symbol (multiple square synbol)
3-horizontal both left and right space symbol (multiple square synbol)
4-horizontal right space symbol (multiple square synbol)
5-vertical lower space symbol (multiple square synbol)
6-vertical both lower and upper space symbol (multiple square synbol)
7-vertical upper space symbol (multiple square symbol)*/
void tableSymbolLength(void);    //find length and connected char & assign in table
void process(void);              //for the purpose of processing
int validation(void);
void basicInitializationInitial(int,int);  //to assign the current(upper left) var 
void basicInitializationString();//new(from string)var
int findPositionString(char);     //to find position of character present in link or findString
int recursion(char,int);	//recursively call the function called under process fun()
char findActualString(int);     //it returns the character
void instruction(void);//show Instruction
//end of function declaration

//start of global variable declaration
char keyboard[MAX][MAX];     //keyboard size
int totalRows,totalColumns;  //totalrows and totalcolumns in keyboard
char findString[40];         //store string for which we have to count keystrokes
short int newstartRow,newstartCol,newendRow,newendCol,newsymbolType;
short int currentstartRow,currentstartCol,currentendRow,currentendCol,currentsymbolType;
short int stringPoint=0;
short int keystrokes=0;
char global[38];
char forPrint[38];
short int globalPoint=0;
short int level=38;
//end of global variable declaration

//start of main
int main()
{
	char check,valid='y';
	if(input())
	{
		tableAnalysis();//find starting and ending indexes
		tableSymbolType();//find type of char
		tableSymbolLength();//assign linked char
		do
		{
			if(validation())	
				process();
		
			printf("\nWant to continue (y/n) : ");
			scanf("\n");
			scanf("%c",&check);
			if(check=='n' || check=='N')
				break;
			do
			{
				if(check=='y' || check=='Y')
					break;
				else if(check=='n' || check=='N')
				{
					valid='n';	
					break;
				}
				printf("\nEntered wrong choice.");
				printf("\nWant to continue (y/n) : ");
				scanf("\n");
				scanf("%c",&check);
			}while(1);
			if(valid=='n')
				break;
			printf("\nEnter \"Message\" to be typed : ");
			scanf("%s",findString);
			findString[strlen(findString)+1]='\0';//first time well exe but second time alreday existed findstring so attach \0 to +1 cause before that * is attached
			keystrokes=0;
			stringPoint=0;
			globalPoint=0;
			level=38;					
			valid='y';
		}while(1);
	}
	return 0;
}
//end of main

//start of input()
int input()
{
	int i,j,asterisk=0;//temporary var 	
	printf("---------------------------------------------------------------------------------------------------------\n\t\t\t\t\t      KEYBOARDING\n---------------------------------------------------------------------------------------------------------\n");	
	instruction();
		
	printf("\nEnter Total Rows and Columns In Keyboard  : ");
	scanf("%d%d",&totalRows,&totalColumns);
	
	if((totalRows<=1 || totalColumns<=1)||(totalRows>50 || totalColumns>50))
	{
		printf("Total Rows and Total Columns must be between 2 and 50.");
		return(0);
	}
		

	printf("\nEnter Keyboard as per the given structure : \n");
	scanf("\n");
	for(i=0;i<totalRows;i++)
	{
		for(j=0;j<totalColumns;j++)
		{
						
			scanf("%c ",&keyboard[i][j]);
			if((keyboard[i][j]<65 || keyboard[i][j]>90)&&(keyboard[i][j]<48 || keyboard[i][j]>57)&&(keyboard[i][j]!=45)&&(keyboard[i][j]!=42))
			{
				printf("\nCharacter in Keyboard must be A-Z(Capital) or 0-9(Digit) or -(dash) or *(asterisk)\n\n");
				return 0;
			}
			if(keyboard[i][j]==42)
				asterisk=1;
		}	
		if(i==totalRows-2)//bcoz the problem flushing
			printf("\nEnter \"Message\" to be typed : ");			
	}	
	if(!asterisk)
	{
		printf("\nAsterisk (*) must present in Keyboard because it represent ENTER key.\n\n");
		return (0);
	}
	//scanf("%s",findString);	
	//printf("Enter string :");
	scanf("%s",findString);
		
	return(1);
}
//end of input();
//start of instruction()
void instruction(void)
{
	printf("\n----------------------------------------------Instruction------------------------------------------------\n");
	printf("1.Valid Character Allowed in Virtual Keyboard are : \n\t:A-Z(Capital)\n\t:0-9(Digits)\n\t:-(Dash)\n\t:*(asterisk)(it is used to indicate end of the text)(known as ENTER Key) \n");
	printf("2.Total Rows and Total Columns must be between 2 and 50.\n");
	printf("3.Type of key : \n\t-Key made up of single square.\n\t-Key made up of multiple square.\n\t\t-Horizontal key.\n\t\t-Vertical Key.\n");
	printf("4.Each row must contain character made up of single square or the vertical key\n  and each column must contain character made up of single square or the Horizontal key..\n");
	printf("5.There is only one key corresponding to any given character.\n");
	printf("6.Each key is made up of one or more grid squares, which will always form a connected region.\n");
	printf("7.If Virtual is made up of multiple squares then :\n");
	printf("\t:it must be in type \"1 row multiple columns (HORIZONTAL KEY) or multiple columns 1 row (VERTICAL KEY)\" no other will be accepted\n8.Key flow :\n\t-if key is made up of single square then cursor move to left,right,upward,downward.\n\t-if key is Horizontal then cursor move to left,right.\n\t-if key is vertical then cursor move to upward,downward.\n\t-\"There must be path for each Key\"\n");
	printf("9.Text to be typed must not empty and dont contain * bcoz asterisk(*) representing ENTER Key.\n");
	printf("\n---------------------------------------------------------------------------------------------------------\n");
}
//start of tableAnalysis()
void tableAnalysis()
{
	int i,j,position;
	for(i=0;i<totalRows;i++)
	{
		for(j=0;j<totalColumns;j++)
		{
			position=findPosition(i,j);
			//symbol already present or not
			if(symbol[position].status!=1)//if not present
			{
				symbol[position].status=1;
				symbol[position].startRow=i;
				symbol[position].startCol=j;
			}
			symbol[position].endRow=i;//this means if one symbol has multiple square
			symbol[position].endCol=j;		
		}
	}
	
	
}
//end of tableAnalysis()
//start of findPosition
int findPosition(int i,int j)
{
	int position;	
	if((keyboard[i][j]>=65)&&(keyboard[i][j]<=90))//it is alpha
	{
		position=keyboard[i][j]-65;//like hashing
	}
	else if((keyboard[i][j]>=48)&&(keyboard[i][j]<=57))//it is digit
	{
		position=(keyboard[i][j]+26)-48;//like hashing
	}
	else if(keyboard[i][j]==45)//it is dash
	{
		position=(keyboard[i][j]+36)-45;//like hashing
	}
	else if(keyboard[i][j]==42)//it is astrisk
	{
		position=(keyboard[i][j]+37)-42;//like hashing
	}
	else
		return(-1);//wrong input

	return(position);
}
//end of findPosition
//start of tableSymbolType();
void tableSymbolType()
{
	int i,j,position;
	for(i=0;i<totalRows;i++)//start of first for
	{
		for(j=0;j<totalColumns;j++)//start of second for
		{

			position=findPosition(i,j);			
			if(symbol[position].status==1)//if symbol is present	
			{
				if((symbol[position].startRow==symbol[position].endRow)&&(symbol[position].startCol==symbol[position].endCol))//this symbol is single square
				{
					symbol[position].type=1;//make it as single square			
				}
				else if((symbol[position].startRow==symbol[position].endRow)&&(symbol[position].startCol!=symbol[position].endCol))//horizontal key
				{
					if((symbol[position].startCol!=0)&&(symbol[position].endCol==totalColumns-1))//left space available horizontal key
					{
						symbol[position].type=2;//left space available
					}
					else if((symbol[position].startCol==0)&&(symbol[position].endCol!=totalColumns-1))
					{
						symbol[position].type=4;//right space available
					}
					else
					{
						symbol[position].type=3;//both left and right space
					}
				}
				else if((symbol[position].startRow!=symbol[position].endRow)&&(symbol[position].startCol==symbol[position].endCol))//vertical key
				{
					if((symbol[position].startRow==0)&&(symbol[position].endRow!=totalRows-1))
					{
						symbol[position].type=5;//lower side space available
					}
					else if((symbol[position].startRow!=0)&&(symbol[position].endRow==totalRows-1))
					{
						symbol[position].type=7;//upper side space available
					}
					else
					{
						symbol[position].type=6;//both lower and upper space
					}
				}
			}	
		}
	}
}
//end of tableSymbolType();
//start of tableSymbolLength(void)
void tableSymbolLength(void)
{
	int i,j,position;
	for(i=0;i<totalRows;i++)
	{
		for(j=0;j<totalColumns;j++)
		{
			position=findPosition(i,j);			
			if(symbol[position].status==1)//if symbol is present	
			{
				if(symbol[position].type==1)//this is mean single character
				{
					if((symbol[position].startRow==0)&&(symbol[position].startCol==0))//upper left single char dont -left and upper
					{
						symbol[position].length=2;		
          symbol[position].connect.link[0]=findActualString(findPosition(i,j+1));
          symbol[position].connect.link[1]=findActualString(findPosition(i+1,j));
					}
					else if((symbol[position].startRow==0)&&(symbol[position].startCol==totalColumns-1))//upper right single char dont -right and upper
					{
						symbol[position].length=2;		
          symbol[position].connect.link[0]=findActualString(findPosition(i,j-1));
          symbol[position].connect.link[1]=findActualString(findPosition(i+1,j));
					}
					else if((symbol[position].startRow==totalRows-1)&&(symbol[position].startCol==0))//lower left single char dont -left and lower
					{
						symbol[position].length=2;		
          symbol[position].connect.link[0]=findActualString(findPosition(i,j+1));
          symbol[position].connect.link[1]=findActualString(findPosition(i-1,j));
					}
					else if((symbol[position].startRow==totalRows-1)&&(symbol[position].startCol==totalColumns-1))//lower right single char dont -right and lower
					{
						symbol[position].length=2;		
          symbol[position].connect.link[0]=findActualString(findPosition(i,j-1));
          symbol[position].connect.link[1]=findActualString(findPosition(i-1,j));
					}
					else if((symbol[position].startRow==0)&&(totalRows>1))//upper line //write else for this
					{
						symbol[position].length=3;		
          symbol[position].connect.link[0]=findActualString(findPosition(i,j-1));
	  symbol[position].connect.link[1]=findActualString(findPosition(i,j+1));
          symbol[position].connect.link[2]=findActualString(findPosition(i+1,j));
					}
					else if((totalColumns>1)&&(symbol[position].startCol==0))//left line
					{
						symbol[position].length=3;		
          symbol[position].connect.link[0]=findActualString(findPosition(i,j+1));//right
	  symbol[position].connect.link[1]=findActualString(findPosition(i+1,j));//lower
          symbol[position].connect.link[2]=findActualString(findPosition(i-1,j));//upper
					}
					else if((symbol[position].startRow==totalRows-1)&&(totalRows>1))//lower line
					{
						symbol[position].length=3;		
          symbol[position].connect.link[0]=findActualString(findPosition(i,j-1));//left
	  symbol[position].connect.link[1]=findActualString(findPosition(i,j+1));//right
          symbol[position].connect.link[2]=findActualString(findPosition(i-1,j));//upper
					}
					else if((totalColumns>1)&&(symbol[position].startCol==totalColumns-1))//right line
					{
						symbol[position].length=3;		
          symbol[position].connect.link[0]=findActualString(findPosition(i,j-1));//left
	  symbol[position].connect.link[1]=findActualString(findPosition(i+1,j));//lower
          symbol[position].connect.link[2]=findActualString(findPosition(i-1,j));//upper
					}
					else
					{
						symbol[position].length=4;		
          symbol[position].connect.link[0]=findActualString(findPosition(i,j-1));//left
	  symbol[position].connect.link[1]=findActualString(findPosition(i,j+1));//right
	  symbol[position].connect.link[2]=findActualString(findPosition(i+1,j));//lower
          symbol[position].connect.link[3]=findActualString(findPosition(i-1,j));//upper
					}
				}
				else if(symbol[position].type==2)//horizontal left space
				{
					symbol[position].length=1;		
          symbol[position].connect.link[0]=findActualString(findPosition(symbol[position].startRow,symbol[position].startCol-1));//left
				}
				else if(symbol[position].type==3)//horizontal both left & right
				{
					symbol[position].length=2;		
          symbol[position].connect.link[0]=findActualString(findPosition(symbol[position].startRow,symbol[position].startCol-1));//left
	  symbol[position].connect.link[1]=findActualString(findPosition(symbol[position].endRow,symbol[position].endCol+1));//right
				}
				else if(symbol[position].type==4)//horizontal right space
				{
					symbol[position].length=1;		
	  symbol[position].connect.link[0]=findActualString(findPosition(symbol[position].endRow,symbol[position].endCol+1));//right
				}						
				else if(symbol[position].type==5)//Vertical lower space 
				{
					symbol[position].length=1;
					symbol[position].connect.link[0]=findActualString(findPosition(symbol[position].endRow+1,symbol[position].endCol));//lower
				}
				else if(symbol[position].type==6)//Vertical lower and upper 
				{
					symbol[position].length=2;
					symbol[position].connect.link[0]=findActualString(findPosition(symbol[position].endRow+1,symbol[position].endCol));//lower
					symbol[position].connect.link[1]=findActualString(findPosition(symbol[position].startRow-1,symbol[position].endCol));//upper
				}		
				else if(symbol[position].type==7)//Vertical upper space 
				{
					symbol[position].length=1;
					symbol[position].connect.link[0]=findActualString(findPosition(symbol[position].startRow-1,symbol[position].endCol));//upper
				}

			}
		}
	}
}
//end of tableSymbolLength(void)
//start of validation()
int validation(void)
{
	short int i,pos,j,check=0;	
	char ch;	
	for(i=0;i<strlen(findString);i++)
	{
		pos=findPositionString(findString[i]);
		if(pos==-1 || pos==37)
		{
			printf("character \'%c\' is not allowed in Message.\n\n",findString[i]);
			return 0;		
		}
		else if(symbol[pos].status==0)
		{
			printf("character \'%c\' present in Message but it is not present in Keyboard.\n\n",findString[i]);
			return 0;

		}
	}
	for(i=0;i<totalRows;i++)
	{
		for(j=0;j<totalColumns;j++)
		{
			ch=keyboard[i][j];
			pos=findPositionString(ch);
			pos=symbol[pos].type;
			if(pos==1 || pos==5 ||pos==6 || pos==7)
			{
				check=1;
				break;
			}
		}
		if(!check)
		{
			printf("error in row :%d so please EXIT\n\n",i+1);
			return(0);
		}
		check=0;
	}
	for(i=0;i<totalColumns;i++)
	{
		for(j=0;j<totalRows;j++)
		{
			ch=keyboard[j][i];
			pos=findPositionString(ch);
			pos=symbol[pos].type;
			if(pos==1 || pos==2 ||pos==3 || pos==4)
			{
				check=1;
				break;
			}
		}
		if(!check)
		{
			printf("error in col :%d so please EXIT\n\n",i+1);
			return(0);
		}
		check=0;
	}
	return 1;
}
//end of validation()
//start of process()
void process(void)
{
	int terminate,temp1,temp2,stroke=0,position;
	char ch,k;	
	basicInitializationInitial(0,0);//initialize upper left	
	findString[strlen(findString)]='*';//attach * to end of string 
	basicInitializationString();//assign value to new var i.e newstartRow ...
	//display purpose	
	printf("----------------------------------------------------------------------------------------------------------\n\t  Cursor Position\tCharacter\tStrokes\t\tPath Which Contain Minimum Strokes");	
	printf("\n----------------------------------------------------------------------------------------------------------\n");
	for(terminate=0;terminate<strlen(findString);terminate++)
	{	
		position=findPosition(currentstartRow,currentstartCol);//find position of character indicated by current var to access its values from symbol table initially gives position of upper left character		
		ch=findActualString(position);//find character using position
		global[globalPoint]=ch;
		stroke=recursion(ch,position);//recursion used for finding paths
		stroke=stroke+1;//1 is added that stroke to select that char		
		keystrokes=keystrokes+stroke;//count total keystrokes required to type string		
		printf("\t\t%c\t\t    %c\t\t   %d",ch,findString[stringPoint],(stroke));
		//for display purpose if stroke is 1 digit then 3 space 2 tab if stroke is 2 digit then 2 space 2 tab
		if(stroke<10)
			printf("   \t\t");
		else
			printf("  \t\t");		
		for(k=0;k<=level;k++ )
		{
			if(k==level)//use for only display purpose at end dont display ->
				printf(" \'%c\'",forPrint[k]);
			else
				printf(" \'%c\'->",forPrint[k]);
		}		
		printf("\n");
		stringPoint++;//inc stringPointer to point to next char present in string 
		
		if(terminate!=strlen(findString))//after * dont call next only for efficient
		{
			basicInitializationInitial(newstartRow,newstartCol);
			basicInitializationString();//to set all new values which has info about next char
		}
		
		globalPoint=0;
		level=38;
		stroke=0;
	}	
	//only for display purpose
	printf("----------------------------------------------------------------------------------------------------------\n\n");
	findString[(strlen(findString)-1)]='\0';	
	printf("Total Minimum Strokes Required to type Message \"%s\" using Virtual Keyboard is :\n %d\n\n\n",findString,keystrokes);
}
//end of process()

//start of findActualString()
char findActualString(int position)
{
	char ch;	
	if(position>=0 && position<=25)
		ch=position+65;//means character
	else if(position>=26 && position<=35)
		ch=(position-26)+48;//means digit
	else if(position==36)
		ch='-';   //it is dash
	else if(position==37)
		ch='*';   //it is astrisk;
	else
		ch='?'; //wrong
	return(ch);

}
//end of findActualString()
int recursion(char ch,int position)
{
	int val=1,k,i,j,temppos;
	char ch1;
	
	if(ch==findActualString(findPosition(newstartRow,newstartCol)))
	{
		if(globalPoint<level)
		{		
			level=globalPoint;//assign length of new path

			for(k=0;k<=globalPoint;k++)//assign character of new path
				forPrint[k]=global[k];			
		}
		globalPoint--;
		return(level);
	}	
	else
	{
		for(i=0;i<symbol[position].length;i++)
		{
			for(j=0;j<=globalPoint;j++)
			{
				if(symbol[position].connect.link[i]==global[j])
				{
					val=0;
					break;
				}
			}
			if(val && (globalPoint<level))
			{
				globalPoint++;
				global[globalPoint]=symbol[position].connect.link[i];
				ch1=symbol[position].connect.link[i];//get pos of linked char	
				temppos=position;//temporary store position of current			
				position=findPositionString(symbol[position].connect.link[i]);//get the position of linked char and pass it to recursion			
				recursion(ch1,position);
				position=temppos;//reinitialize the position or head node of linked var to get its next linked char
			}
			val=1;
			
		}
		globalPoint--;
	}
	return(level);
}
//start of basicInitializationInitial()
void basicInitializationInitial(int i,int j)
{
	int position;
	position=findPosition(i,j);//find the position of upper left initial	
	
	currentstartRow=symbol[position].startRow;
	currentstartCol=symbol[position].startCol;
	currentendRow=symbol[position].endRow;
	currentendCol=symbol[position].endCol;
	currentsymbolType=symbol[position].type;
	
}
//end of basicInitializationInitial
//start of basicInitializationString
void basicInitializationString(void)
{
	int position;
	char ch;
	ch=findString[stringPoint];
		
	position=findPositionString(ch);//find the position of symbol present in findstring	

	newstartRow=symbol[position].startRow;
	newstartCol=symbol[position].startCol;
	newendRow=symbol[position].endRow;
	newendCol=symbol[position].endCol;
	newsymbolType=symbol[position].type;
	
}//end of basicInitializationString

//start of findpositionString
int findPositionString(char ch)
{
	int position;	
	if((ch>=65)&&(ch<=90))//it is alpha
	{
		position=ch-65;//like hashing
	}
	else if((ch>=48)&&(ch<=57))//it is digit
	{
		position=(ch+26)-48;//like hashing
	}
	else if(ch==45)//it is dash
	{
		position=(ch+36)-45;//like hashing
	}
	else if(ch==42)//it is astrisk
	{
		position=(ch+37)-42;//like hashing
	}
	else
		return(-1);//wrong input

	return(position);
}
