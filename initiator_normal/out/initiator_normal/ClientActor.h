












#ifndef __CLIENT_ACTOR_H__
#define __CLIENT_ACTOR_H__

class InitiateRequesterActor;
class CommunicationSubstrateActor;
class ExecuteInvokerActor;
class ServiceActor;

#include "AbstractTimedActor.h"

class ClientActor :
	public virtual AbstractTimedActor
{

public:
    ClientActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~ClientActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	int _ref_TotalLatency;


		long constructorClient(byte creatorID
		);

       virtual void _msg_initiate(byte senderId
        )
        ;
       virtual long msgsrvinitiate(
        )
        ;
       virtual void _msg_ack(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvack(
    		int
	         _ref_Lm
        )
        ;
       virtual void _msg_busy(byte senderId
        )
        ;
       virtual long msgsrvbusy(
        )
        ;
       virtual void _msg_fastSendFailure(byte senderId
        )
        ;
       virtual long msgsrvfastSendFailure(
        )
        ;
       virtual void _msg_timeOutFailure(byte senderId
        )
        ;
       virtual long msgsrvtimeOutFailure(
        )
        ;
       virtual void _msg_excessLoadFailure(byte senderId
        )
        ;
       virtual long msgsrvexcessLoadFailure(
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_initiate(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_ack(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_busy(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_fastSendFailure(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_timeOutFailure(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_excessLoadFailure(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif