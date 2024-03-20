/* 
 * Author: John (Jack) Edwards - edwarddn@bc.edu 
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prog1.h"
#include "slp.h"
#include "util.h"

typedef struct Table_ * Table;
typedef struct IntAndTable_ * IntAndTable;

struct Table_ {
    string id;
    int value;
    Table tail;
};

/* Altered format slightly so IntAndTable acts more like Table in that 
it's a reference to the structure IntAndTable. Changed it because
I was having type errors I could not figure out and I felt this 
created more uniformity. It did require me to make a constructor 
and be careful about referencing table values */ 

struct IntAndTable_ {
    int i;
    Table t;
};

Table make_Table(string id, int value, Table tail);
IntAndTable make_IntAndTable(int i, Table t);
Table update(Table table, string id, int value);
int lookup(Table table, string key);
int max_args_stm(AStm stm);
int max(int a, int b);
int max_args_exp(AExp exp);
int max_args_exp_list(AExpList exps);
int element_count(AExpList exps);
void interp(AStm program);
Table interp_stm(AStm stm, Table table);
IntAndTable interp_exp(AExp exp, Table table);
IntAndTable print_exp_list(AExpList exps, Table table);

Table make_Table(string id, int value, Table tail) {
    Table t = malloc_checked(sizeof(struct Table_));
    t->id = id;
    t->value = value;
    t->tail = tail;
    return t;
}

IntAndTable make_IntAndTable(int i, Table t) {
    IntAndTable it = malloc_checked(sizeof(struct IntAndTable_));
    it->i = i;
    it->t = t;
    return it;
}

Table update(Table table, string id, int value) {
    return make_Table(id, value, table);
}

int lookup(Table table, string key) { 
    //base case: check if the node's key matches
    if (strcmp(table->id, key) == 0) {
        return table->value;
    }

    //other base case: we're at the end (i.e. does not exist in our list)
    if (table->tail == NULL) {
        fprintf(stderr, "Error: could not find %s in table.\n", key);
        exit(EXIT_FAILURE);
    }
     
    return lookup(table->tail, key);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

/*Counts the number of exps in an explist, recursively. Necessary because 
expressions can be eseq, which have statements that can themselves
be print statements. Such statements must be checked for the purpose of 
maxargs, but do not count towards the # args in the original print statement */

int element_count(AExpList exps) {
    if (exps->kind == A_LAST_EXP_LIST) {
        return 1;
    }
    return 1 + element_count(exps->u.pair.tail);
}
        
int max_args_stm(AStm program) { 
    //redundant with max_args as the whole program is a statement. Preserved name.
    switch (program->kind) {
    //postorder traversal
        case A_COMPOUND_STM : {
            int a = max_args_stm(program->u.compound.stm1);
            int b = max_args_stm(program->u.compound.stm2);
            return max(a, b);
        }

        case A_ASSIGN_STM : 
            return max_args_exp(program->u.assign.exp);
        
        case A_PRINT_STM : {
            int my_args = element_count(program->u.print.exps);
            int maybe_sub_prints = max_args_exp_list(program->u.print.exps);
            return max(my_args, maybe_sub_prints);
        }

        default : {
            perror("Default case max_args_stm.");
            exit(EXIT_FAILURE);
        }
    }
}

int max_args_exp(AExp exp) {   
    switch (exp->kind) {       
        case A_ID_EXP : 
            return 0;

        case A_NUM_EXP :
            return 0;

        case A_OP_EXP : {
            int a = max_args_exp(exp->u.op.left);
            int b = max_args_exp(exp->u.op.right);
            return max(a, b); 
        }
        
        case A_ESEQ_EXP : {
            int a = max_args_stm(exp->u.eseq.stm);
            int b = max_args_exp(exp->u.eseq.exp);
            return max(a, b);
        } 

        default: {
            perror("Default case max_args_exp.");
            exit(EXIT_FAILURE);
        }
    }
}

int max_args_exp_list(AExpList exps) {    
    switch (exps->kind) {

        case A_PAIR_EXP_LIST : {
            int a = max_args_exp(exps->u.pair.head);
            int b = max_args_exp_list(exps->u.pair.tail);
            return max(a, b);
        }

        case A_LAST_EXP_LIST : 
            return max_args_exp(exps->u.last);

        default : {
            perror("Default case max_args_exp_list.");
            exit(EXIT_FAILURE);
        }
    }
}

void interp(AStm program) {
    interp_stm(program, NULL);
}

Table interp_stm(AStm stm, Table table) {   
    IntAndTable it;
    
    switch (stm->kind) {

        case A_COMPOUND_STM : {
            table = interp_stm(stm->u.compound.stm1, table);
            table = interp_stm(stm->u.compound.stm2, table);
            return table;
        }
 
        case A_ASSIGN_STM : { 
            it = interp_exp(stm->u.assign.exp, table);
            table = update(it->t, stm->u.assign.id, it->i);
            return table;
        }

        case A_PRINT_STM : {
            it = print_exp_list(stm->u.print.exps, table);
            return it->t;
        }

        default : {
            perror("Default in interp_stm");
            exit(EXIT_FAILURE);
        }
    }
}

IntAndTable interp_exp(AExp exp, Table table) {
    switch (exp->kind) {
    
        case A_ID_EXP : 
            return make_IntAndTable(lookup(table, exp->u.id), table);

        case A_NUM_EXP :
            return make_IntAndTable(exp->u.num, table);
        
        case A_OP_EXP : {
            int l;
            int r;
            IntAndTable temp;

            temp = interp_exp(exp->u.op.left, table);
            l = temp->i;
            temp = interp_exp(exp->u.op.right, temp->t);
            r = temp->i;

            int ret;
            switch (exp->u.op.oper) {
                case A_PLUS :
                    ret = l + r;
                    break;
                case A_MINUS :
                    ret = l - r;
                    break;
                case A_TIMES :
                    ret = l * r;
                    break;
                case A_DIV :
                    ret = l / r;
                    break;
                default : {
                    perror("Error with Binop.");
                    exit(EXIT_FAILURE);
                }
            }

            return make_IntAndTable(ret, temp->t);
        }

        case A_ESEQ_EXP : { 
            table = interp_stm(exp->u.eseq.stm, table);
            return interp_exp(exp->u.eseq.exp, table);                          
        }

        default : {
            perror("Default on interp_exp");
            exit(EXIT_FAILURE);
        }
    }
}

//slight signature change so this outputs an IntAndTable
IntAndTable print_exp_list(AExpList exps, Table table) {
    IntAndTable it;

    switch(exps->kind) {

        case A_PAIR_EXP_LIST : {
            it = interp_exp(exps->u.pair.head, table);
            printf("%d ", it->i);
            return print_exp_list(exps->u.pair.tail, it->t);
        }
                  
        case A_LAST_EXP_LIST : {
            it = interp_exp(exps->u.last, table);
            printf("%d\n", it->i);
            return it;
        }

        default : {
            perror("Default case print_exp_list");
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    AStm program = prog();
    int count = max_args_stm(program); 
    printf("Maximum number of arguments to any print statement: %d\n", count);  
    interp(program);
    return EXIT_SUCCESS;
}

