#include "VisualDisplay.h"
#define STANDARD_SIZE 600
#define CONTAINER_SIZE 640
#define GAP 10 //gap between each piece
#define PIECE_SIZE 200
#define MOVING_SPEED 20



void onMouse( int event, int x, int y, int flags, void* param )
{
	VisualDisplay* vd = (VisualDisplay*) param;
	int cur_selection = -1;


	switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
		
		//find selected position
		for(int i = 0; i < 9; i++)
		{
			if(vd->piece_positions[i].top_left.x <= x && x <= vd->piece_positions[i].bottom_right.x 
				&& vd->piece_positions[i].top_left.y <= y && y <= vd->piece_positions[i].bottom_right.y)
			{
				if(i == vd->target_pos)
				{
					//Make sure there has already been a candiate selected before swaping the pieces; otherwise no actions are taken
					if(!(vd->selected_piece == -1))
					{
						printf("Make a swap with %d!\n", vd->selected_piece);
						vd->swap(vd->selected_piece);
						vd->selected_piece = -1;
					}
				}
				else //if current selection is not the target pos, check whether current position is in the neighborhood of the target position
				{
					for(int nb_idx = 0; nb_idx < (int) vd->moving_directions[vd->target_pos].size(); nb_idx++)
					{				
						if(i == vd->moving_directions[vd->target_pos][nb_idx])
						{
							cur_selection = i;
							break;
						}
					}
				}

			}
		}

		vd->selected_piece = cur_selection;
        break;

	case CV_EVENT_RBUTTONDOWN:
		vd->selected_piece = -1;
		break;
    case CV_EVENT_LBUTTONUP:
			      
        break;

	
	case CV_EVENT_RBUTTONUP:
			
		break;

    }

}

VisualDisplay::VisualDisplay(void)
{
	cv::namedWindow("window", 1);
	cv::setMouseCallback("window", onMouse, this);


	//specify the posi	}on of each piece on the container
	for(int i = 0; i < 9; i++)
	{
		int row_idx = i / 3;
		int col_idx = i % 3;

		piece_positions[i].top_left.x = col_idx * GAP + 10 + PIECE_SIZE * col_idx;
		piece_positions[i].top_left.y = row_idx * GAP + 10 + PIECE_SIZE * row_idx;

		piece_positions[i].bottom_right.x = piece_positions[i].top_left.x + PIECE_SIZE;
		piece_positions[i].bottom_right.y = piece_positions[i].top_left.y + PIECE_SIZE;	
	}

	/*****************	The order is left, bottom, right, top ***********************************/
	moving_directions[0].push_back(3); //for each position, tell the empty space which one in the array that it can be swapped with. 
									   // So only two pieces are considered each time, when the empty space is moving.
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


	//Initialize Mouse data
	selected_piece = -1;
}


VisualDisplay::~VisualDisplay(void)
{
}


/* Draw the container by putting all the 8 sub-images based on their orders*/
void VisualDisplay::paintContainer()
{

	container = cv::Mat(CONTAINER_SIZE, CONTAINER_SIZE, CV_8UC3, cv::Scalar(255, 255, 255));//make the container white color
	
	//put all the pieces into container
	for(int i = 0; i < 9; i++)
	{
		int cur_idx = current_states[i];
		if(cur_idx != 8) //piece-8 does not paint
		{
			if(selected_piece == i)
			{
				cv::Mat high_light = cv::Mat(PIECE_SIZE, PIECE_SIZE, CV_8UC3, cv::Scalar(100, 100, 100));
				cv::Mat temp = high_light + piece[cur_idx];
				temp.copyTo(container(cv::Range(piece_positions[i].top_left.y, piece_positions[i].bottom_right.y), cv::Range(piece_positions[i].top_left.x, piece_positions[i].bottom_right.x)));
			}				
			else
				piece[cur_idx].copyTo(container(cv::Range(piece_positions[i].top_left.y, piece_positions[i].bottom_right.y), cv::Range(piece_positions[i].top_left.x, piece_positions[i].bottom_right.x)));
		}
	}
}


/*Make a swap between the selected candiate piece and the target piece*/
void VisualDisplay::swap(int candidate)
{
	//do the motion
	generateVisualMotion(candidate, target_pos);

	//change the current state
	int temp = current_states[candidate];
	current_states[candidate] = current_states[target_pos];
	current_states[target_pos] = temp;

	//change the target_pos
	target_pos = candidate;
}

/*Generate the motion from the selected piece to the target position*/
void VisualDisplay::generateVisualMotion(int candidate, int target)
{
	cv::Point top_left; //store the transition position of the moving piece
	cv::Point bottom_right;
	top_left.y = piece_positions[candidate].top_left.y;
	top_left.x = piece_positions[candidate].top_left.x;
	bottom_right.y = piece_positions[candidate].bottom_right.y;
	bottom_right.x = piece_positions[candidate].bottom_right.x;
	cv::Mat white_piece = cv::Mat(PIECE_SIZE, PIECE_SIZE, CV_8UC3, cv::Scalar(255, 255, 255)); //used to clear last position of the moving piece
	int cur_idx = current_states[candidate];
	int num_steps = PIECE_SIZE / MOVING_SPEED;


	//check the direction first
	if(candidate < target)
	{
		//moving right
		if((target - candidate) == 1)
		{
			printf("moving right\n");

			//Gradually copy the candidate piece to the transiation position
			for(int step_idx = 1; step_idx <= num_steps; step_idx++)
			{
				top_left.x = piece_positions[candidate].top_left.x + MOVING_SPEED * step_idx;
				bottom_right.x = piece_positions[candidate].bottom_right.x + MOVING_SPEED * step_idx;

				//clear last position
				white_piece.copyTo(container(cv::Range(piece_positions[candidate].top_left.y, piece_positions[candidate].bottom_right.y), cv::Range(piece_positions[candidate].top_left.x, piece_positions[candidate].bottom_right.x)));
				piece[cur_idx].copyTo(container(cv::Range(top_left.y, bottom_right.y), cv::Range(top_left.x, bottom_right.x)));
				int delay = 300/num_steps;
				cv::waitKey(delay);
				imshow("window", container);
			}				
		}
		else //moving down
		{
			printf("moving down\n");
			
			//Gradually copy the candidate piece to the transiation position
			for(int step_idx = 1; step_idx <= num_steps; step_idx++)
			{
				top_left.y = piece_positions[candidate].top_left.y + MOVING_SPEED * step_idx;
				bottom_right.y = piece_positions[candidate].bottom_right.y + MOVING_SPEED * step_idx;

				//clear last position
				white_piece.copyTo(container(cv::Range(piece_positions[candidate].top_left.y, piece_positions[candidate].bottom_right.y), cv::Range(piece_positions[candidate].top_left.x, piece_positions[candidate].bottom_right.x)));
				piece[cur_idx].copyTo(container(cv::Range(top_left.y, bottom_right.y), cv::Range(top_left.x, bottom_right.x)));
				int delay = 300/num_steps;
				cv::waitKey(delay);
				imshow("window", container);
			}	

		}
	}
	else
	{
		if((candidate - target) == 1) //moving left
		{
			printf("moving left\n");

			//Gradually copy the candidate piece to the transiation position
			for(int step_idx = 1; step_idx <= num_steps; step_idx++)
			{
				top_left.x = piece_positions[candidate].top_left.x - MOVING_SPEED * step_idx;
				bottom_right.x = piece_positions[candidate].bottom_right.x - MOVING_SPEED * step_idx;

				//clear last position
				white_piece.copyTo(container(cv::Range(piece_positions[candidate].top_left.y, piece_positions[candidate].bottom_right.y), cv::Range(piece_positions[candidate].top_left.x, piece_positions[candidate].bottom_right.x)));
				piece[cur_idx].copyTo(container(cv::Range(top_left.y, bottom_right.y), cv::Range(top_left.x, bottom_right.x)));
				int delay = 300/num_steps;
				cv::waitKey(delay);
				imshow("window", container);
			}	

		}
		else //moving top
		{
			printf("moving top\n");

			//Gradually copy the candidate piece to the transiation position
			for(int step_idx = 1; step_idx <= num_steps; step_idx++)
			{
				top_left.y = piece_positions[candidate].top_left.y - MOVING_SPEED * step_idx;
				bottom_right.y = piece_positions[candidate].bottom_right.y - MOVING_SPEED * step_idx;

				//clear last position
				white_piece.copyTo(container(cv::Range(piece_positions[candidate].top_left.y, piece_positions[candidate].bottom_right.y), cv::Range(piece_positions[candidate].top_left.x, piece_positions[candidate].bottom_right.x)));
				piece[cur_idx].copyTo(container(cv::Range(top_left.y, bottom_right.y), cv::Range(top_left.x, bottom_right.x)));
				int delay = 300/num_steps;
				cv::waitKey(delay);
				imshow("window", container);
			}	
		}
	}
}

