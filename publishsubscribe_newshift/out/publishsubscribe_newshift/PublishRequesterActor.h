












#ifndef __PUBLISHREQUESTER_ACTOR_H__
#define __PUBLISHREQUESTER_ACTOR_H__

class ClientActor;
class CommunicationSubstrateActor;
class SubscribeInvokerActor;
class ServiceActor;

#include "AbstractTimedActor.h"

class PublishRequesterActor :
	public virtual AbstractTimedActor
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


    	Timer _ref_lastPub;

    	int _ref_PR1Delay;

    	boolean _ref_theFirsttime;


		long constructorPublishRequester(byte creatorID
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
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif