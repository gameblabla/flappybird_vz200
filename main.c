/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include <stdio.h>
#include <games.h>
#include <vz.h>
#include "sprites.h"

#define K_EXIT  'Q'

//#include "pict.h"
/*void __FASTCALL__ put_pict(const unsigned char* spr);*/
void __FASTCALL__ flappybird_player(void);
void __FASTCALL__ wrap_gameplay(void);
void __FASTCALL__ trigger_gameover(void);
void __FASTCALL__ gameplay(void);
void __FASTCALL__ switch_mode(unsigned char mode);
void __FASTCALL__ ready_screen(void);
void __FASTCALL__ clearscreen();

int collision(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char w1, unsigned char h1, unsigned char w2, unsigned char h2);
void put_sprite_screen(int ortype, int x, int y, void *sprite);

unsigned char done, key, i, refresh, key_time, game_mode, time_event;
unsigned short score, highscore;

struct wrap
{
	unsigned char x;
	unsigned char state;
	
} scroll_warps[2];

struct player
{
	unsigned char x;
	unsigned char y;
	unsigned char frame;
	unsigned char state;
	unsigned char time_flap;
} flappy;

int main(void)
{
	key = 0;
	done = 1;
	highscore = 0;
	//put_pict(titlescreen);
	
	trigger_titlescreen();

	while(!(key==K_EXIT))
	{
		key = getk();
		switch(game_mode)
		{
			case 0:
			if (key=='S')
			{
				switch_mode(1);	
			}
			break;
			case 1:
				ready_screen();
			break;
			case 2:
				gameplay();
			break;
			case 3:
			if (key=='T')
			{
				trigger_titlescreen();
			}
			else if (key=='S')
			{
				switch_mode(1);	
			}
			break;
		}
	}
	return 0;
}

void __FASTCALL__ switch_mode(unsigned char mode)
{
	game_mode = mode;
	time_event = 0;
	
	switch(mode)
	{
		case 0:
		case 3:
			vz_clrscr();
			vz_bgrd(1);
			vz_mode(0);
		break;
		case 1:
			vz_bgrd(0);
			vz_mode(1);
			flappy.x = 16;
			flappy.y = 8;
			flappy.frame = 0;
			flappy.state = 0;
			flappy.time_flap = 0;
			key_time = 0;
			score = 0;
			refresh = 0;
			scroll_warps[0].x = 64;
			scroll_warps[1].x = 115;
			scroll_warps[0].state = 1;
			scroll_warps[1].state = 0;
		break;
	}
}

void __FASTCALL__ ready_screen(void)
{
	if (time_event > 60)
	{
		switch_mode(2);
	}
	else
	{
		time_event++;
	}
	putsprite(spr_or, flappy.x, flappy.y, flappy0);
}

void __FASTCALL__ gameplay(void)
{
	if (refresh > 1)
	{
		clearscreen();
		refresh = 0;
	}
	refresh++;	
	flappybird_player();
	wrap_gameplay();
		
	if (time_event > 60)
	{
		trigger_gameover();
	}
}

void __FASTCALL__ flappybird_player(void)
{
		switch(flappy.state)
		{
			case 0:
				flappy.frame++;

				if (flappy.time_flap > 0)
				{
					if (flappy.time_flap == 1) flappy.y = flappy.y - 5;
					else flappy.y = flappy.y - 6;
					flappy.time_flap--;
				}
				else
				{
					if (key == 'F' && key_time > 1)
					{
						flappy.time_flap = 2;
						key_time = 0;
						key = 0;
					}
					
					flappy.y = flappy.y + 1;
					if (key_time < 5) key_time++;
				}
			break;
			case 1:
				time_event++;
				flappy.y = flappy.y + 4;
			break;
		}
		
		if (flappy.y > 52) 
		{
			flappy.state = 1;
			flappy.y = 52;
		}
		else if (flappy.y < 4) 
		{
			flappy.state = 1;
			flappy.y = 4;
		}
		
		
		switch(flappy.frame)
		{
				case 0:
				case 1:
					putsprite(spr_or, flappy.x, flappy.y, flappy0);
				break;
				case 2:
				case 3:
				case 6:
				case 7:
					putsprite(spr_or, flappy.x, flappy.y, flappy1);
				break;
				case 4:
				case 5:
					putsprite(spr_or, flappy.x, flappy.y, flappy2);
				break;
				default:
				flappy.frame = 0;
		}	
}

void __FASTCALL__ wrap_gameplay(void)
{
	for(i=0;i<2;i++)
	{
		switch(scroll_warps[i].state)
		{
			case 0:
				put_sprite_screen(spr_or, scroll_warps[i].x, 0, pipe_down_12);
				put_sprite_screen(spr_or, scroll_warps[i].x, 40, pipe_up_24);
				if (collision(flappy.x, flappy.y, scroll_warps[i].x, 0, 14, 9, 12, 11) || collision(flappy.x, flappy.y, scroll_warps[i].x, 40, 14, 9, 12, 24))
				{
					flappy.state = 1;
				}
			break;
			case 1:
				put_sprite_screen(spr_or, scroll_warps[i].x, 0, pipe_down_24);
				put_sprite_screen(spr_or, scroll_warps[i].x, 52, pipe_up_12);
				if (collision(flappy.x, flappy.y, scroll_warps[i].x, 0, 14, 9, 12, 23) || collision(flappy.x, flappy.y, scroll_warps[i].x, 52, 14, 9, 12, 12))
				{
					flappy.state = 1;
				}
			break;
		}

		if (flappy.state == 0) scroll_warps[i].x = scroll_warps[i].x - 2;
		if (scroll_warps[i].x < 3)
		{
			score++;
			scroll_warps[i].state++;
			if (scroll_warps[i].state>1)
				scroll_warps[i].state = 0;
			scroll_warps[i].x = 115;
		}
	}
}

void __FASTCALL__ trigger_gameover(void)
{
	switch_mode(3);
	printf("GAME OVER\n\n");
	if (score > highscore) 
	{
		highscore = score;
		printf("NEW RECORD !!!\n\n");
	}
	printf("HIGHSCORE : %d\n", highscore);
	printf("YOUR SCORE IS %d\n\n", score);
	printf("PRESS T TO GO BACK TO TSCREEN\n");
	printf("OR PRESS S TO PLAY AGAIN");
}

void __FASTCALL__ trigger_titlescreen(void)
{
	switch_mode(0);
	printf("FLAPPY BIRD\n\n");
	printf("By gameblabla\n\n");
	printf("HIGHSCORE : %d\n\n", highscore);
	printf("PRESS S KEY TO START\n\n");
	printf("INGAME, PRESS F TO FLY\n");
}

void put_sprite_screen(int ortype, int x, int y, void *sprite)
{
	if (x > -1 || x < 129)
		putsprite(ortype, x, y, sprite);
}

int collision(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char w1, unsigned char h1, unsigned char w2, unsigned char h2)
{
	if ((x + w1 > x2) && (x < x2 + w2) && (y + h1 > y2) && (y < y2 + h2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void __FASTCALL__ clearscreen()
{
	#asm
	ld	hl,7000h	; base of graphics area
	ld	(hl),0
	ld	d,h
	ld	e,1			; de	= base_graphics+1
	ld	bc,2730
	ldir				; reset graphics window (2K)
	#endasm
}

/*
void __FASTCALL__ put_pict(const unsigned char* spr)
{
	unsigned char i, j;
	for(i=0;i<128;i++)
	{
		for(j=0;j<64;j++)
		{
			vz_plot(i, j, spr[i+(j*128)]);
		}
	}	
}*/
