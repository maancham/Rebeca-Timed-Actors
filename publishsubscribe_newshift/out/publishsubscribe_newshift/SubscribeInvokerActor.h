












#ifndef __SUBSCRIBEINVOKER_ACTOR_H__
#define __SUBSCRIBEINVOKER_ACTOR_H__

class ClientActor;
class PublishRequesterActor;
class CommunicationSubstrateActor;
class ServiceActor;

#include "AbstractTimedActor.h"

class SubscribeInvokerActor :
	public virtual AbstractTimedActor
{

public:
    SubscribeInvokerActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~SubscribeInvokerActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	Timer _ref_lastPub;

    	boolean _ref_theFirsttime;


		long constructorSubscribeInvoker(byte creatorID
		);

       virtual void _msg_publish(byte senderId
	        ,
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
        )
        ;
       virtual long msgsrvpublish(
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
        )
        ;
       virtual void _msg_consumed(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvconsumed(
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
       virtual void _msg_drop(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvdrop(
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
	        , 
    		int
	         _ref_life
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_consumed(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_busy(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_drop(byte senderId
	        ,
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif