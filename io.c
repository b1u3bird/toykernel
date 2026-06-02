#include "vga.h"
#include "io.h"
unsigned char inb(unsigned short port){
    unsigned char result;
    __asm__ volatile("inb %1,%0":"=a"(result):"Nd"(port));
    return result;
}
void outb(unsigned short port,unsigned char data){
    __asm__ volatile("outb %0,%1"::"a"(data),"Nd"(port));
}

int get_cursor_position(){
    int position = 0;
    outb(VGA_CRT_INDEX_PORT,VGA_CURSOR_HIGH_IDX);
    position |= ((int)inb(VGA_CRT_DATA_PORT)) << 8;
    outb(VGA_CRT_INDEX_PORT,VGA_CURSOR_LOW_IDX);
    position |= ((int)inb(VGA_CRT_DATA_PORT));
    return position;
}

void update_cursor_position(int x,int y){
    int position = x*VGA_SCREEN_WIDTH + y;
    outb(VGA_CRT_INDEX_PORT,VGA_CURSOR_HIGH_IDX);
    outb(VGA_CRT_DATA_PORT,(unsigned char)((position>>8)&0xFF));
    outb(VGA_CRT_INDEX_PORT,VGA_CURSOR_LOW_IDX);
    outb(VGA_CRT_DATA_PORT,(unsigned char)((position)&0xFF));
}

void clear_screen(){
    volatile char* video_memory = (volatile char *)VIDEO_ADDRESS;
    int total_bytes = VGA_SCREEN_HEIGHT*VGA_SCREEN_WIDTH*2;
    for (int i = 0;i < total_bytes;i+=2){
        video_memory[i] = ' ';
    }
}
