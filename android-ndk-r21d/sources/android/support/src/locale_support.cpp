#include <stdlib.h>
#include <xlocale.h>

long long strtoll_l(const char* nptr, char** endptr, int base, locale_t loc) {
  return strtoll(nptr, endptr, base);
}

unsigned long long strtoull_l(const char* nptr, char** endptr, int base,
                              locale_t loc) {
  return strtoull(nptr, endptr, base);
}

long double strtold_l(const char* nptr, char** endptr,
                      locale_t __unused locale) {
  return strtold(nptr, endptr);
}
