#include "C:\Users\mksph\Desktop\opencv_root\project 1\OcvTest\SolutionSearch .h"
#include<iostream>
#include<stdio.h>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <list>
#include "math.h"
#include "RandomGenerator.h"
#include "utility"
#include "cstdio"

SolutionSearch::SolutionSearch(void)
{
	moving_directions[0].push_back(3); //for each position, tell the piece-8 which one in the array that it can be swapped with.

									   // So only two pieces are considered each time, when the piece-8 is moving.

	moving_directions[0].push_back(1);



	moving_directions[1].push_back(0);

	moving_directions[1].push_back(4);

	moving_directions[1].push_back(2);



	moving_directions[2].push_back(1);

	moving_directions[2].push_back(5);



	moving_directions[3].push_back(6);

	moving_directions[3].push_back(4);

	moving_directions[3].push_back(0);



	moving_directions[4].push_back(3);

	moving_directions[4].push_back(7);

	moving_directions[4].push_back(5);

	moving_directions[4].push_back(1);



	moving_directions[5].push_back(4);

	moving_directions[5].push_back(8);

	moving_directions[5].push_back(2);



	moving_directions[6].push_back(7);

	moving_directions[6].push_back(3);



	moving_directions[7].push_back(4);

	moving_directions[7].push_back(6);

	moving_directions[7].push_back(8);



	moving_directions[8].push_back(7);

	moving_directions[8].push_back(5);





	/*Pre-define the goal state, which is 0, 1, 2, ..., 8 order */

	int goal_value[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

	p_goal = goal_value;
}


SolutionSearch::~SolutionSearch(void)
{
}




/*			Student Implementation: here you need to implement the Breadth First Search Method						*/
/*																													*/
/*			Input: the first parameter is the random order of the 9 numbers, which you need to						*/
/*				   re-organize to make them into the correct order.													*/
/*				   the second parameter is actually an output. It returns or stores the moving						*/
/*				   path of the "empty space" that it is involved to make all the sub-images in						*/
/*				   the correct position. The integer seqence varible "solution" should store						*/
/*				   all the steps along the path																		*/
/*                                                                          										*/
/*			For example:																							*/
/*						Input:  data = {8, 4, 1, 3, 0, 2, 6, 7, 5 };												*/
/*						to make it into the correct order {0, 1, 2, 3, 4, 5, 6, 7, 8}								*/
/*						you need to make the following changes on the number 8, since the							*/
/*						number 8 represents the empty space, moving 8 to its neigboring								*/
/*						numbers equals to moving the corresponding number to the empty space.						*/
/*						Below it shows a demo of the steps:															*/
/*																													*/
/*  8 4 1   swap with 4   8 0 1   swap with 1    8 1 0   swap with 2     8 1 2   swap with 5    0 1 2				*/
/*  3 0 2  ------------>  3 4 2  ------------->  3 4 2  -------------->  3 4 8  --------------> 3 4 5  ---> End		*/
/*  6 7 5                 6 7 5                  6 7 5                   6 7 5                  6 7 8				*/
/*																													*/
/*			So from this example, the right path should be {1, 2, 5, 8}.											*/
/*			WHY? You may thought it was {4, 1, 2, 5}, since the number 8 has swapped with them in this order.		*/
/*			That is true. However, we do not care which number it swapped with, but which position the number 8		*/
/*          has gone through. As the numbers can be in any positions during different time, which give no hint		*/
/*			about where the number 8 is. In contrast, the positions are fixed. So we assume the positions are		*/
/*			in the same order as an increasing sequence:															*/
/*																													*/
/*																[0]  [1]  [2]										*/
/*											Fixed Position =    [3]  [4]  [5]										*/
/*																[6]  [7]  [8]										*/
/*																													*/
/*			Here, I use "[]" to distinguish the positions from the numbers. So now you can see, the number 8 starts */
/*			from position [4], then swapped with number 4, which goes to the position [1]; then swapped with number */
/*			1, which goes to the position [2]; then swapped with number 2, which goes to the position [5]; finally, */
/*			swapped with number 5, which goes to the position [8]. So the path you should assign to the parameter   */
/*			"&solution" with the path sequence {1, 2, 5, 8}.
*/


bool SolutionSearch::AStarSearch(int *data, vector<int> &solution)
{
	while (!queue_astar.empty())

		pattern_list.clear();

	parents_map.clear();

	goal_found = 0;

	solution.clear();



	p_original = data;

	Path p_start(data);

	queue_astar.push(p_start);



	while (!queue_astar.empty())

	{

		Path p_current = queue_astar.front(); //Take off the top element from the queue

		queue_astar.pop();



		if (goal_found == 1)

			break;

		if (p_current == p_goal)

		{

			goal_found = 1;

			break;

		}

		else

		{

			int cur_pos = findTargetPos(p_current); //Get the position of 8-piece



													//Find all possible children

			for (int child_idx = 0; child_idx < moving_directions[cur_pos].size(); child_idx++)

			{

				Path p_child = p_current;

				int swap_pos = moving_directions[cur_pos][child_idx];

				int temp = p_child.blocks[cur_pos];

				p_child.blocks[cur_pos] = p_child.blocks[swap_pos];

				p_child.blocks[swap_pos] = temp;





				//Check whether it is not in the existing list

				if (std::find(pattern_list.begin(), pattern_list.end(), p_child) == pattern_list.end())

				{

					pattern_list.push_back(p_child); //Insert the child to the list for redundancy check

					queue_astar.push(p_child); //Enqueue the child

					parents_map.insert(std::pair<Path, Path>(p_child, p_current)); //Put the child-parent relation into map





				}



			}



		}

	}



	//The following code is to trace back to find the path

	if (goal_found == 1)

	{

		printf("Find the goal! It is: \n");

		printPattern(p_goal);



		global_path.clear();





		global_path.push_back(findTargetPos(p_goal)); //put the last step into the path

		Path parent;

		map_it = parents_map.find(p_goal);

		parent = p_goal;

		while (!(parent == p_original) && map_it != parents_map.end())

		{

			parent = map_it->second;

			int target_pos = findTargetPos(parent);

			if (!(parent == p_original)) //The original state is only considered as the start point

				global_path.push_back(target_pos);

			map_it = parents_map.find(parent);

		}



		printf("path size: %d\n", global_path.size());

		for (int i = global_path.size() - 1; i >= 0; i--)

		{

			solution.push_back(global_path[i]);

			printf("%d, ", global_path[i]);

		}

		printf("\n");



		return true;

	}

	else

	{

		printf("Sorry the goal is not found\n");

		return false;

	}




	return 1;
}

int SolutionSearch::findTargetPos(Path p)

{

	int pos = 0;

	for (int i = 0; i < 9; i++)

	{

		if (p.blocks[i] == 8)

			pos = i;

	}

	return pos;

}





/*For test purpose*/
//****************************************************************
void SolutionSearch::printPattern(Path p)

{

	for (int i = 0; i < 9; i++)

	{

		printf("%d, ", p.blocks[i]);

		if ((i + 1) % 3 == 0 && i < 6)

			printf("\n");

	}

	printf("\n");

}
//************************************************************************

int SolutionSearch::heuristic(Path p, int g_x)
{
	p_cur = p;

	int m_dist = 0;

	int z[3][3];

	int k = 0;

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			z[i][j] = p_cur.blocks[k];

			k++;

		}

	}

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			int w = z[i][j];

			for (int m = 0; m < 3; m++) {

				for (int n = 0; n < 3; n++) {

					if (goal[m][n] == w)

						m_dist = m_dist + abs(i - m) + abs(j - n);

				}

			}

		}

	}

	return m_dist;
}
