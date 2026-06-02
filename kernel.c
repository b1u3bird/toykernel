#include "vga.h"
#include "io.h"
void kernel_main(unsigned long magic,unsigned long addr){
    volatile char* video_memory = (volatile char *)VIDEO_ADDRESS;
    const char *s = "Hello,world";
    int pos = get_cursor_position();
    int cur = 0;
    while(s[cur]!='\0'){
        int offset = cur*2 + pos;
        video_memory[offset] = s[cur];
        video_memory[offset+1] = VGA_COLOR_LIGHT_GREEN;
        cur++;
    }
    while(1);
}

