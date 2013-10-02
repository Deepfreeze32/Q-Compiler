// Parser.h
//
// By Travis Cramer
//
// For CS355
//
// Header rewrite by Dwayne Towell -- CS 355 Compiler Construction

class Parser {
public:
    bool                   parse(Tokenizer *tizer);
    string                 logging() {return ERROR.str();}
    void                   clearErrors() {ERROR.str(string());}
	Module *               getModule() { return module.get(); }
    
private:       
    unique_ptr<Module>     parseModule();
    unique_ptr<Func>       parseFunctionSignature();
    unique_ptr<Func>       parseFunctionBody(unique_ptr<Func> f);
    unique_ptr<DeclList>   parseDeclarationList();
    unique_ptr<VarDecl>    parseVariableDeclaration();
    unique_ptr<Type>       parseType();
    unique_ptr<Expr>       parseExpression();
    unique_ptr<LValue>     parseLValue();
    unique_ptr<ExprList>   parseParameterList();
    unique_ptr<StmtList>   parseStatementList();
    unique_ptr<Stmt>       parseStatement();

    Tokenizer *            tokenizer;
    unique_ptr<Module>     module;

    //int                    getArraySize();
    string                 getToke() {return tokenizer->gCurrentToken()->getText();}
    void                   logPos() { ERROR << tokenizer->currentPosition().lineNumber << ":" << tokenizer->currentPosition().character << ": "; }
    
	stringstream ERROR;
};