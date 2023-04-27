# Ggram

## What is it ?

Ggram is a lexer-parser generator. It translates a `.gg` file describing a language syntax using something very close to the [Backus–Naur form (BNF)](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form) into a lexer-parser written in cpp.

## Installation or manual compilation


## What is a `.gg` syntax file ?

As said earlier, a `.gg` file describes a language syntax (so that ggram can generate a lexer-parser out of it).
We'll look at a syntax example for simple calculations :
```
number "[1-9][0-9]*|0"

---

<program> ::= 
```

## Options

### Basic Usage

```
.\ggram -f syntax_file_name.gg -o output_directory_name
```

### Help

```
.\ggram -h
```
or
```
.\ggram --full-help
```

The first show a shorten version of the options descriptions on this page


### Version

```
.\ggram -v
```a


## How to compile it ?
