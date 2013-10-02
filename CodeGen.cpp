// Travis Cramer
// 
// For CS 355: Compiler Construction
//
// CodeGen.cpp, the core implementation of the code generator. 

#include "compiler.h"

#define BOOLEAN_TRUE_LONG "10000000000000000000000000000000b"

#define BOOLEAN_TRUE_SHORT "1000000000000000b"

#define BOOLEAN_TRUE_BYTE "10000000b"

string itoa(int value, int base) {
	
		string buf;
	
		// check that the base if valid
		if (base < 2 || base > 16) return buf;

		enum { kMaxDigits = 35 };
		buf.reserve( kMaxDigits ); // Pre-allocate enough space.
	
		int quotient = value;
	
		// Translating number to string with base:
		do {
			buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
			quotient /= base;
		} while ( quotient );
	
		// Append the negative sign
		if ( value < 0) buf += '-';
	
		std::reverse( buf.begin(), buf.end() );
		return buf;
	}

bool vecContains(vector<string> search, string toFind) {
	for (auto it = search.begin(); it != search.end(); it++) {
		if (*it == toFind)
			return true;
	}
	return false;
}

//MAIN MUST SET APPEND TO 0!
string CodeGen::makeNewLabel() {
	string temp = "Label";
	temp += itoa(append,10);
	//temp += ':';
	assert(!vecContains(labels,temp));
	labels.push_back(temp);
	append++;
	return temp;
}

//ASK
string CodeGen::applyType(Type *t) { 
	return "";
}

string CodeGen::applyModule(Module *,const StringList &vars,const StringList &funcs) {
    stringstream os;
    append = 0;
    os << "SECTION .data\n\n" << concatenate(vars);
    os << "\nSECTION .text\n\n" << concatenate(funcs);
    return os.str();
}

string CodeGen::applyVarDecl(VarDecl *vd,string type) {
	if (vd->name->getScope() == 0) {
		stringstream os;
		os << "_" << vd->name->getText() << ": ";
		if (vd->type->type == ST_LONG) {
			os << "DD ";
			if (vd->type->size >= 2) {
				for (auto i = 0; i < vd->type->size; i++) {
					os << "0";
					if (i < vd->type->size-1)
						os << ", ";
				}
			}
			else {
				os << "0";
			}
		}
		else if (vd->type->type == ST_SHORT) {
			os << "DW ";
			if (vd->type->size >= 2) {
				for (auto i = 0; i < vd->type->size; i++) {
					os << "0";
					if (i < vd->type->size-1)
						os << ", ";
				}
			}
			else {
				os << "0";
			}
		}
		else if (vd->type->type == ST_BYTE || vd->type->type == ST_BOOL) {
			os << "DB ";
			if (vd->type->size >= 2) {
				for (auto i = 0; i < vd->type->size; i++) {
					os << "0";
					if (i < vd->type->size-1)
						os << ", ";
				}
			}
			else {
				os << "0";
			}
		}
		os << endl;
		return os.str();
	}
	return "";
}

string CodeGen::applyFunc(Func *f,const StringList &params,const StringList &vars,const StringList &stmts,string expr) {
    stringstream os;
    //I'm lost....

    //Remember: GLOBAL _<ID>
    //THEN: _<ID>:

    string name = f->name->getText();
    os << "GLOBAL _" << name << endl;
    os << "_" << name << ":\n";
    os << "push ebp\nmov ebp,esp\npush ebx\n";

    //Now do all functiony things

    //Variables...somehow
    if (f->name->getLocalSize() > 0)
    	os << "sub esp," << f->name->getLocalSize() << endl;

    //Statements
    os << concatenate(stmts) << endl;
    os << expr << endl;

    if (f->name->getLocalSize() > 0)
    	os << "add esp," << f->name->getLocalSize() << endl;
    os << "pop ebx\npop ebp\nret\n\n";

    return os.str();
}

