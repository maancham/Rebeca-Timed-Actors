












#ifndef __RESPONDERINVOKER_ACTOR_H__
#define __RESPONDERINVOKER_ACTOR_H__

class BaseActor;
class CommunicationSubstrateActor;
class ClientActor;
class CapnometerActor;
class OximeterActor;
class MonitoringAppActor;
class PumpActor;
class PublishRequesterActor;
class SubscribeInvokerActor;
class RequestRequesterActor;

#include "AbstractTimedActor.h"
#include "BaseActor.h"

class ResponderInvokerActor :
	public virtual AbstractTimedActor
	,
	public virtual BaseActor
{

public:
    ResponderInvokerActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~ResponderInvokerActor();
	
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

    	int _ref_pumpId;

    	int _ref_requesterId;


		long constructorResponderInvoker(byte creatorID
	        ,
    		int
	         _ref__id
	        , 
    		int
	         _ref__pumpId
	        , 
    		int
	         _ref__requesterId
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
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif