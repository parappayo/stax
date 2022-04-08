# Specification

Informal specification of the Stax VM and compiler.

A lot of this spec is aspirational and may not get implemented. I'll try to update it as I go.


## Syntax

One instruction per line. Each instruction can take any number of args.

As a [BNF grammar](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form):

```
<program> := <data-list> | <instruction-list> | ""

<data-list> := <data> <EOL> <data-list> | <data> | ""

<instruction-list> := <instruction> <EOL> <instruction-list> | <instruction> | ""

<data> := <data-type> "(" <data-value> ")"

<data-type> := see list below

<instruction> := see list below

<type> := see list below
```


## Memory Model

The VM has a stack and a heap. The sizes of these are implementation-defined.


## Data Format

The following types are available.

### Value Types

* byte
* int32
* int64
* float32
* float64

### Refrence Types

* string
* bigint
* list
* buffer

Each value on the stack carries type information with it. When values are operated on, if the types are not as expected by the instruction, the intpreter halts.

If types of dynamic length (string, bigint, list) are written to heap and the end of the heap is encountered, the interpreter halts.


## Instruction Set

The VM is to accept the following instructions.

### Stack Manipulation

These basically come from Forth.

* dup `( a -- a a )` - duplicates the top value
* drop `( a -- )` - remove the top value
* depth `( -- a )` - adds the number of elements on the stack to the top of the stack
* swap `( a b -- b a )` - transposes the top two values

### Heap Manipulation

See: [PEEK and POKE](https://en.wikipedia.org/wiki/PEEK_and_POKE)

* peek `( a -- b )` - reads a value from the heap
* poke `( a -- )` - writes a value to the heap

### Input/Output

* cr `( -- )` - prints a newline to stdout
* emit `(a -- )` - prints top value to stdout

### Arithmetic

Follows the C arithimetic operators.

* add `( a b -- c )` - c is `a + b`
* sub `( a b -- c )` - c is `a - b`
* mul `( a b -- c )` - c is `a * b`
* div `( a b -- c )` - c is `a / b`

### Bitwise

Follows the C bitwise operators.

* and `( a b -- c )` - c is `a & b`
* or `( a b -- c )` - c is `a | b`
* not `( a -- b )` - c is `a ~ b`
* xor `( a b -- c )` - c is `a ^ b`
* lshift `( a b -- c )` - c is `a << b`
* rshift `( a b -- c )` - c is `a >> b`
