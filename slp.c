#include "util.h"
#include "slp.h"

AStm make_ACompoundStm(AStm stm1, AStm stm2) {
    AStm s = malloc_checked(sizeof(struct AStm_));
    s->kind = A_COMPOUND_STM;
    s->u.compound.stm1 = stm1;
    s->u.compound.stm2 = stm2;
    return s;
}

AStm make_AAssignStm(string id, AExp exp) {
  AStm s = malloc_checked(sizeof(struct AStm_));
  s->kind = A_ASSIGN_STM;
  s->u.assign.id = id;
  s->u.assign.exp = exp;
  return s;
}

AStm make_APrintStm(AExpList exps) {
  AStm s = malloc_checked(sizeof(struct AStm_));
  s->kind = A_PRINT_STM;
  s->u.print.exps = exps;
  return s;
}

AExp make_AIdExp(string id) {
  AExp e = malloc_checked(sizeof(struct AExp_));
  e->kind = A_ID_EXP;
  e->u.id = id;
  return e;
}

AExp make_ANumExp(int num) {
  AExp e = malloc_checked(sizeof(struct AExp_));
  e->kind = A_NUM_EXP;
  e->u.num = num;
  return e;
}

AExp make_AOpExp(AExp left, ABinop oper, AExp right) {
  AExp e = malloc_checked(sizeof(struct AExp_));
  e->kind = A_OP_EXP;
  e->u.op.left = left;
  e->u.op.oper = oper;
  e->u.op.right = right;
  return e;
}

AExp make_AEseqExp(AStm stm, AExp exp) {
  AExp e = malloc_checked(sizeof(struct AExp_));
  e->kind = A_ESEQ_EXP;
  e->u.eseq.stm = stm;
  e->u.eseq.exp = exp;
  return e;
}

AExpList make_APairExpList(AExp head, AExpList tail) {
  AExpList e = malloc_checked(sizeof(struct AExpList_));
  e->kind = A_PAIR_EXP_LIST;
  e->u.pair.head = head;
  e->u.pair.tail = tail;
  return e;
}

AExpList make_ALastExpList(AExp last) {
  AExpList e = malloc_checked(sizeof(struct AExpList_));
  e->kind = A_LAST_EXP_LIST;
  e->u.last = last;
  return e;
}

