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
    outb(0x3D4,0x0E);
    position |= ((int)inb(0x3D5)) << 8;
    outb(0x3D4,0x0F);
    position |= ((int)inb(0x3D5));
    return position;
}

void update_cursor_position(int x,int y){
    int position = x*VGA_SCREEN_WIDTH + y;
    outb(0x3D4,0x0E);
    outb(0x3D5,(unsigned char)((position>>8)&0xFF));
    outb(0x3D4,0x0F);
    outb(0x3D5,(unsigned char)((position)&0xFF));
}

void clear_screen(){
    volatile char* video_memory = (volatile char *)VIDEO_ADDRESS;
    int total_bytes = VGA_SCREEN_HEIGHT*VGA_SCREEN_WIDTH*2;
    for (int i = 0;i < total_bytes;i+=2){
        video_memory[i] = ' ';
    }
}
