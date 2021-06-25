#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

void initStrings(int n, int m, std::string** events);
void initInts(int n, int m, int** LLCValues);
void readStrFile(int n, int m, std::string** events);
void readIntFile(int n, int m, int** LLCValues);

void runCalcLLC(int n, int m);
void runVerify(int n, int m);
void runBoth(int n, int m);
bool calcLLC(int n, int m, std::string** events, int** LLCValues);
bool verifyLLC(int n, int m, std::string** verifiedEvents, int** LLCValues);

int searchSendArr(int* send, int LLCValue);
bool checkVisited(int n, int m, bool** visited, std::string** verifiedEvents, int** LLCValues, int x, int y, int recvCounter);
void printEvents(int n, int m, std::string** events);
void printLLC(int n, int m, int** LCValues);
int max(int a, int b);

int main() {

	int n, m, menuOpt;

	while (true) {
		std::cout << "Menu" << std::endl;
		std::cout << "----------------------------------------------------------" << std::endl;
		std::cout << "1) Calculate Lamport logical clock values for n x m matrix" << std::endl;
		std::cout << "2) Verify Lamport logical clock values for a n x m matrix" << std::endl;
		std::cout << "3) Run option 1 and 2 in order" << std::endl;
		std::cout << "Enter selection (1/2/3): " << std::endl;
		std::cin >> menuOpt;
		if (menuOpt == 1 || menuOpt == 2 || menuOpt == 3) break;
		else std::cout << "Incorrect option please try again" << std::endl << std::endl;
	}

	//get matrix size
	std::cout << "Enter the number of processes (n): ";
	std::cin >> n;
	std::cout << "Enter the maximum number of events in a process (m): ";
	std::cin >> m;

	switch (menuOpt) {
	case 1: runCalcLLC(n, m); break;
	case 2: runVerify(n, m); break;
	case 3: runBoth(n, m); break;
	}

	return 0;
}

void initStrings(int n, int m, std::string** events) {
	for (int i = 0; i < n; i++) {
		events[i] = new std::string[m];
		for (int j = 0; j < m; j++) events[i][j] = "";
	}
}

void initInts(int n, int m, int** LLCValues) {
	for (int i = 0; i < n; i++) {
		LLCValues[i] = new int[m + 1];
		for (int j = 0; j < m + 1; j++) LLCValues[i][j] = 0;
	}
}

void readStrFile(int n, int m, std::string** events) {
	std::string filename;
	std::cout << "Enter the file name (e.g file.txt): ";
	std::cin >> filename;

	std::ifstream file;
	file.open(filename);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			file >> events[i][j];
}

void readIntFile(int n, int m, int** LLCValues)
{
	std::string filename;
	std::cout << "Enter the file name (e.g file.txt): ";
	std::cin >> filename;

	std::ifstream file;
	file.open(filename);

	for (int i = 0; i < n; i++)
		for (int j = 1; j < m + 1; j++)
			file >> LLCValues[i][j];
}

void runCalcLLC(int n, int m)
{
	bool checkExecution;

	//declare and initialize 2D array of strings to hold the events
	std::string** events = new std::string*[n];
	initStrings(n, m, events);

	//declare and initalize 2D array for storing LC values
	int** LLCValues = new int*[n];
	initInts(n, m, LLCValues);

	//read and print events
	readStrFile(n, m, events);
	std::cout << std::endl << "Events read from file:" << std::endl;
	printEvents(n, m, events);

	//calculate LC values
	checkExecution = calcLLC(n, m, events, LLCValues);

	//verify if the execution of the read events is correct
	if (checkExecution) std::cout << "Program execution is correct." << std::endl;
	else {
		std::cout << "Program execution is incorrect. Send/receive events unmatched or out of order." << std::endl;
		return;
	}

	//print LCValues
	printLLC(n, m, LLCValues);

	//delete dynamic memory
	for (int i = 0; i < n; i++) {
		delete[] events[i];
		delete[] LLCValues[i];
	}

	delete[] events;
	delete[] LLCValues;
}

