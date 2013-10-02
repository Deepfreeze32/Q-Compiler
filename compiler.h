// Dwayne Towell

#include <cassert>
#include <iostream>
#include <algorithm>
#include <list>
#include <memory>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cctype>
#include <fstream>
using namespace std;

#include "Token.h"
#include "AST.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Dump.h"
#include "TypeCheck.h"
#include "OffsetCalc.h"
#include "CodeGen.h"