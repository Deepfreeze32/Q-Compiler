// Dwayne Towell

#include "Compiler.h"

static string typeName[] = {"","bool","byte","short","long"};

string Dump::indent(string s)
{ 
    s.insert(0,1,'\t');
    for (unsigned int i = 0; i < s.length()-1; i++)
    {
        if (s[i] == '\n')
            s.insert(i+1,1,'\t');
    }
    
    return s; 
}

string Dump::applyType(Type *t) { 
    stringstream os;
    os << typeName[t->type];
    if (t->size>0)
        os << "[" << t->size << "]";
    return os.str();
}

string Dump::applyModule(Module *,const StringList &vars,const StringList &funcs) {
    return "Module variables:\n"
        +  indent(concatenate(vars))
        +  "Module functions:\n"
        +  indent(concatenate(funcs));
}

string Dump::applyVarDecl(VarDecl *vd,string type) {
    return vd->name->getText() + " : " + type + "\n";
}

string Dump::applyFunc(Func *f,const StringList &params,const StringList &vars,const StringList &stmts,string expr) {
    stringstream os;
    os << f->name->getText() << " : " << typeName[f->type];
    if (f->returnExpr)
        os << " definition\n";
    else
        os << " declaration\n";
    if (f->params.size())
        os << "Parameters:\n" << indent(concatenate(params));
    if (f->vars.size())
        os << "Local variables:\n" << indent(concatenate(vars));
    if (f->stmts.size())
        os << "Statements:\n" << indent(concatenate(stmts));
    if (f->returnExpr)
        os << "Return expression:\n" << expr;
    return os.str();
}

string Dump::applyExprStmt(ExprStmt *,string expr) {
    return "Expression statement:\n" + indent(expr);
}

string Dump::applyAssignmentStmt(AssignmentStmt *,string lvalue,string expr) {
    return "Assignment statement:\n" 
        +  indent(lvalue)
        +  indent("Expression:\n")
        +  indent(expr);
}

string Dump::applyLoopStmt(LoopStmt *,const StringList &stmts,string expr) {
    return "Loop statement:\n"
        +  indent("Expression:\n")
        +  indent(indent(expr))
        +  indent("Statements:\n")
        +  indent(indent(concatenate(stmts)));
}

string Dump::applyConditionalStmt(ConditionalStmt *,const StringList &thens,const StringList &elses,string expr) {
    return "Conditional statement:\n"
        +  indent("Expression:\n")
        +  indent(indent(expr))
        +  indent("Then statements:\n")
        +  indent(indent(concatenate(thens)))
        +  indent("Else statements:\n")
        +  indent(indent(concatenate(elses)));
}

string Dump::applyUnaryExpr(UnaryExpr *ue,string expr) {
    string op[] = {"not","neg"};
    return "Unary " + op[ue->oper-TT_NOT] + " expression:\n" 
        +  indent(expr);
}

string Dump::applyBinaryExpr(BinaryExpr *be,string leftExpr,string rightExpr) {
    string op[] = {"+","-","*","/","%","<","<=",">",">=","=","<>","and","or"};
    assert(be->oper-TT_PLUS < 14);
    LOG << "Binary expression operator: " << be->oper-TT_PLUS << endl;
    return "Binary " + op[be->oper-TT_PLUS] + " expression:\n"
        + indent("Left expression:\n")
        + indent(indent(leftExpr))
        + indent("Right expression:\n")
        + indent(indent(rightExpr));
}

string Dump::applyConstant(Constant *c) {
    stringstream os;
    os << "Constant: " << c->value << " : " << typeName[c->type] << "\n";
    return os.str();
}

string Dump::applyLValue(LValue *lvalue,string index) {
    string result = "L-Value: " + lvalue->name->getText() + "\n";
    if (lvalue->index)
        result += indent("Index:\n")
                + indent(indent(index)); 
    LOG << "LValue completed: " << result << "\n";
    return result;
}

string Dump::applyIncDecExpr(IncDecExpr *ie,string expr) {
    string op[] = {"Increment","Decrement"};
    return op[ie->oper-TT_INC] + " expression:\n"
        +  indent(expr);
}

string Dump::applyFuncCall(FuncCall *call,const StringList &params) {
    string result = "Function call to " + call->name->getText() + ":\n";
    if (call->params.size()>0)
        result += indent("Parameters:\n")
                + indent(indent(concatenate(params)));
    return result;
}
