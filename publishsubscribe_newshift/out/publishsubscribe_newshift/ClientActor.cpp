                                    


#include "Types.h"
#include "ClientActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "PublishRequesterActor.h"
#include "CommunicationSubstrateActor.h"
#include "SubscribeInvokerActor.h"
#include "ServiceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define PUBLISH 1
#define BUSY 2
#define ACCEPTED 3
#define FASTPUBLICATIONFAILURE 4
#define TIMEOUTFAILURE 5


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<ClientActor*>(rebecs[myID]))
    	#define _ref_PR (dynamic_cast<PublishRequesterActor*>(rebecs[known[1]]))
    
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

	
		
	
		
       		 messageNames[1] = (char*)"PUBLISH";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"BUSY";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"ACCEPTED";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"FASTPUBLICATIONFAILURE";
			messagePriorities[4] = std::numeric_limits<int>::max();
       		 messageNames[5] = (char*)"TIMEOUTFAILURE";
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

ClientActor::~ClientActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void ClientActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case PUBLISH: {
                out << "publish("
                << ")";
                break;
               }
            case BUSY: {
                out << "busy("
                << ")";
                break;
               }
            case ACCEPTED: {
                out << "accepted("
                << ")";
                break;
               }
            case FASTPUBLICATIONFAILURE: {
                out << "fastPublicationFailure("
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


void ClientActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void ClientActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


}


char* ClientActor::getClassName() {
    return (char*)"Client";
}
void ClientActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    
	
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

            case PUBLISH:
                {
                    int i = 0;
                    return msgsrvpublish(
                    );
                }
            case BUSY:
                {
                    int i = 0;
                    return msgsrvbusy(
                    );
                }
            case ACCEPTED:
                {
                    int i = 0;
                    return msgsrvaccepted(
                    );
                }
            case FASTPUBLICATIONFAILURE:
                {
                    int i = 0;
                    return msgsrvfastPublicationFailure(
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


    long ClientActor::constructorClient (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Client";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 16"), temp0)->_timed_msg_publish(myID, 3 + _ref_now, MAX_TIME);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void ClientActor::_msg_publish (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, PUBLISH);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void ClientActor::_msg_busy (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, BUSY);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void ClientActor::_msg_accepted (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, ACCEPTED);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void ClientActor::_msg_fastPublicationFailure (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, FASTPUBLICATIONFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void ClientActor::_msg_timeOutFailure (byte senderId
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









	byte** ClientActor::table = (byte**)0xFFFFFFFF;

	byte** ClientActor::getTable() {
		return table;
	}








	vector<TimeFrame>** ClientActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** ClientActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void ClientActor::_timed_msg_publish (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_publish (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ClientActor::_timed_msg_busy (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_busy (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ClientActor::_timed_msg_accepted (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_accepted (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ClientActor::_timed_msg_fastPublicationFailure (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_fastPublicationFailure (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ClientActor::_timed_msg_timeOutFailure (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_timeOutFailure (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void ClientActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long ClientActor::msgsrvpublish (
    ) {
    			long arrayIndexChecker = 0;PublishRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "publish";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 3;

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
			int _ref_clientDelay;

			(_ref_clientDelay=(__tNumber|=1, nonDetVariable0));
_ref_now += _ref_clientDelay;
			int _ref_life = 15;

			(temp0=_ref_PR, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 23"), temp0)->_timed_msg_publish(myID, (_ref_clientDelay+3), _ref_life, _ref_now, MAX_TIME);
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == 3) {
				nonDetVariable0 = 4;
			}
			else if(nonDetVariable0 == 4) {
				nonDetVariable0 = 3;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}
	    long ClientActor::msgsrvbusy (
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "busy";
		#endif

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
_ref_now += 1;
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 27"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long ClientActor::msgsrvaccepted (
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
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 29"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long ClientActor::msgsrvfastPublicationFailure (
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
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 30"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long ClientActor::msgsrvtimeOutFailure (
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
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 31"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
