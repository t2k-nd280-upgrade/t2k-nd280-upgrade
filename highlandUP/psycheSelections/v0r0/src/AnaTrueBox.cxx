#include "AnaTrueBox.hxx"
#include <typeinfo>

//********************************************************************
AnaTrueBox::AnaTrueBox():ToyBoxB(){
//******************************************************************** 
  
        Vertex = NULL;
        Leptontrack = NULL;
        HMNtrack = NULL;
        flag_mom = 0;
        npion_pos = 0;
        flag_HMN_mom = 0;
        npion = 0;
        PositivePionTPCtracks = NULL;
        random_true = NULL;
        if(random_true == NULL){
            random_true = new TRandom3(1);
        }

} 

//********************************************************************
void AnaTrueBox::Reset(){
//******************************************************************** 

  if (!_ResetCheckDone){
    if( typeid(*this) !=  typeid(AnaTrueBox)){
      std::cerr << "ERROR in AnaTrueBox::Reset(). Either this mandatory method is not implemented " 
                << "by the derived class '" << typeid(*this).name() << "' "
                << "or AnaTrueBox::Reset() is called from the Reset method of the derived class. "
                << "Please correct any of these bugs. " << std::endl;
      
      exit(1);
    }
    _ResetCheckDone=true;
  }
  
}

//********************************************************************
void AnaTrueBox::ResetBase(){
//******************************************************************** 
    if(Leptontrack) delete Leptontrack;
    if(HMNtrack) delete HMNtrack;
    if(Vertex) delete Vertex;
    if(PositivePionTPCtracks) delete PositivePionTPCtracks;

    npion=0;
    npion_pos=0;
    flag_mom=0;
    flag_HMN_mom=0;
//        if(random_true) delete random_true;
  SoftReset();
}  
