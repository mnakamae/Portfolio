#include "LinkedList.h"

int main(int argc, char *argv[]){
	struct LinkedList *LL = create_LL();
	char cmd[512] = "\0";
	char arg[512] = "\0";

	printf("List of Commands:\n\tsearch_key\n\tsearch_index\n\t");
	printf("insert_ordered\n\tinsert_unordered\n\tremove_head\n\t");
	printf("remove_node\n\treverse\n\tprintLL\n\n");

	while(1){
		printf("& ");
		scanf("%[^\n]%*c", cmd);

		if(strcmp(cmd, "search_key") == 0){
			printf("key: ");
			scanf("%[^\n]%*c", arg);

			/* CHECK FOR VALID ARGUMENT */
			if(strlen(arg) > 1){
				printf("error: invalid key\n");
			}
			else{
				printf("index: %d\n", search_key(LL, arg[0]));
			}
		}
		else if(strcmp(cmd, "search_index") == 0){
			printf("index: ");
			scanf("%[^\n]%*c", arg);

			/* CHECK FOR VALID ARGUMENT */
			/* NEED TO CHANGE TO ERROR HANDLING FOR ATOI */
			if(strlen(arg) > 1){
				printf("error: invalid index\n");
			}
			else{
				printf("char: %c\n", search_index(LL, atoi(arg)));
			}
		}
		else if(strcmp(cmd, "insert_ordered") == 0){
			printf("key: ");
			scanf("%[^\n]%*c", arg);
			/* CHECK FOR VALID ARGUMENT */
			if(strlen(arg) > 1){
				printf("error: invalid key\n");
			}
			else{
				insert_ordered(LL, arg[0]);
			}
		}
		else if(strcmp(cmd, "insert_unordered") == 0){
			printf("key: ");
			scanf("%[^\n]%*c", arg);

			/* CHECK FOR VALID ARGUMENT */
			if(strlen(arg) > 1){
				printf("error: invalid key\n");
			}
			else{
				insert_unordered(LL, arg[0]);
			}
		}
		else if(strcmp(cmd, "remove_head") == 0){
			remove_head(LL);
		}
		else if(strcmp(cmd, "remove_node") == 0){
			printf("key: ");
			scanf("%[^\n]%*c", arg);
			/* CHECK FOR VALID ARGUMENT */
			if(strlen(arg) > 1){
				printf("error: invalid index\n");
			}
			else{
				remove_node(LL, arg[0]);
			}
		}
		else if(strcmp(cmd, "reverse") == 0){
			reverse(LL);
		}
		else if(strcmp(cmd, "printLL") == 0){
			printLL(LL);
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

	free_LL(LL);
	return 0;
}
