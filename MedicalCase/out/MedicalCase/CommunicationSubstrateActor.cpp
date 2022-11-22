                                    


#include "Types.h"
#include "CommunicationSubstrateActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "BaseActor.h"
#include "ClientActor.h"
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
#define PUBLISH 2
#define REQUEST 3
#define RESPONSE 4
#define EXCESSLOADFAILURE 5
#define TIMEOUTFAILURE 6
#define DATAUNAVAILABLEFAILURE 7


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
    messageNames = new char *[6 + 2];
    messagePriorities = new int[6 + 2];
	messageNames[0] = (char*)"Empty";

	
		
	
		
		
       		 messageNames[1] = (char*)"BUSY";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"PUBLISH";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"REQUEST";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"RESPONSE";
			messagePriorities[4] = std::numeric_limits<int>::max();
       		 messageNames[5] = (char*)"EXCESSLOADFAILURE";
			messagePriorities[5] = std::numeric_limits<int>::max();
       		 messageNames[6] = (char*)"TIMEOUTFAILURE";
			messagePriorities[6] = std::numeric_limits<int>::max();
       		 messageNames[7] = (char*)"DATAUNAVAILABLEFAILURE";
			messagePriorities[7] = std::numeric_limits<int>::max();








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

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_life;
            
            
                	memcpy(&_ref_life, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_subscriberId;
            
            
                	memcpy(&_ref_subscriberId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "publish("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_topic)
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_life)
					<< ", "
                	<< ((int)_ref_subscriberId)
                << ")";
                break;
               }
            case REQUEST: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderInvokerId;
            
            
                	memcpy(&_ref_responderInvokerId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "request("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_responderInvokerId)
                << ")";
                break;
               }
            case RESPONSE: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_life;
            
            
                	memcpy(&_ref_life, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_requesterId;
            
            
                	memcpy(&_ref_requesterId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "response("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_life)
					<< ", "
                	<< ((int)_ref_requesterId)
                << ")";
                break;
               }
            case EXCESSLOADFAILURE: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_requesterId;
            
            
                	memcpy(&_ref_requesterId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "excessLoadFailure("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_requesterId)
                << ")";
                break;
               }
            case TIMEOUTFAILURE: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_requesterId;
            
            
                	memcpy(&_ref_requesterId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "timeOutFailure("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_requesterId)
                << ")";
                break;
               }
            case DATAUNAVAILABLEFAILURE: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_requesterId;
            
            
                	memcpy(&_ref_requesterId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "dataUnavailableFailure("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_requesterId)
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

            case PUBLISH:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_life;
            
            
                	memcpy(&_ref_life, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_subscriberId;
            
            
                	memcpy(&_ref_subscriberId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvpublish(
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
	        , 
            _ref_life
	        , 
            _ref_subscriberId
                    );
                }
            case REQUEST:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderInvokerId;
            
            
                	memcpy(&_ref_responderInvokerId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvrequest(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderInvokerId
                    );
                }
            case RESPONSE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_life;
            
            
                	memcpy(&_ref_life, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_requesterId;
            
            
                	memcpy(&_ref_requesterId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvresponse(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_life
	        , 
            _ref_requesterId
                    );
                }
            case EXCESSLOADFAILURE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_requesterId;
            
            
                	memcpy(&_ref_requesterId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvexcessLoadFailure(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_requesterId
                    );
                }
            case TIMEOUTFAILURE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_requesterId;
            
            
                	memcpy(&_ref_requesterId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvtimeOutFailure(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_requesterId
                    );
                }
            case DATAUNAVAILABLEFAILURE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_requesterId;
            
            
                	memcpy(&_ref_requesterId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvdataUnavailableFailure(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_requesterId
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



		void CommunicationSubstrateActor::_msg_publish (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
	        , 
    		int
	         _ref_subscriberId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, PUBLISH);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_topic, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_life, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_subscriberId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationSubstrateActor::_msg_request (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderInvokerId
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
            
                	memcpy(&paramsValues [index], &_ref_responderInvokerId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationSubstrateActor::_msg_response (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
	        , 
    		int
	         _ref_requesterId
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
            
                	memcpy(&paramsValues [index], &_ref_requesterId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationSubstrateActor::_msg_excessLoadFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, EXCESSLOADFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_requesterId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationSubstrateActor::_msg_timeOutFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, TIMEOUTFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_requesterId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void CommunicationSubstrateActor::_msg_dataUnavailableFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, DATAUNAVAILABLEFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_requesterId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
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
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
	        , 
    		int
	         _ref_subscriberId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_publish (senderId
	        ,
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
	        , 
            _ref_life
	        , 
            _ref_subscriberId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationSubstrateActor::_timed_msg_request (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderInvokerId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_request (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderInvokerId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationSubstrateActor::_timed_msg_response (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
	        , 
    		int
	         _ref_requesterId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_response (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_life
	        , 
            _ref_requesterId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationSubstrateActor::_timed_msg_excessLoadFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_excessLoadFailure (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_requesterId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationSubstrateActor::_timed_msg_timeOutFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_timeOutFailure (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_requesterId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void CommunicationSubstrateActor::_timed_msg_dataUnavailableFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_dataUnavailableFailure (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_requesterId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void CommunicationSubstrateActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long CommunicationSubstrateActor::msgsrvpublish (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
	        , 
    		int
	         _ref_subscriberId
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

			SubscribeInvokerActor* _ref_si = (dynamic_cast<SubscribeInvokerActor*>(_synchmethod_find(_ref_subscriberId)));

			(temp0=_ref_si, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 51"), temp0)->_timed_msg_publish(myID, _ref_data, _ref_topic, (_ref_Lm+_ref_csDelay), (_ref_life-_ref_csDelay), _ref_csDelay + _ref_now, MAX_TIME);
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
	    long CommunicationSubstrateActor::msgsrvrequest (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderInvokerId
    ) {
    			long arrayIndexChecker = 0;ResponderInvokerActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "request";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 1;

	#ifdef TTS
	#endif

		{
			int _ref_cs1Delay = (__tNumber|=1, nonDetVariable0);

			ResponderInvokerActor* _ref_ri = (dynamic_cast<ResponderInvokerActor*>(_synchmethod_find(_ref_responderInvokerId)));

			(temp0=_ref_ri, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 57"), temp0)->_timed_msg_request(myID, _ref_data, (_ref_Lm+_ref_cs1Delay), _ref_cs1Delay + _ref_now, MAX_TIME);
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
	    long CommunicationSubstrateActor::msgsrvresponse (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
	        , 
    		int
	         _ref_requesterId
    ) {
    			long arrayIndexChecker = 0;RequestRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "response";
		#endif
		long __tNumber = 0;
		static byte nonDetVariable0 = 1;

	#ifdef TTS
	#endif

		{
			int _ref_cs2Delay = (__tNumber|=1, nonDetVariable0);

			RequestRequesterActor* _ref_rr = (dynamic_cast<RequestRequesterActor*>(_synchmethod_find(_ref_requesterId)));

			(temp0=_ref_rr, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 63"), temp0)->_timed_msg_response(myID, _ref_data, (_ref_Lm+_ref_cs2Delay), (_ref_life-_ref_cs2Delay), _ref_cs2Delay + _ref_now, MAX_TIME);
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
	    long CommunicationSubstrateActor::msgsrvexcessLoadFailure (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
    ) {
    			long arrayIndexChecker = 0;RequestRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "excessLoadFailure";
		#endif

	#ifdef TTS
	#endif

		{
			RequestRequesterActor* _ref_rr = (dynamic_cast<RequestRequesterActor*>(_synchmethod_find(_ref_requesterId)));

			(temp0=_ref_rr, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 67"), temp0)->_timed_msg_excessLoadFailure(myID, _ref_data, _ref_Lm, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long CommunicationSubstrateActor::msgsrvtimeOutFailure (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
    ) {
    			long arrayIndexChecker = 0;RequestRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "timeOutFailure";
		#endif

	#ifdef TTS
	#endif

		{
			RequestRequesterActor* _ref_rr = (dynamic_cast<RequestRequesterActor*>(_synchmethod_find(_ref_requesterId)));

			(temp0=_ref_rr, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 71"), temp0)->_timed_msg_timeOutFailure(myID, _ref_data, _ref_Lm, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long CommunicationSubstrateActor::msgsrvdataUnavailableFailure (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
    ) {
    			long arrayIndexChecker = 0;RequestRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "dataUnavailableFailure";
		#endif

	#ifdef TTS
	#endif

		{
			RequestRequesterActor* _ref_rr = (dynamic_cast<RequestRequesterActor*>(_synchmethod_find(_ref_requesterId)));

			(temp0=_ref_rr, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 75"), temp0)->_timed_msg_dataUnavailableFailure(myID, _ref_data, _ref_Lm, _ref_now, MAX_TIME);
		}
		return 0;

}