string CodeGen::applyExprStmt(ExprStmt *e,string expr) {
	stringstream os;
	os << expr << endl;
    return os.str();   
}

string CodeGen::applyAssignmentStmt(AssignmentStmt *as,string lvalue,string expr) {
    stringstream os;
    os << endl << expr << endl << "push eax\n" << lvalue << "\npop ebx\nmov [eax],ebx\n";
    return os.str();
}

string CodeGen::applyLoopStmt(LoopStmt *ls,const StringList &stmts,string expr) {
    stringstream os;
    string top = makeNewLabel();
    	string end = makeNewLabel();
    if (ls->expr->getType() == ST_BOOL){
    	//WHILE LOOP
    	os << endl << top << ":" << "\n\n" << expr;
    	os << "\nor eax,eax\n" << "jz " << end << endl;
    	os << endl << concatenate(stmts) << endl << "jmp " << top << endl;
    	os << end << ":" << endl << endl;
    }
    else {
    	//FOR LOOP
    	//REMEMBER: PUSH ECX
    	os << "\n\n" << expr << endl;
    	os << "\npush ecx\nmov ecx,eax\n" << top << ":\n" << "cmp ecx,0\n " << end << endl;
    	os << endl << concatenate(stmts) << endl << "jmp " << top << endl;
    	os << end << ":" << endl << "pop ecx\n" << endl;	
    }
    return os.str();
}

string CodeGen::applyConditionalStmt(ConditionalStmt *cs,const StringList &thens,const StringList &elses,string expr) {
	stringstream os;
	string elseLabel =  makeNewLabel();
	string end = makeNewLabel();

	string BOOLEAN_TRUE, eax;
	if (cs->conditionalExpr->getType() == ST_LONG) {
		eax = "eax";
		BOOLEAN_TRUE = BOOLEAN_TRUE_LONG;
	}
	else if (cs->conditionalExpr->getType() == ST_SHORT) {
		eax = "ax";
		BOOLEAN_TRUE = BOOLEAN_TRUE_SHORT;
	}
	else {
		eax = "al";
		BOOLEAN_TRUE = BOOLEAN_TRUE_BYTE;
	}

	os << expr << "\nand eax," << BOOLEAN_TRUE << endl << "jz " << elseLabel << endl;
	os << concatenate(thens) << endl << "jmp " << end << endl << elseLabel << ":" << endl;
	os << concatenate(elses) << endl << end << ":" << endl;
    return os.str();
}

//ASK
string CodeGen::applyUnaryExpr(UnaryExpr *ue,string expr) {
    stringstream os;
    os << expr << endl;

    string BOOLEAN_TRUE, eax;
	if (ue->expr->getType() == ST_LONG) {
		eax = "eax";
		BOOLEAN_TRUE = BOOLEAN_TRUE_LONG;
	}
	else if (ue->expr->getType() == ST_SHORT) {
		eax = "ax";
		BOOLEAN_TRUE = BOOLEAN_TRUE_SHORT;
	}
	else {
		eax = "al";
		BOOLEAN_TRUE = BOOLEAN_TRUE_BYTE;
	}

    if (ue->oper == TT_NOT)
    	os << "xor " << eax << "," << BOOLEAN_TRUE << endl;
    if (ue->oper == TT_NEG)
    	os << "neg " << eax << endl;
    return os.str();
}

