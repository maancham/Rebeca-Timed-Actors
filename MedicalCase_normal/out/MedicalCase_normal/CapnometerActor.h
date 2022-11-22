












#ifndef __CAPNOMETER_ACTOR_H__
#define __CAPNOMETER_ACTOR_H__

class BaseActor;
class CommunicationSubstrateActor;
class ClientActor;
class OximeterActor;
class MonitoringAppActor;
class PumpActor;
class PublishRequesterActor;
class SubscribeInvokerActor;
class RequestRequesterActor;
class ResponderInvokerActor;

#include "AbstractTimedActor.h"
#include "ClientActor.h"

class CapnometerActor :
	public virtual AbstractTimedActor
	,
	public virtual ClientActor
{

public:
    CapnometerActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~CapnometerActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);



		long constructorCapnometer(byte creatorID
	        ,
    		int
	         _ref__id
		);

       virtual void _msg_publish(byte senderId
	        ,
    		int
	         _ref_afterDelay
        )
        ;
       virtual long msgsrvpublish(
    		int
	         _ref_afterDelay
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
    		int
	         _ref_afterDelay
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif