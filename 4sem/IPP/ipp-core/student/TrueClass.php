<?php


namespace IPP\Student;

use DOMElement;

class TrueClass extends ObjectClass
{


    function __construct(string $name) 
    {
        $this->name = $name;
        $this->type = "True";
        $this->value = true;
    }


    function new():TrueClass {
        return new TrueClass($this->name);
    }

    /**
     * @param array<ObjectClass> $argv
     */
    function from(array $argv):TrueClass {
        return new TrueClass($this->name);
    }

    /**
     * @param array<ObjectClass> $values
     */
    public function identicalTo(array $values):TrueClass|FalseClass {
        if ($values[0]->type == $this->type) {
            return new TrueClass($this->name);
        } else {
            return new FalseClass($this->name);
        }
    }
    /**
     * @param array<ObjectClass> $values
     */
    public function equalTo(array $values):TrueClass|FalseClass {
        if ($values[0]->value == true) {
            return new TrueClass($this->name);
        } else {
            return new FalseClass($this->name);
        }
    }


    function toString():StringClass
    {
        return new StringClass($this->name, "String", "true");
    }

    function not():FalseClass {
        return new FalseClass($this->name); 
    }

    /**
    * @param array<ObjectClass> $argv
    */
    function and(array $argv):TrueClass|FalseClass {
        if ($argv[0]->value == true) {
            return new TrueClass($this->name);
        } else {
            return new FalseClass($this->name);
        }
    }

    /**
    * @param array<ObjectClass> $argv
    */
    function or(array $argv):TrueClass {
        return new TrueClass($this->name);
    }


    /**
    * @param array<BlockClass> $argv
    */
    function ifTrueifFalse(array $argv):ObjectClass {
        return $argv[0]->value([]);

    }



}