string CodeGen::applyBinaryExpr(BinaryExpr *be,string leftExpr,string rightExpr) {
    stringstream os;
    // CHECK OTHER TYPES
    string eax,ebx,ecx,edx,BOOLEAN_TRUE;
	if (be->getType() == ST_LONG) {
		eax = "eax";
		ebx = "ebx";
		//ecx = "ecx";
		edx = "edx";
		BOOLEAN_TRUE = BOOLEAN_TRUE_LONG;
	}
	else if (be->getType() == ST_SHORT) {
		eax = "ax";
		ebx = "bx";
		//ecx = "cx";
		edx = "dx";
		BOOLEAN_TRUE = BOOLEAN_TRUE_SHORT;
	}
	else if (be->getType() == ST_BYTE || be->getType() == ST_BOOL) {
		eax = "al";
		ebx = "bl";
		//ecx = "cl";
		edx = "dl";
		BOOLEAN_TRUE = BOOLEAN_TRUE_BYTE;
	}

    os << leftExpr;
    if (be->rightExpr->getType() == ST_LONG && be->leftExpr->getType() != ST_LONG) {
    	if (be->leftExpr->getType() == ST_SHORT) {
    		os << "cwd\n";
    	}
    	else {
    		os << "cbw\ncwd\n";
    	}
    }
    else if (be->rightExpr->getType() == ST_SHORT && be->leftExpr->getType() < ST_SHORT) {
    	os << "cbw\n";
    }
    os << "\npush eax\n";
    os << rightExpr;
    os << "\npop ebx\n";
    if (be->leftExpr->getType() == ST_LONG && be->rightExpr->getType() != ST_LONG) {
    	if (be->rightExpr->getType() == ST_SHORT) {
    		os << "cwd\n";
    	}
    	else {
    		os << "cbw\ncwd\n";
    	}
    }
    else if (be->leftExpr->getType() == ST_SHORT && be->rightExpr->getType() < ST_SHORT) {
    	os << "cbw\n";
    }
    //INCLUDE CONDITIONALS 
    //ASK ABOUT TYPE
    if (be->oper == TT_PLUS) {
    	os << "add " << eax << "," << ebx << endl;
    }
    else if (be->oper == TT_MINUS) {
    	os << "sub " << eax << "," << ebx << endl;
    }
    else if (be->oper == TT_MULTIPLY) {
    	os << "push edx\nimul " << ebx << "\npop edx\n";
    }
    else if (be->oper == TT_DIVIDE) {
    	os << "idiv " << ebx << endl;
    }
    else if (be->oper == TT_MOD) {
    	//ASK
    	os << "idiv " << ebx << endl;
    	os << "mov " << eax << "," << edx << endl;
    }
    else if (be->oper == TT_LESS_THAN_OR_EQUAL) {
    	//ASK
    	os << "sub " << ebx << "," << eax << "\nand " << ebx << "," << BOOLEAN_TRUE << "\nmov " << eax << "," << ebx << endl;
    	os << "xor " << eax << "," << BOOLEAN_TRUE << endl;
    }
    else if (be->oper == TT_GREATER_THAN) {
    	//ASK
    	os << "sub " << ebx << "," << eax << "\nand " << ebx << "," << BOOLEAN_TRUE << "\nmov " << eax << "," << ebx << endl;
    }
    else if (be->oper == TT_LESS_THAN) {
    	os << "sub " << eax << "," << ebx << "\nand " << eax << "," << BOOLEAN_TRUE << endl;
    }
    else if (be->oper == TT_GREATER_THAN_OR_EQUAL) {
    	os << "sub " << eax << "," << ebx << "\nand " << ebx << "," << BOOLEAN_TRUE << endl;
    	os << "xor " << eax << "," << BOOLEAN_TRUE << endl;
    }
    else if (be->oper == TT_EQUAL) {
    	string ifEqual = makeNewLabel();
    	string end = makeNewLabel();
    	os << "sub " << eax << "," << ebx << "\njz " << ifEqual << endl;
    	os << "mov " << eax << ",0\njmp " << end << endl;
    	os << ifEqual << ":\nmov " << eax << "," << BOOLEAN_TRUE << endl << end << ":\n"; 
    }
    else if (be->oper == TT_NOT_EQUAL) {
    	string ifEqual = makeNewLabel();
    	string end = makeNewLabel();
    	os << "sub " << eax << "," << ebx << "\njz " << ifEqual << endl;
    	os << "mov " << eax << "," << BOOLEAN_TRUE << endl;
    	os << ifEqual << ":\nmov " << eax << ",0" << endl << end << ":\n"; 
    }
    //ASK
    else if (be->oper == TT_AND) {
    	os << "and " << eax << "," << ebx << endl;
    }
    else if (be->oper == TT_OR) {
    	os << "or " << eax << "," << ebx << endl;
    }
    return os.str();
}

