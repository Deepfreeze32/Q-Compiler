Project Description
===================

This is a simple compiler for the programming language we have dubbed Q.  

Instructions
------------
To compile, use build.sh. Your copy of g++ must support the standard flag  
`gnu++11`. This will generate an executable called `compiler`. Run as you  
would any unix program.

To run the compiler, provide the input file assemblynames.txt using input  
redirection. This file contains the output files that my test cases will  
generate. 

The Q language specification is included below for your reference:

Language Description
====================

Credit: Dr. Dwayne Towell, formerly of Abilene Christian University.

The following is a description of a procedural language to be implemented in this class.

Reserved words and operators
----------------------------

    function vars do return end
    byte short long bool true false 
    and or not neg set if else
    : ; [ ] + - * / % < > <= >= = <> { } ^ ! ++ --

Features
--------

Comments begin with == and go until end of line.  
Identifiers are a letter followed by letter or digit ([a-zA-Z][a-zA-Z0-9]*).  
Constants are a sequence of digits ([0-9]+) interpreted as a decimal number.  
Whitespace is allowed anywhere except within a reserved word, operator, identifier or constant.  
Type checking performed for expressions, assignment, parameters, etc.  
Automatic upward promotion is allowed; no others.  
Parameters are passed by value.  
Variables and parameters are local.  
Recursion is supported.  
Global variables are not exported (or imported from other modules).  
Functions link with other modules written in C.  

BNF grammar definition
----------------------

    M    ::= end $                                  -- module
          | function FS FB M 
          | vars DL end M
    
    FS  ::= <identifier> : ST                       -- function signature (no params)
          | <identifier> : ST DL                    -- function signature (w/ params)
    FB  ::= vars DL do SL return E                  -- function body / definition
          | do SL return E                          -- function body / definition
          | return E                                -- function body / definition
          | end                                     -- function declaration
    
    DL  ::= VD | VD ; DL                            -- declaration list
    VD  ::= <identifier> : T                        -- variable declaration
    T   ::= ST | ST [ <constant> ]                  -- type
    ST  ::= bool | byte | short | long              -- scalar type
    
    E   ::= + E E | - E E | * E E | / E E | % E E   -- num -> num
          | neg E                                   -- num -> num
          | < E E | > E E | <= E E | >= E E         -- num -> bool
          | = E E | <> E E                          -- num -> bool
          | and E E | or E E | not E                -- bool -> bool
          | <constant>                              -- num 
          | true | false                            -- bool
          | ++ LV                                   -- pre-increment, LV must be num
          | -- LV                                   -- pre-decrement, LV must be num
          | <identifier> PL                         -- function call, if <id> is func
          | LV                                      -- determined by identifier declaration
    LV  ::= <identifier>                            -- l-value for scalar types 
          | <identifier> [ E ]                      -- l-value for array reference
    PL  ::= !                                       -- parameter list
          | E PL                                                
    
    SL  ::= S | S ; SL                              -- statement list
    S   ::= E                                       -- expression, presumed to have side effects
          | set LV E                                -- assignment, type(LV) must be type(E)
          | { SL } ^ E                              -- iteration, if E is num E times, else while E  
          | { SL } if E                             -- conditional, E must be bool
          | { SL } if E else { SL }                 -- conditional, E must be bool
