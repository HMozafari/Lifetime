# [4096			6			64				64			32			16				1 			1 				0 				0 			0 				0 				0 				0 				0 		]
# [L2$_Size		MCores		MCD$_Size		MCHWTCs		MCLanes		MCI$Size		SpLane 		ShSLane 		CMCores 		HCores 		HCD$_Size 		HCHWTCs 		HCLanes 		HCI$_Size 		HMCore 	]

import sys, getopt
import os

def find_between( s, first, last ):
    try:
        start = s.index( first ) + len( first ) -1
        end = s.index( last, start )
        return s[start:end]
    except ValueError:
        return ""

def main(argv):

    assignFolderAddr = ''
    outputfile = ''
    index = 0
    Arch_FP_MAIN_Temp_File_Addr = '../ArchFP/Main.cc'
    try:
        opts, args = getopt.getopt(argv, "hs:o:", ["spec=", "ofile="])
    except getopt.GetoptError:
        print 'SIMT_Floorplan.py -s 4096 6 64 64 32	16	1 0	0 0 0 0	0 0 0 -o SIMT_6_MCs_3_HCs_2RLanes'
        print 'were these numbers indiacte [L2$_Size		MCores		MCD$_Size		MCHWTCs		MCLanes		MCI$Size		SpLane 		ShSLane 		CMCores 		HCores 		HCD$_Size 		HCHWTCs 		HCLanes 		HCI$_Size 		HMCore 	]'
        sys.exit(2)

    #print argv
    #print opts
    #print args
    for opt in argv:
        index = index+1;
        if opt == '-h':
            print 'SIMT_Floorplan.py -i 4096 6 64 64 32	16	1 0	0 0 0 0	0 0 0 -o SIMT_6_MCs_3_HCs_2RLanes'
            print 'were these numbers indiacte [L2$_Size		MCores		MCD$_Size		MCHWTCs		MCLanes		MCI$Size		SpLane 		ShSLane 		CMCores 		HCores 		HCD$_Size 		HCHWTCs 		HCLanes 		HCI$_Size 		HMCore 	]'
            sys.exit()
        elif opt in ("-s", "--spec"):
            s=find_between(argv, '-s', '-o')
            [L2_Size, MCores, MC_DCache_Size, MCHWTCs, MCLanes, MC_ICache_Size, Spare_Lane, Shared_Spare_Lane, CMCores, HCores, HC_DCache_Size, HCHWTCs, HC_Lanes, HC_ICache_Size, HMCore]=s

            #print L2_Size
            #print s.split(' ')
            #print L2_Size
            #SIMT_Spec_Args = arg
            #for SIMT_Specific in SIMT_Spec_Args:
            #    print SIMT_Specific
        elif opt in ("-i", "--ifile"):
            Arch_FP_MAIN_Temp_File_Addr = argv[index]
        elif opt in ("-o", "--ofile"):
            outputfile_name = argv[index]
    #print 'Input folder address is "', assignFolderAddr
    #print 'Output file''s name is:', outputfile

    Orig_Cont_of_Arch_FP_MAIN_Temp = Arch_FP_MAIN_file_modifier(Arch_FP_MAIN_Temp_File_Addr, L2_Size, MCores, MC_DCache_Size, MCHWTCs, MCLanes, MC_ICache_Size, Spare_Lane, Shared_Spare_Lane, CMCores, HCores, HC_DCache_Size, HCHWTCs, HC_Lanes, HC_ICache_Size, HMCore, outputfile_name)

    #os.system('DIR=$(dirname '+ Arch_FP_MAIN_Temp_File_Addr +')')
    #os.system('echo $DIR')
    #subprocess.call("bash.sh", shell=True)


    DIR=os.path.dirname(Arch_FP_MAIN_Temp_File_Addr)
    os.chdir(DIR)
    os.system('make')
    os.system('./ArchFP')
    os.system('./hs2pdf ' + outputfile_name + '.flp')

    with open(Arch_FP_MAIN_Temp_File_Addr, 'w') as f:
        f.write(Orig_Cont_of_Arch_FP_MAIN_Temp)
        f.close



        # searchInFolder("../TheWholeClassAssignment2Files/")


def Arch_FP_MAIN_file_modifier(Arch_FP_MAIN_Temp_File_Addr, L2_Size, MCores, MC_DCache_Size, MCHWTCs, MCLanes, MC_ICache_Size, Spare_Lane, Shared_Spare_Lane, CMCores, HCores, HC_DCache_Size, HCHWTCs, HC_Lanes, HC_ICache_Size, HMCore, outputfile_name):
    with open(Arch_FP_MAIN_Temp_File_Addr) as f:
        s= f.read()
        f.close()

    Orig_Cont_of_Arch_FP_MAIN_Temp = s


    s=s.replace('int L2_Cache_Size                   = 1024;    //KB', 'int L2_Cache_Size                   = ' + L2_Size + ';   //KB')
    s=s.replace('int Num_of_MCores                   = 10;',  'int Num_of_MCores                    = ' + MCores + ';')
    s=s.replace('int MC_DCache_Size                  = 16;    //KB', 'int MC_DCache_Size                    = ' + MC_DCache_Size + ';   //KB')
    s=s.replace('int Num_of_MC_HWTCs                 = 32;', 'int Num_of_MC_HWTCs                   = ' + MCHWTCs + ';')
    s=s.replace('int Num_of_MCLanes                  = 32;', 'int Num_of_MCLanes                    = ' + MCLanes + ';')
    s=s.replace('int MC_ICache_Size                  = 16;    //KB','int MC_ICache_Size                     = ' + MC_ICache_Size + ';   //KB')

    s=s.replace('#define  Num_of_SpLane                    0','#define  Num_of_SpLane                   ' + Spare_Lane)
    s=s.replace('#define  Num_of_Sh_SLane                  2','#define  Num_of_Sh_SLane                 ' + Shared_Spare_Lane)
    s=s.replace('int Num_of_Cold_MCores              = 0;','int Num_of_Cold_MCores                     = ' + CMCores + ';  ')
    s=s.replace('#define Num_of_HCores                      0','#define Num_of_HCores                      ' + HCores)
    s=s.replace('int HC_DCache_Size                  = 16;      //KB','int HC_DCache_Size                  = ' + HC_DCache_Size + ';   //KB')
    s=s.replace('int Num_of_HCLanes                  = 2;'    ,'int Num_of_HCLanes                  =' + HC_Lanes + ';')
    s=s.replace('int HC_ICache_Size                  = 16;      //KB','int HC_ICache_Size                 = ' + HC_ICache_Size + ';   //KB')
    s=s.replace('int Num_of_HMCore                   = 2;     //KB','int Num_of_HMCore                   =' + HMCore + ';   //KB')

    s=s.replace('SIMT65.flp', outputfile_name+'.flp')

    with open(Arch_FP_MAIN_Temp_File_Addr, 'w') as f:
        f.write(s)
        f.close()

    return Orig_Cont_of_Arch_FP_MAIN_Temp


if __name__ == "__main__":
    main(sys.argv[1:])
