/*CS251 Interpreter part Lambda/Function
By: Cal Ellowitz, Tenzin Rigden, Dang Quang Tran.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "talloc.h"
#include "linkedlist.h"
#include "interpreter.h"
#include "parser.h"
#include "primitives.h"

// adds an arbitrary number of integers or reals
// returning a real
Value *primitivePlus(Value *args){
    double sum = 0;
    while (args->type != NULL_TYPE){
        if (car(args)->type == DOUBLE_TYPE){
            sum += car(args)->d;
        }
        
        else if (car(args)->type == INT_TYPE){
            sum += car(args)->i;
        }
        
        else{
            printf("Evaluation Error: not a number\n");
            texit(1);
        }
        
        args = cdr(args);
    }
    Value *returnVal = talloc(sizeof(Value));
    returnVal->type = DOUBLE_TYPE;
    returnVal->d = sum;
    return returnVal;
}

Value *primitiveMult(Value *args){
    //Creates a double that is value 1 and multiplies everything to it and returns that.
    double product = 1;
    if (length(args) < 2) {
        printf("need at least two values\n");
        texit(1);
    }
    while (args->type != NULL_TYPE){
        if (car(args)->type == DOUBLE_TYPE){
            product *= car(args)->d;
        }
        
        else if (car(args)->type == INT_TYPE){
            product *= car(args)->i;
        }
        
        else{
            printf("Evaluation Error: not a number\n");
            texit(1);
        }
        
        args = cdr(args);
    }
    Value *returnVal = talloc(sizeof(Value));
    returnVal->type = DOUBLE_TYPE;
    returnVal->d = product;
    return returnVal;
}

Value *primitiveMinus(Value *args){
    //There is no base case for minus like there is for mult, so instead, I take the first value and save it as the difference value and subtract everything from it.
    if (length(args) != 2){
        printf("Need exactly two arguments\n");
        texit(1);
    }
    double difference;
    if (car(args)->type == DOUBLE_TYPE){
        difference = car(args)->d;
    }   
    else if (car(args)->type == INT_TYPE){
        difference = car(args)->i;
    }
    else{
        printf("Evaluation Error: not a number\n");
        texit(1);
    }
    args = cdr(args);
    if (car(args)->type == DOUBLE_TYPE){
        difference -= car(args)->d;
    }
    
    else if (car(args)->type == INT_TYPE){
        difference -= car(args)->i;
    }
    
    else{
        printf("Evaluation Error: not a number\n");
        texit(1);
    }
    Value *returnVal = talloc(sizeof(Value));
    returnVal->type = DOUBLE_TYPE;
    returnVal->d = difference;
    return returnVal;
}

Value *primitiveDivide(Value *args){
    //Also no base case for divide, so I take the first value and then divide everything by it.
    if (length(args) != 2){
        printf("Need exactly two arguments\n");
        texit(1);
    }
    double quotient;
    if (car(args)->type == DOUBLE_TYPE){
        quotient = car(args)->d;
    }   
    else if (car(args)->type == INT_TYPE){
        quotient = car(args)->i;
    }
    else{
        printf("Evaluation Error: not a number\n");
        texit(1);
    }
    args = cdr(args);
    if (car(args)->type == DOUBLE_TYPE){
        quotient /= car(args)->d;
    }
    else if (car(args)->type == INT_TYPE){
        quotient /= car(args)->i;
    }   
    else{
        printf("Evaluation Error: not a number\n");
        texit(1);
    }
    Value *returnVal = talloc(sizeof(Value));
    returnVal->type = DOUBLE_TYPE;
    returnVal->d = quotient;
    return returnVal;
}
// checks for an empty list / null value
// returns true if null, else false
Value* primitiveModulo(Value *args){
    //Takes in two arguments and we can assume both arguements must be ints. Then does the mod between the two. If either value is negative, then we 
    //must add the remainder to the thing we mod by because of the difference in how mod is defined by C and racket.
    if (length(args) != 2){
        printf("Need exactly two arguments\n");
        texit(1);
    }
    if (car(args)->type != INT_TYPE || car(cdr(args))->type != INT_TYPE){
        printf("Both arguments must be ints\n");
        texit(1);
    }
    int remainder;
    remainder = car(args)->i % car(cdr(args))->i;
    if (car(args)->i < 0 || car(cdr(args))->i < 0){
        remainder = remainder + car(cdr(args))->i; 
    }
    Value *returnVal = talloc(sizeof(Value));
    returnVal->type = INT_TYPE;
    //remainder = 15%(-4);
    //printf("here\n");
    returnVal->i = remainder;
    return returnVal;


}


Value *primitiveNull(Value *args){
    //Simply checks the type of the value * object being passed and returns true if it is null.
    if (length(args) != 1){
        printf("Evaluation Error: the number of arguments is not 1\n");
        texit(1);
    }
    Value *boolean = talloc(sizeof(Value));
    boolean->type = BOOL_TYPE;
    if (car(args)->type == NULL_TYPE){
        boolean->i = 1;
    }  
    else{
        boolean->i = 0;
    }
    return boolean;
}
        
// implements racket car function
Value *primitiveCar(Value *args){
    if (length(args) != 1){
        printf("Evaluation Error: the number of arguments is not 1\n");
        texit(1);
    } 
    if (car(args)->type != CONS_TYPE){
        printf("Evaluation Error: the argument is not a list\n");
        texit(1);
    }
    Value *toReturn = talloc(sizeof(Value));
    toReturn = car(car(args));
    return toReturn; 
}

//implements racket cdr function
Value *primitiveCdr(Value *args){
    if (length(args) != 1){
        printf("Evaluation Error: the number of arguments is not 1\n");
        texit(1);
    } 
    if (car(args)->type != CONS_TYPE){
        printf("Evaluation Error: the argument is not a list\n");
        texit(1);
    }
    Value *toReturn = talloc(sizeof(Value));
    toReturn = cdr(car(args));
    return toReturn;
}

//creates a cons cell of the two values
Value *primitiveCons(Value *args){
    if (length(args) != 2){
        printf("Evaluation Error: the number of arguments is not 2\n");
        texit(1);
    }
    Value *toReturn = talloc(sizeof(Value));
    toReturn = cons(car(args), car(cdr(args)));
    return toReturn;
}

Value *primitiveLess(Value *args){
    //Creates a bool type that is default true. It then creates a double type called check and that is the first value being compared to.

    if (length(args) != 2){
        printf("Evaluation Error: the number of arguments is not 2\n");
        texit(1);
    }
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    result->i = 1;
    double check;
    if (length(args) != 2) {
        printf("Evaluation Error: not a correct number of arguments\n");
        texit(1);
    }
    if (car(args)->type == INT_TYPE) {
        check = car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
        check = car(args)->d;
    } else {
        printf("Evaluation Error: argument not a number\n");
        texit(1);
    }
    args = cdr(args);
    //This then compares check to the second value. If check is greater than or equal to the second val, change bool type to false and return, otherwise return the default true.
    if (car(args)->type == INT_TYPE) {
        if (check >= car(args)->i) {
            result->i = 0;
            return result; 
        }
    } else if (car(args)->type == DOUBLE_TYPE) {
        if (check >= car(args)->d) {
            result->i = 0;
            return result;
        }
    } else {
        printf("Evaluation Error: argument not a number\n");
        texit(1);
    }
    return result;
}

Value *primitiveGreater(Value *args){
    //Exact same as less than but the comparison between the first and second value is changed to less than or equal to instead.
    if (length(args) != 2){
        printf("Evaluation Error: the number of arguments is not 2\n");
        texit(1);
    }
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    result->i = 1;
    double check;
    if (length(args) != 2) {
        printf("Evaluation Error: not a correct number of arguments\n");
        texit(1);
    }
    if (car(args)->type == INT_TYPE) {
        check = car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
        check = car(args)->d;
    } else {
        printf("Evaluation Error: argument not a number\n");
        texit(1);
    }
    args = cdr(args);
    if (car(args)->type == INT_TYPE) {
        if (check <= car(args)->i) {
            result->i = 0;
            return result; 
        }
    } else if (car(args)->type == DOUBLE_TYPE) {
        if (check <= car(args)->d) {
            result->i = 0;
            return result;
        }
    } else {
        printf("Evaluation Error: argument not a number\n");
        texit(1);
    }
    return result;
}


//checks numerical equality
Value *primitiveEqual(Value *args){
    //like before, create a double type called check that is the first value then compare it to all the other arguments in the while loop.
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    result->i = 1;
    double check;
    if (length(args) < 2) {
        printf("Evaluation Error: not enough arguments\n");
        texit(1);
    }
    if (car(args)->type == INT_TYPE) {
        check = car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
        check = car(args)->d;
    } else {
        printf("Evaluation Error: argument not a number\n");
        texit(1);
    }
    args = cdr(args);
    while (args->type != NULL_TYPE) {
        if (car(args)->type == INT_TYPE) {
            if (check != car(args)->i) {
                result->i = 0;
                return result; 
            }
        } else if (car(args)->type == DOUBLE_TYPE) {
            if (check != car(args)->d) {
                result->i = 0;
                return result;
            }
        } else {
            printf("Evaluation Error: argument not a number\n");
            texit(1);
        }
        args = cdr(args);
    } 
    return result;
}

