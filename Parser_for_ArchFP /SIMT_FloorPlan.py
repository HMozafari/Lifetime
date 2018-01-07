# [4096			6			64				64			32			16				1 			1 				0 				0 			0 				0 				0 				0 				0 		]
# [L2$_Size		MCores		MCD$_Size		MCHWTCs		MCLanes		MCI$Size		SpLane 		ShSLane 		CMCores 		HCores 		HCD$_Size 		HCHWTCs 		HCLanes 		HCI$_Size 		HMCore 	]

import sys, getopt

def main(argv):
   assignFolderAddr = ''
   outputfile = ''
   try:
      opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
   except getopt.GetoptError:
      print 'SIMT_Floorplan.py -i 4096 6 64 64 32	16	1 1	0 0 0 0	0 0 0 -o SIMT_6_MCs_3_HCs_2RLanes'
      print 'were these numbers indiacte [L2$_Size		MCores		MCD$_Size		MCHWTCs		MCLanes		MCI$Size		SpLane 		ShSLane 		CMCores 		HCores 		HCD$_Size 		HCHWTCs 		HCLanes 		HCI$_Size 		HMCore 	]'
      sys.exit(2)

   print opts
   print args	
   for opt, arg in opts:
      if opt == '-h':
        print 'SIMT_Floorplan.py -i 4096 6 64 64 32	16	1 1	0 0 0 0	0 0 0 -o SIMT_6_MCs_3_HCs_2RLanes'
        print 'were these numbers indiacte [L2$_Size		MCores		MCD$_Size		MCHWTCs		MCLanes		MCI$Size		SpLane 		ShSLane 		CMCores 		HCores 		HCD$_Size 		HCHWTCs 		HCLanes 		HCI$_Size 		HMCore 	]'
        sys.exit()
      elif opt in ("-i", "--ifold"):
      	SIMT_Spec_Args = arg
      	for SIMT_Specific in SIMT_Spec_Args:
        	print SIMT_Specific
      elif opt in ("-o", "--ofile"):
       		outputfile = arg
   print 'Input folder address is "', assignFolderAddr
   print 'Output file''s name is:', outputfile

   
   #searchInFolder("../TheWholeClassAssignment2Files/")


if __name__ == "__main__":
   main(sys.argv[1:])

