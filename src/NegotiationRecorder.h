#include <vector>
#include <pthread.h>
#include <time.h>

#ifndef __NegotiationRecorder_h
#define __NegotiationRecorder_h

/**
 * Class to keep track of the negotiation answers between two people. 
 *
 * Class to keep count of the number of 'Yes' and 'No' in the negation, elapsed negotiation time, 'Yes'/'No' ratio, number of 'Yes' per minute and number of 'No' per minute.
 * 
 * \author Ernesto Coto
 * \date January 2016
 */
class NegotiationRecorder
{
	 //! Mutual exclusion variable
	 pthread_mutex_t m_Mutex;

	 //! 'Yes' registration thread
	 pthread_t m_threadYes;

	 //! 'No' registration thread
	 pthread_t m_threadNo;

	 //! 'Undo' registration thread
	 pthread_t m_threadUndo;

 	 //! 'Yes' thread creation control variable
	 int m_rvYes;
 	 
	 //! 'No' thread creation control variable
	 int m_rvNo;

	 //! 'Undo' thread creation control variable
	 int m_rvUndo;

 	 //! 'Yes' counter
	 int  m_counterYes;

	 //! 'No' counter
	 int  m_counterNo;

 	 //! List of registered answers
	 std::vector<char> m_lastOperation;

	 //! Start time of registration process
	 time_t m_startTime;

	 //! Elapsed time of registration process since the start time
 	 int m_elapsedSecs;

	 //! Number of 'Yes' per minute
	 double m_yesPerMin;

	 //! Number of 'No' per minute
	 double m_noPerMin;

public:
	
	//! Class constructor
	NegotiationRecorder();

	//! Class destructor
	~NegotiationRecorder();

	//! Initializes the registration process
	void initRegistration();

	//! Finishes the registration process
	void endRegistration();

	//! Returns the number of 'Yes' in the registration process
	int getCounterYes();

	//! Returns the number of 'Yes' per minute during the registration process
	double getYesPerMin();

	//! Returns the number of 'No' in the registration process
	int getCounterNo();

	//! Returns the number of 'No' per minute during the registration process
	double getNoPerMin();

	//! Returns the ratio between the number of 'Yes' and the number of 'No'
	float getRatioYesNo();

	//! Returns the elapsed time since the negotation started
	int getRegistrationElapsedSecs();

	//! Registers a 'Yes'
	bool increaseYes();

	//! Registers a 'No'
	bool increaseNo();

	//! Undo the last registered operation
	bool undoLastIncrement();

private:

	//! Auxiliary funcion to redirect a thread call to functionYes()
	static void * functionYesEntryFunc(void * This) {	((NegotiationRecorder *)This)->functionYes();	return NULL;}

	//! Auxiliary funcion to redirect a thread call to functionNo()
	static void * functionNoEntryFunc(void * This)	{	((NegotiationRecorder *)This)->functionNo();	return NULL;}

	//! Auxiliary funcion to redirect a thread call to functionUndo()
	static void * functionUndontryFunc(void * This) {	((NegotiationRecorder *)This)->functionUndo();	return NULL;}
	
	//! Thread function to register a 'Yes'
    void functionYes();

	//! Thread function to register a 'No'
	void functionNo();

	//! Thread function to perform an 'Undo'
	void functionUndo();

};

#endif // __NegotiationRecorder_h
