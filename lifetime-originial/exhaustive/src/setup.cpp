#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>

#include "ComponentLibrary.h"

//#define EXEC "/afs/ece/usr/bhm/Public/reliability/exhaustive_ra/exhaustive"
//#define BM "/afs/ece/usr/bhm/Public/reliability/benchmarks/mpeg_cpl2_nd"
#define EXEC "./exhaustive"
#define BM "../benchmarks/mpeg_cpl2_nd"

#define PS "-s"

#define FP_ITER 100
#define SAMPLES 10000

#define RC 29.775

void usage(int argc, char* argv[]) {
    cout << "Usage " << argv[0] << " <Y=0/1> <arg 1> <arg 2> <# s> [<min permutation> <max "
	 << "permutation> <exec slack> <stor slack>]" << endl;
} // usage

componentType alloc(componentType c) {
    switch(c) {
    case M3:
	return ARM9;
    case ARM9:
	return ARM11;
    case ARM11:
	return MEM64KB;
    case MEM64KB:
	return MEM96KB;
    case MEM96KB:
	return MEM128KB;
    case MEM128KB:
	return MEM192KB;
    case MEM192KB:
	return MEM256KB;
    case MEM256KB:
	return MEM384KB;
    case MEM384KB:
	return MEM512KB;
    case MEM512KB:
	return MEM1MB;
    case MEM1MB:
	return MEM2MB;
    case MEM2MB:
	return SW3X3;
    default:
	cerr << "*** Invalid type specified for redundancy allocation" << endl;
	exit(1);
    } // switch

} // alloc

int main(int argc, char* argv[]) {

    // check for command line arguments
    
    if (!(argc == 5 || argc == 7 || argc == 9)) {
	usage(argc, argv);
	exit(1);
    } // if

    int yield = atoi(argv[1]);

    int updateT = 0;
    int initT = 0;
    float ddp = 0;
    float ddm = 0;

    if (yield == 0) {
	updateT = atoi(argv[2]);
	initT = atoi(argv[3]);
    } else {
	ddp = strtof(argv[2], NULL);
	ddm = strtof(argv[3], NULL);
    }
	
    int cfg = atoi(argv[4]);
    
    int es = 0;
    int ss = 0;

    int minp=1;
    int maxp=1612431360;

    if (argc > 5) {
	minp = atoi(argv[5]);
	maxp = atoi(argv[6]);
	cout << "*** MIN = " << minp << " / MAX = " << maxp << endl;
    }
    
    if (argc == 9) {
	es = atoi(argv[7]);
	ss = atoi(argv[8]);
	cout << "*** ES = " << es << " / SS = " << ss << endl;
    }

    int np = 21;
    int nm = 5;
    
    // begin building the command line
    stringstream setup;
    setup << EXEC;

    if (yield == 0)
	setup << " -u " << updateT << " -i " << initT << " ";
    else
	setup << " -y " << ddp << " " << ddm << " -u 0 -i 0 " ;

    setup << "-a 1 " << "-f " << FP_ITER << " -h " << RC << " "
	  << "-s " << SAMPLES << " "
	  << "-t " << BM << "/mpeg_cpl2_nd.tg "
	  << "-n " << BM << "/fp/" << cfg << PS << ".nets "
	  << "-c " << BM << "/config/" << cfg << PS << ".cfg ";
    
    // default min and max components for each node
    componentType bspvolp_i = M3;
        componentType bspvolp_m = ARM11;
    componentType vlda_i = ARM11;
        componentType vlda_m = ARM11;
    componentType vldb_i = ARM11;
        componentType vldb_m = ARM11;
    componentType shpe_i = M3;
        componentType shpe_m = ARM11;
    componentType motna_i = ARM9;
        componentType motna_m = ARM11;
    componentType motnb_i = ARM9;
        componentType motnb_m = ARM11;
    componentType motnc_i = ARM9;
        componentType motnc_m = ARM11;
    componentType motnd_i = M3;
        componentType motnd_m = ARM11;
    componentType txtra_i = ARM9;
        componentType txtra_m = ARM11;
    componentType txtrb_i = ARM9;
        componentType txtrb_m = ARM11;
    componentType txtrc_i = ARM9;
        componentType txtrc_m = ARM11;
    componentType txtrd_i = ARM9;
        componentType txtrd_m = ARM11;
    componentType txtre_i = ARM9;
        componentType txtre_m = ARM11;
    componentType txtrf_i = ARM9;
        componentType txtrf_m = ARM11;
    componentType rcns_i = ARM11;
        componentType rcns_m = ARM11;
    componentType pada_i = M3;
        componentType pada_m = ARM11;
    componentType padb_i = M3;
        componentType padb_m = ARM11;
    componentType padc_i = M3;
        componentType padc_m = ARM11;
    componentType padd_i = M3;
        componentType padd_m = ARM11;
    componentType pade_i = M3;
        componentType pade_m = ARM11;
    componentType padf_i = M3;
        componentType padf_m = ARM11;

    componentType vbv_i = MEM192KB;
        componentType vbv_m = MEM512KB;
    componentType vcv1_i = MEM128KB;
        componentType vcv1_m = MEM512KB;
    componentType vcv2_i = MEM192KB;
        componentType vcv2_m = MEM512KB;
    componentType vcv3_i = MEM384KB;
        componentType vcv3_m = MEM512KB;
    componentType vmv_i = MEM1MB;
        componentType vmv_m = MEM1MB;

    if (es == 0) {
	bspvolp_m = bspvolp_i;
	shpe_m = shpe_i;
	motna_m = motna_i;
	motnb_m = motnb_i;
	motnc_m = motnc_i;
	motnd_m = motnd_i;
	txtra_m = txtra_i;
	txtrb_m = txtrb_i;
	txtrc_m = txtrc_i;
	txtrd_m = txtrd_i;
	txtre_m = txtre_i;
	txtrf_m = txtrf_i;
	pada_m = pada_i;
	padb_m = padb_i;
	padc_m = padc_i;
	padd_m = padd_i;
	pade_m = pade_i;
	padf_m = padf_i;
    }

    if (ss == 0) {
	vbv_m = vbv_i;
	vcv1_m = vcv1_i;
	vcv2_m = vcv2_i;
	vcv3_m = vcv3_i;
    }
    
    int count = 0;

    int vcv3slack = 0;
    int vcv2slack = 0;
    int vcv1slack = 0;
    int vbvslack = 0;

    int padfslack = 0;
    int padeslack = 0;
    int paddslack = 0;
    int padcslack = 0;
    int padbslack = 0;
    int padaslack = 0;

    int txtrfslack = 0;
    int txtreslack = 0;
    int txtrdslack = 0;
    int txtrcslack = 0;
    int txtrbslack = 0;
    int txtraslack = 0;

    int motndslack = 0;
    int motnbslack = 0;
    int motncslack = 0;
    int motnaslack = 0;

    int shpeslack = 0;
    int bspvolpslack = 0;

    for (componentType vmv=vmv_i; vmv<=vmv_m; vmv=alloc(vmv)) {

    for (componentType vcv3=vcv3_i; vcv3<=vcv3_m; vcv3=alloc(vcv3)) {
	vcv3slack = componentTypeToCapacity(vcv3) - componentTypeToCapacity(vcv3_i);
	
    for (componentType vcv2=vcv2_i; vcv2<=vcv2_m; vcv2=alloc(vcv2)) {
	vcv2slack = componentTypeToCapacity(vcv2) - componentTypeToCapacity(vcv2_i);

    for (componentType vcv1=vcv1_i; vcv1<=vcv1_m; vcv1=alloc(vcv1)) {
	vcv1slack = componentTypeToCapacity(vcv1) - componentTypeToCapacity(vcv1_i);
	
    for (componentType vbv=vbv_i; vbv<=vbv_m; vbv=alloc(vbv)) {
	vbvslack = componentTypeToCapacity(vbv) - componentTypeToCapacity(vbv_i);
 
    for (componentType padf=padf_i; padf<=padf_m; padf=alloc(padf)) {
	padfslack = componentTypeToCapacity(padf) - componentTypeToCapacity(padf_i);

    for (componentType pade=pade_i; pade<=pade_m; pade=alloc(pade)) {
	padeslack = componentTypeToCapacity(pade) - componentTypeToCapacity(pade_i);

    for (componentType padd=padd_i; padd<=padd_m; padd=alloc(padd)) {
	paddslack = componentTypeToCapacity(padd) - componentTypeToCapacity(padd_i);

    for (componentType padc=padc_i; padc<=padc_m; padc=alloc(padc)) {
	padcslack = componentTypeToCapacity(padc) - componentTypeToCapacity(padc_i);

    for (componentType padb=padb_i; padb<=padb_m; padb=alloc(padb)) {
	padbslack = componentTypeToCapacity(padb) - componentTypeToCapacity(padb_i);

    for (componentType pada=pada_i; pada<=pada_m; pada=alloc(pada)) {
	padaslack = componentTypeToCapacity(pada) - componentTypeToCapacity(pada_i);

    for (componentType rcns=rcns_i; rcns<=rcns_m; rcns=alloc(rcns)) {

    for (componentType txtrf=txtrf_i; txtrf<=txtrf_m; txtrf=alloc(txtrf)) {
	txtrfslack = componentTypeToCapacity(txtrf) - componentTypeToCapacity(txtrf_i);

    for (componentType txtre=txtre_i; txtre<=txtre_m; txtre=alloc(txtre)) {
	txtreslack = componentTypeToCapacity(txtre) - componentTypeToCapacity(txtre_i);

    for (componentType txtrd=txtrd_i; txtrd<=txtrd_m; txtrd=alloc(txtrd)) {
	txtrdslack = componentTypeToCapacity(txtrd) - componentTypeToCapacity(txtrd_i);

    for (componentType txtrc=txtrc_i; txtrc<=txtrb_m; txtrc=alloc(txtrc)) {
	txtrcslack = componentTypeToCapacity(txtrc) - componentTypeToCapacity(txtrc_i);

    for (componentType txtrb=txtrb_i; txtrb<=txtrc_m; txtrb=alloc(txtrb)) {
	txtrbslack = componentTypeToCapacity(txtrb) - componentTypeToCapacity(txtrb_i);

    for (componentType txtra=txtra_i; txtra<=txtra_m; txtra=alloc(txtra)) {
	txtraslack = componentTypeToCapacity(txtra) - componentTypeToCapacity(txtra_i);

    for (componentType motnd=motnd_i; motnd<=motnd_m; motnd=alloc(motnd)) {
	motndslack = componentTypeToCapacity(motnd) - componentTypeToCapacity(motnd_i);

    for (componentType motnc=motnc_i; motnc<=motnc_m; motnc=alloc(motnc)) {
	motncslack = componentTypeToCapacity(motnc) - componentTypeToCapacity(motnc_i);

    for (componentType motnb=motnb_i; motnb<=motnb_m; motnb=alloc(motnb)) {
	motnbslack = componentTypeToCapacity(motnb) - componentTypeToCapacity(motnb_i);

    for (componentType motna=motna_i; motna<=motna_m; motna=alloc(motna)) {
	motnaslack = componentTypeToCapacity(motna) - componentTypeToCapacity(motna_i);

    for (componentType shpe=shpe_i; shpe<=shpe_m; shpe=alloc(shpe)) {
	shpeslack = componentTypeToCapacity(shpe) - componentTypeToCapacity(shpe_i);

    for (componentType vldb=vldb_i; vldb<=vldb_m; vldb=alloc(vldb)) {

    for (componentType vlda=vlda_i; vlda<=vlda_m; vlda=alloc(vlda)) {

    for (componentType bspvolp=bspvolp_i; bspvolp<=bspvolp_m; bspvolp=alloc(bspvolp)) {
	bspvolpslack = componentTypeToCapacity(bspvolp) - componentTypeToCapacity(bspvolp_i);

	int eslack = padfslack+padeslack+paddslack+padcslack+padbslack+padaslack+
	    txtrfslack+txtreslack+txtrdslack+txtrcslack+txtrbslack+txtraslack+
	    motndslack+motncslack+motnbslack+motnaslack+
	    shpeslack+bspvolpslack;

	int sslack = vcv3slack+vcv2slack+vcv1slack+vbvslack;

	if (eslack == es && sslack == ss) {
	    count++;

	    if (count >= minp && count <= maxp) {
		//cout << padfslack << " " << padeslack << " " << paddslack << " "
		//	 << padcslack << " " << padbslack << " " << padaslack << " "
		//	 << txtrfslack << " " << txtreslack << " " << txtrdslack << " "
		//	 << txtrcslack << " " << txtrbslack << " " << txtraslack << " "
		//	 << motndslack << " " << motncslack << " " << motnbslack << " "
		//	 << motnaslack << " "
		//	 << shpeslack << " " << bspvolpslack
		//	 << " / " << eslack << endl;
		
		//cout << vcv3slack << " " << vcv2slack << " " << vcv1slack << " " << vbvslack << " / " << sslack << endl;
		
		stringstream ra;
		ra << "-p " << np << " "
		   << componentTypeToString(bspvolp) << " "
		   << componentTypeToString(vlda) << " "
		   << componentTypeToString(vldb) << " "
		   << componentTypeToString(shpe) << " "
		   << componentTypeToString(motna) << " "
		   << componentTypeToString(motnb) << " "
		   << componentTypeToString(motnc) << " "
		   << componentTypeToString(motnd) << " "
		   << componentTypeToString(txtra) << " "
		   << componentTypeToString(txtrb) << " "
		   << componentTypeToString(txtrc) << " "
		   << componentTypeToString(txtrd) << " "
		   << componentTypeToString(txtre) << " "
		   << componentTypeToString(txtrf) << " "
		   << componentTypeToString(rcns) << " "
		   << componentTypeToString(pada) << " "
		   << componentTypeToString(padb) << " "
		   << componentTypeToString(padc) << " "
		   << componentTypeToString(padd) << " "
		   << componentTypeToString(pade) << " "
		   << componentTypeToString(padf) << " ";
		
		ra << "-m " << nm << " "
		   << componentTypeToString(vbv) << " "
		   << componentTypeToString(vcv1) << " "
		   << componentTypeToString(vcv2) << " "
		   << componentTypeToString(vcv3) << " "
		   << componentTypeToString(vmv) << " ";
		
		// combine static and dynamic portions of the command line
		string cmd = setup.str();
		cmd.append(ra.str());
		
		// execute
		cout << cmd << endl;
		//system(cmd.c_str());
	    } // if
	} // if
	
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    
    cout << "*** " << count << endl;
} // main
    
