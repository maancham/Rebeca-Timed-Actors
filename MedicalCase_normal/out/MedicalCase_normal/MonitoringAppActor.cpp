                                    


#include "Types.h"
#include "MonitoringAppActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "BaseActor.h"
#include "CommunicationSubstrateActor.h"
#include "ClientActor.h"
#include "CapnometerActor.h"
#include "OximeterActor.h"
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
#define FASTREQUESTFAILURE 5
#define TIMEOUTFAILURE 6
#define EXCESSLOADFAILURE 7
#define DATAUNAVAILABLEFAILURE 8
#define STALEDATA 9
#define SLOWCONSUMPTION 10
#define STALEDATAPUB 11
#define SLOWPUBLICATION 12


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<MonitoringAppActor*>(rebecs[myID]))
    	#define _ref_SI_c (dynamic_cast<SubscribeInvokerActor*>(rebecs[known[1]]))
    	#define _ref_SI_o (dynamic_cast<SubscribeInvokerActor*>(rebecs[known[2]]))
    	#define _ref_pump (dynamic_cast<RequestRequesterActor*>(rebecs[known[3]]))
    
MonitoringAppActor::MonitoringAppActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[11 + 2];
    messagePriorities = new int[11 + 2];
	messageNames[0] = (char*)"Empty";

	
		
	
		
		
       		 messageNames[1] = (char*)"BUSY";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"PUBLISH";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"REQUEST";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"RESPONSE";
			messagePriorities[4] = std::numeric_limits<int>::max();
       		 messageNames[5] = (char*)"FASTREQUESTFAILURE";
			messagePriorities[5] = std::numeric_limits<int>::max();
       		 messageNames[6] = (char*)"TIMEOUTFAILURE";
			messagePriorities[6] = std::numeric_limits<int>::max();
       		 messageNames[7] = (char*)"EXCESSLOADFAILURE";
			messagePriorities[7] = std::numeric_limits<int>::max();
       		 messageNames[8] = (char*)"DATAUNAVAILABLEFAILURE";
			messagePriorities[8] = std::numeric_limits<int>::max();
       		 messageNames[9] = (char*)"STALEDATA";
			messagePriorities[9] = std::numeric_limits<int>::max();
       		 messageNames[10] = (char*)"SLOWCONSUMPTION";
			messagePriorities[10] = std::numeric_limits<int>::max();
       		 messageNames[11] = (char*)"STALEDATAPUB";
			messagePriorities[11] = std::numeric_limits<int>::max();
       		 messageNames[12] = (char*)"SLOWPUBLICATION";
			messagePriorities[12] = std::numeric_limits<int>::max();








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

MonitoringAppActor::~MonitoringAppActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void MonitoringAppActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
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
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "publish("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_topic)
					<< ", "
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case REQUEST: {

            	boolean _ref_command;
            
            
_ref_command = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "request("
                	<< (_ref_command? "true" : "false")
                << ")";
                break;
               }
            case RESPONSE: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
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
                	<< ((int)_ref_requesterId)
                << ")";
                break;
               }
            case FASTREQUESTFAILURE: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "fastRequestFailure("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_responderId)
                << ")";
                break;
               }
            case TIMEOUTFAILURE: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "timeOutFailure("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_responderId)
                << ")";
                break;
               }
            case EXCESSLOADFAILURE: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "excessLoadFailure("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_responderId)
                << ")";
                break;
               }
            case DATAUNAVAILABLEFAILURE: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "dataUnavailableFailure("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_responderId)
                << ")";
                break;
               }
            case STALEDATA: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "staleData("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_responderId)
                << ")";
                break;
               }
            case SLOWCONSUMPTION: {

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "slowConsumption("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_topic)
					<< ", "
                	<< ((int)_ref_Lm)
                << ")";
                break;
               }
            case STALEDATAPUB: {

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "staleDataPub("
                	<< (_ref_data? "true" : "false")
					<< ", "
                	<< ((int)_ref_Lm)
					<< ", "
                	<< ((int)_ref_responderId)
                << ")";
                break;
               }
            case SLOWPUBLICATION: {

            	boolean _ref_data;
            
            

            	int _ref_topic;
            
            
                	memcpy(&_ref_topic, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[cnt] [i] & AbstractActor::coeff[0]);
++i;                out << "slowPublication("
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


void MonitoringAppActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void MonitoringAppActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){

		BaseActor::exportInsideStateVariablesTagInXML(out, tab);

			    	out << tab << '\t' << "<variable name=\"MonitoringApp.transmitionTime\" type=\"int\">" <<  
			    		((int)_ref_transmitionTime) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"MonitoringApp.TotalLatency\" type=\"int\">" <<  
			    		((int)_ref_TotalLatency) << "</variable>" << endl;
}


char* MonitoringAppActor::getClassName() {
    return (char*)"MonitoringApp";
}
void MonitoringAppActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_transmitionTime, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_TotalLatency, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

    
		BaseActor::marshalVariablesToArray( & array[marshalIndex]);

	
}
void MonitoringAppActor::marshalActorToArray(byte* array) {
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

void MonitoringAppActor::unmarshalActorFromArray(byte* array) {

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

void MonitoringAppActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_transmitionTime, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_TotalLatency, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
    
		BaseActor::unmarshalVariablesFromArray( & array[index]);

	
}

/*#ifdef DEBUG
void MonitoringAppActor::printStateContent(long stateNo) {
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
long MonitoringAppActor::methodSelectorAndExecutor() {

	
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
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvpublish(
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
                    );
                }
            case REQUEST:
                {
                    int i = 0;

            	boolean _ref_command;
            
            
_ref_command = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvrequest(
            _ref_command
                    );
                }
            case RESPONSE:
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
++i;                    return msgsrvresponse(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_requesterId
                    );
                }
            case FASTREQUESTFAILURE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvfastRequestFailure(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
                    );
                }
            case TIMEOUTFAILURE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvtimeOutFailure(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
                    );
                }
            case EXCESSLOADFAILURE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvexcessLoadFailure(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
                    );
                }
            case DATAUNAVAILABLEFAILURE:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvdataUnavailableFailure(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
                    );
                }
            case STALEDATA:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvstaleData(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
                    );
                }
            case SLOWCONSUMPTION:
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
++i;                    return msgsrvslowConsumption(
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
                    );
                }
            case STALEDATAPUB:
                {
                    int i = 0;

            	boolean _ref_data;
            
            

            	int _ref_Lm;
            
            
                	memcpy(&_ref_Lm, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_responderId;
            
            
                	memcpy(&_ref_responderId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
_ref_data = (bool) (paramQueue[0] [i] & AbstractActor::coeff[0]);
++i;                    return msgsrvstaleDataPub(
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
                    );
                }
            case SLOWPUBLICATION:
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
++i;                    return msgsrvslowPublication(
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


    long MonitoringAppActor::constructorMonitoringApp (byte creatorId 
	        ,
    		int
	         _ref__id
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "MonitoringApp";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_id=_ref__id);
			(_ref_transmitionTime=0);
			(_ref_TotalLatency=0);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}



		void MonitoringAppActor::_msg_publish (byte senderId
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

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void MonitoringAppActor::_msg_request (byte senderId
	        ,
    		boolean
	         _ref_command
  	  ){
        queueTail = 0;
        enqueue(messageQueue, REQUEST);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            

paramsValues [index] = 0;
paramsValues [index] |= (_ref_command ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void MonitoringAppActor::_msg_response (byte senderId
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
        enqueue(messageQueue, RESPONSE);
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

		void MonitoringAppActor::_msg_fastRequestFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, FASTREQUESTFAILURE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_responderId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void MonitoringAppActor::_msg_timeOutFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
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
            
                	memcpy(&paramsValues [index], &_ref_responderId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void MonitoringAppActor::_msg_excessLoadFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
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
            
                	memcpy(&paramsValues [index], &_ref_responderId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void MonitoringAppActor::_msg_dataUnavailableFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
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
            
                	memcpy(&paramsValues [index], &_ref_responderId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void MonitoringAppActor::_msg_staleData (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
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
            
                	memcpy(&paramsValues [index], &_ref_responderId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void MonitoringAppActor::_msg_slowConsumption (byte senderId
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
        enqueue(messageQueue, SLOWCONSUMPTION);
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

		void MonitoringAppActor::_msg_staleDataPub (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, STALEDATAPUB);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
            
                	memcpy(&paramsValues [index], &_ref_Lm, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_responderId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

paramsValues [index] = 0;
paramsValues [index] |= (_ref_data ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
++index;            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void MonitoringAppActor::_msg_slowPublication (byte senderId
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
        enqueue(messageQueue, SLOWPUBLICATION);
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









	byte** MonitoringAppActor::table = (byte**)0xFFFFFFFF;

	byte** MonitoringAppActor::getTable() {
		return table;
	}








	vector<TimeFrame>** MonitoringAppActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** MonitoringAppActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void MonitoringAppActor::_timed_msg_publish (byte senderId
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
	    _msg_publish (senderId
	        ,
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_request (byte senderId
	        ,
    		boolean
	         _ref_command
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_request (senderId
	        ,
            _ref_command
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_response (byte senderId
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
	    _msg_response (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_requesterId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_fastRequestFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_fastRequestFailure (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_timeOutFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_timeOutFailure (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_excessLoadFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_excessLoadFailure (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_dataUnavailableFailure (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_dataUnavailableFailure (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_staleData (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_staleData (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_slowConsumption (byte senderId
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
	    _msg_slowConsumption (senderId
	        ,
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_staleDataPub (byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_staleDataPub (senderId
	        ,
            _ref_data
	        , 
            _ref_Lm
	        , 
            _ref_responderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void MonitoringAppActor::_timed_msg_slowPublication (byte senderId
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
	    _msg_slowPublication (senderId
	        ,
            _ref_data
	        , 
            _ref_topic
	        , 
            _ref_Lm
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void MonitoringAppActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	    long MonitoringAppActor::msgsrvpublish (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;SubscribeInvokerActor *temp1;MonitoringAppActor *temp2;MonitoringAppActor *temp3;SubscribeInvokerActor *temp4;MonitoringAppActor *temp5;
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
			int _ref_consumeDelay = (__tNumber|=1, nonDetVariable0);

			(_ref_transmitionTime=_ref_Lm);
			if (((_ref_topic==_ref_OX_SPO2)||(_ref_topic==_ref_OX_PULSERATE))) {
				{
					if ((_ref_data==false)) {
						{
							(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 154"), temp0)->_timed_msg_request(myID, _ref_INACTIVE, _ref_consumeDelay + _ref_now, MAX_TIME);
						}
}
					else {
						{
							(temp1=_ref_SI_o, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 157"), temp1)->_timed_msg_consumed(myID, _ref_data, _ref_topic, (_ref_Lm+3), _ref_consumeDelay + _ref_now, MAX_TIME);
							(temp2=_ref_self, assertion(temp2!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 158"), temp2)->_timed_msg_request(myID, _ref_ACTIVE, _ref_consumeDelay + _ref_now, MAX_TIME);
						}
}
				}
}
			if (((_ref_topic==_ref_CAP_ETCO2)||(_ref_topic==_ref_CAP_RESPIRATORYRATE))) {
				{
					if ((_ref_data==false)) {
						{
							(temp3=_ref_self, assertion(temp3!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 164"), temp3)->_timed_msg_request(myID, _ref_INACTIVE, _ref_consumeDelay + _ref_now, MAX_TIME);
						}
}
					else {
						{
							(temp4=_ref_SI_c, assertion(temp4!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 167"), temp4)->_timed_msg_consumed(myID, _ref_data, _ref_topic, (_ref_Lm+5), _ref_consumeDelay + _ref_now, MAX_TIME);
							(temp5=_ref_self, assertion(temp5!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 168"), temp5)->_timed_msg_request(myID, _ref_ACTIVE, _ref_consumeDelay + _ref_now, MAX_TIME);
						}
}
				}
}
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
	    long MonitoringAppActor::msgsrvrequest (
    		boolean
	         _ref_command
    ) {
    			long arrayIndexChecker = 0;RequestRequesterActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "request";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_pump, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 175"), temp0)->_timed_msg_request(myID, _ref_command, 3, 3 + _ref_now, MAX_TIME);
		}
		return 0;

}
	    long MonitoringAppActor::msgsrvresponse (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "response";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_TotalLatency=_ref_Lm);
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 180"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long MonitoringAppActor::msgsrvfastRequestFailure (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "fastRequestFailure";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 184"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long MonitoringAppActor::msgsrvtimeOutFailure (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "timeOutFailure";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 185"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long MonitoringAppActor::msgsrvexcessLoadFailure (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "excessLoadFailure";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 186"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long MonitoringAppActor::msgsrvdataUnavailableFailure (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "dataUnavailableFailure";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 187"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long MonitoringAppActor::msgsrvstaleData (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "staleData";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 188"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long MonitoringAppActor::msgsrvslowConsumption (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "slowConsumption";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 190"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long MonitoringAppActor::msgsrvstaleDataPub (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderId
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "staleDataPub";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 191"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
	    long MonitoringAppActor::msgsrvslowPublication (
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
    ) {
    			long arrayIndexChecker = 0;MonitoringAppActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "slowPublication";
		#endif

	#ifdef TTS
	#endif

		{
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 192"), temp0)->_timed_msg_busy(myID, _ref_now, MAX_TIME);
		}
		return 0;

}
