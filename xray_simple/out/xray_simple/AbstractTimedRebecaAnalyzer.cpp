#include "AbstractTimedRebecaAnalyzer.h"

		#define _ref_s (dynamic_cast<SupervisorActor*>(rebecs[0]))
		#define _ref_ir_v (dynamic_cast<InitiateRequesterActor*>(rebecs[1]))
		#define _ref_ei_v (dynamic_cast<ExecuteInvokerActor*>(rebecs[2]))
		#define _ref_v (dynamic_cast<VentilatorActor*>(rebecs[3]))
		#define _ref_ir_x (dynamic_cast<InitiateRequesterActor*>(rebecs[4]))
		#define _ref_ei_x (dynamic_cast<ExecuteInvokerActor*>(rebecs[5]))
		#define _ref_x (dynamic_cast<X_RayActor*>(rebecs[6]))
		#define _ref_cs (dynamic_cast<CommunicationSubstrateActor*>(rebecs[7]))
	
	

		#undef _ref_s
		#undef _ref_ir_v
		#undef _ref_ei_v
		#undef _ref_v
		#undef _ref_ir_x
		#undef _ref_ei_x
		#undef _ref_x
		#undef _ref_cs

bool operator > (const OpenBorderNode& node1, const OpenBorderNode& node2) {
    return node1.currentTime > node2.currentTime;
}

AbstractTimedRebecaAnalyzer::AbstractTimedRebecaAnalyzer(ostream &outStream, int hashmapSize) : analysisOutputStream(outStream){

	this->hashmapSize = hashmapSize;
	current.state = NULL;
}


void AbstractTimedRebecaAnalyzer::unmarshalRebecs(TimedRebecStateIndexType* node, TIME_TYPE *shift) {
	int i;
	for (i = 0; i < REBEC_COUNT; i++) {
        rebecs[i]->unmarshalActor(node[i], shift[i]);   // uncompress all states of all rebecs
    }
}

void AbstractTimedRebecaAnalyzer::marshalRebecs(TimedRebecStateIndexType* node, TIME_TYPE *shift) {
	int i;
	for (i=0; i<REBEC_COUNT; i++){
		node[i] = rebecs[i]->marshalActor(shift[i]);
	}
}

OpenBorderNode AbstractTimedRebecaAnalyzer::storeRecentlyCreatedState (byte &result,
	TIME_TYPE &shift, TimedBFSState* parent, int parentBundleIndex, int executedRebecIndex) {

    numberOfTransitions++;

	TimedBFSState temp;
    RebecsShiftTime rebecsLocalShift;
    OpenBorderNode node;
    node.stateActiveBundleNumber = 0;

    marshalRebecs(temp.rebsIndex, rebecsLocalShift.shift);
    #ifdef DEBUG_LEVEL_2
	    analysisOutputStream << "State is marshalled to these values [";
	    for (int cnt2 = 0; cnt2 < REBEC_COUNT; cnt2++) {
	    	analysisOutputStream << "<" << (long)temp.rebsIndex[cnt2].tableIndex << "," << (int)rebecsLocalShift.shift[cnt2] << ">, ";
	    }
	    analysisOutputStream << "]" << endl;
	#endif    
	result = NEW_STATE;
	if ((node.state = bfsStorage->exists(temp)) != null) {
	    #ifdef DEBUG_LEVEL_2
	        analysisOutputStream << "Untimed part of the state is found in the hash table." << endl;
	    #endif
        bool uniformShift = false;
		#ifdef DEBUG_LEVEL_2
        	analysisOutputStream <<"new state: [";
			for(int cnt = 0; cnt < REBEC_COUNT; cnt++) {
				analysisOutputStream <<(int)rebecsLocalShift.shift[cnt] << ",";
			}
			analysisOutputStream <<"]" << endl;
		#endif
		for (std::vector<RebecsShiftTime>::iterator it = node.state->shiftTimes.begin() ; (it != (node.state->shiftTimes.end())) && !uniformShift; ++it) {
            shift = 0;
            boolean first = true;
			uniformShift = true;
            #ifdef DEBUG_LEVEL_2
            	analysisOutputStream << "timed bundle " << (int)node.stateActiveBundleNumber << ": [";
				for(int cnt = 0; cnt < REBEC_COUNT; cnt++) {
					analysisOutputStream << (int)(*it).shift[cnt] << ",";
				}
				analysisOutputStream << "}" << endl;
            #endif
			for(int cnt = 0; cnt < REBEC_COUNT; cnt++) {
				if (!rebecs[cnt]->messageQueue[0])
					continue;
				if (first) {
					first = false;
					shift = rebecsLocalShift.shift[cnt] - (*it).shift[cnt];
				}
				else if (shift != rebecsLocalShift.shift[cnt] - (*it).shift[cnt]) {
					uniformShift = false;
					break;
				}
			}
			if (!uniformShift) {
				node.stateActiveBundleNumber++;
			}
		}

		if (uniformShift) {
		    #ifdef DEBUG_LEVEL_2
		        analysisOutputStream << "Uniform shift is found between this state and state \"" << node.state->stateID << "_" << (int)node.stateActiveBundleNumber << "\"."<< endl;
		    #endif
			result = ~NEW_STATE;
		} else {
		    #ifdef DEBUG_LEVEL_2
		        analysisOutputStream << "New time bundle is attached." << endl;
		    #endif
			shift = 0;
			numberOfStates++;
            numberOfTimedBundles++;
            node.state->shiftTimes.push_back(rebecsLocalShift);
	    	//node.state->stateID = numberOfStates;
        }
	}
	else {
	    #ifdef DEBUG_LEVEL_2
	        analysisOutputStream << "This state has new untimed part." << endl;
	    #endif
        shift = 0;
        node.state = bfsStorage->put(temp, parent, executedRebecIndex, rebecsLocalShift);
        numberOfStates++;
        node.state->stateID = numberOfStates;
        node.state->parents.front().timeBundleIndex = parentBundleIndex;
        numberOfTimedBundles++;
	}
	return node;
}

void AbstractTimedRebecaAnalyzer::changeOrder(int rebecId, int alternativesCounter) {
	for(int shiftQueue = alternativesCounter; shiftQueue > 0; shiftQueue--) {
		typedSwap(rebecs[rebecId]->executionTime[shiftQueue], rebecs[rebecId]->executionTime[shiftQueue - 1], TIME_TYPE)
		typedSwap(rebecs[rebecId]->deadline[shiftQueue], rebecs[rebecId]->deadline[shiftQueue - 1], TIME_TYPE)
		typedSwap(rebecs[rebecId]->messageQueue[shiftQueue], rebecs[rebecId]->messageQueue[shiftQueue - 1], byte)
		typedSwap(rebecs[rebecId]->senderQueue[shiftQueue], rebecs[rebecId]->senderQueue[shiftQueue - 1], byte)
		typedSwap(rebecs[rebecId]->paramQueue[shiftQueue], rebecs[rebecId]->paramQueue[shiftQueue - 1], byte*)
	}
}

int AbstractTimedRebecaAnalyzer::getNumberOfAlternatives(int rebecId, TIME_TYPE executionTime) {
	int numberOfAlternatives = 1;
	long baseTime = rebecs[rebecId]->executionTime[0];
	int basePriority = rebecs[rebecId]->messagePriorities[rebecs[rebecId]->messageQueue[0]];
    while((numberOfAlternatives < rebecs[rebecId]->maxQueueLength) &&
		(rebecs[rebecId]->messageQueue[numberOfAlternatives]) &&
		(rebecs[rebecId]->executionTime[numberOfAlternatives] == baseTime) &&
		(rebecs[rebecId]->messagePriorities[rebecs[rebecId]->messageQueue[numberOfAlternatives]] == basePriority)) {
    	numberOfAlternatives++;
    }
    
    return numberOfAlternatives;
}

TIME_TYPE AbstractTimedRebecaAnalyzer::prepareWithNewState() {
    current = openBorderQueue.top();
    openBorderQueue.pop();
    unmarshalRebecs(current.state->rebsIndex, 
    		current.state->shiftTimes[current.stateActiveBundleNumber].shift);
    return current.currentTime;
}

void AbstractTimedRebecaAnalyzer::instantiationPart() {
	vector<AbstractActor*>* referenceToRebecs = new vector<AbstractActor*>();
	byte maxRebecID = 0;
    	SupervisorActor* _ref_s = new SupervisorActor(maxRebecID++, "s", 20, 12, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 284
    											, hashmapSize
    											);
        rebecs.push_back(_ref_s);
        referenceToRebecs->push_back(_ref_s);
    	InitiateRequesterActor* _ref_ir_v = new InitiateRequesterActor(maxRebecID++, "ir_v", 20, 12, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 297
    											, hashmapSize
    											);
        rebecs.push_back(_ref_ir_v);
        referenceToRebecs->push_back(_ref_ir_v);
    	ExecuteInvokerActor* _ref_ei_v = new ExecuteInvokerActor(maxRebecID++, "ei_v", 20, 12, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 297
    											, hashmapSize
    											);
        rebecs.push_back(_ref_ei_v);
        referenceToRebecs->push_back(_ref_ei_v);
    	VentilatorActor* _ref_v = new VentilatorActor(maxRebecID++, "v", 20, 8, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 204
    											, hashmapSize
    											);
        rebecs.push_back(_ref_v);
        referenceToRebecs->push_back(_ref_v);
    	InitiateRequesterActor* _ref_ir_x = new InitiateRequesterActor(maxRebecID++, "ir_x", 20, 12, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 297
    											, hashmapSize
    											);
        rebecs.push_back(_ref_ir_x);
        referenceToRebecs->push_back(_ref_ir_x);
    	ExecuteInvokerActor* _ref_ei_x = new ExecuteInvokerActor(maxRebecID++, "ei_x", 20, 12, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 297
    											, hashmapSize
    											);
        rebecs.push_back(_ref_ei_x);
        referenceToRebecs->push_back(_ref_ei_x);
    	X_RayActor* _ref_x = new X_RayActor(maxRebecID++, "x", 20, 8, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 204
    											, hashmapSize
    											);
        rebecs.push_back(_ref_x);
        referenceToRebecs->push_back(_ref_x);
    	CommunicationSubstrateActor* _ref_cs = new CommunicationSubstrateActor(maxRebecID++, "cs", 20, 16, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 364
    											, hashmapSize
    											);
        rebecs.push_back(_ref_cs);
        referenceToRebecs->push_back(_ref_cs);

    	_ref_s->known = new byte[MAX_KNOWN];
    	_ref_s->known[0] = _ref_s->myID;
 _ref_s->known[1] = _ref_ir_v->myID;
 _ref_s->known[2] = _ref_ir_x->myID;

		_ref_s->constructorSupervisor(INITIAL_SENDER
	        , 0
		);
    	_ref_ir_v->known = new byte[MAX_KNOWN];
    	_ref_ir_v->known[0] = _ref_ir_v->myID;
 _ref_ir_v->known[1] = _ref_cs->myID;

		_ref_ir_v->constructorInitiateRequester(INITIAL_SENDER
	        , 1
	        , 0
	        , 2
		);
    	_ref_ei_v->known = new byte[MAX_KNOWN];
    	_ref_ei_v->known[0] = _ref_ei_v->myID;
 _ref_ei_v->known[1] = _ref_cs->myID;

		_ref_ei_v->constructorExecuteInvoker(INITIAL_SENDER
	        , 2
	        , 3
	        , 1
		);
    	_ref_v->known = new byte[MAX_KNOWN];
    	_ref_v->known[0] = _ref_v->myID;
 _ref_v->known[1] = _ref_ei_v->myID;

		_ref_v->constructorVentilator(INITIAL_SENDER
	        , 3
		);
    	_ref_ir_x->known = new byte[MAX_KNOWN];
    	_ref_ir_x->known[0] = _ref_ir_x->myID;
 _ref_ir_x->known[1] = _ref_cs->myID;

		_ref_ir_x->constructorInitiateRequester(INITIAL_SENDER
	        , 4
	        , 0
	        , 5
		);
    	_ref_ei_x->known = new byte[MAX_KNOWN];
    	_ref_ei_x->known[0] = _ref_ei_x->myID;
 _ref_ei_x->known[1] = _ref_cs->myID;

		_ref_ei_x->constructorExecuteInvoker(INITIAL_SENDER
	        , 5
	        , 6
	        , 4
		);
    	_ref_x->known = new byte[MAX_KNOWN];
    	_ref_x->known[0] = _ref_x->myID;
 _ref_x->known[1] = _ref_ei_x->myID;

		_ref_x->constructorX_Ray(INITIAL_SENDER
	        , 6
		);
    	_ref_cs->known = new byte[MAX_KNOWN];
    	_ref_cs->known[0] = _ref_cs->myID;

		_ref_cs->constructorCommunicationSubstrate(INITIAL_SENDER
	        , 7
		);

	numberOfStates = 0;
	numberOfTransitions = 0;
    numberOfTimedBundles = 0;

    #ifdef DEBUG
    	analysisOutputStream << "Rebecs are instanciated successfully." << endl;
    #endif

}

void AbstractTimedRebecaAnalyzer::exportState(OpenBorderNode &current, ostream& outStream) {
	outStream << "<state id=\"" << current.state->stateID << "_" << 
		(int)current.stateActiveBundleNumber << "\" ";
	outStream << "atomicpropositions=\""; 
	outStream << "\" ";
	#ifdef SIMPLIFIED_STATESPACE
		#ifdef TTS
			outStream << " now=\"" << rebecs[0]->getNow() << "\"";
		#endif 
		outStream << ">" << endl;
	#else
		outStream << ">" << endl;
		for (int cnt = 0; cnt < REBEC_COUNT; cnt++)
			rebecs[cnt]->exportStateInXML(outStream, "\t");
	#endif
	outStream << "</state>" << endl;
}

void AbstractTimedRebecaAnalyzer::exportTransition(string source, string destination, 
		string sender, string owner, string label, TIME_TYPE executionTime, TIME_TYPE shift, ostream& outStream) {
    outStream << "<transition source=\"" << source << "\" destination=\"" << 
		destination << "\" executionTime=\"" << (int)executionTime << "\" shift=\"" << (int) shift << 
    	"\"> <messageserver sender=\"" << sender << "\" owner=\"" << 
    	owner << "\" title=\"" << label << "\"/></transition>" << endl;
}


void AbstractTimedRebecaAnalyzer::exportTransition(OpenBorderNode &source, OpenBorderNode &destination, 
		string sender, string owner, string label, TIME_TYPE executionTime, TIME_TYPE shift, ostream& outStream) {
		exportTransition(to_string(source.state->stateID) + "_" + to_string((long)source.stateActiveBundleNumber), 
		to_string(destination.state->stateID) + "_" + to_string((long)destination.stateActiveBundleNumber), 
		sender, owner, label, executionTime, shift, outStream);
		}