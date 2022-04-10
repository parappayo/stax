# stax

Stack-based VM and compiler.

Demo project, not intended for production use.

## Usage

* `make`
* `./stax example/add.stax`

## Goals

* Learn stuff
* Have fun
* Shake some of the rust off of my C programming ability
* Simple to implement
* Embeddable in C

## Specification

See [SPEC.md](./SPEC.md) for details.

## Rant

I've wanted to mess around with hand-rolling parsers and compilers for a long time. I took a compilers course as an undergraduate and kinda sucked at it, but it was fun. There's something exhilerating about invention a notation to describe some code that does a thing and then making that thing work.

Like any other of my Gitlab repos, this is a sort of skunk-works project where the goal is to gather ideas and try stuff out. Getting stuff to work or be practical for use is secondary, if a priority at all.

## Architecture

This project uses a traditional parser-compiler style arch which is probably overkill. It is divided into the following modules:

* tokenizer
  * converts the input text into a list of tokens
  * strips whitespace and comments
* parser
  * converts the tokens into abstract syntax
  * handles text-to-binary conversion of data values
* compiler
  * converts the abstract syntax into bytecode
  * enforces endian format
  * will probably skip this step for now and have the interpreter use the abstract syntax
* interpreter
  * executes the bytecode
  * optionally provide a repl environment for live coding

I'm not using a parser generator like lex/yacc, flex/bison, Antlr, etc. Instead I'm hand-rolling a bunch of stuff. Why? Because the goals are learning and simple implementation, and getting caught up in doing things properly is against the spirit of the thing. I just want to go fast and make mistakes.

## Resources

The following list contains references to help learn about the problem domain:

* [Stack-oriented programming](https://en.wikipedia.org/wiki/Stack-oriented_programming)
* [Crafting Interpreters](https://craftinginterpreters.com/)
* [Algorithms + Data Structures = Programs](https://en.wikipedia.org/wiki/Algorithms_%2B_Data_Structures_%3D_Programs)
* [The Dragon Book](https://en.wikipedia.org/wiki/Compilers:_Principles,_Techniques,_and_Tools)
* [lex & yacc Book](https://www.oreilly.com/library/view/lex-yacc/9781565920002/)

### Programming Langs

Here are some tools that may influence my thinking, or that I aspire to learn more about.

* [Nim](https://nim-lang.org/)
* [Racket](https://racket-lang.org/)
* [Forth](https://en.wikipedia.org/wiki/Forth_(programming_language))
* [Befunge](https://en.wikipedia.org/wiki/Befunge)
