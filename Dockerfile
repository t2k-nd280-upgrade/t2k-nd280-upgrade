FROM gitlab-registry.in2p3.fr/guiguem/nd280upgrade

COPY cmake /nd280upgrade/cmake
COPY docker /nd280upgrade/docker
COPY highlandUP /nd280upgrade/highlandUP
COPY Neut /nd280upgrade/Neut
COPY SFGD_Reconstruction /nd280upgrade/SFGD_Reconstruction
COPY src /nd280upgrade/src


# RUN source $COMMON_BUILD_PREFIX/setup.sh &&\
#     cd /nd280upgrade &&\
#     ls -altrh &&\
#     ls /nd280upgrade/src/app &&\
#     mkdir mybuild &&\
#     # source docker/T2KND280Up_CLHEP.sh $PWD &&\
#     cd mybuild &&\
#     cmake ../src &&\
#     make -j3

# RUN echo 'source $COMMON_BUILD_PREFIX/setup.sh' >> /nd280upgrade/setup.sh &&\
#     echo 'export PATH=/nd280upgrade/bin:$PATH' >> /nd280upgrade/setup.sh &&\
#     echo 'export LD_LIBRARY_PATH=/nd280upgrade/lib:$LD_LIBRARY_PATH' >> /nd280upgrade/setup.sh