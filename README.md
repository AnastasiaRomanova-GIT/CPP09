# C++ Module 09

The final module of the 42 C++ curriculum, focused on practical use of STL
containers, input parsing, data handling, and algorithms in C++98.

The repository contains three independent exercises:

- `ex00`: [Bitcoin Exchange](./ex00)
- `ex01`: [Reverse Polish Notation (RPN)](./ex01)
- `ex02`: [PmergeMe](./ex02)

## Exercises

### ex00: Bitcoin Exchange

`btc` calculates the value of a Bitcoin amount on a given date using the
historical exchange rates stored in `data.csv`.

The program:

- reads and validates entries in the `date | value` format;
- accepts values from 0 to 1000;
- validates calendar dates;
- uses the closest earlier exchange rate when the requested date is absent;
- reports invalid lines without stopping the rest of the input processing.

The executable expects one input-file argument and must be run from `ex00` so
that it can find `data.csv`.

```text
date | value
2011-01-03 | 3
2012-01-11 | 1
```

```bash
cd ex00
./btc input.txt
```

### ex01: Reverse Polish Notation

`RPN` evaluates a Reverse Polish Notation expression supplied as a single
command-line argument. Operands are single digits, and the supported operators
are `+`, `-`, `*`, and `/`.

```bash
cd ex01
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
```

The calculator uses integer arithmetic and rejects malformed expressions,
invalid tokens, insufficient operands, and division by zero.

### ex02: PmergeMe

`PmergeMe` sorts a sequence of non-negative integers with the Ford-Johnson
merge-insertion algorithm. The sequence is sorted independently using both
`std::vector` and `std::list`.

The implementation includes:

- input validation;
- recursive pair sorting;
- Jacobsthal-guided insertion order;
- bounded binary-search insertion;
- separate vector and list implementations;
- execution-time measurement for both containers;
- optional comparison-count tracking.

```bash
cd ex02
./PmergeMe 3 5 9 7 4
```

The program prints the original sequence, the sorted sequence, and the elapsed
processing time in microseconds for each container.

## Building

Each exercise has its own Makefile and must be built from its corresponding
directory:

```bash
cd ex00 && make
cd ../ex01 && make
cd ../ex02 && make
```

All three projects compile with:

```text
-Wall -Wextra -Werror -std=c++98
```

The standard Makefile targets are `all`, `clean`, `fclean`, and `re`. Each
exercise also provides `debug` and `exec_only`. In `ex02`, the
`count_operations` target builds a version that prints comparison counts:

```bash
cd ex02
make count_operations
./PmergeMe 3 5 9 7 4
```

## Topics Practised

- C++98
- STL containers
- input parsing and validation
- associative-container lookup
- stack-based expression evaluation
- recursion and binary search
- algorithm implementation and performance measurement
- comparison-count analysis

## Context

These exercises were developed as educational projects under the constraints
of the 42 C++ Module 09 subject.
