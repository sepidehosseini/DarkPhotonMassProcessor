#!/bin/bash

dbDirectory=/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/database

tempDir=/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Scripts/temp

nPhysProc=$(wc -l ${dbDirectory}/PhysicsProcessList.txt | awk '{ print $1 }')

for (( i=1; i<=$(( ${nPhysProc} )); i++ ))
do
	PhysicsProcess=$(sed "${i}q;d" ${dbDirectory}/PhysicsProcessList.txt)

	echo "#!/bin/bash" > ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
	echo "" >> ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
	echo "hadd /nfs/dust/ilc/user/hosseins/MarlinWorkdir/DiMuonProcessor/Background/RootFiles/${PhysicsProcess}.root" >> ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
	ls /nfs/dust/ilc/user/hosseins/MarlinWorkdir/DiMuonProcessor/Background/RootFiles/${PhysicsProcess}/*.root >> ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
	sed -i s/'root'/'root \\'/g ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
	chmod 777 ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
	cd ${tempDir}
	./${PhysicsProcess}_mergeRootFiles.sh
	sleep 10
	rm ${tempDir}/${PhysicsProcess}_mergeRootFiles.sh
done
