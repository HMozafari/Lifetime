/* -*- Mode: C ; indent-tabs-mode: nil ; c-file-style: "stroustrup" -*-

   Rapid Prototyping Floorplanner Project
   Author: Greg Faust

   File:   Main.c     Parse the command line arguments, and call the appropriate functions to do the work.

*/

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <stdexcept>
#include "MathUtil.hh"
#include "Floorplan.hh"

//static bool printNames = true;


void generateSIMT65nm ()
{


     int L2_Cache_Size                   = 1024;    //KB
     int Num_of_MCores                   = 10;
     int MC_DCache_Size                  = 16;      //KB 
     int Num_of_MC_HWTCs                 = 32;
     int Num_of_MCLanes                  = 32;
     int MC_ICache_Size                  = 16;      //KB
#define  Num_of_SpLane                    0
#define  Num_of_Sh_SLane                  2 
     int Num_of_Cold_MCores              = 0;
#define Num_of_HCores                      0
     int HC_DCache_Size                  = 16;      //KB
     int Num_of_HC_HWTCs                 = Num_of_MC_HWTCs; 
     int Num_of_HCLanes                  = 2;
     int HC_ICache_Size                  = 16;      //KB
     int Num_of_HMCore                   = 2; 
     







    // Areas from McPAT.
    double ALU_Area                     = 0.0915*20;
    double Reg_File_Area_per_Lane       = (0.9436/32)*20;
    //double Lane_Area                    = 0.1319;
    double Skeleton_Area_per_Lane       = (1.5801)*20;

    double D_Cache_Area_per_Lane_per_KB = (1.3901/32/16)*20;
    double L2$_Area_per_MB              = (46.0652/4)*20;
    double HTC_Area_per_HTC             = (Skeleton_Area_per_Lane/32)/4;
    double I_Cache_Area_per_KB          = ((Skeleton_Area_per_Lane)/4)/16;
    double Decoder_Area                 = (Skeleton_Area_per_Lane)/4;
    double Write_Back_Area              = (Skeleton_Area_per_Lane)/4;



/////////////////////////////////////////////////////////////////
// MCore sub components definitions

    geogLayout * MC_HTC = new geogLayout();
    MC_HTC->addComponentCluster("HTC", 1, HTC_Area_per_HTC * Num_of_MC_HWTCs, 300., 0.5, Top);


    geogLayout * MC_ICache = new geogLayout();
    MC_ICache->addComponentCluster("I_Cache", 1, I_Cache_Area_per_KB * MC_ICache_Size, 300., 0.5, Top);


    geogLayout * MC_Decoder = new geogLayout();
    MC_Decoder->addComponentCluster("Decoder", 1, Decoder_Area, 300., 0.5, Top);

    geogLayout * Reg = new geogLayout();
    Reg->addComponentCluster("Reg", 1, Reg_File_Area_per_Lane, 300, 0.5, Top);

    geogLayout * ALU = new geogLayout();
    ALU->addComponentCluster("ALU", 1, ALU_Area, 300, 0.5, Top);

    geogLayout * MC_DCache = new geogLayout();
    MC_DCache->addComponentCluster("D_Cache", 1, D_Cache_Area_per_Lane_per_KB, 300., 0.5, Top);

    geogLayout * MC_Lane = new geogLayout();
    MC_Lane->addComponentCluster("Lane", 1, D_Cache_Area_per_Lane_per_KB * HC_DCache_Size + ALU_Area + Reg_File_Area_per_Lane, 300., 0.5, Top);


    // Lane
    //geogLayout * MC_Lane = new geogLayout();
    //MC_Lane->addComponent(Reg, 1, Top);
    //MC_Lane->addComponent(ALU, 1, Center);
    //MC_Lane->addComponent(MC_DCache, 1, Bottom);


    geogLayout * MC_Write_Back = new geogLayout();
    MC_Write_Back->addComponentCluster("Write_Back", 1, Write_Back_Area, 300., 1., Top);


//////////////////////////////////////////////////////////////////

// MCore definition
    geogLayout * MCore = new geogLayout();
    MCore->addComponent(MC_HTC, 1, Top);
    MCore->addComponent(MC_ICache, 1, Top);
    MCore->addComponent(MC_Decoder, 1, Top);
    MCore->addComponent(MC_Lane, Num_of_MCLanes + Num_of_SpLane, Top);
    printf("Num_of_MCLanes + Num_of_SpLane= %d \n \n ", Num_of_MCLanes + Num_of_SpLane);
    MCore->addComponent(MC_Write_Back, 1, Bottom);


/////////////////////////////////////////////////////////////////


// MCore2 definition
    geogLayout * MCore2 = new geogLayout();
    MCore2->addComponent(MC_HTC, 1, Top);
    MCore2->addComponent(MC_ICache, 1, Top);
    MCore2->addComponent(MC_Decoder, 1, Top);
    MCore2->addComponent(MC_Lane, Num_of_MCLanes + Num_of_SpLane, Top);
    printf("Num_of_MCLanes + Num_of_SpLane= %d \n \n ", Num_of_MCLanes + Num_of_SpLane);
    MCore2->addComponent(MC_Write_Back, 1, Bottom);


/////////////////////////////////////////////////////////////////


// MCore sub components definitions

    geogLayout * HC_HTC = new geogLayout();
    HC_HTC->addComponentCluster("HTC", 1, HTC_Area_per_HTC * Num_of_HC_HWTCs, 300., 0.5, Top);


    geogLayout * HC_ICache = new geogLayout();
    HC_ICache->addComponentCluster("I_Cache", 1, I_Cache_Area_per_KB * HC_ICache_Size, 300., 0.5, Top);


    geogLayout * HC_Decoder = new geogLayout();
    HC_Decoder->addComponentCluster("Decoder", 1, Decoder_Area, 300., 0.5, Top);

    geogLayout * HC_DCache_per_Lane = new geogLayout();
    HC_DCache_per_Lane->addComponentCluster("D_Cache", 1, D_Cache_Area_per_Lane_per_KB * HC_DCache_Size, 300., 0.5, Top);

    // Lane
    geogLayout * HC_Lane = new geogLayout();
    HC_Lane->addComponent(Reg, 1, Top);
    HC_Lane->addComponent(ALU, 1, Top);
    HC_Lane->addComponent(HC_DCache_per_Lane, 1, Bottom);


    geogLayout * HC_Write_Back = new geogLayout();
    HC_Write_Back->addComponentCluster("Write_Back", 1, Write_Back_Area, 300., 0.5, Top);


//////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////


// MCore sub components definitions

geogLayout * Empty_Area1 = new geogLayout();
    Empty_Area1->addComponentCluster("Empty_Area1", 1, HTC_Area_per_HTC * Num_of_HC_HWTCs, 300., 0.5, Top);


    geogLayout * Empty_Area2 = new geogLayout();
    Empty_Area2->addComponentCluster("Empty_Area2", 1, I_Cache_Area_per_KB * HC_ICache_Size, 300., 0.5, Top);


    geogLayout * Empty_Area3 = new geogLayout();
    Empty_Area3->addComponentCluster("Empty_Area3", 1, Decoder_Area, 300., 0.5, Top);

    
    geogLayout * Lane = new geogLayout();
    Lane->addComponentCluster("Lane", 1, D_Cache_Area_per_Lane_per_KB + ALU_Area + Reg_File_Area_per_Lane, 300., 0.5, Top);

    geogLayout * Empty_Area4 = new geogLayout();
    Empty_Area4->addComponentCluster("Empty_Area4", 1, Write_Back_Area, 300., 0.5, Top);


//  Single Lane
    geogLayout * Sing_Lane = new geogLayout();
    Sing_Lane->addComponent(Empty_Area1, 1, Top);
    Sing_Lane->addComponent(Empty_Area2, 1, Top);
    Sing_Lane->addComponent(Empty_Area3, 1, Top);
    Sing_Lane->addComponent(Lane, Num_of_Sh_SLane, Top);
    Sing_Lane->addComponent(Empty_Area4, 1, Bottom);
    
//////////////////////////////////////////////////////////////////


// HCore definition
    geogLayout * HCore = new geogLayout();
    HCore->addComponent(HC_HTC, 1, Top);
    HCore->addComponent(HC_ICache, 1, Top);
    HCore->addComponent(HC_Decoder, 1, Top);
    HCore->addComponent(HC_Lane, Num_of_HCLanes, Top);
    HCore->addComponent(HC_Write_Back, 1, Bottom);

    geogLayout * L2_Cache = new geogLayout();
    L2_Cache->addComponentCluster("L2_Cache", 1, (L2$_Area_per_MB * L2_Cache_Size)/1024, 300., 0.5, Top);




/////////////////////// Cold redundancy  Spare Lane ////////////////////////
    
    #if(Num_of_HCores==0 && Num_of_Sh_SLane==0)
    //SIMT Processor
    geogLayout * MultiCore = new geogLayout();
    MultiCore->addComponent(MCore, Num_of_MCores, Top);
    MultiCore->addComponent(L2_Cache, 1, Bottom);
    //Core->addComponent(Decoder, 1, Top);
    //Core->addComponent(ALU, 32, Center);
    //Core->addComponent(Write_Back, 1, Bottom);
    #endif

/////////////////////// Cold redundancy  Shared Spare Lane ////////////////////////
    
    #if(Num_of_HCores==0 &&  Num_of_Sh_SLane)
    //////// Main Core + a single Lane
    geogLayout * MCore_plus_Single_Lane = new geogLayout();
    MCore_plus_Single_Lane->addComponent(MCore, 1, Left);
    MCore_plus_Single_Lane->addComponent(Sing_Lane, 1, Center);
    MCore_plus_Single_Lane->addComponent(MCore2, 1, Right);

    printf("I am here!!!!! \n");

    //SIMT Processor
    geogLayout * MultiCore = new geogLayout();
    MultiCore->addComponent(MCore_plus_Single_Lane, Num_of_MCores/2, Top);
    MultiCore->addComponent(L2_Cache, 1, Bottom);
    //Core->addComponent(Decoder, 1, Top);
    //Core->addComponent(ALU, 32, Center);
    //Core->addComponent(Write_Back, 1, Bottom);
    #endif


    #if(Num_of_HCores && Num_of_Sh_SLane==0)
        geogLayout * MCore_plus_HCore = new geogLayout();
        MCore_plus_HCore->addComponent(MCore, 1, Right);
        MCore_plus_HCore->addComponent(HCore, 1, Left);

        geogLayout * MultiCore = new geogLayout();
        MultiCore->addComponent(MCore_plus_HCore, Num_of_MCores, Top);
        MultiCore->addComponent(L2_Cache, 1, Bottom);   

    #elif (Num_of_HCores && Num_of_Sh_SLane)

        geogLayout * MCore_plus_two_HCores = new geogLayout();
        MCore_plus_two_HCores->addComponent(MCore, 1, Left);
        MCore_plus_two_HCores->addComponent(HCore, 1, LeftRight);
        MCore_plus_two_HCores->addComponent(MCore, 1, Right);

        geogLayout * MultiCore = new geogLayout();
        MultiCore->addComponent(MCore_plus_two_HCores, Num_of_MCores/2, Top);
        MultiCore->addComponent(L2_Cache, 1, Bottom);   

    #endif

    bool success = MultiCore->layout(AspectRatio, 1);
    if (!success) cout << "Unable to layout specified CMP configuration.";
    else 
    {
        setNameMode(true);
        ostream& HSOut = outputHotSpotHeader("SIMT65.flp");
        MultiCore->outputHotSpotLayout(HSOut);
        outputHotSpotFooter(HSOut);
        setNameMode(true);
    }
    delete MultiCore;


}


