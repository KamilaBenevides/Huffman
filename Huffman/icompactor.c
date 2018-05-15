#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tree
{
	int qnt;
	void *item;
	struct Tree *right;
	struct Tree *left;
	struct Tree *next;
};
typedef struct Tree tree;

struct Tree_dcp
{
  unsigned char item;
  struct Tree_dcp* left;
  struct Tree_dcp* right;
};
typedef struct Tree_dcp tree_dcp;

struct List_bin
{
	int value;
	struct List_bin *next;
};
typedef struct List_bin list_bin;

struct Hash
{
	list_bin *letter[256];
};
typedef struct Hash hash;

//////////////////////////////////////////////////////////////////////////////
//  ***COMPACTADOR***

//  ADICIONA UM NÓ À LISTA
tree* add_list(int value, unsigned char letter, tree* bt, int *size_tree)
{
  int i = 0;
  tree* new_tree = (tree*)malloc(sizeof(tree));

  if(letter == '*' || letter == '\\')
  {
    unsigned char *pointer = malloc(sizeof(unsigned char)*2);
    pointer[0] = '\\';
    pointer[1] = letter; 
    new_tree -> item = pointer;
    *size_tree += 2;
  }
  else 
  {
    unsigned char *pointer = malloc(sizeof(unsigned char));
    *pointer = letter; 
    new_tree -> item = pointer;
    *size_tree += 1;
  }
  
  new_tree -> qnt = value;
  new_tree -> right = NULL;
  new_tree -> left = NULL;
  new_tree -> next = NULL;

  if(bt != NULL)
  {
    tree* tree_aux = bt;
    if(tree_aux -> next != NULL)
    {
      while(tree_aux -> next != NULL && tree_aux -> next -> qnt < value)
      {
        tree_aux = tree_aux -> next;
        i++;
      }
    }
    if(tree_aux -> qnt >= value) new_tree -> next = tree_aux;
    else
    {
      if(tree_aux -> next != NULL) new_tree -> next = tree_aux -> next;
      tree_aux -> next = new_tree;
      i++;
    }
  }

  if(i == 0) return new_tree;
  else return bt;
}

//  CRIA A LISTA
tree* create_list(int *frequency, tree *new_tree, int *size_tree)
{
	int i;
	for(i = 0 ; i < 256 ; i++)
	{
		if(frequency[i])
		{
			new_tree = add_list(frequency[i], (unsigned char)i, new_tree, &*size_tree);
		}
	}
  return new_tree;
}

//  TRANSFORMA A LISTA EM ÁRVORE
tree* list_metamorphosis(tree *list)
{
  tree *father = (tree*)malloc(sizeof(tree));
  unsigned char *aux = (unsigned char*)malloc(sizeof(unsigned char));
  *aux = '*';
  father -> qnt = list -> qnt + list -> next -> qnt;
  father -> item = aux;
  father -> left = list;
  father -> right = list -> next;
  father -> next = NULL;
  if(list -> next -> next != NULL)
  {
    tree *aux_tree = list -> next -> next;
    while(aux_tree -> next != NULL && aux_tree -> next -> qnt < father -> qnt)
    {
      aux_tree = aux_tree -> next;
    }
    if(aux_tree -> qnt >= father -> qnt)
    {
      father -> next = aux_tree;
      return father;
    }
    else
    {
      if(aux_tree -> next != NULL)
      {
        father -> next = aux_tree -> next;
        aux_tree -> next = father;
      }
      else aux_tree -> next = father;
    }
    return list -> next -> next;
  }
  else return father;
}

//  CRIA UMA HASH VAZIA
hash* create_hash()
{
  int i;
  hash *new_hash = (hash*)malloc(sizeof(hash));
  for (i = 0; i < 256; i++)
  {
    new_hash -> letter[i] = NULL;
  }
  return new_hash;
}

//  ADICIONA 0 OU 1 À SEQUENCIA BINÁRIA DA HASH
list_bin* add_bin(list_bin *atual_bin, int value)
{
  list_bin *new_bin = (list_bin*)malloc(sizeof(list_bin));
  new_bin -> value = value;
  new_bin -> next = NULL;
  if(atual_bin != NULL) new_bin -> next = atual_bin;
  return new_bin;
}

// CRIA A SEQUENCIA BINÁRIA DE CADA CARACTER PRESENTE NA ÁRVORE
list_bin* create_list_bin(tree *head, unsigned char letter, int *door)
{
  unsigned char *aux = head -> item;
  list_bin *new_list = NULL;
  if(*aux == letter && letter != '*' && letter != '\\') *door = 1;
  if(letter == '*' || letter == '\\') 
  {
    if(aux[1] == letter) *door = 1;
  }
  if(head -> left != NULL && *door == 0)
  {
    new_list = create_list_bin(head -> left, letter, &*door);
    if(*door) new_list = add_bin(new_list, 0);
  }
  if(head -> right != NULL && *door == 0)
  {
    new_list = create_list_bin(head -> right, letter, &*door);
    if(*door) new_list = add_bin(new_list, 1);
  }
  return new_list;
}

//  SETA UM BIT
unsigned char setting_bit(unsigned char c, int i)
{
	unsigned char mask = 1 << i;
	return mask | c;
}

//  VERIFICA A SETAGEM DE BITS
int set_bit(unsigned char c, int i)
{
  unsigned char mask = 1 << i;
  return mask & c;
}

// SETA OS BITS 
int setting(hash* head, unsigned char *buffer, FILE *new_arq, int size, unsigned char *aux_byte)
{
	int i, j;
	unsigned char byte;
	j = 7;
	byte = 0;
	for (i = 0; i < size; i++)
	{
		list_bin *aux = head -> letter[(int)buffer[i]];
		while(aux != NULL)
		{
			if(aux -> value == 1) byte = setting_bit(byte,j);
			if(j == 0)
			{
				fprintf(new_arq, "%c", byte);
				byte = 0;
				j = 7;
			}
			else j--;
			aux = aux -> next;
		}
	}
	*aux_byte = byte;
	return j+1;
}

// CRIA O CABEÇAHLO
void create_new_header(unsigned char *string, int trash, long int size_tree)
{
	unsigned char bit_one, bit_two;
	bit_one = 0;
	bit_two = 0;
	
	bit_one = trash << 5;
	bit_two = bit_two | size_tree;
	size_tree = size_tree >> 8;
	bit_one = size_tree | bit_one;
	//printf("one: %d two:%d \n", bit_one, bit_two);

	string[0] = bit_one;
	string[1] = bit_two;

}

//////////////////////////////////////////////////////////////////////////////
//  ***DESCOMPACTADOR***

////////ARVORE//////
tree_dcp* create_empty_tree()
{
  return NULL;
}

/////////CRIAR ARVORE////////////////
tree_dcp* create_binary_tree(unsigned char valor, tree_dcp* esquerda,tree_dcp* direita)
{
  tree_dcp* new_bt = (tree_dcp*) malloc(sizeof(tree_dcp));
  new_bt -> item = valor;
  new_bt -> left = esquerda;
  new_bt -> right = direita;
  return new_bt;
}

///////////ADICIONAR ARVORE//////////
tree_dcp* add(tree_dcp* bt, char *string, int *i)
{
  if(string[*i] == '\\')
  {
    bt = create_binary_tree(string[*i+1], NULL, NULL);
    *i += 1;
    return bt;
  }
  else
  {
    bt = create_binary_tree(string[*i], NULL, NULL);
  }
  
  if(string[*i] != '*')
  {
    return bt;
  }

  *i += 1;
  bt -> left = add(bt -> left, string, &*i);
  *i += 1;
  bt -> right = add(bt -> right, string, &*i);
  return bt;
}

///LIMPA ARVORE////
void clean(tree_dcp* bt)
{
  if(bt -> left != NULL) clean(bt -> left);
  if(bt -> right != NULL) clean(bt -> right);
  free(bt);
}

//  NAVEGA PELA ÁRVORE CONFORME OS BITS DO ARQUIVO COMPRESSO
tree_dcp* bit(unsigned char letter, tree_dcp *head, tree_dcp *aux, FILE *arq, int trash, int flag)
{
  int i;
  for (i = 7; i >= 0; i--)
  {
    if(flag == 1 && i == trash - 1) break;
    if(set_bit(letter,i)) aux = aux -> right;
    else aux = aux -> left;
    if(aux -> left == NULL)
    {
      fprintf(arq, "%c", aux -> item);
      aux = head;
    }
  }
  return aux;
}


//////////////////////////////////////////////////////////////////////////////
//  ***EXTRA***

//  IMPRIME A LISTA NO TERMINAL
void print_list(tree* bt)
{
  while(bt != NULL)
  {
    unsigned char *pointer = bt -> item;
    if(pointer[0] == '\\') printf("\\%c = %d\n", pointer[1],  bt -> qnt);
    else printf("%c = %d\n", *pointer,  bt -> qnt);
    bt = bt -> next;
  }
}

