

#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "Types.h"

#define SAFE_MODE
#define PROGRESS_REPORT
#define EXPORT_STATE_SPACE


#if (defined DEBUG || defined EXPORT_STATE_SPACE)
	#define ENABLE_STATE_ID
#endif

	 extern int _ref_CAP_ETCO2;
 
	 extern int _ref_CAP_RESPIRATORYRATE;
 
	 extern int _ref_OX_SPO2;
 
	 extern int _ref_OX_PULSERATE;
 
	 extern boolean _ref_ACTIVE;
 
	 extern boolean _ref_INACTIVE;
 
	 extern int _ref_NREQ;
 
	 extern int _ref_LREQ;
 
	 extern int _ref_RREQ;
 
	 extern int _ref_NRES;
 
	 extern int _ref_LRES;
 
	 extern int _ref_RRES;
 
	 extern int _ref_NPUB;
 
	 extern int _ref_LPUB;
 
	 extern int _ref_NSUB;
 
	 extern int _ref_XSUB;
 
	 extern int _ref_LSUB;
 
	 extern int _ref_RPUB;
 
	 extern int _ref_RSUB;
 
	 extern int _ref_consumptionTolerance;
 

#define NEW_STATE 1



#define TTS

#ifdef TTS
#define NO_ATOMIC_EXECUTION
#endif
#define TIME_TYPE int
#define MAX_TIME 0x7FFFFFFF
#define Timer TIME_TYPE

class AbstractActor;
class DeadlineMissedException: public Exception {
public:
	DeadlineMissedException(string message) : Exception(message) {}
	DeadlineMissedException(string message, AbstractActor* causedBy) : Exception(message, causedBy){} 
	~DeadlineMissedException() throw() {}
};


#endif