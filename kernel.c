#include "io.h"
#include "vga.h"
void kernel_main(unsigned long magic, unsigned long addr) {
    volatile char *video_memory = (volatile char *)VIDEO_ADDRESS;
    const char *s = "Hello,world";
    get_cursor_position();
    int cur = 0;
    while (s[cur] != '\0') {
	    putchar(s[cur]);
	    cur++;
    }
    while (1) {
        char c = keyboard_getchar();
        if (c == 0)
            continue;
        putchar(c);
    }
}
