                                    


#include "Types.h"
#include "ServiceActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "ClientActor.h"
#include "PublishRequesterActor.h"
#include "CommunicationSubstrateActor.h"
#include "SubscribeInvokerActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define PUBLISH 1
#define BUSY 2
#define SLOWCONSUMPTION 3
#define STALEDATA 4
#define SLOWPUBLICATION 5


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<ServiceActor*>(rebecs[myID]))
    	#define _ref_si (dynamic_cast<SubscribeInvokerActor*>(rebecs[known[1]]))
    
ServiceActor::ServiceActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
       		 messageNames[3] = (char*)"SLOWCONSUMPTION";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"STALEDATA";
			messagePriorities[4] = std::numeric_limits<int>::max();
       		 messageNames[5] = (char*)"SLOWPUBLICATION";
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

ServiceActor::~ServiceActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void ServiceActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case PUBLISH: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "publish("
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case BUSY: {
                out << "busy("
                << ")";
                break;
               }
            case SLOWCONSUMPTION: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "slowConsumption("
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case STALEDATA: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "staleData("
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case SLOWPUBLICATION: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "slowPublication("
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
    	}
}


void ServiceActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void ServiceActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"Service.transmitionTime\" type=\"int\">" <<  
			    		((int)_ref_transmitionTime) << "</variable>" << endl;
}


char* ServiceActor::getClassName() {
    return (char*)"Service";
}
void ServiceActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_transmitionTime, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

    
	
}
void ServiceActor::marshalActorToArray(byte* array) {
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

void ServiceActor::unmarshalActorFromArray(byte* array) {

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

void ServiceActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_transmitionTime, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
    
	
}

/*#ifdef DEBUG
void ServiceActor::printStateContent(long stateNo) {
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
long ServiceActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case PUBLISH:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvpublish(
            _ref_Lm
                    );
                }
            case BUSY:
                {
                    int i = 0;
                    return msgsrvbusy(
                    );
                }
            case SLOWCONSUMPTION:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvslowConsumption(
            _ref_Lm
                    );
                }
            case STALEDATA:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvstaleData(
            _ref_Lm
                    );
                }
            case SLOWPUBLICATION:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvslowPublication(
            _ref_Lm
                    );
                }
    }
	return -1;
}


    long ServiceActor::constructorService (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Service";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_transmitionTime=0);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void ServiceActor::_msg_publish (byte senderId
	        ,
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, PUBLISH);
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

		void ServiceActor::_msg_busy (byte senderId
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

		void ServiceActor::_msg_slowConsumption (byte senderId
	        ,
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, SLOWCONSUMPTION);
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

		void ServiceActor::_msg_staleData (byte senderId
	        ,
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, STALEDATA);
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

		void ServiceActor::_msg_slowPublication (byte senderId
	        ,
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, SLOWPUBLICATION);
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









	byte** ServiceActor::table = (byte**)0xFFFFFFFF;

	byte** ServiceActor::getTable() {
		return table;
	}








	vector<TimeFrame>** ServiceActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** ServiceActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void ServiceActor::_timed_msg_publish (byte senderId
	        ,
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_publish (senderId
	        ,
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ServiceActor::_timed_msg_busy (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_busy (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ServiceActor::_timed_msg_slowConsumption (byte senderId
	        ,
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_slowConsumption (senderId
	        ,
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ServiceActor::_timed_msg_staleData (byte senderId
	        ,
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_staleData (senderId
	        ,
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void ServiceActor::_timed_msg_slowPublication (byte senderId
	        ,
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_slowPublication (senderId
	        ,
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void ServiceActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long ServiceActor::msgsrvpublish (
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;SubscribeInvokerActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "publish";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 1;

	#ifdef TTS
		if (__pc == 0) {__pc = -1; goto __jumpPoint0;}
	#endif

		{
			int _ref_consumeDelay = (__tNumber|=1, nonDetVariable0);

_ref_now += _ref_consumeDelay;
			(_ref_transmitionTime=_ref_Lm);
			(temp0=_ref_si, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 158"), temp0)->_timed_msg_consumed(myID, (_ref_Lm+_ref_consumeDelay), _ref_now, MAX_TIME);
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
	    long ServiceActor::msgsrvbusy (
    ) {
    			long arrayIndexChecker = 0;ServiceActor *temp0;
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
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 162"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long ServiceActor::msgsrvslowConsumption (
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;ServiceActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "slowConsumption";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 164"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long ServiceActor::msgsrvstaleData (
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;ServiceActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "staleData";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 165"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long ServiceActor::msgsrvslowPublication (
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;ServiceActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "slowPublication";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 166"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
