












#ifndef __REQUESTREQUESTER_ACTOR_H__
#define __REQUESTREQUESTER_ACTOR_H__

class BaseActor;
class CommunicationSubstrateActor;
class ClientActor;
class CapnometerActor;
class OximeterActor;
class MonitoringAppActor;
class PumpActor;
class PublishRequesterActor;
class SubscribeInvokerActor;
class ResponderInvokerActor;

#include "AbstractTimedActor.h"
#include "BaseActor.h"

class RequestRequesterActor :
	public virtual AbstractTimedActor
	,
	public virtual BaseActor
{

public:
    RequestRequesterActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~RequestRequesterActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	int _ref_lastReq;

    	int _ref_responderInvokerId;

    	int _ref_clientId;


		long constructorRequestRequester(byte creatorID
	        ,
    		int
	         _ref__id
	        , 
    		int
	         _ref__clientId
	        , 
    		int
	         _ref__responderInvokerId
		);

       virtual void _msg_request(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvrequest(
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
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
        )
        ;
       virtual void _msg_excessLoadFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvexcessLoadFailure(
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual void _msg_timeOutFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvtimeOutFailure(
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual void _msg_dataUnavailableFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvdataUnavailableFailure(
    		boolean
	         _ref_data
	        , 
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
      	  void _timed_msg_request(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
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
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_excessLoadFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_timeOutFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_dataUnavailableFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif