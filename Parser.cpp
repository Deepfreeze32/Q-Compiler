// Parser.cpp
//
// By Travis Cramer
//
// For CS355

#include "compiler.h"

bool Parser::parse(Tokenizer *tizer) {
	tokenizer = tizer;
	tokenizer->nextToken();

	module = parseModule();
	//#ifdef VERBOSE
	tokenizer->dumpSymbolTable();
	//#endif

	if (!module) 
		return false;
	
	return true;
}

unique_ptr<Module> Parser::parseModule() {

	Position pos = tokenizer->currentPosition();
    if (tokenizer->check(TT_END)) {
		if (!tokenizer->peek(TT_EOF)) {
			logPos();
			ERROR << "Expected end of file. Got: " << getToke() << endl;
			return nullptr;
		}
		
        return own(new Module(pos));
	}
	
    if (tokenizer->check(TT_VARS)) {
    	tokenizer->declaring = true;
		auto dl = parseDeclarationList();
        if (!dl)
			return nullptr;
		tokenizer->declaring = false;
        if (!tokenizer->check(TT_END)) {
			logPos();
			ERROR << "Expected 'end', found: " << getToke() << endl;
			return nullptr;
		}
		
		auto module = parseModule();
        if (!module)
			return nullptr;
		
		for (auto i = dl->begin(); i != dl->end(); i++)
			module->vars.push_back(move(*i));
			
		return module;
	}
	
    if (tokenizer->check(TT_FUNCTION)) {
    	tokenizer->incrementScope();
    	tokenizer->setScope(tokenizer->getNextScope());
    	tokenizer->declaring = true;
		auto fs = parseFunctionSignature();
        if (!fs)
			return nullptr;
		tokenizer->declaring = false;
		auto fb = parseFunctionBody(move(fs));
		if (!fb)
			return nullptr;
		tokenizer->setScope(0);
		auto module = parseModule();
		if (!module)
			return nullptr;
        
		module->funcs.push_front(move(fb));
		return module;
	}
	
	logPos();
	ERROR << "Expected 'vars', 'function', or 'end'. Got: " << getToke() << endl;	
			
    return nullptr;
}

unique_ptr<Func> Parser::parseFunctionSignature() {
	Position pos = tokenizer->currentPosition();
    if (tokenizer->peek(TT_IDENTIFIER)) {
		if (tokenizer->peek(TF_FUNCTION)) {
			logPos();
			ERROR << "Multiple declarations found of: " << tokenizer->gCurrentToken()->getText() << endl;
			return nullptr;
		}
		
		tokenizer->nextToken();
		tokenizer->lastToken()->setFlags(TF_FUNCTION);
        Token* name = tokenizer->lastToken();
		
		if (tokenizer->check(TT_COLON)) {
			auto pType = parseType();
			if (pType) {
				tokenizer->declaring = true;
				if (tokenizer->peek(TT_IDENTIFIER)) {
					
					auto dl = parseDeclarationList();
					if (dl) {
						auto f = own(new Func(pos, name, pType->type, move(dl)));
						name->setDeclaration(f.get());
						return f;
					}
					else
						return nullptr;
					
				}
				else {
					auto f = own(new Func(pos, name, pType->type, own(new DeclList)));
					name->setDeclaration(f.get());
					return f;
				}
				tokenizer->declaring = false;
			}
			else {
				logPos();
				ERROR << "Illegal type for identifier" << endl;
				return nullptr;
			}
		}
		else {
			logPos();
			ERROR << "Expected ':', found: " << tokenizer->gCurrentToken()->getText() << endl;
		}
	}
	else {
		logPos();
		ERROR << "Expected identifier, found: " << tokenizer->gCurrentToken()->getText() << endl;
	}
	
	return nullptr;
}

unique_ptr<Func> Parser::parseFunctionBody(unique_ptr<Func> f) {
    if (tokenizer->check(TT_END)) {
		f->define(move(own(new DeclList)), move(own(new StmtList)), nullptr);
		return f;
	}
	tokenizer->declaring = true;
	unique_ptr<DeclList> dl;	
	if (tokenizer->check(TT_VARS)) {
		dl = parseDeclarationList();
		//cout << "real declaration list here!\n";
		if (!dl)
			return nullptr;
	}
	else 
		dl = own(new DeclList);
	tokenizer->declaring = false;
	unique_ptr<StmtList> sl;
	if (tokenizer->check(TT_DO)) {
		sl = parseStatementList();
		if (!sl)
			return nullptr;
	}
	else 
		sl = own(new StmtList);
	
	if (tokenizer->check(TT_RETURN)) {
		auto expression = parseExpression();
		if (!expression) {
			logPos();
			ERROR << "Return value must be an expression." << endl;
			return nullptr;
		}
		else {
			f->define(move(dl), move(sl), move(expression));
			return f;
		}
	}
	
	logPos();
	ERROR << "Expected 'vars', 'do', 'return' or 'end'. Got: " << getToke() << endl;
	return nullptr;
}

unique_ptr<DeclList> Parser::parseDeclarationList() {
	auto vd = parseVariableDeclaration();
	if (!vd)
		return nullptr;
	
	if (tokenizer->check(TT_SEMICOLON)) {
		auto dl = parseDeclarationList();
		if (!dl)
			return nullptr;
		
		dl->push_front(move(vd));
		return dl;
	}
	
	auto dl = own(new DeclList);
    dl->push_front(move(vd));
	return dl;
}

unique_ptr<VarDecl> Parser::parseVariableDeclaration() {
	Position pos = tokenizer->currentPosition();
	if (!tokenizer->peek(TT_IDENTIFIER)) {
		logPos();
		ERROR << "Expected identifier, got: " << getToke() << endl;
		return nullptr;
	}
	
	if (tokenizer->peek(TF_VARIABLE))
	{
		logPos();
		ERROR << "Expected new identifier, previous declaration found." << endl;
		return nullptr;
	}
	
	tokenizer->nextToken();
	tokenizer->lastToken()->setFlags(TF_VARIABLE);
	Token * name = tokenizer->lastToken();
	if (!tokenizer->check(TT_COLON)) {
		logPos();
		ERROR << "Expected ':', got: " << getToke() << endl;
		return nullptr;
	}
	auto pType = parseType();
	if (!pType)
		return nullptr;
	//cout  << "Variable: " << name->getText() << " is of type: " << pType->type << endl;
	
	auto vd = own(new VarDecl(pos, name, move(pType)));
	name->setDeclaration(vd.get());
	return vd;
}

unique_ptr<Type> Parser::parseType()
{
	Position pos = tokenizer->currentPosition();
	int size = 0;

	if (tokenizer->check(TF_SCALAR_TYPE)) {
		ScalarType st = tokenizer->lastToken()->getScalarType();
		
		if (tokenizer->check(TT_OPEN_ARRAY)) {
			if (!tokenizer->check(TT_CONSTANT)) {
				logPos();
				ERROR << "Expected [ <constant> ] where <constant> is not boolean, got: " << getToke() << endl;
				return nullptr;
			}
			
			if (tokenizer->lastToken()->getText() == "true" || tokenizer->lastToken()->getText() == "false") {
				logPos();
				ERROR << "Expected [ <constant> ] where <constant> is not boolean, got: " << getToke() << endl;
				return nullptr;
			}

			size = tokenizer->lastToken()->getValue();
			if (!tokenizer->check(TT_CLOSE_ARRAY)) {
				logPos();
				ERROR << "Expected [ <constant> ] where <constant> is not boolean, got: " << getToke() << endl;
				return nullptr;
			}
		}
		
		return own(new Type(pos, st, size));	
	}

	logPos();
	ERROR << "Expected Scalar Type, got: " << getToke() << endl;
	return nullptr;
}

unique_ptr<Expr> Parser::parseExpression() {
	Position pos = tokenizer->currentPosition();
	if (tokenizer->check(TT_PLUS) || tokenizer->check(TT_MINUS) || tokenizer->check(TT_MULTIPLY) || tokenizer->check(TT_DIVIDE) || tokenizer->check(TT_MOD) || tokenizer->check(TT_LESS_THAN) || tokenizer->check(TT_GREATER_THAN) || tokenizer->check(TT_GREATER_THAN_OR_EQUAL) || tokenizer->check(TT_LESS_THAN_OR_EQUAL) || tokenizer->check(TT_EQUAL) || tokenizer->check(TT_NOT_EQUAL) || tokenizer->check(TT_AND) || tokenizer->check(TT_OR)) {
		TokenType op = tokenizer->lastToken()->getType();
		//cout << "Binary operator type: " << op << endl;
		auto leftExpr = parseExpression();
		auto rightExpr = parseExpression();
		if (!leftExpr || !rightExpr)
			return nullptr;
		return own(new BinaryExpr(pos, op, move(leftExpr), move(rightExpr)));
	}
	
	if (tokenizer->check(TT_NEG) || tokenizer->check(TT_NOT)) {
		TokenType op = tokenizer->lastToken()->getType();
		auto expression = parseExpression();
		if (!expression) {
			logPos();
			ERROR << "Invalid syntax for unary expression: " << getToke() << endl;
			return nullptr;
		}
		
		return own(new UnaryExpr(pos, op, move(expression)));
	}
	
	if (tokenizer->check(TT_INC) || tokenizer->check(TT_DEC)) {
		TokenType op = tokenizer->lastToken()->getType(); 
		
		auto lv = parseLValue();
		if (!lv)
			return nullptr;
		
		return own(new IncDecExpr(pos, op, move(lv)));
	}

	if (tokenizer->check(TT_TRUE) || tokenizer->check(TT_FALSE)) {
		//cout << "It's true/false\n";
		if (tokenizer->lastToken()->getText() == "true")
			return own(new Constant(pos, true, ST_BOOL));
			
		if (tokenizer->lastToken()->getText() == "false")
			return own(new Constant(pos, false, ST_BOOL));
	}
	
	if (tokenizer->check(TT_CONSTANT)) {
		int value = tokenizer->lastToken()->getValue();
		return own(new Constant(pos, value, ST_LONG));
	}
		
	if (tokenizer->peek(TT_IDENTIFIER)) {
		if (tokenizer->gCurrentToken()->hasAttribute(TF_VARIABLE)) {
			auto lv = parseLValue();
			if (!lv)
				return nullptr;
				
			return move(lv);
		}
		else if (tokenizer->gCurrentToken()->hasAttribute(TF_FUNCTION)) {
			Token * name = tokenizer->gCurrentToken();
			tokenizer->nextToken(); 
			auto pList = parseParameterList();
			if (!pList)
				return nullptr;
			
			return own(new FuncCall(pos, name, move(pList)));
		}
		else  {
			logPos();
			ERROR << "Undeclared Identifier: " << getToke() << endl;	
			return nullptr;
		}
	}
	return nullptr;
}

