//ME 493 Project Beta
//Etzal Corona
//Buddy Coded with Ethan Skemp and Roberto Jimenez


#include<iostream>
#include<cmath>
#include<time.h>
#include<vector>
#include<assert.h>
#include<fstream>


#define x  10
#define y  10
#define num_simulations 10
#define ECRand (long double)rand()/RAND_MAX
#define gamma .9
#define alpha .1
#define greedy .1

using namespace std;



//Defining my void functions
void display_grid(char grid[x][y]);
void clear_grid(char grid[x][y]);
void update_grid(int agentx, int agenty, int goalx, int goaly, char grid[x][y]);


void move_agent(int & agentx, int & agenty, char direction);
void play_gridworld(char grid[x][y]);


void simulate_gridworld(char grid[x][y]);
void reset_qtable(float qtable[][4]);
int sense(int agentx, int agenty);
int find_best_option(int state_value, float qtable[][4]);
int decide_action(int state_value, float qtable[][4]);
void acting_agent(int & agentx, int & agenty, int & direction);
void reacting_agent(int state_value, int & direction, float qtable[][4], int goalx, int goaly, int agentx, int agenty);


int main() {
	//creating a board
	char Board[x][y];
	srand(time(NULL));

	//Initial Menu
	int choice;
	cout << " Welcome to the Grid World" << endl << " Choose from the following options" << endl << " Click 1 to play game" << endl << " Click 2 for simulation" << endl << " Click 0 to abort Application" << endl << endl;
	cin >> choice; //Taking in the choice the user wants


				   //Create an error message for invalid choice and get a second choice
	while ((choice != 1) && (choice != 2) && (choice != 0))
	{
		cout << "Invalid selection.  Please choose 1, 2 or 0: ";
		cin >> choice;
	}

	//if user wants to play, this function will run
	if (choice == 1) {
		play_gridworld(Board);
	}

	//if user wants a simulation it will call out that function
	if (choice == 2) {
		for (int i = 0; i < num_simulations; i++) {
			simulate_gridworld(Board);
			cout << i << endl;
	}
	}
	cin >> choice;
}





//This functions displays the grid to the user and for the simulation
void display_grid(char grid[x][y]) {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			cout << grid[i][j];
		}
		cout << endl;
	}
}





void clear_grid(char grid[x][y]) {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			grid[i][j] = '_';
		}
	}
}






void update_grid(int agentx, int agenty, int goalx, int goaly, char grid[x][y]) {
	clear_grid(grid);
	grid[agentx][agenty] = 'A';
	grid[goalx][goaly] = 'G';
}




//This function moves the agent with the user input
void move_agent(int & agentx, int & agenty, char direction) {
	//This moves the agent up
	if (direction == 'w') {
		agentx -= 1;
	}
	//This moves the agent left
	if (direction == 'a') {
		agenty -= 1;
	}
	//This moves the agent down
	if (direction == 's') {
		agentx += 1;
	}
	//This moves the agent right
	if (direction == 'd') {
		agenty += 1;
	}
	//This creates the boundaries
	if (agentx < 0) {
		agentx = 0;
	}
	if (agentx >= x) {
		agentx = x - 1;
	}
	if (agenty < 0) {
		agenty = 0;
	}
	if (agenty >= y) {
		agenty = y - 1;
	}

}





//This function lets the user play the game
void play_gridworld(char grid[x][y]) {
	//Declarations
	int agentx;
	int agenty;
	int goalx;
	int goaly;
	char direction;

	//Assigning starting position and goal at random square
	agentx = rand() % (x);
	agenty = rand() % (y);
	goalx = rand() % (x);
	goaly = rand() % (y);


	while (1) {
		//Calling and cleaning the board
		clear_grid(grid);
		update_grid(agentx, agenty, goalx, goaly, grid);
		//Displaying the board and directions
		display_grid(grid);
		cout << " Game Ready use w,a,s,d, keys to move piece" << endl << endl;
		cout << " w=up, s= down, a=left, d=right" << endl << endl;
		cin >> direction;
		if (direction == '0') {
			break;
		}
		//This calls out our move agent function
		move_agent(agentx, agenty, direction);
		//If the user wins it displays this message and finishes the program
		if ((agentx == goalx) && (agenty == goaly)) {
			cout << " You beat the grid world! " << endl;
			break;
		}
	}
}





