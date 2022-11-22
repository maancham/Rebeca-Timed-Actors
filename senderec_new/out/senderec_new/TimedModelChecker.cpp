#include "TimedModelChecker.h"
#include<limits>

#ifdef PROGRESS_REPORT

void TimedModelChecker::progressReport(ostream& reportStream) {
    AbstractModelChecker::numberOfStates = AbstractTimedRebecaAnalyzer::numberOfStates;
    AbstractModelChecker::numberOfTransitions = AbstractTimedRebecaAnalyzer::numberOfTransitions;
	AbstractModelChecker::progressReport(reportStream);
}
#endif

TimedModelChecker::TimedModelChecker(ostream &outStream
	#ifdef EXPORT_STATE_SPACE
	    , ostream &statespaceStream
	#endif
    , int hashmapSize) : AbstractModelChecker(outStream
	#ifdef EXPORT_STATE_SPACE
    	, statespaceStream
	#endif
	), 
	AbstractTimedRebecaAnalyzer(outStream, hashmapSize)
	{

	this->hashmapSize = hashmapSize;
	current.state = NULL;
}

string TimedModelChecker::getModelCheckingResultName(int id) {
	return (
		id == DEADLINE_MISSED ? "deadline missed" :
			AbstractModelChecker::getModelCheckingResultName(id)
		);
}

long TimedModelChecker::getConsumedMemory() {
    long totalMem = 0;
    for (int i=0; i<REBEC_COUNT; i++)
    	totalMem += rebecs[i]->consumedMemory();
    totalMem += sizeof(TimedBFSState) * (AbstractTimedRebecaAnalyzer::numberOfStates - AbstractTimedRebecaAnalyzer::numberOfTimedBundles);
    totalMem += sizeof(RebecsShiftTime) * AbstractTimedRebecaAnalyzer::numberOfTimedBundles;
    return totalMem;
}

void TimedModelChecker::printCounterExample(int result, AbstractActor* causedBy) {

	if(!current.state)
		return;

	string lastStateRebecName, lastStateAction, lastStateSender;
	TIME_TYPE lastStateTime;
	if (causedBy != null) {
	    lastStateRebecName = causedBy->getName();
	    lastStateAction = causedBy->activeAction();
	    lastStateSender = rebecs[causedBy->senderQueue[0]]->getName();
	    lastStateTime = rebecs[causedBy->senderQueue[0]]->getNow();
	}
	
	list<TimedBFSState*> counterExampleStates;
	TimedBFSState* cursor = current.state;
	while(cursor) {
		counterExampleStates.push_front(cursor);
		cursor = cursor->parents.front().parent;
	}
	int cnt = 1;
	TimedBFSState* parent = NULL;
	for (list<TimedBFSState*>::iterator it = counterExampleStates.begin(); it != counterExampleStates.end(); it++, cnt++) {

		(*it)->stateID = cnt;

		if (parent != NULL) {
			parent->stateID = cnt - 1;
			printCounterExampleTransition(parent, *it);
		}

		unmarshalRebecs((*it)->rebsIndex, 
    		(*it)->shiftTimes[(*it)->parents.front().timeBundleIndex].shift);
    	
    	OpenBorderNode child;
    	child.state = (*it);
		child.stateActiveBundleNumber = 0;

		exportState(child, out);    	
		
		parent = *it;
	}
	if (causedBy != null) {
	    string terminationStateName = "end";
		switch(result) {
			case ASSERTION_FAILED: terminationStateName = "assertion failed"; break; 
			case DEADLOCK: terminationStateName = "deadlock"; break; 
			case QUEUE_OVERFLOW: terminationStateName = "queue overflow"; break; 
			case DEADLINE_MISSED: terminationStateName = "deadline missed"; break; 
		}
		list<TimedBFSState*>::iterator last = counterExampleStates.end();
		last--;
		string sourceName = to_string((*last)->stateID) + "_0" ;
		exportTransition(sourceName, terminationStateName, lastStateSender, lastStateRebecName, lastStateAction, lastStateTime, 0, out);
		out << "<state id=\"" << terminationStateName << "\" atomicpropositions=\"" << terminationStateName << "\">" << endl << "</state>" << endl;
	}
	
}

void TimedModelChecker::exportModelCheckingResult(string userSelectedPropertyName, string userSelectedPropertyType,
	int result, string message, AbstractActor* causedBy) {

    AbstractModelChecker::numberOfStates = AbstractTimedRebecaAnalyzer::numberOfStates;
    AbstractModelChecker::numberOfTransitions = AbstractTimedRebecaAnalyzer::numberOfTransitions;

    out  <<  "<model-checking-report>"  <<  std::endl;

	exportModelCheckingDetails(userSelectedPropertyName, userSelectedPropertyType, result, message);
	exportModelCheckingReportExtraDetails();
    out  <<  "<counter-example-trace>"  <<  std::endl;
	if (result != HEAP_OVERFLOW && result != SATISFIED)
		printCounterExample(result, causedBy);
    out  <<  "</counter-example-trace>"  <<  std::endl;
    out  <<  "</model-checking-report>"  <<  std::endl;
}

void TimedModelChecker::doModelChecking(string userSelectedPropertyName, string userSelectedPropertyType) {
	vector<AbstractActor*> referenceToRebecs;
	try {
		AbstractModelChecker::doModelChecking(userSelectedPropertyName, userSelectedPropertyType);

	    bfsStorage = new TimedRebecaBFSHashmap(hashmapSize, out);
	    #ifdef DEBUG
	    	out << "State space storage is created successfully." << endl;
	    #endif

		instantiationPart();	    

		doTimedModelChecking(userSelectedPropertyName, userSelectedPropertyType);
	    exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	SATISFIED, "", null);
	    	
    } catch (AssertionFailedException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	ASSERTION_FAILED, e.getMessage(), e.getCausedBy());
    } catch (DeadlockException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	DEADLOCK, "", e.getCausedBy());
    } catch (QueueOverflowException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	QUEUE_OVERFLOW, "", e.getCausedBy());
    } catch (CounterExampleException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	COUNTER_EXAMPLE, "", null);
    } catch (UnknownPropertyException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	UNKNOWN_PROPERTY, e.getMessage(), null);
    } catch (DeadlineMissedException &e) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	DEADLINE_MISSED, "", e.getCausedBy());
    } catch (...) {
    	exportModelCheckingResult(userSelectedPropertyName, userSelectedPropertyType,
	    	HEAP_OVERFLOW, "", null);
    }

    #ifdef EXPORT_STATE_SPACE
	    statespace << "</transitionsystem>" << endl;
	    statespace.flush();
	#endif

}

void TimedModelChecker::segmentationFault() {
	exportModelCheckingResult("", "", SEGMENTATION_FAULT, NULL, null);
	#ifdef EXPORT_STATE_SPACE
	    statespace << "</transitionsystem>" << endl;
	    statespace.flush();
	#endif
}






void TimedModelChecker::printCounterExampleTransition(TimedBFSState* parent, TimedBFSState* child) {

	OpenBorderNode parentNode, childNode;
	
	childNode.state = child;
	childNode.stateActiveBundleNumber = 0;
	
	parentNode.state = parent;
	parentNode.stateActiveBundleNumber = 0;
	
	int rebecId = child->parents.front().executedRebecIndex;
	string label = "";
	label = rebecs[rebecId]->activeAction();
	string sender = rebecs[rebecs[rebecId]->senderQueue[0]]->getName();
	exportTransition(parentNode, childNode, sender, rebecs[rebecId]->getName(), label, rebecs[rebecId]->getNow(), 0, out);
}
void TimedModelChecker::printModelCheckingOptions() {
    out << "\t<option>FTTS</option>" << std::endl;
}

void TimedModelChecker::exportModelCheckingReportExtraDetails() {
}


void TimedModelChecker::storeInitialState() {
	TIME_TYPE currentTime = MAX_TIME;
	for (int r = 0; r < REBEC_COUNT; r++) {
		if (!rebecs[r]->messageQueue[0])
			continue;
		currentTime = currentTime > rebecs[r]->executionTime[0] ? rebecs[r]->executionTime[0] : currentTime;
	}
	
    byte result;
    TIME_TYPE shift;

	for (int r2 = 0; r2 < REBEC_COUNT; r2++) {
		 rebecs[r2]->setNow(currentTime);
	}
	current = storeRecentlyCreatedState (result, shift, NULL, 0, 0);
	current.state->stateID = 1;
	
    #ifdef EXPORT_STATE_SPACE
	    statespace << "<transitionsystem>" << endl;
	    exportState(current, statespace);
    #endif

    #ifdef DEBUG
    	out << "The initial state is stored in the state space storage successfully." << endl;
    #endif

    current.currentTime = currentTime;
	openBorderQueue.push(current);
}

void TimedModelChecker::doTimedModelChecking(string userSelectedPropertyName, string userSelectedPropertyType) {

	storeInitialState();
	    
    #ifdef DEBUG
        out << "Before start state space generation \"while\" loop (The initial state is stored)." << endl;
    #endif
    
    while(!openBorderQueue.empty()) {

        TIME_TYPE currentTime = prepareWithNewState();
	    #ifdef DEBUG
	        out << "State is unmarshalled and try to find an enabled rebec." << endl;
	    #endif
	    
	    int highestPriority = std::numeric_limits<int>::max();
        for (int r = 0; r < REBEC_COUNT; r++) {
			if (!rebecs[r]->messageQueue[0])
                continue;
            TIME_TYPE execTime = (max(rebecs[r]->getNow(), rebecs[r]->executionTime[0]));
            if (execTime > currentTime) {
                continue;
            }        
            highestPriority = min(rebecs[r]->getPriority(), highestPriority);
		}
        for (int r = 0; r < REBEC_COUNT; r++) {
            if (rebecs[r]->getPriority() != highestPriority)
            	continue;
            if (!rebecs[r]->messageQueue[0])
                continue;
            TIME_TYPE execTime = (max(rebecs[r]->getNow(), rebecs[r]->executionTime[0]));
            if (execTime > currentTime) {
                continue;
            }
            long nonDetTrans;
            int numberOfAlternatives = getNumberOfAlternatives(r, execTime);
            for(int alternativesCounter = 0; alternativesCounter < numberOfAlternatives; alternativesCounter++) {
	            do {
					changeOrder(r, alternativesCounter);
				    if (rebecs[r]->deadline[0] < currentTime) {
				        throw DeadlineMissedException("Deadline missed.", rebecs[r]);
				    }
	            	nonDetTrans = executeRebec(r);
					unmarshalRebecs(current.state->rebsIndex, current.state->shiftTimes[current.stateActiveBundleNumber].shift);
				    #ifdef DEBUG_LEVEL_2
						out << "<state>" << endl;
						for (int cnt = 0; cnt < REBEC_COUNT; cnt++)
							rebecs[cnt]->exportStateInXML(out, "\t");
						out << "</state>" << endl;
					#endif
	                currentTime = current.currentTime;
	            } while (nonDetTrans > 0);
            }
        }
    }
}

long TimedModelChecker::executeRebec(int rebecId) {

	long nonDetTrans;
    byte result;
    TIME_TYPE shift;

    #ifdef EXPORT_STATE_SPACE
        char* actionName = rebecs[rebecId]->activeAction();
    #endif
    #ifdef DEBUG
        out << "Enabled rebec is found. Try to excute message \"" << rebecs[rebecId]->getName() << "::" << rebecs[rebecId]->activeAction() << "\"."<< endl;
    #endif

    #ifdef EXPORT_STATE_SPACE
    TIME_TYPE executionTime = rebecs[rebecId]->getNow();
	string sender = rebecs[rebecs[rebecId]->senderQueue[0]]->getName();
    #endif
    nonDetTrans = rebecs[rebecId]->execute();
    #ifdef DEBUG
        out << "Message \"" << rebecs[rebecId]->getName() << "::" << rebecs[rebecId]->activeAction() << "\" is executed " 
        	<< "and the return value is \"" << (long)nonDetTrans << "\"." << endl;
    #endif

    int nextRebecTime = MAX_TIME;
    for (int r2 = 0; r2 < REBEC_COUNT; r2++) {
    	int index = (rebecId == r2 ? 1 : 0);
		if (rebecs[r2]->messageQueue[index]) {
            TIME_TYPE rebecNextMessageTime = max(rebecs[r2]->getNow(), rebecs[r2]->executionTime[index]);
            nextRebecTime = min(rebecNextMessageTime ,nextRebecTime);
        }
    }

    for (int r2 = 0; r2 < REBEC_COUNT; r2++) {
    	if (rebecs[r2]->getNow() < nextRebecTime)
        	rebecs[r2]->setNow(nextRebecTime);
    }
    OpenBorderNode newState = storeRecentlyCreatedState(result, shift, current.state, current.stateActiveBundleNumber, rebecId);
    try {
    } catch (AssertionFailedException &e) {
    	e.setCausedBy(rebecs[rebecId]);
    	throw;
	}

    if (nextRebecTime == MAX_TIME) {
    	current = newState;
        throw DeadlockException("Deadlock");
    }

    if (result & NEW_STATE) {
        newState.currentTime = nextRebecTime;

        openBorderQueue.push(newState);
        
	    #ifdef EXPORT_STATE_SPACE
	    	exportState(newState, statespace);
	    #endif
        
    }
    #ifdef EXPORT_STATE_SPACE
    string label = actionName;
    exportTransition(current, newState, sender, rebecs[rebecId]->getName(), label, executionTime, shift, statespace);
	#endif
    return nonDetTrans;
}

