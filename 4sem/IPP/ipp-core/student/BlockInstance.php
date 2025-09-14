<?php

namespace IPP\Student;

use IPP\Core\Interface\InputReader;
use DOMElement;


class BlockInstance extends Instance
{

    protected array $variableObjects = [];
    protected array $assignsArray = [];
    protected string $arity;    

    function __construct(string $arity, string $type, DOMElement $root, string $input) 
    {
        $this->arity = $arity;
        $this->type = $type;
        $this->root = $root;
        $this->input = $input;

    }
    
    
    function parseChildren():void {
        
        foreach($this->root->childNodes as $child_node) {

            if ($child_node->nodeName == "assign") {

                $assignPair = [];
                $assign = $child_node;

                $variable = $assign->childNodes[1];
                $variable_name = $variable->getAttribute('name');
                
                // this loop handles redefinitions of variables
                // if there's already an object with the same name as the new one stored, it is removed
                foreach($this->variableObjects as $index => $obj) {

                    if ($variable_name == $obj->name) {

                        unset($this->variableObjects[$index]);
                        break;
                    }
                }
                
                $expression = $assign->childNodes[3];
                $newExpr = new ExpressionInstance($expression, $this->classObjects, $this->input);
                $newObj = $newExpr->value($variable_name, $this->variableObjects);            

                $this->variableObjects[] = $newObj;

                $assignPair[] = $variable_name;
                $assignPair[] = $newExpr;
                $this->assignsArray[] = $assignPair;
            }
        }
    }
}