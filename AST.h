// Dwayne Towell -- CS 355 Compiler Construction
//
// AST.h

// forwards
class AST;
class Type;
class Module;
class VarDecl;
class Func;
class Stmt;
class ExprStmt;
class AssignmentStmt;
class LoopStmt;
class ConditionalStmt;
class Expr;
class UnaryExpr;
class BinaryExpr;
class Constant;
class LValue;
class IncDecExpr;
class FuncCall;

// handy short cuts
typedef list<unique_ptr<VarDecl>>   DeclList;
typedef list<unique_ptr<Expr>>      ExprList;
typedef list<unique_ptr<Stmt>>      StmtList;
typedef list<unique_ptr<Func>>      FuncList;
typedef list<string>                StringList;

class Visitor {
public:
    virtual string applyType(Type *type) = 0;
    virtual string applyModule(Module *module,const StringList &vars,const StringList &funcs) = 0;
    virtual string applyVarDecl(VarDecl *vd,string type) = 0;
    virtual string applyFunc(Func *func,const StringList &params,const StringList &vars,const StringList &stmts,string returnExpr) = 0;
    virtual string applyExprStmt(ExprStmt *es,string expr) = 0;
    virtual string applyAssignmentStmt(AssignmentStmt *as,string lvalue,string expr) = 0;
    virtual string applyLoopStmt(LoopStmt *ls,const StringList &stmts,string expr) = 0;
    virtual string applyConditionalStmt(ConditionalStmt *cs,const StringList &thens,const StringList &elses,string expr) = 0;
    virtual string applyUnaryExpr(UnaryExpr *ue,string expr) = 0;
    virtual string applyBinaryExpr(BinaryExpr *be,string left,string right) = 0;
    virtual string applyConstant(Constant *c) = 0;
    virtual string applyLValue(LValue *lv,string expr) = 0;
    virtual string applyIncDecExpr(IncDecExpr *id,string lvalue) = 0;
    virtual string applyFuncCall(FuncCall *fc,const StringList &params) = 0;

protected:
    static string concatenate(const StringList &strings);
};

// Value-semantic object specifying a location in a source file.
struct Position {
    Position() : lineNumber(1), character(1) {};
    int lineNumber;
    int character;
};

template <class T>
unique_ptr<T> own(T *thing) { return unique_ptr<T>(thing); };

// Abstract Syntax Tree
//
class AST {
public:
    AST(const Position &pos) : position(pos) { };
    virtual ~AST();

    virtual string apply(Visitor &visitor) = 0;
        
    const Position &location()                  const { return position; };

protected:
    template <class T>
    void applyList(Visitor &v,const list<T> &items,StringList &results) {
        LOG << "List application\n";
        for (auto i=items.begin(); i!=items.end(); ++i)
            results.push_back((*i)->apply(v));
    }
    
private:
    Position position;
    
    // prevent copying
    AST(const AST &other);
    void operator=(const AST &other);
};

class Type : public AST {
public:
    Type(const Position &p,ScalarType st,int s) :AST(p),type(st),size(s) { };
    virtual string apply(Visitor &v) { return v.applyType(this); };

    ScalarType                  type;
    int                         size;  // 0 if scalartype, else size of array
};

class Module : public AST {
public:
    Module(const Position &p) : AST(p) {};
    virtual string apply(Visitor &v);

    DeclList                    vars;
    FuncList                    funcs;
};

class VarDecl : public AST {
public:
    VarDecl(const Position &p,Token *_name,unique_ptr<Type> _type);
    virtual string apply(Visitor &v);
    
    Token *                     name;
    unique_ptr<Type>            type;
};

class Func : public AST {
public:
    Func(const Position &p,Token *i,ScalarType st,unique_ptr<DeclList> dl);
    void define(unique_ptr<DeclList> v, unique_ptr<StmtList> s, unique_ptr<Expr> e);
    virtual string apply(Visitor &v);
    
    Token *                     name;
    ScalarType                  type;
    DeclList                    params;