/*Load necessary information into the class: image name, original sequence, and the position of empty space*/
void VisualDisplay::load(char* file_name, int* data, int pos)
{
	cv::Mat temp = cv::imread(file_name);
	int height = temp.rows;
	int width = temp.cols;
	cv::Mat sub_img;

	if(width > height)
	{
		int start_col = (width - height)/2;
		int end_col = start_col + height;
		sub_img = temp(cv::Range(0, height), cv::Range(start_col, end_col));
	}
	else
	{
		int start_row = (height - width)/2;
		int end_row = start_row + width;
		sub_img = temp(cv::Range(start_row, end_row), cv::Range(0, width));
	}

	cv::resize(sub_img, image, cv::Size(STANDARD_SIZE, STANDARD_SIZE));

	//Assign different parts of the image into different pieces
	for(int i = 0; i < 9; i++)
	{
		int col_idx = i % 3;
		int row_idx = i / 3;

		image(cv::Range(row_idx * PIECE_SIZE, (row_idx + 1) * PIECE_SIZE), cv::Range(col_idx * PIECE_SIZE, (col_idx + 1) * PIECE_SIZE)).copyTo(piece[i]);
	}


	original_states = data; //Load the data sequence
	current_states = data;
	target_pos = pos; //store the current target position
}


/*Run the game: showing the container of multiple sub-images and enabling user to re-organize the sub-images */
void VisualDisplay::run()
{
	while(1)
	{

		paintContainer(); //draw the whole pattern by putting all the sub-images into the container
		
	
		imshow("window", container);
		char c = cv::waitKey(1);
		if(c == 27)
			break;
		else if(c == 's' || c == 'S') //At any time, when the user press the "s" or "S" key, it automatically finds the solution
		{
			showSolution(); //Use your implemented search algorithm to find the solution and show it
		}
	}
}


/*Student Implementation: actually you do not need to implement anything here. Just for testing purpose, 
                          you may test different search algorithms here. But be aware, only one algorithm
						  is called everytime.
*/
void VisualDisplay::showSolution()
{
	path.clear();


	/*Only one of the following search function is called each time, so comment the others*/
	solution_finder.AStarSearch(current_states, path); //use A-star to find the solution


	showPath(); //show the path on the GUI
}


/* Show the solution on the GUI by moving those sub-images step by step 
   until all the sub-image get to the right order */
void VisualDisplay::showPath()
{
	//Printout the solution on the console
	printf("The solution path is: \n");
	for(int i = 0; i < (int)path.size(); i++)
	{
		printf("%d, ", path[i]);
	}
	printf("\n");


	//Show the path visually
	selected_piece = -1;
	for(int i = 0; i < (int)path.size(); i++)
	{
		swap(path[i]);
		paintContainer();		
		imshow("window", container);
		cv::waitKey(800);
	}

}