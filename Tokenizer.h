// Dwayne Towell -- CS 355 Compiler Construction


string formatError(const Position &p,const string &error);

typedef list<unique_ptr<Token>> TokenList;

class Tokenizer {
public:
    Tokenizer(istream &input);

    void            nextToken();
    //Token *         getTokenPointer(Token * t);
    
    // look at the current token only
    bool            peek(TokenType expected);
    bool            peek(TokenFlag expected);
    
    // look at the current token and skip if it matches
    bool            check(TokenType expected);
    bool            check(TokenFlag expected);
    
    // look at the current token, skip if match, and complain if it does not match
    bool            expect(TokenType expected,string description);
    bool            expect(TokenFlag expected,string description);
    
    //void            report(string error) { errors << formatError(position,error); }

    Token *          gCurrentToken()      { return currentToken; }
    Token *          lastToken()          { assert(prevToken); return prevToken; }
    
    const Position &currentPosition()   const { return current; }
    const Position &previousPosition()  const { return previous; }

    bool            doesThisExist(const Token &toFind) const;
    
    void            dumpSymbolTable();

    void            incrementScope() {scopeCount++;}
    int             getNextScope() const {return scopeCount;}
    void            setScope(int sc) {currentScope = sc;}

    bool            declaring;
private:
    //istream &       input;                  
    //ostream &       errors;
	//This is where Tokens are stored for reference	
    TokenList       tokens;       
    
    //Position        position;               
    string          lineBuffer;             

    //Token *         currentToken;          
    //Token *         previousToken;             
    
    Position current;
    Position previous;

    Token * currentToken;
    Token * prevToken;

    string input;

    int lineNumber;
    int charNumber;
    unsigned int absoluteCharNo;
	
    int scopeCount;
    int currentScope;

	bool endOfFile;

    //TokenType ttarray[43];
    string keywords[43];
    
    // Helper functions
    TokenFlag getAttribute(int index);
    void whitespaceCleanup();
    void commentAdvancement();
    int indexOfKeyword(string in);
    void notePosition();
    void tokenPointer();
    void makeToken(string text, TokenType type, TokenFlag flags, int scope);

    // disallow these
    Tokenizer(const Tokenizer &);
    void operator=(const Tokenizer &);
};

