/* Copyright 2008 Carnegie Mellon University */

#include <gsl/gsl_rng.h>
#include <iomanip>
#include <iostream>
#include <vector>

#include "config.h"
#include "Component.h"
#include "ComponentLibrary.h"
#include "mcs.h"

#define EVAL

using namespace std;

void initializeProcessorRedundancies(System *sys, vector<componentType> types) {
    // sanity check
    // ensure the number of components specified matches the number of processors in the system
    if (types.size() != sys->getProcessors().size()) {
	cerr << "*** Error: processor count mismatch in initializeProcessorRedundancies" << endl;
	sys->cleanUpAndExit(1);
    } // if

    vector<Component*> processors = sys->getProcessors();
    vector<Component*>::iterator piter = processors.begin();
    vector<componentType>::iterator titer = types.begin();
    while (piter != processors.end()) {
	Component *c = *piter;
	componentType t = *titer;

	// set component type
	c->setComponentType(t);

	// validate that this component can support its tasks
	if (!c->validateInitialMapping()) {
	    cerr << "*** Error: component " << c->getName() << " no longer adequate for its initial tasks" << endl;
	    sys->cleanUpAndExit(1);
	} 
	
	piter++;
	titer++;
    } // while
} // initializeProcessorRedundancies

void initializeMemoryRedundancies(System *sys, vector<componentType> types) {
    // sanity check
    // ensure the number of components specified matches the number of memories in the system
    if (types.size() != sys->getMemories().size()) {
	cerr << "*** Error: processor count mismatch in initializeMemoriesRedundancies" << endl;
	sys->cleanUpAndExit(1);
    } // if

    vector<Component*> memories = sys->getMemories();
    vector<Component*>::iterator miter = memories.begin();
    vector<componentType>::iterator titer = types.begin();
    while (miter != memories.end()) {
	Component *c = *miter;
	componentType t = *titer;

	// set component type
	c->setComponentType(t);

	// validate that this component can support its tasks
	if (!c->validateInitialMapping()) {
	    cerr << "*** Error: component " << c->getName() << " no longer adequate for its initial tasks" << endl;
	    sys->cleanUpAndExit(1);
	} 
	
	miter++;
	titer++;
    } // while
} // initializeMemoryRedundancies

bool permuteRedundancies(System *sys, vector<Component*> components) {
    // push redundancy from the front of the list toward the back of the list, 
    // starting from the last redundant component in the list
    
    int deallocCount = 0;
    bool done = false;
    vector<Component*>::reverse_iterator iter = components.rbegin();
		
    while (!done && iter != components.rend()) {
	// starting from the back of the list, find a redundant component
	Component *redundantC = *iter;
		
	if (redundantC->isRedundant()) {
	    // is this the last component?
	    if (iter != components.rbegin()) {
		// is the next component in the list (toward the back) not redundant?
		Component *nextC = *(iter-1);
		if (!nextC->isRedundant()) {
		    // deallocate redundantC, allocate nextC
		    sys->deallocateRedundancy(redundantC);
		    sys->allocateRedundancy(nextC);
					
		    // deallocate all trailing redundancy (after nextC)
		    vector<Component*>::reverse_iterator deallocIter = components.rbegin();
		    while (deallocIter < iter-1) {
			Component *trailing = *deallocIter;
			if (trailing->isRedundant()) {
			    sys->deallocateRedundancy(trailing);
			    deallocCount++;
			} // if
						
			deallocIter++;
		    } // while
					
		    // reallocate all trailing redundancy (after nextC) to immediately follow nextC
		    while (deallocCount > 0) {
			Component *trailing = *(iter-1-deallocCount);
			sys->allocateRedundancy(trailing);
						
			deallocCount--;
		    } // while
					
		    done = true;
		} // if nextC not redundant
	    } // if redundantC is last component
	} // if redundantC redundant
		
	iter++;
    } // while
	
    return done;
} // permuteRedundancies

void printComponents(vector<Component*> components) {
    vector<Component*>::iterator iter = components.begin();
    while (iter != components.end()) {
	Component *c = *iter;
	cout << c->getName() << " ";
		
	iter++;
    } // while
} // printComponents

void printProcessorRedundancies(vector<Component*> processors) {
    vector<Component*>::iterator iter = processors.begin();
    while (iter != processors.end()) {
	Component *c = *iter;		
	//cout << (c->isRedundant() ? 1 : 0) << " ";
	cout << c->getInitialCapacity() << " ";
	
	iter++;
    } // while
} // printProcessorRedundancies

void printMemoryRedundancies(vector<Component*> memories) {
    vector<Component*>::iterator iter = memories.begin();
    while (iter != memories.end()) {
	Component *c = *iter;
	cout << c->getInitialCapacity() << " ";
	
	iter++;
    } // while
} // printMemoryRedundancies

inline int fastfactorial(int n) {
    if (n > 1)
	return n*factorial(n-1);
    else
	return 1;
} // factorial

int choose(int n, int k) {
    return fastfactorial(n) / (fastfactorial(k) * (fastfactorial(n - k)));
} // choose

void parse_cmd_exhaustive(int argc, char *argv[], System &sys) {

    for (int arg=1; arg<argc; arg++) {
	// -p specifies the number of redundant processors
	if (!strncmp("-p", argv[arg], 2)) {
	    int n_proc = atoi(argv[arg+1]);
	    vector<componentType> types;
	    
	    for (int i=0;i<n_proc;i++) {
		string str = argv[arg+2+i];

		//cout << i << " " << str << endl;
		
		componentType ctype = stringToComponentType(str);
		if ( !(ctype >= M3 && ctype <= ARM11) ) {
		    cerr << "*** Error: invalid processor type passed in -p: >" << str << "< (" << ctype << ")" << endl;
		    sys.cleanUpAndExit(1);
		} // if
		
		types.push_back(stringToComponentType(str));
	    } // for
	    
	    initializeProcessorRedundancies(&sys, types);
	    arg += 1 + n_proc;
	} // if

	// -m begins the specification of memory sizes
	if (!strncmp("-m", argv[arg], 2)) {
	    int n_mem = atoi(argv[arg+1]);
	    vector<componentType> types;
	    
	    for (int i=0;i<n_mem;i++) {
		string str = argv[arg+2+i];

		//cout << i << " " << str << endl;
		
		componentType ctype = stringToComponentType(str);
		if ( !(ctype >= MEM64KB && ctype <= MEM2MB) ) {
		    cerr << "*** Error: invalid processor type passed in -m: >" << str << "< (" << ctype << ")" << endl;
		    sys.cleanUpAndExit(1);
		} // if
		
		types.push_back(stringToComponentType(str));
	    } // for
	    
	    initializeMemoryRedundancies(&sys, types);
	    arg += 1 + n_mem;
	} // if
    } // for

} // parse_cmd_exhaustive

int main(int argc, char* argv[]) {
    System sys;
	
    // Initialize the component library
    initializeComponentLibrary(sys.getComponentLibrary());
	
    // parse input arguments
    parse_cmd_sys(argc, argv, sys);
    
    // initialize redundancies
    parse_cmd_exhaustive(argc, argv, sys);
		
    // print header for reference
    //printComponents(sys.getProcessors());
    //printComponents(sys.getMemories());
    //cout << "area wl mttf svar var conf" << endl;
	
    // build failure scenarios
    //sys.buildAllScenariosOrdered();
	
    // begin redundancy allocation
    //for (int pAlloc=0; pAlloc<choose(sys.getProcessors().size(), rProc); pAlloc++) {
    
#ifdef EVAL
    sys.samplingRun();
    
    float area = sys.getArea();
    float wl = sys.getWL();
    float mttf = sys.getMTTF();
    float var = sys.getVar();
    float s_var = sys.getSVar();
    float conf = sys.getConfidence();
#endif
    
    // print redundancy allocation and results
    printProcessorRedundancies(sys.getProcessors());
    printMemoryRedundancies(sys.getMemories());
#ifdef EVAL
    cout << setprecision(4) << area << " " << wl << " " << mttf << " " << s_var << " " << var << " " << conf << endl;
#else
    cout << endl;
#endif
    
    // change allocation of redundancy to processors
    //permuteRedundancies(&sys, sys.getProcessors());
    //} // for pAlloc

    // remove the working directory
    sys.removeWorkingDirectory();
} // main
