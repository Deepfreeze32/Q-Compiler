// Tokenizer.cpp
//
// By Travis Cramer
//
// For CS 355

#include "compiler.h"

//Random global functions
bool isEndOfLine(char i) {
	return (i == '\n');
}

bool possibleComment(char i) {
	return (i == '=');
}

bool isOperator(char i) {
	return (i == '%' || i == '+' || i == '-' || i == '=' || i == '!' || i == '^' || i == '{' || i == '}' || i == '[' || i == ']' || i == ':' || i == ';' || i == '*' || i == '/' || i == '>' || i == '<');
}

bool stringIsChar(string in, char i) {
	if (in.length() != 1)
		return false;
	return (in[0] == i);
}

TokenFlag Tokenizer::getAttribute(int index) {
	string i = keywords[index];
	if (i == "identifier")
		return TF_FUNCTION;
	if (i == "++" || i == "--") {
		return TF_INCDEC_OPERATOR;
	} 
	if (i == "%" || i == "+" || i == "-" || i == "=" || i == "*" || i == "/" || i == ">" || i == "<" || i == "<>" || i == "<=" || i == ">=" || i == "or" || i == "and") {
		return TF_BINARY_OPERATOR;
	}
	if (i == "not" || i == "neg")
		return TF_UNARY_OPERATOR;
	if (i == "bool" || i == "byte" || i == "short" || i == "long")
		return TF_SCALAR_TYPE;
	if (i == "true" || i == "false")
		return TF_BOOL_CONSTANT;
	return TF_NONE;
}

//First of the Tokenizer definitions
Tokenizer::Tokenizer(istream &in) {
	lineNumber = 1;
	absoluteCharNo = 0; 
	charNumber = 1;
	ostringstream out;
	out << in.rdbuf();
	input = out.str();
	Token * n = new Token("pleaseignoreme",TokenType(0),TF_NONE);
	currentToken = n;
	prevToken = n;
	previous.lineNumber = 1;
	previous.character = 1;
	current.lineNumber = 1;
	current.character = 1;
	scopeCount = 0;
	currentScope = 0;
	//tokens.push_back(move(own(&n)));
	//cout << (*tokens.front()).getText() << endl;

	endOfFile = false;
	
	string array[] = { 
	    "",
	    "",

	    "bool", 
	    "byte", 
	    "short", 
	    "long",

	    "+",
	    "-",
	    "*",
	    "/",
	    "%%",
	    "<",
	    "<=",
	    ">",
	    ">=",
	    "=",
	    "<>",
	    "and",
	    "or",
	    
	    "not",
	    "neg",
	    "++",
	    "--",
	    
	    "true",
	    "false",
	    
	    "function",
	    "vars",
	    "do",
	    "return",
	    "end",
	    "set",
	    "if",
	    "else",
	    "{",
	    "}",
	    "[",
	    "]",
	    ":",
	    ";",
	    "^",
	    "!",
	    
	    "",
	    ""
	};
	for (int i = 0; i < 43; i++)
		keywords[i] = array[i];
}

void Tokenizer::whitespaceCleanup() {
	if (isspace(input[absoluteCharNo]) || isEndOfLine(input[absoluteCharNo])) {
		//handle whitespace
		while (isspace(input[absoluteCharNo]) || isEndOfLine(input[absoluteCharNo])) {
			//if we've reached the end of the line
			if (isEndOfLine(input[absoluteCharNo])) {
				lineNumber++;
				charNumber = 0;
			}
			absoluteCharNo++;
			charNumber++;
		}
	}
}

void Tokenizer::commentAdvancement() {
	int temp = absoluteCharNo;
	while (!isEndOfLine(input[temp])) {
		temp++;
	}
	absoluteCharNo = temp + 1;
	lineNumber++;
	charNumber = 1;
}

int Tokenizer::indexOfKeyword(string in) {
	for (int i = 2; i < 41; i++) {
		if (in == keywords[i])
			return i;
	}
	return -1;
}

void Tokenizer::notePosition() {
	current.lineNumber = lineNumber;
	current.character = charNumber;
}

bool Tokenizer::doesThisExist(const Token &toFind) const {
	for (auto it = tokens.begin(); it != tokens.end(); it++) {
		if ((*it)->getText() == toFind.getText() && (*it)->getType() == toFind.getType()) {
			return true;
		}
			
	}
	return false;
}

void Tokenizer::dumpSymbolTable() {
	cout << "Dumping symbol table. First element: " << (*tokens.front()).getText() << endl;
	for (auto it = tokens.begin(); it != tokens.end(); it++) {
		string text = (*it)->getText();
		TokenType type = (*it)->getType();
		if (type == TT_IDENTIFIER)
			cout << "Symbol Table element: " << text << " Type: " << type  << " Flags: " << (*it)->getFlags() << " Scope: " << (*it)->getScope() << endl;
	}
}

