
/**********************************************************************************************************************/
/* This project is for students to practice search algorithms by appling them to solve the 9 puzzle problems.	 	  */
/*																												 	  */
/* Usage: The whole image is divided into 3-by-3 squares. There are 8 sub-images and 1 empty space.				 	  */
/*        These sub-images are placed in a random order, which makes the whole pattern look messy.               	  */
/*        So the goal is to re-organize these sub-images to align them in the right order.						 	  */
/*		  User can only move one sub-image at each time by left clicking it follwed by clicking the empty space. 	  */
/*		  Each time, only neighboring sub-images around the empty space can be moved. At any time, even in the   	  */
/*        begining, if you do not want to continue playing the game, you can simply hit the keyboard - "s" key to	  */
/*        obtain the solution. The system then automatically search for a possible path by using your implemented	  */
/*        search algorithmns. So after you click the "s" key, what you should do is just to wait for the search  	  */
/*        result and see the sub-images move automatically followed by the output path until all the sub-images  	  */
/*        become perfectly aligned.																				 	  */
/*																												 	  */
/* Author: Ju Shen, Department of Computer Science, University of Dayton							             	  */
/*																												 	  */
/* Instructions:																								 	  */
/*				For this project, you only focus on implementing the search algorithms. They are the functions   	  */
/*              in the "SolutionSearch" class, which are marked as "Student Implementation:" in the comments.    	  */
/*              You can search the key words "Student Implementation" to find the places require you to write    	  */
/*              your code.																						 	  */
/*																												 	  */
/*              The GUI has already been implemted, so there is no need to do anything on it for this project.   	  */
/*              However, students are encouraged to get farmiliar with them, as you may need to implement them in	  */
/*				the future projects.																			 	  */
/*																												 	  */
/**********************************************************************************************************************/


#include <vector>       // std::vector
#include <iostream> 
#include <queue>
#include <map>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include "stdio.h"
#include "math.h"

using namespace cv;
using namespace std;

#include "RandomGenerator.h"
#include "VisualDisplay.h"


void printData(int* data)
{
	printf("%d, %d, %d\n %d, %d, %d\n %d, %d, %d\n", data[0], data[1], data[2],
		data[3], data[4], data[5], data[6], data[7], data[8]);
}




int main(int arg, char** argv)
{
	int num_seq[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 }; //These numbers are used to represent those sub-images (or pieces), 
													// by default, the number 8 represents the empty space. 
													// So when these numbers are in different order, it means the correseponding sum-images      
													// are in different places. Initially, we make all the sub-images in the correct order.
													// But they will be randomly reshuffled in the following code.
													/*  For example: {0, 1, 2, 3, 4, 5, 6, 7, 8} in the GUI, its layout is:
													0   1   2
													3   4   5
													6   7   8
													which is the one we want.
													*/

	RandomGenerator rg; //Used to generate different random orders
	rg.getRandomSeq(num_seq, 52); //The generated random order is stored in the "num_seq" array. The second parameter is an
								  // integer which indicates how much degree you want to mess up the sub-images. The bigger
								  // it is, the more messy it could be.
								  /* For example, the random sequence after reshuffle can be: num_seq = {3, 8, 5, 2, 0, 4, 6, 7, 1}
								  3   8   5
								  2   0   4
								  6   7   1
								  */

	VisualDisplay display; //The class for the GUI, which you do not need to do any change


	int target_pos; //This stores the posit

					/*Find the position of the empty space*/
	for (int i = 0; i < 9; i++)
	{
		if (num_seq[i] == 8)
			target_pos = i;
	}


	/*For testing purpose, you can see the output of the generated random order in the console*/
	printf("The Current Pattern is :\n");
	printData(num_seq);



	/*******************************************   The two lines below are to show and run the game **************************************************/
	/*			Before you implement the code, if you run the program, and hit the "s" key, there is nother happened. Because you the "s"			 */
	/*			is to trigger the search algorithm to find the possible solution. Since you have not done anything on it yet, it returns			 */
	/*			an empty path, which you can see from the console.																					 */
	/*																																				 */
	/*			When you are testing your code or doing debug, you can simply comment out these two lines of codes. So you can focus on the			 */
	/*			data and may output them to the console or use the debug tool to analyze them. After you are confident of your search code,			 */
	/*			you can not uncomment these lines and enjoy the game.																				 */
	/*																																				 */
	display.load(const_cast<char*>("test.jpg"), num_seq, target_pos); //The first parameter is the image file name. For fun, you can place any image you like, 
												   //no need to be square image, as long as it is close to square. 
												   //The second parameter is the generated random sequence.
												   //The third parameter is the detected empty space position in the array
	display.run();

	return 1;
}