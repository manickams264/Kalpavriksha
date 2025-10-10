#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<ctype.h>

// Stacks for values and operators

int value_stack[100];
char operator_stack[100];

// Stack top pointers

int value_top = -1;
int operator_top = -1;

// Function to check whether a given character is a valid operator

bool Is_Operator(char character) {
    if (character == '+' || character == '-' || character == '*' || character == '/') {
        return true;
    }
    else {
        return false;
    }
}

// Function to return the precedence level of an operator

int Check_Precedence(char character) {
    if (character == '+' || character == '-') {
        return 1;
    } 
    else if (character == '*' || character == '/') {
        return 2;
    }
    return 0;
}

// Function to perform arithmetic operations between two operands

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

// Push a value into the value stack

void Push_Value(int value){
    if (value_top < 99) {
        value_stack[++value_top] = value;
    }
    else {
        printf("Invalid Expression");
        exit(0);
    }
}

// Pop a value from the value stack

int Pop_Value() {
    if (value_top >= 0){
        return value_stack[value_top--];
    }
    else {
        printf("Invalid Expression");
        exit(0);
    }
}   

// Push an operator into the operator stack

void Push_Operator(char operator1) {
    if (operator_top < 99) {
        operator_stack[++operator_top] = operator1;
    }
    else {
        printf("Invalid Expression");
        exit(0);
    }
}

// Pop an operator from the operator stack

char Pop_Operator() {
    if (operator_top >= 0) {
        return operator_stack[operator_top--];
    }
    else {
        printf("Invalid Expression");
        exit(0);
    }
}

// Peek at the top operator of the operator stack

char Peek_Operator() {
    return operator_stack[operator_top];
}

// Function to evaluate the arithmetic expression entered by the user

int Evaluate_Expression(char *expression) {
  int i=0; 
  int last_taken = 0;
  int has_operator = 0;
  
  // Loop through the expression character by character
  while (expression[i] != '\0') {
      char character = expression[i];
      
      // Skip spaces
      if (isspace(character)) {
          i++;
          continue;
      }
      
      // If current character is a digit
      else if (isdigit(character)) {
          
          // Two consecutive numbers without an operator are invalid
          if (last_taken == 1) {
              printf("Invalid Expression");
              exit(0);
          }
          
          // Convert the number from string to integer
          int number = 0;
          while (expression[i] != '\0' && isdigit(expression[i])) {
              number = number * 10 + (expression[i] - '0');
              i++;
          }
          
          // Push the parsed number into the value stack
          Push_Value(number);
          last_taken = 1;
          continue;
      }
      
      // If current character is an operator
      else if (Is_Operator(character)) {
          has_operator = 1;
          
          // Expression should not start or have consecutive operators
          if (last_taken != 1) {
              printf("Invalid Expression");
              exit(0);
          }
          
          // Evaluate previous operators with higher or equal precedence
          while (operator_top >= 0 && Check_Precedence(Peek_Operator()) >= Check_Precedence(character)) {
              int operand2 = Pop_Value();
              int operand1 = Pop_Value();
              char operator1 = Pop_Operator();
              int result = Perform_Operation(operand1, operand2, operator1);
              Push_Value(result);
          }
          
          // Push the current operator onto the stack
          Push_Operator(character);
          last_taken = 2;
          
      // If any invalid character is encountered
      }
      else {
          printf("Invalid Expression");
          exit(0);
      }
      i++;
  }
  
  // Expression should have at least one operator
  if (has_operator == 0) {
      printf("Invalid Expression");
      exit(0);
  }
  
  // Perform all remaining operations in the stack
  while (operator_top >= 0) {
      int operand2 = Pop_Value();
      int operand1 = Pop_Value();
      char operator1 = Pop_Operator();
      int result = Perform_Operation(operand1, operand2, operator1);
      Push_Value(result);
  }
  
  // Return the final result from the value stack
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