//
//  main.cpp
//  inklings
//
//  Authors: Jean-Yves Hervé, Shaun Wallace, and Luis Hernandez
//

 /*-------------------------------------------------------------------------+
 |	A graphic front end for a grid+state simulation.						|
 |																			|
 |	This application simply creates a a colored grid and displays           |
 |  some state information in the terminal using ASCII art.			        |
 |	Only mess with this after everything else works and making a backup		|
 |	copy of your project.                                                   |
 |																			|
 |	Current Keyboard Events                                     			|
 |		- 'ESC' --> exit the application									|
 |		- 'r' --> add red ink												|
 |		- 'g' --> add green ink												|
 |		- 'b' --> add blue ink												|
 +-------------------------------------------------------------------------*/

#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <sys/stat.h>
#include <fstream>

#include "ascii_art.h"

//==================================================================================
//	Function prototypes
//==================================================================================
void displayGridPane(void);
void displayStatePane(void);
void initializeApplication(void);
void threadFunction(InklingInfo* inkling, int inklingID);
void getNewDirection(InklingInfo* inkling);
bool checkIfInCorner(InklingInfo* inkling);
void redColorThreadFunc();
void greenColorThreadFunc();
void blueColorThreadFunc();
bool checkEnoughInk(InklingInfo* inkling, int moveAmount);
std::string getCurrentTimestamp();
std::string travelDirectionToString(TravelDirection dir);
std::string inklingTypeToString(InklingType type);

//==================================================================================
//	Application-level global variables
//==================================================================================

//	The state grid and its dimensions
int** grid;
int NUM_ROWS, NUM_COLS;

//	the number of live threads (that haven't terminated yet)
int MAX_NUM_TRAVELER_THREADS;
int numLiveThreads = 0;

//vector to store each struct
std::vector<InklingInfo> info;
bool DRAW_COLORED_TRAVELER_HEADS = true;

//	the ink levels
int MAX_LEVEL = 50;
int MAX_ADD_INK = 10;
int REFILL_INK = 10;
int redLevel = 20, greenLevel = 10, blueLevel = 40;

// create locks for color levels
std::mutex redLock;
std::mutex blueLock;
std::mutex greenLock;
std::mutex blueCellLock;
std::mutex redCellLock;
std::mutex greenCellLock;

// ink producer sleep time (in microseconds)
// [min sleep time is arbitrary]
const int MIN_SLEEP_TIME = 30000; // 30000
int producerSleepTime = 100000; // 100000

// inkling sleep time (in microseconds)
int inklingSleepTime = 1000000; // 1000000


//==================================================================================
//	These are the functions that tie the simulation with the rendering.
//	Some parts are "don't touch."  Other parts need your help to ensure
//	that access to critical data and the ASCII art are properly synchronized
//==================================================================================

void displayGridPane(void) {
	//---------------------------------------------------------
	//	This is the call that writes ASCII art to render the grid.
	//
	//	Should we synchronize this call?
	//---------------------------------------------------------
    drawGridAndInklingsASCII(grid, NUM_ROWS, NUM_COLS, info);
}

void displayStatePane(void) {
	//---------------------------------------------------------
	//	This is the call that updates state information
	//
	//	Should we synchronize this call?
	//---------------------------------------------------------
	drawState(numLiveThreads, redLevel, greenLevel, blueLevel);
}

//------------------------------------------------------------------------
//	These are the functions that would be called by a inkling thread in
//	order to acquire red/green/blue ink to trace its trail.
//	You *must* synchronize access to the ink levels (C++ lock and unlock)
//------------------------------------------------------------------------
// You probably want to edit these...
bool acquireRedInk(int theRed) {
	bool ok = false;
	if (redLevel >= theRed)
	{
		redLevel -= theRed;
		ok = true;
	}
	return ok;
}

bool acquireGreenInk(int theGreen) {
	bool ok = false;
	if (greenLevel >= theGreen)
	{
		greenLevel -= theGreen;
		ok = true;
	}
	return ok;
}

bool acquireBlueInk(int theBlue) {
	bool ok = false;
	if (blueLevel >= theBlue)
	{
		blueLevel -= theBlue;
		ok = true;
	}
	return ok;
}

