#include <string.h>

// i just copied this implementation from gcc/libiberty
// well its public domain 🤷

int strncmp(const char *s1, const char *s2, register size_t n) {
  register unsigned char u1, u2;

  while (n-- > 0)
    {
      u1 = (unsigned char) *s1++;
      u2 = (unsigned char) *s2++;
      if (u1 != u2)
	return u1 - u2;
      if (u1 == '\0')
	return 0;
    }
  return 0;
}