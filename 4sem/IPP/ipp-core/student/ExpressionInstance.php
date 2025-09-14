<?php

namespace IPP\Student;

use DOMElement;

class ExpressionInstance extends Instance
{

    /** 
     * @var array<string> 
    */
    private array $constructors = [
        "read",
        "from:", 
        "new", 
        "attr:"
    ];
    /** 
     * @var array<string> 
    */
    private array $builtins = [
        "asString",
        "asInteger", 
        "identicalTo:", 
        "equalTo:",
        "isNumber",
        "isString",
        "isBlock",
        "isNil",
        "greaterThan:",
        "plus:",
        "minus:",
        "multiplyBy:",
        "divBy:",
        "timesRepeat:",
        "concatenateWith:",
        "startsWith:endsBefore:",
        "whileTrue:",
        "not",
        "and:",
        "or:",
        "ifTrue:ifFalse:",
        "print"
    ];

    function __construct(DOMElement $root, array $classObjects, string $input)
    {
        $this->root = $root;
        $this->input = $input;
        $this->classObjects = $classObjects;
    }

    function value($variable_name, array &$variableObjects):ObjectClass {

        $subelement = $this->root->childNodes[1];
        $subelement_type = $this->root->childNodes[1]->nodeName;
        if ($subelement_type == "literal") {

            $literal_type = $subelement->getAttribute('class');
            $literal_value = $subelement->getAttribute('value');

            //based on literal type, object can be created
           
            $newObj = $this->findLiteral($literal_type, $literal_value, $variable_name);
            
            return $newObj;
        }
        if ($subelement_type == "var") {
            $var_name = $subelement->getAttribute('name');
            foreach($variableObjects as $obj) {
                    
                // first I need to find object in existing objects whose name matches the one on the right side of the expression
                if ($obj->name == $var_name) {
                    // then based on type of this object, new object with new name but existing type and value can be created
                    switch($obj->type) {

                        case "Integer":
                            $newObj = new IntegerClass($variable_name, "Integer", $obj->value);                                
                            break;

                        case "String":
                            $newObj = new StringClass($variable_name, "String", $obj->value);                                
                            break;

                        case "Nil":
                            $newObj = new NilClass("Nil");        
                            break;

                        case "True":
                            $newObj = new TrueClass($variable_name);
                            break;

                        case "False":
                            $newObj = new FalseClass($variable_name);
                            break;   
                        default:
                            $newObj = new ObjectClass($variable_name, "", "");
                            break;
                    }
                    return $newObj;
                }
            }

        }
        if ($subelement_type == "send") {
        
            $send_selector = $subelement->getAttribute('selector');
                
            $first_node = $subelement->childNodes[1];
            $argc = substr_count($send_selector, ":");

            // if message to be sent is built-in function
            if (in_array($send_selector, $this->builtins)) {
                
                $send_selector = str_replace([":"], "", $send_selector);

                $newExpr = new ExpressionInstance($this->root, $this->classObjects, $this->input);
                $send_to_var_name = "";
                $argv = [];

                if ($first_node->childNodes[1]->nodeName == "var") {

                    $newExpr = new ExpressionInstance($subelement, $this->classObjects, $this->input);
                    $send_to_var_name = $first_node->childNodes[1]->getAttribute('name');

                } else if ($first_node->nodeName == "arg") {
                    
                    // if message has arguments, we need to process them
                    foreach($subelement->childNodes as $child) {

                        if ($child->nodeName == "arg") {

                            $arg = $child;
                            $arg_order = $arg->getAttribute('order');
                            $expr_arg = $child->childNodes[1];

                            $newExprArg = new ExpressionInstance($expr_arg, $this->classObjects, $this->input);
                        
                            $argObj = $newExprArg->value("arg".$arg_order, $variableObjects);
                            $argv[] = $argObj;  

                        }
                        if ($child->nodeName == "expr") {

                            $newExpr = new ExpressionInstance($child, $this->classObjects, $this->input);
                            $send_to_var_name = $child->childNodes[1]->getAttribute('name');

                            if (!$send_to_var_name) {
                                $send_to_var_name = $variable_name;
                            }
                        }
                    }

                } else {

                    $newExpr = new ExpressionInstance($first_node, $this->classObjects, $this->input);
                    $send_to_var_name = $variable_name;
                    
                }

                // lastly variable, that is going to be receiving the message is created and the message is sent
                $send_to_var = $newExpr->value($send_to_var_name, $variableObjects);
                foreach($variableObjects as $obj) {

                    if ($send_to_var_name == $obj->name || $send_to_var_name == $variable_name) {  
                        
                        //based on number of arguments, $argv array is passed with the message
                        if ($argc == 0) {
                            if ($variable_name != $send_to_var_name) {
                                $newObj = $obj->$send_selector();
                            } else {
                                $newObj = $send_to_var->$send_selector();
                            }
                        } else {
                            $newObj = $send_to_var->$send_selector($argv);
                        }
                        if ($newObj) {
                            $newObj->name = $variable_name;
                        }
                        return $newObj;
                    }
                }
                if ($argc == 0) {
                    return $send_to_var->$send_selector();
                } else {
                    return $send_to_var->$send_selector($argv);
                }
                

            // if message to be sent is a constructor
            } else if (in_array($send_selector, $this->constructors)) {

                $send_selector = str_replace([":"], "", $send_selector);

                $newExpr = new ExpressionInstance($this->root, $this->classObjects, $this->input);
                $send_to_var_name = "";
                $argv = [];
                
                if ($first_node->nodeName == "arg") {
                    
                    //process arguments
                    $argv = [];
                    foreach($subelement->childNodes as $child) {
                        if ($child->nodeName == "arg") {

                            $arg = $child;
                            $arg_order = $arg->getAttribute('order');
                            $expr_arg = $child->childNodes[1];

                            $newExprArg = new ExpressionInstance($expr_arg, $this->classObjects, $this->input);
                        
                            $argObj = $newExprArg->value("arg ".$arg_order, $variableObjects);
                            
                            $argv[] = $argObj;
                        }
                        if ($child->nodeName == "expr") {

                            $newExpr = new ExpressionInstance($child, $this->classObjects, $this->input);
                            $send_to_var_name = $variable_name;

                        }
                    }

                } else {

                    // any other case means, that there's nested 'send'
                    $newExpr = new ExpressionInstance($first_node, $this->classObjects, $this->input);
                    $send_to_var_name = $variable_name;

                } 

                // before sending message, receiver of the message must be defined
                $send_to_var = $newExpr->value($send_to_var_name, $variableObjects);
                $send_to_var->input = $this->input;
                
                if ($argc == 0) {
                    $newObj = $send_to_var->$send_selector();
                } else {
                    $newObj = $send_to_var->$send_selector($argv);
                }
                
                return $newObj;
            } 
        }

        // block is created, its instance is returned for later processing
        if ($subelement_type == "block") {
            
            $argv = [];
            $arity = $subelement->getAttribute('arity');
            $newObj = new BlockClass($arity, "block", $subelement, $this->input, $argv);
            $newObj->name = $variable_name;
            
            return $newObj;

        }
        $newObj = new NilClass("");
        return $newObj;
    }



    // this function assigns correct literal type to the variable even when inheriting from other classes
    // using recursive calls and creates correct object
    function findLiteral(string $literal_type, string $literal_value, string $variable_name):ObjectClass {
        $newObj = new ObjectClass("","","");
        switch ($literal_type) {
            case "Integer":
                $newObj = new IntegerClass($variable_name, "Integer", (int)$literal_value);
                break;

            case "String":
                $newObj = new StringClass($variable_name, "String", (string)$literal_value);
                break;

            case "Nil":
                $newObj = new NilClass("Nil");
                break;

            case "True":
                $newObj = new TrueClass($variable_name);
                break;

            case "False":
                $newObj = new FalseClass($variable_name);
                break;

            case "class":
                

                switch($literal_value) {
                    case "String":
                        $newObj = new StringClass($variable_name, $literal_value, "");
                        break;
                    case "Integer":
                        $newObj = new IntegerClass($variable_name, $literal_value, "");
                        break;
                    case "True":
                        $newObj = new TrueClass($variable_name);
                        break;
                    case "False":
                        $newObj = new FalseClass($variable_name);
                        break;
                    case "Nil":
                        $newObj = new NilClass("Nil");
                        break;
                    default:

                        foreach($this->classObjects as $classObj) {
                            
                            if ($literal_value == $classObj->name) {
                                $newObj = $this->findLiteral($literal_type, $classObj->parent, $variable_name);                               
                            }
                        }
                        break;
                }
                break;

            default:
                $newObj = new ObjectClass($variable_name, "", "");
                break;
        }
        return $newObj;
    }
}