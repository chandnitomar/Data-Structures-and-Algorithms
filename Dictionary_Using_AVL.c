/*
This is my DSA Project of 3rd Semester

This code takes a bunch of words from the user and gives the dictionary order as the output
Also it facilitates the searching of certain words presence in that bunch of words

Working:
We Insert the input word in the AVL tree by comparing the ASCII values of the 1st string literal, if found same then compare the ASCII values of 2nd string literal and so on
Then we simply traverse the tree with the help of Inorder Traversal Function 
Dictionary order is basically the inorder traversal of this AVL tree
*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h> 
#include <assert.h>


struct node_s
{
	struct node_s *left;
	struct node_s *right;
	char* value;
};

typedef struct node_s avl_node_t;

struct tree_s
{
	struct node_s *root;
};

typedef struct tree_s avl_tree_t;


/* Create a new AVL tree. */
avl_tree_t *create()
{
	avl_tree_t *tree = NULL;

	if( ( tree = malloc( sizeof( avl_tree_t ) ) ) == NULL )
	{
		return NULL;
	}

	tree->root = NULL;

	return tree;
}

/* Initialize a new node. */
avl_node_t *createnode()
{
	avl_node_t *node = NULL;

	if( ( node = malloc( sizeof( avl_node_t ) ) ) == NULL )
	{
		return NULL;
	}

	node->left = NULL;
	node->right = NULL;
	node->value = malloc( sizeof(char*)*64 );

	return node;
}

/* Find the height of an AVL node recursively */
int nodeheight( avl_node_t *node )
{
	int left_height = 0;
	int right_height = 0;

	if( node->left )
		left_height = nodeheight( node->left );
	if( node->right )
		right_height = nodeheight( node->right );

	return right_height > left_height ? ++right_height : ++left_height;
}

/* Find the balance of an AVL node */
int bal_factor( avl_node_t *node )
{
	int bf = 0;

	if( node->left  )
		bf += nodeheight( node->left );
	if( node->right )
		bf -= nodeheight( node->right );

	return bf ;
}

/* Left Left Rotate */
avl_node_t *rotate_leftleft( avl_node_t *node )
{
 	avl_node_t *a = node;
	avl_node_t *b = a->left;

	a->left = b->right;
	b->right = a;

	return b ;
}

/* Left Right Rotate */
avl_node_t *rotate_leftright( avl_node_t *node )
{
	avl_node_t *a = node;
	avl_node_t *b = a->left;
	avl_node_t *c = b->right;

	a->left = c->right;
	b->right = c->left;
	c->left = b;
	c->right = a;

	return c;
}

/* Right Left Rotate */
avl_node_t *rotate_rightleft( avl_node_t *node )
{
	avl_node_t *a = node;
	avl_node_t *b = a->right;
	avl_node_t *c = b->left;

	a->right = c->left;
	b->left = c->right;
	c->right = b;
	c->left = a;

	return c ;
}

/* Right Right Rotate */
avl_node_t *rotate_rightright( avl_node_t *node )
{
	avl_node_t *a = node;
	avl_node_t *b = a->right;

	a->right = b->left;
	b->left = a;

	return b ;
}

/* Balance a given node */
avl_node_t *bal_node( avl_node_t *node )
{
	avl_node_t *newroot = NULL;

	/* Balance our children, if they exist. */
	if( node->left )
		node->left  = bal_node( node->left  );
	if( node->right )
		node->right = bal_node( node->right );

	int bf = bal_factor(node);

	if( bf >= 2 )
	{
		/* Left Heavy */

		if( bal_factor(node->left) <= -1 )
			newroot = rotate_leftright(node);
		else
			newroot = rotate_leftleft(node);

	}
	else if( bf <= -2 )
	{
		/* Right Heavy */

		if(bal_factor(node->right) >= 1)
			newroot = rotate_rightleft(node);
		else
			newroot = rotate_rightright(node);

	}
	else
	{
		/* This node is balanced -- no change. */
		newroot = node;
	}

	return newroot;
}

