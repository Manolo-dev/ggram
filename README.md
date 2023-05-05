# Ggram

- [Ggram](#ggram)
	- [What is it ?](#what-is-it-)
	- [Installation or manual compilation](#installation-or-manual-compilation)
		- [Compilation](#compilation)
	- [What is a `.gg` syntax file ?](#what-is-a-gg-syntax-file-)
	- [Options](#options)
		- [Basic Usage](#basic-usage)
		- [Command line options](#command-line-options)
			- [Restype](#restype)
	- [Syntax](#syntax)
		- [Lexer](#lexer)
		- [Parser](#parser)
	- [Example](#example)

## What is it ?

Ggram is a lexer-parser generator. It translates a `.gg` file describing a language syntax using something very close to the [Backus–Naur form (BNF)](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form) into a lexer-parser written in cpp.

## Installation or manual compilation

### Compilation

Download the source code [here](https://github.com/Manolo-dev/ggram).

Then, in the source directory, run :

```sh
make
```

## What is a `.gg` syntax file ?

As said earlier, a `.gg` file describes a language syntax (so that ggram can generate a lexer-parser out of it).
We'll look at a syntax example for simple calculations :

```ggram
number "[1-9][0-9]*|0"

---

<program> ::= 
```

## Options

### Basic Usage

```sh
./ggram -f syntax_file_name.gg -o output_directory_name
./ggram syntax_file_name.gg -o output_directory_name
```

### Command line options

| Option | Description | Possible values |
| :---: | :---: | :---: |
| `-f` | Specify the syntax file to use. | A file path |
| `-o` | Specify the output directory. | A directory path |
| `-h` / `--help` | Display the help. | |
| `-v` / `--version` | Display the version name | |
| `-r` / `--restype` | Controls how the parser works interally | `ORS`, `TRY_CATCH`, `ERROR_TOKEN` |

#### Restype

TODO

## Syntax

### Lexer

```sh
terminal "regexp_expression"
```

A terminal can be ignored by the parser by adding `.ignore` before it.

The lexer return a vector of Token. A Token is an object containing the terminal name (type), the value of the terminal, line and column.

### Parser

```ggram
<rule_name> ::= <rule> ;
```

Rule can contain parenthesis for priority, brackets for optionality, braces for repetition and the `|` operator for alternatives. Rule can also be a terminal. Rule can be recursive (but not left recursive). If you want check the value of a token, you can use `"token_value"` as a rule if the token is a terminal and if the token is lexed by the lexer.

The main rule is the one named `program`. Without it, the parser won't be able to parse anything.

The parser return a tree of Token (yes, the Token object contains a vector of Token, the "children").

## Example

For simple arithmetic expressions :

```ggram
int "[1-9][0-9]*|0"
float "[1-9][0-9]*.[0-9]*|[0-9]*.[0-9]*"
operator "[+-*/]"
parenthesis "[()]"
fun "[fgh]"
var "[x-z]"
const "[a-e]"
.ignore "[\n\t\r ]"

---

<program> ::= { <expression> ";" } <expression> | <expression> ;

<expression> ::= <fun> "(" <expression> ")"
    | <term> <operator> <expression>
    | <term> ;

<term> ::= <int> | <float> | <var> | <const> | "(" <expression> ")" ;
```
