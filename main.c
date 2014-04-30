#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Buffer.c"



int main(int argc, char *argv[])
{
	buffer bufferPool;
	field  fieldList;
	int choose = 0;
	while(choose != 2)
	{
		printf("Escolha uma opção\n0- Preencher o Buffer\n1- Listar Buffer\n2- Sair\n");
		scanf("%d",&choose);
		if(choose == 0)
		{
			fillBuffer(&bufferPool,&fieldList,"meta.dat","data.dat");
			printf("Buffer Preenchido\n");
		}
		if(choose == 1)
		{
			showBuffer(&bufferPool);
		}
	}
	return 0;

}