void runVerify(int n, int m) 
{
	bool checkExecution;

	//declare and initialize 2D array of strings to hold the events
	std::string** events = new std::string*[n];
	initStrings(n, m, events);

	//declare and initalize 2D array for storing LC values
	int** LLCValues = new int*[n];
	initInts(n, m, LLCValues);

	//read and print Lamport logical clock values
	readIntFile(n, m, LLCValues);
	printLLC(n, m, LLCValues);

	//verify LLC values
	checkExecution = verifyLLC(n, m, events, LLCValues);

	if (checkExecution) std::cout << "Program verification is correct." << std::endl;
	else {
		std::cout << "Program verification is incorrect. No send event for one or more receive events." << std::endl;
		return;
	}

	//print verified events
	std::cout << std::endl << "Verified events:" << std::endl;
	printEvents(n, m, events);

	//delete dynamic memory
	for (int i = 0; i < n; i++) {
		delete[] events[i];
		delete[] LLCValues[i];
	}

	delete[] events;
	delete[] LLCValues;
}

void runBoth(int n, int m) {

	bool checkExecution;

	//declare and initialize 2D array of strings to hold the events
	std::string** events = new std::string * [n];
	initStrings(n, m, events);

	//declare and initalize 2D array for storing LC values
	int** LLCValues = new int* [n];
	initInts(n, m, LLCValues);

	//declare and initalize 2D array for storing verified events
	std::string** verifiedEvents = new std::string * [n];
	initStrings(n, m, verifiedEvents);

	//read and print events
	readStrFile(n, m, events);
	std::cout << std::endl << "Events read from file:" << std::endl;
	printEvents(n, m, events);

	//calculate LC values
	checkExecution = calcLLC(n, m, events, LLCValues);

	//verify if the execution of the read events is correct
	if (checkExecution) std::cout << "Program execution is correct." << std::endl;
	else {
		std::cout << "Program execution is incorrect. Send/receive events unmatched or out of order." << std::endl;
		return;
	}

	//print LCValues
	printLLC(n, m, LLCValues);

	//verify LLC values
	checkExecution = verifyLLC(n, m, verifiedEvents, LLCValues);

	if (checkExecution) std::cout << "Program verification is correct." << std::endl;
	else {
		std::cout << "Program verification is incorrect. No send event for one or more receive events." << std::endl;
		return;
	}

	//print verified events
	std::cout << std::endl << "Verified events:" << std::endl;
	printEvents(n, m, verifiedEvents);

	//delete dynamic memory
	for (int i = 0; i < n; i++) {
		delete[] events[i];
		delete[] LLCValues[i];
		delete[] verifiedEvents[i];
	}

	delete[] events;
	delete[] LLCValues;
	delete[] verifiedEvents;
}

bool calcLLC(int n, int m, std::string** events, int** LLCValues) {
	int send[10];
	bool recv[10];
	int temp;

	for (int i = 0; i < 10; i++) {
		send[i] = 0;
		recv[i] = false;
	}

	//loop 1: loop for all processes 10 times
	//loop 2: loop for all processes
	//loop 3: loop for each event in a process
	for (int i = 1; i < 10; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 1; k < m + 1; k++) {
				if (events[j][k - 1][0] == 's') {
					//calculate LC value for the send event
					LLCValues[j][k] = LLCValues[j][k - 1] + 1;
					//get the send event number
					temp = events[j][k - 1][1] - '0';
					//store the send event LC value in the sent array
					send[temp] = LLCValues[j][k];
				}
				//receive event condition
				else if (events[j][k - 1][0] == 'r') {
					//get receive event number
					temp = events[j][k - 1][1] - '0';
					//mark the receive event
					recv[temp] = true;
					//if event was not sent break inner loop
					if (send[temp] == 0) break;
					//if it was sent and it is the first event in the process
					else if (k == 1) LLCValues[j][k] = send[temp] + 1;
					//if it was sent and it is not the first event in the process
					else LLCValues[j][k] = max(send[temp], LLCValues[j][k - 1]) + 1;
				}
				//null event condition (LC value 0)
				else if (events[j][k - 1] == "NULL") break;
				//internal event condition
				else LLCValues[j][k] = LLCValues[j][k - 1] + 1;
			}
		}
	}

	//check for correct execution when calculating LLC
	for (int i = 0; i < 10; i++)
		if ((send[i] == 0 && recv[i] == true) || (send[i] != 0 && recv[i] == false)) return false;
	return true;
}

