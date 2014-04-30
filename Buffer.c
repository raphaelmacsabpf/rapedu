#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TNAME_LENGHT 20
#define BUFFER_SIZE 16
typedef struct field
{
    char fName[TNAME_LENGHT];
    char fType;
    int fLenght;
}field;

typedef struct bufferPage
{
	int diskSeek; //Posição no disco para poder regravar os dados se necessário
	int pinCount; //Pin Count
	int rewriteBit;//Dirty Bit
	char *data; //Tupla de dados
}bufferPage;
typedef struct buffer
{
	int countItems;
	int nextPageAvaliable;
	field *fieldList;
	int fieldCount;
	bufferPage bp[BUFFER_SIZE];
}buffer;

void initBuffer(buffer *bPool,int lenght,field *fieldList, int fieldCount)
{
	int i;
	//Inicializa o buffer com todos os diskSeek em -1 para ser a flag que verifica se está em uso ou não.
	for(i = 0; i < lenght; i++)
	{
		bPool->bp[i].diskSeek = -1;
	}
	bPool->countItems = 0;
	bPool->nextPageAvaliable = 0;
	bPool->fieldList = fieldList;
	bPool->fieldCount = fieldCount;
}
void findNextAvaliable(buffer *bPool)
{
	int i;
	//Simples verificação se a página está disponível se do diskSeek for igual à -1
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		if(bPool->bp[i].diskSeek == -1)
		{
			bPool->nextPageAvaliable = i;
			break;
		}

	}
}
void applyReplacementPolicies(buffer *bPool)
{
	//Aqui é escolhido a página do buffer que tem o menor pinCount para ser substituida
	int i, lower = bPool->bp[0].pinCount;
	bufferPage bPage = bPool->bp[0];
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		if(bPool->bp[i].pinCount < lower)
		{
			bPage = bPool->bp[i];
			lower = bPool->bp[i].pinCount;
		}
	}
	//A página encontrada fica armazenada em bPage
	bPage.rewriteBit = 1;
	if(bPage.rewriteBit == 0)
	{
		bPool->bp[i].diskSeek = -1;
		bPool->nextPageAvaliable = i;
	}
	else
	{
		//Aqui deve ser colocado o código para regravação do arquivo.

	}
}
void bufferInsert(buffer *bPool,char *tuple, int diskSeek, int tupleLenght)
{
	//Primeiro verifica se o buffer está cheio, caso esteja ele aplica as políticas de troca
	if(bPool->countItems >= BUFFER_SIZE)
	{
		applyReplacementPolicies(bPool);
	}
	//É inserido na próxima página disponível
	bPool->bp[bPool->nextPageAvaliable].diskSeek = diskSeek;
	bPool->bp[bPool->nextPageAvaliable].data = (char *)malloc(tupleLenght*sizeof(char));
	strcpy(bPool->bp[bPool->nextPageAvaliable].data,tuple);
	bPool->bp[bPool->nextPageAvaliable].pinCount = 0;
	bPool->bp[bPool->nextPageAvaliable].rewriteBit = 0;
	bPool->countItems ++;
	//É procurado a próxima página disponível
	findNextAvaliable(bPool);
}
void showBuffer(buffer *bufferPool)
{
	//Primeiro é mostrado os campos
	int i;
	printf("\n");
	for(i = 0; i < bufferPool->fieldCount; i ++)
	{
		printf("%12s",bufferPool->fieldList[i].fName);
	}
	printf("\n");
	//Depois é mostrado o conteúdo do buffer
	for(i = 0; i < BUFFER_SIZE; i++)
	{
		//Só é mostrado as páginas que estão em uso.
		if(bufferPool->bp[i].diskSeek != -1)
			printf("%s\n",bufferPool->bp[i].data);
	}
	printf("\n\n");
}
int fillBuffer(buffer *bufferPool, field *fieldList,char fMeta[], char fData[])
{
	//Abertura dos arquivos de dados e de metadados
	FILE *fp = fopen(fMeta,"r+b");
	FILE *data = fopen(fData, "r+b");
	if(fp == NULL)
	{
		printf("File '%s' not found",fMeta);
		return 0;
	}
	if(data == NULL)
	{
		printf("File '%s' not found",fData);
		return 0;
	}

	int fieldCount;
	fseek(fp,0,0);
	//Lê a quantidade de campos no arquivo de metadados
	fread(&fieldCount,sizeof(fieldCount),1,fp);
	fieldList = malloc(sizeof(field) * fieldCount);
	initBuffer(bufferPool,BUFFER_SIZE,fieldList,fieldCount);
	int i,j,breakPoint = 0;
	int tupleLenght = 0;
	for(i = 0; i < fieldCount; i++)
	{
		for(j = 0;breakPoint == 0; j++)
		{
			//Encontra o nome do campo
			fread(&fieldList[i].fName[j],sizeof(char),1,fp);
			if(fieldList[i].fName[j] == '\0')
				breakPoint = j;
		}
		breakPoint = 0;
		fread(&fieldList[i].fType,sizeof(char),1,fp);
		fread(&fieldList[i].fLenght,sizeof(int),1,fp);
		//Vai montando o tamanho da tupla com base nos tamanhos dos campos encontrados
		tupleLenght += fieldList[i].fLenght;
	}
	//Como o arquivo de metadados não precisa ser mais utilizado, o mesmo é fechado
	fclose(fp);
	//Cria os campos temporários para a montagem da tupla
	int *tInt = malloc(sizeof(int));
	double *tDouble = malloc(sizeof(double));
	char *tChar = NULL;
	char *tTuple = malloc(sizeof(char)*tupleLenght);
	//Começa a leitura dos dados.
	for(strcpy(tTuple,"");;strcpy(tTuple,""))
	{
		//Cada tupla lida é inserida no buffer
		for(i = 0; i < fieldCount; i++)
		{
			//Cada campo de tupla é checado seu tipo e adicionado corretamente na tupla temporária *tTuple
			if(fieldList[i].fType == 'I')
			{
				//Inteiro
				fread(tInt,sizeof(int),1,data);
				sprintf(tTuple, "%s%d",tTuple,*tInt);
			}
			else if(fieldList[i].fType == 'D')
			{
				//Double
				fread(tDouble,sizeof(double),1,data);
				sprintf(tTuple, "%s%lf",tTuple,*tDouble);
			}
			else if(fieldList[i].fType == 'S')
			{
				//String
				tChar = (char *)malloc(sizeof(char) * fieldList[i].fLenght);
				fread(tChar,sizeof(char),fieldList[i].fLenght,data);
				sprintf(tTuple, "%s%s",tTuple,tChar);
			}
			else if(fieldList[i].fType == 'C')
			{
				//Caracter
				tChar = (char *)malloc(sizeof(char));
				fread(tChar,sizeof(char),1,data);
				sprintf(tTuple, "%s%s",tTuple,tChar);
			}
		}
		//Se o arquivo de dados chegar ao fim, as tuplas param de ser entregues ao bufferPool
		if(feof(data))
			break;
		bufferInsert(bufferPool,tTuple,ftell(data)-tupleLenght,tupleLenght);

	}
	//Fecha o arquivo de dados
	fclose(data);
	return 0;
}
