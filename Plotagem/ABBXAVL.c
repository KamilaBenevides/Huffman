#include <stdio.h>
#include <stdlib.h>
#include <time.h>



struct binary_tree
{
    long int value;
    int h;
    struct binary_tree *left;
    struct binary_tree *right;
};


int max(int a, int b)
{
    return (a > b) ? a : b;
}


int h(struct binary_tree *bt)
{
    if (bt == NULL)
    {
        return -1;
    }
    else
    {
        return 1 + max(h(bt->left), h(bt->right));
    }
}


int is_balanced(struct binary_tree *bt)
{
    int bf = h(bt->left) - h(bt->right);
    return ((-1 <= bf) && (bf <= 1));
}

int balance_factor(struct binary_tree *bt)
{
    if (bt == NULL)
    {
        return 0;
    }
    else if ((bt->left != NULL) && (bt->right != NULL))
    {
        return (bt->left->h - bt->right->h);
    }
    else if ((bt->left != NULL) && (bt->right == NULL))
    {
        return (1 + bt->left->h);
    }
    else
    {
        return (-bt->right->h - 1);
    }
}


struct binary_tree *create_empty_binary_tree()
{
    return NULL;
}


struct binary_tree *create_binary_tree(long int value, struct binary_tree *left, struct binary_tree *right)
{
    struct binary_tree *new_bt = (struct binary_tree *)malloc(sizeof(struct binary_tree));

    new_bt->value = value;
    new_bt->left = left;
    new_bt->right = right;
    new_bt->h = 0;

    return new_bt;
}

struct binary_tree *rotate_left(struct binary_tree *bt)
{
    struct binary_tree *subtree_root = NULL;
    if (bt != NULL && bt->right != NULL)
    {
        subtree_root = bt->right;
        bt->right = subtree_root->left;
        subtree_root->left = bt;
    }
    subtree_root->h = h(subtree_root);
    bt->h = h(bt);
    return subtree_root;
}


struct binary_tree *rotate_right(struct binary_tree *bt)
{
    struct binary_tree *subtree_root = NULL;
    if (bt != NULL && bt->left != NULL)
    {
        subtree_root = bt->left;
        bt->left = subtree_root->right;
        subtree_root->right = bt;
    }
    subtree_root->h = h(subtree_root);
    bt->h = h(bt);
    return subtree_root;
}

struct binary_tree *addForAVL(struct binary_tree *bt, int value)
{
    if (bt == NULL)
    {
        return create_binary_tree(value, NULL, NULL);
    }
    else if (bt->value > value)
    {
        bt->left = addForAVL(bt->left, value);
    }
    else
    {
        bt->right = addForAVL(bt->right, value);
    }

    bt->h = h(bt);
    struct binary_tree *child;
    if (balance_factor(bt) == 2 || balance_factor(bt) == -2)
    {
        if (balance_factor(bt) == 2)
        {
            child = bt->left;
            if (balance_factor(child) == -1)
            {
                bt->left = rotate_left(child);
            }
            bt = rotate_right(bt);
        }
        else if (balance_factor(bt) == -2)
        {
            child = bt->right;
            if (balance_factor(child) == 1)
            {
                bt->right = rotate_right(child);
            }
            bt = rotate_left(bt);
        }
    }
    return bt;
}


struct binary_tree *search_abb(FILE *fp, struct binary_tree *bt, long int value, int cont)
{
    if(bt == NULL)
    {
        fprintf(fp, "%d,", cont);
        return bt;
    }
    if(bt->value == value)
    {
        fprintf(fp, "%d,", cont);
        return bt;
    }    
    else if(bt->value > value)
    {
        return search_abb(fp, bt->left, value, cont+1);
    }
    else
    {
        return search_abb(fp, bt->right, value, cont+1);
    }

    return bt;
}

struct binary_tree *search_avl(FILE *fp2, struct binary_tree *bt, long int value, int cont)
{
    if(bt == NULL)
    {
        fprintf(fp2, "%d,", cont);
        return bt;
    }
    if(bt->value == value)
    {
        fprintf(fp2, "%d,", cont);
        return bt;
    }    
    else if(bt->value > value)
    {
        return search_avl(fp2, bt->left, value, cont+1);
    }
    else
    {
        return search_avl(fp2, bt->right, value, cont+1);
    }

    return bt;
}


struct binary_tree *add(struct binary_tree *bt, long int value)
{
    if (bt == NULL)
    {
        bt = create_binary_tree(value, NULL, NULL);
        return bt;
    }
    struct binary_tree *current = bt;
    struct binary_tree *previous = NULL;
    while (current != NULL)
    {
        previous = current;
        if (current->value > value)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    if (previous->value > value)
    {
        previous->left = create_binary_tree(value, NULL, NULL);
    }
    else
    {
        previous->right = create_binary_tree(value, NULL, NULL);
    }

    return bt;
}

int main()
{
    srand(time(NULL));  

    FILE *fp;
    FILE *fp2; 
    FILE *fp3;                         
    fp = fopen("ABB.txt", "a");
    fp2 = fopen("AVL.txt","a");
    fp3 = fopen("numSort.txt","a");
                                

  
    struct binary_tree *ABB = create_empty_binary_tree();
    struct binary_tree *AVL = create_empty_binary_tree();
    
    long int i = 0;
    
    for(i=0;i<100;i++) 
    {
        long int j = rand();
        AVL = addForAVL(AVL, j);
        ABB = add(ABB, j);
    }
    
    int array[100];
    i = 0;

    for(i=0;i<100;i++)  
    {
        array[i] = rand();
        fprintf(fp3,"%d,",array[i]); 
    }
    i = 0;
    
     for(i=0;i<100;i++) 
    {
        search_abb(fp, ABB, array[i], 0);           
        search_avl(fp2, AVL, array[i], 0);           
                                             
    }
   

    fclose(fp);
    fclose(fp2);
    fclose(fp3);
    return 0;
}