# stax

Stack-based VM and compiler.

Demo project, not intended for production use.

## Usage

* `make`
* `./stax example/add.stax`

## Goals

* Learn stuff
* Have fun
* Simple to implement
* Embeddable in C

## Specification

See [SPEC.md](./SPEC.md) for details.

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

I'm not using a parser generator like lex/yacc, flex/bison, Antlr, etc. Instead I'm hand-rolling a bunch of stuff. Why? Because the goals are learning and simple implementation, and getting caught up in doing things properly is against the spirit of the thing. I just want to go fast and make mistakes.

## References

* [Stack-oriented programming](https://en.wikipedia.org/wiki/Stack-oriented_programming)
* [Forth lang](https://en.wikipedia.org/wiki/Forth_(programming_language))
* [Befunge lang](https://en.wikipedia.org/wiki/Befunge)
* [Crafting Interpreters](https://craftinginterpreters.com/)
