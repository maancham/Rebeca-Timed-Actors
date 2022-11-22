#include "AbstractTimedRebecaAnalyzer.h"

		#define _ref_c (dynamic_cast<ClientActor*>(rebecs[0]))
		#define _ref_sr (dynamic_cast<SendRequesterActor*>(rebecs[1]))
		#define _ref_cs (dynamic_cast<CommunicationSubstrateActor*>(rebecs[2]))
		#define _ref_ri (dynamic_cast<ReceiveInvokerActor*>(rebecs[3]))
		#define _ref_s (dynamic_cast<ServiceActor*>(rebecs[4]))
	
	

		#undef _ref_c
		#undef _ref_sr
		#undef _ref_cs
		#undef _ref_ri
		#undef _ref_s

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
    	ClientActor* _ref_c = new ClientActor(maxRebecID++, "c", 20, 4, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 124
    											, hashmapSize
    											);
        rebecs.push_back(_ref_c);
        referenceToRebecs->push_back(_ref_c);
    	SendRequesterActor* _ref_sr = new SendRequesterActor(maxRebecID++, "sr", 20, 4, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 137
    											, hashmapSize
    											);
        rebecs.push_back(_ref_sr);
        referenceToRebecs->push_back(_ref_sr);
    	CommunicationSubstrateActor* _ref_cs = new CommunicationSubstrateActor(maxRebecID++, "cs", 20, 4, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 120
    											, hashmapSize
    											);
        rebecs.push_back(_ref_cs);
        referenceToRebecs->push_back(_ref_cs);
    	ReceiveInvokerActor* _ref_ri = new ReceiveInvokerActor(maxRebecID++, "ri", 20, 4, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 137
    											, hashmapSize
    											);
        rebecs.push_back(_ref_ri);
        referenceToRebecs->push_back(_ref_ri);
    	ServiceActor* _ref_s = new ServiceActor(maxRebecID++, "s", 20, 4, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 120
    											, hashmapSize
    											);
        rebecs.push_back(_ref_s);
        referenceToRebecs->push_back(_ref_s);

    	_ref_c->known = new byte[MAX_KNOWN];
    	_ref_c->known[0] = _ref_c->myID;
 _ref_c->known[1] = _ref_sr->myID;

		_ref_c->constructorClient(INITIAL_SENDER
		);
    	_ref_sr->known = new byte[MAX_KNOWN];
    	_ref_sr->known[0] = _ref_sr->myID;
 _ref_sr->known[1] = _ref_cs->myID;
 _ref_sr->known[2] = _ref_c->myID;

		_ref_sr->constructorSendRequester(INITIAL_SENDER
		);
    	_ref_cs->known = new byte[MAX_KNOWN];
    	_ref_cs->known[0] = _ref_cs->myID;
 _ref_cs->known[1] = _ref_sr->myID;
 _ref_cs->known[2] = _ref_ri->myID;

		_ref_cs->constructorCommunicationSubstrate(INITIAL_SENDER
		);
    	_ref_ri->known = new byte[MAX_KNOWN];
    	_ref_ri->known[0] = _ref_ri->myID;
 _ref_ri->known[1] = _ref_cs->myID;
 _ref_ri->known[2] = _ref_s->myID;

		_ref_ri->constructorReceiveInvoker(INITIAL_SENDER
		);
    	_ref_s->known = new byte[MAX_KNOWN];
    	_ref_s->known[0] = _ref_s->myID;
 _ref_s->known[1] = _ref_ri->myID;

		_ref_s->constructorService(INITIAL_SENDER
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