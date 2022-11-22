












#ifndef __SERVICE_ACTOR_H__
#define __SERVICE_ACTOR_H__

class BaseActor;
class SupervisorActor;
class InitiateRequesterActor;
class CommunicationSubstrateActor;
class ExecuteInvokerActor;
class VentilatorActor;
class X_RayActor;

#include "AbstractTimedActor.h"
#include "BaseActor.h"

class ServiceActor :
	public virtual AbstractTimedActor
	,
	public virtual BaseActor
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



		long constructorService(byte creatorID
		);

       virtual void _msg_execute(byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
        )
        	=0
        ;
       virtual long msgsrvexecute(
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
        )
        	=0
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
        		virtual
      	  void _timed_msg_execute(byte senderId
	        ,
    		int
	         _ref_action
	        , 
    		int
	         _ref_Lm
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		=0
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif