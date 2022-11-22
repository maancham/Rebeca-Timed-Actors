                                    


#include "Types.h"
#include "InitiateRequesterActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "ClientActor.h"
#include "CommunicationSubstrateActor.h"
#include "ExecuteInvokerActor.h"
#include "ServiceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define INITIATE 1
#define ACK 2
#define EXCESSLOADFAILURE 3
#define TIMEOUTFAILURE 4


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<InitiateRequesterActor*>(rebecs[myID]))
    	#define _ref_cs (dynamic_cast<CommunicationSubstrateActor*>(rebecs[known[1]]))
    	#define _ref_c (dynamic_cast<ClientActor*>(rebecs[known[2]]))
    
InitiateRequesterActor::InitiateRequesterActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














)
	:
		AbstractTimedActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
			#ifdef DEBUG_LEVEL_2 
				, out
			#endif







		, stateSize, hashmapSize














		)
	, AbstractActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, out
		#endif







		, stateSize, hashmapSize














	)
{
    messageNames = new char *[4 + 1];
    messagePriorities = new int[4 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"INITIATE";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"ACK";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"EXCESSLOADFAILURE";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"TIMEOUTFAILURE";
			messagePriorities[4] = std::numeric_limits<int>::max();








	if (table == (byte**)0xFFFFFFFF) {	// avoid reinitialization
		table = new byte* [1 << hashmapSize];
        memset (table, 0, sizeof(byte*)*(1 << hashmapSize));
	}








	if (hashtableTimeExtension == (vector<TimeFrame>**)0xFFFFFFFF) {	// avoid reinitialization
        hashtableTimeExtension = new vector<TimeFrame>*[1 << hashmapSize];
        memset (hashtableTimeExtension, 0, sizeof(vector<TimeFrame>*)*(1 << hashmapSize));
	}







	
	
	queueTail = 0;
}

InitiateRequesterActor::~InitiateRequesterActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void InitiateRequesterActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case INITIATE: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "initiate("
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case ACK: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "ack("
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case EXCESSLOADFAILURE: {
                out << "excessLoadFailure("
                << ")";
                break;
               }
            case TIMEOUTFAILURE: {
                out << "timeOutFailure("
                << ")";
                break;
               }
    	}
}


void InitiateRequesterActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void InitiateRequesterActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"InitiateRequester.lastIni\" type=\"Timer\">" <<  
			    		((_ref_lastIni == -1) ? string("STOP") : std::to_string(_ref_lastIni)) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"InitiateRequester.theFirsttime\" type=\"boolean\">" <<  
			    		(_ref_theFirsttime? "true" : "false") << "</variable>" << endl;
}


char* InitiateRequesterActor::getClassName() {
    return (char*)"InitiateRequester";
}
void InitiateRequesterActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_lastIni, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            

array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_theFirsttime ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++marshalIndex;    
	
}
void InitiateRequesterActor::marshalActorToArray(byte* array) {
    int i,j, marshalIndex = 0;

	for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
        array[marshalIndex++] = messageQueue[i];
    }

    if (shift) {
        array[marshalIndex++] = 0;
    }
    
	for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
        array[marshalIndex++] = senderQueue[i];
    }

    if (shift) {
        array[marshalIndex++] = 0;
    }

	if(maxParamLength != 0) {
        for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
            for (j = 0; j < maxParamLength; j++) {
                array[marshalIndex++] = paramQueue[i][j];
            }
        }
        if (shift) {
            for (j = 0; j < maxParamLength; j++) {
                array[marshalIndex++] = 0;
            }
        }
    }

	 #ifdef NO_ATOMIC_EXECUTION
    	memcpy(&array [marshalIndex], &__pc, 4);
    	marshalIndex += 4;
    	//array[marshalIndex++] = __pc;
    #endif

		marshalVariablesToArray( & array[marshalIndex]);

   

}

void InitiateRequesterActor::unmarshalActorFromArray(byte* array) {

    int index = 0, i, j;
    shift = 0;

	for (i = 0; i < maxQueueLength; i++) {
        messageQueue[i] = array[index++];
    }
	
	for (i = 0; i < maxQueueLength; i++) {
        senderQueue[i] = array[index++];
    }
    
	if(maxParamLength != 0) {
        for (i = 0; i < maxQueueLength; i++) {
            for (j = 0; j < maxParamLength; j++) {
                paramQueue[i][j] = array[index++];
            }
        }
    }
    
	#ifdef NO_ATOMIC_EXECUTION
    	memcpy(&__pc, &array [index], 4);
    	index += 4;
    	//__pc = array[index++];
    #endif
    
	unmarshalVariablesFromArray( & array[index]);
}

void InitiateRequesterActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_lastIni, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
_ref_theFirsttime = (bool) (array [index] & AbstractActor::coeff[0]);
++index;    
	
}

/*#ifdef DEBUG
void InitiateRequesterActor::printStateContent(long stateNo) {
	out << myName << "=>[";
	byte* currentValues = getTable()[stateNo];
	for (int cnt = 0; cnt < STATE_SIZE; cnt++)
		out << (int)currentValues[cnt] << ", ";
	out << "]";
}
#endif
*/

