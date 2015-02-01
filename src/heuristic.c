/*
 * heuristic.c
 *
 *  Created on: Feb 1, 2015
 *      Author: gbillig
 */
#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"

int twoLine[128][2];
int threeLine[128][3];
int twoLineCounter = 0;
int threeLineCounter = 0;


int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int array_eq(int *x, int *y, int n)
{
	int i;
    for (i=0; i<n; i++)
        if (x[i] - y[i] != 0)
            return 0;
    return 1;
}

int heuristic(gridType* gridRef) {
	largeGridType newGrid;
	int i,j;
	for (i=0; i<8; i++) {
		for (j=0; j<9; j++) {
			if (i==0 || i==7 || j==0 || j==8) {
				newGrid[i][j] = EDGE;
			} else {
				newGrid[i][j] = (*gridRef)[i-1][j-1];
			}
		}
	}

	/* Attribute definitions:
	 *
	 * Name: 4 in a line
	 * Value: +INF
	 */

	int value = 0;
	value += calc_lines(newGrid);

	for(i=0; i<twoLineCounter; i++) {
		printf("2line found : {%d, %d, %d}\n", twoLine[i][0], twoLine[i][1], twoLine[i][2]);
	}
	for(i=0; i<threeLineCounter; i++) {
		printf("3line found : {%d, %d, %d}\n", threeLine[i][0], threeLine[i][1], threeLine[i][2]);
	}

	newGrid[2][3] = 10;
	printf("Reference value: %d\nLocal Value: %d\n", (*gridRef)[2][3], newGrid[2][3]);

	return value;
}

int calc_lines(largeGridType grid) {
	int i,j;
	int PLAYER = P1;
	for (i=1; i<7; i++) {
		for (j=1; j<8; j++) {
			if (grid[i][j] == PLAYER) {
				search(grid, i, j, 0, PLAYER);
				search(grid, i, j, 1, PLAYER);
				search(grid, i, j, 2, PLAYER);
				search(grid, i, j, 3, PLAYER);
			}
		}
	}
	return 0;
}

void search(largeGridType grid, int i, int j, int direction, int PLAYER) {
	int x, y, xOffset, yOffset;
	int line[4] = {0};
	line[0] = j + i*8;
	int lineCounter = 1;

	switch(direction) {
		case 0 :
			xOffset = 1;
			yOffset = 0;
			break;
		case 1 :
			xOffset = 0;
			yOffset = 1;
			break;
		case 2 :
			xOffset = 1;
			yOffset = 1;
			break;
		case 3 :
			xOffset =  1;
			yOffset = -1;
			break;
	}

	//search first direction
	int searching = 1;
	int searchCounter = 1;
	while (searching) {
		x = j+xOffset*searchCounter;
		y = i+yOffset*searchCounter;
		if (grid[y][x] == PLAYER) {
			line[lineCounter] = x + y*8;
			lineCounter++;
			searchCounter++;
		} else {
			searching = 0;
		}
	}

	//search second direction
	searching = 1;
	searchCounter = 1;
	while (searching) {
		x = j+xOffset*searchCounter*(-1);
		y = i+yOffset*searchCounter*(-1);
		if (grid[y][x] == PLAYER) {
			line[lineCounter] = x + y*8;
			lineCounter++;
			searchCounter++;
		} else {
			searching = 0;
		}
	}

	int lineSize = 0;
	int n;
	for (n=0; n<4; n++) {
		if (line[n] != 0) {
			lineSize++;
		}
	}

	int newLine;
	if (lineSize > 1) {
		qsort(line, 4, sizeof(*line), cmpfunc);
		if (lineSize == 2) {
			newLine = 1;
			for(n=0; n<twoLineCounter; n++) {
				if (array_eq(&line[2], &twoLine[n][0], 2)) {
					newLine = 0;
					break;
				}
			}
			if (newLine) {
				twoLine[twoLineCounter][0] = line[2];
				twoLine[twoLineCounter][1] = line[3];
				twoLineCounter++;
			}
		}
		if (lineSize == 3) {
			newLine = 1;
			for(n=0; n<threeLineCounter; n++) {
				if (array_eq(&line[1], &threeLine[n][0], 3)) {
					newLine = 0;
					break;
				}
			}
			if (newLine) {
				threeLine[twoLineCounter][0] = line[1];
				threeLine[twoLineCounter][1] = line[2];
				threeLine[twoLineCounter][2] = line[3];
				threeLineCounter++;
			}
		}
		if (lineSize == 4) {
			printf("CONNECT FOUR!\n");
		}

		//printf("line found : {%d, %d, %d, %d}\n", line[0], line[1], line[2], line[3]);
	}
}
