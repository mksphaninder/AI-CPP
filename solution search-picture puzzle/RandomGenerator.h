#pragma once
#include <vector> 

class RandomGenerator
{
private: 
	std::vector<int> moving_directions[9];
public:
	RandomGenerator(void);
	~RandomGenerator(void);

	void getRandomSeq2(int *num); /*Purely generate random number, not gurantee it has a solution*/
	void getRandomSeq(int *num, int steps); /*Generate the random sequence based on moving from goal status*/
};

