#include "BST.h"

int main(int argc, char *argv[]){
	struct bst *tree = create_bst();
	char cmd[512] = "\0";
	char arg[512] = "\0";

	printf("List of Commands:\n\tinsert\n\tfollow_path\n");

	while(1){
		printf("& ");
		scanf("%[^\n]%*c", cmd);

		if(strcmp(cmd, "insert") == 0){
			printf("key: ");
			scanf("%[^\n]%*c", arg);
			if(insert(tree, atoi(arg)) == -1){
				printf("error: duplicate key\n");
			}
		}
		else if(strcmp(cmd, "follow_path") == 0){
			printf("path: ");
			scanf("%[^\n]%*c", arg);
			if(follow_path(tree, arg) == -1){
				printf("error: invalid path\n");
			}
		}
		else if(strcmp(cmd, "EXIT") == 0){
			printf("exiting . . . \n");
			break;
		}
		else{
			printf("error: invalid command\n");
		}

		clear_string(arg);
		clear_string(cmd);
	}
	/*
	insert(tree, 5);
	printf("INSERTED ROOT\n\n");

	insert(tree, 3);
	printf("INSERTED LEFT\n\n");
	insert(tree, 4);
	printf("INSERTED LEFT RIGHT\n\n");
	insert(tree, 2);
	printf("INSERTED LEFT LEFT\n\n");

	insert(tree, 7);
	printf("INSERTED RIGHT\n\n");
	insert(tree, 6);
	printf("INSERTED RIGHT LEFT\n\n");
	insert(tree, 8);
	printf("INSERTED RIGHT RIGHT\n\n");
	
	printf("ROOT:\t%d [ %d %d ]\n", tree->root->key, tree->root->left->key, tree->root->right->key);
	printf("LEFT:\t%d [ %d %d ]\n", tree->root->left->key, tree->root->left->left->key, tree->root->left->right->key);
	printf("RIGHT:\t%d [ %d %d ]\n", tree->root->right->key, tree->root->right->left->key, tree->root->right->right->key);
	*/
	return 0;
}