int main(int argc, char* argv[])
{
  string usageString = "Usage:\nArchFP [-h] [-v]\n-h     Print out this help information.\n-v     Output verbose layout information to stdout.\n"
    "ArchFP does not take in declarative specifications for floorplans.\n"
    "To produce a different floorplan, please edit Main.cc and recompile.\n";
  string copyrightString = "ArchFP: A pre-RTL rapid prototyping floorplanner.\nAuthor: Greg Faust (gf4ea@virginia.edu).\n";

  for (int x = 1; x < argc; x++)
    {
      string arg = string(argv[x]);
      if (arg == "-h")
        {
          cout << copyrightString;
          cout << usageString;
          return 0;
        }
      else if (arg == "-v")
        {
          verbose = true;
        }
      else
        {
          cout << copyrightString;
          cout << usageString;
          return 1;
        }
    }
  cout << copyrightString;

  ///////////////////////////////////////////////////////
  // Look at these subroutines above for examples of how to build floorplans using this tool.
  //////////////////////////////////////////////////////

  //generateTRIPS_Examples();
  //generateCheckerBoard_Examples();
  //generateMcPAT_Examples();
  //generateFixedLayout_Example();

  // Example code to generate floorplan in SoC2012 Open Source Tool submission.
  generateSIMT65nm ();
  //generatePenryn45nm();
  //generate8corePenrynCMP();

  return 0;
}