//------------------------------------------------------------------------
//	These are the functions that would be called by a producer thread in
//	order to refill the red/green/blue ink tanks.
//	You *must* synchronize access to the ink levels (C++ lock and unlock)
//------------------------------------------------------------------------
// You probably want to edit these...
bool refillRedInk(int theRed) {
	bool ok = false;
	if (redLevel + theRed <= MAX_LEVEL)
	{
		redLevel += theRed;
		ok = true;
	}
	return ok;
}

bool refillGreenInk(int theGreen) {
	bool ok = false;
	if (greenLevel + theGreen <= MAX_LEVEL)
	{
		greenLevel += theGreen;
		ok = true;
	}
	return ok;
}

bool refillBlueInk(int theBlue) {
	bool ok = false;
	if (blueLevel + theBlue <= MAX_LEVEL)
	{
		blueLevel += theBlue;
		ok = true;
	}
	return ok;
}

//------------------------------------------------------------------------
//	You shouldn't have to touch this one.  Definitely if you do not
//	add the "producer" threads, and probably not even if you do.
//------------------------------------------------------------------------
void speedupProducers(void) {
	// decrease sleep time by 20%, but don't get too small
	int newSleepTime = (8 * producerSleepTime) / 10;
	
	if (newSleepTime > MIN_SLEEP_TIME) {
		producerSleepTime = newSleepTime;
	}
}

void slowdownProducers(void) {
	// increase sleep time by 20%
	producerSleepTime = (12 * producerSleepTime) / 10;
}



//-------------------------------------------------------------------------------------
//	You need to change the TODOS in the main function to pass the the autograder tests
//-------------------------------------------------------------------------------------
int main(int argc, char** argv) {
    // a try/catch block for debugging to catch weird errors in your code
    try {
        // check that arguments are valid, must be a 20x20 or greater and at least 8 threads/inklings
        if (argc == 4) {
            if (std::stoi(argv[1]) >= 20 && std::stoi(argv[2]) >= 20 && std::stoi(argv[3]) >= 8) {
                NUM_ROWS = std::stoi(argv[1]);
                NUM_COLS = std::stoi(argv[2]);
                MAX_NUM_TRAVELER_THREADS = std::stoi(argv[3]);
                numLiveThreads = std::stoi(argv[3]);
            }
        } else {
            std::cout << "No arguments provided, running with 8x8 grid and 4 threads.\n\tThis message will dissapear in 2 seconds... \n";
            sleep(2); // so the user can read the message in std::cout one line up
            clearTerminal();
            // some small defaults, will these run?
            NUM_ROWS = 8;
            NUM_COLS = 8;
            MAX_NUM_TRAVELER_THREADS = 4;
            numLiveThreads = 4;
        }
        
		// create the logFolder directory with 0755 permissions
		system("mkdir -m 0755 -p logFolder");

        initializeFrontEnd(argc, argv, displayGridPane, displayStatePane);
        
        initializeApplication();

		std::vector<std::thread> inklingThreads;
		for (size_t i = 0; i < info.size(); i++) {
			// Capture the index and pass inkling pointer and ID
			inklingThreads.emplace_back([&, i]() {
				threadFunction(&info[i], i + 1);
			});
		}

		// join the threads
		for (auto& thread : inklingThreads) {
			if (thread.joinable()) {
				thread.join();
			}
		}

        // TODO: create producer threads that check the levels of each ink
		std::thread redProducerThread(refillRedInk, REFILL_INK);
		std::thread greenProducerThread(refillGreenInk, REFILL_INK);
		std::thread blueProducerThread(refillBlueInk, REFILL_INK);

        // TODO: create threads for the inklings

		std::thread redThread(redColorThreadFunc);
		std::thread greenThread(greenColorThreadFunc);
		std::thread blueThread(blueColorThreadFunc);

        
        // now we enter the main event loop of the program
        myEventLoop(0);

        // ensure main does not return immediately, killing detached threads
        std::this_thread::sleep_for(std::chrono::seconds(30));
        
    } catch (const std::exception& e) {
        std::cerr << "ERROR :: Oh snap! unhandled exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR :: Red handed! unknown exception caught" << std::endl;
    }

	return 0;
}


//==================================================================================
//
//	TODO this is a part that you have to edit and add to.
//
//==================================================================================

void cleanupAndQuit(const std::string& msg) {
    std::cout << "Somebody called quits, goodbye sweet digital world, this was their message: \n" << msg;
	// should we join all the threads before you free the grid and other allocated data structures.  
    // you may run into seg-fault and other ugly termination issues otherwise.
	keepRunning = false;
	if (listenerThread.joinable()) {
		listenerThread.join();
	}
	// also, if you crash there, you know something is wrong in your code.
	for (int i=0; i< NUM_ROWS; i++)
		delete []grid[i];
	delete []grid;

	// clear the inkling list
    return;
}

void initializeApplication(void) {
	//	Allocate the grid
	grid = new int*[NUM_ROWS];
	for (int i=0; i<NUM_ROWS; i++)
		grid[i] = new int[NUM_COLS];
	
	// Initialize the grid's pixels to have something to display
	std::random_device myRandomDevice;
	std::default_random_engine myEngine(myRandomDevice());
	std::uniform_int_distribution<int> distRow(1, NUM_ROWS-2); // Avoid corners
	std::uniform_int_distribution<int> distCol(1, NUM_COLS-2); // Avoid corners

	// Random number generator for inkling types
	std::mt19937 gen(myRandomDevice());
	std::uniform_int_distribution<int> dist(0, NUM_TRAV_TYPES - 1);
	std::uniform_int_distribution<int> distDir(0, NUM_TRAVEL_DIRECTIONS - 1);

	// Initialize to zero
	for (int i=0; i<NUM_ROWS; i++) {
		for (int j=0; j<NUM_COLS; j++) {
			grid[i][j] = 0;
		}
	}

	// Initialize the inklings at random locations
	for (int i = 0; i < MAX_NUM_TRAVELER_THREADS; i++) {
		bool positionFound = false;
		while (!positionFound) {
			int row = distRow(myEngine);
			int col = distCol(myEngine);
			bool occupied = false;	

			// check if the position is already occupied
			for (const auto& inkling : info) {
				if (inkling.col == col && inkling.row == row) {
					occupied = true;
					break;
				}
			}

			if (!occupied) {
				InklingType type;
				if (i % 3 == 0) {
					type = RED_TRAV;
				} else if (i % 3 == 1) {
					type = GREEN_TRAV;
				} else {
					type = BLUE_TRAV;
				}

				TravelDirection dir = static_cast<TravelDirection>(distDir(gen));

				InklingInfo inkling = {type, row, col, dir, true};
				info.push_back(inkling);
				positionFound = true;
			}
		}
	}
}

void threadFunction(InklingInfo* inkling, int inklingID) {
	// Create the log file
	std::string filename = "logFolder/inkling" + std::to_string(inklingID) + ".txt";
	std::ofstream logFile(filename);
	if (!logFile.is_open()) {
		std::cerr << "ERROR :: Could not open log file for inkling " << inklingID << std::endl;
		return;
	}

	chmod(filename.c_str(), 0755);

	// Log initial state
	logFile << getCurrentTimestamp() << ",inkling" << inklingID << ","
			<< inklingTypeToString(inkling->type)
			<< ",row" << inkling->row << ",col" << inkling->col << std::endl;
	
	while (true) {
		// Check if there is enough ink; if not, exit the loop.
		if (!checkEnoughInk(inkling, 1))
			break;

		// Determine a new direction
		getNewDirection(inkling);

		// Update position
		switch (inkling->dir) {
			case NORTH:
				inkling->row -= 1;
				break;
			case EAST:
				inkling->col += 1;
				break;
			case SOUTH:
				inkling->row += 1;
				break;
			case WEST:
				inkling->col -= 1;
				break;
			default:
				break;
		}

		// Ensure the inkling stays within bounds
		if (inkling->row < 0) inkling->row = 0;
		if (inkling->row >= NUM_ROWS) inkling->row = NUM_ROWS - 1;
		if (inkling->col < 0) inkling->col = 0;
		if (inkling->col >= NUM_COLS) inkling->col = NUM_COLS - 1;

		// Log the new state
		logFile << getCurrentTimestamp() << ",inkling" << inklingID << ","
				<< travelDirectionToString(inkling->dir)
				<< ",row" << inkling->row << ",col" << inkling->col << std::endl;
		logFile.flush();

		updateTerminal();

		// Sleep for a bit
		usleep(inklingSleepTime);
	}

	// Log the termination
	logFile << getCurrentTimestamp() << ",inkling" << inklingID << ",terminated" << std::endl;
	logFile.close();
}

