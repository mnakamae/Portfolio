#include "Hashtable.h"

struct node * create_node(int value, char *key){
	struct node *tmp = (struct node *)calloc(1, sizeof(struct node));
	tmp->value = value;
	tmp->key = key;
	return tmp;
}

struct hashtable * create_hashtable(int size){
	struct hashtable *tmp = (struct hashtable *)calloc(1, sizeof(struct hashtable));
	tmp->values = (struct node *)calloc(size, sizeof(struct node));
	tmp->ht_size = size;
	tmp->num_nodes = 0;
	return tmp;
}

float load_factor(struct hashtable *ht){
	return (ht->num_nodes / ht->ht_size);
}

int hash(char *key){
	int i, hashed = 0;
	for(i = 0; i < strlen(key); i++){
		hashed += key[i];
	}
	return hashed;
}

int quadratic(int attempts){
	return (attempts * attempts * 3 + 1);
}

void rehash(struct hashtable *ht){
	int i;
	struct hashtable *new_ht = create_hashtable((ht->ht_size) * 2);
	for(i = 0; i < ht->ht_size; i++){
		if((ht->values)[i] != NULL){
			insert(new_ht, ((ht->values)[i])->value, ((ht->values)[i])->key)
		}
	}
	free(ht);
	ht = new_ht;
}

int insert(struct hashtable *ht, int value, char *key){
	int attempts = 0;
	int index;

	while(1){
		index = (hash(key) + quadratic(attempts)) % ht->ht_size;
		if((ht->values)[index] == NULL){
			(ht->values)[index] = key;
			ht->num_nodes++;
			break;
		}
		attempts++;
	}

	if(load_factor(ht) > 0.7){
		rehash(ht);
	}
	return 0;
}








