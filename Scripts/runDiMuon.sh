#!/bin/bash

dbDirectory=/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/database

tempDir=/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/temp

nPhysProc=$(wc -l ${dbDirectory}/PhysicsProcessList.txt | awk '{ print $1 }')

for (( i=1; i<=$(( ${nPhysProc} )); i++ ))
do
	PhysicsProcess=$(sed "${i}q;d" ${dbDirectory}/PhysicsProcessList.txt)
	nTotalFiles=$(wc -l ${dbDirectory}/ILD_l5_o1_v02/${PhysicsProcess}/${PhysicsProcess}.txt | awk '{ print $1 }')

	mkdir /nfs/dust/ilc/user/hosseins/MarlinWorkdir/DiMuonProcessor/Background/RootFiles/${PhysicsProcess}
	echo "#!/bin/bash" > ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
	echo "" >> ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
	echo "hadd /nfs/dust/ilc/user/hosseins/MarlinWorkdir/DiMuonProcessor/Background/RootFiles/${PhysicsProcess}.root \\" >> ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
	chmod 777 ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh

	echo "#!/bin/bash" > ${tempDir}/${PhysicsProcess}_deleteRootFiles.sh
	echo "" >> ${tempDir}/${PhysicsProcess}_deleteRootFiles.sh
	chmod 777 ${tempDir}/${PhysicsProcess}_deleteRootFiles.sh

	ndbFiles=1+${nTotalFiles}/1000
	echo "running condor ${ndbFiles} job for ${PhysicsProcess} on ${nTotalFiles} input files"

	for (( f=1; f<=$(( ${ndbFiles} )); f++ ))
	do
		dbFile=${dbDirectory}/ILD_l5_o1_v02/${PhysicsProcess}/${PhysicsProcess}_${f}.txt
		nFiles=$(wc -l ${dbFile} | awk '{ print $1 }')
		cp /afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/submitDiMuonJob.sub ${tempDir}/${PhysicsProcess}_${f}.sub
		sed -i s/N=nFiles/N=${nFiles}/g ${tempDir}/${PhysicsProcess}_${f}.sub
		sed -i s/outName/${PhysicsProcess}/g ${tempDir}/${PhysicsProcess}_${f}.sub
		sed -i s/PhysProsIter/${f}/g ${tempDir}/${PhysicsProcess}_${f}.sub
		echo "condor_submit ${tempDir}/${PhysicsProcess}_${f}.sub"
		condor_submit ${tempDir}/${PhysicsProcess}_${f}.sub
	done
done
