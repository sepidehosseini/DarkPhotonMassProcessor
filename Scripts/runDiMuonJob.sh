#!/bin/bash

source /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-03/init_ilcsoft.sh

# source /afs/desy.de/project/ilcsoft/sw/x86_64_gcc82_centos7/v02-02-03pre/init_ilcsoft.sh

export MARLIN_DLL=$MARLIN_DLL/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/lib/libDarkPhotonMassProcessor.so

tempDir=/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/temp

Iter=$1

lineNumber=$(expr ${Iter} + 1)

outName=$2

dbFileIter=$3

dbDirectory=/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/database/ILD_l5_o1_v02/${outName}

dbFile=${dbDirectory}/${outName}_${dbFileIter}.txt

outDir=/nfs/dust/ilc/user/hosseins/MarlinWorkdir/DiMuonProcessor/Background/RootFiles/${outName}

inputfile=$(sed "${lineNumber}q;d" ${dbFile})

let rootFileIter=(${dbFileIter}-1)\*1000+${Iter}

echo "MARLIN_DLL=$MARLIN_DLL"

echo "Marlin /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/DiMuonProcessor.xml"
echo "	--global.LCIOInputFiles=${inputfile}"
echo "	--constant.OutputDirectory=${outDir}"
echo "	--constant.OutputBaseName=${outName}"
echo "	--constant.OutputBaseNameIter=${rootFileIter}"


Marlin /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/DiMuonProcessor.xml \
	--global.LCIOInputFiles=${inputfile} \
	--constant.OutputDirectory=${outDir} \
	--constant.OutputBaseName=${outName} \
	--constant.OutputBaseNameIter=${rootFileIter}

sleep 120

echo "${outDir}/${outName}_DiMuonInvMass_${rootFileIter}.root \\" >> ${tempDir}/${outName}_mergeRootFiles.sh
echo "rm ${outDir}/${outName}_DiMuonInvMass_${rootFileIter}.root \\" >> ${tempDir}/${outName}_deleteRootFiles.sh

echo "Finished"
