// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME mydict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "../../src/app/nd280UpEvent/ND280SFGDVoxelSet.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDVoxel.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDHit.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDTrack.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDEvent.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ND280SFGDHit(void *p = 0);
   static void *newArray_ND280SFGDHit(Long_t size, void *p);
   static void delete_ND280SFGDHit(void *p);
   static void deleteArray_ND280SFGDHit(void *p);
   static void destruct_ND280SFGDHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ND280SFGDHit*)
   {
      ::ND280SFGDHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ND280SFGDHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ND280SFGDHit", ::ND280SFGDHit::Class_Version(), "../../src/app/nd280UpEvent/ND280SFGDHit.hh", 10,
                  typeid(::ND280SFGDHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ND280SFGDHit::Dictionary, isa_proxy, 4,
                  sizeof(::ND280SFGDHit) );
      instance.SetNew(&new_ND280SFGDHit);
      instance.SetNewArray(&newArray_ND280SFGDHit);
      instance.SetDelete(&delete_ND280SFGDHit);
      instance.SetDeleteArray(&deleteArray_ND280SFGDHit);
      instance.SetDestructor(&destruct_ND280SFGDHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ND280SFGDHit*)
   {
      return GenerateInitInstanceLocal((::ND280SFGDHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ND280SFGDHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ND280SFGDVoxel(void *p = 0);
   static void *newArray_ND280SFGDVoxel(Long_t size, void *p);
   static void delete_ND280SFGDVoxel(void *p);
   static void deleteArray_ND280SFGDVoxel(void *p);
   static void destruct_ND280SFGDVoxel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ND280SFGDVoxel*)
   {
      ::ND280SFGDVoxel *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ND280SFGDVoxel >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ND280SFGDVoxel", ::ND280SFGDVoxel::Class_Version(), "../../src/app/nd280UpEvent/ND280SFGDVoxel.hh", 11,
                  typeid(::ND280SFGDVoxel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ND280SFGDVoxel::Dictionary, isa_proxy, 4,
                  sizeof(::ND280SFGDVoxel) );
      instance.SetNew(&new_ND280SFGDVoxel);
      instance.SetNewArray(&newArray_ND280SFGDVoxel);
      instance.SetDelete(&delete_ND280SFGDVoxel);
      instance.SetDeleteArray(&deleteArray_ND280SFGDVoxel);
      instance.SetDestructor(&destruct_ND280SFGDVoxel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ND280SFGDVoxel*)
   {
      return GenerateInitInstanceLocal((::ND280SFGDVoxel*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ND280SFGDVoxel*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ND280SFGDVoxelSet(void *p = 0);
   static void *newArray_ND280SFGDVoxelSet(Long_t size, void *p);
   static void delete_ND280SFGDVoxelSet(void *p);
   static void deleteArray_ND280SFGDVoxelSet(void *p);
   static void destruct_ND280SFGDVoxelSet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ND280SFGDVoxelSet*)
   {
      ::ND280SFGDVoxelSet *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ND280SFGDVoxelSet >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ND280SFGDVoxelSet", ::ND280SFGDVoxelSet::Class_Version(), "../../src/app/nd280UpEvent/ND280SFGDVoxelSet.hh", 14,
                  typeid(::ND280SFGDVoxelSet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ND280SFGDVoxelSet::Dictionary, isa_proxy, 4,
                  sizeof(::ND280SFGDVoxelSet) );
      instance.SetNew(&new_ND280SFGDVoxelSet);
      instance.SetNewArray(&newArray_ND280SFGDVoxelSet);
      instance.SetDelete(&delete_ND280SFGDVoxelSet);
      instance.SetDeleteArray(&deleteArray_ND280SFGDVoxelSet);
      instance.SetDestructor(&destruct_ND280SFGDVoxelSet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ND280SFGDVoxelSet*)
   {
      return GenerateInitInstanceLocal((::ND280SFGDVoxelSet*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ND280SFGDVoxelSet*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ND280SFGDTrack(void *p = 0);
   static void *newArray_ND280SFGDTrack(Long_t size, void *p);
   static void delete_ND280SFGDTrack(void *p);
   static void deleteArray_ND280SFGDTrack(void *p);
   static void destruct_ND280SFGDTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ND280SFGDTrack*)
   {
      ::ND280SFGDTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ND280SFGDTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ND280SFGDTrack", ::ND280SFGDTrack::Class_Version(), "../../src/app/nd280UpEvent/ND280SFGDTrack.hh", 11,
                  typeid(::ND280SFGDTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ND280SFGDTrack::Dictionary, isa_proxy, 4,
                  sizeof(::ND280SFGDTrack) );
      instance.SetNew(&new_ND280SFGDTrack);
      instance.SetNewArray(&newArray_ND280SFGDTrack);
      instance.SetDelete(&delete_ND280SFGDTrack);
      instance.SetDeleteArray(&deleteArray_ND280SFGDTrack);
      instance.SetDestructor(&destruct_ND280SFGDTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ND280SFGDTrack*)
   {
      return GenerateInitInstanceLocal((::ND280SFGDTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ND280SFGDTrack*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ND280SFGDEvent(void *p = 0);
   static void *newArray_ND280SFGDEvent(Long_t size, void *p);
   static void delete_ND280SFGDEvent(void *p);
   static void deleteArray_ND280SFGDEvent(void *p);
   static void destruct_ND280SFGDEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ND280SFGDEvent*)
   {
      ::ND280SFGDEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ND280SFGDEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ND280SFGDEvent", ::ND280SFGDEvent::Class_Version(), "../../src/app/nd280UpEvent/ND280SFGDEvent.hh", 9,
                  typeid(::ND280SFGDEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ND280SFGDEvent::Dictionary, isa_proxy, 4,
                  sizeof(::ND280SFGDEvent) );
      instance.SetNew(&new_ND280SFGDEvent);
      instance.SetNewArray(&newArray_ND280SFGDEvent);
      instance.SetDelete(&delete_ND280SFGDEvent);
      instance.SetDeleteArray(&deleteArray_ND280SFGDEvent);
      instance.SetDestructor(&destruct_ND280SFGDEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ND280SFGDEvent*)
   {
      return GenerateInitInstanceLocal((::ND280SFGDEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ND280SFGDEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ND280SFGDHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ND280SFGDHit::Class_Name()
{
   return "ND280SFGDHit";
}

//______________________________________________________________________________
const char *ND280SFGDHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ND280SFGDHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ND280SFGDHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ND280SFGDHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ND280SFGDVoxel::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ND280SFGDVoxel::Class_Name()
{
   return "ND280SFGDVoxel";
}

//______________________________________________________________________________
const char *ND280SFGDVoxel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDVoxel*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ND280SFGDVoxel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDVoxel*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ND280SFGDVoxel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDVoxel*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ND280SFGDVoxel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDVoxel*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ND280SFGDVoxelSet::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ND280SFGDVoxelSet::Class_Name()
{
   return "ND280SFGDVoxelSet";
}

//______________________________________________________________________________
const char *ND280SFGDVoxelSet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDVoxelSet*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ND280SFGDVoxelSet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDVoxelSet*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ND280SFGDVoxelSet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDVoxelSet*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ND280SFGDVoxelSet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDVoxelSet*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ND280SFGDTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ND280SFGDTrack::Class_Name()
{
   return "ND280SFGDTrack";
}

//______________________________________________________________________________
const char *ND280SFGDTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ND280SFGDTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ND280SFGDTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ND280SFGDTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ND280SFGDEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ND280SFGDEvent::Class_Name()
{
   return "ND280SFGDEvent";
}

//______________________________________________________________________________
const char *ND280SFGDEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ND280SFGDEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ND280SFGDEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ND280SFGDEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ND280SFGDEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ND280SFGDHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ND280SFGDHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ND280SFGDHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(ND280SFGDHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ND280SFGDHit(void *p) {
      return  p ? new(p) ::ND280SFGDHit : new ::ND280SFGDHit;
   }
   static void *newArray_ND280SFGDHit(Long_t nElements, void *p) {
      return p ? new(p) ::ND280SFGDHit[nElements] : new ::ND280SFGDHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ND280SFGDHit(void *p) {
      delete ((::ND280SFGDHit*)p);
   }
   static void deleteArray_ND280SFGDHit(void *p) {
      delete [] ((::ND280SFGDHit*)p);
   }
   static void destruct_ND280SFGDHit(void *p) {
      typedef ::ND280SFGDHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ND280SFGDHit

//______________________________________________________________________________
void ND280SFGDVoxel::Streamer(TBuffer &R__b)
{
   // Stream an object of class ND280SFGDVoxel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ND280SFGDVoxel::Class(),this);
   } else {
      R__b.WriteClassBuffer(ND280SFGDVoxel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ND280SFGDVoxel(void *p) {
      return  p ? new(p) ::ND280SFGDVoxel : new ::ND280SFGDVoxel;
   }
   static void *newArray_ND280SFGDVoxel(Long_t nElements, void *p) {
      return p ? new(p) ::ND280SFGDVoxel[nElements] : new ::ND280SFGDVoxel[nElements];
   }
   // Wrapper around operator delete
   static void delete_ND280SFGDVoxel(void *p) {
      delete ((::ND280SFGDVoxel*)p);
   }
   static void deleteArray_ND280SFGDVoxel(void *p) {
      delete [] ((::ND280SFGDVoxel*)p);
   }
   static void destruct_ND280SFGDVoxel(void *p) {
      typedef ::ND280SFGDVoxel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ND280SFGDVoxel

//______________________________________________________________________________
void ND280SFGDVoxelSet::Streamer(TBuffer &R__b)
{
   // Stream an object of class ND280SFGDVoxelSet.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ND280SFGDVoxelSet::Class(),this);
   } else {
      R__b.WriteClassBuffer(ND280SFGDVoxelSet::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ND280SFGDVoxelSet(void *p) {
      return  p ? new(p) ::ND280SFGDVoxelSet : new ::ND280SFGDVoxelSet;
   }
   static void *newArray_ND280SFGDVoxelSet(Long_t nElements, void *p) {
      return p ? new(p) ::ND280SFGDVoxelSet[nElements] : new ::ND280SFGDVoxelSet[nElements];
   }
   // Wrapper around operator delete
   static void delete_ND280SFGDVoxelSet(void *p) {
      delete ((::ND280SFGDVoxelSet*)p);
   }
   static void deleteArray_ND280SFGDVoxelSet(void *p) {
      delete [] ((::ND280SFGDVoxelSet*)p);
   }
   static void destruct_ND280SFGDVoxelSet(void *p) {
      typedef ::ND280SFGDVoxelSet current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ND280SFGDVoxelSet

//______________________________________________________________________________
void ND280SFGDTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class ND280SFGDTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ND280SFGDTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(ND280SFGDTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ND280SFGDTrack(void *p) {
      return  p ? new(p) ::ND280SFGDTrack : new ::ND280SFGDTrack;
   }
   static void *newArray_ND280SFGDTrack(Long_t nElements, void *p) {
      return p ? new(p) ::ND280SFGDTrack[nElements] : new ::ND280SFGDTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_ND280SFGDTrack(void *p) {
      delete ((::ND280SFGDTrack*)p);
   }
   static void deleteArray_ND280SFGDTrack(void *p) {
      delete [] ((::ND280SFGDTrack*)p);
   }
   static void destruct_ND280SFGDTrack(void *p) {
      typedef ::ND280SFGDTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ND280SFGDTrack

//______________________________________________________________________________
void ND280SFGDEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class ND280SFGDEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ND280SFGDEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(ND280SFGDEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ND280SFGDEvent(void *p) {
      return  p ? new(p) ::ND280SFGDEvent : new ::ND280SFGDEvent;
   }
   static void *newArray_ND280SFGDEvent(Long_t nElements, void *p) {
      return p ? new(p) ::ND280SFGDEvent[nElements] : new ::ND280SFGDEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_ND280SFGDEvent(void *p) {
      delete ((::ND280SFGDEvent*)p);
   }
   static void deleteArray_ND280SFGDEvent(void *p) {
      delete [] ((::ND280SFGDEvent*)p);
   }
   static void destruct_ND280SFGDEvent(void *p) {
      typedef ::ND280SFGDEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ND280SFGDEvent

namespace ROOT {
   static TClass *vectorlEvectorlEshortgRsPgR_Dictionary();
   static void vectorlEvectorlEshortgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEshortgRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEshortgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEshortgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEshortgRsPgR(void *p);
   static void destruct_vectorlEvectorlEshortgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<short> >*)
   {
      vector<vector<short> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<short> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<short> >", -2, "vector", 447,
                  typeid(vector<vector<short> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEshortgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<vector<short> >) );
      instance.SetNew(&new_vectorlEvectorlEshortgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEshortgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEshortgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEshortgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEshortgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<short> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<short> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEshortgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<short> >*)0x0)->GetClass();
      vectorlEvectorlEshortgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEshortgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEshortgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<short> > : new vector<vector<short> >;
   }
   static void *newArray_vectorlEvectorlEshortgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<short> >[nElements] : new vector<vector<short> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEshortgRsPgR(void *p) {
      delete ((vector<vector<short> >*)p);
   }
   static void deleteArray_vectorlEvectorlEshortgRsPgR(void *p) {
      delete [] ((vector<vector<short> >*)p);
   }
   static void destruct_vectorlEvectorlEshortgRsPgR(void *p) {
      typedef vector<vector<short> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<short> >

namespace ROOT {
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary();
   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEintgRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEintgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p);
   static void destruct_vectorlEvectorlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<int> >*)
   {
      vector<vector<int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<int> >", -2, "vector", 447,
                  typeid(vector<vector<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEintgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<vector<int> >) );
      instance.SetNew(&new_vectorlEvectorlEintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEintgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<int> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<int> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<int> >*)0x0)->GetClass();
      vectorlEvectorlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> > : new vector<vector<int> >;
   }
   static void *newArray_vectorlEvectorlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<int> >[nElements] : new vector<vector<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEintgRsPgR(void *p) {
      delete ((vector<vector<int> >*)p);
   }
   static void deleteArray_vectorlEvectorlEintgRsPgR(void *p) {
      delete [] ((vector<vector<int> >*)p);
   }
   static void destruct_vectorlEvectorlEintgRsPgR(void *p) {
      typedef vector<vector<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<int> >

namespace ROOT {
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary();
   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p);
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<double> >*)
   {
      vector<vector<double> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<double> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<double> >", -2, "vector", 447,
                  typeid(vector<vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEdoublegRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<vector<double> >) );
      instance.SetNew(&new_vectorlEvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<double> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<double> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<double> >*)0x0)->GetClass();
      vectorlEvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> > : new vector<vector<double> >;
   }
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> >[nElements] : new vector<vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete ((vector<vector<double> >*)p);
   }
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete [] ((vector<vector<double> >*)p);
   }
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p) {
      typedef vector<vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<double> >

namespace ROOT {
   static TClass *vectorlEshortgR_Dictionary();
   static void vectorlEshortgR_TClassManip(TClass*);
   static void *new_vectorlEshortgR(void *p = 0);
   static void *newArray_vectorlEshortgR(Long_t size, void *p);
   static void delete_vectorlEshortgR(void *p);
   static void deleteArray_vectorlEshortgR(void *p);
   static void destruct_vectorlEshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<short>*)
   {
      vector<short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<short>", -2, "vector", 447,
                  typeid(vector<short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEshortgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<short>) );
      instance.SetNew(&new_vectorlEshortgR);
      instance.SetNewArray(&newArray_vectorlEshortgR);
      instance.SetDelete(&delete_vectorlEshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEshortgR);
      instance.SetDestructor(&destruct_vectorlEshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<short> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<short>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<short>*)0x0)->GetClass();
      vectorlEshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<short> : new vector<short>;
   }
   static void *newArray_vectorlEshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<short>[nElements] : new vector<short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEshortgR(void *p) {
      delete ((vector<short>*)p);
   }
   static void deleteArray_vectorlEshortgR(void *p) {
      delete [] ((vector<short>*)p);
   }
   static void destruct_vectorlEshortgR(void *p) {
      typedef vector<short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<short>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 447,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 447,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEND280SFGDVoxelmUgR_Dictionary();
   static void vectorlEND280SFGDVoxelmUgR_TClassManip(TClass*);
   static void *new_vectorlEND280SFGDVoxelmUgR(void *p = 0);
   static void *newArray_vectorlEND280SFGDVoxelmUgR(Long_t size, void *p);
   static void delete_vectorlEND280SFGDVoxelmUgR(void *p);
   static void deleteArray_vectorlEND280SFGDVoxelmUgR(void *p);
   static void destruct_vectorlEND280SFGDVoxelmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ND280SFGDVoxel*>*)
   {
      vector<ND280SFGDVoxel*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ND280SFGDVoxel*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ND280SFGDVoxel*>", -2, "vector", 447,
                  typeid(vector<ND280SFGDVoxel*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEND280SFGDVoxelmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ND280SFGDVoxel*>) );
      instance.SetNew(&new_vectorlEND280SFGDVoxelmUgR);
      instance.SetNewArray(&newArray_vectorlEND280SFGDVoxelmUgR);
      instance.SetDelete(&delete_vectorlEND280SFGDVoxelmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEND280SFGDVoxelmUgR);
      instance.SetDestructor(&destruct_vectorlEND280SFGDVoxelmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ND280SFGDVoxel*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ND280SFGDVoxel*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEND280SFGDVoxelmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ND280SFGDVoxel*>*)0x0)->GetClass();
      vectorlEND280SFGDVoxelmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEND280SFGDVoxelmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEND280SFGDVoxelmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ND280SFGDVoxel*> : new vector<ND280SFGDVoxel*>;
   }
   static void *newArray_vectorlEND280SFGDVoxelmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ND280SFGDVoxel*>[nElements] : new vector<ND280SFGDVoxel*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEND280SFGDVoxelmUgR(void *p) {
      delete ((vector<ND280SFGDVoxel*>*)p);
   }
   static void deleteArray_vectorlEND280SFGDVoxelmUgR(void *p) {
      delete [] ((vector<ND280SFGDVoxel*>*)p);
   }
   static void destruct_vectorlEND280SFGDVoxelmUgR(void *p) {
      typedef vector<ND280SFGDVoxel*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ND280SFGDVoxel*>

namespace ROOT {
   static TClass *vectorlEND280SFGDTrackmUgR_Dictionary();
   static void vectorlEND280SFGDTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEND280SFGDTrackmUgR(void *p = 0);
   static void *newArray_vectorlEND280SFGDTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEND280SFGDTrackmUgR(void *p);
   static void deleteArray_vectorlEND280SFGDTrackmUgR(void *p);
   static void destruct_vectorlEND280SFGDTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ND280SFGDTrack*>*)
   {
      vector<ND280SFGDTrack*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ND280SFGDTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ND280SFGDTrack*>", -2, "vector", 447,
                  typeid(vector<ND280SFGDTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEND280SFGDTrackmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ND280SFGDTrack*>) );
      instance.SetNew(&new_vectorlEND280SFGDTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEND280SFGDTrackmUgR);
      instance.SetDelete(&delete_vectorlEND280SFGDTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEND280SFGDTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEND280SFGDTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ND280SFGDTrack*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ND280SFGDTrack*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEND280SFGDTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ND280SFGDTrack*>*)0x0)->GetClass();
      vectorlEND280SFGDTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEND280SFGDTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEND280SFGDTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ND280SFGDTrack*> : new vector<ND280SFGDTrack*>;
   }
   static void *newArray_vectorlEND280SFGDTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ND280SFGDTrack*>[nElements] : new vector<ND280SFGDTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEND280SFGDTrackmUgR(void *p) {
      delete ((vector<ND280SFGDTrack*>*)p);
   }
   static void deleteArray_vectorlEND280SFGDTrackmUgR(void *p) {
      delete [] ((vector<ND280SFGDTrack*>*)p);
   }
   static void destruct_vectorlEND280SFGDTrackmUgR(void *p) {
      typedef vector<ND280SFGDTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ND280SFGDTrack*>

namespace ROOT {
   static TClass *vectorlEND280SFGDHitmUgR_Dictionary();
   static void vectorlEND280SFGDHitmUgR_TClassManip(TClass*);
   static void *new_vectorlEND280SFGDHitmUgR(void *p = 0);
   static void *newArray_vectorlEND280SFGDHitmUgR(Long_t size, void *p);
   static void delete_vectorlEND280SFGDHitmUgR(void *p);
   static void deleteArray_vectorlEND280SFGDHitmUgR(void *p);
   static void destruct_vectorlEND280SFGDHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ND280SFGDHit*>*)
   {
      vector<ND280SFGDHit*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ND280SFGDHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ND280SFGDHit*>", -2, "vector", 447,
                  typeid(vector<ND280SFGDHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEND280SFGDHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ND280SFGDHit*>) );
      instance.SetNew(&new_vectorlEND280SFGDHitmUgR);
      instance.SetNewArray(&newArray_vectorlEND280SFGDHitmUgR);
      instance.SetDelete(&delete_vectorlEND280SFGDHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEND280SFGDHitmUgR);
      instance.SetDestructor(&destruct_vectorlEND280SFGDHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ND280SFGDHit*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ND280SFGDHit*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEND280SFGDHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ND280SFGDHit*>*)0x0)->GetClass();
      vectorlEND280SFGDHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEND280SFGDHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEND280SFGDHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ND280SFGDHit*> : new vector<ND280SFGDHit*>;
   }
   static void *newArray_vectorlEND280SFGDHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ND280SFGDHit*>[nElements] : new vector<ND280SFGDHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEND280SFGDHitmUgR(void *p) {
      delete ((vector<ND280SFGDHit*>*)p);
   }
   static void deleteArray_vectorlEND280SFGDHitmUgR(void *p) {
      delete [] ((vector<ND280SFGDHit*>*)p);
   }
   static void destruct_vectorlEND280SFGDHitmUgR(void *p) {
      typedef vector<ND280SFGDHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ND280SFGDHit*>

namespace {
  void TriggerDictionaryInitialization_mydict_Impl() {
    static const char* headers[] = {
0
    };
    static const char* includePaths[] = {
"/Applications/root_v6.12.06/include",
"/Users/cjesus/Documents/PhD/t2k-nd280-upgrade/SFGD_Reconstruction/utils/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "mydict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace std{inline namespace __1{template <class _Tp> class __attribute__((annotate("$clingAutoload$iosfwd")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}}
class __attribute__((annotate("$clingAutoload$../../src/app/nd280UpEvent/ND280SFGDVoxelSet.hh")))  ND280SFGDHit;
class __attribute__((annotate("$clingAutoload$../../src/app/nd280UpEvent/ND280SFGDVoxelSet.hh")))  ND280SFGDVoxel;
class __attribute__((annotate("$clingAutoload$../../src/app/nd280UpEvent/ND280SFGDVoxelSet.hh")))  ND280SFGDVoxelSet;
class __attribute__((annotate("$clingAutoload$../../src/app/nd280UpEvent/ND280SFGDTrack.hh")))  ND280SFGDTrack;
class __attribute__((annotate("$clingAutoload$../../src/app/nd280UpEvent/ND280SFGDEvent.hh")))  ND280SFGDEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "mydict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "../../src/app/nd280UpEvent/ND280SFGDVoxelSet.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDVoxel.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDHit.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDTrack.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDEvent.hh"

#include <vector>

#ifdef __MAKECINT__

#pragma link C++ class vector<int>;
#pragma link C++ class vector<vector<int> > ;
#pragma link C++ class vector<short>;
#pragma link C++ class vector<vector<short> > ;
#pragma link C++ class vector<vector<double> >;

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclasses;

// Reconstruction Classes
#pragma link C++ class ND280SFGDVoxel+;
#pragma link C++ class ND280SFGDVoxelSet+;
#pragma link C++ class ND280SFGDHit++;
#pragma link C++ class ND280SFGDEvent+;
#pragma link C++ class ND280SFGDTrack+;

#endif // __CINT__

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ND280SFGDEvent", payloadCode, "@",
"ND280SFGDHit", payloadCode, "@",
"ND280SFGDTrack", payloadCode, "@",
"ND280SFGDVoxel", payloadCode, "@",
"ND280SFGDVoxelSet", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("mydict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_mydict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_mydict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_mydict() {
  TriggerDictionaryInitialization_mydict_Impl();
}
