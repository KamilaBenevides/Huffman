#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CUnit/Basic.h"

struct Tree
{
	long int qnt;
	int item;
	struct Tree *right;
	struct Tree *left;
	struct Tree *next;
};


typedef struct Tree tree;


/*
Pointer to the file used by the tests.
*Ponteiro para o arquivo usado pelos testes.*
*/
static FILE* test_file = NULL;


/*
INITIALIZE_SUITE
The suite initialization function.
*A função de inicialização do conjunto.*

Opens the temporary file used by the tests.
*Abre o arquivo temporário usado pelos testes.*

Returns zero on success, non-zero otherwise.
*Retorna zero em caso de sucesso, diferente de zero.*
*/
int initialize_suite(void)
{
	if(NULL == (test_file = fopen("test.txt", "w+")))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}


/*
CLEAN_SUITE
The suite cleanup function.
*A função de limpeza da suíte.*

Closes the temporary file used by the tests.
*Fecha o arquivo temporário usado pelos testes.*

Returns zero on success, non-zero otherwise.
*Retorna zero em caso de sucesso, ou diferente de zero.*
*/
int clean_suite(void)
{
	if(0 != fclose(test_file))
	{
		return -1;
	}
	else
	{
		test_file = NULL;
		return 0;
	}
}


tree* add(tree *bt, unsigned char letter, int value)
{
	tree *new_tree = (tree*) malloc(sizeof(tree));

	new_tree -> qnt = value;
	new_tree -> item = letter;
	new_tree -> right = NULL;
	new_tree -> left = NULL;
	new_tree -> next = bt;

	return new_tree;
}

tree* purge(tree* queue, int *item)
{

	tree *aux = queue;

	queue = queue -> next;

	*item = aux -> item;

	return queue;
}


void print(tree* bt)
{
	printf("\n queue:");

	while(bt != NULL)
	{
		printf(" %d", bt -> item);
		bt = bt -> next;
	}
	puts("");

}

void testqueue(void)
{
	int i, item;
	tree *queue = NULL;

	CU_ASSERT(queue == NULL);

	for(i = 50; i > 0; i--)
	{
		queue = add(queue, i, 1);
	}

	print(queue);

	for(i = 1; i <= 50; i++)
	{
		queue = purge(queue, &item);
		i += 8;
		CU_ASSERT(i == item);
	}

	CU_ASSERT(queue == NULL);
}


int main()
{
	CU_pSuite pSuite = NULL;

	/* initializes the CUnit test record
	   inicializa o registro de teste CUnit */
	if(CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	/* adds a suite to the registry 
	   adiciona um conjunto ao registro */
	pSuite = CU_add_suite("suite", initialize_suite, clean_suite);
	if(NULL == pSuite)
	{
		CU_cleanup_registry();

		return CU_get_error();
	}

	/* adds the tests to the suite 
	   adiciona os testes ao suite*/
	if((NULL == CU_add_test(pSuite, "test queue", testqueue)))
	{
		CU_cleanup_registry();

		return CU_get_error();
	}

	/* Performs all tests using the CUnit Basic interface 
      Executa todos os testes usando a interface CUnit Basic*/
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}

