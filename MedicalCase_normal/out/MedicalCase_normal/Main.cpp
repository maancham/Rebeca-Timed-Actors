                                          







#include "Types.h"
#include "Config.h"

#include "CommandLineParser.h"

		#include "TimedModelChecker.h"

#include <signal.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdexcept>

using namespace std;

	 int _ref_CAP_ETCO2 = 0;
 
	 int _ref_CAP_RESPIRATORYRATE = 1;
 
	 int _ref_OX_SPO2 = 2;
 
	 int _ref_OX_PULSERATE = 3;
 
	 boolean _ref_ACTIVE = true;
 
	 boolean _ref_INACTIVE = false;
 
	 int _ref_NREQ = 2;
 
	 int _ref_LREQ = 25;
 
	 int _ref_RREQ = 15;
 
	 int _ref_NRES = 5;
 
	 int _ref_LRES = 5;
 
	 int _ref_RRES = 10;
 
	 int _ref_NPUB = 2;
 
	 int _ref_LPUB = 6;
 
	 int _ref_NSUB = 7;
 
	 int _ref_XSUB = 20;
 
	 int _ref_LSUB = 5;
 
	 int _ref_RPUB = 30;
 
	 int _ref_RSUB = 5;
 
	 int _ref_consumptionTolerance = 5;
 

		#ifdef EXPORT_STATE_SPACE
		ofstream statespace;
		#endif
		TimedModelChecker *analyzer;

string operator+(const string left, const int right) {
	return left + std::to_string((long long)right);
}
string operator+(const string left, const short right) {
	return left + std::to_string((long long)right);
}
string operator+(const string left, const float right) {
	return left + std::to_string((long double)right);
}
string operator+(const string left, const double right) {
	return left + std::to_string((long double)right);
}
string operator+(const string left, const byte right) {
	return left + std::to_string((long long)right);
}
string operator+(const string left, const boolean right) {
	return left + (right ? "true" : "false");
}

string operator+(const string left, const AbstractActor *right) {
	return left + right->getName();
}

void segmentationFaultHandler(int signum) {
	analyzer->segmentationFault();
	exit(0);
}

#ifdef PROGRESS_REPORT
#include <thread> 
#include <chrono> 
ostream *progressReportStream;
boolean continueReportProgress;
void progressReport() {
	while(continueReportProgress) {
		analyzer->progressReport(*progressReportStream);
	    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}
#endif

int BOOLEAN_SIZE, BYTE_SIZE, SHORT_SIZE, INT_SIZE, FLOAT_SIZE, DOUBLE_SIZE, REACTIVE_CLASS_SIZE;
int main(int argc, char* argv[]) {

	BOOLEAN_SIZE = sizeof(boolean);
	BYTE_SIZE = sizeof(byte);
	SHORT_SIZE = sizeof(short);
	INT_SIZE = sizeof(int);
	FLOAT_SIZE = sizeof(float);
	DOUBLE_SIZE = sizeof(double);
	REACTIVE_CLASS_SIZE = sizeof(void*);
	
	signal(SIGSEGV, segmentationFaultHandler);

	CommandLineParser parser;
	parser.addOption('h', "help", "Print this message.");
	parser.addOption('s', "hashmapSize", "Set the size of the hashmap to \"2^value\" (default size is 20 and the value should be bigger that 20).", "value");
	parser.addOption('o', "exportResult", "Exporting the model checking result in file \"fileName\".", "fileName");
	#ifdef PROGRESS_REPORT
		parser.addOption('g', "progress", "Report progress in file \"fileName\".", "fileName");
	#endif
	
	#ifdef MONTE_CARLO
	parser.addOption('e', "error", "Error value of the calculated results", "value");
	parser.addOption('d', "delta", "Confidence value of the calculated results", "fileName");
	#endif
	
	    #ifdef EXPORT_STATE_SPACE	
		parser.addOption('x', "exportStatespace", "Exporting the state space in file \"fileName\".", "fileName");
		#endif
	    
	try {
		parser.parse(argc, argv);
	} catch (Exception e) {
		cout << e.getMessage() << endl;
		parser.printHelp();
		exit(0);
	}
	if (parser.hasOption('h')) {
		parser.printHelp();
		exit(0);
	}
	
    int hashmapSize = 24;
	ostream *result = &cout;

	if (parser.hasOption('s')) {
		int newSize = atoi(parser.getOptionValue('s').c_str());
		hashmapSize = newSize > hashmapSize ? newSize : hashmapSize;
	}
	if (parser.hasOption('o')) {
		result = new ofstream(parser.getOptionValue('o').c_str(), ofstream::out|ofstream::trunc);
	}
	#ifdef PROGRESS_REPORT
		if (parser.hasOption('g')) {
			progressReportStream = new ofstream(parser.getOptionValue('g').c_str(), ofstream::out|ofstream::trunc);
		}
	#endif

	for (list<string>::iterator it = parser.getEnvironmentVariables().begin(); it != parser.getEnvironmentVariables().end(); ++it) {
		string envVar = *it;
		int index = envVar.find("=");
		if (index == string::npos || index == 0 || index == envVar.length() - 1)
			continue;
		string name = envVar.substr(0, index);
		string value = envVar.substr(index + 1, envVar.length());
				
				if (name == "CAP_ETCO2")				
					try {
							_ref_CAP_ETCO2 = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"CAP_ETCO2\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"CAP_ETCO2\".-->" << endl;
					}

					
				
				if (name == "CAP_RESPIRATORYRATE")				
					try {
							_ref_CAP_RESPIRATORYRATE = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"CAP_RESPIRATORYRATE\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"CAP_RESPIRATORYRATE\".-->" << endl;
					}

					
				
				if (name == "OX_SPO2")				
					try {
							_ref_OX_SPO2 = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"OX_SPO2\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"OX_SPO2\".-->" << endl;
					}

					
				
				if (name == "OX_PULSERATE")				
					try {
							_ref_OX_PULSERATE = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"OX_PULSERATE\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"OX_PULSERATE\".-->" << endl;
					}

					
				
				if (name == "ACTIVE")				
					try {
							cout << "<!--Dynamic value assignment only supports numerical types not " <<
								"\"boolean\" type of variable" << 
								" \"ACTIVE\".-->" << endl;
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"ACTIVE\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"ACTIVE\".-->" << endl;
					}

					
				
				if (name == "INACTIVE")				
					try {
							cout << "<!--Dynamic value assignment only supports numerical types not " <<
								"\"boolean\" type of variable" << 
								" \"INACTIVE\".-->" << endl;
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"INACTIVE\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"INACTIVE\".-->" << endl;
					}

					
				
				if (name == "NREQ")				
					try {
							_ref_NREQ = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"NREQ\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"NREQ\".-->" << endl;
					}

					
				
				if (name == "LREQ")				
					try {
							_ref_LREQ = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"LREQ\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"LREQ\".-->" << endl;
					}

					
				
				if (name == "RREQ")				
					try {
							_ref_RREQ = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"RREQ\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"RREQ\".-->" << endl;
					}

					
				
				if (name == "NRES")				
					try {
							_ref_NRES = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"NRES\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"NRES\".-->" << endl;
					}

					
				
				if (name == "LRES")				
					try {
							_ref_LRES = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"LRES\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"LRES\".-->" << endl;
					}

					
				
				if (name == "RRES")				
					try {
							_ref_RRES = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"RRES\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"RRES\".-->" << endl;
					}

					
				
				if (name == "NPUB")				
					try {
							_ref_NPUB = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"NPUB\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"NPUB\".-->" << endl;
					}

					
				
				if (name == "LPUB")				
					try {
							_ref_LPUB = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"LPUB\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"LPUB\".-->" << endl;
					}

					
				
				if (name == "NSUB")				
					try {
							_ref_NSUB = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"NSUB\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"NSUB\".-->" << endl;
					}

					
				
				if (name == "XSUB")				
					try {
							_ref_XSUB = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"XSUB\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"XSUB\".-->" << endl;
					}

					
				
				if (name == "LSUB")				
					try {
							_ref_LSUB = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"LSUB\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"LSUB\".-->" << endl;
					}

					
				
				if (name == "RPUB")				
					try {
							_ref_RPUB = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"RPUB\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"RPUB\".-->" << endl;
					}

					
				
				if (name == "RSUB")				
					try {
							_ref_RSUB = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"RSUB\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"RSUB\".-->" << endl;
					}

					
				
				if (name == "consumptionTolerance")				
					try {
							_ref_consumptionTolerance = stoi(value);
					} catch(std::invalid_argument e) {
						cout << "<!--Invalid value for variable \"consumptionTolerance\".-->" << endl;
					} catch(std::out_of_range e) {
						cout << "<!--Value out of range for variable \"consumptionTolerance\".-->" << endl;
					}

					
    }
		#ifdef EXPORT_STATE_SPACE
		if (parser.hasOption('x')) {
			statespace.open(parser.getOptionValue('x').c_str(), ofstream::out|ofstream::trunc);
		} else {
			statespace.open("statespace.xml", ofstream::out|ofstream::trunc);
			#ifdef DEBUG_LEVEL_2 
			cout.rdbuf(statespace.rdbuf());
			#endif
			
		}
		#endif
		analyzer = new TimedModelChecker(*result
	    	#ifdef EXPORT_STATE_SPACE
	    		, statespace
			#endif
			, hashmapSize
			);
		#ifdef PROGRESS_REPORT
			continueReportProgress = true;
			std::thread t1(progressReport);
		#endif
		analyzer->doModelChecking("Deadlock-Freedom and No Deadline Missed", "Reachability");
		#ifdef PROGRESS_REPORT
			continueReportProgress = false;
			t1.join();
		#endif
	
    if (parser.hasOption('o')) {
        ((ofstream*)result)->close();
    }
    return 0;
}
