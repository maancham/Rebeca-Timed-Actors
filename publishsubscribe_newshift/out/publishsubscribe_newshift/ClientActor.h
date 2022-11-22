












#ifndef __CLIENT_ACTOR_H__
#define __CLIENT_ACTOR_H__

class PublishRequesterActor;
class CommunicationSubstrateActor;
class SubscribeInvokerActor;
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



		long constructorClient(byte creatorID
		);

       virtual void _msg_publish(byte senderId
        )
        ;
       virtual long msgsrvpublish(
        )
        ;
       virtual void _msg_busy(byte senderId
        )
        ;
       virtual long msgsrvbusy(
        )
        ;
       virtual void _msg_accepted(byte senderId
        )
        ;
       virtual long msgsrvaccepted(
        )
        ;
       virtual void _msg_fastPublicationFailure(byte senderId
        )
        ;
       virtual long msgsrvfastPublicationFailure(
        )
        ;
       virtual void _msg_timeOutFailure(byte senderId
        )
        ;
       virtual long msgsrvtimeOutFailure(
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
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_busy(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_accepted(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_fastPublicationFailure(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_timeOutFailure(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif