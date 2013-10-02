// Dwayne Towell -- CS 355 Compiler Construction

#include "Compiler.h"

string Visitor::concatenate(const StringList &strings) {
    string result = "";
    for (auto i=strings.begin(); i!=strings.end(); ++i)
        result += *i;
    return result;
}


// AST ----------------------------
AST::~AST() {
}

// Module ---------------------------
string Module::apply(Visitor &v) {
    LOG << "Module application\n";
    list<string> varResults;
    list<string> funcResults;
    applyList(v,vars,varResults);
    applyList(v,funcs,funcResults);
    return v.applyModule(this,varResults,funcResults);
}

// VarDecl ---------------------------
VarDecl::VarDecl(const Position &p,Token *_name,unique_ptr<Type> _type) 
:   AST(p)
,   name(_name)
,   type(move(_type)) 
{
    //cout << "Variable: " << name->getText() << " is of type: " << type->type << endl;
    name->setDeclaration(this);
}

string VarDecl::apply(Visitor &v) {
    LOG << "VarDecl application\n";
    string typeResult = type->apply(v);
    return v.applyVarDecl(this,typeResult);
}

// Func ----------------------------
Func::Func(const Position &p,Token *i,ScalarType st,unique_ptr<DeclList> dl)
:   AST(p)
,   name(i)
,   type(st)
{
    params.swap(*dl);
    name->setDeclaration(this);
}

void Func::define(unique_ptr<DeclList> v, unique_ptr<StmtList> s, unique_ptr<Expr> e) {
    vars.swap(*v);
    stmts.swap(*s);
    returnExpr = move(e);
}

string Func::apply(Visitor &v) {
    LOG << "Func application\n";
    list<string> paramResults;
    list<string> varResults;
    list<string> stmtResults;
    applyList(v,params,paramResults);
    applyList(v,vars,varResults);
    applyList(v,stmts,stmtResults);
    string exprResult = returnExpr ? returnExpr->apply(v) : "";
    return v.applyFunc(this,paramResults,varResults,stmtResults,exprResult); 
}

// ExprStmt --------------------------------
string ExprStmt::apply(Visitor &v) {
    LOG << "ExprStmt application\n";
    return v.applyExprStmt(this,expr->apply(v));
}

// AssignmentStmt --------------------------------
string AssignmentStmt::apply(Visitor &v) {
    LOG << "AssignmentStmt application\n";
    lvalue->rlv=true;
    return v.applyAssignmentStmt(this,lvalue->apply(v),expr->apply(v));
}

// LoopStmt --------------------------------

string LoopStmt::apply(Visitor &v) {
    LOG << "LoopStmt application\n";
    list<string> stmtResults;
    applyList(v,stmts,stmtResults);
    return v.applyLoopStmt(this,stmtResults,expr->apply(v));
}

// ConditionalStmt --------------------------------
string ConditionalStmt::apply(Visitor &v) {
    LOG << "ConditionalStmt application\n";
    list<string> thenStmtResults;
    list<string> elseStmtResults;
    applyList(v,thenStmts,thenStmtResults);
    applyList(v,elseStmts,elseStmtResults);
    return v.applyConditionalStmt(this,thenStmtResults,elseStmtResults,conditionalExpr->apply(v));
}

bool Expr::convertibleTo(ScalarType t) {
    ScalarType myType = getType();
    assert(myType>=ST_BOOL && myType<=ST_LONG);
    if (t==ST_BOOL)
        return myType==ST_BOOL;
    return myType>=ST_BYTE && myType<=t; 
}    


// UnaryExpr --------------------------------
string UnaryExpr::apply(Visitor &v) {
    LOG << "UnaryExpr application\n";
    return v.applyUnaryExpr(this,expr->apply(v));
}

ScalarType UnaryExpr::getType() const {
    return expr->getType();
}    

// BinaryExpr --------------------------------
string BinaryExpr::apply(Visitor &v) {
    LOG << "BinaryExpr application\n";
    return v.applyBinaryExpr(this,leftExpr->apply(v),rightExpr->apply(v));
}

ScalarType BinaryExpr::getType() const {
    if (oper >= TT_LESS_THAN)
        return ST_BOOL;
    return (ScalarType)max(leftExpr->getType(),rightExpr->getType());
}    

// Constant --------------------------------
string Constant::apply(Visitor & v) {
    LOG << "Constant application\n";
    return v.applyConstant(this);
}

ScalarType Constant::getType() const {
    return type;
}    

// LValue --------------------------------
string LValue::apply(Visitor & v) {
    LOG << "lvalue application\n";
    return v.applyLValue(this,index ? index->apply(v) : "");
}

ScalarType LValue::getType() const {
    return name->getScalarType();
}    

// IncDecExpr --------------------------------
string IncDecExpr::apply(Visitor & v) {
    LOG << "IncDecExpr application\n";
    lvalue->rlv=true;
    return v.applyIncDecExpr(this,lvalue->apply(v));
}

ScalarType IncDecExpr::getType() const {
    return lvalue->getType();
}    

// FuncCall --------------------------------
FuncCall::FuncCall(const Position &p,Token *i,unique_ptr<ExprList> pl) 
:   Expr(p)
,   name(i)
{
    params.swap(*pl);
}

string FuncCall::apply(Visitor & v) {
    LOG << "FuncCall application\n";
    list<string> paramResults;
    applyList(v,params,paramResults);
    return v.applyFuncCall(this,paramResults);
}

ScalarType FuncCall::getType() const {
    return name->getScalarType();
}
