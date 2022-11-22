#include "AbstractTimedRebecaAnalyzer.h"

		#define _ref_c (dynamic_cast<CapnometerActor*>(rebecs[0]))
		#define _ref_pr_c (dynamic_cast<PublishRequesterActor*>(rebecs[1]))
		#define _ref_si_c (dynamic_cast<SubscribeInvokerActor*>(rebecs[2]))
		#define _ref_o (dynamic_cast<OximeterActor*>(rebecs[3]))
		#define _ref_pr_o (dynamic_cast<PublishRequesterActor*>(rebecs[4]))
		#define _ref_si_o (dynamic_cast<SubscribeInvokerActor*>(rebecs[5]))
		#define _ref_cs (dynamic_cast<CommunicationSubstrateActor*>(rebecs[6]))
		#define _ref_ma (dynamic_cast<MonitoringAppActor*>(rebecs[7]))
		#define _ref_rr (dynamic_cast<RequestRequesterActor*>(rebecs[8]))
		#define _ref_ri (dynamic_cast<ResponderInvokerActor*>(rebecs[9]))
		#define _ref_p (dynamic_cast<PumpActor*>(rebecs[10]))
	
	

		#undef _ref_c
		#undef _ref_pr_c
		#undef _ref_si_c
		#undef _ref_o
		#undef _ref_pr_o
		#undef _ref_si_o
		#undef _ref_cs
		#undef _ref_ma
		#undef _ref_rr
		#undef _ref_ri
		#undef _ref_p

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
    	CapnometerActor* _ref_c = new CapnometerActor(maxRebecID++, "c", 20, 4, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 124
    											, hashmapSize
    											);
        rebecs.push_back(_ref_c);
        referenceToRebecs->push_back(_ref_c);
    	PublishRequesterActor* _ref_pr_c = new PublishRequesterActor(maxRebecID++, "pr_c", 20, 13, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 320
    											, hashmapSize
    											);
        rebecs.push_back(_ref_pr_c);
        referenceToRebecs->push_back(_ref_pr_c);
    	SubscribeInvokerActor* _ref_si_c = new SubscribeInvokerActor(maxRebecID++, "si_c", 20, 13, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 316
    											, hashmapSize
    											);
        rebecs.push_back(_ref_si_c);
        referenceToRebecs->push_back(_ref_si_c);
    	OximeterActor* _ref_o = new OximeterActor(maxRebecID++, "o", 20, 4, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 124
    											, hashmapSize
    											);
        rebecs.push_back(_ref_o);
        referenceToRebecs->push_back(_ref_o);
    	PublishRequesterActor* _ref_pr_o = new PublishRequesterActor(maxRebecID++, "pr_o", 20, 13, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 320
    											, hashmapSize
    											);
        rebecs.push_back(_ref_pr_o);
        referenceToRebecs->push_back(_ref_pr_o);
    	SubscribeInvokerActor* _ref_si_o = new SubscribeInvokerActor(maxRebecID++, "si_o", 20, 13, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 316
    											, hashmapSize
    											);
        rebecs.push_back(_ref_si_o);
        referenceToRebecs->push_back(_ref_si_o);
    	CommunicationSubstrateActor* _ref_cs = new CommunicationSubstrateActor(maxRebecID++, "cs", 20, 17, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 384
    											, hashmapSize
    											);
        rebecs.push_back(_ref_cs);
        referenceToRebecs->push_back(_ref_cs);
    	MonitoringAppActor* _ref_ma = new MonitoringAppActor(maxRebecID++, "ma", 20, 9, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 232
    											, hashmapSize
    											);
        rebecs.push_back(_ref_ma);
        referenceToRebecs->push_back(_ref_ma);
    	RequestRequesterActor* _ref_rr = new RequestRequesterActor(maxRebecID++, "rr", 20, 9, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 236
    											, hashmapSize
    											);
        rebecs.push_back(_ref_rr);
        referenceToRebecs->push_back(_ref_rr);
    	ResponderInvokerActor* _ref_ri = new ResponderInvokerActor(maxRebecID++, "ri", 20, 9, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 236
    											, hashmapSize
    											);
        rebecs.push_back(_ref_ri);
        referenceToRebecs->push_back(_ref_ri);
    	PumpActor* _ref_p = new PumpActor(maxRebecID++, "p", 20, 5, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 144
    											, hashmapSize
    											);
        rebecs.push_back(_ref_p);
        referenceToRebecs->push_back(_ref_p);

    	_ref_c->known = new byte[MAX_KNOWN];
    	_ref_c->known[0] = _ref_c->myID;
 _ref_c->known[1] = _ref_pr_c->myID;

		_ref_c->constructorCapnometer(INITIAL_SENDER
	        , 0
		);
    	_ref_pr_c->known = new byte[MAX_KNOWN];
    	_ref_pr_c->known[0] = _ref_pr_c->myID;
 _ref_pr_c->known[1] = _ref_cs->myID;

		_ref_pr_c->constructorPublishRequester(INITIAL_SENDER
	        , 1
	        , 0
	        , 2
		);
    	_ref_si_c->known = new byte[MAX_KNOWN];
    	_ref_si_c->known[0] = _ref_si_c->myID;

		_ref_si_c->constructorSubscribeInvoker(INITIAL_SENDER
	        , 2
	        , 10
		);
    	_ref_o->known = new byte[MAX_KNOWN];
    	_ref_o->known[0] = _ref_o->myID;
 _ref_o->known[1] = _ref_pr_o->myID;

		_ref_o->constructorOximeter(INITIAL_SENDER
	        , 5
		);
    	_ref_pr_o->known = new byte[MAX_KNOWN];
    	_ref_pr_o->known[0] = _ref_pr_o->myID;
 _ref_pr_o->known[1] = _ref_cs->myID;

		_ref_pr_o->constructorPublishRequester(INITIAL_SENDER
	        , 6
	        , 5
	        , 7
		);
    	_ref_si_o->known = new byte[MAX_KNOWN];
    	_ref_si_o->known[0] = _ref_si_o->myID;

		_ref_si_o->constructorSubscribeInvoker(INITIAL_SENDER
	        , 7
	        , 10
		);
    	_ref_cs->known = new byte[MAX_KNOWN];
    	_ref_cs->known[0] = _ref_cs->myID;

		_ref_cs->constructorCommunicationSubstrate(INITIAL_SENDER
	        , 12
		);
    	_ref_ma->known = new byte[MAX_KNOWN];
    	_ref_ma->known[0] = _ref_ma->myID;
 _ref_ma->known[1] = _ref_si_c->myID;
 _ref_ma->known[2] = _ref_si_o->myID;
 _ref_ma->known[3] = _ref_rr->myID;

		_ref_ma->constructorMonitoringApp(INITIAL_SENDER
	        , 10
		);
    	_ref_rr->known = new byte[MAX_KNOWN];
    	_ref_rr->known[0] = _ref_rr->myID;
 _ref_rr->known[1] = _ref_cs->myID;

		_ref_rr->constructorRequestRequester(INITIAL_SENDER
	        , 11
	        , 10
	        , 13
		);
    	_ref_ri->known = new byte[MAX_KNOWN];
    	_ref_ri->known[0] = _ref_ri->myID;
 _ref_ri->known[1] = _ref_cs->myID;

		_ref_ri->constructorResponderInvoker(INITIAL_SENDER
	        , 13
	        , 14
	        , 11
		);
    	_ref_p->known = new byte[MAX_KNOWN];
    	_ref_p->known[0] = _ref_p->myID;
 _ref_p->known[1] = _ref_ri->myID;

		_ref_p->constructorPump(INITIAL_SENDER
	        , 14
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