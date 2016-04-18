/*
*	command to run LD_PRELOAD=./mymalloc.so a.out
*/
#include <stdlib.h>
#include <stdio.h>
//#include <mymalloc.c>

//void print_globals();
int main()
{
	char * s = "Hello string12356";
	char * d = malloc(sizeof(char)*14);
	printf("%s %x\n"," destination address: ", d);
	char *res = strcpy(d,s);
	if(res == NULL) 
		{
			printf("%s\n", "value returned NULL" );
		}
		else{
		printf("%s\n", res);		
		}
	

	printf("Hello World ! %s", "Abhijeet\n");

	int* p = (int*) malloc(sizeof(int));

	if (p ==0)
	{
		printf("%s\n", "Error ! memory not allocated");
		return 1;
	}

	*p = 10;
	printf("%s %d\n", "Value is :", *p);


	//print_globals();
	return 0;
}