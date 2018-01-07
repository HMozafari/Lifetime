# [4096			6			64				64			32			16				1 			1 				0 				0 			0 				0 				0 				0 				0 		]
# [L2$_Size		MCores		MCD$_Size		MCHWTCs		MCLanes		MCI$Size		SpLane 		ShSLane 		CMCores 		HCores 		HCD$_Size 		HCHWTCs 		HCLanes 		HCI$_Size 		HMCore 	]

import sys, getopt
import os
import numpy as np

def find_between( s, first, last ):
    try:
        start = s.index( first ) + len( first ) -1
        end = s.index( last, start )
        return s[start:end]
    except ValueError:
        return ""

def main(argv):
    configFileAddr =''
    configNum =''
    application=''
    outTempFoldrAddr=''
    L2 = ''
    MCores =''
    MC_D =''
    MC_HWTC = ''
    MC_Lane=''
    MC_I=''
    numSp_Lane=''
    numShS_Lane=''
    numCMCore=''
    numHC=''
    numHC_D=''
    numHC_HWTC=''
    numHC_Lanes=''
    HC_I=''
    numHMCore=''

    assignFolderAddr = ''

    outputfile = ''
    try:
        opts, args = getopt.getopt(argv, "hc:n:a:o:", ["help", "configFileAddr=", "configNum=", "application=", "outTempFoldrAddr="])
    except getopt.GetoptError:
        print 'HotSpot-Parser.py -c ../configFileAddr -n configNum -a appName -o ../outTempFoldrAddr'

        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'HotSpot-Parser.py -c ../configFileAddr -n configNum -a appName -o ../outTempFoldrAddr'
            sys.exit()
        elif opt in ("-c", "--configFileAddr"):
            configFileAddr = arg
        elif opt in ("-n", "--configNum"):
            configNum = arg
        elif opt in ("-a", "--application"):
            application = arg
        elif opt in ("-o", "--outTempFoldrAddr"):
            outTempFoldrAddr = arg
    print 'Config File Address is ', configFileAddr
    print 'Config Number is ', configNum
    print 'Target Application is ', application
    print 'The Temp Address Folder is ', outTempFoldrAddr

    [L2, 	 numMCores,	 MC_D,	 MC_HWTC,	 numMC_Lanes,	 MC_I,	 numSp_Lane,	 numShS_Lane,	 numCMCore,	 numHC,	 numHC_D,	 numHC_HWTC, 	 numHC_Lanes,	 HC_I, 	 numHMCore] = retrunConfig(configFileAddr, configNum)

    header = powerTracesFileGenerator(2, 4, 0, 0, 0, 2, 2, 0)
    print header

def powerTracesFileGenerator(numMCores, numMC_Lanes, numSp_Lane,  numShS_Lane,	 numCMCore, numHC, numHC_Lanes, numHMCore):
    header = ''
    acc=1
    if (numHC):
        if (numShS_Lane):
            print numHC
        else:
            for i in range(1, (numMCores+numHC), 2):
                header += "HTC" + str(i) + "\t" + "I_Cache" + str(i) + "\t" + "Decoder" + str(i) + "\t"
                for j in range((i/2)*numMC_Lanes+1, ((i/2)*numMC_Lanes+1) + numMC_Lanes):
                    header += "Lane" + str(j) + "\t"
                header += "Write_Back" + str(acc) + "\t"
                acc+=1
                header += "HTC" + str(i+1) + "\t" + "I_Cache" + str(i+1) + "\t" + "Decoder" + str(i+1) + "\t"
                for j in range((i/2)*numHC_Lanes+1, ((i/2)*numHC_Lanes+1) + numHC_Lanes):
                    header += "Reg" + str(j) + "\t" + "ALU" + str(j) + "\t" + "D_Cache" + str(j) + "\t"
                header += "Write_Back" + str(acc) + "\t"
                acc += 1
    else:
        for i in range(1, numMCores+1):
            header += "HTC" + str(i) + "\t" + "I_Cache" + str(i) + "\t" + "Decoder" + str(i) + "\t"
            for j in range((i-1)*numMC_Lanes+1, (i)*numMC_Lanes+1):
                header += "Lane" + str(j) + "\t"
            header += "Write_Back" + str(i) + "\t"
    header += 'L2_Cache1'

    return header


def retrunConfig(configFileAddr, configNum):
    with open(configFileAddr) as f:
        lines = (line for line in f if not line.startswith('#'))
        configs = np.loadtxt(lines, delimiter='\t', skiprows=1, usecols=(0,1,2,3,4,5,6,7,8,10,11,12,13,14,15,16,17,18))

    return configs[configNum][2], configs[configNum][3], configs[configNum][4], configs[configNum][5], configs[configNum][6], configs[configNum][7], configs[configNum][8], configs[configNum][9], configs[configNum][10], configs[configNum][11], configs[configNum][12], configs[configNum][13], configs[configNum][14], configs[configNum][15], configs[configNum][16]
    ####### L2 	 MCores 	 MC_D$ 	 MC_HWTC 	 MC_Lane 	 MC_I$ 	 #Sp_Lane 	 #ShS_Lane 	 #CMCore 	 #HC 	 HC_D$ 	 HC_HWTC 	 HC_Lanes 	 HC_I$ 	 HMCore

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
