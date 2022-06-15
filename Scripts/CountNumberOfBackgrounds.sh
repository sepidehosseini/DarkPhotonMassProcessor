#!/bin/bash

source /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03/init_ilcsoft.sh

# source /afs/desy.de/project/ilcsoft/sw/x86_64_gcc82_centos7/v02-02-03pre/init_ilcsoft.sh

Iter=$1

physProc=$2

root -b -q /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${physProc}_${Iter}.C
