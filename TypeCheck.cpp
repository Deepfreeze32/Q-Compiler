// Travis Cramer
// 
// For CS 355: Compiler Construction
//
// TypeCheck.cpp, the core implementation of the type checker. Functions which return "" don't have any type to check.

#include "Compiler.h"
string TypeChecker::applyType(Type *t) { 
    return "";
}

string TypeChecker::applyModule(Module *,const StringList &vars,const StringList &funcs) {
    return concatenate(vars) + concatenate(funcs);
}

string TypeChecker::applyVarDecl(VarDecl *vd,string type) {
    return "";
}

string TypeChecker::applyFunc(Func *f,const StringList &params,const StringList &vars,const StringList &stmts,string expr) {
    stringstream os;
    if (f->returnExpr != 0) {   
        if ((f->type == ST_BOOL && f->returnExpr->getType() != ST_BOOL) || (f->type != ST_BOOL && f->returnExpr->getType() == ST_BOOL))
            os << f->returnExpr->location().lineNumber << ":" << f->returnExpr->location().character << ": Return type does not match (Or cannot be converted to) function type." << endl;

        if (f->type == ST_BYTE && f->returnExpr->getType() > ST_BYTE)
            os << f->returnExpr->location().lineNumber << ":" << f->returnExpr->location().character << ": Return type does not match (Or cannot be converted to) function type." << endl;
        
        if (f->type == ST_SHORT && f->returnExpr->getType() == ST_LONG)
            os << f->returnExpr->location().lineNumber << ":" << f->returnExpr->location().character << ": Return type does not match (Or cannot be converted to) function type." << endl;
    }
    
    return concatenate(params) + concatenate(vars) + concatenate(stmts) + expr + os.str();
}

string TypeChecker::applyExprStmt(ExprStmt *,string expr) {
    return expr;
}

string TypeChecker::applyAssignmentStmt(AssignmentStmt *as,string lvalue,string expr) {
    stringstream os;

    if ((as->lvalue->getType() == ST_BOOL && as->expr->getType() != ST_BOOL) || (as->lvalue->getType() != ST_BOOL && as->expr->getType() == ST_BOOL))
        os << as->location().lineNumber << ":"  << as->location().character << ": Invalid assignment between number and non-numeric type Bool." << endl;
    
    if (as->lvalue->getType() == ST_BYTE && as->expr->getType() > ST_BYTE)
        os << as->location().lineNumber << ":"  << as->location().character << ": Invalid assignment between Long or Short and Byte." << endl;
    
    if (as->lvalue->getType() == ST_SHORT && as->expr->getType() == ST_LONG)
        os << as->location().lineNumber << ":"  << as->location().character << ": Invalid assignment between Short and Long." << endl;

    return lvalue + expr + os.str();
}

string TypeChecker::applyLoopStmt(LoopStmt *,const StringList &stmts,string expr) {
    return expr + concatenate(stmts);
}

string TypeChecker::applyConditionalStmt(ConditionalStmt *cs,const StringList &thens,const StringList &elses,string expr) {
    stringstream os;
   
    if (cs->conditionalExpr->getType() != ST_BOOL)
        os << cs->location().lineNumber << ":"  << cs->location().character << ": Conditional statements must be of Boolean type." << endl;

    return expr + concatenate(thens) + concatenate(elses) + os.str();
}

string TypeChecker::applyUnaryExpr(UnaryExpr *ue,string expr) {
    stringstream os;
    
    if (ue->oper == TT_NEG) {
        if (ue->expr->getType() == ST_BOOL)
            os << ue->location().lineNumber << ":"  << ue->location().character << ": Only numeric values are permitted for Unary Expression type Neg." << endl;
    }
    
    if (ue->oper ==  TT_NOT) {
        if (ue->expr->getType() != ST_BOOL)
            os << ue->location().lineNumber << ":"  << ue->location().character << ": Only boolean values are permitted for Unary Expression type Not." << endl;
    }
        
    return expr + os.str();
}

string TypeChecker::applyBinaryExpr(BinaryExpr *be,string leftExpr,string rightExpr) {
    stringstream os;
    
    if (be->oper == TT_AND || be->oper == TT_OR) {
        if (be->leftExpr->getType() != ST_BOOL || be->rightExpr->getType() != ST_BOOL) {
            //cout << "Left: " << be->leftExpr->getType() << " and Right: " << be->rightExpr->getType() << endl;
            os << be->location().lineNumber << ":"  << be->location().character << ": Expression requires type Boolean." << endl;
        }
    }
    
    else if (be->oper == TT_EQUAL || be->oper == TT_NOT_EQUAL) {
        if ((be->leftExpr->getType() == ST_BOOL && be->rightExpr->getType() != ST_BOOL) || (be->leftExpr->getType() != ST_BOOL && be->rightExpr->getType() == ST_BOOL))
            os << be->location().lineNumber << ":"  << be->location().character << ": Type of left and right expressions are incompatible." << endl;
    }
    else {
        if (be->leftExpr->getType() == ST_BOOL || be->rightExpr->getType() == ST_BOOL)
            os << be->location().lineNumber << ":"  << be->location().character << ": Expression expected numeric type." << endl;
    }
    
    return leftExpr + rightExpr + os.str();
}

string TypeChecker::applyConstant(Constant *c) {
    return "";
}

string TypeChecker::applyLValue(LValue *lvalue,string index) {
    stringstream os;
    
    if (lvalue->index != 0) {
        if (lvalue->index->getType() == ST_BOOL)
           os << lvalue->location().lineNumber << ":"  << lvalue->location().character << ": Array indices must be numeric, non-numeric type found." << endl;
    }
    
    return index + os.str();
}

string TypeChecker::applyIncDecExpr(IncDecExpr *ie,string expr) {
    stringstream os;
    
    if (ie->lvalue->getType() == ST_BOOL)
       os << ie->location().lineNumber << ":"  << ie->location().character << ": Increment and Decrement operators can only take numeric types." << endl;
    
    return expr + os.str();
}

string TypeChecker::applyFuncCall(FuncCall *call,const StringList &params) {
    stringstream os;
    auto funcDecl = dynamic_cast<Func*>(call->name->getDeclaration());

    auto decIt = funcDecl->params.begin();

    if (call->params.size() > 0) {   

        for (auto callIt = call->params.begin(); callIt != call->params.end(); callIt++) {

            auto declParameter = (*decIt)->type->type;
            auto callParameter = (*callIt)->getType();

            if  ((declParameter == ST_BOOL && callParameter != ST_BOOL) || (declParameter != ST_BOOL && callParameter == ST_BOOL) || (declParameter == ST_BYTE && callParameter > ST_BYTE) || (declParameter == ST_SHORT && callParameter == ST_LONG))
                os << call->location().lineNumber << ":"  << call->location().character << ": Call to function '" << call->name->getText() << "' failed; parameters do not match function's declaration." << endl;
            decIt++;
        }
    }

    string temp = concatenate(params);
    temp += os.str();
    return temp;
}
