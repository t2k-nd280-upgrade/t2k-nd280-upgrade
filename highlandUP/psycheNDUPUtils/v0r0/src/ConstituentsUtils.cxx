#include "ConstituentsUtils.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "BaseDataClasses.hxx"
#include "DetectorDefinition.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "SubDetId.hxx"

#include <bitset>

namespace anaUtils {}


//**************************************************
Int_t anaUtils::GetOneSegmentPerTPC(AnaTPCParticleB* in[], Int_t nseg_in, AnaTPCParticleB* out[]) {
//**************************************************

  // This method takes as input all TPC segments in a track and returns an array with only one segment per TPC, the one with more nodes

    Int_t DeltaLYZ_max[7]={0,0,0,0,0,0,0};
    Int_t itrack_DeltaLYZ_max[7]={-1,-1,-1,-1,-1,-1,-1};
    for(Int_t iseg=0; iseg< nseg_in; iseg++){
      Int_t TPC = SubDetId::GetTPC(in[iseg]->Detectors)-1;
      if (in[iseg]->DeltaLYZ > DeltaLYZ_max[TPC]){
        DeltaLYZ_max[TPC] = in[iseg]->DeltaLYZ;
        itrack_DeltaLYZ_max[TPC]=iseg;
      }      
    }

    int nseg_out = 0;    
    for(Int_t i=0;i< 7;i++){
      if (itrack_DeltaLYZ_max[i]!=-1)
        out[nseg_out++]=in[itrack_DeltaLYZ_max[i]];
    }
    
    return nseg_out;
}

//**************************************************
SubDetId::SubDetEnum anaUtils::GetClosestTPC(const AnaTrackB& track){
//**************************************************


  AnaParticleB* TPCSegment = anaUtils::GetSegmentWithMostNodesInClosestTPC(track);
  
  if (TPCSegment) return SubDetId::GetSubdetectorEnum(TPCSegment->Detectors);
  else return SubDetId::kInvalid;
}

//**************************************************
int anaUtils::GetSegmentsInDet(const AnaTrackB& track, SubDetId::SubDetEnum det, AnaParticleB* segments[]){
//**************************************************
    if (det == SubDetId::kInvalid) {
        return 0;
    }
    if(!SubDetId::GetDetectorUsed(track.Detectors, det)){

        return 0;
    }

    int count = 0;

    // Return segments for complete detector subsystems (all Target etc.) first
    switch(det){
        case SubDetId::kTarget :
            std::copy(&track.TPCSegments[0], &track.TPCSegments[track.nTPCSegments], segments);
            return track.nTPCSegments;
            break;
        case SubDetId::kTPC :
            std::copy(&track.TPCSegments[0], &track.TPCSegments[track.nTPCSegments], segments);
            return track.nTPCSegments;
            break;
        default:

            if(SubDetId::IsTarget(det)){
                for(int i = 0; i < track.nTPCSegments; ++i){
                    AnaTPCParticleB* Target_track = track.TPCSegments[i];
                    if(SubDetId::GetDetectorUsed(Target_track->Detectors, det)){
                        segments[count] = Target_track;
                        ++count;
                    }
                }
            }
            else if(SubDetId::IsTPC(det)){
                for(int i = 0; i < track.nTPCSegments; ++i){
                    AnaTPCParticleB* TPC_track = track.TPCSegments[i];
                    if(SubDetId::GetDetectorUsed(TPC_track->Detectors, det)){
                        segments[count] = TPC_track;
                        ++count;
                    }
                }
            }
            return count;
    }
    return count;
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentWithMostNodesInClosestTPC(const AnaTrackB& track){
//**************************************************

  return GetSegmentWithMostNodesInClosestTPC(track, track.PositionStart);
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentWithMostNodesInClosestTPC(const AnaTrackB& track, const Float_t* pos){
//**************************************************

    int TPC_closest = SubDetId::kInvalid;
    int TPC = SubDetId::kInvalid;

    AnaParticleB* subtrack[7] = {NULL, NULL, NULL,NULL,NULL, NULL,NULL};

    Float_t dist = 9999999.;
    int DeltaLYZ[7] = {0,0,0,0,0,0,0};

    for(int i = 0; i < track.nTPCSegments; ++i){
        AnaTPCParticleB* TPC_track = track.TPCSegments[i];
        Float_t dist_tmp = std::min(
            GetSeparationSquared(pos, TPC_track->PositionStart), 
            GetSeparationSquared(pos, TPC_track->PositionEnd)
            );
        TPC = SubDetId::GetTPC(TPC_track->Detectors);
       
        if(dist_tmp < dist){
            dist = dist_tmp;
            TPC_closest = TPC;
        }
        // TPC number is not zero ordered
        if(TPC_track->DeltaLYZ > DeltaLYZ[TPC-1]){
            DeltaLYZ[TPC-1] = TPC_track->DeltaLYZ;
            subtrack[TPC-1] = TPC_track;
        }
    }

    if(TPC_closest != (int)SubDetId::kInvalid) return subtrack[TPC_closest-1];

    return NULL;
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentWithMostNodesInDet(const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************

    if (det == SubDetId::kInvalid) {
        return NULL;
    }
    if(!SubDetId::GetDetectorUsed(track.Detectors, det)){
        return NULL;
    }

    int DeltaLYZ = 0;
    AnaParticleB* subtrack = NULL;

    switch(det){
        case SubDetId::kTarget :
            for(int i = 0; i < track.nTargetSegments; ++i){
                AnaTargetParticleB* Target_track = track.TargetSegments[i];
                if(Target_track->DeltaLYZ > DeltaLYZ){
                    DeltaLYZ = Target_track->DeltaLYZ;
                    subtrack = Target_track;
                }
            }
            return subtrack;
            break;
        case SubDetId::kTPC :
            for(int i = 0; i < track.nTPCSegments; ++i){
                AnaTPCParticleB* TPC_track = track.TPCSegments[i];
                if(TPC_track->DeltaLYZ > DeltaLYZ){
                    DeltaLYZ = TPC_track->DeltaLYZ;
                    subtrack = TPC_track;
                }
            }
            return subtrack;
            break;
        default:
            if(SubDetId::IsTarget(det)){
                for(int i = 0; i < track.nTargetSegments; ++i){
                    AnaTargetParticleB* Target_track = track.TargetSegments[i];
                    if(SubDetId::GetDetectorUsed(Target_track->Detectors, det)){
                        if(Target_track->DeltaLYZ > DeltaLYZ){
                            DeltaLYZ = Target_track->DeltaLYZ;
                            subtrack = Target_track;
                        }
                    }
                }
                return subtrack;
            }
            else if(SubDetId::IsTPC(det)){
                for(int i = 0; i < track.nTPCSegments; ++i){
                    AnaTPCParticleB* TPC_track = track.TPCSegments[i];
                    if(SubDetId::GetDetectorUsed(TPC_track->Detectors, det)){
                        if(TPC_track->DeltaLYZ > DeltaLYZ){
                            DeltaLYZ = TPC_track->DeltaLYZ;
                            subtrack = TPC_track;
                        }
                    }
                }
                return subtrack;
            }
            return NULL;
    }
}

//**************************************************
AnaParticleB* anaUtils::GetSegmentInDet(const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************
 
    if(SubDetId::IsTPC(det)){
        for(int i = 0; i < track.nTPCSegments; ++i){
            AnaTPCParticleB* TPC_track = track.TPCSegments[i];
            if(SubDetId::GetDetectorUsed(TPC_track->Detectors, det)){
                return TPC_track;
            }
        }
    }
    if(SubDetId::IsTarget(det)){
        for(int i = 0; i < track.nTargetSegments; ++i){
            AnaTargetParticleB* Target_track = track.TargetSegments[i];
 
            if(SubDetId::GetDetectorUsed(Target_track->Detectors, det)){
                return Target_track;
            }
        }
    }
    return NULL;
}

//**************************************************
SubDetId::SubDetEnum anaUtils::GetDetector(const Float_t* pos){
//**************************************************  
    for(Int_t it = 0; it != static_cast<Int_t>(SubDetId::kInvalidSubdetector); it++) {
        SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(it);
        if (anaUtils::InDetVolume(det, pos))
            return det;
    }
    return SubDetId::kInvalid;
}

//**************************************************
bool anaUtils::InDetVolume(SubDetId::SubDetEnum det, const Float_t* pos){
//**************************************************

    Float_t null[7] = {0.,0.,0.,0.,0.,0.,0.};
 
    //account for a case when a "general" volume is provided
    switch(det){
        case SubDetId::kTPC:
            return (InFiducialVolume(SubDetId::kTPCUp1,pos) || InFiducialVolume(SubDetId::kTPCUp2,pos) || InFiducialVolume(SubDetId::kForwTPC1,pos) || InFiducialVolume(SubDetId::kForwTPC2,pos) || InFiducialVolume(SubDetId::kForwTPC3,pos) || InFiducialVolume(SubDetId::kTPCDown1,pos) || InFiducialVolume(SubDetId::kTPCDown2,pos));
            break;
        case SubDetId::kTPCUp1:
            return anaUtils::InFiducialVolume(SubDetId::kTPCUp1, pos, FVDef::FVdefminTPCUp1, FVDef::FVdefmaxTPCUp1);
            break;
        case SubDetId::kTPCUp2:
            return anaUtils::InFiducialVolume(SubDetId::kTPCUp2, pos, FVDef::FVdefminTPCUp2, FVDef::FVdefmaxTPCUp2);
            break;
        case SubDetId::kForwTPC1:
            return anaUtils::InFiducialVolume(SubDetId::kForwTPC1, pos, FVDef::FVdefminForwTPC1, FVDef::FVdefmaxForwTPC1);
            break;
        case SubDetId::kForwTPC2:
            return anaUtils::InFiducialVolume(SubDetId::kForwTPC2, pos, FVDef::FVdefminForwTPC2, FVDef::FVdefmaxForwTPC2);
            break;
        case SubDetId::kForwTPC3:
            return anaUtils::InFiducialVolume(SubDetId::kForwTPC3, pos, FVDef::FVdefminForwTPC3, FVDef::FVdefmaxForwTPC3);
            break;
       case SubDetId::kTPCDown1:
            return anaUtils::InFiducialVolume(SubDetId::kTPCDown1, pos, FVDef::FVdefminTPCDown1, FVDef::FVdefmaxTPCDown1);
            break;
        case SubDetId::kTPCDown2:
            return anaUtils::InFiducialVolume(SubDetId::kTPCDown2, pos, FVDef::FVdefminTPCDown2, FVDef::FVdefmaxTPCDown2);
            break;
 
        case SubDetId::kTarget:
            return (
                InFiducialVolume(SubDetId::kTarget1, pos, null, null) ||
                InFiducialVolume(SubDetId::kTarget2, pos, null, null) );
            break;
        case SubDetId::kTarget1:
            return anaUtils::InFiducialVolume(SubDetId::kTarget1, pos, null, null);
            break;
        case SubDetId::kTarget2:
            return anaUtils::InFiducialVolume(SubDetId::kTarget2, pos, null, null);
            break;
        default:
            std::cout << "Warning: anaUtils::InDetVolume() No Volume set for " << det << std::endl;
            return false;
            break;
    }

}

//**************************************************
bool anaUtils::InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos){
//**************************************************


    


    Float_t null[7] = {0.,0.,0.,0.,0.,0.,0.};
    switch(det){
        case SubDetId::kTPC:
            return (InFiducialVolume(SubDetId::kTPCUp1,pos) || InFiducialVolume(SubDetId::kTPCUp2,pos) || InFiducialVolume(SubDetId::kForwTPC1,pos) || InFiducialVolume(SubDetId::kForwTPC2,pos) || InFiducialVolume(SubDetId::kForwTPC3,pos) || InFiducialVolume(SubDetId::kTPCDown1,pos) || InFiducialVolume(SubDetId::kTPCDown2,pos));
            break;
        case SubDetId::kTPCUp1:
            return anaUtils::InFiducialVolume(SubDetId::kTPCUp1, pos, FVDef::FVdefminTPCUp1, FVDef::FVdefmaxTPCUp1);
            break;
        case SubDetId::kTPCUp2:
            return anaUtils::InFiducialVolume(SubDetId::kTPCUp2, pos, FVDef::FVdefminTPCUp2, FVDef::FVdefmaxTPCUp2);
            break;
        case SubDetId::kForwTPC1:
            return anaUtils::InFiducialVolume(SubDetId::kForwTPC1, pos, FVDef::FVdefminForwTPC1, FVDef::FVdefmaxForwTPC1);
            break;
        case SubDetId::kForwTPC2:
            return anaUtils::InFiducialVolume(SubDetId::kForwTPC2, pos, FVDef::FVdefminForwTPC2, FVDef::FVdefmaxForwTPC2);
            break;
        case SubDetId::kForwTPC3:
            return anaUtils::InFiducialVolume(SubDetId::kForwTPC3, pos, FVDef::FVdefminForwTPC3, FVDef::FVdefmaxForwTPC3);
            break;
       case SubDetId::kTPCDown1:
            return anaUtils::InFiducialVolume(SubDetId::kTPCDown1, pos, FVDef::FVdefminTPCDown1, FVDef::FVdefmaxTPCDown1);
            break;
        case SubDetId::kTPCDown2:
            return anaUtils::InFiducialVolume(SubDetId::kTPCDown2, pos, FVDef::FVdefminTPCDown2, FVDef::FVdefmaxTPCDown2);
            break;
 
        case SubDetId::kTarget:
            return (
                InFiducialVolume(SubDetId::kTarget1, pos, null, null) ||
                InFiducialVolume(SubDetId::kTarget2, pos, null, null) );
            break;
        case SubDetId::kTarget1:
            return anaUtils::InFiducialVolume(SubDetId::kTarget1, pos, null, null);
            break;
        case SubDetId::kTarget2:
            return anaUtils::InFiducialVolume(SubDetId::kTarget2, pos, null, null);
            break;
        case SubDetId::kECAL:
            return (
                    anaUtils::InFiducialVolume(SubDetId::kP0DECal, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kBrlECal, pos, null, null) ||
                    anaUtils::InFiducialVolume(SubDetId::kDsECal, pos, null, null)
                   );
            break;
        case SubDetId::kBrlECal:
            return anaUtils::InFiducialVolume(SubDetId::kBrlECal, pos, null, null);
            break;
        case SubDetId::kP0DECal:
            return anaUtils::InFiducialVolume(SubDetId::kP0DECal, pos, null, null);
            
            break;
        case SubDetId::kDsECal:
            return anaUtils::InFiducialVolume(SubDetId::kDsECal, pos, null, null);
            break;
        default:
            std::cout << "Warning: anaUtils::InFiducialVolume() No Fiducial Volume set for " << det << std::endl;
            return false;
            break;
    }
}

//**************************************************
bool anaUtils::InFiducialVolume(SubDetId::SubDetEnum det, const Float_t* pos, const Float_t* FVdefmin, const Float_t* FVdefmax){
//**************************************************

    switch(det){
        case SubDetId::kTPCUp1:
          //  if (pos[0] > DetDef::TPCUp1min[0]+FVdefmin[0] &&
            //        pos[0] < DetDef::TPCUp1max[0]-FVdefmax[0] &&
              //      pos[1] > DetDef::TPCUp1min[1]+FVdefmin[1] &&
                //    pos[1] < DetDef::TPCUp1max[1]-FVdefmax[1] &&
                  //  pos[2] > DetDef::TPCUp1min[2]+FVdefmin[2] &&
                  //  pos[2] < DetDef::TPCUp1max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kTPCUp2:
        //    if (pos[0] > DetDef::TPCUp2min[0]+FVdefmin[0] &&
          //          pos[0] < DetDef::TPCUp2max[0]-FVdefmax[0] &&
            //        pos[1] > DetDef::TPCUp2min[1]+FVdefmin[1] &&
              //      pos[1] < DetDef::TPCUp2max[1]-FVdefmax[1] &&
                //    pos[2] > DetDef::TPCUp2min[2]+FVdefmin[2] &&
                  //  pos[2] < DetDef::TPCUp2max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kTPCDown1:
         //   if (pos[0] > DetDef::TPCDown1min[0]+FVdefmin[0] &&
           //         pos[0] < DetDef::TPCDown1max[0]-FVdefmax[0] &&
             //       pos[1] > DetDef::TPCDown1min[1]+FVdefmin[1] &&
               //     pos[1] < DetDef::TPCDown1max[1]-FVdefmax[1] &&
                 //   pos[2] > DetDef::TPCDown1min[2]+FVdefmin[2] &&
                   // pos[2] < DetDef::TPCDown1max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kTPCDown2:
        //    if (pos[0] > DetDef::TPCDown2min[0]+FVdefmin[0] &&
          //          pos[0] < DetDef::TPCDown2max[0]-FVdefmax[0] &&
            //        pos[1] > DetDef::TPCDown2min[1]+FVdefmin[1] &&
              //      pos[1] < DetDef::TPCDown2max[1]-FVdefmax[1] &&
                //    pos[2] > DetDef::TPCDown2min[2]+FVdefmin[2] &&
                  //  pos[2] < DetDef::TPCDown2max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kForwTPC1:
  //          if (pos[0] > DetDef::ForwTPC1min[0]+FVdefmin[0] &&
    //                pos[0] < DetDef::ForwTPC1max[0]-FVdefmax[0] &&
      //              pos[1] > DetDef::ForwTPC1min[1]+FVdefmin[1] &&
        //            pos[1] < DetDef::ForwTPC1max[1]-FVdefmax[1] &&
          //          pos[2] > DetDef::ForwTPC1min[2]+FVdefmin[2] &&
            //        pos[2] < DetDef::ForwTPC1max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kForwTPC2:
           // if (pos[0] > DetDef::ForwTPC2min[0]+FVdefmin[0] &&
             //       pos[0] < DetDef::ForwTPC2max[0]-FVdefmax[0] &&
               //     pos[1] > DetDef::ForwTPC2min[1]+FVdefmin[1] &&
                 //   pos[1] < DetDef::ForwTPC2max[1]-FVdefmax[1] &&
                   // pos[2] > DetDef::ForwTPC2min[2]+FVdefmin[2] &&
                  //  pos[2] < DetDef::ForwTPC2max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kForwTPC3:
        //    if (pos[0] > DetDef::ForwTPC3min[0]+FVdefmin[0] &&
          //          pos[0] < DetDef::ForwTPC3max[0]-FVdefmax[0] &&
            //        pos[1] > DetDef::ForwTPC3min[1]+FVdefmin[1] &&
                //    pos[1] < DetDef::ForwTPC3max[1]-FVdefmax[1] &&
              //      pos[2] > DetDef::ForwTPC3min[2]+FVdefmin[2] &&
                  //  pos[2] < DetDef::ForwTPC3max[2]-FVdefmax[2])
                return true;
            break;
        case SubDetId::kTarget1:
            if (pos[0] > DetDef::Target1min[0] &&
                    pos[0] < DetDef::Target1max[0] &&
                    pos[1] > DetDef::Target1min[1] &&
                    pos[1] < DetDef::Target1max[1] &&
                    pos[2] > DetDef::Target1min[2] &&
                    pos[2] < DetDef::Target1max[2])
                return true;
            break;
        case SubDetId::kTarget2:
            if (pos[0] > DetDef::Target2min[0] &&
                    pos[0] < DetDef::Target2max[0] &&
                    pos[1] > DetDef::Target2min[1] &&
                    pos[1] < DetDef::Target2max[1] &&
                    pos[2] > DetDef::Target2min[2] &&
                    pos[2] < DetDef::Target2max[2])
                 return true;
	    break;
        case SubDetId::kDsECal:
            if (pos[0] > DetDef::dsecalmin[0] &&
                    pos[0] < DetDef::dsecalmax[0] &&
                    pos[1] > DetDef::dsecalmin[1] &&
                    pos[1] < DetDef::dsecalmax[1] &&
                    pos[2] > DetDef::dsecalmin[2] &&
                    pos[2] < DetDef::dsecalmax[2])
                return true;
            break;
            //BarrelECal (TEcal)
        case SubDetId::kBrlECal:
            if (pos[0] > DetDef::tecalLmin[0] &&
                    pos[0] < DetDef::tecalLmax[0] &&
                    pos[1] > DetDef::tecalLmin[1] &&
                    pos[1] < DetDef::tecalLmax[1] &&
                    pos[2] > DetDef::tecalLmin[2] &&
                    pos[2] < DetDef::tecalLmax[2])
                return true;
            if (pos[0] > DetDef::tecalRmin[0] &&
                    pos[0] < DetDef::tecalRmax[0] &&
                    pos[1] > DetDef::tecalRmin[1] &&
                    pos[1] < DetDef::tecalRmax[1] &&
                    pos[2] > DetDef::tecalRmin[2] &&
                    pos[2] < DetDef::tecalRmax[2])
                return true;
            if (pos[0] > DetDef::tecalTLmin[0] &&
                    pos[0] < DetDef::tecalTLmax[0] &&
                    pos[1] > DetDef::tecalTLmin[1] &&
                    pos[1] < DetDef::tecalTLmax[1] &&
                    pos[2] > DetDef::tecalTLmin[2] &&
                    pos[2] < DetDef::tecalTLmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::tecalTRmin[0] &&
                    pos[0] < DetDef::tecalTRmax[0] &&
                    pos[1] > DetDef::tecalTRmin[1] &&
                    pos[1] < DetDef::tecalTRmax[1] &&
                    pos[2] > DetDef::tecalTRmin[2] &&
                    pos[2] < DetDef::tecalTRmax[2])
                return true;
            if (pos[0] > DetDef::tecalBLmin[0] &&
                    pos[0] < DetDef::tecalBLmax[0] &&
                    pos[1] > DetDef::tecalBLmin[1] &&
                    pos[1] < DetDef::tecalBLmax[1] &&
                    pos[2] > DetDef::tecalBLmin[2] &&
                    pos[2] < DetDef::tecalBLmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::tecalBRmin[0] &&
                    pos[0] < DetDef::tecalBRmax[0] &&
                    pos[1] > DetDef::tecalBRmin[1] &&
                    pos[1] < DetDef::tecalBRmax[1] &&
                    pos[2] > DetDef::tecalBRmin[2] &&
                    pos[2] < DetDef::tecalBRmax[2])
                return true;
            break;
            //P0DECal (PEcal)
        case SubDetId::kP0DECal:
            if (pos[0] > DetDef::pecalLmin[0] &&
                    pos[0] < DetDef::pecalLmax[0] &&
                    pos[1] > DetDef::pecalLmin[1] &&
                    pos[1] < DetDef::pecalLmax[1] &&
                    pos[2] > DetDef::pecalLmin[2] &&
                    pos[2] < DetDef::pecalLmax[2])
                return true;
            if (pos[0] > DetDef::pecalRmin[0] &&
                    pos[0] < DetDef::pecalRmax[0] &&
                    pos[1] > DetDef::pecalRmin[1] &&
                    pos[1] < DetDef::pecalRmax[1] &&
                    pos[2] > DetDef::pecalRmin[2] &&
                    pos[2] < DetDef::pecalRmax[2])
                return true;
             if (pos[0] > DetDef::pecalTLmin[0] &&
                    pos[0] < DetDef::pecalTLmax[0] &&
                    pos[1] > DetDef::pecalTLmin[1] &&
                    pos[1] < DetDef::pecalTLmax[1] &&
                    pos[2] > DetDef::pecalTLmin[2] &&
                    pos[2] < DetDef::pecalTLmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::pecalTRmin[0] &&
                    pos[0] < DetDef::pecalTRmax[0] &&
                    pos[1] > DetDef::pecalTRmin[1] &&
                    pos[1] < DetDef::pecalTRmax[1] &&
                    pos[2] > DetDef::pecalTRmin[2] &&
                    pos[2] < DetDef::pecalTRmax[2])
                return true;
           if (pos[0] > DetDef::pecalBLmin[0] &&
                    pos[0] < DetDef::pecalBLmax[0] &&
                    pos[1] > DetDef::pecalBLmin[1] &&
                    pos[1] < DetDef::pecalBLmax[1] &&
                    pos[2] > DetDef::pecalBLmin[2] &&
                    pos[2] < DetDef::pecalBLmax[2])
                return true;
            //two parts symmetric w.r.t to Z axis
            if (pos[0] > DetDef::pecalBRmin[0] &&
                    pos[0] < DetDef::pecalBRmax[0] &&
                    pos[1] > DetDef::pecalBRmin[1] &&
                    pos[1] < DetDef::pecalBRmax[1] &&
                    pos[2] > DetDef::pecalBRmin[2] &&
                    pos[2] < DetDef::pecalBRmax[2])
                return true;
            break;        

        default:
            std::cout << "Error:  anaUtils::InFiducialVolume() given an unknown subdetector enumeration: " << det << std::endl;

    }

    return false;

}

//**************************************************
int anaUtils::GetAllChargedTrajInTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]) {
//**************************************************
    int count = 0;
    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      if(event.TrueParticles[i]->Charge==0)continue;
      Float_t dist=-9999999;
        for(Int_t idet=0;idet<event.TrueParticles[i]->nDetCrossings;idet++){
            //i.e crossing the active part of the tpc
            if(SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kTPC) && event.TrueParticles[i]->DetCrossings[idet]->InActive) {
                Float_t sep = GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition, event.TrueParticles[i]->DetCrossings[idet]->ExitPosition);
                if(sep>dist) dist=sep;
            }
        }
        // 30* 30 originally
        if((dist)>900 && event.TrueParticles[i]->Momentum>5){//bigger than 3 TPC hits (30*30 is faster that sqrt(dist)), and momentum > 5 MeV 
      chargedtrajInBunch[count] = event.TrueParticles[i];
      ++count;
        }
    }
    return count;
}

//**************************************************
int anaUtils::GetAllChargedTrajInTargetInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det){
//**************************************************
    /* 
     * we need here to select in-Target tracks that potentially should have been reconstruced
     * by Target iso recon (the function name is confusing);
     * this involves putting some min requirements for the true tracks:
     * since Target iso recon requires a track to extend for at least 4 Z layers (i.e. having hits in five consequitive layers)
     * in order to be reconstruced this requirement should be applied for the true tracks as well.
     * In principle one can use the geometry info to retrieve layers that true entrance and exit point correspond to
     * but it can be time taking,  so we use an approximation: a true trajectory should have a length in Z at least of the one of 4 Target layers:
     * so 4 cm

     */

  int count = 0;
  for (Int_t i = 0; i < event.nTrueParticles; i++) {
    if(!event.TrueParticles[i]->TrueVertex) continue;
    if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
    if(event.TrueParticles[i]->Charge==0)continue;
    Float_t dist = -9999999;
    for (Int_t idet = 0; idet < event.TrueParticles[i]->nDetCrossings; idet++) {
      // i.e crossing the active part of the Target
      if (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kTarget)){
        if (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, det) && event.TrueParticles[i]->DetCrossings[idet]->InActive) {
          //the separation should be done using the z position, since the Target is separated by layer in z,
          //making the z position the reconstructed quantity to be cut on
          Float_t sep = fabs(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition[2] - event.TrueParticles[i]->DetCrossings[idet]->ExitPosition[2]);
          if(sep>dist) dist=sep;
        }
      }
    }
    // apply the cut (this cut is only valid for Target!)
    if (dist > 40){
      chargedtrajInBunch[count] = event.TrueParticles[i];
      ++count;
    }
  }

  return count;
}


//**************************************************
int anaUtils::GetAllChargedTrajInTargetAndNoTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[],SubDetId::SubDetEnum det){
//**************************************************  
    AnaTrueParticleB* trajInBunchInTargetx[NMAXTRUEPARTICLES];
    Int_t ntrajInBunchInTargetx = anaUtils::GetAllChargedTrajInTargetInBunch(event, trajInBunchInTargetx,det);

    Int_t count = 0;
    for (Int_t i = 0; i < ntrajInBunchInTargetx; i++) {
      Float_t dist=-999999.;
      for(Int_t idet=0;idet<trajInBunchInTargetx[i]->nDetCrossings;idet++){
        //i.e crossing the active part of the tpc
        if(SubDetId::GetDetectorUsed(trajInBunchInTargetx[i]->DetCrossings[idet]->Detector, SubDetId::kTPC) && trajInBunchInTargetx[i]->DetCrossings[idet]->InActive) {
          Float_t sep = GetSeparationSquared(trajInBunchInTargetx[i]->DetCrossings[idet]->EntrancePosition, trajInBunchInTargetx[i]->DetCrossings[idet]->ExitPosition);
          
          if(sep>dist) dist=sep;
        }
      }
      
      bool cross_tpc = false;
      // 250*250 originally
      if(dist>62500)//bigger than the ~1/4 of the width of the TPC
        cross_tpc = true;
      
      if (!cross_tpc){
        chargedtrajInBunch[count] = trajInBunchInTargetx[i];
        ++count;
      }
    }
    
    return count;
}

//**************************************************
int anaUtils::GetAllChargedTrajInTPCTargetInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]){
//**************************************************

    int count = 0;

    for(Int_t i=0;i<event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      if(event.TrueParticles[i]->Charge==0)continue;

        Float_t dist=-9999999;
        for(Int_t idet=0;idet<event.TrueParticles[i]->nDetCrossings;idet++){
            //i.e crossing the active part of one of the Targets
          if(SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kTarget)){
            for(Int_t idet1=0;idet1<event.TrueParticles[i]->nDetCrossings;idet1++){
              //look for TPC1-Target1, Target1-TPC2, TPC2-Target2, Target2-TPC3 trajectories
              if((SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kTarget1) && 
                  (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kTPCUp1) || SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kForwTPC2) || SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kTPCDown1) ||SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kForwTPC1))) || 
                 (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kTarget2) && 
                  (SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kForwTPC2) || SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kTPCDown2) || SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kTPCUp2) || SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet1]->Detector, SubDetId::kForwTPC3)))) 
                {
                  Float_t sep = GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet1]->EntrancePosition, event.TrueParticles[i]->DetCrossings[idet1]->ExitPosition);
                  if(sep>dist) dist=sep;
                }
            }
          }
        }
        
        // 10*10 originally, now 100
        if(dist>100){
            chargedtrajInBunch[count] = event.TrueParticles[i];
            ++count;
        }
    }

    return count;

}

//**************************************************
int anaUtils::GetAllBigEnoughChargedTrajInTPCInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]){
//**************************************************

    int count = 0;

    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      if(event.TrueParticles[i]->Charge==0)continue;

      Float_t dist=0;
        for(Int_t idet=0;idet<event.TrueParticles[i]->nDetCrossings;idet++){
            //i.e crossing the active part of the tpc
            if(SubDetId::GetDetectorUsed(event.TrueParticles[i]->DetCrossings[idet]->Detector, SubDetId::kTPC) && event.TrueParticles[i]->DetCrossings[idet]->InActive) {
                Float_t sep = GetSeparationSquared(event.TrueParticles[i]->DetCrossings[idet]->EntrancePosition, event.TrueParticles[i]->DetCrossings[idet]->ExitPosition);
                if(sep>dist) dist=sep;
            }
        }
        //250*250 originally 
        if(dist>62500){//bigger than the ~1/4 of the width of the TPC
            chargedtrajInBunch[count] = event.TrueParticles[i];
            ++count;
        }

    }
    return count;
}

//**************************************************
bool anaUtils::HasTrackUsingTarget(const AnaEventB& event) {
//**************************************************
  return anaUtils::HasTrackUsingDet(event, SubDetId::kTarget);
}

//**************************************************
int anaUtils::GetAllTracksUsingTPC(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kTPC, selTracks);
}

//**************************************************
int anaUtils::GetAllTracksUsingTarget(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kTarget, selTracks);
}

//**************************************************
int anaUtils::GetNTracksUsingTPCAndDet(const AnaEventB& event, SubDetId::SubDetEnum det) {
//**************************************************

    int count = 0;

    SubDetId::SubDetEnum dets[2];
    dets[0] = SubDetId::kTPC;
    dets[1] = det;

    // Loop over all tracks
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (anaUtils::TrackUsesDets(*track, dets, 2)){
            count ++;
        }
    }

    return count;
}

//**************************************************
int anaUtils::GetAllTracksUsingECAL(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************
    return GetAllTracksUsingDet(event, SubDetId::kECAL, selTracks);
}