    DeclList                    vars;       // empty if only a declaration
    StmtList                    stmts;      // empty if only a declaration
    unique_ptr<Expr>            returnExpr; // null  if only a declaration
};


class Stmt : public AST {
public:
    Stmt(const Position &p) : AST(p) { };
};

class ExprStmt : public Stmt {
public:
    ExprStmt(const Position &p,unique_ptr<Expr> e) : Stmt(p),expr(move(e)) {};
    virtual string apply(Visitor &v);

    unique_ptr<Expr>            expr;
};

class AssignmentStmt : public Stmt {
public:
    AssignmentStmt(const Position &p,unique_ptr<LValue> lv,unique_ptr<Expr> e) 
        : Stmt(p),lvalue(move(lv)), expr(move(e)) { };
    virtual string apply(Visitor &v);

    unique_ptr<LValue>          lvalue;
    unique_ptr<Expr>            expr;
};

class LoopStmt : public Stmt {
public:
    LoopStmt(const Position &p,unique_ptr<Expr> e,unique_ptr<StmtList> sl) : Stmt(p), expr(move(e)) { stmts.swap(*sl); } 
    virtual string apply(Visitor &v);

    unique_ptr<Expr>            expr;
    StmtList                    stmts;
};

class ConditionalStmt : public Stmt {
public:
    ConditionalStmt(const Position &p,unique_ptr<Expr> ce,unique_ptr<StmtList> ts,unique_ptr<StmtList> es) 
        : Stmt(p), conditionalExpr(move(ce)) { thenStmts.swap(*ts); if (es) elseStmts.swap(*es); }
    virtual string apply(Visitor &v);

    unique_ptr<Expr>            conditionalExpr;
    StmtList                    thenStmts;
    StmtList                    elseStmts;  // empty if no else stmts
};

class Expr : public AST {
public:
    Expr(const Position &p) : AST(p) { };
    virtual ScalarType getType() const = 0;
    bool convertibleTo(ScalarType t);    
};

class UnaryExpr : public Expr {
public:
    UnaryExpr(const Position &p,TokenType o,unique_ptr<Expr> e) : Expr(p),oper(o),expr(move(e)) { };
    virtual string apply(Visitor &v);
    ScalarType getType() const;    
    
    TokenType                   oper;
    unique_ptr<Expr>            expr;
};

class BinaryExpr : public Expr {
public:
    BinaryExpr(const Position &p,TokenType o,unique_ptr<Expr> le,unique_ptr<Expr> re) 
        : Expr(p),oper(o), leftExpr(move(le)), rightExpr(move(re)) { };
    virtual string apply(Visitor &v);
    ScalarType getType() const;    
        
    TokenType                   oper;
    unique_ptr<Expr>            leftExpr;
    unique_ptr<Expr>            rightExpr;
};

class Constant : public Expr {
public:
    Constant(const Position &p,int v,ScalarType t) : Expr(p),value(v), type(t) { };
    virtual string apply(Visitor &v);
    ScalarType getType() const;    
    
    int                         value;
    ScalarType                  type;
};

class LValue : public Expr {
public:
    LValue(const Position &p,Token *i,unique_ptr<Expr> e) : Expr(p),name(i), index(move(e)) { rlv = false; };
    virtual string apply(Visitor &v);
    ScalarType getType() const;    
    
    bool                        rlv;    //Only if a real LValue
    Token *                     name;
    unique_ptr<Expr>            index;  // only if type of name is array
};

class IncDecExpr : public Expr {
public:
    IncDecExpr(const Position &p,TokenType op,unique_ptr<LValue> lv) : Expr(p),oper(op),lvalue(move(lv)) { };
    virtual string apply(Visitor &v);
    ScalarType getType() const;    
    
    TokenType                   oper;
    unique_ptr<LValue>          lvalue;
};

class FuncCall : public Expr {
public:
    FuncCall(const Position &p,Token *i,unique_ptr<ExprList> pl);
    virtual string apply(Visitor &v);
    ScalarType getType() const;    
    
    Token *                     name;
    ExprList                    params;
};