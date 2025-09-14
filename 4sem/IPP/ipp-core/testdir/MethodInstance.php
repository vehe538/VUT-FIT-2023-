<?php

namespace IPP\Student;

use DOMElement;

class MethodInstance extends Instance
{
    /**
     * @var array<BlockInstance> $blockObjects
     */
    protected array $blockObjects = [];
    protected string $selector;
    

    function __construct(string $selector, string $type, DOMElement $root, string $input) 
    {
        $this->selector = $selector;
        $this->type = $type;
        $this->root = $root;
        $this->input = $input;
    }


    function parseChildren():void
    {
        
        $block = $this->root->getElementsByTagName("block")[0];
        $arity = $block->getAttribute('arity');

        $newBlock = new BlockInstance($arity, "block", $block, $this->input);
        $newBlock->classObjects = $this->classObjects;
        
        $newBlock->parseChildren();
        $blockObjects[] = $newBlock;
    }

}