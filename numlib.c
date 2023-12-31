#include "numlib.h"
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

void skipspaces(FILE *f) {
  while (1) {
    int c = fgetc(f);
    if (!isspace(c)) {
      if (c != EOF) {
        ungetc(c, f);
      }
      return;
    }
  }
}

int read_uint_ascii(FILE *f, uint32_t *out) {
  int read = 0;
  uint32_t num = 0;

  while (1) {
    int c = fgetc(f);

    if (c >= '0' && c <= '9') {
      num = num * 10 + (c - '0');
    } else {
      if (c != EOF) {
        ungetc(c, f);
      }
      if (read == 0) {
        if (c == EOF) {
          return EOF;
        } else {
          return 1;
        }
      } else {
        *out = num;
        return 0;
      }
    }
    read++;
  }
}

int read_double_ascii(FILE *f, double *out) {
  int read = 0;
  double num = 0.0;
  int s = 1;
  int d_seen = 0;
  double d_place = 1.0;

  while (1) {
    int c = fgetc(f);

    if (c == '-' && read == 0) {
      s = -1;
    } else if (c >= '0' && c <= '9') {
      num = num * 10.0 + (c - '0');
      if (d_seen) {
        d_place *= 0.1;
      }
    } else if (c == '.' && !d_seen) {
      d_seen = 1;
    } else {
      if (c != EOF) {
        ungetc(c, f);
      }
      if (read == 0) {
        if (c == EOF) {
          return EOF;
        } else {
          return 1;
        }
      } else {
        *out = s * num * d_place;
        return 0;
      }
    }
    read++;
  }
}

int read_uint_le(FILE *f, uint32_t *out) {
  int b0, b1, b2, b3;
  b0 = fgetc(f);

  if (b0 == EOF) {
    return EOF;
  }

  b1 = fgetc(f);
  b2 = fgetc(f);
  b3 = fgetc(f);

  if (b1 == EOF || b2 == EOF || b3 == EOF) {
    return 1;
  }

  *out =
    ((uint32_t)b0 )
    | ((uint32_t)b1 << 8)
    | ((uint32_t)b2 << 16)
    | ((uint32_t)b3 << 24);
  return 0;
}

int read_uint_be(FILE *f, uint32_t *out) {
   int b0, b1, b2, b3;
  b0 = fgetc(f);

  if (b0 == EOF) {
    return EOF;
  }

  b1 = fgetc(f);
  b2 = fgetc(f);
  b3 = fgetc(f);

  if (b1 == EOF || b2 == EOF || b3 == EOF) {
    return 1;
  }

  *out =
    ((uint32_t)b3)
    | ((uint32_t)b2 << 8)
    | ((uint32_t)b1 << 16)
    | ((uint32_t)b0 << 24);
  return 0;
}

int read_double_bin(FILE *f, double *out) {
  int b0 = fgetc(f);

  if (b0 == EOF) {
    return EOF;
  }

  int b1 = fgetc(f);
  int b2 = fgetc(f);
  int b3 = fgetc(f);
  int b4 = fgetc(f);
  int b5 = fgetc(f);
  int b6 = fgetc(f);
  int b7 = fgetc(f);

  if (b1 == EOF || b2 == EOF || b3 == EOF || b4 == EOF 
      || b5 == EOF || b6 == EOF || b7 == EOF) {
    return 1;
  }

  ((uint8_t*)out)[0] = (uint8_t)b0;
  ((uint8_t*)out)[1] = (uint8_t)b1;
  ((uint8_t*)out)[2] = (uint8_t)b2;
  ((uint8_t*)out)[3] = (uint8_t)b3;
  ((uint8_t*)out)[4] = (uint8_t)b4;
  ((uint8_t*)out)[5] = (uint8_t)b5;
  ((uint8_t*)out)[6] = (uint8_t)b6;
  ((uint8_t*)out)[7] = (uint8_t)b7;

  return 0;
}

int write_uint_ascii(FILE *f, uint32_t x) {
  if (fprintf(f, "%u", x) < 0) {
    return 1;
  } else {
    return 0;
  }
}

int write_double_ascii(FILE *f, double x) {
    if (fprintf(f, "%lf", x) < 0) {
    return 1;
  } else {
    return 0;
  }
}

int write_uint_le(FILE *f, uint32_t x) {
  fputc(x>>0,  f);
  fputc(x>>8,  f);
  fputc(x>>16, f);
  fputc(x>>24, f);
  return 0;
}

int write_uint_be(FILE *f, uint32_t x) {
  fputc(x>>24, f);
  fputc(x>>16, f);
  fputc(x>>8,  f);
  fputc(x>>0,  f);
  return 0;
}

int write_double_bin(FILE *f, double x) {
  uint8_t *x_ptr = (uint8_t*)&x;

  for (size_t i = 0; i < sizeof(double); ++i) {
    fputc(*(x_ptr + i), f);
  }
  return 0;
}
