/*
	Program name: Final Project - finalProject.cpp
	Author: John Lee
	Date last updated: 5/11/2018
	Purpose: Make the game Phase 10 Dice

	6/7/2018	Added to Github
*/

//	Include files
#include <windows.h> 
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <limits>
#include <algorithm>
#include <cstdio>
#include <tchar.h>
#include <strsafe.h>
#include <conio.h>

using namespace std;

//	used because of pre defined max in a library
#undef max
//	array of text color set by windows console
int colorArray[4] = { 6, 4, 1, 2 };
//	enum variable for colors of the dice
enum diceColors { orange, red, blue, green };
//	string representation of the colors
string colorString[4] = { "Orange", "Red", "Blue", "Green" };
//	phase description in array format so it can called called easily in a loop
string phaseDesc[10] = {	"2 Sets of 3\nEx.\t[1 1 1 3 3 3] = 12 pts\n\t[7 7 W 9 9 9] = 41 pts\n",
							"1 Set of 3 and 1 Run of 4\nEx.\t[2 2 2 4 5 6 7] = 28 pts\n\t[9 9 W 7 8 W 10] = 43 pts\n",
							"1 Set of 4 and 1 Run of 4\nEx.\t[5 5 5 W 6 W 8 W] = 29 pts\n\t[10 10 W W 7 8 9 10] = 54 pts\n",
							"1 Run of 7\nEx.\t[1 2 W W 5 6 7] = 21 pts\n\t[4 5 6 7 W 9 10] = 41 pts\n",
							"1 Run of 8\nEx.\t[W 2 W 4 5 6 7 8] = 32 pts\n\t[3 4 5 6 W 8 9 10] = 45 pts\n",
							"1 Run of 9\nEx.\t[1 W W 4 5 6 7 8 9] = 40 pts\n\t[2 W 4 5 6 7 W 9 10] = 43 pts\n",
							"2 Set of 4\nEx.\t[5 5 W W 6 6 6 W] = 28 pts\n\t[8 8 W W 10 10 10 W] = 46 pts\n",
							"7 of all one color\nEx.\t[5 3 2 W 7 5 W] of all orange = 22 pts\n\t[5 3 8 7 9 10 W] of all blue = 42 pts\n",
							"1 Set of 5 and 1 Set of 2\nEx.\t[2 2 7 7 7 W W] = 25 pts\n\t[9 9 10 10 10 W W] = 48 pts\n",
							"1 Set of 5 and 1 set of 3\nEx.\t[5 5 W 6 6 W W W] = 22 pts\n\t[9 9 W 10 10 10 10 W] = 58 pts\n" };
//	dice structure
struct dice {
	int face[6];
	diceColors color[6];
	int value;
};

//	player structure
struct player {
	string name;
	int phase = 1;
	int score = 0;
	int scorePhase[10] = { 0 };
	int pass = 0;
	int phase5Bonus = 0;
	int failed = 0;
	bool firstPlace = false;
};

//	dice array for the game, represents the 10 dice
dice gameDice[10];

//	function declarations
void ClearScreen();
void titleScreen();
int menuScreen();
int scoreDice(dice[]);
int determineFirst(int);
int doTurn(player, dice[]);
void initializeDice();
void rollDice(int);
void showDice();

int phase1();
int phase2();
int phase3();
int phase4();
int phase5();
int phase6();
int phase7();
int phase8();
int phase9();
int phase10();

bool diceExist(int);
bool diceExist(int, dice[], int[]);
int numOfWild();
int numOfWild(dice[], int[]);
int highDice();
void showAllScore(int, player[], int);
int setTitle();
int largestDice(int[], diceColors);
int intUserInput(int, int);
int getPhaseResult(int);
void rollPattern(int);

int main(void)
{
	int numOfPlayers;
	int startPlayer;
	int sum = 0;
	string input;
	int rollBit = 1023;
	player players[4];
	int result;
	srand((unsigned int)time(NULL));
	int current;
	int chances = 1;
	char playAgain;
	int round = 1;
	bool solitaire = false;
	bool endTurn = false;
	bool lastRound = false;
	int lastPlayer = -1;
	string dummy;
	int topScore;
	int winnerIndex;

	//	set the Dice numbers and colors based on the physical game
	initializeDice();
	//	set title of the window
	setTitle();
	//	main header screen with description of project
	titleScreen();

	//	start of game loop
	do {

		ClearScreen();

		//	get number of players participating
		numOfPlayers = menuScreen();
		//	if only 1 player, set the flag for solitaire mode. Later used for scoring.
		if (numOfPlayers == 1) {
			cout << "Solitaire mode activated" << endl;
			solitaire = true;
		}
		else {
			solitaire = false;
		}

		//	initilize each player
		for (int i = 0; i < numOfPlayers; i++) {
			int getDummy;
			string inputName;
			cout << "Enter name for player " << i << " (max. 10 chars): ";
			cin >> inputName;
			getDummy = getchar();
			players[i].name = inputName.substr(0, 10);
			players[i].phase = 1;//	change to 1 later
			players[i].score = 0;
			for (int j = 0; j < 10; j++) {
				players[i].scorePhase[j] = 0;
			}
			players[i].pass = 0;
			players[i].firstPlace = false;
			players[i].phase5Bonus = 0;
		}

		//	determine who goes first
		//	solitaire mode skips this
		if (numOfPlayers > 1 && numOfPlayers < 5)
			startPlayer = determineFirst(numOfPlayers);
		else
			startPlayer = 0;

		cout << "Press any key to begin game...";
		int getint = _getch();
		if (getint == 224) getint = _getch();

		ClearScreen();

		current = startPlayer;

		do {
			//	end game if turn reaches the last player(whoever finished phase 10 first) again
			if (lastPlayer == current) {
				break;
			}

			//	show turn header at the start of phase
			if (chances == 1) {
				cout << "Player: " << "[" << current << ": " << players[current].name << " ]" << endl;
				cout << "Phase number: " << players[current].phase << endl;
				cout << phaseDesc[players[current].phase - 1] << endl;
			}
			cout << "Attempt: " << chances << endl;
			cout << "Press any key to roll your dice...";
			int getint = _getch();
			if (getint == 224 || getint == 0) getint = _getch();
			cout << endl;

			//	roll the dice, rollBit is set by number bits. 1023 means rolling all the dice.
			//rollPattern(rollBit);
			//cout << endl;

			rollDice(rollBit);
			//	display all the dice
			showDice();

			//	checks if you still have enough chances to re roll
			if (chances < 3) {
				cout << endl;
				int inputN = -2;
				
				//	get input of what dice to re roll
				//	check for input validation
				do {
					rollBit = 0;
					cout << "Enter dice to reroll: ";
					getline(cin, input);
					istringstream iss(input);
					while (iss >> inputN || !iss.eof()) {
						if (iss.fail()) {
							iss.clear();
							string dummy;
							iss >> dummy;

							inputN = -2;
							break;
							continue;
						}
						if (inputN == -1) {
							endTurn = true;
							break;
						}
						if (inputN >= 0 && inputN <= 9)
							rollBit += (int)pow(2, inputN);
						else
							inputN = -2;
					}
					
					int issState = iss.rdstate();
					if (inputN == -2) cout << endl << "Please enter appropriate input" << endl;
				} while (inputN == -2);

				//	player entered -1 to end turn early
				if (endTurn) {
					chances = 3;
				}
				cout << endl;
			}
			chances++;

			//	no more rolls left on phase for player, turn goes to next player or player opted to end turn early
			if (chances > 3 || endTurn) {
				//	check if the dice passes the current phase
				result = getPhaseResult(players[current].phase);
				if (result != -1) {
					//	if passes add score to player data
					cout << endl;
					cout << "Passed with Dice Score: " << result << endl;
					players[current].score += result;
					//	check if player passed on phase 5 to add bonus points
					//	this won't be added to round score
					if (players[current].phase == 5)
					if (players[current].score > 220) {
						cout << endl << "Bonus for scoring over 220 points: +40 points" << endl << endl;
						players[current].phase5Bonus = 40;
					}
					players[current].scorePhase[players[current].phase - 1] = result;
					players[current].pass = 1;

					//	reset turn attributes per turn for next turn
					chances = 1;
					cout << "Next player" << endl;
					cout << "Press any key to continue...";
					int getint = _getch();
					if (getint == 224) getint = _getch();
					rollBit = 1023;

					//	if this is the last phase, mark this player to be first place bonus starter.
					if (lastPlayer == -1 && players[current].phase == 10) {
						lastPlayer = current;
						players[current].firstPlace = true;
					}
					
					if (lastPlayer != -1) {
						if (players[current].phase == 10) {
							if (lastPlayer > 0) {
								//	calculate who gets the bonus
								if (numOfPlayers - ((lastPlayer - startPlayer) % numOfPlayers) < (current - lastPlayer) % numOfPlayers) {
									cout << endl << "You got first place bonus of +40 points!" << endl << endl;
									players[current].firstPlace = true;
								}
							}
						}
					}
					
					//	set attributes to player for next phase after completing the phase
					players[current].phase++;
					//	go to next player
					current++;
					//	check to pass turn to player in rotation
					if (current >= numOfPlayers) {
						current = 0;
					}
					//	show round score if player was the last player to go
					if (current == startPlayer) {
						showAllScore(round, players, numOfPlayers);
						round++;
					}
					//	reset variable to end turn early
					endTurn = false;
					ClearScreen();
				}
				else {
					//	case of failing a phase
					cout << "Phase failed" << endl;
					cout << "Next player" << endl;
					cout << "Press any key to continue...";
					int getint = _getch();
					if (getint == 224) getint = _getch();

					//	reset settings for next player
					chances = 1;
					rollBit = 1023;
					//	increase the fail counter for solitaire mode
					if (solitaire) players[current].failed++;
					current++;
					if (current >= numOfPlayers) {
						current = 0;
					}
					if (current == startPlayer) {
						showAllScore(round, players, numOfPlayers);
						round++;
					}
					endTurn = false;
					ClearScreen();
				}
			}
		} while (1);

		//	Game ended
		lastPlayer = -1;
		topScore = 0;
		winnerIndex = 0;

		//	get player id of who won with highest score
		for (int i = 0; i < numOfPlayers; i++) {
			int calcScore = players[i].score + (players[i].phase5Bonus) + ((players[i].firstPlace) ? 40 : 0) + (players[i].failed * -5);
			if (calcScore > topScore) {
				topScore = calcScore;
				winnerIndex = i;
			}
		}

		//	congratulate player
		cout << "Congratulations!" << endl;
		cout << "Player [" << winnerIndex << "] " << players[winnerIndex].name << " won with a score of " << 
			players[winnerIndex].score + (players[winnerIndex].phase5Bonus) + ((players[winnerIndex].firstPlace) ? 40 : 0) + (players[winnerIndex].failed * -5) << endl;
		cout << endl;
		
		//	Score header
		cout << "Score Card" << endl;
		cout << setw(20) << left << " ";
		for (int i = 0; i < numOfPlayers; i++) {
			cout << setw(10) << right << players[i].name;
		}
		cout << endl;

		//	display score by phases
		for (int row = 0; row < 10; row++) {
			cout << setw(20) << left << "Phase " + to_string(row + 1);
			for (int i = 0; i < numOfPlayers; i++) {
				cout << setw(10) << right << players[i].scorePhase[row];
			}
			cout << endl;
		}

		//	display phase five bonus
		cout << setw(20) << left << "Phase five bonus: ";
		for (int i = 0; i < numOfPlayers; i++) {
				cout << setw(10) << right << players[i].phase5Bonus;
		}
		cout << endl;
		
		//	display first place bonus
		cout << setw(20) << left << "First place bonus: ";
		for (int i = 0; i < numOfPlayers; i++) {
			if (players[i].firstPlace)
				cout << setw(10) << right << "40";
			else
				cout << setw(10) << right << "0";
		}
		cout << endl;

		//	if it's in solitaire mode, show how much penalty because of phase fails
		if (solitaire) {
			cout << endl;
			cout << setw(20) << left << "Solitaire fails: " << players[0].failed * -5 << " points" << endl;
		}

		//	show the total score of all players
		cout << setw(20) << left << "Total";
		for (int i = 0; i < numOfPlayers; i++) {
			cout << setw(10) << right << players[i].score + (players[i].phase5Bonus) + ((players[i].firstPlace) ?  40 : 0) + (players[i].failed * -5);
		}
		cout << endl;
		cout << endl;

		//	ask user if they want to play again
		cout << "Do you want to play again (Y/N)? ";
		do {
			playAgain = _getch();
		} while (tolower(playAgain) != 'y' && tolower(playAgain) != 'n');
		cout << endl;
	} while (toupper(playAgain) == 'Y');

	cout << endl;
	cout << "Thank you for playing!" << endl;
	cout << "Goodbye." << endl;
	cout << endl;

	return 0;
}

void rollPattern(int rollBit) {
	//int bit = 1;
	do {
		if (rollBit & 1) {
			cout << "1 ";
		}
		else cout << "0 ";
		//bit = bit << 1;
		rollBit = rollBit >> 1;
	} while (rollBit != 0);

}

/*
Name: getPhaseResult
Parameter: int phase - phase that the dice will be testes against
Returns: int - returns the score of the phase otherwise -1 if it fails
Description: get the phase score using the current dice, if it fail it will return -1
*/
int getPhaseResult(int phase) {
	int result = -1;
	switch (phase) {
	case 1: result = phase1(); break;
	case 2: result = phase2(); break;
	case 3: result = phase3(); break;
	case 4: result = phase4(); break;
	case 5: result = phase5(); break;
	case 6: result = phase6(); break;
	case 7: result = phase7(); break;
	case 8: result = phase8(); break;
	case 9: result = phase9(); break;
	case 10: result = phase10(); break;
	}
	return result;
}

/*
Name: setTitle
Parameter: void
Returns: int - return 0 if nothing went wrong other -1
Description: set the title of the console window
*/
int setTitle() {

	TCHAR szOldTitle[MAX_PATH];
	TCHAR szNewTitle[MAX_PATH];

	if (GetConsoleTitle(szOldTitle, MAX_PATH))
	{
		StringCchPrintf(szNewTitle, MAX_PATH, TEXT("PHASE 10 DICE"), szOldTitle);
		if (!SetConsoleTitle(szNewTitle))
		{
			return 1;
		}
	}
	return 0;
}

/*
Name: ClearScreen
Parameter: void
Returns: void
Description: clear the console window
*/
void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

/*
Name: titleScreen
Parameter: void
Returns: void
Description: display program header
*/
void titleScreen() {
	//cout << setw(2);


	cout << endl;
	cout << "  *********************************************" << endl;
	cout << "  *    Programming Assignment Final Project   *" << endl;
	cout << "  *           Computer Programming I          *" << endl;
	cout << "  *              Author: John Lee             *" << endl;
	cout << "  *         Due Date: Thursday, May  11       **" << endl;
	cout << "  *********************************************" << endl;
	cout << endl;
	cout << "Phase 10 Dice" << endl;
	cout << endl;
	cout << "Final project based on the physical game Phase 10 Dice." << endl;
	cout << endl;
	cout << endl;
	cout << "Note: this game is designed to run on a console window." << endl;
	cout << endl;
	cout << "Deisgned on visual studio 2017 community" << endl;
	cout << "For windows-os based platform." << endl;

	cout << "Press any key to continue..." << endl;
	int getint = _getch();
	if (getint == 224) getint = _getch();
	
}

/*
Name: menuScreen
Parameter: void
Returns: int - return number of players participating
Description: display game instructions and get input of how many players will be playing
*/
int menuScreen() {
	int num;
	cout << "Description of the game." << endl;
	cout << endl;
	cout << "";
	cout << "Object of the game:" << endl;
	cout << endl;
	cout << "\tIn order to win the game of Phase 10 Dice you'll need to be the player who can get through the 10 phases with the highest number of points." << endl;
	cout << endl;
	cout << "\tThere are 10 phases to complete. Each phase has a rule of sets, runs, and/or colors. Sets are arrangements of the same number. Runs are arrangements of consecutive numbers. Wilds can be used as a placeholder to complete these. For phase 8, wilds of different color will not complete the phase." << endl;
	cout << endl;
	cout << endl;
	cout << "Phase 10 Dice:" << endl;
	cout << endl;
	cout << "\tThere are 10 dice to be rolled. There are 4 dice for low numbers consisting of 1,2,3,4, and 2 wilds. Wilds can be used as any number from 1-10. There are 6 dice for high numbers consisting of 5, 6, 7, 8, 9, 10. There are also 4 colors, orange, red, blue, and green mixed on the faces of the dice." << endl;
	cout << endl;
	cout << endl;
	cout << "Bonus rules:" << endl;
	cout << endl;
	cout << "\tFive phase bonus: If you complete phase 5 with a score higher than 220, you will get 40 extra points" << endl;
	cout << "\tFirst finish bonus: Players who finish phase 10 first get 40 extra points, this bonus applies to subsequent players that finish phase in the same round after the initial first place finish." << endl;
	cout << "\tSolitaire play: There will be a 5 point penalty for each phase that fails." << endl;
	cout << endl;
	cout << endl;
	cout << "Turn:" << endl;
	cout << "\tEach turn you have three chances to complete the phase, enter which dice number from 0 to 9 to reroll followed by a space and more dice numbers to reroll if needed. You can end the turn early and go straight to scoring for the phase by entering -1" << endl;
	cout << "After three chances if you complete the phase the score will be added to your score and turn goes to the next player, on your next turn you will work on the next phase" << endl;
	cout << "If you couldn't complete the phaes, no scoring will be added. Turn goes to the next player. On your next turn you work on the same phase." << endl;

	cout << endl;
	cout << "Enter number of players to play [1-4]: ";
	//cin >> num;
	num = intUserInput(1, 4);

	return num;
}

/*
Name: intUserInput
Parameter:	int a - lower range of integer input
			int b - upper range of integer input
Returns: int - return the validated integer input from user
Description: get input from user within a range with validation
*/
int intUserInput(int a, int b) {
	int inp;
	//	input validation: input is between a and b inclusive
	do {
		
		cin >> inp;
		if ((cin.fail()) || !(inp >= a && inp <= b)) {
			cout << "Please enter a number between " << a << " and " << b << ": ";
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
		}
		else {
			int c = cin.peek();
			if (c == EOF) break;
			if (c == 10) break;

			cin >> inp;
			if ((cin.fail()) || !(inp >= 0 && inp <= 8)) {
				cout << "Please enter a number between " << a << " and " << b << ": ";
				cin.clear();
				cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
				inp = -1;
			}
			else {
				cout << "Please enter a number between " << a << " and " << b << ": ";
				cin.clear();
				cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
				inp = -1;
			}
		}
	} while (!(inp >= a && inp <= b));
	return inp;
}

/*
Name: determineFirst
Parameter: int num - number of players
Returns: int - player id that will go first
Description: randomly calculate who does first
*/
int determineFirst(int num) {
	int r;
	int p = -1;
	int h = 0;

	cout << "Rolling high dice to determine which player goes first." << endl;
	cout << "Whoever rolled the highest dice first is determined the first player." << endl;
	cout << "Rolling... " << endl;
	for (int i = 0; i < num; i++) {
		cout << "Rolling for player " << i << endl;
		r = rand() % (10 - 5 + 1) + 5;
		cout << "[ " << r << " ] has been rolled." << endl;
		if (r > h) {
			h = r;
			p = i;
		}
	}
	cout << endl;
	cout << "player " << p << " goes first." << endl;
	return p;
}

/*
Name: initializeDice
Parameter: void
Returns: void
Description: set the data to the dice based on the physical game
*/
void initializeDice() {
	gameDice[0] = { { 1,2,3,4,0,0 },{ blue, green, orange, green, orange, red } };
	gameDice[1] = { { 1,2,3,4,0,0 },{ green, orange, blue, red, red, green } };
	gameDice[2] = { { 1,2,3,4,0,0 },{ orange, red, red, blue, blue, green } };
	gameDice[3] = { { 1,2,3,4,0,0 },{ red, blue, green, orange, orange, blue } };
	gameDice[4] = { { 5,6,7,8,9,10 },{ red, green, orange, blue, red, blue } };
	gameDice[5] = { { 5,6,7,8,9,10 },{ red, orange, blue, green, orange, green } };
	gameDice[6] = { { 5,6,7,8,9,10 },{ blue, orange, green, red, green, orange } };
	gameDice[7] = { { 5,6,7,8,9,10 },{ green, red, green, orange, blue, red } };
	gameDice[8] = { { 5,6,7,8,9,10 },{ blue, blue, red, orange, red, green } };
	gameDice[9] = { { 5,6,7,8,9,10 },{ orange, blue, red, blue, green, orange } };
}

/*
Name: rollDice
Parameter: int bin - bitwise number used to figure which dice to roll
Returns: void
Description: roll random dice based on the bitwise number
*/
void rollDice(int bin) {
	int randDiceIndex;
	int pos = 1;

	for (int i = 0; i < 10; i++) {
		if ((bin & pos) == pos) {
			randDiceIndex = rand() % (6);
			gameDice[i].value = randDiceIndex;
		}
		pos <<= 1;
	}
}

/*
Name: showDice
Parameter: void
Returns: void
Description: display all the dice with respective colors
*/
void showDice() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < 10; i++)
	{
			SetConsoleTextAttribute(hConsole, colorArray[gameDice[i].color[gameDice[i].value]]);
			cout << "Dice #" << i << " - [ ";
			if (gameDice[i].face[gameDice[i].value] == 0) cout << "W";
			else cout << gameDice[i].face[gameDice[i].value];
			cout << ", " << colorString[gameDice[i].color[gameDice[i].value]] << " ]" << endl;
			SetConsoleTextAttribute(hConsole, 15);
	}
}

/*
Name: numOfWild
Parameter: void
Returns: int - return number of wilds shown in all dice
Description: counts all the wilds in the current dice array
*/
int numOfWild() {
	int wild = 0;
	for (int i = 0; i < 10; i++) {
		if (gameDice[i].face[gameDice[i].value] == 0) wild++;
	}
	return wild;
}

/*
Name: numOfWild
Parameter:	dice tmpDice[] - temporary set of dice
			int bit[] - array of flagged dice to avoid
Returns: int - number of wilds that are not flagged
Description: count all the wilds in the temporary dice that are not flagged
*/
int numOfWild(dice tmpDice[], int bit[]) {
	int wild = 0;
	for (int i = 0; i < 10; i++) {
		if (bit[i] != 1)
			if (tmpDice[i].face[tmpDice[i].value] == 0) wild++;
	}
	return wild;
}

/*
Name: highDice
Parameter: void
Returns: int - return the highest face value of all the dice
Description: gets the value of the highest face value of current dice
*/
int highDice() {
	int high = 0;
	for (int i = 0; i < 10; i++) {
		if (gameDice[i].face[gameDice[i].value] > high) high = gameDice[i].face[gameDice[i].value];
	}
	return high;
}

/*
Name: highDice
Parameter:	dice tmpDice[] - temporary set of dice
			int bit[] - array of flagged dice
Returns: int - number of highest face value
Description: gets the value of the highest face value with temporary dice that are not flagged
*/
int highDice(dice tmpDice[], int bit[]) {
	int high = 0;
	for (int i = 0; i < 10; i++) {
		if (bit[i] != 1)
			if (tmpDice[i].face[tmpDice[i].value] > high) high = tmpDice[i].face[tmpDice[i].value];
	}
	return high;
}

/*
Name: diceExist
Parameter: int num - number to be checked
Returns: bool - condition if number exists among the dice
Description: check through all the dice if the number exists among the dice
*/
bool diceExist(int num) {
	for (int i = 0; i < 10; i++) {
		if (gameDice[i].face[gameDice[i].value] == num) return true;
	}
	return false;
}

/*
Name: diceExist
Parameter:	int num - number to be checked
			dice tmpDice[] - temporary set of dice
			int bit[] - array of flagged dice
Returns: bool - condition if number exists among the dice
Description: check through all the temporary dice if the number exists that are not flagged
*/
bool diceExist(int num, dice tmpDice[], int bit[]) {
	for (int i = 0; i < 10; i++) {
		if (bit[i] != 1)
			if (tmpDice[i].face[tmpDice[i].value] == num) return true;
	}
	return false;
}

/*
Name: largeestDice
Parameter:	int flag[] - flagged dice
			diceColors target - color to check against
Returns: int - dice index
Description: check through all the game dice with largest value that matches the target color
*/
int largestDice(int flag[], diceColors target) {
	int index = -1;
	int value = -1;
	for (int i = 0; i < 10; i++) {
		if (flag[i] != 1)
			if (gameDice[i].color[gameDice[i].value] == target)
				if (gameDice[i].face[gameDice[i].value] > value) {
					value = gameDice[i].face[gameDice[i].value];
					index = i;
				}
	}
	return index;
}

/*
Name: phase1
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 1
*/
int phase1() {
	int wild = 0;
	int highValue = 0;
	int flag[10] = { 0 };
	dice tmp[10];
	int valTmp[10];
	int streak = 0;
	int sum = 0;
	int matched = 0;
	int set1 = 3;
	int set2 = 3;
	//	store the dice in a temp variable of int array to be used in the sort function
	for (int i = 0; i < 10; i++) {
		tmp[i] = gameDice[i];
		valTmp[i] = gameDice[i].face[gameDice[i].value];
	}

	//	sort the values of the dice
	sort(valTmp, valTmp + 10);

	//	store the sorted array back to a temp dice array
	for (int i = 0; i < 10; i++) {
		switch (valTmp[i]) {
		case 0: tmp[i].value = 5; break;
		case 1: tmp[i].value = 0; break;
		case 2: tmp[i].value = 1; break;
		case 3: tmp[i].value = 2; break;
		case 4: tmp[i].value = 3; break;
		case 5: tmp[i].value = 0; break;
		case 6: tmp[i].value = 1; break;
		case 7: tmp[i].value = 2; break;
		case 8: tmp[i].value = 3; break;
		case 9: tmp[i].value = 4; break;
		case 10: tmp[i].value = 5; break;
		}
	}

	//	new algorithm
	int wildUsed = 0;
	int maxTarget = 0;
	int maxScore = 0;
	int maxCon = 0;
	int consecutive = 1;
	//	go through all the dice
	for (int i = 0; i < 10; i++) {
		consecutive = 1;
		//	check the next dice
		for (int j = i + 1; j < 10; j++) {
			//	check if same dice value appeared again
			if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
				consecutive++;
			}
			else {
				break;
			}
		}
		if (consecutive + numOfWild(tmp, flag) >= set1) {
			if (consecutive * tmp[i].face[tmp[i].value] > maxScore) {
				maxCon = consecutive;
				maxScore = consecutive * tmp[i].face[tmp[i].value];
				maxTarget = tmp[i].face[tmp[i].value];
			}
		}
	}

	int setMax = set1;
	//	flag the dice that match the set of 3 criteria
	for (int i = 0; i < 10; i++) {
		if (tmp[i].face[tmp[i].value] == maxTarget) {
			flag[i] = 1;
			setMax--;
		}
		if (setMax == 0) break;
	}
	wildUsed = set1 - maxCon;
	//	use the wild dice to fulfil the set of 3
	if (wildUsed > 0)
		for (int i = 0; i < 10; i++) {
			if (flag[i] != 1) {
				if (tmp[i].face[tmp[i].value] == 0) {
					flag[i] = 1;
					wildUsed--;
				}
				if (wildUsed == 0) break;
			}
		}

	//	find out if we got any matched with the sets
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched != set1) return -1;


	wildUsed = 0;
	maxTarget = 0;
	maxScore = 0;
	maxCon = 0;
	consecutive = 1;
	for (int i = 0; i < 10; i++) {
		if (flag[i] != 1) {
			consecutive = 1;
			for (int j = i + 1; j < 10; j++) {
				if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
					consecutive++;
				}
				else {
					break;
				}
			}
			if (consecutive + numOfWild(tmp, flag) >= set2) {
				if (consecutive * tmp[i].face[tmp[i].value] > maxScore) {
					maxCon = consecutive;
					maxScore = consecutive * tmp[i].face[tmp[i].value];
					maxTarget = tmp[i].face[tmp[i].value];
				}
			}
		}
	}

	setMax = set2;
	for (int i = 0; i < 10; i++) {
		if (flag[i] != 1)
			if (tmp[i].face[tmp[i].value] == maxTarget) {
				flag[i] = 1;
				setMax--;
			}
		if (setMax == 0) break;
	}
	wildUsed = set2 - maxCon;
	if (wildUsed > 0)
		for (int i = 0; i < 10; i++) {
			if (flag[i] != 1) {
				if (tmp[i].face[tmp[i].value] == 0) {
					flag[i] = 1;
					wildUsed--;
				}
				if (wildUsed == 0) break;
			}
		}

	//	find out if we got any matched with the sets
	matched = 0;
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched != (set1 + set2)) return -1;

	//	get the score
	cout << endl;
	cout << "Result: ";
	for (int i = 0; i < 10; i++) {
		if (flag[i] == 1) {
			if (tmp[i].face[tmp[i].value] == 0)
				cout << "W" << " ";
			else
				cout << tmp[i].face[tmp[i].value] << " ";
			sum += tmp[i].face[tmp[i].value];
		}
	}
	cout << endl;

	return sum;

}

/*
Name: phase2
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 2
*/
int phase2() {
	int wild = 0;
	int highValue = 0;
	int flag[10] = { 0 };
	dice tmp[10];
	int valTmp[10];
	int consecutive = 0;
	int streak = 0;
	//int sum = 0;
	int matched = 0;
	int maxSet1 = 3;

	//	store the dice in a temp variable of int array to be used in the sort function
	for (int i = 0; i < 10; i++) {
		tmp[i] = gameDice[i];
		valTmp[i] = gameDice[i].face[gameDice[i].value];
	}

	//	sort the values of the dice
	sort(valTmp, valTmp + 10);

	//	store the sorted array back to a temp dice array
	for (int i = 0; i < 10; i++) {
		switch (valTmp[i]) {
		case 0: tmp[i].value = 5; break;
		case 1: tmp[i].value = 0; break;
		case 2: tmp[i].value = 1; break;
		case 3: tmp[i].value = 2; break;
		case 4: tmp[i].value = 3; break;
		case 5: tmp[i].value = 0; break;
		case 6: tmp[i].value = 1; break;
		case 7: tmp[i].value = 2; break;
		case 8: tmp[i].value = 3; break;
		case 9: tmp[i].value = 4; break;
		case 10: tmp[i].value = 5; break;
		}
	}

	//	find the longest streak of a number
	for (int i = 0; i < 10; i++) {
		consecutive = 1;
		for (int j = i + 1; j < 10; j++) {
			if (tmp[i].face[tmp[i].value] != 0) {
				if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
					consecutive++;
				}
				else {
					break;
				}
			}
		}
		if (consecutive >= streak) {
			streak = consecutive;
			highValue = tmp[i].face[tmp[i].value];
		}
	}

	//	find how many wilds there are
	for (int i = 0; i < 10; i++) {
		if (gameDice[i].face[gameDice[i].value] == 0) {
			wild++;
		}
	}

	//	flag all the dice that represent the longest streak
	if (streak != 1) {
		if (wild != 0) {
			for (int i = 0; i < 10; i++) {
				if (tmp[i].face[tmp[i].value] == highValue) {
					flag[i] = 1;
					maxSet1--;
				}
				if (maxSet1 == 0) break;
			}
		}
	}

	//	if the longest streak wasn't enough to be a set of 3
	//	use the wilds to fill the spots
	if (streak < 3) {
		if (streak != 1) {
			if (wild != 0) {
				int filler = 3 - streak;
				for (int i = 0; i < 10; i++) {
					if (tmp[i].face[tmp[i].value] == 0) {
						flag[i] = 1;
						filler--;

					}
					if (filler == 0) break;
				}
			}
		}
	}

	//	find out if we got any matched with the sets
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched == 0) return -1;
	if (matched != 3) return -1;

	//	done finding first set.

	consecutive = 0;
	streak = 0;
	highValue = highDice(tmp, flag);
	wild = numOfWild(tmp, flag);
	matched = 0;
	int run = 1;


	//	find runs
	//	find unique numbers in desc order
	int lastNum = -1;
	dice subTmp[10];
	int subSize = 0;
	int j = 0;
	int runSize = 4;
	string result = "";
	int sum = 0;

	for (int i = 9; i >= 0; i--) {
		if (tmp[i].face[tmp[i].value] != 0) {
			if (flag[i] != 1)
				if (tmp[i].face[tmp[i].value] != lastNum) {
					subTmp[j] = tmp[i];
					j++;
					lastNum = tmp[i].face[tmp[i].value];
				}
		}
	}
	/*
	for (int i = 0; i < j; i++) {
		cout << "subTmp: " << subTmp[i].face[subTmp[i].value] << endl;
	}
	*/
	subSize = j;

	if (highValue < runSize) return -1;
	for (int i = highValue; i > highValue - runSize; i--) {
		if (diceExist(i, tmp, flag)) {
			sum += i;
			result += to_string(i) + " ";
		}
		else {
			if (wild > 0) {
				result += "W ";
				wild--;
			}
			else {
				return -1;
			}
		}
	}

	int allSum = 0;
	cout << endl;
	cout << "Result: ";
	for (int i = 0; i < 10; i++) {
		if (flag[i] == 1) {
			if (tmp[i].face[tmp[i].value] == 0)
				cout << "W" << " ";
			else
				cout << tmp[i].face[tmp[i].value] << " ";
			allSum += tmp[i].face[tmp[i].value];
		}
	}

	cout << result << endl;

	sum += allSum;

	return sum;
}

/*
Name: phase3
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 3
*/
int phase3() {
	int wild = 0;
	int highValue = 0;
	int flag[10] = { 0 };
	dice tmp[10];
	int valTmp[10];
	int consecutive = 0;
	int streak = 0;
	//int sum = 0;
	int matched = 0;
	int maxSet1 = 4;

	//	store the dice in a temp variable of int array to be used in the sort function
	for (int i = 0; i < 10; i++) {
		tmp[i] = gameDice[i];
		valTmp[i] = gameDice[i].face[gameDice[i].value];
	}

	//	sort the values of the dice
	sort(valTmp, valTmp + 10);

	//	store the sorted array back to a temp dice array
	for (int i = 0; i < 10; i++) {
		switch (valTmp[i]) {
		case 0: tmp[i].value = 5; break;
		case 1: tmp[i].value = 0; break;
		case 2: tmp[i].value = 1; break;
		case 3: tmp[i].value = 2; break;
		case 4: tmp[i].value = 3; break;
		case 5: tmp[i].value = 0; break;
		case 6: tmp[i].value = 1; break;
		case 7: tmp[i].value = 2; break;
		case 8: tmp[i].value = 3; break;
		case 9: tmp[i].value = 4; break;
		case 10: tmp[i].value = 5; break;
		}
	}

	//	find the longest streak of a number
	for (int i = 0; i < 10; i++) {
		consecutive = 1;
		for (int j = i + 1; j < 10; j++) {
			if (tmp[i].face[tmp[i].value] != 0) {
				if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
					consecutive++;
				}
				else {
					break;
				}
			}
		}
		if (consecutive >= streak) {
			streak = consecutive;
			highValue = tmp[i].face[tmp[i].value];
		}
	}

	//	find how many wilds there are
	for (int i = 0; i < 10; i++) {
		if (gameDice[i].face[gameDice[i].value] == 0) {
			wild++;
		}
	}

	//	flag all the dice that represent the longest streak
	if (streak != 1) {
		if (wild != 0) {
			for (int i = 0; i < 10; i++) {
				if (tmp[i].face[tmp[i].value] == highValue) {
					flag[i] = 1;
					maxSet1--;
				}
				if (maxSet1 == 0) break;
			}
		}
	}

	//	if the longest streak wasn't enough to be a set of 3
	//	use the wilds to fill the spots
	if (streak < 4) {
		if (streak != 1) {
			if (wild != 0) {
				int filler = 4 - streak;
				for (int i = 0; i < 10; i++) {
					if (tmp[i].face[tmp[i].value] == 0) {
						flag[i] = 1;
						filler--;

					}
					if (filler == 0) break;
				}
			}
		}
	}

	//	find out if we got any matched with the sets
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched == 0) return -1;
	if (matched != 4) return -1;

	//	done finding first set.

	consecutive = 0;
	streak = 0;
	highValue = highDice(tmp, flag);
	wild = numOfWild(tmp, flag);
	matched = 0;
	int run = 1;


	//	find runs
	//	find unique numbers in desc order
	int lastNum = -1;
	dice subTmp[10];
	int subSize = 0;
	int j = 0;
	int runSize = 4;
	string result = "";
	int sum = 0;

	for (int i = 9; i >= 0; i--) {
		if (tmp[i].face[tmp[i].value] != 0) {
			if (flag[i] != 1)
				if (tmp[i].face[tmp[i].value] != lastNum) {
					subTmp[j] = tmp[i];
					j++;
					lastNum = tmp[i].face[tmp[i].value];
				}
		}
	}
	
	subSize = j;

	if (highValue < runSize) return -1;
	for (int i = highValue; i > highValue - runSize; i--) {
		if (diceExist(i, tmp, flag)) {
			sum += i;
			result += to_string(i) + " ";
		}
		else {
			if (wild > 0) {
				result += "W ";
				wild--;
			}
			else {
				return -1;
			}
		}
	}

	int allSum = 0;
	cout << endl;
	cout << "Result: ";
	for (int i = 0; i < 10; i++) {
		if (flag[i] == 1) {
			if (tmp[i].face[tmp[i].value] == 0)
				cout << "W" << " ";
			else
				cout << tmp[i].face[tmp[i].value] << " ";
			allSum += tmp[i].face[tmp[i].value];
		}
	}

	cout << result << endl;

	sum += allSum;

	return sum;
}

/*
Name: phase4
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 4
*/
int phase4() {
	int wild = 0;
	int high = 0;
	int sum = 0;
	string result = "";

	//	find how many wild's there are
	wild = numOfWild();
	high = highDice();
	if (high <= 6) return -1;
	for (int i = high; i > high - 7; i--) {
		if (diceExist(i)) {
			sum += i;
			result += to_string(i) + " ";
		}
		else {
			if (wild > 0) {
				result += "W ";
				wild--;
			}
			else {
				return -1;
			}

		}
	}
	if (high < 7) {
		cout << endl;
		cout << "Result: " << result << endl;
		if (high + wild >= 7) return sum;
	}

	cout << endl;
	cout << "Result: " << result << endl;
	return sum;
}

/*
Name: phase5
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 5
*/
int phase5() {
	int wild = 0;
	int high = 0;
	int sum = 0;
	string result = "";

	//	find how many wild's there are
	wild = numOfWild();
	high = highDice();

	for (int i = high; i > high - 8; i--) {
		if (diceExist(i)) {
			sum += i;
			result += to_string(i) + " ";
		}
		else {
			if (wild > 0) {
				result += "W ";
				wild--;
			}
			else {
				return -1;
			}

		}
	}
	if (high < 8) {
		cout << endl;
		cout << "Result: " << result << endl;
		if (high + wild >= 7) return sum;
	}

	cout << endl;
	cout << "Result: " << result << endl;
	return sum;

}

/*
Name: phase6
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 6
*/
int phase6() {
	int wild = 0;
	int high = 0;
	int sum = 0;
	string result = "";

	//	find how many wild's there are
	wild = numOfWild();
	high = highDice();

	for (int i = high; i > high - 9; i--) {
		if (diceExist(i)) {
			sum += i;
			result += to_string(i) + " ";
		}
		else {
			if (wild > 0) {
				result += "W ";
				wild--;
			}
			else {
				return -1;
			}

		}
	}
	if (high < 9) {
		cout << endl;
		cout << "Result: " << result << endl;
		if (high + wild >= 7) return sum;
	}

	cout << endl;
	cout << "Result: " << result << endl;
	return sum;

}

/*
Name: phase7
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 7
*/
int phase7() {
	int wild = 0;
	int highValue = 0;
	int flag[10] = { 0 };
	dice tmp[10];
	int valTmp[10];
	int streak = 0;
	int sum = 0;
	int matched = 0;
	int set1 = 4;
	int set2 = 4;
	//	store the dice in a temp variable of int array to be used in the sort function
	for (int i = 0; i < 10; i++) {
		tmp[i] = gameDice[i];
		valTmp[i] = gameDice[i].face[gameDice[i].value];
	}

	//	sort the values of the dice
	sort(valTmp, valTmp + 10);

	//	store the sorted array back to a temp dice array
	for (int i = 0; i < 10; i++) {
		switch (valTmp[i]) {
		case 0: tmp[i].value = 5; break;
		case 1: tmp[i].value = 0; break;
		case 2: tmp[i].value = 1; break;
		case 3: tmp[i].value = 2; break;
		case 4: tmp[i].value = 3; break;
		case 5: tmp[i].value = 0; break;
		case 6: tmp[i].value = 1; break;
		case 7: tmp[i].value = 2; break;
		case 8: tmp[i].value = 3; break;
		case 9: tmp[i].value = 4; break;
		case 10: tmp[i].value = 5; break;
		}
	}

	//	new algorithm
	int wildUsed = 0;
	int maxTarget = 0;
	int maxScore = 0;
	int maxCon = 0;
	int consecutive = 1;
	//	go through all the dice
	for (int i = 0; i < 10; i++) {
		consecutive = 1;
		//	check the next dice
		for (int j = i + 1; j < 10; j++) {
			//	check if same dice value appeared again
			if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
				consecutive++;
			}
			else {
				break;
			}
		}
		if (consecutive + numOfWild(tmp, flag) >= set1) {
			if (consecutive * tmp[i].face[tmp[i].value] > maxScore) {
				maxCon = consecutive;
				maxScore = consecutive * tmp[i].face[tmp[i].value];
				maxTarget = tmp[i].face[tmp[i].value];
			}
		}
	}

	int setMax = set1;
	//	flag the dice that match the set of 3 criteria
	for (int i = 0; i < 10; i++) {
		if (tmp[i].face[tmp[i].value] == maxTarget) {
			flag[i] = 1;
			setMax--;
		}
		if (setMax == 0) break;
	}
	wildUsed = set1 - maxCon;
	//	use the wild dice to fulfil the set of 3
	if (wildUsed > 0)
		for (int i = 0; i < 10; i++) {
			if (flag[i] != 1) {
				if (tmp[i].face[tmp[i].value] == 0) {
					flag[i] = 1;
					wildUsed--;
				}
				if (wildUsed == 0) break;
			}
		}

	//	find out if we got any matched with the sets
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched != set1) return -1;


	wildUsed = 0;
	maxTarget = 0;
	maxScore = 0;
	maxCon = 0;
	consecutive = 1;
	for (int i = 0; i < 10; i++) {
		if (flag[i] != 1) {
			consecutive = 1;
			for (int j = i + 1; j < 10; j++) {
				if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
					consecutive++;
				}
				else {
					break;
				}
			}
			if (consecutive + numOfWild(tmp, flag) >= set2) {
				if (consecutive * tmp[i].face[tmp[i].value] > maxScore) {
					maxCon = consecutive;
					maxScore = consecutive * tmp[i].face[tmp[i].value];
					maxTarget = tmp[i].face[tmp[i].value];
				}
			}
		}
	}

	setMax = set2;
	for (int i = 0; i < 10; i++) {
		if (flag[i] != 1)
			if (tmp[i].face[tmp[i].value] == maxTarget) {
				flag[i] = 1;
				setMax--;
			}
		if (setMax == 0) break;
	}
	wildUsed = set2 - maxCon;
	if (wildUsed > 0)
		for (int i = 0; i < 10; i++) {
			if (flag[i] != 1) {
				if (tmp[i].face[tmp[i].value] == 0) {
					flag[i] = 1;
					wildUsed--;
				}
				if (wildUsed == 0) break;
			}
		}

	//	find out if we got any matched with the sets
	matched = 0;
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched != (set1 + set2)) return -1;

	//	get the score
	cout << endl;
	cout << "Result: ";
	for (int i = 0; i < 10; i++) {
		if (flag[i] == 1) {
			if (tmp[i].face[tmp[i].value] == 0)
				cout << "W" << " ";
			else
				cout << tmp[i].face[tmp[i].value] << " ";
			sum += tmp[i].face[tmp[i].value];
		}
	}
	cout << endl;

	return sum;
}

