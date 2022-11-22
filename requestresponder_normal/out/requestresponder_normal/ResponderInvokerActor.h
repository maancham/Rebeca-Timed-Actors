












#ifndef __RESPONDERINVOKER_ACTOR_H__
#define __RESPONDERINVOKER_ACTOR_H__

class ClientActor;
class RequestRequesterActor;
class CommunicationSubstrateActor;
class ServiceActor;

#include "AbstractTimedActor.h"

class ResponderInvokerActor :
	public virtual AbstractTimedActor
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

    	boolean _ref_theFirsttime;


		long constructorResponderInvoker(byte creatorID
		);

       virtual void _msg_request(byte senderId
	        ,
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvrequest(
    		int
	         _ref_Lm
        )
        ;
       virtual void _msg_response(byte senderId
	        ,
    		int
	         _ref_Lm
	        , 
    		int
	         _ref_life
        )
        ;
       virtual long msgsrvresponse(
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
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_response(byte senderId
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