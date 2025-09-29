#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<ctype.h>

int value_stack[100];
char operator_stack[100];
int value_top = -1;
int operator_top = -1;

bool Is_Operator(char character) {
    if (character == '+' || character == '-' || character == '*' || character == '/') {
        return true;
    }
    else {
        return false;
    }
}

int Check_Precedence(char character) {
    if (character == '+' || character == '-') {
        return 1;
    } 
    else if (character == '*' || character == '/') {
        return 2;
    }
    return 0;
}

int Perform_Operation(int operand1, int operand2, char character) {
    switch (character){
        case '+':
            return operand1+operand2;
        case '-':
            return operand1-operand2;
        case '*':
            return operand1*operand2;
        case '/':
            if (operand2 == 0){
                printf("Division by zero");
                exit(0);
            }
            else {
                return operand1/operand2;
            }
    }
}

void Push_Value(int value){
    if (value_top < 99) {
        value_stack[++value_top] = value;
    }
    else {
        printf("Invalid Expression");
        exit(0);
    }
}

int Pop_Value() {
    if (value_top >= 0){
        return value_stack[value_top--];
    }
    else {
        printf("Invalid Expression");
        exit(0);
    }
}   

void Push_Operator(char operator1) {
    if (operator_top < 99) {
        operator_stack[++operator_top] = operator1;
    }
    else {
        printf("Invalid Expression");
        exit(0);
    }
}

char Pop_Operator() {
    if (operator_top >= 0) {
        return operator_stack[operator_top--];
    }
    else {
        printf("Invalid Expression");
        exit(0);
    }
}

char Peek_Operator() {
    return operator_stack[operator_top];
}

int Evaluate_Expression(char *expression) {
  int i=0; 
  int lasttaken = 0;
  while (expression[i] != '\0') {
      char character = expression[i];
      if (isspace(character)) {
          i++;
          continue;
      }
      else if (isdigit(character)) {
          int number = 0;
          while (expression[i] != '\0' && isdigit(expression[i])) {
              number = number * 10 + (expression[i] - '0');
              i++;
          }
          Push_Value(number);
          lasttaken = 1;
          continue;
      }
      else if (Is_Operator(character)) {
          if (lasttaken != 1) {
              printf("Invalid Expression");
              exit(0);
          }
          while (operator_top >= 0 && Check_Precedence(Peek_Operator()) >= Check_Precedence(character)) {
              int operand2 = Pop_Value();
              int operand1 = Pop_Value();
              char operator1 = Pop_Operator();
              int result = Perform_Operation(operand1, operand2, operator1);
              Push_Value(result);
          }
          Push_Operator(character);
      }
      else {
          printf("Invalid Expression");
          exit(0);
      }
      i++;
  }
  while (operator_top >= 0) {
      int operand2 = Pop_Value();
      int operand1 = Pop_Value();
      char operator1 = Pop_Operator();
      int result = Perform_Operation(operand1, operand2, operator1);
      Push_Value(result);
  }
   return Pop_Value(); 
}

int main() {
    char expression[100];
    printf("Enter the expression: ");
    scanf("%[^\n]",expression);
    int result = Evaluate_Expression(expression);
    printf("Result: %d",result);
    return 0;
}
