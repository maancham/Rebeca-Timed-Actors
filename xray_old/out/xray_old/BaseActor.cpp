                                    


#include "Types.h"
#include "BaseActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "ServiceActor.h"
#include "SupervisorActor.h"
#include "InitiateRequesterActor.h"
#include "CommunicationSubstrateActor.h"
#include "ExecuteInvokerActor.h"
#include "VentilatorActor.h"
#include "X_RayActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define BUSY 1


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<BaseActor*>(rebecs[myID]))
    
BaseActor::BaseActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[1 + 1];
    messagePriorities = new int[1 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"BUSY";
			messagePriorities[1] = std::numeric_limits<int>::max();








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

BaseActor::~BaseActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void BaseActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case BUSY: {
                out << "busy("
                << ")";
                break;
               }
    	}
}


void BaseActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void BaseActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"Base.id\" type=\"int\">" <<  
			    		((int)_ref_id) << "</variable>" << endl;
}


char* BaseActor::getClassName() {
    return (char*)"Base";
}
void BaseActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_id, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

    
	
}
void BaseActor::marshalActorToArray(byte* array) {
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

void BaseActor::unmarshalActorFromArray(byte* array) {

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

void BaseActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_id, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
    
	
}

/*#ifdef DEBUG
void BaseActor::printStateContent(long stateNo) {
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
long BaseActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case BUSY:
                {
                    int i = 0;
                    return msgsrvbusy(
                    );
                }
    }
	return -1;
}


    long BaseActor::constructorBase (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Base";
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

 
		BaseActor* BaseActor::_synchmethod_find (
    		int
	         _ref__id
   		 ) {
    			long arrayIndexChecker = 0;BaseActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "find";
		#endif

	#ifdef TTS
	#endif

		{
			ArrayList<AbstractActor*> _ref_allActors = getAllActors();

			for (int _ref_i = 0;
((_ref_i<_ref_allActors.size())); (_ref_i++)) {
				{
					BaseActor* _ref_actor = (dynamic_cast<BaseActor*>(_ref_allActors.operator[](_ref_i)));

					if (((temp0=_ref_actor, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 23"), temp0)->_ref_id==_ref__id)) {
						{
							return _ref_actor;

						}
}
				}
}
		}

		}


		void BaseActor::_msg_busy (byte senderId
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









	byte** BaseActor::table = (byte**)0xFFFFFFFF;

	byte** BaseActor::getTable() {
		return table;
	}








	vector<TimeFrame>** BaseActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** BaseActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void BaseActor::_timed_msg_busy (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_busy (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void BaseActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long BaseActor::msgsrvbusy (
    ) {
    			long arrayIndexChecker = 0;BaseActor *temp0;
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
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 30"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
