#include "LinkedList.h"

/* CREATES A NODE WITH A SPECIFIED KEY VALUE */
struct node * create_node(char *key){
	/* CHECK FOR NULL CHAR */
	if(key == '\0'){
		return NULL;
	}

	struct node *tmp = (struct node *)calloc(1, sizeof(struct node));
	tmp->key = *key;
	tmp->next = NULL;
	return tmp;
}

/* INITIALIZES AN EMPTY LINKED LIST */
struct LinkedList * create_LL(void){
	struct LinkedList *tmp = (struct LinkedList *)calloc(1, sizeof(struct LinkedList));
	tmp->num_nodes = 0;
	tmp->head = NULL;
	return tmp;
}

/* TAKES AN INPUT KEY CHAR AND RETURNS THE INDEX OF THE KEY IF THE KEY EXISTS IN THE LINKED LIST */
/* ELSE THIS FUNCTION WILL RETURN -1 IF THE KEY IS NOT FOUND */
int search_key(struct LinkedList *LL, char key){
	int idx = 0;
	struct node *cur = LL->head;

	/* SEARCHING FOR KEY */
	while(cur != NULL){
		/* KEY NODE FOUND */
		if(cur->key == key){
			return idx;
		}
		/* NOT KEY NODE */
		else{
			idx++;
			cur = cur->next;
		}
	}

	/* KEY NOT FOUND */
	return -1;
}

/* RETURNS THE KEY VALUE AT A SPECIFIED INDEX. IF INDEX OUT OF BOUNDS THEN THIS FUNCTION
RETURNS THE ASCII EQUIVALENT 0 */
char search_index(struct LinkedList *LL, int idx){
	struct node *cur = LL->head;
	int count = 0;
	/* CHECK LIST BOUNDS */
	if(idx > LL->num_nodes){
		return '\0';
	}

	/* MOVE POINTER TO CORRECT SPOT */
	while(count < idx){
		cur = cur->next;
		count++;
	}

	return cur->key;
}

/* TAKES AN INPUT KEY CHAR VALUE AND INSERTS THE NODE AT THE HEAD OF THE LIST */
/* RETURNS A -1 IF INSERTION UNSUCCESSFUL */
int insert_unordered(struct LinkedList *LL, char key){
	struct node *tmp = create_node(&key);

	/* FIRST CHECK TO SEE IF NODE ALREADY EXISTS IN THE LIST */
	if(search_key(LL, key) != -1){
		return -1;
	}

	/* LINKED LIST IS EMPTY */
	if(LL->head == NULL){
		LL->head = tmp;
	}

	/* INSERTING TO A NONEMPTY LINKED LIST */
	else{
		tmp->next = LL->head;
		LL->head = tmp;
	}

	LL->num_nodes++;
	return 0;
}

/* TAKES AN INPUT KEY CHAR VALUE AND INSERTS THE NODE INTO THE LIST LEXOGRAPHICALLY */
/* RETURNS A -1 IF INSERTION UNSUCCESSFUL */
int insert_ordered(struct LinkedList *LL, char key){
	int inserted = 0;
	struct node *tmp = create_node(&key);
	struct node *cur = LL->head;
	struct node *prev = NULL;

	/* EMPTY LINKED LIST */
	if(LL->num_nodes == 0){
		LL->head = tmp;
		LL->num_nodes++;
		return 0;
	}

	/* NONEMPTY LINKED LIST */
	while(!inserted){
		/* NODE POINTING TO CORRECT PLACE FOR INSERTION */
		if(cur == NULL || tmp->key < cur->key){
			if(prev != NULL){
				prev->next = tmp;
			}
			else{
				LL->head = tmp;
			}
			tmp->next = cur;
			LL->num_nodes++;
			return 0;
		}

		/* CANNOT HAVE NODES OF THE SAME VALUE */
		else if(tmp->key == cur->key){
			return -1;
		}

		/* CONTINUE WALKING THROUGHT THE LINKED LIST */
		else{
			prev = cur;
			cur = cur->next;
		}
	}

	return -1;
}

/* REMOVES THE HEAD NODE IN THE LINKED LIST */
int remove_head(struct LinkedList *LL){
	char key = '\0';
	struct node *tmp = NULL;

	/* NULL CHECK */
	if(LL->head == NULL){
		return 0;
	}
	else{
		tmp = LL->head;
		LL->head = LL->head->next;
		key = tmp->key;
		free(tmp);
		LL->num_nodes--;
		return 0;
	}
}

/* REMOVES A SPECIFIED KEY VALUE FROM THE LINKED LIST */
int remove_node(struct LinkedList *LL, char key){
	int found = 0;
	struct node *cur = LL->head;
	struct node *prev = NULL;

	while(found == 0){
		/* NULL CHECK */
		if(cur == NULL){
			return 0;
		}
		/* NODE FOUND */
		else if(cur->key == key){
			if(prev != NULL){
				prev->next = cur->next;
			}
			free(cur);
			LL->num_nodes--;
			return 0;
		}
		/* CONTINUE SEARCHING FOR NODE */
		else{
			prev = cur;
			cur = cur->next;
		}
	}

	return -1;
}

/* TAKES AN INPUT LINKED LIST AND REVERSES THE ORDER OF ITS CONTENTS IN PLACE */
/* NOTE THIS WILL MAKE THE LINKED LIST NON LEXOGRAPHIC */
void reverse(struct LinkedList *LL){
	struct node *cur = LL->head;
	struct node *next = NULL;
	struct node *prev = NULL;

	/* LINKED LIST IS EMPTY OR IS OF LENGTH ONE */
	if(cur == NULL || cur->next == NULL){
		return;
	}

	/* LINKED LIST IS GREATER THAN LENGTH ONE */
	else{
		while(cur != NULL){
			next = cur->next;
			cur->next = prev;
			prev = cur;
			cur = next;
		}
		LL->head = prev;
	}
}

/* PRINTS THE LINKED LIST IN [ KEY ] -> [ KEY ] -> .... -> [ NULL ] FORMAT */
void printLL(struct LinkedList *LL){
	struct node *cur = LL->head;

	while(cur != NULL){
		printf("[ %c ] -> ", cur->key);
		cur = cur->next;
	}
	printf("[ NULL ]\n");
}

/* FREES NODES IN THE LINKED LIST */
void free_LL(struct LinkedList *LL){
	struct node *cur = LL->head;
	struct node *next = NULL;

	while(cur != NULL){
		next = cur->next;
		free(cur);
		cur = next;
	}

	free(LL);
}

/* SETS EACH CHAR IN THE ARRAY TO A NULL CHAR*/
void clear_string(char string[256]){
	int i;
	for(i = 0; i < 256; i++){
		string[i] = '\0';
	}
}















