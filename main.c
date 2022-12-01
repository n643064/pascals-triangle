#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>

void sigwinch_handler(int s);

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Not enough arguments\n");
		return 0;
	}
	int n = atoi(argv[1]);
	int size = 0;
	for (int i = 1; i <= n; i++)
	{
		size += i + 1;
	}
	
	int** rows = malloc(n * sizeof(int*));
	int start[1] = {1};
	rows[0] = start;

	for (int i = 1; i < n; i++)
	{
		int* a = malloc((i+1) * sizeof(int));
		a[0] = 1;
		a[i] = 1;
		for (int o = 1; o < i; o++)
		{
			a[o] = rows[i-1][o-1] + rows[i-1][o];
		}
		rows[i] = a;
	}

	initscr();
	curs_set(false);
	noecho();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);

	signal(SIGWINCH, sigwinch_handler);
	
	int cx, cy;
	timeout(10);
	while (getch() != 'q')
	{
		getmaxyx(stdscr, cy, cx);
		cx /= 2;
		//cy /= 2;
		for (int i = 0; i < n; i++)
		{
			int* a = rows[i];
			int off_y = 1 + i;
			int off_x = cx - i*2 - 1;
			attron(COLOR_PAIR(2));
			for (int o = 0; o <= i; o++)
			{
				if (a[o] % 2 == 0) continue;
				mvaddstr(off_y, off_x+o*4, "  ");			

			}
			attroff(COLOR_PAIR(2));
		}
		refresh();
		
	}
	echo();
	curs_set(true);
	endwin();
}

void sigwinch_handler(int s)
{

	struct winsize win;
	ioctl(0, TIOCGWINSZ, &win);
	resizeterm(win.ws_row, win.ws_col);
	clear();
	refresh();
}

