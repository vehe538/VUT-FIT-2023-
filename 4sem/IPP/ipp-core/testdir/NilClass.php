<?php 

namespace IPP\Student;

class NilClass extends ObjectClass
{


    function __construct(string $type)
    {
        $this->type = $type;    
        $this->name = "Nil";
        $this->value = "Nil";
    }


    function new():NilClass {
        return new NilClass("Nil");
    }

    /**
     * @param array<ObjectClass> $argv
     */
    function from(array $argv):ObjectClass {
        return new NilClass("Nil");
    }

    /**
     * @param array<ObjectClass> $values
     */
    public function identicalTo(array $values):TrueClass|FalseClass {
        if ($values[0] == $this) {
            return new TrueClass($this->name);
        } else {
            return new FalseClass($this->name);
        }
    }
    /**
     * @param array<ObjectClass> $values
     */
    public function equalTo(array $values):TrueClass|FalseClass {
        if ($values[0]->value == "Nil") {
            return new TrueClass($this->name);
        } else {
            return new FalseClass($this->name);
        }
    }

    public function asString():StringClass {
        return new StringClass($this->name, $this->type, "nil");
    }



    function toString():StringClass
    {
        return new StringClass($this->name, "Nil", "nil");
    }


    public function isNumber():FalseClass {
        return new FalseClass($this->name);
    }

    public function isString():FalseClass {
        return new FalseClass($this->name);
    }

    public function isBlock():FalseClass {
        return new FalseClass($this->name);
    }

    public function isNil():TrueClass {
        return new TrueClass($this->name);
    }
}