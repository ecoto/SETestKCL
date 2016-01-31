#include "NegotiationRecorder.h"
#include <stdio.h>

/**
 * Class constructor
 */
NegotiationRecorder::NegotiationRecorder()
{
	m_Mutex = PTHREAD_MUTEX_INITIALIZER;
}

/**
 * Class destructor
 */
NegotiationRecorder::~NegotiationRecorder()
{
}

/**
 * Initializes the registration process
 */
void NegotiationRecorder::initRegistration()
{ 
	m_counterYes = m_counterNo = 0;
	m_rvYes = m_rvNo = m_rvUndo = -1;
	time (&m_startTime);
	m_lastOperation.clear();
}

/**
 * Finishes the registration process
 */
void NegotiationRecorder::endRegistration()
{ 
	// only do the join if the corresponding thread was actually created
	if (m_rvYes != -1)	pthread_join( m_threadYes, NULL);
	if (m_rvUndo != -1)	pthread_join( m_threadUndo, NULL);
	if (m_rvNo != -1)	pthread_join( m_threadNo, NULL);
	m_rvYes = m_rvNo = m_rvUndo = -1;
}

/**
 * Returns the number of 'Yes' in the registration process
 *
 * \return the number of 'Yes' in the registration process
 */
int NegotiationRecorder::getCounterYes() 
{ 
	return m_counterYes; 
}
	
/**
 * Returns the number of 'Yes' per minute during the registration process
 *
 * \note Note than when the elapsed time is less than a minute, the returned number is mostly a proyection of what the number of 'Yes' will be by the end of the first minute.
 * \return the number of 'Yes' per minute during the registration process
 */
double NegotiationRecorder::getYesPerMin()
{
	return m_yesPerMin;
}

/**
 * Returns the number of 'No' in the registration process
 *
 * \return the number of 'No' in the registration process
 */
int NegotiationRecorder::getCounterNo() 
{ 
	return m_counterNo; 
}

/**
 * Returns the number of 'No' per minute during the registration process
 *
 * \note Note than when the elapsed time is less than a minute, the returned number is mostly a proyection of what the number of 'No' will be by the end of the first minute.
 * \return the number of 'No' per minute during the registration process
 */
double NegotiationRecorder::getNoPerMin()
{
	return m_noPerMin;
}

/**
 * Returns the ratio between the number of 'Yes' and the number of 'No'
 * 
 * Calculates number of 'Yes' divided by the number of 'No'.
 * 
 * \return -1 if the number of 'No' is equal to zero, otherwise it returns number of 'Yes' divided by the number of 'No'.
 */
float NegotiationRecorder::getRatioYesNo() 
{ 
	return (m_counterNo>0) ? float(m_counterYes)/float(m_counterNo) : -1; 
}

/**
 * Returns the elapsed time since the negotation started, in seconds.
 * 
 * \note It also updates the number of 'Yes' and 'No' per minute.
 * \return the elapsed time since the negotation started, in seconds.
 */
int NegotiationRecorder::getRegistrationElapsedSecs()
{
	pthread_mutex_lock( &m_Mutex );
	
	time_t nowTime;
	time (&nowTime);
	m_elapsedSecs = int( difftime (nowTime,m_startTime) );
	double elapseMins =  m_elapsedSecs/60.0;
	m_yesPerMin = double(m_counterYes)*60.0/m_elapsedSecs;
	m_noPerMin = double(m_counterNo)*60.0/m_elapsedSecs;

	pthread_mutex_unlock( &m_Mutex );

	return m_elapsedSecs;
}

/**
 * Registers a 'Yes'
 *
 * Starts a separate thread to perform the registration
 *
 * \return true is the thread was started successfully, it returns false otherwise
 */
bool NegotiationRecorder::increaseYes()
{
	return (m_rvYes = pthread_create( &m_threadYes, NULL, &functionYesEntryFunc, this) ==0);
}

/**
 * Registers a 'No'
 *
 * Starts a separate thread to perform the registration
 *
 * \return true is the thread was started successfully, it returns false otherwise
 */
bool NegotiationRecorder::increaseNo()
{
	return (m_rvNo = pthread_create( &m_threadNo, NULL, &functionNoEntryFunc, this) ==0);
}

/**
 * Undo the last registered operation
 *
 * It will continue to Undo operations if invoked repeatedly.
 *
 * \return true is the thread was started successfully, it returns false otherwise
 */
bool NegotiationRecorder::undoLastIncrement()
{
	return (m_rvUndo = pthread_create( &m_threadUndo, NULL, &functionUndontryFunc, this) ==0);
}

/**
 * Thread function to register a 'Yes'
 *
 * Registers a 'Yes', updates the elapse time and the number of 'Yes' and 'No' per minute
 */
void NegotiationRecorder::functionYes()
{
	pthread_mutex_lock( &m_Mutex );
	
	m_counterYes++;
	m_lastOperation.push_back('y');
	time_t nowTime;
	time (&nowTime);
	m_elapsedSecs = int( difftime (nowTime,m_startTime) );
	double elapseMins =  m_elapsedSecs/60.0;
	m_yesPerMin = double(m_counterYes)*60.0/m_elapsedSecs;
	m_noPerMin = double(m_counterNo)*60.0/m_elapsedSecs;

	pthread_mutex_unlock( &m_Mutex );
}

/**
 * Thread function to register a 'No'
 *
 * Registers a 'No', updates the elapse time and the number of 'Yes' and 'No' per minute *
 */
void NegotiationRecorder::functionNo()
{
	pthread_mutex_lock( &m_Mutex );

	m_counterNo++;
	m_lastOperation.push_back( 'n');
	time_t nowTime;
	time (&nowTime);
	m_elapsedSecs = int( difftime (nowTime,m_startTime) );
	double elapseMins =  m_elapsedSecs/60.0;
	m_yesPerMin = double(m_counterYes)*60.0/m_elapsedSecs;
	m_noPerMin = double(m_counterNo)*60.0/m_elapsedSecs;

	pthread_mutex_unlock( &m_Mutex );
}

/**
 * Thread function to perform an 'Undo'
 *
 * Removes the last entry in the list of registered answers
 */
void NegotiationRecorder::functionUndo()
{
	pthread_mutex_lock( &m_Mutex );

	if (m_lastOperation.size() > 0) {
		if (m_lastOperation.back() == 'y') m_counterYes--;
		else if (m_lastOperation.back() == 'n') m_counterNo--;
		m_lastOperation.pop_back(); 
	}

	pthread_mutex_unlock( &m_Mutex );
}

