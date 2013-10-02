// Dwayne Towell -- CS 355 Compiler Construction

#include <string>

enum TokenType { 
    TT_INVALID,
    TT_EOF,

    TT_BOOL, 
    TT_BYTE, 
    TT_SHORT, 
    TT_LONG,

    TT_PLUS,
    TT_MINUS,
    TT_MULTIPLY,
    TT_DIVIDE,
    TT_MOD,
    TT_LESS_THAN,
    TT_LESS_THAN_OR_EQUAL,
    TT_GREATER_THAN,
    TT_GREATER_THAN_OR_EQUAL,
    TT_EQUAL,
    TT_NOT_EQUAL,
    TT_AND,
    TT_OR,
    
    TT_NOT,
    TT_NEG,
    TT_INC,
    TT_DEC,
    
    TT_TRUE,
    TT_FALSE,
    
    TT_FUNCTION,
    TT_VARS,
    TT_DO,
    TT_RETURN,
    TT_END,
    TT_SET,
    TT_IF,
    TT_ELSE,
    TT_OPEN_COMPOUND,
    TT_CLOSE_COMPOUND,
    TT_OPEN_ARRAY,
    TT_CLOSE_ARRAY,
    TT_COLON,
    TT_SEMICOLON,
    TT_CARET,
    TT_BANG,
    
    TT_CONSTANT,
    TT_IDENTIFIER
};

enum TokenFlag {
    TF_NONE             = 0,
    TF_SCALAR_TYPE      = 1,
    TF_BINARY_OPERATOR  = 2,
    TF_UNARY_OPERATOR   = 4,
    TF_INCDEC_OPERATOR  = 8,
    TF_BOOL_CONSTANT    = 16,
    
    TF_VARIABLE         = 32, // not used in tokenizer assignment
    TF_ARRAY            = 64,
    TF_LOCAL            = 128,
    TF_FUNCTION         = 256,
    TF_DECLARED_TWICE   = 512,
    // ...
};

enum ScalarType { 
    ST_INVALID,
    ST_BOOL, 
    ST_BYTE, 
    ST_SHORT, 
    ST_LONG
};

// forward
class AST;

class Token {
public:
    Token() {}
    Token(const string &text,TokenType type,int scope);
    Token(const string &text,TokenType type,TokenFlag flags,int scope);
	Token(const Token &old);

	void                operator=(const Token &old);
    bool                operator==(const Token& other);

    void                setDeclaration(AST *decl);  // not used in tokenizer assignment
    void                youAreAFunction() { flags = TF_FUNCTION; }
    void                youAreAVariable() { flags = TF_VARIABLE; }
    void                youWereDeclaredTwice() {flags = (TokenFlag)(flags | 512);}
    void                setFlags(TokenFlag f) {flags = f;};
    void                setScope(int sc);
    
    TokenType           getType()       const { return type; }
    const string &      getText()       const { return text; }
    int                 getFlags()      const { return flags; }

    bool                hasAttribute(TokenFlag bit) const { return flags & bit; }
    
    int                 getScope()      const;
    int                 getValue()      const;
    ScalarType          getScalarType() const;
    bool                getBoolValue()  const;
    int                 getArraySize()  const;  // not used in tokenizer assignment

    int                 getLocalSize()  const {return localSize;}
    void                setLocalSize(int size){localSize = size;}

    AST *               getDeclaration() const { return declaration; }
    
private:
    string              text;
    TokenType           type;
    int                 flags;
    int                 scope;
    AST *               declaration;    // not used in tokenizer assignment
    int                 localSize;
    //bool                function;
    //bool                variable;
};

#ifdef VERBOSE
    #define LOG cout
#else
    #include <iostream>
    // create a handy "stream" that does nothing, to consume logging expressions
    struct nullstream : ostream { nullstream() : ios(0), ostream(0) {} };
    static nullstream logstream;
    #define LOG if(0) logstream
#endif