unique_ptr<LValue> Parser::parseLValue() {
	Position pos = tokenizer->currentPosition();
	if (tokenizer->check(TT_IDENTIFIER)) {
		cout << tokenizer->lastToken()->getText() << " flags: " << tokenizer->lastToken()->getFlags() << endl;
		if (!tokenizer->lastToken()->hasAttribute(TF_VARIABLE)) {
			logPos();
			ERROR << "Undeclared identifier: " << tokenizer->lastToken()->getText() << endl;		
			return nullptr;
		}
		Token * name = tokenizer->lastToken();
		if (tokenizer->check(TT_OPEN_ARRAY)) {
			auto expression = parseExpression();
			if (!expression) {
				logPos();
				ERROR << "Arrays must be instantiated with an expression. Found: " << tokenizer->gCurrentToken()->getText() << endl;
				return nullptr;
			}
			if (!tokenizer->check(TT_CLOSE_ARRAY)) {
				logPos();
				ERROR << "Expected ']', got: " << tokenizer->gCurrentToken()->getText() << endl;
				return nullptr; 
			}
			//cout << "LValue: " << name->getText() << endl;
			return own(new LValue(pos, name, move(expression)));
		}
		
		return own(new LValue(pos, name, nullptr));
	}
	
	logPos();
	ERROR << "Expected identifier or constant, got: " << getToke() << endl;
	return nullptr;
}

unique_ptr<ExprList> Parser::parseParameterList() {
	if (!tokenizer->check(TT_BANG)) {
		auto expression = parseExpression();
		auto pList = parseParameterList();
		if (!(expression && pList)) {
			logPos();
			ERROR << "Invalid syntax for Parameter list. Got: " << getToke() << endl;	
			return nullptr;
		}
			
		pList->push_front(move(expression));
		return pList;
	}
	
	auto pList = own(new ExprList);
	return pList;
}

unique_ptr<StmtList> Parser::parseStatementList() {
	auto stmt = parseStatement();
	if (!stmt)
		return nullptr;
	
	if (tokenizer->check(TT_SEMICOLON)) {
		auto sl = parseStatementList();
		if (!sl)
			return nullptr;
			
		sl->push_front(move(stmt));
		return sl;
	}
	
	auto sl = own(new StmtList);
	sl->push_front(move(stmt));
	return sl;
}

unique_ptr<Stmt> Parser::parseStatement() {
	Position pos = tokenizer->currentPosition();
	if (tokenizer->check(TT_SET)) {
		auto lv = parseLValue();
		auto expression = parseExpression(); 
		if (!(lv && expression)) {
			logPos();
			ERROR << "Illegal syntax for assignment Statement" << endl;
			return nullptr;
		}
		
		return own(new AssignmentStmt(pos, move(lv), move(expression)));
	}
	
	if (tokenizer->check(TT_OPEN_COMPOUND)) {
		auto mainSL = parseStatementList();
		if (mainSL && tokenizer->check(TT_CLOSE_COMPOUND)) {
			if (tokenizer->check(TT_CARET)) {
				auto expression = parseExpression(); 
				if (!expression) {
					logPos();
					ERROR << "Illegal syntax for loop Statement." << endl;
					return nullptr;
				}
				
				return own(new LoopStmt(pos, move(expression), move(mainSL)));
			}
			
			if (tokenizer->check(TT_IF)) {
				auto expression = parseExpression(); 
				if (!expression) {
					logPos();
					ERROR << "Illegal syntax for if clause." << endl;
					return nullptr;
				}
				
				if (tokenizer->check(TT_ELSE)) {
					bool isOpenCompound = tokenizer->check(TT_OPEN_COMPOUND);
					auto elseSL = parseStatementList();
					if (!(isOpenCompound && elseSL && tokenizer->check(TT_CLOSE_COMPOUND))) {
						logPos();
						ERROR << "Incorrect syntax for else clause." << endl;
						return nullptr;
					}
					
					return own(new ConditionalStmt(pos, move(expression), move(mainSL), move(elseSL)));
				}
				else
					return own(new ConditionalStmt(pos, move(expression), move(mainSL), move(own(new StmtList))));
			}
			
			logPos();
			ERROR << "Expected 'if', or '^', found: " << getToke() << endl;
		}
		else {
			logPos();
			ERROR << "Illegal syntax for statement list." << endl;
		}
		
		return nullptr;
	}
	
	auto expression = parseExpression();
	if (!expression)
		return nullptr;
	
	return own(new ExprStmt(pos, move(expression)));
}
