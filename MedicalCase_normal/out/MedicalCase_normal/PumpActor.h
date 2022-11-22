












#ifndef __PUMP_ACTOR_H__
#define __PUMP_ACTOR_H__

class BaseActor;
class CommunicationSubstrateActor;
class ClientActor;
class CapnometerActor;
class OximeterActor;
class MonitoringAppActor;
class PublishRequesterActor;
class SubscribeInvokerActor;
class RequestRequesterActor;
class ResponderInvokerActor;

#include "AbstractTimedActor.h"
#include "BaseActor.h"

class PumpActor :
	public virtual AbstractTimedActor
	,
	public virtual BaseActor
{

public:
    PumpActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~PumpActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);



		long constructorPump(byte creatorID
	        ,
    		int
	         _ref__id
		);

       virtual void _msg_response(byte senderId
	        ,
    		boolean
	         _ref_data
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvresponse(
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
      	  void _timed_msg_response(byte senderId
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