<?php


namespace IPP\Student;

use DOMElement;

class FalseClass extends ObjectClass
{


    function __construct(string $name) 
    {
        $this->value = false;
        $this->name = $name;
        $this->type = "Bool";
    }


    function not():TrueClass {
        return new TrueClass($this->name); 
    }

    /**
    * @param array<ObjectClass> $argv
    */
    function and(array $argv):FalseClass {
        return new FalseClass($this->name);
    }

    /**
    * @param array<ObjectClass> $argv
    */
    function or(array $argv):FalseClass|TrueClass {
        if ($argv[0]->value == false) {
            return new FalseClass($this->name);
        } else {
            return new TrueClass($this->name);
        }
    }

    /**
    * @param array<BlockClass> $argv
    */
    function ifTrueifFalse(array $argv):ObjectClass {
        return $argv[1]->value([]);

    }



}