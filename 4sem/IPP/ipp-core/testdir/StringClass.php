<?php

namespace IPP\Student;

use IPP\Core\ReturnCode;

class StringClass extends ObjectClass 
{


    function read():StringClass {
        
        return new StringClass($this->name, "String", $this->input);
    }

    function print():ObjectClass {
        if (gettype($this->value) == "string"){
            $this->value = str_replace("\n", "", $this->value);
            echo $this->value;

        }
        return $this;
    }



    function asString():StringClass {
        return $this;
    }

    function asInteger():IntegerClass|NilClass {
        
        if (ctype_digit($this->value)) {
            $newObj = new IntegerClass($this->name, "Integer", $this->value);
            return $newObj;
        } else {

            return new NilClass("null");            

        }
    }


    /**
     * @param array<ObjectClass> $argv
     */
    function from(array $argv):ObjectClass {
        return new StringClass($this->name, "String", $argv[0]->value);
    }

    /**
     * @param array<ObjectClass> $values
     */
    function equalTo(array $values):TrueClass|FalseClass|int
    {   
        if (strcmp($this->value, (string)$values[0]->value) === 0) {
            return new TrueClass($this->name);
        } else {
            return new FalseClass($this->name);
        }

    } 

    /**
     * @param array<ObjectClass> $values
     */
    function concatenateWith(array $values):StringClass|NilClass {
        if (strcmp($values[0]->value, "string") === 0) {
            return new StringClass($this->name, "String", $this->value.$values[0]->value);
        } else {
            return new NilClass("Nil");
        }
    }


    /**
     * @param array<ObjectClass> $values
     */
    function startsWithendsBefore(array $values):StringClass {
        if ($values[1]->value - $values[0]->value <= 0) {
            $newObj = new StringClass($this->name, "String", "");
            return $newObj;
        } else {
            if (gettype($this->value) == "string") {
                $newObj = new StringClass($this->name, "String",substr($this->value, $values[0]->value, $values[1]->value));
            } else {
                $newObj = new StringClass("","","");
                echo "Incorrect value type\n";
            }
            return $newObj;
        }
    }
    public function isNumber():FalseClass {
        return new FalseClass($this->name);
    }

    public function isString():TrueClass {
        return new TrueClass($this->name);
    }

    public function isBlock():FalseClass {
        return new FalseClass($this->name);
    }

    public function isNil():FalseClass {
        return new FalseClass($this->name);
    }

}