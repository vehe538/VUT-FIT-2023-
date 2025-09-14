<?php

namespace IPP\Student;

use Composer\Autoload\ClassLoader;
use IPP\Core\AbstractInterpreter;
use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class Interpreter extends AbstractInterpreter
{
    public function execute():int
    {
        // TODO: Start your code here
        // Check IPP\Core\AbstractInterpreter for predefined I/O objects:
        // $this->stdout->writeString("stdout");
        // $this->stderr->writeString("stderr");

        
        $input = $this->input->readString();
        $dom = $this->source->getDOMDocument();
        $classObjects = [];

        foreach($dom->getElementsByTagName("class") as $class) {
            
            $class_name = $class->getAttribute('name');
            $class_parent = $class->getAttribute('parent');

            $newClass = new ClassInstance($class_name, "class", $class, $class_parent, (string)$input);
            $classObjects[] = $newClass;

        }

        foreach($classObjects as $class) {
            
            $class->classObjects = $classObjects;
            $class->parseChildren();
        }
        
        return ReturnCode::OK;
    }
}
