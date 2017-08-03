#include<stdio.h>
#include <stdlib.h>
#define ROWS 13
#define COLS 13
typedef struct {
	char expression[20];
}node;  // structure for nodes in grid




typedef struct {
	node *nodes[2];
	char op;
}operation;	  //structure for evaluation

node sheet[ROWS][COLS];

char operators[] = { '+', '-', '*', '/' };		//basic operations that are done 

/*	 display m x n grid*/
void display() {												
	for (int j = 0; j < COLS; j++) {
				printf("______");
			}
			printf("__\n");
			for (int i = 0; i < ROWS; i++) {
				printf("| ");
		
				for (int j = 0; j < COLS; j++) {
					if (strcmp(sheet[i][j].expression, "") == 0)
						printf("     |");
					else
						printf("%4d |", eval(&sheet[i][j]));
		
				}
				printf("\n");
				for (int j = 0; j < COLS; j++) {
					printf("______");
				}
				printf("__\n");
		
			}
	
}		                     
	

/* module to check does the given string is a value or expression*/
int is_value(char* str) {
	int len = strlen(str);
	int i = 0;
	if (str[0] == '-')
		i++;
	for (; i < len; i++) {
		if (!(str[i] >= '0' && str[i] <= '9')) {
			return 0;
		}
	}
	return 1;
}

/* parser that parses the value in to int from string and returns*/
int parse_value(char* str) {
	int result = 0;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		result *= 10;
		result += str[i] - '0';
	}
	return result;
}


/*create a node with expression storing in it*/
node* create_node(char* name) {
	node* new_node = (node*)malloc(sizeof(node));
	strcpy(new_node->expression, name);
}

/*returns positions of a given character in a given string*/
int strpos(char* str, char ch) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (ch == str[i])
			return i;
	}
	return -1;

}


/*returns the substring position as a pointer pointing it in the given string*/

char* strstrr(char *str, char *substr)
{
	while (*str)
	{
		char *Begin = str;
		char *pattern = substr;

		// If first character of sub string match, check for whole string
		while (*str && *pattern && *str == *pattern)
		{
			str++;
			pattern++;
		}
		// If complete sub string match, return starting address 
		if (!*pattern)
			return Begin;

		str = Begin + 1;	// Increament main string 
	}
	return NULL;
}


/*returns the operation_code for particular operator*/
int get_op(char* str, char *op, char *result) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (op[0] == str[i]){
			*result = op[0];
			return i;
		}
		if (op[1] == str[i]) {
			*result = op[1];
			return i;
		}
		if (op[2] == str[i]) {
			*result = op[2];
			return i;
		}
		if (op[3] == str[i]) {
			*result = op[3];
			return i;
		}

	}
	return -1;

}


/*parses the string A1 and returns row and column value*/
int *get_dimensions(char *str, int start, int end) {

	int *dimensions = (int*)calloc(2, sizeof(int));
	dimensions[1] = str[start] - 65;
	char a[5];
	int i = 0;
	start++;
	while (start <= end) {
		a[i] = str[start];
		i++; start++;
	}
	a[i] = '\0';
	dimensions[0] = atoi(a) - 1;
	return dimensions;
}


/*module for parsing the expression*/
operation* parse_expression(char* expression) { /////////////////////////////
	operation *oper = (operation*)malloc(sizeof(operation));
		char op;
		int pos = get_op(expression, operators, &op);
		if (pos == -1) {
			printf("Invalid expression\n");
		}
		else {
			oper->op = op;
			int i;
			int *dimensions1 = NULL;
			int *dimensions2 = NULL;
			char *temp = (char*)malloc((pos)* sizeof(char));
			for (i = 0; i < pos; i++) {
				temp[i] = expression[i];
			}
			temp[i] = '\0';
			if (is_value(temp)) {
				oper->nodes[0] = (node*)malloc(sizeof(node));
				strcpy(oper->nodes[0]->expression, temp);
			}
			else {
				if (get_op(temp, operators, &op) != -1) {
					oper->nodes[0] = create_node(temp);
				}
				else {
					dimensions1 = get_dimensions(expression, 0, pos - 1);
					oper->nodes[0] = &(sheet[dimensions1[0]][dimensions1[1]]);
				}
			}
			for (i = pos + 1; i < strlen(expression); i++) {
				temp[i - pos - 1] = expression[i];
			}
			temp[i - (pos + 1)] = '\0';
			if (is_value(temp)) {
				oper->nodes[1] = (node*)malloc(sizeof(node));
				strcpy(oper->nodes[1]->expression, temp);
			}
			else {
				if (get_op(temp, operators, &op) != -1) {
					oper->nodes[1] = (node*)malloc(sizeof(node));
					strcpy(oper->nodes[1]->expression, temp);
				}
				else {
					dimensions2 = get_dimensions(expression, pos + 1, strlen(expression) - 1);
					oper->nodes[1] = &(sheet[dimensions2[0]][dimensions2[1]]);
				}
	
			}
		}
		return oper;
}