/*
Name: phase8
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 8
*/
int phase8() {
	int colorMatch[4] = { 0,0,0,0 };
	int colorSum[4] = { 0,0,0,0 };
	int colorTarget = -1;
	int colorDice[10];
	int flag[10] = { 0 };

	//	count how many colors it appears in dice
	for (int i = 0; i < 10; i++) {
		colorMatch[gameDice[i].color[gameDice[i].value]]++;

		colorSum[gameDice[i].color[gameDice[i].value]] += gameDice[i].face[gameDice[i].value];
	}
	
	//	sum the dice that equals that color
	//	store the dice in a temp variable of int array to be used in the sort function
	int valTmp[10] = { 0 };
	dice tmp[10];
	for (int i = 0; i < 10; i++) {
		tmp[i] = gameDice[i];
		valTmp[i] = gameDice[i].face[gameDice[i].value];
		colorDice[i] = gameDice[i].color[gameDice[i].value];
	}

	int shortSum = 0;
	int maxColorSet = 7;
	diceColors targetColor = (diceColors)(-1);

	if (colorMatch[0] >= 7) targetColor = orange;
	if (colorMatch[1] >= 7) targetColor = red;
	if (colorMatch[2] >= 7) targetColor = blue;
	if (colorMatch[3] >= 7) targetColor = green;
	if (targetColor == -1) return -1;

	int largestNumIndex = 0;
	for (int i = 0; i < 10; i++) {
		largestNumIndex = largestDice(flag, targetColor);
		if (largestNumIndex != -1) {
			//cout << "index: " << largestNumIndex << endl;
			shortSum += gameDice[largestNumIndex].face[gameDice[largestNumIndex].value];
			flag[largestNumIndex] = 1;
			maxColorSet--;
			if (maxColorSet == 0) {
				break;
			}
		}
	}
	if (maxColorSet > 0) return -1;

	return shortSum;

	//	below method won't work with my data type.
	//	sort the values of the dice
	sort(valTmp, valTmp + 10);
	//sort(tmp->face[tmp->value], tmp->face[tmp->value] + 10);

	//	store the sorted array back to a temp dice array
	for (int i = 0; i < 10; i++) {
		switch (valTmp[i]) {
		case 0: tmp[i].value = 5; break;
		case 1: tmp[i].value = 0; break;
		case 2: tmp[i].value = 1; break;
		case 3: tmp[i].value = 2; break;
		case 4: tmp[i].value = 3; break;
		case 5: tmp[i].value = 0; break;
		case 6: tmp[i].value = 1; break;
		case 7: tmp[i].value = 2; break;
		case 8: tmp[i].value = 3; break;
		case 9: tmp[i].value = 4; break;
		case 10: tmp[i].value = 5; break;
		}
	}

	for (int i = 9; i >= 0; i--) {
		if (tmp[i].color[tmp[i].value] == targetColor) {
			shortSum += tmp[i].face[tmp[i].value];
			maxColorSet--;
		}
		if (maxColorSet == 0) {
			break;
		}

	}
	if (maxColorSet > 0) return -1;

	return shortSum;
}

/*
Name: phase9
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 9
*/
int phase9() {
	int wild = 0;
	int highValue = 0;
	int flag[10] = { 0 };
	dice tmp[10];
	int valTmp[10];
	int streak = 0;
	int sum = 0;
	int matched = 0;
	int set1 = 5;
	int set2 = 2;
	//	store the dice in a temp variable of int array to be used in the sort function
	for (int i = 0; i < 10; i++) {
		tmp[i] = gameDice[i];
		valTmp[i] = gameDice[i].face[gameDice[i].value];
	}

	//	sort the values of the dice
	sort(valTmp, valTmp + 10);

	//	store the sorted array back to a temp dice array
	for (int i = 0; i < 10; i++) {
		switch (valTmp[i]) {
		case 0: tmp[i].value = 5; break;
		case 1: tmp[i].value = 0; break;
		case 2: tmp[i].value = 1; break;
		case 3: tmp[i].value = 2; break;
		case 4: tmp[i].value = 3; break;
		case 5: tmp[i].value = 0; break;
		case 6: tmp[i].value = 1; break;
		case 7: tmp[i].value = 2; break;
		case 8: tmp[i].value = 3; break;
		case 9: tmp[i].value = 4; break;
		case 10: tmp[i].value = 5; break;
		}
	}

	//	new algorithm
	int wildUsed = 0;
	int maxTarget = 0;
	int maxScore = 0;
	int maxCon = 0;
	int consecutive = 1;
	//	go through all the dice
	for (int i = 0; i < 10; i++) {
		consecutive = 1;
		//	check the next dice
		for (int j = i + 1; j < 10; j++) {
			//	check if same dice value appeared again
			if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
				consecutive++;
			}
			else {
				break;
			}
		}
		if (consecutive + numOfWild(tmp, flag) >= set1) {
			if (consecutive * tmp[i].face[tmp[i].value] > maxScore) {
				maxCon = consecutive;
				maxScore = consecutive * tmp[i].face[tmp[i].value];
				maxTarget = tmp[i].face[tmp[i].value];
			}
		}
	}

	int setMax = set1;
	//	flag the dice that match the set of 3 criteria
	for (int i = 0; i < 10; i++) {
		if (tmp[i].face[tmp[i].value] == maxTarget) {
			flag[i] = 1;
			setMax--;
		}
		if (setMax == 0) break;
	}
	wildUsed = set1 - maxCon;
	//	use the wild dice to fulfil the set of 3
	if (wildUsed > 0)
		for (int i = 0; i < 10; i++) {
			if (flag[i] != 1) {
				if (tmp[i].face[tmp[i].value] == 0) {
					flag[i] = 1;
					wildUsed--;
				}
				if (wildUsed == 0) break;
			}
		}

	//	find out if we got any matched with the sets
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched != set1) return -1;


	wildUsed = 0;
	maxTarget = 0;
	maxScore = 0;
	maxCon = 0;
	consecutive = 1;
	for (int i = 0; i < 10; i++) {
		if (flag[i] != 1) {
			consecutive = 1;
			for (int j = i + 1; j < 10; j++) {
				if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
					consecutive++;
				}
				else {
					break;
				}
			}
			if (consecutive + numOfWild(tmp, flag) >= set2) {
				if (consecutive * tmp[i].face[tmp[i].value] > maxScore) {
					maxCon = consecutive;
					maxScore = consecutive * tmp[i].face[tmp[i].value];
					maxTarget = tmp[i].face[tmp[i].value];
				}
			}
		}
	}

	setMax = set2;
	for (int i = 0; i < 10; i++) {
		if (flag[i] != 1)
			if (tmp[i].face[tmp[i].value] == maxTarget) {
				flag[i] = 1;
				setMax--;
			}
		if (setMax == 0) break;
	}
	wildUsed = set2 - maxCon;
	if (wildUsed > 0)
		for (int i = 0; i < 10; i++) {
			if (flag[i] != 1) {
				if (tmp[i].face[tmp[i].value] == 0) {
					flag[i] = 1;
					wildUsed--;
				}
				if (wildUsed == 0) break;
			}
		}

	//	find out if we got any matched with the sets
	matched = 0;
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched != (set1 + set2)) return -1;

	//	get the score
	cout << endl;
	cout << "Result: ";
	for (int i = 0; i < 10; i++) {
		if (flag[i] == 1) {
			if (tmp[i].face[tmp[i].value] == 0)
				cout << "W" << " ";
			else
				cout << tmp[i].face[tmp[i].value] << " ";
			sum += tmp[i].face[tmp[i].value];
		}
	}
	cout << endl;

	return sum;
}

