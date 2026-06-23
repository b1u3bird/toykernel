#include "string.h"
int strcmp(const char *a, const char *b) {
    while (*a != '\0' && *a == *b) {
        a++;
        b++;
    }
    return *(const unsigned char *)a - *(const unsigned char *)b;
}
int strncmp(const char *a, const char *b, unsigned int n) {
    if (n == 0) {
        return 0;
    }

    while (n-- && *a && *a == *b) {
        if (n == 0) {
            return 0;
        }
        a++;
        b++;
    }
    return *(const unsigned char *)a - *(const unsigned char *)b;
}
int strlen(const char *s) {
    int len = 0;
    while (*s != '\0') {
        s++;
        len++;
    }
    return len;
}
void *memset(void *s, int c, unsigned int n) {
    char *p = s;
    while (n--) {
        *p++ = (char)c;
    }
    return s;
}
