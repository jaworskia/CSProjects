#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>

//Adam Jaworski
//CS 1550 Tuesday/Thursday 2:30-3:45
//Project 1: Graphics Library
//September 20, 2014

typedef unsigned short color_t;

char * buffer;
int fileDescriptor;
struct termios settings;
int total;
fd_set set;

void init_graphics()
{
        fileDescriptor = open("/dev/fb0", O_RDWR); //open framebuffer
        //get framebuffer's information
        struct fb_var_screeninfo variable;
        ioctl(fileDescriptor,FBIOGET_VSCREENINFO, &variable);
        struct fb_fix_screeninfo fixed;
        ioctl(fileDescriptor,FBIOGET_FSCREENINFO, &fixed);
        //total bytes we'll need to map in
        total = (variable.yres_virtual)*(fixed.line_length);
        //map in said bytes (read/write, shared)
        buffer = mmap(0, total, PROT_WRITE | PROT_READ,
        MAP_SHARED, fileDescriptor, 0);
        ioctl(0,TCGETS, &settings); //get terminal settings
        settings.c_lflag &= ~ECHO;
        settings.c_lflag &= ~ICANON; //set these bits off
        ioctl(0, TCSETS, &settings); //set terminal settings
}

void exit_graphics()
{
        close(fileDescriptor); //close the framebuffer
        munmap(buffer, total); //unmap the memory
        settings.c_lflag |= ECHO;
        settings.c_lflag |= ICANON; //set these bits back on
        ioctl(0, TCSETS, &settings); //restore terminal settings
}

void clear_screen()
{
        write(0, "\033[2J", 7); //writing this clears the screen
}

char getkey()
{
        struct timeval time;
        time.tv_usec = 0; //0 ms
        time.tv_sec = 0; //0 seconds, so it returns immediately
        FD_SET(0, &set); //keep an eye on the terminal
        int number = select(1, &set, NULL, NULL, &time); //poll it
        if (number > 0) //if it's ready
        {
                FD_CLR(0, &set); //not sure if this is needed
                char key[1]; //to store what we read
                read(0, key, 1); //read it
                return key[0]; //return it
        }
        else
        {
                FD_CLR(0, &set);
                return 'Z'; //returns capital Z by default
        }
}

void sleep_ms(long ms)
{
        if (ms > 999) //will sleep for at most 999 ms
                ms = 999;
        struct timespec time;
        time.tv_sec = 0; //ignore seconds
        time.tv_nsec = ms*1000000; //convert ms to ns
        nanosleep(&time, NULL); //sleep
}

void draw_pixel(int x, int y, color_t color)
{
        int yoffset = y*1280; //1280 bytes per row
        int xoffset = x*2;  //2 bytes per pixel
        int offset = yoffset + xoffset;
        color_t front8 = color >> 8; //shift off back 8 bits
        color_t back8 = color & 255; //lop off all 1s in first 8 bits
        buffer[offset] = back8; //back 8 bits
        buffer[offset + 1] = front8;//front 8 bits
}

void draw_line(int x1, int y1,int height, color_t c)
{ //calls drawpixel a whole bunch of times
        int i = 0;
        while (i < height) //for each pixel in the column
        {
                draw_pixel(x1, y1 + i, c); //draw it
                i += 1;
        }
}

color_t make_color(color_t red, color_t green, color_t blue)
{
        if (red > 31) //we don't want values higher than this
                red = 31;
        if (green > 63)
                green = 63;
        if (blue > 31)
                blue = 31;
        red = red << 11; //shift red bits all the way to the left
        green = green << 5; //shift green bits to where they belong
        color_t black = 0; //all bits should be zero

        color_t fusion = black | red; //OR together into one
        fusion = fusion | green;
        fusion = fusion | blue;
        return fusion;
}

void draw_rect(int x1, int y1, int width, int height, color_t c)
{ //calls drawline a whole bunch of times
        int i = 0;
        while (i < width) //for each pixel column
        {
                draw_line(x1 + i, y1, height, c); //draw a line
                i += 1;
        }
}
