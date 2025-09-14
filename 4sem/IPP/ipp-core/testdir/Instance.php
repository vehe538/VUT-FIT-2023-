<?php

namespace IPP\Student;

use DOMElement;


class Instance
{

    protected string $type;
    protected string $input;
    protected DOMElement $root;
    public array $classObjects;

    function __construct(string $type, DOMElement $root, string $input) 
    {
        $this->type = $type;
        $this->root = $root;
        $this->input = $input;
    }

    function parseChildren():void {
        return;
    }



}