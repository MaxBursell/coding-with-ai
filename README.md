# C template for multiple main files

For https://c-programming.aydos.de

Select a C main file and press <kbd>F5</kbd> to start your program. There are two examples for testing: `hello.c` and `pi-estimator.c`.

## Test the CSV age filter

The `test-input.csv` file contains valid rows and malformed rows to check that
the age filter skips bad data without crashing. Compile and run it from the
workspace root:

```sh
g++ -std=c++23 -x c++ src/hello.c -o bin/hello
./bin/hello 18 test-input.csv
```

The program should print `Bob, 18` to standard output. `Alice` is below the
minimum age and is filtered out. The malformed rows are skipped, with
diagnostic messages printed to standard error.