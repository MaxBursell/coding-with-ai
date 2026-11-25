// Filters CSV rows, keeping only those with given maximum age
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constexpr size_t INPUT_LINE_SIZE = 100; // Maximum line length
const char DELIMITER[] = ",";    // CSV delimiter

char *ifile, *ofile;
unsigned filter_age_min;
FILE *istream, *ostream;

const char USAGE[] =
    R"(Filters CSV rows, keeping only those with given minimum age
%s minimum-age [input-file] [output-file]

Example: 
%s 18 input.csv output.csv
%s 18 input.csv (outputs to stdout)
%s 18           (inputs from stdin, outputs to stdout)
)";

void filter_stream(FILE *istream, FILE *ostream) {
  char line[INPUT_LINE_SIZE];
  size_t line_no = 0;

  while (fgets(line, INPUT_LINE_SIZE, istream)) {
    ++line_no;

    if (!strchr(line, '\n') && !feof(istream)) {
      int character;
      while ((character = fgetc(istream)) != '\n' && character != EOF) {
      }
      fprintf(stderr, "Line %zu: Line is too long.\n", line_no);
      continue;
    }

    line[strcspn(line, "\r\n")] = '\0';
    char *separator = strchr(line, DELIMITER[0]);
    if (!separator || separator == line) {
      fprintf(stderr, "Line %zu: Name or age missing.\n", line_no);
      continue;
    }

    *separator = '\0';
    char *age_str = separator + 1;
    char *age_start = age_str;
    while (isspace((unsigned char)*age_start)) {
      ++age_start;
    }
    errno = 0;
    char *age_end;
    unsigned long parsed_age = strtoul(age_str, &age_end, 10);
    int age_was_parsed = age_end != age_str;
    while (isspace((unsigned char)*age_end)) {
      ++age_end;
    }
    if (!age_was_parsed || *age_end != '\0' || errno == ERANGE ||
        parsed_age > UINT_MAX || *age_start == '-') {
      fprintf(stderr, "Line %zu: Age value is invalid.\n", line_no);
      continue;
    }

    unsigned age = (unsigned)parsed_age;
    if (age >= filter_age_min) {
      fprintf(ostream, "%s, %u\n", line, age);
    }
  }
}

int main(int argc, char *argv[]) {
  switch (argc) {
  case 4:
    // minimum-age ifile ofile
    ofile = argv[3];
  case 3:
    // minimum-age ifile
    ifile = argv[2];
  case 2:
    // minimum-age
    if (!sscanf(argv[1], "%d", &filter_age_min)) {
      puts("First argument is not an age.");
      exit(EXIT_FAILURE);
    }
    break;
  default:
    printf(USAGE, argv[0], argv[0], argv[0], argv[0]);
    return EXIT_SUCCESS;
  }

  // Input file
  if (ifile) {
    istream = fopen(ifile, "r");

    if (!istream) {
      perror("");
      exit(EXIT_FAILURE);
    }
  } else // No file provided
    istream = stdin;

  // Output file
  if (ofile) {
    ostream = fopen(ofile, "w");

    if (!ostream) {
      perror("");
      exit(EXIT_FAILURE);
    }
  } else
    ostream = stdout;

  filter_stream(istream, ostream);
}