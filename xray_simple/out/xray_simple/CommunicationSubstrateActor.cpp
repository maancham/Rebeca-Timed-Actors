                                    


#include "Types.h"
#include "CommunicationSubstrateActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "BaseActor.h"
#include "ServiceActor.h"
#include "SupervisorActor.h"
#include "InitiateRequesterActor.h"
#include "ExecuteInvokerActor.h"
#include "VentilatorActor.h"
#include "X_RayActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define INITIATE 2
#define ACK 3
#define EXCESSLOADFAILURE 4
#define TIMEOUTFAILURE 5


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<CommunicationSubstrateActor*>(rebecs[myID]))
    
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
    messageNames = new char *[4 + 2];
    messagePriorities = new int[4 + 2];
	messageNames[0] = (char*)"Empty";

	
		
	
		
		
       		 messageNames[1] = (char*)"BUSY";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"INITIATE";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"ACK";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"EXCESSLOADFAILURE";
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

CommunicationSubstrateActor::~CommunicationSubstrateActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void CommunicationSubstrateActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case INITIATE: {

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_invokerId;
            
            
                	memcpy(&_ref_invokerId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "initiate("
                	<< ((int)_ref_action)
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_invokerId)
                << ")";
                break;
               }
            case ACK: {

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_actionStatus;
            
            
                	memcpy(&_ref_actionStatus, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_initiatorId;
            
            
                	memcpy(&_ref_initiatorId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "ack("
                	<< ((int)_ref_action)
					<< ", "
                	<< ((int)_ref_actionStatus)
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_initiatorId)
                << ")";
                break;
               }
            case EXCESSLOADFAILURE: {

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_initiatorId;
            
            
                	memcpy(&_ref_initiatorId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "excessLoadFailure("
                	<< ((int)_ref_action)
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_initiatorId)
                << ")";
                break;
               }
            case TIMEOUTFAILURE: {

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_actionStatus;
            
            
                	memcpy(&_ref_actionStatus, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_initiatorId;
            
            
                	memcpy(&_ref_initiatorId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "timeOutFailure("
                	<< ((int)_ref_action)
					<< ", "
                	<< ((int)_ref_actionStatus)
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_initiatorId)
                << ")";
                break;
               }
        		default:
				BaseActor::exportAMessageInXML(out, tab, cnt);
    	}
}


void CommunicationSubstrateActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void CommunicationSubstrateActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){

		BaseActor::exportInsideStateVariablesTagInXML(out, tab);

}


