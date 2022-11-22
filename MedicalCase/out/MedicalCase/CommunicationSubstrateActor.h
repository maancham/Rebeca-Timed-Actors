












#ifndef __COMMUNICATIONSUBSTRATE_ACTOR_H__
#define __COMMUNICATIONSUBSTRATE_ACTOR_H__

class BaseActor;
class ClientActor;
class CapnometerActor;
class OximeterActor;
class MonitoringAppActor;
class PumpActor;
class PublishRequesterActor;
class SubscribeInvokerActor;
class RequestRequesterActor;
class ResponderInvokerActor;

#include "AbstractTimedActor.h"
#include "BaseActor.h"

class CommunicationSubstrateActor :
	public virtual AbstractTimedActor
	,
	public virtual BaseActor
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
	        ,
    		int
	         _ref__id
		);

       virtual void _msg_publish(byte senderId
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
        )
        ;
       virtual long msgsrvpublish(
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
        )
        ;
       virtual void _msg_request(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderInvokerId
        )
        ;
       virtual long msgsrvrequest(
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderInvokerId
        )
        ;
       virtual void _msg_response(byte senderId
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
        )
        ;
       virtual long msgsrvresponse(
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
        )
        ;
       virtual void _msg_excessLoadFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
        )
        ;
       virtual long msgsrvexcessLoadFailure(
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
        )
        ;
       virtual void _msg_timeOutFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
        )
        ;
       virtual long msgsrvtimeOutFailure(
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
        )
        ;
       virtual void _msg_dataUnavailableFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
        )
        ;
       virtual long msgsrvdataUnavailableFailure(
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_publish(byte senderId
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
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_request(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_responderInvokerId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_response(byte senderId
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
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_excessLoadFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_timeOutFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_dataUnavailableFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_requesterId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif