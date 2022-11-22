                                    


#include "Types.h"
#include "CommunicationSubstrateActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "ClientActor.h"
#include "PublishRequesterActor.h"
#include "SubscribeInvokerActor.h"
#include "ServiceActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define PUBLISH 1


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<CommunicationSubstrateActor*>(rebecs[myID]))
    	#define _ref_si (dynamic_cast<SubscribeInvokerActor*>(rebecs[known[1]]))
    
CommunicationSubstrateActor::CommunicationSubstrateActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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

	
		
	
		
       		 messageNames[1] = (char*)"PUBLISH";
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

CommunicationSubstrateActor::~CommunicationSubstrateActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void CommunicationSubstrateActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case PUBLISH: {

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_life;
            
            
                	memcpy(&_ref_life, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "publish("
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_life)
                << ")";
                break;
               }
    	}
}


void CommunicationSubstrateActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void CommunicationSubstrateActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


}


char* CommunicationSubstrateActor::getClassName() {
    return (char*)"CommunicationSubstrate";
}
void CommunicationSubstrateActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    
	
}
void CommunicationSubstrateActor::marshalActorToArray(byte* array) {
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

void CommunicationSubstrateActor::unmarshalActorFromArray(byte* array) {

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

void CommunicationSubstrateActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;
    
	
}

/*#ifdef DEBUG
void CommunicationSubstrateActor::printStateContent(long stateNo) {
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
long CommunicationSubstrateActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case PUBLISH:
                {
                    int i = 0;

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_life;
            
            
                	memcpy(&_ref_life, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvpublish(
            _ref_Lm
	        , 
            _ref_life
                    );
                }
    }
	return -1;
}


    long CommunicationSubstrateActor::constructorCommunicationSubstrate (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "CommunicationSubstrate";
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



		void CommunicationSubstrateActor::_msg_publish (byte senderId
	        ,
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
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
            
                	memcpy(&paramsValues [index], &_ref_life, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** CommunicationSubstrateActor::table = (byte**)0xFFFFFFFF;

	byte** CommunicationSubstrateActor::getTable() {
		return table;
	}








	vector<TimeFrame>** CommunicationSubstrateActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** CommunicationSubstrateActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void CommunicationSubstrateActor::_timed_msg_publish (byte senderId
	        ,
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_publish (senderId
	        ,
            _ref_Lm
	        , 
            _ref_life
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void CommunicationSubstrateActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long CommunicationSubstrateActor::msgsrvpublish (
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
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
	#endif

		{
			int _ref_csDelay = (__tNumber|=1, nonDetVariable0);

			(temp0=_ref_si, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 85"), temp0)->_timed_msg_publish(myID, (_ref_Lm+_ref_csDelay), (_ref_life-_ref_csDelay), _ref_csDelay + _ref_now, MAX_TIME);
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