//This function simulates the agent to play the gridworld game
void simulate_gridworld(char grid[x][y]) {
	//Declarations
	int agentx;
	int agenty;
	int goalx;
	int goaly;

	//Assigning starting position and goal at random square
	agentx = rand() % (x);
	agenty = rand() % (y);
	goalx = rand() % (x);
	goaly = rand() % (y);
	float qtable[x*y][4];
	reset_qtable(qtable);
	int state_value = 0;
	int direction;

	int dumy=0; 
	float dummy;
	for (int i =0; i < num_simulations; i++)
	while (1) {
		dumy++;
		//cout << "Location " << agentx << " " << agenty << endl;
		state_value = sense(agentx, agenty);
		direction = decide_action(state_value, qtable);
		reacting_agent(state_value, direction, qtable, goalx, goaly, agentx, agenty);
		acting_agent(agentx, agenty, direction);
		//cin >> dumy;
		//Shows if the agent finds the goal on its own
		if ((agentx == goalx) && (agenty == goaly)) {
			cout << " You beat the grid world! " << dumy  << endl;
			cin >> dumy;
			break;
		}
		
	}
	//for (int jj = 0; jj < x * y; jj++) {
	//	cout << find_best_option(jj, qtable);
		//if (!(jj % (x)))
			//cout << endl;
	//}
}





void reset_qtable(float qtable[][4]) {
	for (int i = 0; i < x*y; i++) {
		for (int j = 0; j < 4; j++) {
			qtable[i][j] = ECRand;
		}
	}
}





int sense(int agentx, int agenty) {
	return agentx*y + agenty;
}





int decide_action(int state_value, float qtable[][4]) {
	float random_number = ECRand;
	if (random_number < greedy) {
		return rand() % 4;
	}
	return find_best_option(state_value, qtable);
}





int find_best_option(int state_value, float qtable[][4]) {
	int best_choice = 0;
	for (int i = 0; i < 4; i++) {
		if (qtable[state_value][i] > qtable[state_value][best_choice]) {
			best_choice = i;
		}
	}
	return best_choice;
}




void acting_agent(int & agentx, int & agenty, int & direction) {
	//cout << direction << endl; 
	if (direction == 0) {
		agenty++;
	}
	if (direction == 1) {
		agenty--;
	}
	if (direction == 2) {
		agentx--;
	}
	if (direction == 3) {
		agentx++;
	}
	if (agentx < 0) {
		agentx = 0;
	}
	if (agentx >= x) {
		agentx = x - 1;
	}
	if (agenty < 0) {
		agenty = 0;
	}
	if (agenty >= y) {
		agenty = y - 1;
	}

}





void reacting_agent(int state_value, int & direction, float qtable[][4], int goalx, int goaly, int agentx, int agenty) {
	float og_action_value = qtable[state_value][direction];
	float reward;
	if ((agentx == goalx) && (agenty == goaly)) {
		reward = 100;
	}
	else {
		reward = -1;
	}
	float max_action_value;
	int statet_1;

	if (direction == 0) {
		statet_1 = sense(agentx, agenty + 1);
	}
	if (direction == 1) {
		statet_1 = sense(agentx, agenty - 1);
	}
	if (direction == 2) {
		statet_1 = sense(agentx - 1, agenty);
	}
	if (direction == 3) {
		statet_1 = sense(agentx + 1, agenty );
	}

	max_action_value = qtable[statet_1][find_best_option(statet_1, qtable)];

	qtable[state_value][direction] = og_action_value + alpha*(reward + (gamma*max_action_value) - og_action_value);

	//Test D
	//assert(state_value[goalx][go =! reward);
}


