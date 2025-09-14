<?php

namespace IPP\Student;

use DOMElement;

class AssignInstance extends Instance
{

    protected string $variable_name;
    protected ExpressionInstance $expr;

    function __construct(string $variable_name, ExpressionInstance $expr) 
    {
        $this->variable_name = $variable_name;
        $this->expr = $expr;

    }
    
    function parseChildren():void {
        return;
    }


}