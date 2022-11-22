












#ifndef __BASE_ACTOR_H__
#define __BASE_ACTOR_H__

class ServiceActor;
class SupervisorActor;
class InitiateRequesterActor;
class CommunicationSubstrateActor;
class ExecuteInvokerActor;
class VentilatorActor;
class X_RayActor;

#include "AbstractTimedActor.h"

class BaseActor :
	public virtual AbstractTimedActor
{

public:
    BaseActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














    );
	~BaseActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	int _ref_id;


		long constructorBase(byte creatorID
		);

       virtual void _msg_busy(byte senderId
        )
        ;
       virtual long msgsrvbusy(
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();







	protected:
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_busy(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
    	BaseActor* _synchmethod_find(
    		int
	         _ref__id
        )
        ;
};

#endif