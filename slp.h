#pragma once

#include "util.h"

typedef struct AStm_ * AStm;
typedef struct AExp_ * AExp;
typedef struct AExpList_ * AExpList;
typedef enum {A_PLUS, A_MINUS, A_TIMES, A_DIV} ABinop;

struct AStm_ {
    enum {A_COMPOUND_STM, A_ASSIGN_STM, A_PRINT_STM} kind;
    union {
        struct {AStm stm1, stm2;} compound;
        struct {string id; AExp exp;} assign;
        struct {AExpList exps;} print;
    } u;
};

AStm make_ACompoundStm(AStm stm1, AStm stm2);
AStm make_AAssignStm(string id, AExp exp);
AStm make_APrintStm(AExpList exps);

struct AExp_ {
    enum {A_ID_EXP, A_NUM_EXP, A_OP_EXP, A_ESEQ_EXP} kind;
    union {
        string id;
        int num;
        struct {AExp left; ABinop oper; AExp right;} op;
        struct {AStm stm; AExp exp;} eseq;
    } u;
};

AExp make_AIdExp(string id);
AExp make_ANumExp(int num);
AExp make_AOpExp(AExp left, ABinop oper, AExp right);
AExp make_AEseqExp(AStm stm, AExp exp);

struct AExpList_ {
    enum {A_PAIR_EXP_LIST, A_LAST_EXP_LIST} kind;
    union {
        struct {AExp head; AExpList tail;} pair;
        AExp last;
    } u;
};

AExpList make_APairExpList(AExp head, AExpList tail);
AExpList make_ALastExpList(AExp last);

//ctrl + d scrolls screen halfway down
//ctrl + u scrolls screen halfway up
//M to middle of screen
//L to bottom
//H to top

