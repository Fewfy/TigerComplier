#pragma once
#include "tree.h"
#include "temp.h"
#include "util.h"
#include "frame.h"
#include <stdio.h>

typedef struct Tr_exp_* Tr_exp;
typedef struct Tr_Access_* Tr_Access;
typedef struct Tr_level_ * Tr_level;
typedef struct Tr_accessList_* Tr_accessList;
typedef struct Tr_expList_* Tr_expList;
typedef struct Tr_node_* Tr_node;

struct Cx {
	patchList trues;
	patchList falses;
	T_stm stm;
};

struct Tr_exp_ {
	enum {
		Tr_ex, Tr_nx, Tr_cx
	}kind;
	union {
		T_exp ex;
		T_stm nx;
		struct Cx cx;
	}u;
};

struct Tr_Access_ {
	Tr_level level;
	F_access access;
};

struct Tr_level_ {
	Tr_level parent;
	Temp_label name;
	F_frame frame;
	Tr_accessList formals;
};

struct Tr_accessList_ {
	Tr_Access head;
	Tr_accessList tail;
};

struct Tr_expList_ {
	Tr_node first;
	Tr_node last;
};

struct Tr_node_ {
	Tr_exp head;
	Tr_node next;
};

static Tr_exp Tr_Ex(T_exp ex);
static Tr_exp Tr_Nx(T_stm nx);
static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm);

typedef struct patchList_ * patchList;
struct patchList_ { Temp_label *head; patchList tail; };
static patchList PatchList(Temp_label *head, patchList tail);

static T_exp unEx(Tr_exp e);
static T_stm unNx(Tr_exp e);
static struct Cx unCx(Tr_exp e);
static Tr_level outer = NULL;

Tr_level Tr_outermost(void);
Tr_level Tr_newLvel(Tr_level parent, Temp_label name, U_boolList formals);
Tr_Access Tr_allocLocal(Tr_level level, bool escape);
Tr_accessList Tr_AccessList(Tr_Access head, Tr_accessList tail);
Tr_accessList Tr_formals(Tr_level level);

Tr_expList Tr_ExpList(void);

void Tr_ExpList_append(Tr_expList list, Tr_exp expr);

void Tr_ExpList_prepend(Tr_expList list, Tr_exp expr);
int Tr_ExpList_empty(Tr_expList list);

Tr_exp Tr_seqExp(Tr_expList list);
Tr_exp Tr_fieldVar(Tr_exp record, int fieldOffset);
Tr_exp Tr_subcriptVar(Tr_exp arrayBase, Tr_exp index);

Tr_exp Tr_arrayExp(Tr_exp size, Tr_exp init);
Tr_exp Tr_recordExp(int n, Tr_expList list);

Tr_exp Tr_letExp(Tr_expList list);
Tr_exp Tr_doneExp(void);
Tr_exp Tr_breakExp(Tr_exp breakk);
Tr_exp Tr_whileExp(Tr_exp cond, Tr_exp done, Tr_exp body);
Tr_exp Tr_ifExp(Tr_exp cond, Tr_exp then, Tr_exp elsee);
Tr_exp Tr_assignExp(Tr_exp var, Tr_exp exp);

Tr_exp Tr_arithExp(Tr_exp left, Tr_exp right )


void doPatch(patchList tList, Temp_label label) {
	for (; tList; tList = tList->tail)
		*(tList->head) = label;
}

patchList joinPatch(patchList first, patchList second) {
	if (!first)return second;
	for (; first->tail; first = first->tail);
	first->tail = second;
	return first;
}

Tr_exp Tr_simpleVar(Tr_Access, Tr_level);