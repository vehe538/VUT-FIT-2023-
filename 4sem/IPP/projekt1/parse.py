import sys
from lark import Lark, Token, Tree, UnexpectedToken, UnexpectedCharacters
import xml.etree.ElementTree as ET
import xml.dom.minidom
import re

# check is amount of arguments is correct
if (len(sys.argv) > 2):
    sys.stderr.write("error 10: Wrong arguments.\n")
    sys.exit(10)

# '-h' and '--help' CLI argument handling 
elif len(sys.argv) == 2:
    
    if (sys.argv[1] == "--help" or sys.argv[1] == "-h"):
        print("---------------------------------------------")
        print(" usage: python3.11 [--help | <SOL25 file name>]")
        print("---------------------------------------------")
        sys.exit(0)

    else:
        sys.stderr.write("error 10: Wrong arguments.\n")
        sys.exit(10)
# load from standard input into variable as string
try:
    sol_code = sys.stdin.read()
except OSError as e:
    sys.stderr.write(f"error 12: Reading from stdin failed: {e}.\n")
    sys.exit(12)  

# definition of grammar with custom regexes for various IDs
grammar = r"""
%import common.WS
%ignore WS
%import common.NEWLINE
%ignore NEWLINE
COMMENT: "\"" /.*?/ "\""
%ignore COMMENT

program: class program?

class: "class" CID ":" CID "{" method? "}" 

method: selector block  method?

selector: ID | ID2 selectortail?

selectortail: ID2 selectortail?

block: "["  blockpar? "|"  blockstat? "]" 

blockpar: ID3 blockpar?

blockstat: ID ":=" expr "."  blockstat? 

expr: exprbase exprtail

exprtail: ID | exprsel?

exprsel: ID2 exprbase exprsel?

exprbase: INT | STR | ID | CID | block | "(" expr ")"

ID: /[a-z_][a-zA-Z0-9]*/   
ID2: /[a-z_][:a-zA-Z0-9]*:/
ID3: /:[a-z]+[a-zA-Z0-9]*/ 
CID: /[A-Z_][a-zA-Z0-9]*/
STR: /\'.*\'/
INT: /[0-9]+/
"""

parser = Lark(grammar, start='program', parser='lalr')

# i use Lark parser to create abstract lark tree, that will allow me to cycle through its roots and create XML representation more easily
# Lark also detects invalid order of symbol based on grammar, so I can catch these errors and exit with relevant exit code.
try:
    lark_tree = parser.parse(sol_code)

except UnexpectedToken as err:

    if err.column == 1 and err.line == 1 and 'CLASS' in err.expected:

        sys.stderr.write("error 31: Missing 'Main' class.\n")
        sys.exit(31)
    else:
        print(err, end='')
        sys.stderr.write("error 22: Invalid syntax.\n")
        sys.exit(22)

except UnexpectedCharacters as err:
    
    print(err, end='')
    sys.stderr.write("error 21: Invalid character.\n")
    sys.exit(21)

# lists of predefined classes, methods and keywords
built_in_classes = ["Object", "Nil", "True", "False", "Integer", "String", "Block"]
keywords = ["class", "self", "super", "nil", "true", "false", "new"]

integer_methods = ["new", "from:", "equalTo:", "greaterThan:", "plus:", "minus:", "multiplyBy:", "divBy:", "asString:", "asInteger", "timesRepeat:"]
string_methods = ["new", "read", "print", "equalTo:", "asString", "asInteger", "concatenateWith:", "startsWith:endsBefore:"]

# block parameters are in lark tree nested, so to get their name, I use this recursive function
def getParameters(tree: Tree, plist: list): 

    name = ''
    for p in tree.children:
        
        if type(p) == Tree and p.data == "blockpar":
            getParameters(p, plist)
        elif type(p) == Token:
            name = str(p)
            name = name[1:]
            if name in keywords:
                sys.stderr.write(f"error 22: Invalid name of parameter: \'{name}\'.\n")
                sys.exit(22)
            else:
                plist.append(name)

# function recursively fill parameter 'mlist' with methods from given tree
def getMethods(tree: Tree, mlist: list): 
    name = ''
    for m in tree.children:

        if type(m) == Tree and m.data == "method":
            mlist.append(str(m.children[0].children[0]))
            getMethods(m, mlist)    

# with multiple assigns in lark tree, each 'blockstat' rule is nested in its previous, therefore i need to separate them so i can work with their subtrees individually
# for this i created list of lark trees
def getAssignTrees(tree: Tree, vlist: list, tlist: list):
    
    var_name = ''
    for p in tree.children:

        if type(p) == Tree and p.data == "blockstat":
            tlist.append(p) 
            getAssignTrees(p, vlist, tlist)
        elif type(p) == Token:
            var_name = str(p)
            vlist.append(var_name)

# function returns one of possible SOL25 data types based on 'var' parameter
def getLiteralType(var: str) -> str:

    match var:
        case "nil":
            return "Nil"
        case "true":
            return "True"
        case "false":
            return "False"
    
    pattern = re.search("[0-9]+", var)

    if pattern != None and pattern.group() == var:
        return "Integer"
    
    if var[0] == '\'' and var[-1] == '\'':
        return "String"

    if ord(var[0]) in range(ord('A'), ord('Z')):
        return "class"

    return "variable_name"


# function loops through given tree and appends selector to 'attrs_list' if one exists 
def findSelector(tree: Tree, attr_list: list):

    for child in tree.children:

        if type(child) == Token and (child.type == "ID" or child.type == "ID2"):
            attr_list.append(child.value)
                
        if type(child) == Tree and (child.data == "exprsel"):
            findSelector(child, attr_list)

#this function checks if selector that's being used with a certain class is valid according to its parent
def checkSelector(sel: str, class_type: str):

    if class_type == "Integer" and sel not in integer_methods: 
        sys.stderr.write(f"error 32: Cannot use \'{sel}\' with \'{class_type}\' class.\n")
        sys.exit(32)
    if class_type == "String" and sel not in string_methods:
        sys.stderr.write(f"error 32: Cannot use \'{sel}\' with \'{class_type}\' class.\n")
        sys.exit(32)

# function loops through tree and append class name to 'attrs_list'
def findClass(tree: Tree, attr_list: list):
    
    for child in tree.children:

        if type(child) == Token and child.type == "CID":
            attr_list.append(child.value)

        if type(child) == Tree and child.data == "exprbase":
            findClass(child, attr_list)


# function loops through tree and append variable value to 'attrs_list' (whichever type of value - string, integer, different variable,...)
def findVariable(tree: Tree, attr_list: list):
    for child in tree.children:

        if type(child) == Token and (child.type == "ID" or child.type == "INT"):
            attr_list.append(child.value)
                
        if type(child) == Tree and (child.data == "exprbase" or child.data == "exprsel" or child.data == "expr"):
            findVariable(child, attr_list)

# this function is used for finding parent among classes and creates a list consisting of [<child_class_name>, <parent_class_name>]
# I use this function e.g. to detect circular inheritance of classes
def findParent(tree: Tree,  tmp: list, parent_class: str):

    for child in tree.children:

        if type(child) == Tree and (child.data == "program" or child.data == "class"):

            if type(child.children[0]) == Token and child.children[0].value == parent_class:
                tmp.append(child.children[0].value)
                tmp.append(child.children[1].value)
            else:
                findParent(child, tmp, parent_class)


# main function responsible for constructing final XML tree 
def constructXml(parent: ET, roottype: str, rootattr: list, tree: Tree):
    
# -------------CLASS-------------
    if roottype == "class":

        t_class = ET.SubElement(parent, "class")
        t_class.set("name", rootattr[0])
        t_class.set("parent",rootattr[1]) # when defining class, parent is meant as attribute, not xml parent

        exp_classes = str(lark_tree).count("class")

        # semantics check for use of undefined classes
        if rootattr[1] not in built_in_classes and rootattr[1] not in defined_classes and len(defined_classes) == exp_classes: 
            sys.stderr.write(f"error 32: Use of undefined class \'{rootattr[1]}\'.\n")
            sys.exit(32)

        # semantics check for circular inheritance of classes
        class_info = []
        findParent(lark_tree,  class_info, rootattr[1])
        if rootattr[1] in defined_classes and rootattr[1] not in built_in_classes and class_info[1] not in built_in_classes:
            sys.stderr.write(f"error 35: Detected circular inheritance of class \'{rootattr[1]}\'.\n")
            sys.exit(35)

        has_run_method = False # assures that program has 'run' method included

        current_class_methods = []
        
        getMethods(tree, current_class_methods)
        for method in current_class_methods: # loop for creating all methods of a class
            
            # semantics check, ensuring that methods are not named after SOL keywords
            if method in keywords:
                sys.stderr.write(f"error 22: Invalid method name \'{method}\'.\nSOL25 keywords: {keywords}.\n")
                sys.exit(22)

            if current_class_methods.count(method) > 1:
                sys.stderr.write(f"error 35: Redefinition of \'{method}\' method detected.\n")
                sys.exit(35)

            if method == "run":
                has_run_method = True

            attrs = []
            attrs.append(method)

            for child in tree.children: # before creating 'method' subelement, i have to choose the right subtree
                if type(child) == Tree and child.data == "method":
                    tree = child 

            constructXml(t_class, "method", attrs, tree) # creates methods subelement, class is parent
            
            attrs.pop()

        # if until now no 'run' method was detected, error is raised
        if has_run_method == False and rootattr[0] == "Main":  
            sys.stderr.write("error 31: Missing 'run' method in 'Main' class.\n")
            sys.exit(31)


# -------------METHOD-------------
    if roottype == "method":

        t_method = ET.SubElement(parent, "method")
        t_method.set("selector", rootattr[0])
    
        
        for child in tree.children:            
            param_names = [] # list for storing parameter names
            if type(child) == Tree and child.data == "block": 

                exp_arity = rootattr[0].count(":")
                real_arity = str(child).count("blockpar") # i need count of current block parameters
                
                if int(real_arity) != exp_arity:
                    sys.stderr.write(f"error 33: Expected arity of \'{exp_arity}\' , got \'{real_arity}\' in \'{rootattr[0]}\' method.\n")
                    sys.exit(33)

                if rootattr[0] == "run" and int(real_arity) > 0:
                    sys.stderr.write("error 33: 'run' method cannot have any parameters.\n")
                    sys.exit(33)

                if int(real_arity) > 0:
                    getParameters(child, param_names)

                attrs = []                       
                attrs.append(str(real_arity)) # passing arity as first value of rootattr
                attrs.append(param_names) # second value will be list of variable names

                constructXml(t_method, "block", attrs, child) # creates block subelements, method is parent

        
# -------------BLOCK-------------
    if roottype == "block":

        t_block = ET.SubElement(parent, "block")
        t_block.set("arity", rootattr[0]) # used rootattr[0] for passing arity value
        
        if int(rootattr[0]) > 0: # if the block has parameters, their subelements will be created
            for order in range(1, len(rootattr[1])+1):
                attrs = []
                attrs.append(rootattr[1][order-1]) # parameter name
                attrs.append(str(order)) 
                constructXml(t_block, "parameter", attrs, tree)   

        for child in tree.children:
        
            if type(child) == Tree and child.data == "blockstat":
                
                num_of_assigns = str(child).count("blockstat") # how many 'assign' subelements will be created
                var_names = []
                nested_trees = []
                getAssignTrees(child, var_names, nested_trees)
                
                param_names = rootattr[1]
    
                # semantics check if formal parameter isn't redefined in the same block
                for var in var_names:
                    if var in param_names:
                        sys.stderr.write(f"error 34: Redefinition of local parameter \'{var}\'.\n")
                        sys.exit(34)

                # semantics check if formal parameters aren't defined more than once in the same block
                for param in param_names:
                    if param_names.count(param) > 1:
                        sys.stderr.write(f"error 35: Parameter \'{param}\' defined multiple times.\n")
                        sys.exit(35)
            
                for order in range(1, num_of_assigns+1):
                    attrs = []
                    attrs.append(str(order))
                    vars = []
                    vars.append(var_names[order-1])
                    attrs.append(vars)

                    if order > 1:                
                        constructXml(t_block, "assign", attrs, nested_trees[order-2])
                    else:
                        constructXml(t_block, "assign", attrs, child)
                
# -------------PARAMETER-------------
    if roottype == "parameter":
        t_param = ET.SubElement(parent, "parameter")
        t_param.set("name", rootattr[0])
        t_param.set("order", rootattr[1])
  
        # all block parameters can be considered as defined variables
        defined_variables.append(rootattr[0])

# -------------ASSIGN-------------
    if roottype == "assign":

        t_assign = ET.SubElement(parent, "assign")
        t_assign.set("order", rootattr[0])
        

        # first root attribute should be list of all detected variables
        # therefore for each variable in this list additional subtrees will be created
        for var in rootattr[1]:
            attrs = []
            attrs.append(var)

            # ensuring variable name is valid 
            if var in built_in_classes:

                sys.stderr.write(f"error 35: Variable cannot be named after built-in class.\nBuilt-in classes: {built_in_classes}.\n")
                sys.exit(35)
            
            elif var in keywords:
                
                sys.stderr.write(f"error 22: Variable name is a SOL25 keyword.\nSOL25 keywords: \'{keywords}\'.\n")
                sys.exit(22)


            constructXml(t_assign, "var", attrs, tree)

            child = tree.children[1] # child is in this case first lark subtree after variable name
            if type(child) == Tree and child.data == "expr":
                constructXml(t_assign, "expr", attrs, child)

# -------------VAR-------------

    if roottype == "var":

        t_var = ET.SubElement(parent, "var")
        t_var.set("name", rootattr[0])

        # parent to be 'assign' mean that given variable is to be defined just now, I can update 'defined_variables' list
        if parent.tag == "assign":
            defined_variables.append(rootattr[0])
        
        # if parent is not 'assign' -- 'var' is on the right side of expression -- I should check if variable was already defined
        elif rootattr[0] not in defined_variables:
            sys.stderr.write(f"error 32: Use of undefined variable \'{rootattr[0]}\'. \n")
            sys.exit(32)

# -------------EXPR-------------
    if roottype == "expr":

        t_expr = ET.SubElement(parent, "expr")

        for child in tree.children:
            
            if type(child) == Tree and child.data == "exprbase":
                attrs = []
                class_type = ""
                selector = ""

                tmp_var = []
                tmp_class = []
                tmp_sel = []

                 # based on first child from current subtree, we can determine what comes after '=:' symbol
                first = child.children[0]
                value = first

                # these two functions will search for variable values, variable names, classes and put them into 'tmp' lists
                findVariable(child, tmp_var)
                findClass(child, tmp_class)

                # first literal can be either some value...
                if len(tmp_var) > 0:
                    value = tmp_var[0]
                # ...or built-in class...
                elif len(tmp_class) > 0:
                    value = tmp_class[0]
                #...or completely new subtree
                else:
                    # when there's nested block or another expression, there need to be another subtree created
                    if type(first) == Tree:

                        if first.data == "expr": # if right side of expression is another expression...
                            
                            findClass(first, tmp_class)
                            findSelector(first.children[1], tmp_sel)
                            for s in tmp_sel:
                                checkSelector(s, tmp_class[0])

                            findVariable(first.children[1], tmp_var)
                            value = tmp_var[0]

                        elif first.data == "block": # if right side of expression is new block...

                            arity = str(first).count(":")
                            attrs.append(str(arity))
                            constructXml(t_expr, "block", attrs, first)
                            break

                # I can divide what kind of 'expr' subtree will be created based on its parent
                if parent.tag == "assign" or parent.tag == "arg":

                    exp_classes = str(lark_tree).count("class")

                    # based on datatype of value, additional subtrees are created
                    if getLiteralType(value) == "class":   

                        findSelector(tree.children[1], attrs)
                        if len(attrs) > 0:
                            selector = attrs[0]

                        # checking if class uses only allowed methods
                        if value in built_in_classes:
                            checkSelector(selector, value)

                        # checking if class was defined or is built-in
                        elif value not in defined_classes and len(defined_classes) == exp_classes:
                            
                            sys.stderr.write(f"error 32: Use of undefined class \'{value}\'.\n")
                            sys.exit(32)

                        else:
                            #semantics check for correct class method
                            #class info always have class, that's being used on 1st position and its parent on 2nd
                            class_info = []
                            findParent(lark_tree, class_info, value)
                            if len(class_info) > 0 and class_info[1] in built_in_classes:
                                checkSelector(selector, class_info[1])

                        # if no errors were raised, 'send' subelement is created
                        attrs.append(value)
                        constructXml(t_expr, "send", attrs, tree.children[1])

                    elif getLiteralType(value) == "variable_name":
                        
                        # check if variable was already defined
                        if value not in defined_variables:
                            sys.stderr.write(f"error 32: Use of undefined variable \'{value}\'.\n")
                            sys.exit(32)

                        attrs.append(value)
                        #if no errors were raised, 'var' subelement is created
                        constructXml(t_expr, "var", attrs, child)
                    
                    # if 'value' is not a variable or a class, I can work with whatever getLiteralType() function returns safely
                    else:
                        
                        selectors = []
                        vars = []
                        findSelector(tree.children[1], selectors)
                        findVariable(tree.children[1], vars)

                        # semantics check for invalid selectors
                        for sel in selectors:
                            if sel in keywords:
                                sys.stderr.write(f"error 22: Invalid selector \'{sel}\'.\n")
                                sys.exit(22)
                            
                        class_type = getLiteralType(value)
                        
                        # in case variable is a string a remove initial apostrophes
                        if class_type == "String" and value[0] == '\'' and value[-1] == '\'': 
                            value = value[1:-1]
                            # checking if string contains allowed characters only
                            for symbol in range(len(value)):
                                # following conditions should handle backslash escaping 
                                if value[symbol] == "\\" and value[symbol+1] != "\'" and value[symbol+1] != "\\" and value[symbol+1] != "n":
                                    if value[symbol-1] != "\\":
                                        sys.stderr.write(f"error 21: String character \'{value[symbol]}\' not allowed.\n")
                                        sys.exit(21)

                                if (value[symbol] < chr(32)): 
                                    sys.stderr.write(f"error 21: String character \'{value[symbol]}\' not allowed.\n")
                                    sys.exit(21)

                        # if no errors were raised and no selectors found, 'literal' subelement is created with 
                        if len(selectors) == 0:
                            attrs.append(class_type)        
                            attrs.append(value)
                            constructXml(t_expr, "literal", attrs, child)                    
                        else:
                        # if there's a selector detected, 'send' need to be created first
                            for i in range(len(selectors)):
                                attrs.append(selectors[i])
                                attrs.append(vars[i])
                                constructXml(t_expr, "send", attrs, child)
                                attrs = []
            else:

                if parent.tag == "send":
                    attrs = []
                    value = rootattr[0]

                    class_type = getLiteralType(value)
                    # based on class_name, we decide what subelement should be created
                    if class_type != "variable_name":
                        attrs.append(class_type)
                        attrs.append(value)
                        constructXml(t_expr, "literal", attrs, child)     
                    else:
                        attrs.append(value)
                        constructXml(t_expr, "var", attrs, child)

# -------------SEND-------------
    if roottype == "send":

        t_send = ET.SubElement(parent, "send")
        t_send.set("selector", rootattr[0])            
        selector = rootattr[0]
        
        child = tree.children[0]
        
        # in root attributes is selector on first position and class type on second
        attrs = []
        attrs.append(rootattr[1])

        # if first child of given tree in of type Tree, it means another 'expr' and all detected 'arg' subelements should be created
        if type(child) == Tree:
            constructXml(t_send, "expr", attrs, child)
            num_of_args = selector.count(":")

            for order in range(1, num_of_args+1):
                attrs.append(str(order))
                constructXml(t_send, "arg", attrs, child)
                attrs.pop()

        # if first child of given tree in Token, only new 'expr' is created
        if type(child) == Token:
            constructXml(t_send, "expr", attrs, tree)
        
# -------------LITERAL-------------
    if roottype == "literal":

        t_literal = ET.SubElement(parent, "literal")
        t_literal.set("class", rootattr[0])
        t_literal.set("value", rootattr[1])

# -------------ARG-------------
    if roottype == "arg":

        t_arg = ET.SubElement(parent, "arg")
        t_arg.set("order", rootattr[1])
        attrs = []

        # findVariable() function adds variable value to 'attrs' list to pass to 'expr' creation
        findVariable(tree.children[1], attrs)

        # no new subtree is need, therefore I can pass same tree to 'expr' creation
        constructXml(t_arg, "expr", attrs, tree)
            
t_root = ET.Element("program")
t_root.set("language", "SOL25") 
description = ''       

# if there are comments in source code, first will be in 'program' tree as description attribute
des_start = sol_code.find("\"")
des_end = sol_code.find("\"", des_start+1)

# I check only if closing parentheses were detected, because opening ones could be inside string value and that would lead to false output
if des_end != -1:
    description = sol_code[des_start+1:des_end]
    t_root.set("description", description)

if "Token('CID', 'Main')" not in str(lark_tree):
    sys.stderr.write("error 31: Missing 'Main' class.\n")
    sys.exit(31)

defined_classes = []
defined_variables = ["self", "super"]

# function is responsible for initial parsing of all classes and putting them each separately under 'program' XML root
def parseClasses(tree: Tree):

    for child in tree.children:

        if type(child) == Tree and child.data == "class":
            
            name = str(child.children[0])
            parent = str(child.children[1])

            # name of the class is passed as first attribute to constructXml, its parent is second
            attrs = []
            attrs.append(name)
            attrs.append(parent)

            if name not in defined_classes:
                defined_classes.append(name)
            else: # semantics check for class redefinition
                sys.stderr.write(f"error 35: Class \'{name}\' was already defined.\n")
                sys.exit(35)
            
            constructXml(t_root, "class", attrs , tree.children[0])

        # classes are all nested in each other -- I need to use recursion
        elif type(child) == Tree and child.data == "program":
            parseClasses(child)

parseClasses(lark_tree)

tree_str = ET.tostring(t_root, encoding="UTF-8") 
dom = xml.dom.minidom.parseString(tree_str)
pretty_xml = dom.toprettyxml(indent="   ")
try:
    print(pretty_xml, end="")
except OSError as e:
    sys.stderr.write(f"error 12: Writing to stdout failed: {e}\n")
    sys.exit(12)  # Exit with an error code

