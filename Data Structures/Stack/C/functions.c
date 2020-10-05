#include "Stack.h"

struct stack * create_stack(int max){
	struct stack *tmp = (struct stack *)calloc(1, sizeof(struct stack));
	tmp->size = max;
	tmp->top = 0;
	return tmp;
}

int is_empty(struct stack *st){
	if(st->top == 0){
		return 1;
	}
	return 0;
}

int is_full(struct stack *st){
	if(st->top == st->max){
		return 1;
	}
	return 0;
}

int push(struct stack *st, int value){
	if(is_full(st)){
		return -1;
	}
	else{
		(st->heap)[st->top] = value;
		return 0;
	}
}

int pop(struct stack *st){
	if(is_empty(st)){
		return void;
	}
	(st->top)--;
	return (st->heap)[st->top + 1];
}