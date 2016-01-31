Negotiation answer registrator problem
======================================

Welcome to the Negotiation answer registrator project !!!!.

--------------------------
Description of the problem
--------------------------

For a sociological study, your customer is running a trial to link the role of positive language with the outcome of a negotiation. Your customer wants to count how many times the negotiator (Dr. Pater) says *yes* and *no* during the negotiation (which will be a casual conversation between Dr. Pater and his collaborator Dr. Fils). At the end, your customer wishes to prove that if *yes* is said systematically more often than *no*, the negotiation is more likely to succeed.

To achieve this, your customer wants you to make a software that counts the number of *yes* and *no* in real time by hitting the keys *y* and *n* of the keyboard as the negotiation goes on. Your customer uses Ubuntu 14.04, but is considering sharing the software with colleagues that might use different operating systems.

-----------------------
Functional requirements
-----------------------

You must code a C++ program that waits for keyboard input from the user. The user input will always be a key stroke, and can be any of the following:

- *y*: the program should increase the count of *yes's*.
- *n*: the program should increase the count of *no's*.
- *z*: the program should undo the last operation, that is decrease the number of *yes's* or *no's* by one (depending what the last operation was). This allows corrections of user mistakes.
- *s*: the program should show a summary of the count so far. Details on what the summary should look like are given below.
- *q*: the program should exit, showing the summary before.
- *h*: the program should show a help message, explaining how to use the software.

The summary should include the number of *yes*, the number of *no*, and the ratio between the two. It should also show the elapsed time and the number of *yes* and *no* per minute. This output should be given in a reasonably formatted way.

--------------------------
Description of the solution
---------------------------

The solution to this problem is a program written in C++, which uses the main thread to captures the key strokes, but let each key stroke be processed in a separate thread. Mutual exclusion is provided via a Mutex. See more details below.

### Dependencies

The C++ program depends on the [pthread library] (https://computing.llnl.gov/tutorials/pthreads/). The library is portable over OS X, Linux and Windows. It comes pre-installed in Ubuntu 14.0 and OS X, so there is no need to install anything additional on these platforms. For windows, I used [pthread-win32] (https://www.sourceware.org/pthreads-win32/). The necessary libs, includes and dlls to use pthread-win32 are included in this repository.

For Linux and OS X, you have the choice of using [ncurses] (https://www.gnu.org/software/ncurses/) for I/O in the terminal. If that is the case, ncurses needs to be installed in your PC before attempting to compile the code. For Linux, you have the choice of not using ncurses, since an alternative header for dealing with the I/O is provided.

### Portability

This code can be compiled and executed in Windows, OS X and Linux. For Windows, it has been tested successfully with Windows 8.1 and MS Visual Studio 2010. For Linux, it has been tested successfully in Ubuntu 14.04. It remains to be tested in OS X, but since all the dependencies are portable to OS X, it should not fail.

### GUI

The program is basically console-based with a minimal GUI, for simplicity and portability. [Tiny file dialogs] (http://sourceforge.net/projects/tinyfiledialogs/) was used for displaying message boxes when the user enters *s*, *h* or *q*. The code of tiny file dialogs is included in this repository without alteration. This library is also portable to Windows, OS X and Linux.

### Getting the code

Download this repository as ZIP or clone it in your PC by doing: 'git clone https://github.com/ecoto/SETestKCL.git'

### Building

A CMakeList.txt is provided to build the project solution with CMake. 

For Windows, I recommend using cmake-gui. No CMake variable needs to be set, you just need to specify where is the source code and where to build the binaries. If you use MS Visual Studio, like me, after generating the solution with CMake, just open the solution in Visual Studio and build the projects. Before execution, build the INSTALL project too, so that the dll of phtread-win32 is deployed to the executable directory.

For Ubuntu 14.04, you can use cmake from the command-line. Use 'cmake CMakeList.txt' to build without *ncurses* support, otherwise use 'cmake -D USE_NCURSES_LIB=True CMakeLists.txt'. After CMake finishes, compile using 'make'. The executable will be in the *bin* directory within the project directory. 

As a last alternative for Ubuntu 14.04, if you don't want to use CMake, simple execute 'g++ -pthread -std=c++11 Negotiation.cpp NegotiationRecorder.cpp tinyfiledialogs.c -o Negotiation -lncurses' inside the *src* directory to generate the executable.

### Execution

The name of the executable file is 'Negotiation' ('Negotiation.exe' in Windows).

Open a terminal and start the program from the command line. After that, start pressing the keys described in the *Functional Requirements* section. When pressing *y* or *n* a simple string will show up in the terminal window indicating that the answer has been registered. When pressing *z*, a simple string will show up in the terminal window indicating that the last answer has been unregistered. You can unregister answers up until the number of *yes* and *no* goes back to zero, i.e., up to the beginning of the registration process. When *s*, *h* or *q*, a graphical message box will show up in the screen with the respective information. In the case of *q*, the registration process will end after the message box is closed.

### Documentation

Doxygen-style comments have been added to the code. A doxygen configuration file is provided in this repository to automatically build the documentation. The documentation can be built using *Doxywizard* or by doing: 'doxygen Doxyfile' in the command line.

