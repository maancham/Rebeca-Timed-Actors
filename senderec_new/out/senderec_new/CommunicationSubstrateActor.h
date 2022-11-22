












#ifndef __COMMUNICATIONSUBSTRATE_ACTOR_H__
#define __COMMUNICATIONSUBSTRATE_ACTOR_H__

class ClientActor;
class SendRequesterActor;
class ReceiveInvokerActor;
class ServiceActor;

#include "AbstractTimedActor.h"

class CommunicationSubstrateActor :
	public virtual AbstractTimedActor
{

public:
    CommunicationSubstrateActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~CommunicationSubstrateActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);



		long constructorCommunicationSubstrate(byte creatorID
		);

       virtual void _msg_send(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvsend(
    		int
	         _ref_Lm
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
       virtual void _msg_excessLoadFailure(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvexcessLoadFailure(
    		int
	         _ref_Lm
        )
        ;
       virtual void _msg_timeOutFailure(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvtimeOutFailure(
    		int
	         _ref_Lm
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_send(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_ack(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_excessLoadFailure(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_timeOutFailure(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif