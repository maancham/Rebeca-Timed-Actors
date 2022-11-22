                                    


#include "Types.h"
#include "RequestRequesterActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "ClientActor.h"
#include "CommunicationSubstrateActor.h"
#include "ResponderInvokerActor.h"
#include "ServiceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define REQUEST 1
#define RESPONSE 2
#define EXCESSLOADFAILURE 3
#define TIMEOUTFAILURE 4
#define DATAUNAVAILABLEFAILURE 5


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<RequestRequesterActor*>(rebecs[myID]))
    	#define _ref_cs (dynamic_cast<CommunicationSubstrateActor*>(rebecs[known[1]]))
    	#define _ref_c (dynamic_cast<ClientActor*>(rebecs[known[2]]))
    
RequestRequesterActor::RequestRequesterActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[5 + 1];
    messagePriorities = new int[5 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"REQUEST";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"RESPONSE";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"EXCESSLOADFAILURE";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"TIMEOUTFAILURE";
			messagePriorities[4] = std::numeric_limits<int>::max();
       		 messageNames[5] = (char*)"DATAUNAVAILABLEFAILURE";
			messagePriorities[5] = std::numeric_limits<int>::max();








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

RequestRequesterActor::~RequestRequesterActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void RequestRequesterActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case REQUEST: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "request("
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case RESPONSE: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_life;
            
            
                	memcpy(&_ref_life, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "response("
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_life)
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
            case DATAUNAVAILABLEFAILURE: {
                out << "dataUnavailableFailure("
                << ")";
                break;
               }
    	}
}


void RequestRequesterActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void RequestRequesterActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"RequestRequester.lastReq\" type=\"Timer\">" <<  
			    		((_ref_lastReq == -1) ? string("STOP") : std::to_string(_ref_lastReq)) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"RequestRequester.theFirsttime\" type=\"boolean\">" <<  
			    		(_ref_theFirsttime? "true" : "false") << "</variable>" << endl;
}


char* RequestRequesterActor::getClassName() {
    return (char*)"RequestRequester";
}
void RequestRequesterActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_lastReq, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            

array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_theFirsttime ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++marshalIndex;    
	
}
void RequestRequesterActor::marshalActorToArray(byte* array) {
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

void RequestRequesterActor::unmarshalActorFromArray(byte* array) {

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

void RequestRequesterActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_lastReq, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
_ref_theFirsttime = (bool) (array [index] & AbstractActor::coeff[0]);
++index;    
	
}

/*#ifdef DEBUG
void RequestRequesterActor::printStateContent(long stateNo) {
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
long RequestRequesterActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case REQUEST:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvrequest(
            _ref_Lm
                    );
                }
            case RESPONSE:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_life;
            
            
                	memcpy(&_ref_life, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvresponse(
            _ref_Lm
	        , 
            _ref_life
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
            case DATAUNAVAILABLEFAILURE:
                {
                    int i = 0;
                    return msgsrvdataUnavailableFailure(
                    );
                }
    }
	return -1;
}


    long RequestRequesterActor::constructorRequestRequester (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "RequestRequester";
		#endif

	#ifdef TTS
	#endif

		{
			stopTimer(_ref_lastReq);
			(_ref_theFirsttime=true);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void RequestRequesterActor::_msg_request (byte senderId
	        ,
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, REQUEST);
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

		void RequestRequesterActor::_msg_response (byte senderId
	        ,
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
  	  ){
        queueTail = 0;
        enqueue(messageQueue, RESPONSE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_life, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void RequestRequesterActor::_msg_excessLoadFailure (byte senderId
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

		void RequestRequesterActor::_msg_timeOutFailure (byte senderId
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

		void RequestRequesterActor::_msg_dataUnavailableFailure (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, DATAUNAVAILABLEFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** RequestRequesterActor::table = (byte**)0xFFFFFFFF;

	byte** RequestRequesterActor::getTable() {
		return table;
	}








	vector<TimeFrame>** RequestRequesterActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** RequestRequesterActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void RequestRequesterActor::_timed_msg_request (byte senderId
	        ,
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_request (senderId
	        ,
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void RequestRequesterActor::_timed_msg_response (byte senderId
	        ,
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_response (senderId
	        ,
            _ref_Lm
	        , 
            _ref_life
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void RequestRequesterActor::_timed_msg_excessLoadFailure (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_excessLoadFailure (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void RequestRequesterActor::_timed_msg_timeOutFailure (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_timeOutFailure (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void RequestRequesterActor::_timed_msg_dataUnavailableFailure (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_dataUnavailableFailure (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void RequestRequesterActor::setNow(TIME_TYPE now) {
	
						if (this->_ref_lastReq != -1)
		            		this->_ref_lastReq += (now - _ref_now);
		AbstractTimedActor::setNow(now);
	}	
	

	    long RequestRequesterActor::msgsrvrequest (
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;CommunicationSubstrateActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "request";
		#endif

	#ifdef TTS
	#endif

		{
			int _ref_RR1Delay;

			(_ref_RR1Delay=2);
			int _ref_passedTime = getTimerValue(_ref_lastReq);

			stopTimer(_ref_lastReq);
			if (((_ref_passedTime<_ref_NREQ)&(!_ref_theFirsttime))) {
				{
					(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 62"), temp0)->_timed_msg_fastRequestFailure(myID, _ref_now, MAX_TIME);
				}
}
			else {
				{
					startTimer(_ref_lastReq);
					(temp1=_ref_cs, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 67"), temp1)->_timed_msg_request(myID, (_ref_Lm+_ref_RR1Delay), _ref_RR1Delay + _ref_now, MAX_TIME);
				}
}
			(_ref_theFirsttime=false);
		}
		return 0;

}
	    long RequestRequesterActor::msgsrvresponse (
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;ClientActor *temp1;ClientActor *temp2;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "response";
		#endif

	#ifdef TTS
	#endif

		{
			boolean _ref_canConsume = true;

			int _ref_RR2Delay;

			(_ref_RR2Delay=2);
			(_ref_Lm=(_ref_Lm+_ref_RR2Delay));
			int _ref_passedTime = getTimerValue(_ref_lastReq);

			stopTimer(_ref_lastReq);
			if (((_ref_life>0)&&(_ref_life<_ref_RREQ))) {
				{
					(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 80"), temp0)->_timed_msg_staleData(myID, _ref_now, MAX_TIME);
					(_ref_canConsume=false);
				}
}
			if ((_ref_passedTime>=_ref_LREQ)) {
				{
					(temp1=_ref_c, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 84"), temp1)->_timed_msg_timeOutFailure(myID, _ref_now, MAX_TIME);
				}
}
			else {
				if ((_ref_canConsume&&(_ref_RR2Delay<_ref_LREQ))) {
					{
						(temp2=_ref_c, assertion(temp2!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 87"), temp2)->_timed_msg_response(myID, _ref_Lm, _ref_RR2Delay + _ref_now, MAX_TIME);
					}
}}
		}
		return 0;

}
	    long RequestRequesterActor::msgsrvexcessLoadFailure (
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
			(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 92"), temp0)->_timed_msg_excessLoadFailure(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long RequestRequesterActor::msgsrvtimeOutFailure (
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
			(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 95"), temp0)->_timed_msg_timeOutFailure(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long RequestRequesterActor::msgsrvdataUnavailableFailure (
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "dataUnavailableFailure";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 98"), temp0)->_timed_msg_dataUnavailableFailure(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
