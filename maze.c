#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// bar[0] : up
// bar[1] : right
// bar[2] : down
// bar[3] : left
typedef struct _maze {
	int id;
	short bar[4];
} MAZE;

void deleteBar(MAZE** maze, int i, int j, int not, int id)
{
	int z;
	for (z = 0; z < 4; z++)
	{
		if (maze[i][j].bar[z] == 0 && z != not)
		{
			if (z == 0)
				deleteBar(maze, i - 1, j, 2, id);
			else if (z == 1)
				deleteBar(maze, i, j + 1, 3, id);
			else if (z == 2)
				deleteBar(maze, i + 1, j, 0, id);
			else if (z == 3) {
				deleteBar(maze, i, j - 1, 1, id);
			}
		}
	}
	maze[i][j].id = id;
	return;
}

int main()
{
	int total, mazeHeight, mazeWidth, i, j, z, id = 0, k, flag_bar;
	MAZE **maze;
	FILE *fp;
	int* flag;

	srand(time(NULL));
	fp = fopen("maze.maz", "w");
	
	printf("WIDTH : ");
	scanf("%d", &mazeWidth);
	printf("HEIGHT : ");
	scanf("%d", &mazeHeight);

	total = mazeHeight * mazeWidth;
	maze = (MAZE**)malloc(sizeof(MAZE*) * mazeHeight);
	for (i = 0; i < mazeHeight; i++) {
		maze[i] = (MAZE*)malloc(sizeof(MAZE)*mazeWidth);
		for (j = 0; j < mazeWidth; j++)
		{
			maze[i][j].id = 0;
			for (z = 0; z < 4; z++)
				maze[i][j].bar[z] = 1;
		}
	}

	flag = (int*)malloc(sizeof(int)*(total + 1));
	for (i = 0; i < total + 1; i++)
		flag[i] = 0;

	//first line && flag initialize
	for (i = 0; i < mazeWidth; i++)
	{
		maze[0][id].id = id + 1;
		id++;
	}


	for (i = 0; i < mazeWidth - 1; i++)
	{
		if (rand() % 2 == 1)
		{
			maze[0][i].bar[1] = maze[0][i + 1].bar[3] = 0; // right // left
			maze[0][i + 1].id = maze[0][i].id;
		}
	}

	// from second line
	for (i = 0; i < mazeHeight - 1; i++)
	{
		for (j = 0; j < mazeWidth; j++)
			flag[maze[i][j].id]++;
		j = 0;
		while (j < mazeWidth)
		{
			k = flag[maze[i][j].id];
			flag_bar = 0;
			for (z = 0; z < k; z++)
			{
				if (rand() % 2 == 1)
				{
					maze[i][j].bar[2] = maze[i + 1][j].bar[0] = 0; // down // up
					maze[i + 1][j].id = maze[i][j].id;
					flag_bar++;
				}
				j++;
			}
			if (flag_bar == 0)
			{
				maze[i][j - 1].bar[2] = maze[i + 1][j - 1].bar[0] = 0; //down // up
				maze[i + 1][j - 1].id = maze[i][j - 1].id;
			}
		}
		for (j = 0; j < mazeWidth; j++)
			if (maze[i + 1][j].id == 0)
				maze[i + 1][j].id = ++id;

		for (j = 0; j < mazeWidth - 1; j++)
		{
			if (maze[i + 1][j].id != maze[i + 1][j + 1].id)
			{
				maze[i + 1][j].bar[1] = maze[i + 1][j + 1].bar[3] = 0; // right // left
				maze[i + 1][j + 1].id = maze[i + 1][j].id;
				deleteBar(maze, i + 1, j + 1, 3, maze[i + 1][j + 1].id);
			}
		}
		for (j = 0; j < total + 1; j++)
			flag[j] = 0;
	}

	// last line
	for (i = 0; i < mazeWidth - 1; i++)
	{
		if (maze[mazeHeight - 1][i].id != maze[mazeHeight - 1][i + 1].id)
		{
			maze[mazeHeight - 1][i].bar[1] = maze[mazeHeight - 1][i + 1].bar[3] = 0;	// right // left
			maze[mazeHeight - 1][i + 1].id = maze[mazeHeight - 1][i].id;
			deleteBar(maze, mazeHeight - 1, i + 1, 3, maze[mazeHeight - 1][i + 1].id);
		}
	}

	for (i = 0; i < mazeWidth; i++)
		fprintf(fp, "+-");
	fprintf(fp, "+\n");
	for (i = 0; i < mazeHeight; i++)
	{
		fprintf(fp, "|");
		for (j = 0; j < mazeWidth - 1; j++)
		{
			if (maze[i][j].bar[1] == 1)
				fprintf(fp, " |");
			else
				fprintf(fp, "  ");
		}
		fprintf(fp, " |\n+");
		for (j = 0; j < mazeWidth; j++)
		{
			if (maze[i][j].bar[2] == 1)
				fprintf(fp, "-+");
			else
				fprintf(fp, " +");
		}
		fprintf(fp, "\n");
	}

	for (i = 0; i < mazeHeight; i++)
		free(maze[i]);
	free(maze);
	free(flag);
	fclose(fp);
	return 0;
}