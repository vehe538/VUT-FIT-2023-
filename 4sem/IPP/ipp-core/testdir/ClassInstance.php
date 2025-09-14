<?php

namespace IPP\Student;

use DOMElement;

class ClassInstance extends Instance
{
    /**
     * @var array<MethodInstance> $methodObjects
    */
    protected array $methodObjects = [];
    public string $name;
    public string $input;
    public string $parent;

    function __construct(string $name, string $type, DOMElement $root, string $parent, string $input) 
    {
        $this->name = $name;
        $this->type = $type;
        $this->root = $root;
        $this->input = $input;
        $this->parent = $parent;
    }
    
    function parseChildren():void {

        foreach($this->root->getElementsByTagName("method") as $method) {

            $method_selector = $method->getAttribute('selector');
            
            // create method instance and add it to the array
            $newMethod = new MethodInstance($method_selector, "method", $method, $this->input);
            $newMethod->classObjects = $this->classObjects;
            
            $newMethod->parseChildren();
            $this->methodObjects[] = $newMethod;

        }

    }



}