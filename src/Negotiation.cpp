// Some standard includes
#include <stdio.h>
#include <stdlib.h>
#include <sstream> 
#include <iomanip>
#ifdef _WIN32
    #include <conio.h>
#else
    #ifdef USE_NCURSES_LIB
        #include <curses.h>
    #elif __linux__ 
        // Prefer our own implementation of _getch() in Linux
        // taken from http://stackoverflow.com/questions/7469139/what-is-equivalent-to-getch-getche-in-linux
        #include "_getch.h"
    #endif
#endif
// Use simple tiny file dialogs for the gui http://sourceforge.net/projects/tinyfiledialogs/
#include "tinyfiledialogs.h"
// Our own Negotation Recorder
#include "NegotiationRecorder.h"

/**
 * \mainpage Proposed solution to the Negotation answer registration problem
 * 
 * \section Context
 * 
 * For a sociological study, your customer is running a trial to link the role of
 * positive language with the outcome of a negotiation. Your customer wants to
 * count how many times the negotiator (Dr. Pater) says 'yes' and 'no' during
 * the negotiation (which will be a casual conversation between Dr. Pater and his
 * collaborator Dr. Fils). At the end, your customer wishes to prove that if 'yes'
 * is said systematically more often than 'no', the negotiation is more likely to
 * succeed.
 *
 * To achieve this your customer wants you to make a software that counts
 * the number of 'yes' and 'no' in real time by hitting the keys 'y' and 'n' of the
 * keyboard as the negotiation goes on.
 *
 * 
 * \section Requirements
 *
 * You must code a C++ program that waits for keyboard input from the user. The user input will always be a key stroke, and can be any of the following:
 * - y: the program should increase the count of yes's.
 * - n: the program should increase the count of no's.
 * - z: the program should undo the last operation, that is decrease the number of yes's or no's by one (depending what the last operation was). This allows  corrections of user mistakes.
 * - s: the program should show a summary of the count so far. Details on what the summary should look like are given below.
 * - q: the program should exit, showing the summary before.
 * - h: the program should show a help message, explaining how to use the software.
 *
 * The summary should include the number of 'yes', the number of 'no', and the ratio between the two. It should also show the elapsed time and the number of 'yes' and 'no' per minute. This output should be given in a reasonably formatted way.
 * 
 * \author Ernesto Coto
 * \date January, 2016
 */
int main()
{
    #ifdef USE_NCURSES_LIB
        initscr();  // Start curses mode 
        noecho();
        nodelay(stdscr, TRUE);
    #endif
    
	// Instantiate negotiation recorder
	NegotiationRecorder negReg;

	// Initialize registration process
	negReg.initRegistration();

	// Use the main thread to captures the keystrokes, but let each keystroke be processed in a separate thread.
	char inChar =' ';
	while (inChar != 'q') 
	{
        #ifdef USE_NCURSES_LIB
            inChar = getch();
        #else
            inChar = _getch();
        #endif
		switch(inChar)
		{
			case 'y': // a 'Yes' must be registered
				{
					if (!negReg.increaseYes()) 
					{ 
						printf("Failed to increase Yes\n");
					}
					else
					{
						printf("Increased Yes\n");
					}
				}
				break;

			case 'n': // a 'No' must be registered
				{
					if (!negReg.increaseNo()) 
					{
						printf("Failed to increase No\n");
					}
					else
					{
						printf("Increased No\n");
					}
				}
				break;

			case 'z': // Undo the last registered operation
				{
					if (!negReg.undoLastIncrement())  
					{
						printf("Failed to undo last operation\n");
					}
					else
					{
						printf("Undid last operation\n");
					}
				}
				break;
			
			case 's': // Print a summary of the registration process
				{     

					int elapsedSecs =  negReg.getRegistrationElapsedSecs();
					std::stringstream convert; 
					convert << "#Yes: " << negReg.getCounterYes() <<  std::endl 
							<< "#No: " << negReg.getCounterNo() <<  std::endl << std::fixed
							<< "Ratio Yes/No: " ;
					if (negReg.getRatioYesNo()>=0)
					{
						convert << negReg.getRatioYesNo() << std::endl; 
					}
					else
					{
						convert << "NaN" << std::endl; // when the ratio is negative, indicating an invalid number, just print Not-A-Number
					}
					convert << "Elapsed time (min): " << elapsedSecs/60 << "m " <<  std::setfill('0') << std::setw(2) << elapsedSecs%60 <<  "s " << std::endl
							<< "#Yes/min: " <<  negReg.getYesPerMin() <<  std::endl
							<< "#No/min: " <<  negReg.getNoPerMin() << std::endl;

					// Use a simple graphical MessageBox to show the summary.
					tinyfd_messageBox( "Negotiation summary", convert.str().c_str(),  "ok", "info",	1 );

				}
				break;

			case 'h': // Print the tipical help 'Usage' message
				{
					std::stringstream help; 
					help << "Usage:" << std::endl 
						<< "\tEnter \'y\' to register a \'Yes\'" << std::endl 
						<< "\tEnter \'n\' to register a \'No\'" << std::endl 
						<< "\tEnter \'z\' to to undo the last registration" << std::endl 
						<< "\tEnter \'s\' to display a summary of the registration process" << std::endl 
						<< "\tEnter \'h\' to display this message" << std::endl 
						<< "\tEnter \'q\' to exit the program" << std::endl;

					// Use a simple graphical MessageBox to show the 'Usage'.
					tinyfd_messageBox( "Negotation answer registration program", help.str().c_str(),  "ok", "info",	1 );

				}
				break;

			case 'q': // Exit the simulation, but print a final summary first
				{
					int elapsedSecs =  negReg.getRegistrationElapsedSecs();
					std::stringstream convert; 
					convert << "#Yes: " << negReg.getCounterYes() <<  std::endl 
							<< "#No: " << negReg.getCounterNo() <<  std::endl << std::fixed
							<< "Ratio Yes/No: " ;
					if (negReg.getRatioYesNo()>=0)
					{
						convert << negReg.getRatioYesNo() << std::endl;
					}
					else
					{
						convert << "NaN" << std::endl; // when the ratio is negative, indicating an invalid number, just print Not-A-Number
					}
					convert << "Elapsed time (min): " << elapsedSecs/60 << "m " <<  std::setfill('0') << std::setw(2) << elapsedSecs%60 << "s " << std::endl
							<< "#Yes/min: " <<  negReg.getYesPerMin() <<  std::endl
							<< "#No/min: " <<  negReg.getNoPerMin() << std::endl;

					// Use a simple graphical MessageBox to show the final summary.
					tinyfd_messageBox( "Final results", convert.str().c_str(),  "ok", "info",	1 );

					// Finish the registration process
					negReg.endRegistration();
				}
				break;

		} // end switch

	} // end while

    #ifdef USE_NCURSES_LIB
        endwin();	
    #endif
	return 0;
}