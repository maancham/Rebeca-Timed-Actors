                                    


#include "Types.h"
#include "ReceiveInvokerActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "ClientActor.h"
#include "SendRequesterActor.h"
#include "CommunicationSubstrateActor.h"
#include "ServiceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define SEND 1
#define ACK 2


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<ReceiveInvokerActor*>(rebecs[myID]))
    	#define _ref_cs (dynamic_cast<CommunicationSubstrateActor*>(rebecs[known[1]]))
    	#define _ref_s (dynamic_cast<ServiceActor*>(rebecs[known[2]]))
    
ReceiveInvokerActor::ReceiveInvokerActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[2 + 1];
    messagePriorities = new int[2 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"SEND";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"ACK";
			messagePriorities[2] = std::numeric_limits<int>::max();








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

ReceiveInvokerActor::~ReceiveInvokerActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void ReceiveInvokerActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case SEND: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "send("
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
    	}
}


void ReceiveInvokerActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void ReceiveInvokerActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"ReceiveInvoker.time\" type=\"int\">" <<  
			    		((int)_ref_time) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"ReceiveInvoker.nRec\" type=\"int\">" <<  
			    		((int)_ref_nRec) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"ReceiveInvoker.lastSend\" type=\"Timer\">" <<  
			    		((_ref_lastSend == -1) ? string("STOP") : std::to_string(_ref_lastSend)) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"ReceiveInvoker.theFirsttime\" type=\"boolean\">" <<  
			    		(_ref_theFirsttime? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"ReceiveInvoker.lRec\" type=\"int\">" <<  
			    		((int)_ref_lRec) << "</variable>" << endl;
}


char* ReceiveInvokerActor::getClassName() {
    return (char*)"ReceiveInvoker";
}
void ReceiveInvokerActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_time, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_nRec, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_lastSend, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
            
                	memcpy(&array [marshalIndex], &_ref_lRec, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_theFirsttime ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++marshalIndex;    
	
}
void ReceiveInvokerActor::marshalActorToArray(byte* array) {
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

void ReceiveInvokerActor::unmarshalActorFromArray(byte* array) {

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

void ReceiveInvokerActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_time, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_nRec, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_lastSend, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            

            
            
                	memcpy(&_ref_lRec, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
_ref_theFirsttime = (bool) (array [index] & AbstractActor::coeff[0]);
++index;    
	
}

/*#ifdef DEBUG
void ReceiveInvokerActor::printStateContent(long stateNo) {
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
long ReceiveInvokerActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case SEND:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvsend(
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
    }
	return -1;
}


    long ReceiveInvokerActor::constructorReceiveInvoker (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "ReceiveInvoker";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_nRec=5);
			(_ref_lRec=12);
			(_ref_time=0);
			stopTimer(_ref_lastSend);
			(_ref_theFirsttime=true);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void ReceiveInvokerActor::_msg_send (byte senderId
	        ,
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, SEND);
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

		void ReceiveInvokerActor::_msg_ack (byte senderId
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









	byte** ReceiveInvokerActor::table = (byte**)0xFFFFFFFF;

	byte** ReceiveInvokerActor::getTable() {
		return table;
	}








	vector<TimeFrame>** ReceiveInvokerActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** ReceiveInvokerActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void ReceiveInvokerActor::_timed_msg_send (byte senderId
	        ,
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_send (senderId
	        ,
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ReceiveInvokerActor::_timed_msg_ack (byte senderId
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
	void ReceiveInvokerActor::setNow(TIME_TYPE now) {
	
						if (this->_ref_lastSend != -1)
		            		this->_ref_lastSend += (now - _ref_now);
		AbstractTimedActor::setNow(now);
	}	
	

	    long ReceiveInvokerActor::msgsrvsend (
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;CommunicationSubstrateActor *temp0;ServiceActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "send";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 1;

	#ifdef TTS
	#endif

		{
			int _ref_RI1Delay;

			(_ref_RI1Delay=(__tNumber|=1, nonDetVariable0));
			int _ref_passedTime = getTimerValue(_ref_lastSend);

			stopTimer(_ref_lastSend);
			(_ref_Lm=(_ref_Lm+_ref_RI1Delay));
			if (((_ref_passedTime<_ref_nRec)&(!_ref_theFirsttime))) {
				{
					(temp0=_ref_cs, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 152"), temp0)->_timed_msg_excessLoadFailure(myID, _ref_Lm, _ref_now, MAX_TIME);
				}
}
			else {
				{
					startTimer(_ref_lastSend);
					(temp1=_ref_s, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 157"), temp1)->_timed_msg_send(myID, _ref_Lm, _ref_RI1Delay + _ref_now, MAX_TIME);
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
	    long ReceiveInvokerActor::msgsrvack (
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;CommunicationSubstrateActor *temp0;CommunicationSubstrateActor *temp1;
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
			int _ref_passedTime = getTimerValue(_ref_lastSend);

			stopTimer(_ref_lastSend);
			int _ref_RI2Delay;

			(_ref_RI2Delay=(__tNumber|=1, nonDetVariable0));
			if ((_ref_passedTime>_ref_lRec)) {
				{
					(temp0=_ref_cs, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 169"), temp0)->_timed_msg_timeOutFailure(myID, (_ref_Lm+_ref_RI2Delay), _ref_now, MAX_TIME);
				}
}
			else {
				{
					(temp1=_ref_cs, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 172"), temp1)->_timed_msg_ack(myID, (_ref_Lm+_ref_RI2Delay), _ref_RI2Delay + _ref_now, MAX_TIME);
				}
}
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