void Tokenizer::tokenPointer() {
	//auto t1 = own(&currentToken);
	#ifdef DEBUGP
	//cout << "Symbol table adding: " << currentToken->getText() << " Type: " << currentToken->getType() << endl;
	#endif
	tokens.push_back(own(new Token(*currentToken)));
	#ifdef DEBUGP
	//cout << (*tokens.front()).getType() << endl;
	#endif
}

void Tokenizer::makeToken(string text, TokenType type, TokenFlag flags, int scope) {
	Token tempToken = Token(text, type, flags, scope);
	TokenList::iterator it;
	for (it = tokens.begin(); it != tokens.end(); it++) {
		if ((**it) == tempToken)
			break;
	}
	
	if (!declaring && it == tokens.end()) {
		tempToken.setScope(0);
		for (it = tokens.begin(); it != tokens.end(); it++) {
			if ((**it) == tempToken)
				break;
		}
	}

	if (it == tokens.end()) {
		currentToken = new Token(text, type, flags, scope);
		tokens.push_back(own(currentToken));
	}
	else 
	{
		currentToken = it->get();
	}
}

// move to next token
void Tokenizer::nextToken() {
	prevToken = currentToken;
	previous = current; 

	whitespaceCleanup();

	string sinceLastToken = "";
	if (input.length() <= absoluteCharNo) {
		//cout << "found end" << endl;
		Token * Temp = new Token("",TokenType(1),TF_NONE);
		currentToken = Temp;
		endOfFile = true;
	}

	//first check to see if a comment is there
	while (true) {
		if (possibleComment(input[absoluteCharNo]))
			if (possibleComment(input[absoluteCharNo+1]))
				commentAdvancement();
			else
				break;
		else
			break;
	}

	//Now start getting the next token

	//is an alphabetical character
	if (isalpha(input[absoluteCharNo])) {
		//do stuff
		sinceLastToken += input[absoluteCharNo];
		notePosition();
		charNumber++;
		absoluteCharNo++;
		while (true) {
			if (isdigit(input[absoluteCharNo]) || isalpha(input[absoluteCharNo])) {
				sinceLastToken += input[absoluteCharNo];
				absoluteCharNo++;
				charNumber++;
			}
			else 
				break;
		}

		int t = indexOfKeyword(sinceLastToken);
		if (t == -1) {
			//Token * Temp = new Token(sinceLastToken,TokenType(42),getAttribute(42));
			//currentToken = Temp;
			makeToken(sinceLastToken,TokenType(42),getAttribute(42),currentScope);
		}
		else {
			//Token * Temp = new Token(sinceLastToken,TokenType(t),getAttribute(t));
			//currentToken = Temp;
			makeToken(sinceLastToken,TokenType(t),getAttribute(t),currentScope);
		}
		//cout << t << endl;
		//cleanup whitespace
		whitespaceCleanup();
	}

	//if it's a digit
	else if (isdigit(input[absoluteCharNo])) {
		//Start adding a series of digits together
		sinceLastToken += input[absoluteCharNo];
		notePosition();
		charNumber++;
		absoluteCharNo++;

		while (true) {
			if (!isdigit(input[absoluteCharNo])){
				break;
			}
			sinceLastToken += input[absoluteCharNo];
			absoluteCharNo++;
			charNumber++;
		}

		//Token * Temp = new Token(sinceLastToken,TokenType(41));
		//currentToken = Temp;
		makeToken(sinceLastToken,TokenType(41), TF_NONE,currentScope);
		//Perform cleanup
		whitespaceCleanup();
	}

	//if it's an operator
	else if (isOperator(input[absoluteCharNo])) {
		if (input[absoluteCharNo] != '+' && input[absoluteCharNo] != '-' && input[absoluteCharNo] != '<' && input[absoluteCharNo] != '>' && input[absoluteCharNo] != '=') {
			int start1 = 8;
			int end1 = 11;
			int start2 = 33;
			int end2 = 40;
			//15
			for (int i = start1; i < end1; i++) {
				if (stringIsChar(keywords[i],input[absoluteCharNo])) {
					//Token * Temp = new Token(keywords[i],TokenType(i),getAttribute(i));
					//currentToken = Temp;
					makeToken(keywords[i],TokenType(i),getAttribute(i),currentScope);
				}
			}
			for (int i = start2; i <= end2; i++) {
				if (stringIsChar(keywords[i],input[absoluteCharNo])) {
					//Token * Temp = new Token(keywords[i],TokenType(i),getAttribute(i));
					//currentToken = Temp;
					makeToken(keywords[i],TokenType(i),getAttribute(i),currentScope);
				}
			}
			notePosition();
			charNumber++;
			absoluteCharNo++;
		}
		else {
			notePosition();
			if (input[absoluteCharNo] == '<') {
				if (input[absoluteCharNo+1] == '='){
					//12
					//Token * Temp = new Token("<=",TokenType(12),TF_BINARY_OPERATOR);
					//currentToken = Temp;
					makeToken("<=",TokenType(12),TF_BINARY_OPERATOR,currentScope);
					charNumber += 2; 
					absoluteCharNo += 2;
				}
				else if (input[absoluteCharNo+1] == '>'){
					//12
					//Token * Temp = new Token("<>",TokenType(16),TF_BINARY_OPERATOR);
					//currentToken = Temp;
					makeToken("<>",TokenType(16),TF_BINARY_OPERATOR,currentScope);
					charNumber += 2; 
					absoluteCharNo += 2;
				}
				else {
					//11
					//Token * Temp = new Token("<",TokenType(11),TF_BINARY_OPERATOR);
					//currentToken = Temp;
					makeToken("<",TokenType(11),TF_BINARY_OPERATOR,currentScope);
					charNumber++; 
					absoluteCharNo++;
				}
			}
			else if (input[absoluteCharNo] == '>') {
				if (input[absoluteCharNo+1] == '='){
					//14
					//Token * Temp = new Token(">=",TokenType(14),TF_BINARY_OPERATOR);
					//currentToken = Temp;
					makeToken(">=",TokenType(14),TF_BINARY_OPERATOR,currentScope);
					charNumber += 2; 
					absoluteCharNo += 2;
				}
				else {
					//13
					//Token * Temp = new Token(">",TokenType(13),TF_BINARY_OPERATOR);
					//currentToken = Temp;
					makeToken(">",TokenType(13),TF_BINARY_OPERATOR,currentScope);
					charNumber++; 
					absoluteCharNo++;
				}
			}
			else if (input[absoluteCharNo] == '+') {
				if (input[absoluteCharNo+1] == '+'){
					//12
					//Token * Temp = new Token("++",TokenType(21),TF_INCDEC_OPERATOR);
					//currentToken = Temp;
					makeToken("++",TokenType(21),TF_INCDEC_OPERATOR,currentScope);
					charNumber += 2; 
					absoluteCharNo += 2;
				}
				else {
					//11
					//Token * Temp = new Token("+",TokenType(6),TF_BINARY_OPERATOR);
					//currentToken = Temp;
					makeToken("+",TokenType(6),TF_BINARY_OPERATOR,currentScope);
					charNumber++; 
					absoluteCharNo++;
				}
			}
			else if (input[absoluteCharNo] == '-') {
				if (input[absoluteCharNo+1] == '-'){
					//12
					//Token * Temp = new Token("--",TokenType(22),TF_INCDEC_OPERATOR);
					//currentToken = Temp;
					makeToken("--",TokenType(22),TF_INCDEC_OPERATOR,currentScope);
					charNumber += 2; 
					absoluteCharNo += 2;
				}
				else {
					//11
					//Token * Temp = new Token("-",TokenType(7),TF_BINARY_OPERATOR);
					//currentToken = Temp;
					makeToken("-",TokenType(7),TF_BINARY_OPERATOR,currentScope);
					charNumber++; 
					absoluteCharNo++;
				}
			}
			else if (input[absoluteCharNo] == '=') {
				if (input[absoluteCharNo+1] == '='){
					commentAdvancement();
				}
				else {
					//Token * Temp = new Token("=",TokenType(15),TF_BINARY_OPERATOR);
					//currentToken = Temp;
					makeToken("=",TokenType(15),TF_BINARY_OPERATOR,currentScope);
					charNumber++; 
					absoluteCharNo++;
				}
			}
		}
		//Perform cleanup
		whitespaceCleanup();
	}
	else if (!endOfFile) {
		#ifdef DEBUG
		cout << "Invalid character: " << input[absoluteCharNo] << endl;
		#endif
		//if it's invalid
		notePosition();
		sinceLastToken += input[absoluteCharNo];
		//Token * Temp = new Token(sinceLastToken,TT_INVALID,TF_NONE);
		//currentToken = Temp;
		makeToken(sinceLastToken,TT_INVALID,TF_NONE,currentScope);
		charNumber++;
		absoluteCharNo++;
		whitespaceCleanup();
	}
	/*if (currentToken->getType() == TT_IDENTIFIER || currentToken->getType() == TT_CONSTANT)
	{	
		#ifdef DEBUGP
		//cout << "Now making token pointer: " << currentToken->getText() << endl;
		#endif	
		tokenPointer();
	}*/
}
    
// look at the current token
bool Tokenizer::peek(TokenType expected) {
	return (currentToken->getType() == expected);
}

// look at the current token and skip if it matches
bool Tokenizer::check(TokenType expected) {
	if (currentToken->getType() == expected) {
		nextToken();
		return true;
	}
	return false;
}

bool Tokenizer::peek(TokenFlag expected) {
	return (currentToken->hasAttribute(expected));
}

// look at the current token and skip if it matches
bool Tokenizer::check(TokenFlag expected) {
	if (currentToken->hasAttribute(expected)) {
		nextToken();
		return true;
	}
	return false;
}

// get the last (previous) token
/*Token * Tokenizer::lastToken() const {
	return &prevToken;
}*/
    
// get the location of the current token
/*const Position & Tokenizer::currentPosition() const {
	return &current;
}*/