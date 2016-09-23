#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
pass1(){
	FILE *input,*sym,*temp;
	int i,j,k,line=1;
	char c,str[100];
	input=fopen("code.txt","r");
	if(input==NULL){
		printf("Code File Not Exist\n");
		exit(0);
	}
	temp=fopen("temp.txt","w");
	sym=fopen("symbol.txt","w");
	c=getc(input);
	while(c==' ' || c=='\n')
		c=getc(input);
	if(c==EOF){
		printf("Code File Is Empty\n");
		exit(0);
	}
	while(c!=EOF){
		if(c>96 && c<123)
		c-=32;
		putc(c,temp);
		c=getc(input);
	}
	fclose(temp);
	fclose(input);
	temp=fopen("temp.txt","r"); 
	input=fopen("input.txt","w");
	while(1){
		c=getc(temp);
		while(c==' ' || c=='\n')
			c=getc(temp);	
		if(c==EOF)
			break;
		for(k=0;c!='\n' && c!=EOF;k++)
		{
			str[k]=c;
			c=getc(temp);
		}
		str[k]='\0';
		for(i=0;i<k;i++){
			if(str[i]==':')
			{
				for(j=0;j<i;j++){
					if(str[j]!=' ')
						putc(str[j],sym);
				}
				fprintf(sym," %d ",line);
				putc('\n',sym);
				break;
			}
		}
		if(str[i]==':')
		{
			for(i++;str[i]==' ';i++);
			fprintf(input,"%s\n",str+i);
		}
		else{
			fprintf(input,"%s\n",str);
		}
		line++;
	}
	fclose(sym);
	fclose(input);
	fclose(temp);
	remove("temp.txt");	
}
pass2(){
	FILE *input,*output,*inst;
	int line=1,n,i,j;
	char str[100],opcode[32],c,opr[5][20];
	input=fopen("input.txt","r");
	output=fopen("output.txt","w");
	while(1){
		c=getc(input);
		while(c==' ' || c=='\n')
			c=getc(input);
		if(c==EOF)
			exit(0);
		for(i=0;c!=' ' && c!='\n';i++){
			str[i]=c;
			c=getc(input);
		}
		str[i]='\0';
		while(c==' ')
				c=getc(input);
		for(j=0,n=0;c!='\n';n++,j=0){
			while(c!=' ' && c!='\n'){
				opr[n][j]=c;
				c=getc(input);
				j++;
			}
			opr[n][j]='\0';
			while(c==' ')
				c=getc(input);	
		}
		sopcode(str,opcode,line,0);
		if(opcode[0]=='0'){
			if(n!=2){
				printf("Invalid no. of oprands in Line No. %d",line);
				exit(0);
			}
			fprintf(output,"%s",opcode);
			sopcode(opr[0],opcode,line,1);
			fprintf(output,"%s",opcode);
			sopcode(opr[1],opcode,line,1);
			fprintf(output,"%s",opcode);
			fprintf(output,"0000000000000000\n");
		}
		else if(opcode[4]=='0'){
			if(opcode[5]=='0')
			{
				if(n!=1){
				printf("Invalid no. of oprands in Line No. %d",line);
				exit(0);
				}
				fprintf(output,"%s",opcode);
				sopcode(opr[0],opcode,line,1);
				fprintf(output,"%s",opcode);
				fprintf(output,"0000000000000000\n");	
			}
			else{
				if(n!=2){
				printf("Invalid no. of oprands in Line No. %d",line);
				exit(0);
				}
				fprintf(output,"%s",opcode);
				sopcode(opr[0],opcode,line,1);
				fprintf(output,"%s",opcode);
				strbin(opr[1],opcode,line);
				fprintf(output,"%s\n",opcode);
			}
		}
		else{
			if(opcode[8]=='0')
			{
				if(n!=0){
				printf("Invalid no. of oprands in Line No. %d",line);
				exit(0);
				}
				fprintf(output,"%s",opcode);	
				fprintf(output,"0000000000000000\n");
			}
			else{
				if(n!=1){
				printf("Invalid no. of oprands in Line No. %d",line);
				exit(0);
				}
				fprintf(output,"%s",opcode);
				ssymbol(opr[0],opcode,line);
				fprintf(output,"%s\n",opcode);
			}
		}
        line++;
	}
}
sopcode(char str[],char opcode[],int line,int s){
	FILE *inst;
	if(s==0)
		inst=fopen("instruction.txt","r");
	else
		inst=fopen("register.txt","r");
	if(inst==NULL && s==0){
		printf("Instruction File Not Exist");
		exit(0);
	}
	else if(inst==NULL && s==1){
		printf("Register File Not Exist");
		exit(0);
	}
	char temp1[20],temp2[33];
	while(1){
		fscanf(inst,"%s%s",temp1,temp2);
		if(strcmp(temp1,str)==0){
			strcpy(opcode,temp2);
			fclose(inst);
			break;
		}
		if(feof(inst)){
			fclose(inst);
		    printf("Instruction or oprand %s in line no. %d dont Exist",str,line);
		    exit(0);
		} 
	}
}
ssymbol(char str[],char opcode[],int line){
	FILE *sym=fopen("symbol.txt","r");
	char temp1[10];
	int temp2=0;
	while(1){
		fscanf(sym,"%s %d",temp1,&temp2);
		if(strcmp(temp1,str)==0){
			fclose(sym);
			break;
		}
		if(feof(sym)){
			fclose(sym);
			printf("Symbol in line no. %d dont Exist",line);
		    exit(0);
		} 
	}
	intbin(temp2,opcode,line);
}
strbin(char str[],char temp[],int line){
	int a=strlen(str),i,n=0,j;
	if(a!=4)
	{
		printf("Invalid hexadecimal data in line no. %d",line);
		exit(0);
	}
	for(i=0,j=3;i<4;i++,j--){
		if(str[1]>47 && str[i]<58)
		n+=pow(16,j)*(str[i]-48);
		else if(str[1]>64 && str[i]<71)
		n+=pow(16,j)*(str[i]-55);
		else{
			printf("Invalid hexadecimal char %c in line no. %d",str[i],line);
			exit(0);
		}
	}
	intbin(n,temp,line);
}
intbin(int n,char temp[],int line){
	int i;
	if(n<1 || n>65535){
		printf("data out of bounds in line no. %d",line);
		exit(0);
	}
	temp[16]='\0';
	for(i=15;i>=0;i--)
	{
		if(n%2==1)
			temp[i]='1';
		else
			temp[i]='0';
		n/=2;
	}
}
int main()
{
	pass1();
	pass2();
	return 0;
}
