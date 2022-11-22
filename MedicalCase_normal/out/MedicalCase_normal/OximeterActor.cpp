                                    


#include "Types.h"
#include "OximeterActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "BaseActor.h"
#include "CommunicationSubstrateActor.h"
#include "ClientActor.h"
#include "CapnometerActor.h"
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
#define PUBLISH 5


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<OximeterActor*>(rebecs[myID]))
    	#define _ref_PR_o (dynamic_cast<PublishRequesterActor*>(rebecs[known[1]]))
    
OximeterActor::OximeterActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
		ClientActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
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
    messageNames = new char *[1 + 5];
    messagePriorities = new int[1 + 5];
	messageNames[0] = (char*)"Empty";

	
		
	
		
		
		
       		 messageNames[1] = (char*)"BUSY";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"ACCEPTED";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"FASTPUBLICATIONFAILURE";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"TIMEOUTFAILURE";
			messagePriorities[4] = std::numeric_limits<int>::max();
       		 messageNames[5] = (char*)"PUBLISH";
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

OximeterActor::~OximeterActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void OximeterActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case PUBLISH: {

            	int _ref_afterDelay;
            
            
                	memcpy(&_ref_afterDelay, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "publish("
                	<< ((int)_ref_afterDelay)
                << ")";
                break;
               }
        		default:
				ClientActor::exportAMessageInXML(out, tab, cnt);
    	}
}


void OximeterActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void OximeterActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){

		ClientActor::exportInsideStateVariablesTagInXML(out, tab);

}


char* OximeterActor::getClassName() {
    return (char*)"Oximeter";
}
void OximeterActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    
		ClientActor::marshalVariablesToArray( & array[marshalIndex]);

	
}
void OximeterActor::marshalActorToArray(byte* array) {
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

void OximeterActor::unmarshalActorFromArray(byte* array) {

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

void OximeterActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;
    
		ClientActor::unmarshalVariablesFromArray( & array[index]);

	
}

/*#ifdef DEBUG
void OximeterActor::printStateContent(long stateNo) {
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
long OximeterActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case PUBLISH:
                {
                    int i = 0;

            	int _ref_afterDelay;
            
            
                	memcpy(&_ref_afterDelay, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvpublish(
            _ref_afterDelay
                    );
                }
    }
    return ClientActor::methodSelectorAndExecutor();   // if queue empty, we have no transitions
}


    long OximeterActor::constructorOximeter (byte creatorId 
	        ,
    		int
	         _ref__id
    ) {
    			long arrayIndexChecker = 0;OximeterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Oximeter";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_id=_ref__id);
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 117"), temp0)->_timed_msg_publish(myID, 3, _ref_now, MAX_TIME);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void OximeterActor::_msg_publish (byte senderId
	        ,
    		int
	         _ref_afterDelay
  	  ){
        queueTail = 0;
        enqueue(messageQueue, PUBLISH);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_afterDelay, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** OximeterActor::table = (byte**)0xFFFFFFFF;

	byte** OximeterActor::getTable() {
		return table;
	}








	vector<TimeFrame>** OximeterActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** OximeterActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void OximeterActor::_timed_msg_publish (byte senderId
	        ,
    		int
	         _ref_afterDelay
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_publish (senderId
	        ,
            _ref_afterDelay
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void OximeterActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long OximeterActor::msgsrvpublish (
    		int
	         _ref_afterDelay
    ) {
    			long arrayIndexChecker = 0;PublishRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "publish";
		#endif
		long __tNumber = 0;
		static boolean nonDetVariable0 = true;
		static boolean nonDetVariable1 = true;
		static byte nonDetVariable2 = 1;

	#ifdef TTS
	#endif

		{
			boolean _ref_spo2 = (__tNumber|=1, nonDetVariable0);

			boolean _ref_pulseRate = (__tNumber|=2, nonDetVariable1);

			int _ref_pubDelay = (__tNumber|=4, nonDetVariable2);


			int _ref_life = 60;

			(temp0=_ref_PR_o, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 127"), temp0)->_timed_msg_publish(myID, _ref_spo2, _ref_OX_SPO2, (_ref_pubDelay+_ref_afterDelay), _ref_life, _ref_pubDelay + _ref_now, MAX_TIME);
		}
		if (__tNumber & 4) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable2 == 1) {
				nonDetVariable2 = 2;
			}
			else if(nonDetVariable2 == 2) {
				nonDetVariable2 = 3;
			}
			else if(nonDetVariable2 == 3) {
				nonDetVariable2 = 1;
				__tNumber &= ~4;
			}
			if (__tNumber & 4) {
				return __tNumber;
			}
		}
		if (__tNumber & 2) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable1 == true) {
				nonDetVariable1 = false;
			}
			else if(nonDetVariable1 == false) {
				nonDetVariable1 = true;
				__tNumber &= ~2;
			}
			if (__tNumber & 2) {
				return __tNumber;
			}
		}
		if (__tNumber & 1) {
			if (false){/*Dummy 'if' statement to create nested else-if easily!*/}
			else if(nonDetVariable0 == true) {
				nonDetVariable0 = false;
			}
			else if(nonDetVariable0 == false) {
				nonDetVariable0 = true;
				__tNumber &= ~1;
			}
			if (__tNumber & 1) {
				return __tNumber;
			}
		}
		return 0;

}