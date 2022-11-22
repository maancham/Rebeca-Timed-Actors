












#ifndef __X_RAY_ACTOR_H__
#define __X_RAY_ACTOR_H__

class BaseActor;
class ServiceActor;
class SupervisorActor;
class InitiateRequesterActor;
class CommunicationSubstrateActor;
class ExecuteInvokerActor;
class VentilatorActor;

#include "ServiceActor.h"
#include "AbstractTimedActor.h"

class X_RayActor :
	public virtual ServiceActor
	,
	public virtual AbstractTimedActor
{

public:
    X_RayActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~X_RayActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);



		long constructorX_Ray(byte creatorID
	        ,
    		int
	         _ref__id
		);

       virtual void _msg_execute(byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
        )
        ;
       virtual long msgsrvexecute(
    		int
	         _ref_action
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
      	  void _timed_msg_execute(byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif