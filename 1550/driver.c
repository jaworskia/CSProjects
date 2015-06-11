#include <stdio.h>

//Adam Jaworski
//CS 1550 Tuesday/Thursday 2:30-3:45
//Project 1: Graphics Library Driver
//September 20, 2014

typedef unsigned short color_t;

void clear_screen();
void exit_graphics();
void init_graphics();
char getkey();
void sleep_ms(long ms);
color_t make_color(unsigned char r, unsigned char g, unsigned char b);

void draw_pixel(int x, int y, color_t color);
void draw_line(int x1, int y1, int height, color_t c);
void draw_rect(int x1, int y1, int width, int height, color_t c);

//compile like so: gcc -Wall driver.c library.c -o driver

int main()
{
        clear_screen();
        printf("\nWelcome to Skywriter!\n");
        printf("\nLeft: j, Right: l, Up: i, Down: k");
        printf("\nClear clouds: c, Set Draw Mode: m");
        printf("\n(Press q to quit)\n");
        printf("\n\nPress b to begin!\n\n\n\n\n\n\n\n\n\n\n\n");
        char character = 'Z'; //getkey() returns 'Z' when it can't read
        long mils = 99;
        init_graphics(); //initialize the graphics stuff
        while (character != 'b') //gives time to read instructions
        {
                character = getkey();
                sleep_ms(mils);
        }
        clear_screen(); //clear screen
        mils = 10; //balance excessive polling and excessive input lag
        color_t green = make_color(0, 63, 0); //make colors to use later
        color_t teal = make_color(0, 63, 31);
        color_t white = make_color(31, 63, 31);
        int xbase = 315;
        int ybase = 200;
        draw_rect(0, 0, 639, 300, teal); //sky
        draw_rect(0, 300, 639, 160, green); //grass
        int drawmode = 1;
        while (character != 'q') //type 'q' to quit
        {
                character = getkey(); //get input if it's there
                if (character == 'j' && xbase > 4) //stays in sky box
                        xbase -= 5;
                else if (character == 'l' && xbase < 629)
                        xbase += 5;
                else if (character == 'i' && ybase > 4)
                        ybase -= 5;
                else if (character == 'k' && ybase < 290)
                        ybase += 5;
                else if (character == 'm') //turn draw mode on/off
                {
                        if (drawmode == 1)
                                drawmode = 0;
                        else if (drawmode == 0)
                                drawmode = 1;
                }
                else if (character == 'c') //clear
                        draw_rect(0, 0, 639, 300, teal); //redraw sky
                draw_rect(xbase, ybase, 10, 10, white); //draw cloud
                if (drawmode == 0) //if not in draw mode, erase cloud
                {
                        sleep_ms(50); //so you can see the cursor still
                        draw_rect(xbase, ybase, 10, 10, teal);
                }
                sleep_ms(mils); //sleep
        }
        clear_screen(); //so we don't have a mess afterwards
        exit_graphics(); //undo the graphics stuff
        return 0;
}
