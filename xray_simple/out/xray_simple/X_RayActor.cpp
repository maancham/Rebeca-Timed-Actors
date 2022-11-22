                                    


#include "Types.h"
#include "X_RayActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "BaseActor.h"
#include "ServiceActor.h"
#include "SupervisorActor.h"
#include "InitiateRequesterActor.h"
#include "CommunicationSubstrateActor.h"
#include "ExecuteInvokerActor.h"
#include "VentilatorActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define EXECUTE 3


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<X_RayActor*>(rebecs[myID]))
    	#define _ref_EI_X_RAY (dynamic_cast<ExecuteInvokerActor*>(rebecs[known[1]]))
    
X_RayActor::X_RayActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














)
	:
		ServiceActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
			#ifdef DEBUG_LEVEL_2 
				, out
			#endif







		, stateSize, hashmapSize














		)
		,
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
	, BaseActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, out
		#endif







		, stateSize, hashmapSize














	)
{
		delete [] messageNames;
    messageNames = new char *[1 + 3];
    messagePriorities = new int[1 + 3];
	messageNames[0] = (char*)"Empty";

	
		
	
		
		
		
       		 messageNames[1] = (char*)"BUSY";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"EXECUTE";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"EXECUTE";
			messagePriorities[3] = std::numeric_limits<int>::max();








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

X_RayActor::~X_RayActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void X_RayActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case EXECUTE: {

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "execute("
                	<< ((int)_ref_action)
					<< ", "
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
        		default:
				ServiceActor::exportAMessageInXML(out, tab, cnt);
    	}
}


void X_RayActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void X_RayActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){

		ServiceActor::exportInsideStateVariablesTagInXML(out, tab);

}


char* X_RayActor::getClassName() {
    return (char*)"X_Ray";
}
void X_RayActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    
		ServiceActor::marshalVariablesToArray( & array[marshalIndex]);

	
}
void X_RayActor::marshalActorToArray(byte* array) {
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

void X_RayActor::unmarshalActorFromArray(byte* array) {

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

void X_RayActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;
    
		ServiceActor::unmarshalVariablesFromArray( & array[index]);

	
}

/*#ifdef DEBUG
void X_RayActor::printStateContent(long stateNo) {
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
long X_RayActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case EXECUTE:
                {
                    int i = 0;

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvexecute(
            _ref_action
	        , 
            _ref_Lm
                    );
                }
    }
    return ServiceActor::methodSelectorAndExecutor();   // if queue empty, we have no transitions
}


    long X_RayActor::constructorX_Ray (byte creatorId 
	        ,
    		int
	         _ref__id
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "X_Ray";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_id=_ref__id);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void X_RayActor::_msg_execute (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
  	  ){
        queueTail = 0;
        enqueue(messageQueue, EXECUTE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_action, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** X_RayActor::table = (byte**)0xFFFFFFFF;

	byte** X_RayActor::getTable() {
		return table;
	}








	vector<TimeFrame>** X_RayActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** X_RayActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void X_RayActor::_timed_msg_execute (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_execute (senderId
	        ,
            _ref_action
	        , 
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void X_RayActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long X_RayActor::msgsrvexecute (
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;ExecuteInvokerActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "execute";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 1;

	#ifdef TTS
	#endif

		{
			int _ref_exeDelay = (__tNumber|=1, nonDetVariable0);

			int _ref_actionStatus = 1;

			(temp0=_ref_EI_X_RAY, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 237"), temp0)->_timed_msg_ack(myID, _ref_action, _ref_actionStatus, _ref_Lm, _ref_exeDelay + _ref_now, MAX_TIME);
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == 1) {
				nonDetVariable0 = 2;
			}
			else if(nonDetVariable0 == 2) {
				nonDetVariable0 = 3;
			}
			else if(nonDetVariable0 == 3) {
				nonDetVariable0 = 1;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}
