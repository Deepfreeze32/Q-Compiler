// Token.cpp
//
// By Travis Cramer
//
// For CS 355

#include "compiler.h"

Token::Token(const string &text,TokenType type,int scope) : text(text),type(type),scope(scope) {}

Token::Token(const string &text,TokenType type,TokenFlag flags,int scope) : text(text),type(type),flags(flags),scope(scope) {}

void Token::setDeclaration(AST * decl) {
	declaration = decl;
}

void Token::operator=(const Token &other) {
	if (this == &other)
		return;
	text = other.getText();
	type = other.getType();
	flags = TF_NONE;
	scope = other.getScope();
}

bool Token::operator==(const Token& other)
{
	return this->text == other.text && this->scope == other.scope;
}

Token::Token(const Token &old) {
	text = old.getText();
	type = old.getType();
	flags = TF_NONE;
	scope = old.getScope();
}

int Token::getValue() const {
	const char * txt = text.c_str();
	return atoi(txt);

	/*int temp = 0;
	for (int i = 0; i < text.length(); i++) {
		if (!isdigit(text[i]))
			return -1;

	}*/
}

void Token::setScope(int sc) {
	//assert(type == TT_IDENTIFIER);
	scope = sc;
}

int Token::getScope() const {
	//assert(type == TT_IDENTIFIER);
	return scope;
}

ScalarType Token::getScalarType() const {
	if (type == TT_IDENTIFIER)
	{	
		auto output = dynamic_cast<VarDecl*>(declaration);
		return output->type->type;
	}
	else if (text == "bool")
		return ST_BOOL;
	else if (text == "byte")
		return ST_BYTE;
	else if (text == "long")
		return ST_LONG;
	else if (text == "short")
		return ST_SHORT;
	else
		return ST_INVALID;
}

bool Token::getBoolValue() const {
	if (text == "true")
		return true;
	if (text == "false")
		return false;
	return false;
}

 int Token::getArraySize()  const {
	return 0;
 }
