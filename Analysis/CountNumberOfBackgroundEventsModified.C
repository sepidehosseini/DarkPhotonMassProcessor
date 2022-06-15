void CountNumberOfBackgroundEvents_iterationOfPhysicsProcess_fileIteration()
{
	string directory = "/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/tempDir";
	int i_PhysProc = iterationOfPhysicsProcess;
	string physProc = "physicsProcessName";
	int iter = fileIteration;
	int nFiles = numberOfFiles;

	if ( i_PhysProc < 0 || i_PhysProc > 7 || iter < 0 || iter >= 20 )
	{
		std::cout << "choose Physics Process from 0-7, Choose iteration from 0-20" << std::endl;
		return;
	}

	int nLR_BG = nEvents_eLpR;
	int nRL_BG = nEvents_eRpL;
	int nLL_BG = nEvents_eLpL;
	int nRR_BG = nEvents_eRpR;

	ofstream databaseFile;
	databaseFile.open( ( directory + "/loadNumberOfEvents_" + physProc + "_" + to_string( iter ) + ".h" ).c_str() );

	std::cout << "Counting Background events for Physics Process:" << physProc << " in " << iter << "th iteration" << std::endl;
	std::cout << "Writing output on :" << ( directory + "/loadNumberOfEvents_" + physProc + "_" + to_string( iter ) + ".h" ).c_str() << std::endl;

	float logStep = 100000.0;
	double epol_target = 0.8;
	double ppol_target = 0.3;
	std::vector<float> lumi_target = { 900.0 , 900.0 , 100.0 , 100.0 };
	std::vector<float> ePol_target = { -1.0 , +1.0 , -1.0 , +1.0 };
	std::vector<float> pPol_target = { +1.0 , -1.0 , -1.0 , +1.0 };
	std::vector<float> NumberOfSigmaRight={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<float> NumberOfSigmaLeft={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<float> darkPhotonMass={ 10.0 , 20.0 , 30.0 , 40.0 , 50.0 , 60.0 , 70.0 , 80.0 , 100.0 , 110.0 , 120.0 , 130.0 , 140.0 , 150.0 , 160.0 , 170.0 , 180.0 , 190.0 , 200.0 , 210.0 , 220.0 , 230.0 , 240.0 };
	double NumberOfBackgroundEvents[ darkPhotonMass.size() ][ NumberOfSigmaRight.size() ][ NumberOfSigmaLeft.size() ];
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ] = 0.0;
			}
		}
	}

	string rootFileDir = "/nfs/dust/ilc/user/hosseins/MarlinWorkdir/DiMuonProcessor/Background/RootFiles";
	TFile *rootFile = new TFile( ( rootFileDir + "/" + physProc + ".root" ).c_str() );
	cout << "Reading File: " << ( rootFileDir + "/" + physProc + ".root" ).c_str() << endl;
	TTree *tree = (TTree*)rootFile->Get("DiMuons");

	float crossSection_BG;
	float ePolarization_BG;
	float pPolarization_BG;
	int nTracks_BG = 0;
	int nRecoDiMuons_BG = 0;
	vector<float> *MassResolution_BG{};
	vector<float> *RecoDiMuonInvMass_BG{};
	vector<float> *m_RecoMuon1Px{};
	vector<float> *m_RecoMuon1Py{};
	vector<float> *m_RecoMuon1Pz{};
	vector<float> *m_RecoMuon1E{};
	vector<float> *m_RecoMuon1SigmaPx2{};
	vector<float> *m_RecoMuon1SigmaPxPy{};
	vector<float> *m_RecoMuon1SigmaPy2{};
	vector<float> *m_RecoMuon1SigmaPxPz{};
	vector<float> *m_RecoMuon1SigmaPyPz{};
	vector<float> *m_RecoMuon1SigmaPz2{};
	vector<float> *m_RecoMuon1SigmaPxE{};
	vector<float> *m_RecoMuon1SigmaPyE{};
	vector<float> *m_RecoMuon1SigmaPzE{};
	vector<float> *m_RecoMuon1SigmaE2{};
	vector<float> *m_RecoMuon2Px{};
	vector<float> *m_RecoMuon2Py{};
	vector<float> *m_RecoMuon2Pz{};
	vector<float> *m_RecoMuon2E{};
	vector<float> *m_RecoMuon2SigmaPx2{};
	vector<float> *m_RecoMuon2SigmaPxPy{};
	vector<float> *m_RecoMuon2SigmaPy2{};
	vector<float> *m_RecoMuon2SigmaPxPz{};
	vector<float> *m_RecoMuon2SigmaPyPz{};
	vector<float> *m_RecoMuon2SigmaPz2{};
	vector<float> *m_RecoMuon2SigmaPxE{};
	vector<float> *m_RecoMuon2SigmaPyE{};
	vector<float> *m_RecoMuon2SigmaPzE{};
	vector<float> *m_RecoMuon2SigmaE2{};


	tree->SetBranchAddress("CrossSection",&crossSection_BG);
	tree->SetBranchAddress("ePolarization",&ePolarization_BG);
	tree->SetBranchAddress("pPolarization",&pPolarization_BG);
	tree->SetBranchAddress("RecoDiMuonInvMass",&RecoDiMuonInvMass_BG);
	tree->SetBranchAddress("nTracks",&nTracks_BG);
	tree->SetBranchAddress("MassResolution_CovMat",&MassResolution_BG);
	tree->SetBranchAddress("nRecoDiMuons",&nRecoDiMuons_BG);
	tree->SetBranchAddress("RecoMuon1Px", &m_RecoMuon1Px);
        tree->SetBranchAddress("RecoMuon1Py", &m_RecoMuon1Py);
        tree->SetBranchAddress("RecoMuon1Pz", &m_RecoMuon1Pz);
        tree->SetBranchAddress("RecoMuon1E", &m_RecoMuon1E);
        tree->SetBranchAddress("RecoMuon1SigmaPx2", &m_RecoMuon1SigmaPx2);
        tree->SetBranchAddress("RecoMuon1SigmaPxPy", &m_RecoMuon1SigmaPxPy);
        tree->SetBranchAddress("RecoMuon1SigmaPy2", &m_RecoMuon1SigmaPy2);
        tree->SetBranchAddress("RecoMuon1SigmaPxPz", &m_RecoMuon1SigmaPxPz);
        tree->SetBranchAddress("RecoMuon1SigmaPyPz", &m_RecoMuon1SigmaPyPz);
        tree->SetBranchAddress("RecoMuon1SigmaPz2", &m_RecoMuon1SigmaPz2);
        tree->SetBranchAddress("RecoMuon1SigmaPxE", &m_RecoMuon1SigmaPxE);
        tree->SetBranchAddress("RecoMuon1SigmaPyE", &m_RecoMuon1SigmaPyE);
        tree->SetBranchAddress("RecoMuon1SigmaPzE", &m_RecoMuon1SigmaPzE);
        tree->SetBranchAddress("RecoMuon1SigmaE2", &m_RecoMuon1SigmaE2);
        tree->SetBranchAddress("RecoMuon2Px", &m_RecoMuon2Px);
        tree->SetBranchAddress("RecoMuon2Py", &m_RecoMuon2Py);
        tree->SetBranchAddress("RecoMuon2Pz", &m_RecoMuon2Pz);
        tree->SetBranchAddress("RecoMuon2E", &m_RecoMuon2E);
        tree->SetBranchAddress("RecoMuon2SigmaPx2", &m_RecoMuon2SigmaPx2);
        tree->SetBranchAddress("RecoMuon2SigmaPxPy", &m_RecoMuon2SigmaPxPy);
        tree->SetBranchAddress("RecoMuon2SigmaPy2", &m_RecoMuon2SigmaPy2);
        tree->SetBranchAddress("RecoMuon2SigmaPxPz", &m_RecoMuon2SigmaPxPz);
        tree->SetBranchAddress("RecoMuon2SigmaPyPz", &m_RecoMuon2SigmaPyPz);
        tree->SetBranchAddress("RecoMuon2SigmaPz2", &m_RecoMuon2SigmaPz2);
        tree->SetBranchAddress("RecoMuon2SigmaPxE", &m_RecoMuon2SigmaPxE);
        tree->SetBranchAddress("RecoMuon2SigmaPyE", &m_RecoMuon2SigmaPyE);
        tree->SetBranchAddress("RecoMuon2SigmaPzE", &m_RecoMuon2SigmaPzE);
        tree->SetBranchAddress("RecoMuon2SigmaE2", &m_RecoMuon2SigmaE2);

	int nEntries = tree->GetEntries();
	std::cout << "Total Number of events: " << nEntries << std::endl;
	std::cout << "iteration of this run: " << iter << std::endl;
	int eventStepSize = nEntries / nFiles;
	std::cout << "Number of events in this run: " << eventStepSize << std::endl;
	int startEvent = iter * eventStepSize;
	int endEvent = startEvent + eventStepSize - 1;
	if ( endEvent > nEntries ) endEvent = nEntries;
	std::cout << "Counting Background events from event " << startEvent << " to event " << endEvent << " from Total " << nEntries << " events" << std::endl;

	for ( int i = startEvent ; i <= endEvent ; ++i )
	{
		int iStep = (int) ( i / logStep );
		double fStep = i / logStep;
		if ( iStep == fStep ) std::cout << "Getting mumu-InvMass for event " << i << " / " << nEntries << " from Backgrounds" << std::endl;
		tree->GetEntry(i);
		int nEventPerPol = 0;
		double polweight = 0.0;
		for ( unsigned int i_pol = 0 ; i_pol < ePol_target.size() ; ++i_pol )
		{
			if ( ePolarization_BG == 1 && pPolarization_BG == 1 ) nEventPerPol = nRR_BG;
			if ( ePolarization_BG == -1 && pPolarization_BG == -1) nEventPerPol = nLL_BG;
			if ( ePolarization_BG == 1 && pPolarization_BG == -1) nEventPerPol = nRL_BG;
			if ( ePolarization_BG == -1 && pPolarization_BG == 1) nEventPerPol = nLR_BG;
			if ( nEventPerPol > 0 ) polweight += ( 1 + ePolarization_BG * ePol_target[ i_pol ] * epol_target ) * ( 1 + pPolarization_BG * pPol_target[ i_pol ] * ppol_target ) * lumi_target[ i_pol ] / ( 4.0 * nEventPerPol ) ;
		}
		double weight = polweight * crossSection_BG;
		if ( nTracks_BG <= 10 )
		{
			for ( int i_combinations = 0 ; i_combinations < RecoDiMuonInvMass_BG->size() ; ++i_combinations )
			{
				for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
				{
					float dM_dPx1		= -1.0 * m_RecoMuon2Px->at( i_combinations ) / darkPhotonMass[ i_m ];
					float dM_dPy1		= -1.0 * m_RecoMuon2Py->at( i_combinations ) / darkPhotonMass[ i_m ];
					float dM_dPz1		= -1.0 * m_RecoMuon2Pz->at( i_combinations ) / darkPhotonMass[ i_m ];
					float dM_dE1		= m_RecoMuon2E->at( i_combinations ) / darkPhotonMass[ i_m ];
					float dM_dPx2		= -1.0 * m_RecoMuon1Px->at( i_combinations ) / darkPhotonMass[ i_m ];
					float dM_dPy2		= -1.0 * m_RecoMuon1Py->at( i_combinations ) / darkPhotonMass[ i_m ];
					float dM_dPz2		= -1.0 * m_RecoMuon1Pz->at( i_combinations ) / darkPhotonMass[ i_m ];
					float dM_dE2		= m_RecoMuon1E->at( i_combinations ) / darkPhotonMass[ i_m ];
					double sigma_M2		= pow( dM_dPx1 , 2 ) * m_RecoMuon1SigmaPx2->at( i_combinations )
								+ pow( dM_dPy1 , 2 ) * m_RecoMuon1SigmaPy2->at( i_combinations )
								+ pow( dM_dPz1 , 2 ) * m_RecoMuon1SigmaPz2->at( i_combinations )
								+ pow( dM_dE1 , 2 ) * m_RecoMuon1SigmaE2->at( i_combinations )
								+ pow( dM_dPx2 , 2 ) * m_RecoMuon2SigmaPx2->at( i_combinations )
								+ pow( dM_dPy2 , 2 ) * m_RecoMuon2SigmaPy2->at( i_combinations )
								+ pow( dM_dPz2 , 2 ) * m_RecoMuon2SigmaPz2->at( i_combinations )
								+ pow( dM_dE2 , 2 ) * m_RecoMuon2SigmaE2->at( i_combinations )
								+ 2.0 * dM_dPx1 * dM_dPy1 * m_RecoMuon1SigmaPxPy->at( i_combinations )
								+ 2.0 * dM_dPx1 * dM_dPz1 * m_RecoMuon1SigmaPxPz->at( i_combinations )
								+ 2.0 * dM_dPy1 * dM_dPz1 * m_RecoMuon1SigmaPyPz->at( i_combinations )
								+ 2.0 * dM_dPx1 * dM_dE1 * m_RecoMuon1SigmaPxE->at( i_combinations )
								+ 2.0 * dM_dPy1 * dM_dE1 * m_RecoMuon1SigmaPyE->at( i_combinations )
								+ 2.0 * dM_dPz1 * dM_dE1 * m_RecoMuon1SigmaPzE->at( i_combinations )
								+ 2.0 * dM_dPx2 * dM_dPy2 * m_RecoMuon2SigmaPxPy->at( i_combinations )
								+ 2.0 * dM_dPx2 * dM_dPz2 * m_RecoMuon2SigmaPxPz->at( i_combinations )
								+ 2.0 * dM_dPy2 * dM_dPz2 * m_RecoMuon2SigmaPyPz->at( i_combinations )
								+ 2.0 * dM_dPx2 * dM_dE2 * m_RecoMuon2SigmaPxE->at( i_combinations )
								+ 2.0 * dM_dPy2 * dM_dE2 * m_RecoMuon2SigmaPyE->at( i_combinations )
								+ 2.0 * dM_dPz2 * dM_dE2 * m_RecoMuon2SigmaPzE->at( i_combinations );
					float massResolution	= sqrt( sigma_M2 );
					for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
					{
						for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
						{
							if ( darkPhotonMass[ i_m ] - NumberOfSigmaLeft[ i_L ] * massResolution <= RecoDiMuonInvMass_BG->at( i_combinations ) && RecoDiMuonInvMass_BG->at( i_combinations ) <= NumberOfSigmaRight[ i_R ] * massResolution + darkPhotonMass[ i_m ] )
							{
								NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ] += weight;
							}
						}
					}
				}
			}
		}
	}

	cout << "void loadNumberOfBackgroundEvents_" << i_PhysProc << "_" << iter << "( double (&NumberOfBackgroundEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				cout << "	NumberOfBackgroundEvents[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] += " << NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

	databaseFile << "void loadNumberOfBackgroundEvents_" << i_PhysProc << "_" << iter << "( double (&NumberOfBackgroundEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				databaseFile << "	NumberOfBackgroundEvents[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] += " << NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;

	databaseFile.close();
}
