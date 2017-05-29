#include "translate.h"



static T_exp unEx(Tr_exp e) {
	switch (e->kind) {
	case Tr_ex: {
		return e->u.ex;
	}
	case Tr_cx: {
		Temp_temp r = Temp_newtemp();
		Temp_label t = Temp_newlabel(), f = Temp_newlabel();
		doPatch(e->u.cx.trues, t);
		doPatch(e->u.cx.falses, f);
		return T_Eseq(T_Move(T_Temp(r), T_Const(1)),
			T_Eseq(e->u.cx.stm,
				T_Eseq(T_Label(f),
					T_Eseq(T_Move(T_Temp(r),
						T_Const(0)),
						T_Eseq(T_Label(t), T_Temp(r))))));
	}
	case Tr_nx: {
		return T_Eseq(e->u.nx, T_Const(0));
	}default:
				assert(0);
	}
}

static T_stm unNx(Tr_exp e) {
	switch (e->kind) {
	case Tr_ex: {
		return T_Exp(e->u.ex);
	}
	case Tr_nx:
		return e->u.nx;
	case Tr_cx: {
		Temp_temp r = Temp_newTemp();
		Temp_temp t = Temp_newlabel(), f = Temp_newlabel();
		doPatch(e->u.cx.trues, t);
		doPatch(e->u.cx.falses, f);
		return T_Exp(T_Eseq(T_Move(T_Temp(r), T_Const(1)),
			T_Eseq(e->u.cx.stm,
				T_Eseq(T_Label(f),
					T_Eseq(T_Move(T_Temp(r), T_Const(0)),
						T_Eseq(T_Label(t), T_Temp(r)))))));
	}
	default:
		assert(0);
	}
}

static struct Cx unCx(Tr_exp e) {
	switch (e->kind) {
	case Tr_ex: {
		struct Cx cx;
		cx.stm = T_Cjump(T_eq, e->u.ex, T_Const(0), NULL, NULL);
		cx.trues = PatchList(&(cx.stm->u.CJUMP.falses), NULL);
		cx.falses = PatchList(&(cx.stm->u.CJUMP.trues), NULL);
		return cx;
	}
	case Tr_cx:
		return e->u.cx;
	default:
		assert(0);
	}
}

static patchList PatchList(Temp_label *head, patchList tail) {
	patchList patch = (patchList)malloc(sizeof(struct patchList_));
	patch->head = head;
	patch->tail = tail;
	return patch;
}

static Tr_exp Tr_Ex(T_exp ex) {
	Tr_exp expres = (Tr_exp)malloc(sizeof(struct Tr_exp_));
	expres->kind = Tr_ex;
	expres->u.ex = ex;
	return expres;
}

static Tr_exp Tr_Nx(T_stm stm) {
	Tr_exp exp = (Tr_exp)malloc(sizeof(struct Tr_exp_));
	exp->kind = Tr_nx;
	exp->u.nx = stm;
	return exp;
}

static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm) {
	Tr_exp exp = (Tr_exp)malloc(sizeof(struct Tr_exp_));
	exp->kind = Tr_cx;
	exp->u.cx.trues = trues;
	exp->u.cx.falses = falses;
	exp->u.cx.stm = stm;
	return exp;
}

Tr_exp Tr_simpleVar(Tr_Access access, Tr_level level) {
	Tr_exp expr = T_Temp(F_FP());
	//静态链查找
	while (level != access->level) {
		F_access base = level->frame->formals->head;
		expr = F_Exp(base, expr);
		//向上查找
		level = level->parent;
	}
	return Tr_Ex(F_Exp(access->access, expr));
}