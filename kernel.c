#define VIDEO_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x07
void kernel_main(unsigned long magic,unsigned long addr){
    volatile char* video_memory = (volatile char *)VIDEO_ADDRESS;
    char x[] = "Hello,world";
    int cur = 0;
    while(x[cur]!='\0'){
        int offset = cur*2;
        video_memory[offset] = x[cur];
        video_memory[offset+1] = WHITE_ON_BLACK;
        cur++;
    }
    return;
}
