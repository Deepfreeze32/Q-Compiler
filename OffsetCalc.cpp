// Travis Cramer
//
// For CS 355
//
// Offset calculation

#include "compiler.h"

string OffsetCalc::applyType(Type *t) { 
	return "";
}

string OffsetCalc::applyModule(Module *,const StringList &vars,const StringList &funcs) {
    return concatenate(vars) + concatenate(funcs);
}

string OffsetCalc::applyVarDecl(VarDecl *vd,string type) {
	return "";
}

string OffsetCalc::applyFunc(Func *f,const StringList &params,const StringList &vars,const StringList &stmts,string expr) {
    //4 bytes per paramter (KEEP JUST IN CASE)
    int paramOffset = 0;

    for (auto it = f->params.begin(); it != f->params.end(); it++) {
        if ((*it)->type->size >= 2)
            paramOffset += (4 * (*it)->type->size);
        else
            paramOffset += 4;
        (*it)->name->setLocalSize(paramOffset);
    }


    //4 bytes for local longs, 2 for local shorts, 1 for local bools and bytes
    int localOffset = 0;
    for (auto it = f->vars.begin(); it != f->vars.end(); it++) {
        //cout << "Looping for longs\n";
        if ((*it)->type->type == ST_LONG) {
            //cout << "got a long!" << endl;
            if ((*it)->type->size >= 2)
                localOffset += (4 * (*it)->type->size);
            else
                localOffset += 4;
            cout << localOffset * -1 << " for variable: " << (*it)->name->getText() << endl;
            (*it)->name->setLocalSize(localOffset * -1);
        }
    }
    for (auto it = f->vars.begin(); it != f->vars.end(); it++) {
        //cout << "Looping for shorts\n";
        if ((*it)->type->type == ST_SHORT) {
            //cout << "got a short!" << endl;
            if ((*it)->type->size >= 2)
                localOffset += (2 * (*it)->type->size);
            else
                localOffset += 2;
            cout << localOffset * -1 << " for variable: " << (*it)->name->getText() << endl;
            (*it)->name->setLocalSize(localOffset * -1);
        }
    }

    for (auto it = f->vars.begin(); it != f->vars.end(); it++) {
        //cout << "Looping for shorts\n";
        if ((*it)->type->type == ST_BOOL || (*it)->type->type == ST_BYTE) {
            //cout << "got a byte/bool!" << endl;
            if ((*it)->type->size >= 2)
                localOffset += (1 * (*it)->type->size);
            else
                localOffset += 1;
            cout << localOffset * -1 << " for variable: " << (*it)->name->getText() << endl;
            (*it)->name->setLocalSize(localOffset * -1);
        }
    }

    if (localOffset % 4 !=  0) {
        if (localOffset+1 % 4 !=  0) {
            localOffset += 2;
        }
        else
            localOffset += 1;
    }
    localOffset *= -1; 
    //cout << localOffset << endl;
    f->name->setLocalSize(localOffset);

    return "";
}

string OffsetCalc::applyExprStmt(ExprStmt *,string expr) {
	return "";   
}

string OffsetCalc::applyAssignmentStmt(AssignmentStmt *as,string lvalue,string expr) {
    return "";
}

string OffsetCalc::applyLoopStmt(LoopStmt *,const StringList &stmts,string expr) {
    return "";
}

string OffsetCalc::applyConditionalStmt(ConditionalStmt *cs,const StringList &thens,const StringList &elses,string expr) {
	return "";
}

string OffsetCalc::applyUnaryExpr(UnaryExpr *ue,string expr) {
    return "";
}

string OffsetCalc::applyBinaryExpr(BinaryExpr *be,string leftExpr,string rightExpr) {
    return "";
}

string OffsetCalc::applyConstant(Constant *c) {
	return "";
}

string OffsetCalc::applyLValue(LValue *lvalue,string index) {
    return "";
}

string OffsetCalc::applyIncDecExpr(IncDecExpr *ie,string expr) {
    return "";
}

string OffsetCalc::applyFuncCall(FuncCall *call,const StringList &params) {
    return "";
}
