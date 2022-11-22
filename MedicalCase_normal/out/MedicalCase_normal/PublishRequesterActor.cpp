                                    


#include "Types.h"
#include "PublishRequesterActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "BaseActor.h"
#include "CommunicationSubstrateActor.h"
#include "ClientActor.h"
#include "CapnometerActor.h"
#include "OximeterActor.h"
#include "MonitoringAppActor.h"
#include "PumpActor.h"
#include "SubscribeInvokerActor.h"
#include "RequestRequesterActor.h"
#include "ResponderInvokerActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define PUBLISH 2


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<PublishRequesterActor*>(rebecs[myID]))
    	#define _ref_cs (dynamic_cast<CommunicationSubstrateActor*>(rebecs[known[1]]))
    
PublishRequesterActor::PublishRequesterActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[1 + 2];
    messagePriorities = new int[1 + 2];
	messageNames[0] = (char*)"Empty";

	
		
	
		
		
       		 messageNames[1] = (char*)"BUSY";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"PUBLISH";
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

PublishRequesterActor::~PublishRequesterActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void PublishRequesterActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
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
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "publish("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_topic)
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_life)
                << ")";
                break;
               }
        		default:
				BaseActor::exportAMessageInXML(out, tab, cnt);
    	}
}


void PublishRequesterActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void PublishRequesterActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){

		BaseActor::exportInsideStateVariablesTagInXML(out, tab);

			    	out << tab << '\t' << "<variable name=\"PublishRequester.lastPub\" type=\"int\">" <<  
			    		((int)_ref_lastPub) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"PublishRequester.subscriberInvokerId\" type=\"int\">" <<  
			    		((int)_ref_subscriberInvokerId) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"PublishRequester.clientId\" type=\"int\">" <<  
			    		((int)_ref_clientId) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"PublishRequester.PR1Delay\" type=\"int\">" <<  
			    		((int)_ref_PR1Delay) << "</variable>" << endl;
}


char* PublishRequesterActor::getClassName() {
    return (char*)"PublishRequester";
}
void PublishRequesterActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_lastPub, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_subscriberInvokerId, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_clientId, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_PR1Delay, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

    
		BaseActor::marshalVariablesToArray( & array[marshalIndex]);

	
}
void PublishRequesterActor::marshalActorToArray(byte* array) {
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

void PublishRequesterActor::unmarshalActorFromArray(byte* array) {

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

void PublishRequesterActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_lastPub, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_subscriberInvokerId, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_clientId, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_PR1Delay, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
    
		BaseActor::unmarshalVariablesFromArray( & array[index]);

	
}

/*#ifdef DEBUG
void PublishRequesterActor::printStateContent(long stateNo) {
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
long PublishRequesterActor::methodSelectorAndExecutor() {

	
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
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvpublish(
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
	        , 
            _ref_life
                    );
                }
    }
    return BaseActor::methodSelectorAndExecutor();   // if queue empty, we have no transitions
}


    long PublishRequesterActor::constructorPublishRequester (byte creatorId 
	        ,
    		int
	         _ref__id
	        , 
    		int
	         _ref__clientId
	        , 
    		int
	         _ref__subscriberInvokerId
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "PublishRequester";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_lastPub=0);
			(_ref_id=_ref__id);
			(_ref_clientId=_ref__clientId);
			(_ref_subscriberInvokerId=_ref__subscriberInvokerId);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void PublishRequesterActor::_msg_publish (byte senderId
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

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** PublishRequesterActor::table = (byte**)0xFFFFFFFF;

	byte** PublishRequesterActor::getTable() {
		return table;
	}








	vector<TimeFrame>** PublishRequesterActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** PublishRequesterActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void PublishRequesterActor::_timed_msg_publish (byte senderId
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
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void PublishRequesterActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long PublishRequesterActor::msgsrvpublish (
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
    ) {
    			long arrayIndexChecker = 0;ClientActor *temp0;ClientActor *temp1;CommunicationSubstrateActor *temp2;ClientActor *temp3;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "publish";
		#endif

	#ifdef TTS
	#endif

		{
			int _ref_time = _ref_now;

			(_ref_PR1Delay=3);
			ClientActor* _ref_c = (dynamic_cast<ClientActor*>(_synchmethod_find(_ref_clientId)));

			if (((_ref_time-_ref_lastPub)<_ref_NPUB)) {
				{
					(temp0=_ref_c, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 240"), temp0)->_timed_msg_fastPublicationFailure(myID, _ref_data, _ref_topic, _ref_Lm, _ref_now, MAX_TIME);
				}
}
			else {
				if ((_ref_PR1Delay>_ref_LPUB)) {
					{
						(temp1=_ref_c, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 243"), temp1)->_timed_msg_timeOutFailure(myID, _ref_data, _ref_topic, _ref_Lm, _ref_now, MAX_TIME);
					}
}
				else {
					{
						(_ref_lastPub=_ref_now);
						(temp2=_ref_cs, assertion(temp2!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 248"), temp2)->_timed_msg_publish(myID, _ref_data, _ref_topic, (_ref_Lm+_ref_PR1Delay), (_ref_life-_ref_PR1Delay), _ref_subscriberInvokerId, _ref_PR1Delay + _ref_now, MAX_TIME);
						(temp3=_ref_c, assertion(temp3!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 249"), temp3)->_timed_msg_accepted(myID, _ref_data, _ref_topic, _ref_Lm, _ref_PR1Delay + _ref_now, MAX_TIME);
					}
}}
		}
		return 0;

}
