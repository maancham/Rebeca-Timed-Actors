                                    


#include "Types.h"
#include "ClientActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "BaseActor.h"
#include "CommunicationSubstrateActor.h"
#include "CapnometerActor.h"
#include "OximeterActor.h"
#include "MonitoringAppActor.h"
#include "PumpActor.h"
#include "PublishRequesterActor.h"
#include "SubscribeInvokerActor.h"
#include "RequestRequesterActor.h"
#include "ResponderInvokerActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define ACCEPTED 2
#define FASTPUBLICATIONFAILURE 3
#define TIMEOUTFAILURE 4


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<ClientActor*>(rebecs[myID]))
    
ClientActor::ClientActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
		,
		BaseActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
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
		delete [] messageNames;
    messageNames = new char *[3 + 2];
    messagePriorities = new int[3 + 2];
	messageNames[0] = (char*)"Empty";

	
		
	
		
		
       		 messageNames[1] = (char*)"BUSY";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"ACCEPTED";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"FASTPUBLICATIONFAILURE";
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

ClientActor::~ClientActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void ClientActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case ACCEPTED: {

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "accepted("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_topic)
					<< ", "
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case FASTPUBLICATIONFAILURE: {

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "fastPublicationFailure("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_topic)
					<< ", "
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case TIMEOUTFAILURE: {

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "timeOutFailure("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_topic)
					<< ", "
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
        		default:
				BaseActor::exportAMessageInXML(out, tab, cnt);
    	}
}


void ClientActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void ClientActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){

		BaseActor::exportInsideStateVariablesTagInXML(out, tab);

}


char* ClientActor::getClassName() {
    return (char*)"Client";
}
void ClientActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    
		BaseActor::marshalVariablesToArray( & array[marshalIndex]);

	
}
void ClientActor::marshalActorToArray(byte* array) {
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

void ClientActor::unmarshalActorFromArray(byte* array) {

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

void ClientActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;
    
		BaseActor::unmarshalVariablesFromArray( & array[index]);

	
}

/*#ifdef DEBUG
void ClientActor::printStateContent(long stateNo) {
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
long ClientActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case ACCEPTED:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvaccepted(
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
                    );
                }
            case FASTPUBLICATIONFAILURE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvfastPublicationFailure(
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
                    );
                }
            case TIMEOUTFAILURE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvtimeOutFailure(
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
                    );
                }
    }
    return BaseActor::methodSelectorAndExecutor();   // if queue empty, we have no transitions
}


    long ClientActor::constructorClient (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Client";
		#endif

	#ifdef TTS
	#endif

		{
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void ClientActor::_msg_accepted (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, ACCEPTED);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_topic, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void ClientActor::_msg_fastPublicationFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, FASTPUBLICATIONFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_topic, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void ClientActor::_msg_timeOutFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, TIMEOUTFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_topic, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** ClientActor::table = (byte**)0xFFFFFFFF;

	byte** ClientActor::getTable() {
		return table;
	}








	vector<TimeFrame>** ClientActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** ClientActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void ClientActor::_timed_msg_accepted (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_accepted (senderId
	        ,
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ClientActor::_timed_msg_fastPublicationFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_fastPublicationFailure (senderId
	        ,
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ClientActor::_timed_msg_timeOutFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_timeOutFailure (senderId
	        ,
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void ClientActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long ClientActor::msgsrvaccepted (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "accepted";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 80"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long ClientActor::msgsrvfastPublicationFailure (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "fastPublicationFailure";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 81"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long ClientActor::msgsrvtimeOutFailure (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
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
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 82"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