bool verifyLLC(int n, int m, std::string** verifiedEvents, int** LLCValues) {

	char internalEvent = 'a';
	bool** visited = new bool*[n];
	bool** received = new bool*[n];

	int send[10];

	int recvCounter = 1;
	int temp;
	bool found = false;
	bool check = true;

	for (int i = 0; i < 10; i++) send[i] = 0;

	for (int i = 0; i < n; i++) {
		visited[i] = new bool[m];
		received[i] = new bool[m];
		for (int j = 0; j < m; j++) {
			visited[i][j] = false;
			received[i][j] = false;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 1; k < m + 1; k++) {
				if (!visited[j][k - 1]) {
					//receive event
					if (LLCValues[j][k] - LLCValues[j][k - 1] > 1) {
						temp = searchSendArr(send, LLCValues[j][k]);
						if (temp != 0) {
							verifiedEvents[j][k - 1] = 'r';
							verifiedEvents[j][k - 1] += std::to_string(temp);
							visited[j][k - 1] = true;
							received[j][k - 1] = false;
						}
						else {
							received[j][k - 1] = true;
							found = checkVisited(n, m, visited, verifiedEvents, LLCValues, j, k, recvCounter);
							if (found) {
								send[recvCounter++] = LLCValues[j][k] - 1;
								received[j][k - 1] = false;
							}
							else break;
						}
					}
					//null event
					else if (LLCValues[j][k] == 0) verifiedEvents[j][k - 1] = "NULL";
					//send or internal event
					else {
						verifiedEvents[j][k - 1] = internalEvent++;
						visited[j][k - 1] = true;

						//stop s and r being used for internal events
						if (internalEvent == 'r' || internalEvent == 's') internalEvent++;
					}
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (received[i][j]) check = false;
		}
	}

	for (int i = 0; i < n; i++) {
		delete[] visited[i];
		delete[] received[i];
	}

	delete[] visited;
	delete[] received;

	return check;
}

int searchSendArr(int* send, int LLCValue) {
	for (int i = 0; i < 10; i++) if (send[i] == LLCValue - 1) return i;
	return 0;
}

bool checkVisited(int n, int m, bool** visited, std::string** verifiedEvents, int** LLCValues, int x, int y, int recvCounter) {
	for (int i = 0; i < n; i++) {
		for (int j = 1; j < m + 1; j++) {
			if (LLCValues[i][j] == LLCValues[x][y] - 1 && visited[i][j - 1]) {
				verifiedEvents[i][j - 1] = 's';
				verifiedEvents[i][j - 1] += std::to_string(recvCounter);
				verifiedEvents[x][y - 1] = 'r';
				verifiedEvents[x][y - 1] += std::to_string(recvCounter);
				visited[x][y - 1] = true;
				return true;
			}
		}
	}
	return false;
}

void printEvents(int n, int m, std::string** events) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			std::cout << std::setw(4) << events[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void printLLC(int n, int m, int** LCValues) {
	std::cout << std::endl << "Lamport clock values:" << std::endl;

	for (int i = 0; i < n; i++) {
		for (int j = 1; j < m + 1; j++)
			std::cout << std::setw(4) << LCValues[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int max(int a, int b) { return a > b ? a : b; }
