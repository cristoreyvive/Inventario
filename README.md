# Inventory System

Simple inventory management system in C++ (outputs are without accents).

## Compilation

To compile on Windows (using MinGW):

```sh
g++ inventario.cpp -o inventario
```

## Usage

```sh
./inventario
```

The program creates and uses a binary file called `inventario.dat` in the same directory.

## Features

- Add products
- List active products
- Search products by category
- Search products by code
- Modify products
- Logical deletion

## Notes

- The program outputs do not contain accents or the letter "ñ".
- The source code is in a single file: `inventario.cpp`