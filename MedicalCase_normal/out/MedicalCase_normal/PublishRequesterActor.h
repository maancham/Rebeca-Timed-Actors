












#ifndef __PUBLISHREQUESTER_ACTOR_H__
#define __PUBLISHREQUESTER_ACTOR_H__

class BaseActor;
class CommunicationSubstrateActor;
class ClientActor;
class CapnometerActor;
class OximeterActor;
class MonitoringAppActor;
class PumpActor;
class SubscribeInvokerActor;
class RequestRequesterActor;
class ResponderInvokerActor;

#include "AbstractTimedActor.h"
#include "BaseActor.h"

class PublishRequesterActor :
	public virtual AbstractTimedActor
	,
	public virtual BaseActor
{

public:
    PublishRequesterActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~PublishRequesterActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	int _ref_lastPub;

    	int _ref_subscriberInvokerId;

    	int _ref_clientId;

    	int _ref_PR1Delay;


		long constructorPublishRequester(byte creatorID
	        ,
    		int
	         _ref__id
	        , 
    		int
	         _ref__clientId
	        , 
    		int
	         _ref__subscriberInvokerId
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
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif