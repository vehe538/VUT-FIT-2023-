<?php

namespace IPP\Student;

use DOMElement;


class BlockClass extends ObjectClass
{
    protected array $argv;
    protected array $parameters;

    protected array $variableObjects = [];
    protected array $assignsArray = [];
    protected string $arity;
    protected DOMElement $root;
    

    function __construct(string $arity, string $type, DOMElement $root, string $input, array $argv) 
    {
        $this->arity = $arity;
        $this->type = $type;
        $this->root = $root;
        $this->input = $input;
        $this->argv = $argv;
    }
    
    //behaviour is similar to BlockInstance, but with added parameters funcionality
    function parseChildren():void {

        $param_index = 0;
        foreach($this->root->childNodes as $child_node) {

            if ($child_node->nodeName == "assign") {
                $assignPair = [];

                // first define variable of the assign
                $variable = $child_node->childNodes[1];
                $variable_name = $variable->getAttribute('name');

                // if we are redefining, the old one needs to be removed from the array, so there'll be no duplicates
                foreach($this->variableObjects as $index => $obj) {

                    if ($variable_name == $obj->name) {
                        unset($this->variableObjects[$index]);
                        break;
                    }
                }

                // this is the 'expr' node right after the 'var' node 
                $expression = $child_node->childNodes[3];
                
                // from that expression instance is created and new value is added to the array
                $newExpr = new ExpressionInstance($expression, [], $this->input);
                $newObj = $newExpr->value($variable_name, $this->variableObjects);
                $this->variableObjects[] = $newObj;

                $assignPair[] = $variable_name;
                $assignPair[] = $newExpr;
                $this->assignsArray[] = $assignPair;
            }

            if ($child_node->nodeName == "parameter") {

                // if there are parameters in the block instance, we need to link it to the passed arguments
                $param_name = "";
                if ($child_node instanceof DOMElement) {
                    $param_name = $child_node->getAttribute('name');
                }

                // current argument's name will be changed and added to variableObjects[]
                $this->argv[$param_index]->name = $param_name;
                $this->variableObjects[] = $this->argv[$param_index];
                $param_index++;

            }

        }
    }

    /**
     * @param array<ObjectClass> $argv
     */
    function value(array $argv):BlockClass {

        $this->argv = $argv;
        $this->parseChildren();
        return $this;
    }

    public function isNumber():FalseClass {
        return new FalseClass($this->name);
    }

    public function isString():FalseClass {
        return new FalseClass($this->name);
    }

    public function isBlock():TrueClass {
        return new TrueClass($this->name);
    }

    public function isNil():FalseClass {
        return new FalseClass($this->name);
    }


}