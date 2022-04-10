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

<instruction> := see list below

<data> := <data-type> "(" <data-value-list> ")"

<data-type> := see list below

<data-value-list> := <data-value> "," <data-value-list> | <data-value> | ""

<data-value> := alpha-numeric text
```

The above BNF is just a sketch for now, might get more formal later.


## Memory Model

The VM has a stack and a heap. The sizes of these are implementation-defined.


## Data Types

Stax code is explicit about what type of data is being pushed to the stack. Consider the following Forth-like code:

```
1 2 add emit
```

Simple enough, but what data format are those integers? 32-bit, 64-bit, string, float, Decimal, BigInt, other? Stax syntax requires the user to specify this:

```
int32{ 1, 2 } add emit
```

It is inefficient, but the type info is also added to the stack along with the data. This allows the interpreter to halt when incompatible types are detected.

For simplicity, all value types are 64 bits wide, reference types contain a pointer to the value on the heap.

The following types are available.

### Value Types

* byte
* int16
* int32
* int64
* uint16
* uint32
* uint64
* float
* double
* type

Type is implemented as an enum under the hood (or will be.)

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

Borrows some from Forth.

* dup `( a -- a a )` - duplicates the top value
* drop `( a -- )` - remove the top value
* depth `( -- a )` - adds the number of elements on the stack to the top of the stack
* swap `( a b -- b a )` - transposes the top two values
* assert `( a b c -- )` - halt with error message a if the values b and c are not equal
* convert `( a b --  c)` - changes data type of value a to type b

### Heap Manipulation

See: [PEEK and POKE](https://en.wikipedia.org/wiki/PEEK_and_POKE)

Note: peek will need to know what type it is reading, either this is a stack arg or it is part of the instruction.

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
* mod `( a b -- c )` - c is `a % b`

### Bitwise

Follows the C bitwise operators.

* and `( a b -- c )` - c is `a & b`
* or `( a b -- c )` - c is `a | b`
* not `( a -- b )` - c is `a ~ b`
* xor `( a b -- c )` - c is `a ^ b`
* lshift `( a b -- c )` - c is `a << b`
* rshift `( a b -- c )` - c is `a >> b`