/* Balance a given tree */
void bal_tree(avl_tree_t *tree)
{

	avl_node_t *newroot = NULL;

	newroot = bal_node( tree->root );

	if( newroot != tree->root )
		tree->root = newroot;

}

/* Insert a new node. */
void insert( avl_tree_t *tree, char* value )
{
	avl_node_t *node = NULL;
	avl_node_t *next = NULL;
	avl_node_t *last = NULL;


	if( tree->root == NULL ) {
		node = createnode();
		node->value = value;

		tree->root = node;


	}
	else
	{
		next = tree->root;

		while( next != NULL )
		{
			last = next;

			if( strcmp(value,next->value) < 0 )
			{
				next = next->left;
			}
			else if( strcmp(value,next->value) > 0 )
			{
				next = next->right;
			}
			else if( strcmp(value,next->value) == 0 )
			{
				return;
			}
		}

		node = createnode();
		node->value = value;

		if( strcmp(value,last->value) < 0 )
			last->left = node;
		if( strcmp(value,last->value) > 0 )
			last->right = node;

	}

	bal_tree(tree);
}


int find( avl_tree_t *tree, char* value )
{
	int z;
	avl_node_t *current = tree->root;

	while(current && (strcmp(current->value,value)!=0))
	{
		if(strcmp(value,current->value) > 0)
			current = current->right;
		else
			current = current->left;
	}
 if(current && strcmp(current->value,value)==0)
			z = 1;
	else
		z = 0;
	//printf("\nz here is %d \n",z);


	return z;
}


void avl_print_node( avl_node_t *node, char* fmt)
{
	int i = 0;

	if( node->left ) avl_print_node( node->left, fmt);

	printf( fmt, node->value);

	if( node->right ) avl_print_node( node->right, fmt);
}

void avl_print( avl_tree_t *tree, char* fmt)
{
	avl_print_node( tree->root, fmt);
}

main()
{
	avl_tree_t *tree = NULL;
	int i = 0,choice;
	int no_of_words,z;
	char r[20][20],findword[20],word[20];
	char* aux;


	printf("\nEnter the no. of words: ");
	scanf("%d",&no_of_words);
    fflush(stdin);

	tree = create();





	for( i = 0; i < no_of_words; i++ )
    {
        printf("\nEnter Lettered Word %d: ",(i+1));
        gets(r[i]);

		aux = strdup(r[i]);

		insert(tree, aux);

	}

	printf("\n\t\tDictionary Order(Inorder Traversal):\n\t\t\t");
	avl_print( tree , " %s \n\t\t\t");

	do
    {
        printf("\n\t\tEnter choice\n\t\t\t1.Insert\n\t\t\t2.Find\n\t\t\t3.Exit");
        scanf("%d",&choice);
        fflush(stdin);
        switch(choice)
        {
        case 1:
            printf("\nEnter Word: ");
            gets(word);

            aux = strdup(word);
            insert(tree,aux);
            printf("\n\t\tDictionary Order(Inorder Traversal):\n\t\t\t");
            avl_print( tree , "\t\t\t %s \n\t\t\t");
            break;
        case 2:
            printf("\nEnter the word to find: ");
            gets(findword);
            aux = strdup(findword);
            z = find(tree,aux);
            if(z == 1)
                printf("\n%s : Found in Tree",findword);

            else if(z == 0)
                printf("\n%s : No-match Found",findword);
                break;

        }
        }while(choice!=3);

	/*printf("\nEnter the word to find: ");
	gets(findword);
	aux = strdup(findword);
	z = find(tree,aux);



	//printf("\n hii");
	//printf("\n%c",search);
	if(z == 1)
        printf("\n%s : Found in Tree",findword);

    else if(z == 0)
        printf("\n%s : No-match Found",findword);*/

}
