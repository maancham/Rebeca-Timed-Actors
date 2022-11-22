












#ifndef __EXECUTEINVOKER_ACTOR_H__
#define __EXECUTEINVOKER_ACTOR_H__

class BaseActor;
class ServiceActor;
class SupervisorActor;
class InitiateRequesterActor;
class CommunicationSubstrateActor;
class VentilatorActor;
class X_RayActor;

#include "AbstractTimedActor.h"
#include "BaseActor.h"

class ExecuteInvokerActor :
	public virtual AbstractTimedActor
	,
	public virtual BaseActor
{

public:
    ExecuteInvokerActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~ExecuteInvokerActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	Timer _ref_lastIni;

    	int _ref_serviceId;

    	int _ref_initiatorId;

    	boolean _ref_theFirsttime;


		long constructorExecuteInvoker(byte creatorID
	        ,
    		int
	         _ref__id
	        , 
    		int
	         _ref__serviceId
	        , 
    		int
	         _ref__initiatorId
		);

       virtual void _msg_initiate(byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvinitiate(
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual void _msg_ack(byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_actionStatus
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvack(
    		int
	         _ref_action
	        , 
    		int
	         _ref_actionStatus
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
      	  void _timed_msg_initiate(byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_ack(byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_actionStatus
	        , 
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif