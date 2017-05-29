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
		}
		assert(0);
	}
}