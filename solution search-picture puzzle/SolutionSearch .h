



/* To make the code compatible, different search algorithms have the same input and output parameters                */


#pragma once

#include <vector>       // std::vector
#include<iostream>
#include<queue>
#include<stack>
#include<map>

using namespace std;

class SolutionSearch
{
	struct Path
	{
		int blocks[9];
		Path() {};
		Path(int* data)
		{
			for (int i = 0; i < 9; i++) {
				blocks[i] = data[i];
				//cout << "\n from the pattern structure printing blocks from array" << blocks[i] << endl;
			}
		};

		bool operator==(const Path& p) const

		{

			if (equal(begin(blocks), end(blocks), begin(p.blocks)))
			{
				return true;
			}
			else
			{
				return false;
			}


			/*if( blocks[0] == p.blocks[0] &&
			blocks[1] == p.blocks[1] &&
			blocks[2] == p.blocks[2] &&
			blocks[3] == p.blocks[3] &&
			blocks[4] == p.blocks[4] &&
			blocks[5] == p.blocks[5] &&
			blocks[6] == p.blocks[6] &&
			blocks[7] == p.blocks[7] &&
			blocks[8] == p.blocks[8] )

			return true;

			else

			return false;*/

		}

		Path& operator=(const Path& p)

		{

			for (int i = 0; i < 9; i++)

			{

				blocks[i] = p.blocks[i];

			}



			return *this;

		}



		//relode assignment operator 2

		Path& operator=(const int* data)

		{

			for (int i = 0; i < 9; i++)

				blocks[i] = data[i];



			return *this;

		}





		friend bool operator <(const Path& lhs, const Path& rhs)
		{

			for (int i = 0; i < 9; i++)

			{

				if (lhs.blocks[i] < rhs.blocks[i])

					return true;

				else if (lhs.blocks[i] > rhs.blocks[i])

					return false;

			}



			return false;

		}


		// to set the current values to the data array.
		void setData(int *data)

		{

			for (int i = 0; i < 9; i++)

				data[i] = blocks[i];

		}
	};

	struct cmp
	{
		int p_m;
		int q_m;
		bool operator() (Path p, int p_g_x, Path q, int q_g_x)
		{
			SolutionSearch s;
			p_m = s.heuristic(p, p_g_x);
			q_m = s.heuristic(q, q_g_x);
			return p_m < q_m;
		}
	};

	Path p_goal; //the goal pattern for the search

	Path p_original;

	Path p_cur;

	//priority_queue<struct Path, vector<struct Path>, cmp > queue_astar;


	//************************************************************************************
public:
	SolutionSearch(void);
	~SolutionSearch(void);

	bool AStarSearch(int *data, vector<int> &solution);

	//CODING START!!!
	vector<int> moving_directions[9]; //To mention which
									  //blocks it can slide based on current position of empty block.

	vector<int> global_path;

	map<Path, Path>::iterator map_it;

	int findTargetPos(Path p);

	void printPattern(Path p);

	int heuristic(Path p, int g_x);

	int goal_found;

	//pattern p_origin;
	vector<Path> pattern_list;

	map<Path, Path> parents_map;

	int goal[3][3] = { { 0,1,2 },{ 3,4,5 },{ 6,7,8 } };
	queue<Path> queue_astar;
};
