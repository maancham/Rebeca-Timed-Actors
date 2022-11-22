












#ifndef __SERVICE_ACTOR_H__
#define __SERVICE_ACTOR_H__

class ClientActor;
class PublishRequesterActor;
class CommunicationSubstrateActor;
class SubscribeInvokerActor;

#include "AbstractTimedActor.h"

class ServiceActor :
	public virtual AbstractTimedActor
{

public:
    ServiceActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~ServiceActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	int _ref_transmitionTime;


		long constructorService(byte creatorID
		);

       virtual void _msg_publish(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvpublish(
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
       virtual void _msg_slowConsumption(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvslowConsumption(
    		int
	         _ref_Lm
        )
        ;
       virtual void _msg_staleData(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvstaleData(
    		int
	         _ref_Lm
        )
        ;
       virtual void _msg_slowPublication(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvslowPublication(
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
      	  void _timed_msg_publish(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_busy(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_slowConsumption(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_staleData(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_slowPublication(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif