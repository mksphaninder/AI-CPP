#include "RandomGenerator.h"

#include <ctime>        // std::time
#include <algorithm>
#include <iostream> 

RandomGenerator::RandomGenerator(void)
{
	/*****************  Pre-store the available moving direction or child of current tree ********/	
	/*  0  1  2  */
	/*  3  4  5  */
	/*  6  7  8  */
	/*****************	The order is left, bottom, right, top ***********************************/
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

	moving_directions[7].push_back(6);
	moving_directions[7].push_back(8);
	moving_directions[7].push_back(4);

	moving_directions[8].push_back(7);
	moving_directions[8].push_back(5);
}


RandomGenerator::~RandomGenerator(void)
{
}


/*Generate the random sequence based on moving from goal status*/
void RandomGenerator::getRandomSeq(int* num, int steps)
{
	int cur_pos = 8; //The position of the 8-piece
	int swap_pos; //The position that is used to be swapped with the 8-piece
	int temp; //Used for data swap
	int choices; //The number of choiece for different positions of 8-piece
	int x; //The choice randomly made from different choieces
	srand(time(NULL));
	for(int i = 0; i < steps; i++)
	{
		int choices = moving_directions[cur_pos].size();
		x = rand() % choices;
		swap_pos = moving_directions[cur_pos][x];
		temp = num[cur_pos];
		num[cur_pos] = num[swap_pos];
		num[swap_pos] = temp;	
		cur_pos = swap_pos;
	}
}

/*Purely generate random number, not gurantee it has a solution*/
void RandomGenerator::getRandomSeq2(int* num)
{
	std::srand ( unsigned ( std::time(0) ) );
	std::vector<int> myvector;


		
	for (int i = 0; i < 9; i++) 
		myvector.push_back(i); // 0 1 2 3 4 5 6 7 8 

	 // using built-in random generator:
	std::random_shuffle ( myvector.begin(), myvector.end() );
	
	int idx = 0;
	for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    {
		num[idx] = *it;
		idx++;
	}

};