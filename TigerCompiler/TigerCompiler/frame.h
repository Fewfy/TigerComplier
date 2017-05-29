#pragma once
#include "util.h"
#include "temp.h"
#include "tree.h"

typedef struct F_frag_ *F_frag;
typedef struct F_frame_ *F_frame;
typedef struct F_access_* F_access;
typedef struct F_accessList_* F_accessList;



struct F_frag_ {
	enum {
		F_stringFrag, F_procFrag
	}kind;
	union{
		struct {
			Temp_label label;
			string str;
		}stringg;
		struct {
			T_stm body;
			F_frame frame;
		}proc;
	}u;
};

struct F_access_ {
	enum {
		inFrame,
		outFrame
	}kind;
	union {
		int offset;//ÔÚÕ»Ö¡ÖÐµÄÆ«ÒÆÁ¿
		Temp_temp reg;//¼Ä´æÆ÷
	}u;
};

struct F_frame_ {
	Temp_label name;
	F_accessList formals;
	int local_count;
};

struct F_accessList_ {
	F_access head;
	F_accessList tail;
};

F_frag F_StringFrag(Temp_label, string);
F_frag F_ProcFrag(T_stm body, F_frame frame);

typedef struct F_fragList_* F_fragList;
struct F_fragList_ {
	F_frag head;
	F_fragList tail;
};

F_fragList F_FragList(F_frag head, F_fragList tail);

F_fragList Tr_getResult(void);

Temp_temp F_FP(void);
T_exp F_Exp(F_access acc, T_exp framePtr);