// TODO help me please, I have no direction
void getNewDirection(InklingInfo* inkling) {
	std::random_device myRandomDevice;
	std::default_random_engine myEngine(myRandomDevice());
	std::uniform_int_distribution<int> dist(0, 3);

	inkling->dir = static_cast<TravelDirection>(dist(myEngine));
}

bool checkIfInCorner(InklingInfo* inkling) {
    if ((inkling->col == 0 && inkling->row == 0) ||  // Top-left
        (inkling->col == NUM_COLS - 1 && inkling->row == 0) ||  // Top-right
        (inkling->col == 0 && inkling->row == NUM_ROWS - 1) ||  // Bottom-left
        (inkling->col == NUM_COLS - 1 && inkling->row == NUM_ROWS - 1)) {  // Bottom-right
        return true;
    }
    return false;
}

bool checkEnoughInk(InklingInfo* inkling, int moveAmount) {
    bool ok = false;
    if (inkling->type == RED_TRAV) {
        std::lock_guard<std::mutex> lock(redLock);
        if (redLevel >= moveAmount) {
            redLevel -= moveAmount;
            ok = true;
        }
    } else if (inkling->type == GREEN_TRAV) {
        std::lock_guard<std::mutex> lock(greenLock);
        if (greenLevel >= moveAmount) {
            greenLevel -= moveAmount;
            ok = true;
        }
    } else if (inkling->type == BLUE_TRAV) {
        std::lock_guard<std::mutex> lock(blueLock);
        if (blueLevel >= moveAmount) {
            blueLevel -= moveAmount;
            ok = true;
        }
    }
    return ok;
}

void redColorThreadFunc() {
    while (true) {
        std::lock_guard<std::mutex> lock(redLock);
        if (redLevel < MAX_LEVEL) {
            redLevel += REFILL_INK;
            if (redLevel > MAX_LEVEL) redLevel = MAX_LEVEL;
        }
        usleep(producerSleepTime);
    }
}

void greenColorThreadFunc() {
    while (true) {
        std::lock_guard<std::mutex> lock(greenLock);
        if (greenLevel < MAX_LEVEL) {
            greenLevel += REFILL_INK;
            if (greenLevel > MAX_LEVEL) greenLevel = MAX_LEVEL;
        }
        usleep(producerSleepTime);
    }
}

void blueColorThreadFunc() {
    while (true) {
        std::lock_guard<std::mutex> lock(blueLock);
        if (blueLevel < MAX_LEVEL) {
            blueLevel += REFILL_INK;
            if (blueLevel > MAX_LEVEL) blueLevel = MAX_LEVEL;
        }
        usleep(producerSleepTime);
    }
}

// Returns the current time
std::string getCurrentTimestamp() {
	using namespace std::chrono;
	auto now = system_clock::now();
	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
	std::time_t t = system_clock::to_time_t(now);
	std::tm tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
	return oss.str();
}

// Converts TravelDirection to string
std::string travelDirectionToString(TravelDirection dir) {
	switch (dir) {
		case NORTH:
			return "north";
		case EAST:
			return "east";
		case SOUTH:
			return "south";
		case WEST:
			return "west";
		default:
			return "";
	}
}

// Convert InklingType to string
std::string inklingTypeToString(InklingType type) {
	switch (type) {
		case RED_TRAV:
			return "red";
		case GREEN_TRAV:
			return "green";
		case BLUE_TRAV:
			return "blue";
		default:
			return "";
	}
}