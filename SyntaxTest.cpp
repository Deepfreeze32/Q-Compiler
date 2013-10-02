// this is a test of the tokenizer
//
//By Travis Cramer
//
// For CS 355

#include "compiler.h"

stringstream input1;
Tokenizer * t;
string errors;

bool input(string in, Parser & p)
{
  input1.str(in);
  t = new Tokenizer(input1);
  if (!p.parse(t)) {
    cout << p.logging() << endl;
    errors = p.logging();
    p.clearErrors();
    return false;
  }
  //t->dumpSymbolTable();
  errors = p.logging();
  p.clearErrors();
  LOG << "No errors detected.\n";
  return true;
}
void correct0(Parser & p)
{
  string t2 = "1968 bool short byte long $ true false\n";
  input1.str(t2);
  t = new Tokenizer(input1); 
  cout << t2 << endl;

  t->nextToken();
  assert(t->gCurrentToken()->getText() == "1968");
  
  Token * temp = t->lastToken();
  Token temp2(*temp);
  
  assert(t->gCurrentToken()->getType() == 41);
  assert(t->gCurrentToken()->getValue() == 1968);
  
  t->nextToken();
  assert(t->gCurrentToken()->getScalarType() == 1);
  t->nextToken();
  assert(t->gCurrentToken()->getScalarType() == 3);
  t->nextToken();
  assert(t->gCurrentToken()->getScalarType() == 2);
  t->nextToken();
  assert(t->gCurrentToken()->getScalarType() == 4);
  t->nextToken();
  assert(t->gCurrentToken()->getScalarType() == 0);
  
  t->nextToken();
  assert(t->gCurrentToken()->getBoolValue() == 1);
  t->nextToken();
  assert(t->gCurrentToken()->getBoolValue() == 0);
  
  cout << "SUCCESS" << endl;
}

void correct1(Parser & p)
{
  string t2 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars e:long do {++c} if = 5 + c b else {--b}; ++b ;set c a;  ++b return b end\n";
  //string t2 = "function kinsmen:long a:byte\n vars e:long; b:long do {++e} if + a b; ++b ;  ++b return b end\n";
  //string t2 = "function kinsmen:long a:byte\n vars e:long; b:long do ++b ;set b a;  ++b return b end\n";
  //input(t2,p);  
  cout << t2 << endl;
  if (input(t2,p))
  cout << "SUCCESS" << endl;
  else
  cout << "FAILED" << endl;
  
  assert(errors == "");
  
  Module * module = p.getModule();
    Dump dump;
  cout << endl << endl << module->apply(dump);
  TypeChecker tc;
  string output = module->apply(tc);
  cout << endl << output;
  if (output == "") {
    OffsetCalc oc;
    module->apply(oc);
    CodeGen cg;
    string code = module->apply(cg);
    cout << endl << code;
    cout << "Please give this program a descriptive name: ";
    string name;
    cin >> name;
    ofstream file(name);
    file << code;
    file.close();
  }
}

//Spacing test
void correct3(Parser & p)
{
  string t2 = "function   x    :     long  \n  return            3\t    \n   \t      end   \n";
  //input(t2,p);  
  cout << t2 << endl;
  if (input(t2,p))
  cout << "SUCCESS" << endl;
  else
  cout << "FAILED" << endl; 
  
  assert(errors == "");
  
  Module * module = p.getModule();
    Dump dump;
  cout << endl << endl << module->apply(dump);

  TypeChecker tc;
  string output = module->apply(tc);
  cout << endl << output;
  if (output == "") {
    OffsetCalc oc;
    cout << endl << module->apply(oc);
    CodeGen cg;
    string code = module->apply(cg);
    cout << endl << code;
    cout << "Please give this program a descriptive name: ";
    string name;
    cin >> name;
    ofstream file(name);
    file << code;
    file.close();
  }
}

//Comment test
void correct4(Parser & p)
{
  string t2 = "== the first comment \nfunction x : long\n  ==also a comment \nreturn 3 ==commentary stuff bro\nend\n";  
  //input(t2,p);  
  cout << t2 << endl;
  if (input(t2,p))
  cout << "SUCCESS" << endl;
  else
  cout << "FAILED" << endl;
  
  assert(errors == "");
  
  Module * module = p.getModule();
    Dump dump;
  cout << endl << endl << module->apply(dump);
  TypeChecker tc;
  string output = module->apply(tc);
  cout << endl << output;
  if (output == "") {
    OffsetCalc oc;
    module->apply(oc);
    CodeGen cg;
    string code = module->apply(cg);
    cout << endl << code;
    cout << "Please give this program a descriptive name: ";
    string name;
    cin >> name;
    ofstream file(name);
    file << code;
    file.close();
  }
}

void correct5(Parser & p)
{
  string t2 = "function adding: long\n b: long; f:short\nvars \n  c: long; a:long\ndo\n   set c ++a; set c b\n return c \nend\n";
  //input(t2,p);  
  cout << t2 << endl;
  if (input(t2,p))
  cout << "SUCCESS" << endl;
  else
  cout << "FAILED" << endl;
  
  assert(errors == "");
  
  Module * module = p.getModule();
    Dump dump;
  cout << endl << endl << module->apply(dump);
  TypeChecker tc;
  string output = module->apply(tc);
  cout << endl << output;
  if (output == "") {
    OffsetCalc oc;
    module->apply(oc);
    CodeGen cg;
    string code = module->apply(cg);
    cout << endl << code;
    cout << "Please give this program a descriptive name: ";
    string name;
    cin >> name;
    ofstream file(name);
    file << code;
    file.close();
  }
}

void correct6(Parser & p)
{
  string t2 = "function adding: long\n b: long\nvars \n  c: long\ndo\n neg c; \n {\n  {\n set c 10; ++c \n } if = c b else \n{\n++c\n}  \n } ^ < c 10; --c \n return c \nend\n";
  //string t2 = "function adding: long\n b: long\nvars \n  c: long\ndo\n 2 \n return c \nend\n";
  //string t2 = "function adding: long\n b: long\nvars \n  c: long\ndo\n neg c \n return c \nend\n";
  //string t2 = "function adding: long\n b: long\nvars \n  c: long\ndo\n 2 \n return neg neg c \nend\n";
  //input(t2,p);  
  cout << t2 << endl;
  if (input(t2,p))
  cout << "SUCCESS" << endl;
  else
  cout << "FAILED" << endl;
  
  assert(errors == "");
  
  Module * module = p.getModule();
    Dump dump;
  cout << endl << endl << module->apply(dump);
  TypeChecker tc;
  string output = module->apply(tc);
  cout << endl << output;
  if (output == "") {
    OffsetCalc oc;
    module->apply(oc);
    CodeGen cg;
    string code = module->apply(cg);
    cout << endl << code;
    cout << "Please give this program a descriptive name: ";
    string name;
    cin >> name;
    ofstream file(name);
    file << code;
    file.close();
  }
}

void correct7(Parser & p)
{
  string t1 = "function x : long\nend\nend\n";
  cout << t1 << endl;
  
  if (input(t1,p))
  cout << "SUCCESS" << endl;
  else
  cout << "FAILED" << endl;

  assert(errors == "");
  
  Module * module = p.getModule();
    Dump dump;
  cout << endl << endl << module->apply(dump);
  TypeChecker tc;
  string output = module->apply(tc);
  cout << endl << output;
  if (output == "") {
    OffsetCalc oc;
    module->apply(oc);
    CodeGen cg;
    string code = module->apply(cg);
    cout << endl << code;
    cout << "Please give this program a descriptive name: ";
    string name;
    cin >> name;
    ofstream file(name);
    file << code;
    file.close();
  }
}

void correct8(Parser & p)
{
  string t1 = "vars array  :long[10];\nmax  :long\nend\nfunction fillArray:bool\n vars index:long\ndo\n set index 0;\n{\nset array[index]";
  t1 += " index;\n++index\n} ^ < index 10\nreturn true \nvars a  :long[10];\nthing    :long\nend\nfunction findMax:long min:long; arr:long[10]\n";
  t1 += "vars i:long \ndo\nset index 0;\nset max 0;\n{\n       ++a; not max; neg index; true; false; <= a index;        \n ++index\n } ^ < index 10\nreturn max\nend";
  cout << t1 << endl;
  
  if (input(t1,p))
  cout << "SUCCESS" << endl;
  else
  cout << "FAILED" << endl;

  assert(errors == "19:11: Undeclared identifier: index\n19:12: Illegal syntax for assignment Statement\n");
  
}

void fail1(Parser & p)
{
  string t2 = "&&\n";
  //input(t2,p);  
  cout << t2 << endl;
  
  if (input(t2,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;
 // cout << errors << "$" << endl;

  assert(errors == "1:1: Expected 'vars', 'function', or 'end'. Got: &\n");

}

void fail2(Parser & p)
{
  string t2 = "function adding: stuff\n";
  //input(t2,p);  
  cout << t2 << endl;
  if (input(t2,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;
  
  assert(errors == "1:18: Expected Scalar Type, got: stuff\n1:18: Illegal type for identifier\n");
}

void fail3(Parser & p)
{
  string t2 = "function bool:\n";
  //input(t2,p);  
  cout << t2 << endl;
  if (input(t2,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;
  
  assert(errors ==  "1:10: Expected identifier, found: bool\n");
}

void fail4(Parser & p)
{
  string t1 = "end function x : long\nreturn 3\nend\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "1:5: Expected end of file. Got: function\n");
}

void fail5(Parser & p)
{
  string t1 = "vars x :: long\nreturn 3\nend\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "1:9: Expected Scalar Type, got: :\n");
}

void fail6(Parser & p)
{
  string t1 = "vars x : long \nreturn 3\nend\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:1: Expected 'end', found: return\n");
}

void fail7(Parser & p)
{
  string t1 = "vars x : long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;

  assert(errors == "1:15: Expected 'vars', 'function', or 'end'. Got: \n");
}

void fail8(Parser & p)
{
  string t1 = "function x : long y:short; z:byte[15] vars bob::long end \n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "1:48: Expected Scalar Type, got: :\n");
}

void fail9(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c if + c b else {--b}; ++b ;set c a;  ++b return foo end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:22: Illegal syntax for statement list.\n");
}

void fail10(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c} if + c b else {--b}; ++b ;set c a;  ++b return foo\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:70: Undeclared Identifier: foo\n2:70: Return value must be an expression.\n");
}

void fail11(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a::byte; b:short[10]\n vars c:long do {++c} if + c b else {--b}; ++b ;set c a;  ++b return foo end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "1:62: Expected Scalar Type, got: :\n");
}

void fail12(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen long a:byte; b:short[10]\n vars c:long do {++c} if + c b else {--b}; ++b ;set c a;  ++b return foo end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "1:55: Expected ':', found: long\n");
}

void fail13(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s::long do {++c} if + c b else {--b}; ++b ;set c a;  ++b return foo end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:9: Expected Scalar Type, got: :\n");
}

void fail14(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;

  assert(errors == "2:70: Return value must be an expression.\n");
}

void fail15(Parser & p)
{
  string t1 = "vars c long; b:short; t:byte[10] end function kinsmen:long a:byte; b:short[10]\n vars c:long do {++c} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "1:8: Expected ':', got: long\n");
}

void fail16(Parser & p)
{
  string t1 = "vars short:long; b:short; t:byte[10] end function kinsmen:long a:byte; b:short[10]\n vars c:long do {++c} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;

  assert(errors == "1:6: Expected identifier, got: short\n");
}

void fail17(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[bob] end function kinsmen:long a:byte; b:short[10]\n vars c:long do {++c} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;

  assert(errors == "1:30: Expected [ <constant> ] where <constant> is not boolean, got: bob\n");
}

void fail18(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10 end function kinsmen:long a:byte; b:short[10]\n vars c:long do {++c} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;

  assert(errors == "1:33: Expected [ <constant> ] where <constant> is not boolean, got: end\n");
}

void fail19(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c} if + long b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:28: Illegal syntax for if clause.\n");
}

void fail20(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++long} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;

  assert(errors == "2:20: Expected identifier or constant, got: long\n2:20: Illegal syntax for statement list.\n");
}

void fail21(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c; neg long} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:27: Invalid syntax for unary expression: long\n2:27: Illegal syntax for statement list.\n");
}

void fail22(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c[long]} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:22: Arrays must be instantiated with an expression. Found: long\n2:22: Illegal syntax for statement list.\n");
}

void fail23(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c[10} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:24: Expected ']', got: }\n2:24: Illegal syntax for statement list.\n");
}

void fail24(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c[10]} if + c b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:74: Return value must be an expression.\n");
}

void fail25(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c[10]} if + c b[long] else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:36: Arrays must be instantiated with an expression. Found: long\n2:36: Illegal syntax for if clause.\n");
}

void fail26(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c[10]} if + c b[10 else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:39: Expected ']', got: else\n2:39: Illegal syntax for if clause.\n");
}

void fail27(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c[10]; set long 10} if + c[10] b else {--b}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:31: Expected identifier or constant, got: long\n2:31: Illegal syntax for assignment Statement\n2:31: Illegal syntax for statement list.\n");
}

void fail28(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c[10]; set c 10} ^ long return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:39: Illegal syntax for loop Statement.\n");
}

void fail29(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c[10]; set c 10} if + c[10] b else {long}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;

  assert(errors == "2:56: Incorrect syntax for else clause.\n");
}

void fail30(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {++c[10]; set c 10} + c[10] b else {long}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:37: Expected 'if', or '^', found: +\n");
}

void fail31(Parser & p)
{
  string t1 = "vars c:long; b:short; t:byte[10] end function kinsmen:long a:byte; d:short[10]\n vars s:long do {+-c[10]; set c 10} + c[10] b else {long}; ++b ;set c a;  ++b return long end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "FAILED: Results did not meet expectations" << endl;
  else
  cout << "SUCCESS" << endl;  
  
  assert(errors == "2:25: Illegal syntax for statement list.\n");
}

void typeCheck1(Parser & p)
{
  string t1 = "function kinsmen:bool return 68 end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "1:30: Return type does not match (Or cannot be converted to) function type.\n");
}

void typeCheck2(Parser & p)
{
  string t1 = "function kinsmen:byte return 68 end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "1:30: Return type does not match (Or cannot be converted to) function type.\n");
}

void typeCheck3(Parser & p)
{
  string t1 = "function kinsmen:short return 68 end\n";
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "1:31: Return type does not match (Or cannot be converted to) function type.\n");
}

void typeCheck4(Parser & p)
{
  string t1 = "vars by:byte; bo:bool; lo:long; sh:short end\n";
    
    t1 += "function a:bool return sh\n"; //error
      t1 += "function b:bool return lo\n"; //error
    t1 += "function c:bool return by\n"; //error
      t1 += "function d:bool return bo\n"; //NO ERROR
    
    t1 += "function e:byte return sh\n"; //error
      t1 += "function f:byte return lo\n"; //error
    t1 += "function g:byte return by\n"; //NO ERROR
      t1 += "function h:byte return bo\n"; //error
    
    t1 += "function i:short return sh\n"; //NO ERROR
      t1 += "function j:short return lo\n"; //error
    t1 += "function k:short return by\n"; //NO ERROR
      t1 += "function l:short return bo\n"; //error
    
    t1 += "function m:long return sh\n"; //NO ERROR
      t1 += "function n:long return lo\n"; //NO ERROR
    t1 += "function o:long return by\n"; //NO ERROR
      t1 += "function p:long return bo\n"; //error
    
    t1 += "end";
    
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
    Dump dump;
  cout << endl << endl << module->apply(dump);
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "2:24: Return type does not match (Or cannot be converted to) function type.\n3:24: Return type does not match (Or cannot be converted to) function type.\n4:24: Return type does not match (Or cannot be converted to) function type.\n6:24: Return type does not match (Or cannot be converted to) function type.\n7:24: Return type does not match (Or cannot be converted to) function type.\n9:24: Return type does not match (Or cannot be converted to) function type.\n11:25: Return type does not match (Or cannot be converted to) function type.\n13:25: Return type does not match (Or cannot be converted to) function type.\n17:24: Return type does not match (Or cannot be converted to) function type.\n");
}

void typeCheck5(Parser & p)
{
  string t1 = "vars by:byte; by2:byte; bo:bool; lo:long; sh:short; sh2:short end\n";
    
    t1 += "function a:bool do set bo true return bo\n"; // line 2 no error
      t1 += "function b:bool do set bo by return bo\n";   
    t1 += "function c:bool do set bo sh return bo\n";
      t1 += "function d:bool do set bo lo return bo\n";
    
    t1 += "function e:byte do set by by2 return by\n"; // line 6 no error
      t1 += "function f:byte do set by bo return by\n";
    t1 += "function g:byte do set by sh return by\n";
      t1 += "function h:byte do set by lo return by\n";
    
    t1 += "function i:short do set sh sh2 return sh\n"; //line 10 no error
      t1 += "function j:short do set sh by return sh\n";  //line 11 no error
    t1 += "function k:short do set sh bo return sh\n";
      t1 += "function l:short do set sh lo return sh\n";
    
    t1 += "function m:long do set lo 68 return lo\n"; //line 14 no error
      t1 += "function n:long do set lo sh return lo\n"; // line 15 no error
    t1 += "function o:long do set lo by return lo\n"; // line 16 no error
      t1 += "function p:long do set lo bo return lo\n";
    
    t1 += "end";
    
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
    Dump dump;
  cout << endl << endl << module->apply(dump);
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "3:20: Invalid assignment between number and non-numeric type Bool.\n4:20: Invalid assignment between number and non-numeric type Bool.\n5:20: Invalid assignment between number and non-numeric type Bool.\n7:20: Invalid assignment between number and non-numeric type Bool.\n8:20: Invalid assignment between Long or Short and Byte.\n9:20: Invalid assignment between Long or Short and Byte.\n12:21: Invalid assignment between number and non-numeric type Bool.\n13:21: Invalid assignment between Short and Long.\n17:20: Invalid assignment between number and non-numeric type Bool.\n");
}

void typeCheck6(Parser & p)
{
  string t1 = "vars by:byte; by2:byte; bo:bool; lo:long; sh:short; sh2:short end\n";
    
    t1 += "function foo1:bool do {++lo} if bo return bo\n"; // line 2 no error
    t1 += "function foo2:bool do {++lo} if = by lo return bo\n"; // line 3 no error
    t1 += "function foo3:bool do {++lo} if <> by lo return bo\n"; // line 4 no error
    t1 += "function foo4:bool do {++lo} if true return bo\n"; // line 5 no error
      t1 += "function bar:bool do {++lo} if - lo sh return bo\n"; 
    
    t1 += "end";
    
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
    Dump dump;
  cout << endl << endl << module->apply(dump);
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "6:22: Conditional statements must be of Boolean type.\n");
}

void typeCheck7(Parser & p)
{
  string t1 = "vars by:byte; by2:byte; bo:bool; lo:long; sh:short; sh2:short end\n";
    
    t1 += "function foo1:bool do not bo return bo\n"; // line 2 no error
    t1 += "function foo2:bool do neg by return bo\n"; // line 3 no error
    t1 += "function foo3:bool do not true return bo\n"; // line 4 no error
    t1 += "function foo4:bool do neg 68 return bo\n"; // line 5 no error
    t1 += "function foo5:bool do not sh return bo\n"; 
    t1 += "function foo6:bool do neg bo return bo\n";
    t1 += "function foo7:bool do neg false return bo\n"; 
    
    t1 += "end";
    
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
    Dump dump;
  cout << endl << endl << module->apply(dump);
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "6:23: Only boolean values are permitted for Unary Expression type Not.\n7:23: Only numeric values are permitted for Unary Expression type Neg.\n8:23: Only numeric values are permitted for Unary Expression type Neg.\n");
}

void typeCheck8(Parser & p)
{
  string t1 = "vars by:byte; by2:byte; bo:bool; lo:long; sh:short; sh2:short end\n";
    
    t1 += "function foo1:bool do set bo and true bo return bo\n"; // line 2 no error
    t1 += "function foo2:bool do set bo or bo true return bo\n"; // line 3 no error
    t1 += "function foo3:bool do {++lo} if and bo by return bo\n"; 
    t1 += "function foo4:bool do {++lo} if and 68 true return bo\n"; 
    t1 += "function foo5:bool do {++lo} if or bo by return bo\n"; 
    t1 += "function foo6:bool do {++lo} if or 68 true return bo\n";
    
    t1 += "function foo7:bool do set lo + lo sh return bo\n"; // line 8 no error
    t1 += "function foo8:bool do set lo - true sh return bo\n";
    
    t1 += "function foo9:bool do {++lo} if = lo by return bo\n"; // line 10 no error
    t1 += "function foo10:bool do {++lo} if <> lo by return bo\n"; // line 11 no error
    t1 += "function foo11:bool do {++lo} if = bo true return bo\n"; // line 12 no error
    t1 += "function foo12:bool do {++lo} if <> bo true return bo\n"; // line 13 no error
    t1 += "function foo13:bool do {++lo} if = bo sh return bo\n"; 
    t1 += "function foo14:bool do {++lo} if = 68 false return bo\n";
    
    t1 += "end";
    
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
    Dump dump;
  cout << endl << endl << module->apply(dump);
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "4:33: Expression requires type Boolean.\n5:33: Expression requires type Boolean.\n6:33: Expression requires type Boolean.\n7:33: Expression requires type Boolean.\n9:30: Expression expected numeric type.\n14:34: Type of left and right expressions are incompatible.\n15:34: Type of left and right expressions are incompatible.\n");
}

void typeCheck9(Parser & p)
{
  string t1 = "vars by:byte; by2:byte; bo:bool; lo:long; sh:short; sh2:short end\n";
    
    t1 += "function foo1:bool do set lo[sh] by return bo\n"; // line 2 no error
    t1 += "function foo2:bool do set lo[68] sh return bo\n"; // line 3 no error
    t1 += "function foo3:bool do set lo[true] sh2 return bo\n"; 
    t1 += "function foo4:bool do set lo[bo] sh2 return bo\n"; 
    
    t1 += "function foo6:bool do {--bo} if bo return bo\n"; 
    
    t1 += "end";
    
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
    Dump dump;
  cout << endl << endl << module->apply(dump);
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "4:27: Array indices must be numeric, non-numeric type found.\n5:27: Array indices must be numeric, non-numeric type found.\n6:24: Increment and Decrement operators can only take numeric types.\n");
}

void typeCheck10(Parser & p)
{
  string t1 = "vars by:byte; by2:byte; bo:bool; lo:long; sh:short; sh2:short end\n";
    
    t1 += "function foo:long b:byte; s:short; l:long do set lo by return lo\n"; 
    
    t1 += "function bar:bool do foo by2 sh2 lo! return bo\n"; 
    
    //t1 += "vars g:byte; h:byte; i:bool; j:long; k:short; l:short end\n";
    
    t1 += "end";
    
  cout << t1 << endl;
  if (input(t1,p))
  cout << "SUCCESS" << endl;  
  else
  cout << "FAILED: Results did not meet expectations" << endl;
  
  Module * module = p.getModule();
    TypeChecker tc;
    Dump dump;
  cout << endl << endl << module->apply(dump);
  string output = module->apply(tc);
  cout << endl << output;
  assert(output == "");
  if (output == "") {
    OffsetCalc oc;
    module->apply(oc);
    CodeGen cg;
    string code = module->apply(cg);
    cout << endl << code;
    cout << "Please give this program a descriptive name: ";
    string name;
    cin >> name;
    ofstream file(name);
    file << code;
    file.close();
  }
}

int main(int argc, char* argv[])
{
  Parser p;
  
  cout << "******************************************************************" << endl;
  cout << "**************** TESTS FOR  PARSING CORRECTNESS ******************" << endl;
  cout << "******************************************************************" << endl << endl;
  
  cout << "\nCORRECT 0: ";
  correct0(p);
  
  cout << "\nCORRECT 1: ";
  correct1(p);
  
  cout << "\nCORRECT 2: SUCCESS" << endl;
  //correct2(p);
  
  cout << "\nCORRECT 3: ";
  correct3(p);
  
  cout << "\nCORRECT 4: ";
  correct4(p);
  
  cout << "\nCORRECT 5: ";
  correct5(p);
  
  cout << "\nCORRECT 6: ";
  correct6(p);
  
  cout << "\nCORRECT 7: ";
  correct7(p);
  
  cout << "\nCORRECT 8: ";
  correct8(p);
  
  cout << endl << endl;
  cout << "**************************************************************************" << endl;
  cout << "**************** TESTS FOR 'ERROR PRINTING' CORRECTNESS ******************" << endl;
  cout << "**************************************************************************" << endl << endl;
  
  cout << "\nFAIL 1: ";
  fail1(p);
  
  cout << "\nFAIL 2: ";
  fail2(p);
  
  cout << "\nFAIL 3: ";
  fail3(p);
  
  cout << "\nFAIL 4: ";
  fail4(p);
  
  cout << "\nFAIL 5: ";
  fail5(p);
  
  cout << "\nFAIL 6: ";
  fail6(p);
  
  cout << "\nFAIL 7: ";
  fail7(p);
  
  cout << "\nFAIL 8: ";
  fail8(p);
  
  cout << "\nFAIL 9: ";
  fail9(p);
  
  cout << "\nFAIL 10: ";
  fail10(p);
  
  cout << "\nFAIL 11: ";
  fail11(p);
  
  cout << "\nFAIL 12: ";
  fail12(p);
  
  cout << "\nFAIL 13: ";
  fail13(p);
  
  cout << "\nFAIL 14: ";
  fail14(p);
  
  cout << "\nFAIL 15: ";
  fail15(p);
  
  cout << "\nFAIL 16: ";
  fail16(p);
  
  cout << "\nFAIL 17: ";
  fail17(p);
  
  cout << "\nFAIL 18: ";
  fail18(p);
  
  cout << "\nFAIL 19: ";
  fail19(p);
  
  cout << "\nFAIL 20: ";
  fail20(p);
  
  cout << "\nFAIL 21: ";
  fail21(p);
  
  cout << "\nFAIL 22: ";
  fail22(p);
  
  cout << "\nFAIL 23: ";
  fail23(p);
  
  cout << "\nFAIL 24: ";
  fail24(p);
  
  cout << "\nFAIL 25: ";
  fail25(p);
  
  cout << "\nFAIL 26: ";
  fail26(p);
  
  cout << "\nFAIL 27: ";
  fail27(p);
  
  cout << "\nFAIL 28: ";
  fail28(p);
  
  cout << "\nFAIL 29: ";
  fail29(p);
  
  cout << "\nFAIL 30: ";
  fail30(p);
  
  cout << "\nFAIL 31: ";
  fail31(p);
  
  
  cout << "************************************************************************" << endl;
  cout << "**************** TESTS FOR  TYPE CHECKING CORRECTNESS ******************" << endl;
  cout << "************************************************************************" << endl << endl;
  
  cout << "\nTYPE CHECK CASE 1: ";
  typeCheck1(p);
  
  cout << "\nTYPE CHECK CASE 2: ";
  typeCheck2(p);
  
  cout << "\nTYPE CHECK CASE 3: ";
  typeCheck3(p);
  
  cout << "\nTYPE CHECK CASE 4: ";
  typeCheck4(p);
  
  cout << "\nTYPE CHECK CASE 5: ";
  typeCheck5(p);
  
  cout << "\nTYPE CHECK CASE 6: ";
  typeCheck6(p);
  
  cout << "\nTYPE CHECK CASE 7: ";
  typeCheck7(p);
  
  cout << "\nTYPE CHECK CASE 8: ";
  typeCheck8(p);
  
  cout << "\nTYPE CHECK CASE 9: ";
  typeCheck9(p);
  
  cout << "\nTYPE CHECK CASE 10: ";
  typeCheck10(p);
}

