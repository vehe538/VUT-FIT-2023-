<?php


namespace IPP\Student;

use IPP\Core\AbstractInterpreter;
use IPP\Core\ReturnCode;

class IntegerClass extends ObjectClass 
{

    /**
     * @param array<ObjectClass> $argv
     */
    function from(array $argv):ObjectClass {
        return new IntegerClass($this->name, "Integer", $argv[0]->value);
    }

    /**
     * @param array<ObjectClass> $values
     */
    function equalTo(array $values):ObjectClass {
        if ($this->value == $values[0]->value) {
            return new TrueClass($this->name);
        } else {
            return new FalseClass($this->name);
        }
    }

    /**
     * @param array<ObjectClass> $values
    */
    function greaterThan(array $values):TrueClass|FalseClass {
        if ($this->value > $values[0]->value) {
            return new TrueClass($this->name);
        } else {
            return new FalseClass($this->name);
        }
    }

    /**
     * @param array<ObjectClass> $values
    */
    function plus(array $values):IntegerClass|int {

        if (ctype_digit($this->value) || gettype($this->value) == "integer") {
            return new IntegerClass($this->name, "Integer", $this->value + $values[0]->value);
        } else {
            return ReturnCode::INTERPRET_TYPE_ERROR;
        }
    }

    /**
     * @param array<ObjectClass> $values
    */
    function minus(array $values):IntegerClass|int {
        if (gettype($this->value) == "integer") {
            return new IntegerClass($this->name, "Integer", $this->value - $values[0]->value);
        } else {
            return ReturnCode::INTERPRET_TYPE_ERROR;
        }
    }

    /**
     * @param array<ObjectClass> $values
    */
    function multiplyBy(array $values):IntegerClass|int {
        if (gettype($this->value) == "integer") {
            return new IntegerClass($this->name, "Integer", $this->value * $values[0]->value);
        } else {
            return ReturnCode::INTERPRET_TYPE_ERROR;
        }
    }


    /**
     * @param array<ObjectClass> $values
    */
    function divBy(array $values):IntegerClass|int {
       
        if ($values[0]->value == 0) {
            echo "Division by zero\n";
            return ReturnCode::INTERPRET_TYPE_ERROR;
        } else {
            if (gettype($this->value) == "integer") {
                return new IntegerClass($this->name, "Integer", $this->value / $values[0]->value);
            } else {
                return ReturnCode::INTERPRET_TYPE_ERROR;
            }
        }
    }


    function asString():StringClass {
        return new StringClass($this->name, "String", (string)$this->value); 
    }

    function asInteger():IntegerClass {
        return $this;
    }
    /**
     * @param array<ObjectClass> $values
     */
    function timesRepeat(array $values):void {
        return;
    }

    public function isNumber():TrueClass {
        return new TrueClass($this->name);
    }

    public function isString():FalseClass {
        return new FalseClass($this->name);
    }

    public function isBlock():FalseClass {
        return new FalseClass($this->name);
    }

    public function isNil():FalseClass {
        return new FalseClass($this->name);
    }
    
}