/*module to calculare max */
int get_max(char *str, int *length,int  is_min){
	int start = 0, end = 0;
	char *str1 = str;
	while (*str1 != '('){
		str1++;
		start++; end++;
	}
	str1++; start++; end++;
	while (*str1 != ':'){
		end++;
		str1++;
	}
	int *dimensions1 = NULL;
	dimensions1 = get_dimensions(str, start, end);
	start = end + 1;
	while (*str1 != ')'){
		end++;
		str1++;
	}
	end--;
	int *dimensions2 = NULL;
	dimensions2 = get_dimensions(str, start, end);
	int result=0,k = 0;
	if (dimensions1[0] > dimensions2[0])
		return NULL;
	if (!is_min){
		 if (dimensions1[0] == dimensions2[0]){
			*length = dimensions2[0] - dimensions1[0] + 1;
			for (int i = dimensions1[1]; i <= dimensions2[1]; i++){
				k = eval(&sheet[dimensions1[0]][i]);
				if (result < k)
					result = k;
			}
		}
		else{
			*length = dimensions2[0] - dimensions1[0] + 1;
			for (int i = dimensions1[0]; i <= dimensions2[0]; i++){
				k = eval(&sheet[i][dimensions1[1]]);
				if (result < k)
					result = k;
			}
		}
	}
	else{
		if (dimensions1[0] == dimensions2[0]){
			result = INT_MIN;
			*length = dimensions2[0] - dimensions1[0] + 1;
			for (int i = dimensions1[1]; i <= dimensions2[1]; i++){
				k = eval(&sheet[dimensions1[0]][i]);
				if (result > k)
					result = k;
			}
		}
		else{
			*length = dimensions2[0] - dimensions1[0] + 1;
			result = INT_MAX;
			for (int i = dimensions1[0]; i <= dimensions2[0]; i++){
				k = eval(&sheet[i][dimensions1[1]]);
				if (result > k)
					result = k;
			}
		}
	}
	return result;
}

/*module to calculate sum*/
int get_sum_result(char *str,int *length){
	int start = 0, end = 0;
	char *str1 = str;
	while (*str1 != '('){
	str1++;
		start++; end++;
	}
	str1++; start++; end++;
	while (*str1 != ':'){
		end++;
		str1++;
	}
	int *dimensions1 = NULL;
	dimensions1 = get_dimensions(str, start, end);
	start = end + 1;
	while (*str1 != ')'){
		end++;
		str1++;
	}
	end--; 
	int *dimensions2 = NULL;
	dimensions2 = get_dimensions(str, start, end);
	int result=0;
	if (dimensions1[0] > dimensions2[0])
		return NULL;
	else if (dimensions1[0] == dimensions2[0]){
		*length = dimensions2[0] - dimensions1[0] + 1;
		for (int i = dimensions1[1]; i <= dimensions2[1]; i++){
			result += eval(&sheet[dimensions1[0]][i]);
		}
	}
	else{
		*length = dimensions2[0] - dimensions1[0] + 1;

		for (int i = dimensions1[0]; i <= dimensions2[0]; i++){
			result += eval(&sheet[i][dimensions1[1]]);
		}
	}
	return result;
}


/*module for evaluating expression*/
int eval(node *n) {
	if (is_value(n->expression))
		return parse_value(n->expression);
	else if (strstrr(n->expression, "SUM") || strstrr(n->expression, "AVG")) {
		int length = 0;
		int result = get_sum_result(n->expression,&length);
		if (result == NULL){
			return 0;
		}
		else{
			if (strstrr(n->expression, "AVG"))
				return result / length;
			else return result;
		}

	}
			else if (strstrr(n->expression, "MAX")) {
				int length = 0;
				int result = get_max(n->expression, &length,0);
				if (result == NULL){
					return 0;
				}
				else{	
				 return result;
				}
			}
			else if (strstrr(n->expression, "MIN")) {
				int length = 0;
				int result = get_max(n->expression, &length, 1);
				if (result == NULL){
					return 0;
				}
				else{
					return result;
				}
			}
	else {
		operation *oper;
		oper = parse_expression(n->expression);
		while (1)
		{
			switch (oper->op) {
			case '+':
				return eval(oper->nodes[0]) + eval(oper->nodes[1]);
			case '-':
				return eval(oper->nodes[0]) - eval(oper->nodes[1]);
				break;
			case '*':
				return eval(oper->nodes[0]) * eval(oper->nodes[1]);
				break;
			case '/':
				return eval(oper->nodes[0]) / eval(oper->nodes[1]);
				break;
			default:
				break;
			}

		}
	}
}


/*returns single intruction from file*/
char *get_instruction(FILE *fp){
	char * instr = (char*)calloc(20, sizeof(char));
	if (fgets(instr, 100, fp) == NULL)
		return NULL;
	instr[strlen(instr) - 1] = '\0';
	return instr;
}

char *get_from_instruction(char *str, int start, int end){
	char *temp = (char*)calloc(20, sizeof(char));
	for (int i = 0; i <= end; i++)
		temp[i] = str[i];
	temp[end + 1] = '\0';
	return temp;
}

int main() {
	FILE *fp = fopen("excel_save.txt", "r");

	char* expr = get_instruction(fp);
	display();
	while (expr!=NULL) {
		
		printf("\nEnter 'exit' to stop\n\n> \n");
		fflush(stdin);
		
			int pos = strpos(expr, ' ');
			char *instr = get_from_instruction(expr, 0, pos - 1);
			expr = expr + pos + 1;
			 pos = strpos(expr, ' ');
			int *dimensions = get_dimensions(expr, 0, pos - 1);
			if (strcmp(instr, "SET") == 0){
				expr = expr + pos + 1;
				strcpy(sheet[dimensions[0]][dimensions[1]].expression, expr);
			}
			else if (strcmp(instr, "SETF") == 0){
				expr = expr + pos + 1;
				strcpy(sheet[dimensions[0]][dimensions[1]].expression, expr);

			}
			display();
		expr = get_instruction(fp);
	}
	getchar();
	return 0;
}


