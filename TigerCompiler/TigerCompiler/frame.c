#include "frame.h"

T_exp F_Exp(F_access acc, T_exp framePtr) {
	if (acc->kind == inFrame) {
		return T_Mem(T_Binop(T_plus, acc->u.offset, framePtr));
	}
	else {
		return T_Temp(acc->u.reg);
	}
}

static Temp_temp fp = NULL;
Temp_temp F_FP(void) {
	if (!fp) {
		fp = Temp_newtemp();
	}
	return fp;
}