char* CommunicationSubstrateActor::getClassName() {
    return (char*)"CommunicationSubstrate";
}
void CommunicationSubstrateActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    
		BaseActor::marshalVariablesToArray( & array[marshalIndex]);

	
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
    
		BaseActor::unmarshalVariablesFromArray( & array[index]);

	
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

            case INITIATE:
                {
                    int i = 0;

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_invokerId;
            
            
                	memcpy(&_ref_invokerId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvinitiate(
            _ref_action
	        , 
            _ref_Lm
	        , 
            _ref_invokerId
                    );
                }
            case ACK:
                {
                    int i = 0;

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_actionStatus;
            
            
                	memcpy(&_ref_actionStatus, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_initiatorId;
            
            
                	memcpy(&_ref_initiatorId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvack(
            _ref_action
	        , 
            _ref_actionStatus
	        , 
            _ref_Lm
	        , 
            _ref_initiatorId
                    );
                }
            case EXCESSLOADFAILURE:
                {
                    int i = 0;

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_initiatorId;
            
            
                	memcpy(&_ref_initiatorId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvexcessLoadFailure(
            _ref_action
	        , 
            _ref_Lm
	        , 
            _ref_initiatorId
                    );
                }
            case TIMEOUTFAILURE:
                {
                    int i = 0;

            	int _ref_action;
            
            
                	memcpy(&_ref_action, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_actionStatus;
            
            
                	memcpy(&_ref_actionStatus, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_initiatorId;
            
            
                	memcpy(&_ref_initiatorId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvtimeOutFailure(
            _ref_action
	        , 
            _ref_actionStatus
	        , 
            _ref_Lm
	        , 
            _ref_initiatorId
                    );
                }
    }
    return BaseActor::methodSelectorAndExecutor();   // if queue empty, we have no transitions
}


    long CommunicationSubstrateActor::constructorCommunicationSubstrate (byte creatorId 
	        ,
    		int
	         _ref__id
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
			(_ref_id=_ref__id);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void CommunicationSubstrateActor::_msg_initiate (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_invokerId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, INITIATE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_action, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_invokerId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationSubstrateActor::_msg_ack (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_actionStatus
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_initiatorId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, ACK);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_action, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_actionStatus, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_initiatorId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationSubstrateActor::_msg_excessLoadFailure (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_initiatorId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, EXCESSLOADFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_action, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_initiatorId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationSubstrateActor::_msg_timeOutFailure (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_actionStatus
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_initiatorId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, TIMEOUTFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_action, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_actionStatus, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_initiatorId, (1 * INT_SIZE));
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








	    void CommunicationSubstrateActor::_timed_msg_initiate (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_invokerId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_initiate (senderId
	        ,
            _ref_action
	        , 
            _ref_Lm
	        , 
            _ref_invokerId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationSubstrateActor::_timed_msg_ack (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_actionStatus
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_initiatorId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_ack (senderId
	        ,
            _ref_action
	        , 
            _ref_actionStatus
	        , 
            _ref_Lm
	        , 
            _ref_initiatorId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationSubstrateActor::_timed_msg_excessLoadFailure (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_initiatorId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_excessLoadFailure (senderId
	        ,
            _ref_action
	        , 
            _ref_Lm
	        , 
            _ref_initiatorId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationSubstrateActor::_timed_msg_timeOutFailure (byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_actionStatus
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_initiatorId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_timeOutFailure (senderId
	        ,
            _ref_action
	        , 
            _ref_actionStatus
	        , 
            _ref_Lm
	        , 
            _ref_initiatorId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void CommunicationSubstrateActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long CommunicationSubstrateActor::msgsrvinitiate (
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_invokerId
    ) {
    			long arrayIndexChecker = 0;ExecuteInvokerActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "initiate";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 2;

	#ifdef TTS
	#endif

		{
			int _ref_csDelay = (__tNumber|=1, nonDetVariable0);

			ExecuteInvokerActor* _ref_ei = (dynamic_cast<ExecuteInvokerActor*>(_synchmethod_find(_ref_invokerId)));

			(temp0=_ref_ei, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 142"), temp0)->_timed_msg_initiate(myID, _ref_action, (_ref_Lm+_ref_csDelay), _ref_csDelay + _ref_now, MAX_TIME);
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == 2) {
				nonDetVariable0 = 3;
			}
			else if(nonDetVariable0 == 3) {
				nonDetVariable0 = 4;
			}
			else if(nonDetVariable0 == 4) {
				nonDetVariable0 = 2;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}
	    long CommunicationSubstrateActor::msgsrvack (
    		int
	         _ref_action
	        , 
    		int
	         _ref_actionStatus
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_initiatorId
    ) {
    			long arrayIndexChecker = 0;InitiateRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "ack";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 3;

	#ifdef TTS
	#endif

		{
			int _ref_cs2Delay = (__tNumber|=1, nonDetVariable0);

			InitiateRequesterActor* _ref_ir = (dynamic_cast<InitiateRequesterActor*>(_synchmethod_find(_ref_initiatorId)));

			(temp0=_ref_ir, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 147"), temp0)->_timed_msg_ack(myID, _ref_action, _ref_actionStatus, (_ref_Lm+_ref_cs2Delay), _ref_cs2Delay + _ref_now, MAX_TIME);
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == 3) {
				nonDetVariable0 = 4;
			}
			else if(nonDetVariable0 == 4) {
				nonDetVariable0 = 5;
			}
			else if(nonDetVariable0 == 5) {
				nonDetVariable0 = 3;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}
	    long CommunicationSubstrateActor::msgsrvexcessLoadFailure (
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_initiatorId
    ) {
    			long arrayIndexChecker = 0;InitiateRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "excessLoadFailure";
		#endif

	#ifdef TTS
	#endif

		{
			InitiateRequesterActor* _ref_ir = (dynamic_cast<InitiateRequesterActor*>(_synchmethod_find(_ref_initiatorId)));

			(temp0=_ref_ir, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 151"), temp0)->_timed_msg_excessLoadFailure(myID, _ref_action, _ref_Lm, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long CommunicationSubstrateActor::msgsrvtimeOutFailure (
    		int
	         _ref_action
	        , 
    		int
	         _ref_actionStatus
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_initiatorId
    ) {
    			long arrayIndexChecker = 0;InitiateRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "timeOutFailure";
		#endif

	#ifdef TTS
	#endif

		{
			InitiateRequesterActor* _ref_ir = (dynamic_cast<InitiateRequesterActor*>(_synchmethod_find(_ref_initiatorId)));

			(temp0=_ref_ir, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 155"), temp0)->_timed_msg_timeOutFailure(myID, _ref_action, _ref_actionStatus, _ref_Lm, _ref_now, MAX_TIME);
		}
		return 0;

}