//---------------------------------------------------------
/**
  Checks which action of this rebec is enabled, and executes it. Actions are the
  same as the location-counter defined at the beginning of this file.
  In the case of non-atomic message servers, a location-counter may point to the
  middle of a message server.
  If this action leads to more than one transition (non_det), this method executes
  only one of these transitions. With the help of some static variables, consecutive
  calls to this method execute different transitions caused by this action.
  Local variables must be properly valuated before any call to this method (see marshal()).

  returns: >0 : there are more possible next states (non-det transitions).
		   =0 : no more non_det transitions
		   -1 : no enabled action (empty queue)
		   -2 : Local hash table is full - immediate termination of search
 */
long InitiateRequesterActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case INITIATE:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvinitiate(
            _ref_Lm
                    );
                }
            case ACK:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvack(
            _ref_Lm
                    );
                }
            case EXCESSLOADFAILURE:
                {
                    int i = 0;
                    return msgsrvexcessLoadFailure(
                    );
                }
            case TIMEOUTFAILURE:
                {
                    int i = 0;
                    return msgsrvtimeOutFailure(
                    );
                }
    }
	return -1;
}


    long InitiateRequesterActor::constructorInitiateRequester (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "InitiateRequester";
		#endif

	#ifdef TTS
	#endif

		{
			stopTimer(_ref_lastIni);
			(_ref_theFirsttime=true);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void InitiateRequesterActor::_msg_initiate (byte senderId
	        ,
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, INITIATE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void InitiateRequesterActor::_msg_ack (byte senderId
	        ,
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, ACK);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void InitiateRequesterActor::_msg_excessLoadFailure (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, EXCESSLOADFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void InitiateRequesterActor::_msg_timeOutFailure (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, TIMEOUTFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** InitiateRequesterActor::table = (byte**)0xFFFFFFFF;

	byte** InitiateRequesterActor::getTable() {
		return table;
	}








	vector<TimeFrame>** InitiateRequesterActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** InitiateRequesterActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void InitiateRequesterActor::_timed_msg_initiate (byte senderId
	        ,
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_initiate (senderId
	        ,
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void InitiateRequesterActor::_timed_msg_ack (byte senderId
	        ,
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_ack (senderId
	        ,
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void InitiateRequesterActor::_timed_msg_excessLoadFailure (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_excessLoadFailure (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void InitiateRequesterActor::_timed_msg_timeOutFailure (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_timeOutFailure (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void InitiateRequesterActor::setNow(TIME_TYPE now) {
	
						if (this->_ref_lastIni != -1)
		            		this->_ref_lastIni += (now - _ref_now);
		AbstractTimedActor::setNow(now);
	}	
	

	    long InitiateRequesterActor::msgsrvinitiate (
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;CommunicationSubstrateActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "initiate";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 1;

	#ifdef TTS
	#endif

		{
			int _ref_IR1Delay;

			(_ref_IR1Delay=(__tNumber|=1, nonDetVariable0));
			int _ref_passedTime = getTimerValue(_ref_lastIni);

			stopTimer(_ref_lastIni);
			if (((_ref_passedTime<_ref_N_INI)&(!_ref_theFirsttime))) {
				{
					(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 58"), temp0)->_timed_msg_fastSendFailure(myID, _ref_now, MAX_TIME);
				}
}
			else {
				{
					startTimer(_ref_lastIni);
					(temp1=_ref_cs, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 63"), temp1)->_timed_msg_initiate(myID, (_ref_Lm+_ref_IR1Delay), _ref_IR1Delay + _ref_now, MAX_TIME);
				}
}
			(_ref_theFirsttime=false);
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == 1) {
				nonDetVariable0 = 2;
			}
			else if(nonDetVariable0 == 2) {
				nonDetVariable0 = 1;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}
	    long InitiateRequesterActor::msgsrvack (
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;ClientActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "ack";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 1;

	#ifdef TTS
	#endif

		{
			int _ref_IR2Delay;

			(_ref_IR2Delay=(__tNumber|=1, nonDetVariable0));
			(_ref_Lm=(_ref_Lm+_ref_IR2Delay));
			int _ref_passedTime = getTimerValue(_ref_lastIni);

			stopTimer(_ref_lastIni);
			if ((_ref_passedTime>=_ref_L_INI)) {
				{
					(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 75"), temp0)->_timed_msg_timeOutFailure(myID, _ref_now, MAX_TIME);
				}
}
			else {
				if ((_ref_IR2Delay<_ref_L_INI)) {
					{
						(temp1=_ref_c, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 78"), temp1)->_timed_msg_ack(myID, _ref_Lm, _ref_IR2Delay + _ref_now, MAX_TIME);
					}
}}
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == 1) {
				nonDetVariable0 = 2;
			}
			else if(nonDetVariable0 == 2) {
				nonDetVariable0 = 1;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}
	    long InitiateRequesterActor::msgsrvexcessLoadFailure (
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "excessLoadFailure";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 82"), temp0)->_timed_msg_excessLoadFailure(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long InitiateRequesterActor::msgsrvtimeOutFailure (
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "timeOutFailure";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 85"), temp0)->_timed_msg_timeOutFailure(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
