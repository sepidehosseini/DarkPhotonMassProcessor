#!/bin/bash

rm -rf /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir
mkdir /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir
Directory=( "/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir" "/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir")
PhysicsProcess=("2f_leptonic" "4f_singleZee_leptonic" "4f_singleZnunu_leptonic" "4f_WW_leptonic" "4f_ZZ_leptonic" "4f_ZZnunu_leptonic" "4f_ZZ_semileptonic" "4f_ZZWWMix_leptonic")
PhysicsProcess_length=${#PhysicsProcess[@]}
nEvents_eLpR=( "69386837" "18404682" "686351" "5202626" "264888" "270753" "5580" "0" )
nEvents_eRpL=( "40486071" "17780352" "204887" "47389" "214988" "205150" "4383" "67540" )
nEvents_eLpL=( "0" "3825006" "0" "0" "0" "0" "0" "0" )
nEvents_eRpR=( "0" "3798991" "0" "0" "0" "0" "0" "0" )
nFile=20
tempDirectory=${Directory[0]}
for (( p=0; p<$(( $PhysicsProcess_length )); p++))
do
#	p=0
	current_PhysicsProcess=${PhysicsProcess[p]}
	current_nEvents_eLpR=${nEvents_eLpR[p]}
	current_nEvents_eRpL=${nEvents_eRpL[p]}
	current_nEvents_eLpL=${nEvents_eLpL[p]}
	current_nEvents_eRpR=${nEvents_eRpR[p]}
	for (( i=0; i<$(( $nFile )); i++))
	do
		cp /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/CountNumberOfBackgroundEventsModified.C /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
#		sed -i s/'tempDir'/${tempDirectory}/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
		sed -i s/'iterationOfPhysicsProcess'/${p}/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
		sed -i s/'physicsProcessName'/${current_PhysicsProcess}/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
		sed -i s/'fileIteration'/${i}/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
		sed -i s/'nEvents_eLpR'/${current_nEvents_eLpR}/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
		sed -i s/'nEvents_eRpL'/${current_nEvents_eRpL}/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
		sed -i s/'nEvents_eLpL'/${current_nEvents_eLpL}/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
		sed -i s/'nEvents_eRpR'/${current_nEvents_eRpR}/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
		sed -i s/'numberOfFiles'/'20'/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}_${i}.C
	done
	cp /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/CountNumberOfBackgroundEvents.sub /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}.sub
	sed -i s/'nFiles'/'20'/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}.sub
	sed -i s/'physProc'/${p}/g /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}.sub
	condor_submit /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir/CountNumberOfBackgroundEvents_${p}.sub
done
