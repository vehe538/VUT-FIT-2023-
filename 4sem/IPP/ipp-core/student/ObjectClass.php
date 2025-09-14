<?php

namespace IPP\Student;

use IPP\Core\AbstractInterpreter;
use IPP\Core\Interface\InputReader;
use IPP\Core\Interface\OutputWriter;
use IPP\Core\Interface\SourceReader;
use IPP\Core\Settings;

class ObjectClass
{

    public string $name;
    public string $type;
    public string|int|bool|null|float $value;
    public string $input;

    function __construct(string $name, string $type, string|int|bool|null|float $value) 
    {
        $this->name = $name;
        $this->type = $type;
        $this->value = $value;

    }


    function new():ObjectClass {
        return $this;
    }

    /**
     * @param array<ObjectClass> $argv
     */
    function from(array $argv):ObjectClass {
        $tmp = new ObjectClass("","","");
        return $tmp;
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
    public function equalTo(array $values):ObjectClass|int {
        $tmp = new ObjectClass("", "", "");
        return $tmp;
    }

    public function asString():StringClass {
        $tmp = new StringClass("", "", "");
        return $tmp;
    }

    public function isNumber():ObjectClass {
        $tmp = new ObjectClass("", "", "");
        return $tmp;
    }

    public function isString():ObjectClass {
        $tmp = new ObjectClass("", "", "");
        return $tmp;
    }

    public function isBlock():ObjectClass {
        $tmp = new ObjectClass("", "", "");
        return $tmp;
    }

    public function isNil():ObjectClass {
        $tmp = new ObjectClass("", "", "");
        return $tmp;
    }



}