/*
Name: phase10
Parameter: void
Returns: int - score of phase
Description: calculate if the game dice passes phase 10
*/
int phase10() {
	int wild = 0;
	int highValue = 0;
	int flag[10] = { 0 };
	dice tmp[10];
	int valTmp[10];
	int streak = 0;
	int sum = 0;
	int matched = 0;
	int set1 = 5;
	int set2 = 3;
	//	store the dice in a temp variable of int array to be used in the sort function
	for (int i = 0; i < 10; i++) {
		tmp[i] = gameDice[i];
		valTmp[i] = gameDice[i].face[gameDice[i].value];
	}

	//	sort the values of the dice
	sort(valTmp, valTmp + 10);

	//	store the sorted array back to a temp dice array
	for (int i = 0; i < 10; i++) {
		switch (valTmp[i]) {
		case 0: tmp[i].value = 5; break;
		case 1: tmp[i].value = 0; break;
		case 2: tmp[i].value = 1; break;
		case 3: tmp[i].value = 2; break;
		case 4: tmp[i].value = 3; break;
		case 5: tmp[i].value = 0; break;
		case 6: tmp[i].value = 1; break;
		case 7: tmp[i].value = 2; break;
		case 8: tmp[i].value = 3; break;
		case 9: tmp[i].value = 4; break;
		case 10: tmp[i].value = 5; break;
		}
	}

	//	new algorithm
	int wildUsed = 0;
	int maxTarget = 0;
	int maxScore = 0;
	int maxCon = 0;
	int consecutive = 1;
	//	go through all the dice
	for (int i = 0; i < 10; i++) {
		consecutive = 1;
		//	check the next dice
		for (int j = i + 1; j < 10; j++) {
			//	check if same dice value appeared again
			if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
				consecutive++;
			}
			else {
				break;
			}
		}
		if (consecutive + numOfWild(tmp, flag) >= set1) {
			if (consecutive * tmp[i].face[tmp[i].value] > maxScore) {
				maxCon = consecutive;
				maxScore = consecutive * tmp[i].face[tmp[i].value];
				maxTarget = tmp[i].face[tmp[i].value];
			}
		}
	}

	int setMax = set1;
	//	flag the dice that match the set of 3 criteria
	for (int i = 0; i < 10; i++) {
		if (tmp[i].face[tmp[i].value] == maxTarget) {
			flag[i] = 1;
			setMax--;
		}
		if (setMax == 0) break;
	}
	wildUsed = set1 - maxCon;
	//	use the wild dice to fulfil the set of 3
	if (wildUsed > 0)
		for (int i = 0; i < 10; i++) {
			if (flag[i] != 1) {
				if (tmp[i].face[tmp[i].value] == 0) {
					flag[i] = 1;
					wildUsed--;
				}
				if (wildUsed == 0) break;
			}
		}

	//	find out if we got any matched with the sets
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched != set1) return -1;


	wildUsed = 0;
	maxTarget = 0;
	maxScore = 0;
	maxCon = 0;
	consecutive = 1;
	for (int i = 0; i < 10; i++) {
		if (flag[i] != 1) {
			consecutive = 1;
			for (int j = i + 1; j < 10; j++) {
				if (tmp[i].face[tmp[i].value] == tmp[j].face[tmp[j].value]) {
					consecutive++;
				}
				else {
					break;
				}
			}
			if (consecutive + numOfWild(tmp, flag) >= set2) {
				if (consecutive * tmp[i].face[tmp[i].value] > maxScore) {
					maxCon = consecutive;
					maxScore = consecutive * tmp[i].face[tmp[i].value];
					maxTarget = tmp[i].face[tmp[i].value];
				}
			}
		}
	}

	setMax = set2;
	for (int i = 0; i < 10; i++) {
		if (flag[i] != 1)
			if (tmp[i].face[tmp[i].value] == maxTarget) {
				flag[i] = 1;
				setMax--;
			}
		if (setMax == 0) break;
	}
	wildUsed = set2 - maxCon;
	if (wildUsed > 0)
		for (int i = 0; i < 10; i++) {
			if (flag[i] != 1) {
				if (tmp[i].face[tmp[i].value] == 0) {
					flag[i] = 1;
					wildUsed--;
				}
				if (wildUsed == 0) break;
			}
		}

	//	find out if we got any matched with the sets
	matched = 0;
	for (int i = 0; i < 10; i++) {
		matched += flag[i];
	}
	if (matched != (set1 + set2)) return -1;

	//	get the score
	cout << endl;
	cout << "Result: ";
	for (int i = 0; i < 10; i++) {
		if (flag[i] == 1) {
			if (tmp[i].face[tmp[i].value] == 0)
				cout << "W" << " ";
			else
				cout << tmp[i].face[tmp[i].value] << " ";
			sum += tmp[i].face[tmp[i].value];
		}
	}
	cout << endl;

	return sum;
}

/*
Name: showAllScore
Parameter:	int round - round number
			player p[] - array of players
			int size - number of players
Returns: void
Description: show all the score made in the round
*/
void showAllScore(int round, player p[], int size) {
	ClearScreen();
	cout << "Score for round " << round << endl;
	cout << "===================================" << endl;
	//	use setw to make alignment
	cout << left << setw(20) << "NAME" <<
		left << setw(10) << "PHASE" <<
		left << setw(10) << "SCORE" << endl;
	for (int i = 0; i < size; i++) {
		if (p[i].pass == 1)
			cout << left << setw(20) << p[i].name << //"[" << i << ": " << p[i].name << " ]" << 
			left << setw(10) << p[i].phase - 1 <<
			left << setw(10) << p[i].scorePhase[p[i].phase - 2] << endl;
		else
			cout << left << setw(20) << p[i].name << //"[" << i << ": " << p[i].name << " ]" << 
			left << setw(10) << p[i].phase <<
			left << setw(10) << p[i].scorePhase[p[i].phase - 1] << endl;
		p[i].pass = 0;
	}

	cout << endl;
	cout << "Press any key to continue...";
	int getint = _getch();
	if (getint == 224) getint = _getch();
}
