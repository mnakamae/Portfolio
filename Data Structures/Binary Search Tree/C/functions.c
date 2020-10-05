#include "BST.h"

/* CREATE A POINTER TO A NODE WITH A SPECIFIED KEY VALUE */
struct node * create_node(int key){
	struct node *tmp = (struct node *)calloc(1, sizeof(struct node));
	tmp->key = key;
	tmp->left = NULL;
	tmp->right = NULL;
	return tmp;
}


/* CREATE A POINTER TO AN EMPTY BINARY SEARCH TREE */
struct bst * create_bst(void){
	struct bst *tree = (struct bst *)calloc(1, sizeof(struct bst));
	return tree;
}

/* HELPER FUNCTION TO SEE IF A BINARY SEARCH TREE CONTAINS A KEY */
int contains_helper(struct node *parent, int key){
	/* NULL CHECK */
	if(parent == NULL){
		return 0;
	}/* BEST CONTAINS THE KEY */
	else if(parent->key == key){
		return 1;
	}/* RECURSE TO THE LEFT BRANCH */
	else if(key < parent->key){
		return contains_helper(parent->left, key);
	}/* RECURSE TO THE RIGHT BRANCH */
	else{
		return contains_helper(parent->right, key);
	}
}

/* RETURNS 0 IF THE BINARY SEARCH TREE DOES NOT CONTAIN THE KEY
AND RETURNS 1 IF THE BINARY SEARCH TREE CONTAINS THE KEY */
int contains(struct bst *tree, int key){
	return contains_helper(tree->root, key);
}

/* HELPER FUNCTION TO INSERT A KEY INTO A BINARY SEARCH TREE */
int insert_helper(struct node **parent, int key){
	/* NULL CHECK */
	if(*parent == NULL){
		*parent = create_node(key);
		return 0;
	}/* INSERTED KEY < PARENT KEY */
	else if(key < (*parent)->key){
		return insert_helper(&((*parent)->left), key);
	}/* INSERTED KEY > PARENT KEY */
	else{
		return insert_helper(&((*parent)->right), key);
	}
}

/* INSERTS A KEY INTO THE BINARY SEARCH TREE ASSUMING THAT KEY DOES
NOT ALREADY EXIST IN THE TREE */
int insert(struct bst *tree, int key){
	/* CHECK IF KEY ALREADY EXISTS */
	if(contains(tree, key) == 1){
		return -1;
	}

	/* IF THE TREE DOES NOT CONTAIN KEY CONTINUE INSERT */
	return insert_helper(&(tree->root), key);
}

/* SETS EACH CHAR IN THE ARRAY TO A NULL CHAR*/
void clear_string(char string[256]){
	int i;
	for(i = 0; i < 256; i++){
		string[i] = '\0';
	}
}

/* FOLLOWS A GIVEN INPUT PATH BASED ON 0'S (LEFT) AND 1'S (RIGHT) THEN PRINTS
THE KEY ASSOCIATED WITH THE PATH*/
int follow_path(struct bst *tree, char path[256]){
	int i;
	struct node *tmp = tree->root;
	for(i = 0; i < strlen(path); i++){
		/* INVALID PATH */
		if(tmp == NULL){
			return -1;
		}

		/* TRAVERSE LEFT */
		if(path[i] == '0'){
			tmp = tmp->left;
		}/* TRAVERSE RIGHT */
		else if(path[i] == '1'){
			tmp = tmp->right;
		}
		else{
			return -1;
		}
	}
	printf("key: %d\n", tmp->key);
	return 0;
}