// IMPRIME A ÁRVORE NO TERMINAL
void print_tree(tree *bt)
{
  if(bt != NULL)
  {
    unsigned char *aux = bt -> item;
    printf("(%c", *aux);
    print_tree(bt -> left);
    print_tree(bt -> right);
    printf(")");
  }
  else printf("()");
}

// IMPRIME A ÁRVORE NO ARQUIVO
void print_tree_Arq(tree *head, FILE *arq)
{
	if(head != NULL)
	{
		unsigned char *print = head -> item;
		if(print[0] == '\\') fprintf(arq, "%c%c", print[0], print[1]);
		else fprintf(arq, "%c", print[0]);
		print_tree_Arq(head -> left, arq);
		print_tree_Arq(head -> right, arq);
	}
}

// DA FREE NA ÁRVORE
void free_all(tree *new_tree)
{
	if(new_tree -> right != NULL) free_all(new_tree -> right);
	if(new_tree -> left != NULL) free_all(new_tree -> left);
	if(new_tree -> item != NULL) free(new_tree -> item);
	free(new_tree);
}

int main()
{
	tree *map_tree = NULL;
  FILE *arq = NULL;
	unsigned char string[50], *buffer;
	int frequency[256] = {0};
	int result, size_file, i, size_tree, trash, option;
  memset(string,(unsigned char)0,sizeof(unsigned char)*50);
	
  //  CABEÇALHO
	printf("\tBem vindo ao ICompactor!\nO compactador de arquivos feito pelos melhores alunos do IC!\n\n");
	printf("1 - Compactar\n2 - Descompactar\n");
  printf("\n\n\n\n\n");
  printf("\t\t\tPressione qualquer tecla para sair.\n\n");
  scanf("%d", &option);
  getchar();
  system("clear");

  switch(option)
  {
    case 1:
      puts("Digite o nome do arquivo a ser compactado:");
      scanf("%s", string);
      arq = fopen(string,"rb");
      if(arq == NULL)
      {
        printf("Arquivo nao encontrado.\nPrograma finalizado.\n");
        return 0;
      }
      //  ABRE O ARQUIVO, DESCOBRE O TAMANHO, LÊ E GUARDA TUDO NO BUFFER
      fseek(arq , 0 , SEEK_END);
      size_file = ftell(arq);
      rewind(arq);
      buffer = malloc (sizeof(unsigned char)*size_file);
      result = fread (buffer, 1, size_file, arq);
      
      //  CONFERE SE FOI POSSÍVEL LER TODOS OS CARACTERES DO ARQUIVO
      //printf("size_file = %d\n", size_file);
      //printf("result = %d\n", result);

      //  SALVA A FREQUÊNCIA DE CADA CARACTERE DO TEXTO
      for(i = 0 ; i < size_file ; i++)
      {
        frequency[(int)buffer[i]]++;
      }

      //  CRIA A LISTA COM TODOS OS CARACTERES QUE APARECERAM, ORDENADA DO QUE MENOS APARECEU PARA O QUE MAIS APARECEU
      size_tree = 0;
      map_tree = create_list(frequency, map_tree, &size_tree);

      //  IMPRIME A LISTA COMPLETA NO TERMINAL
      /*print_list(map_tree);
      printf("size_tree = %d\n", size_tree);*/

      //  TRANSFORMA A LISTA EM ÁRVORE
      while(map_tree -> next != NULL)
      {
        map_tree = list_metamorphosis(map_tree);
        size_tree++;
      }

      //  IMPRIME A ÁRVORE COMPLETA NO TERMINAL
      /*print_tree(map_tree);
      puts("");
      printf("size_tree = %d\n", size_tree);*/

      //  CRIA A UMA HASH COM O CAMINHO DE CADA CARACTERE NA ÁRVORE
      int door = 0;
      hash *hash_table = create_hash();
      for (i = 0; i < 256; i++)
      {
        if(frequency[i])
        {
          hash_table -> letter[i] = create_list_bin(map_tree,(unsigned char)i, &door);
          door = 0;
        }
      }

      //  IMPRIME A HASH COMPLETA
      for(i = 0; i < 256; i++)
      {
        if(frequency[i])
        {
          list_bin *aux = hash_table -> letter[i];
          while(aux != NULL)
          {
            aux = aux -> next;
          }
        }
      }

      //  ABRE UM NOVO ARQUIVO A SER ESCRITO 
      memset(string,(unsigned char)0,sizeof(unsigned char)*50);
      puts("Digite o novo nome do arquivo compactado:");
      scanf("%s",string);
      FILE *arq_compac = fopen(string,"wb");

      //  CRIA DOS UNSIGNED CHAR PARA ESCREVER OS DOIS PRIMEIRO BYTES NO ARQUIVO 
      unsigned char header[2];
      header[0] = '*';
      header[1] = '*';
      fprintf(arq_compac, "%c%c", header[0], header[1]);
      memset(header,(unsigned char)0,sizeof(unsigned char)*2);

      //  IMPRIME A ÁRVORE NO ARQUIVO
      print_tree_Arq(map_tree, arq_compac);

      //  CHAMA A FUNÇÃO QUE SETA OS BITS E ESCREVE NO ARQUIVO
      unsigned char aux_byte = 0;
      trash = setting(hash_table, buffer, arq_compac, size_file, &aux_byte);
      if(trash != 8) fprintf(arq_compac, "%c", aux_byte);

      //printf("lixo = %d\n%c", trash, aux_byte);
      //printf("size_tree = %d\n", size_tree);

      //  FINALIZA O PROGRAMA
      rewind(arq_compac);
      create_new_header(header, trash, size_tree);
      fprintf(arq_compac, "%c%c", header[0], header[1]);
      fclose(arq_compac);
      break;

    case 2:
      puts("Digite o nome do arquivo a ser descompactado:");
      scanf("%s",string);
      tree_dcp* new_tre = create_empty_tree();
      arq = fopen(string,"rb");
      if(arq == NULL)
      {
        printf("Arquivo nao encontrado.\nPrograma finalizado.\n");
        return 0;
      }

      //  ABRE O ARQUIVO, DESCOBRE O TAMANHO, LÊ E GUARDA TUDO EM LETTER
      int tamanho_file, confere;
      fseek (arq , 0, SEEK_END);
      tamanho_file = ftell(arq);
      rewind(arq);
      unsigned char *letter = (unsigned char*)malloc(sizeof(unsigned char)*tamanho_file);
      confere = fread(letter,1,tamanho_file,arq);

      //  VERIFICA SE FOI POSSÍVEL LER COMPLETAMENTE O ARQUIVO
      if (confere != tamanho_file) printf("Nao leu tudo\n");
      //printf("tamanho_file = %d e confere = %d\n", tamanho_file, confere);

      //  DESCOBRE O VALOR DO LIXO E O TAMANHO DA ÁRVORE.
      unsigned char a[2], aux;
      a[0] = letter[0];
      a[1] = letter[1];
      int trash = 0, size_tree = 0, i, j;

      aux = a[0];
      trash = a[0] >> 5;
      a[0] = aux;
      a[0] = a[0] << 3;
      a[0] = a[0] >> 3;

      size_tree = a[0] << 8;
      size_tree = size_tree | a[1];
      //printf("lixo:%d tamanho da arvore:%d\n", trash, size_tree);

      //  LÊ A ÁRVORE COMPLETA
      unsigned char tree_string[size_tree];
      j = 0;
      for (i = 2; i < size_tree+2; i++,j++)
      {
        tree_string[j] = letter[i];
      }

      //  CRIA A ÁRVORE
      i = 0;
      new_tre = add(new_tre, tree_string, &i);

      //  IMPRIME A ÁRVORE NO TERMINAL
      //print(new_tre);
      //puts("");

      //  ABRE UM NOVO ARQUIVO A SER ESCRITO
      memset(string,(unsigned char)0,sizeof(unsigned char)*50);
      puts("Digite o novo nome do arquivo descompactado:");
      scanf("%s",string);
      FILE *desq = fopen(string,"w");
      
      //  NAVEGA PELA ÁRVORE CONFORME OS BITS DO ARQUIVO COMPRESSO E DESCOMPRIME NO NOVO ARQUIVO
      int cont_s = size_tree+2, flag = 0;
      tree_dcp *aux_tree = new_tre;
      for (i = cont_s; i < tamanho_file; i++)
      {
        if(i == tamanho_file-1) flag = 1;
        aux_tree = bit(letter[i], new_tre, aux_tree, desq, trash, flag);
      }

      //  FINALIZA O PROGRAMA
      clean(new_tre);  
      fclose(arq);
      fclose(desq);
      free(letter);
      break;
  }

 return 0;
}