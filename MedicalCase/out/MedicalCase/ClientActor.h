












#ifndef __CLIENT_ACTOR_H__
#define __CLIENT_ACTOR_H__

class BaseActor;
class CommunicationSubstrateActor;
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

class ClientActor :
	public virtual AbstractTimedActor
	,
	public virtual BaseActor
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



		long constructorClient(byte creatorID
		);

       virtual void _msg_accepted(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvaccepted(
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual void _msg_fastPublicationFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvfastPublicationFailure(
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
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
	         _ref_topic
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
	         _ref_topic
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
      	  void _timed_msg_accepted(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
	        , 
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_fastPublicationFailure(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_topic
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
	         _ref_topic
	        , 
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif