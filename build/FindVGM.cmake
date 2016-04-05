MESSAGE(STATUS "\n Looking for VGM...")	

FIND_PATH(VGM_LIBRARY_DIR NAMES libBaseVGM.so PATHS
 			  		      $ENV{VGM_INSTALL_DIR}/lib
					      $ENV{VGM_INSTALL_DIR}/lib64
					      NO_DEFAULT_PATH
)	

FIND_PATH(VGM_INCLUDE_DIR NAMES  BaseVGM  PATHS
  			  	 	  $ENV{VGM_INSTALL_DIR}/include
					  $ENV{VGM_INCLUDE}
					  NO_DEFAULT_PATH
)

  MESSAGE("include directory ${VGM_INCLUDE_DIR}")
  MESSAGE("library directory ${VGM_LIBRARY_DIR}")

if (VGM_LIBRARY_DIR AND VGM_INCLUDE_DIR)

 SET(VGM_INCLUDE_DIR ${VGM_INCLUDE_DIR} ${VGM_INCLUDE_DIR}/BaseVGM
                                         ${VGM_INCLUDE_DIR}/ClhepVGM 
                                         ${VGM_INCLUDE_DIR}/Geant4GM
                                         ${VGM_INCLUDE_DIR}/RootGM
                                         ${VGM_INCLUDE_DIR}/VGM
                                         ${VGM_INCLUDE_DIR}/XmlVGM)

  SET(VGM_LIBRARIES ${VGM_LIBRARIES} BaseVGM ClhepVGM Geant4GM RootGM XmlVGM)
  
   SET(VGM_FOUND true) 

  MESSAGE("include ${VGM_INCLUDE_DIR}")
  MESSAGE(STATUS "found in ${VGM_LIBRARY_DIR}")
  MESSAGE(STATUS "VGM libraries ${VGM_LIBRARIES}")

else (VGM_LIBRARY_DIR AND VGM_INCLUDE_DIR)

    message(FATAL_ERROR "Could NOT find VGM!")
    message(STATUS "Could NOT find VGM!")

endif (VGM_LIBRARY_DIR AND VGM_INCLUDE_DIR)




