/*
 * Copyright (C) 2022 Robert Alex Marder (ram@robertmarder.com)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WORDFILE "wordlist.txt"

int roll_dice(const int min, const int max)
{
	/* this is ugly but this is the correct way in C to get a random number from a specific range using the standard library only */
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

/* this converts between real int numbers and ascii numbers, similar to atoi(). ascii '0' is 48 */
int ctoi(const int c)
{
	return c - '0';
}

void process()
{
	/* integer array to hold our dice roll results */
	int dice[5];

	/* file pointer */
	FILE * fp;

	/* character used as fgetc() buffer */
	int c;

	/* printf("dice roll was: "); /* debugging */

	/* roll the dice, and populate our dice array */
	for(int i = 0; i < 5; i++)
	{
		dice[i] = roll_dice(1, 6);
		/* printf("%i", dice[i]); /* debugging */
	}

	/* dice[] now contains what word we will use, so find it in the file and output it */
	fp = fopen(WORDFILE, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "Fatal Error: unable to open %s for reading.\n", WORDFILE);
		exit(1); /* nonzero exit status is often used to indicate failure to the caller. */
	}

	/* loop forever while we read through the file */
	/* we are using fgetc() here instead of fgets() because it's much safer. */
	/* generally working one character at a time is the best thing to do in plain C to avoid memory issues */
	while(1)
	{
		c = fgetc(fp);
		if(feof(fp) > 0) { break; } /* end of file reached, break the loop */

		if(ctoi(c) == dice[0])
		{
			/* we read something that matches first digit from our dice, check 2nd digit */
			c = fgetc(fp);
			if(ctoi(c) == dice[1])
			{
				/* second digit looks good, check 3rd digit */
				c = fgetc(fp);
				if(ctoi(c) == dice[2])
				{
					/* third digit looks good, check 4th digit */
					c = fgetc(fp);
					if(ctoi(c) == dice[3])
					{
						/* 4th digit looks good, check 5th and final digit */
						c = fgetc(fp);
						if(ctoi(c) == dice[4])
						{
							/* output all characters remaining on this line of the file, this is the word we want */
							while(1)
							{
								c = fgetc(fp);
								if(feof(fp) > 0) { break; } /* hit end of file */
								if(c == '\r') { break; } /* hit CR, assume end of line reached */
								if(c == '\n') { break; } /* hit NL, assume end of line reached */

								if(c == ' ') { continue; } /* skip space */
								if(c == '\t') { continue; } /* skip tab */

								printf("%c", c);
							}
						}
					}
				}
			}
		}
	}
	fclose(fp);
}

int main()
{
	int dice_times;
	int loop;

	/* seed the random number generator using the current time */
	time_t t;
	srand((unsigned) time(&t));


	printf("How many words do you want in your password? ");
	scanf("%d", &dice_times);
	printf("Creating password with %d words...\n", dice_times);

	loop = 0;
	while(loop < dice_times)
	{
		loop++;
		printf("Word %i: ", loop);
		process();
		printf("\n");
	}

	return 0;
}