//ASK ABOUT THIS
string CodeGen::applyConstant(Constant *c) {
	stringstream os;
	if (c->type != ST_BOOL)
		os << "mov eax," << c->value << endl;
	else {
		if (c->value)
			os << "mov eax," << BOOLEAN_TRUE_LONG << endl;
		else
			os << "mov eax,0\n";
	}
    return os.str();
}

//ASK ABOUT THIS
string CodeGen::applyLValue(LValue *lvalue,string index) {
    stringstream os;
    string eax = "eax";
    if (lvalue->getType() == ST_SHORT)
    	eax = "ax";
    else if (lvalue->getType() == ST_BYTE || lvalue->getType() == ST_BOOL)
    	eax = "al";
    if (index != "") {
    	os << index << endl;
    	if (lvalue->getType() == ST_LONG)
			os << "mov ecx,4\nimul ecx\nmov ecx,eax\n";
		else if (lvalue->getType() == ST_SHORT)
			os << "mov ecx,2\nimul ecx\nmov ecx,eax\n";
		else
			os << "mov ecx,eax\n";
	}
    if (lvalue->rlv) {
    	os << "lea eax,[";
    	if (lvalue->name->getScope() == 0) {
    		os << "_" << lvalue->name->getText();
    		if (index != "") {
    			os << "+ecx";
    		}
    	}
    	else {
    		//Something...
    		cout << "local l-Var: " << lvalue->name->getText() << " offset: " << lvalue->name->getLocalSize() << endl;
    		int offset = lvalue->name->getLocalSize();
    		if (offset > 0)
    			os << "ebp+" << offset;
    		else
    			os << "ebp" << offset;
    		if (index != "")
    			os << "+ecx";
    	}
    	os << "]\n";
    }
    else {
    	os << "mov " << eax << ",[";
    	if (lvalue->name->getScope() == 0) {
    		os << "_" << lvalue->name->getText();
    		if (index != "") {
    			os << "+ecx";
    		}
    	}
    	else {
    		//something...
    		cout << "local r-Var: " << lvalue->name->getText() << " offset: " << lvalue->name->getLocalSize() << endl;
    		int offset = lvalue->name->getLocalSize();
    		if (offset > 0)
    			os << "ebp+" << offset;
    		else
    			os << "ebp" << offset;
    		if (index != "")
    			os << "+ecx";
    	}
    	os << "]\n";
    }
    //os << 
    return os.str();
}

string CodeGen::applyIncDecExpr(IncDecExpr *ie,string expr) {
    stringstream os;
    os << endl << expr << endl;
    
    if (ie->oper == TT_INC) {
		os << "inc";
		if (ie->lvalue->getType() == ST_LONG)
			os << " dword ";
		else if (ie->lvalue->getType() == ST_SHORT)
			os << " word ";
		else
			os << " byte ";
		os << "[eax]\n";
	}
	else {
		os << "dec";
		if (ie->lvalue->getType() == ST_LONG)
			os << " dword ";
		else if (ie->lvalue->getType() == ST_SHORT)
			os << " word ";
		else
			os << " byte ";
		os << "[eax]\n";
	}
	//os << "mov eax,[eax]\n" << endl;
    return os.str();
}

//DON'T EVEN KNOW
string CodeGen::applyFuncCall(FuncCall *call,const StringList &params) {
    stringstream os;
    int k = 0;
    for (auto i = params.begin(); i != params.end();i++) {
    	os << *i << endl << "push eax" << endl << endl;
    	k++;
    }
    os << "call _" << call->name->getText() << endl;
    os << "add esp," << k*4 << endl;
    return os.str();
}
