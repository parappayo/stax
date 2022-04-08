# stax

Stack-based VM and compiler.

Demo project, not intended for production use.

## Goals

* Learn stuff
* Have fun
* Simple to implement
* Embeddable in C

## Specification

See [SPEC.md](./SPEC.md) for details.

## Architecture

This project uses a traditional parser-compiler style arch which is probably overkill. It is divided into the following modules:

* tokenizer - converts the input text into a list of tokens
* parser - converts the tokens into an abstract syntax tree
* compiler - converts the abstract syntax tree into bytecode
* interpreter - executes the bytecode

## References

* [Stack-oriented programming](https://en.wikipedia.org/wiki/Stack-oriented_programming)
* [Forth lang](https://en.wikipedia.org/wiki/Forth_(programming_language))
* [Befunge lang](https://en.wikipedia.org/wiki/Befunge)
