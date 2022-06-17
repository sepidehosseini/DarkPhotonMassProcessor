void CountNumberOfSignalEvents()
{
	ofstream databaseFile;
	databaseFile.open( "/afs/desy.de/group/flc/pool/hosseins/DarkPhotonMassProcessor/Analysis/loadNumberOfSignalEvents.h" );
	ofstream nPolFile;
	nPolFile.open( "NumberOfPolConf_Signal.txt" );

	float logStep = 100000.0;
	double epol_target = 0.8;
	double ppol_target = 0.3;
	std::vector<float> lumi_target = { 900.0 , 900.0 , 100.0 , 100.0 };
	std::vector<float> ePol_target = { -1.0 , +1.0 , -1.0 , +1.0 };
	std::vector<float> pPol_target = { +1.0 , -1.0 , -1.0 , +1.0 };
//	std::vector<float> lumi_target = { 900.0 , 900.0 };
//	std::vector<float> ePol_target = { -1.0 , +1.0 };
//	std::vector<float> pPol_target = { +1.0 , -1.0 };
	std::vector<float> NumberOfSigmaRight={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<float> NumberOfSigmaLeft={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<string> signalMass={ "10GeV" , "20GeV" , "30GeV" , "40GeV" , "50GeV" , "60GeV" , "70GeV" , "80GeV" , "100GeV" , "110GeV" , "120GeV" , "130GeV" , "140GeV" , "150GeV" , "160GeV" , "170GeV" , "180GeV" , "190GeV" , "200GeV" , "210GeV" , "220GeV" , "230GeV" , "240GeV" };
	std::vector<float> darkPhotonMass={ 10.0 , 20.0 , 30.0 , 40.0 , 50.0 , 60.0 , 70.0 , 80.0 , 100.0 , 110.0 , 120.0 , 130.0 , 140.0 , 150.0 , 160.0 , 170.0 , 180.0 , 190.0 , 200.0 , 210.0 , 220.0 , 230.0 , 240.0 };
	float NumberOfSignalEvents[ darkPhotonMass.size() ][ NumberOfSigmaRight.size() ][ NumberOfSigmaLeft.size() ];
	float totalNumberOfSignalEvents[ darkPhotonMass.size() ][ NumberOfSigmaRight.size() ][ NumberOfSigmaLeft.size() ];
	string rootFileDirSignal = "/nfs/dust/ilc/user/hosseins/DarkPhoton/rec/root";

	float crossSection;
	float ePolarization;
	float pPolarization;
	vector<float> *RecoDiMuonInvMass{};
	int nTracks = 0;
	int nRecoDiMuons = 0;
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

	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] = 0.0;
				totalNumberOfSignalEvents[ i_m ][ i_R ][ i_L ] = 0.0;
			}
		}
	}

	cout << "Processing " << darkPhotonMass.size() << " Signal Files" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++ i_m )
	{
		int nRR = 0;
		int nLL = 0;
		int nLR = 0;
		int nRL = 0;
		TFile *rootFile = new TFile( ( rootFileDirSignal + "/" + signalMass[ i_m ] + "_DiMuonInvMass.root" ).c_str() );
		cout << "Reading File: " << ( rootFileDirSignal + "/" + signalMass[ i_m ] + "_DiMuonInvMass.root" ).c_str() << endl;
		TTree *tree = (TTree*)rootFile->Get("DiMuons");
		tree->SetBranchAddress("CrossSection",&crossSection);
		tree->SetBranchAddress("ePolarization",&ePolarization);
		tree->SetBranchAddress("pPolarization",&pPolarization);
		tree->SetBranchAddress("RecoDiMuonInvMass",&RecoDiMuonInvMass);
		tree->SetBranchAddress("nTracks",&nTracks);
//		tree->SetBranchAddress("MassResolution_CovMat",&MassResolution);
		tree->SetBranchAddress("nRecoDiMuons",&nRecoDiMuons);
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
		for ( int i = 0 ; i < nEntries ; ++i )
		{
			tree->GetEntry(i);
			if ( nTracks <= 10 )
			{
				if (ePolarization == 1 && pPolarization == 1) ++nRR;
				if (ePolarization == -1 && pPolarization == -1) ++nLL;
				if (ePolarization == 1 && pPolarization == -1) ++nRL;
				if (ePolarization == -1 && pPolarization == 1) ++nLR;
			}
		}
		std::cout << "Number of Signal events with polarization eLpR, nLR =  " << nLR << std::endl;
		std::cout << "Number of Signal events with polarization eRpL, nRL =  " << nRL << std::endl;
		std::cout << "Number of Signal events with polarization eLpL, nLL =  " << nLL << std::endl;
		std::cout << "Number of Signal events with polarization eRpR, nRR =  " << nRR << std::endl;

		nPolFile << "Number of Signal events with polarization eLpR, nLR =  " << nLR << std::endl;
		nPolFile << "Number of Signal events with polarization eRpL, nRL =  " << nRL << std::endl;
		nPolFile << "Number of Signal events with polarization eLpL, nLL =  " << nLL << std::endl;
		nPolFile << "Number of Signal events with polarization eRpR, nRR =  " << nRR << std::endl;

		for ( int i = 0 ; i < nEntries ; ++i )
		{
			tree->GetEntry(i);
			int nEventPerPol = 0;
			double polweight = 0.0;
			for ( unsigned int i_pol = 0 ; i_pol < ePol_target.size() ; ++i_pol )
			{
				if ( ePol_target[ i_pol ] == 1 && pPol_target[ i_pol ] == 1 ) nEventPerPol = nRR;
				if ( ePol_target[ i_pol ] == -1 && pPol_target[ i_pol ] == -1) nEventPerPol = nLL;
				if ( ePol_target[ i_pol ] == 1 && pPol_target[ i_pol ] == -1) nEventPerPol = nRL;
				if ( ePol_target[ i_pol ] == -1 && pPol_target[ i_pol ] == 1) nEventPerPol = nLR;
				if ( nEventPerPol > 0 ) polweight += ( 1 + ePolarization * ePol_target[ i_pol ] * epol_target ) * ( 1 + pPolarization * pPol_target[ i_pol ] * ppol_target ) * lumi_target[ i_pol ] / ( 4.0 * nEventPerPol ) ;
			}
			float weight = polweight * crossSection;
			if ( nTracks <= 10 )
			{
				for ( int i_combinations = 0 ; i_combinations < RecoDiMuonInvMass->size() ; ++i_combinations )
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
							totalNumberOfSignalEvents[ i_m ][ i_R ][ i_L ] += weight;
							if ( darkPhotonMass[ i_m ] - NumberOfSigmaLeft[ i_L ] * massResolution <= RecoDiMuonInvMass->at( i_combinations ) && RecoDiMuonInvMass->at( i_combinations ) <= NumberOfSigmaRight[ i_R ] * massResolution + darkPhotonMass[ i_m ] )
							{
								NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] += weight;
							}
						}
					}
				}
			}
		}
	}

/*
	cout << "void loadDarkPhotonMasses( double (&darkPhotonMass)[ " << darkPhotonMass.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		cout << "	darkPhotonMass[ " << i_m << " ] = " << darkPhotonMass[ i_m ] << ";" << std::endl;
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

	cout << "void loadNumberOfSigmaRights( double (&NumberOfSigmaRight)[ " << NumberOfSigmaRight.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
	{
		cout << "	NumberOfSigmaRight[ " << i_R << " ] = " << NumberOfSigmaRight[ i_R ] << ";" << std::endl;
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

	cout << "void loadNumberOfSigmaLefts( double (&NumberOfSigmaLeft)[ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
	{
		cout << "	NumberOfSigmaLeft[ " << i_L << " ] = " << NumberOfSigmaLeft[ i_L ] << ";" << std::endl;
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

	cout << "void loadNumberOfSignalEvents( double (&NumberOfsignalEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				cout << "	NumberOfsignalEvents[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] = " << NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

	cout << "void loadTotalNumberOfSignalEvents( double (&totalNumberOfsignalEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				cout << "	totalNumberOfsignalEvents[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] = " << totalNumberOfSignalEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

	cout << "void loadBeta( double (&beta)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				cout << "	beta[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] = " << NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] / totalNumberOfSignalEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

*/




	databaseFile << "void loadDarkPhotonMasses( double (&darkPhotonMass)[ " << darkPhotonMass.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		databaseFile << "	darkPhotonMass[ " << i_m << " ] = " << darkPhotonMass[ i_m ] << ";" << std::endl;
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;

	databaseFile << "void loadNumberOfSigmaRights( double (&NumberOfSigmaRight)[ " << NumberOfSigmaRight.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
	{
		databaseFile << "	NumberOfSigmaRight[ " << i_R << " ] = " << NumberOfSigmaRight[ i_R ] << ";" << std::endl;
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;

	databaseFile << "void loadNumberOfSigmaLefts( double (&NumberOfSigmaLeft)[ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
	{
		databaseFile << "	NumberOfSigmaLeft[ " << i_L << " ] = " << NumberOfSigmaLeft[ i_L ] << ";" << std::endl;
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;

	databaseFile << "void loadNumberOfSignalEvents( double (&NumberOfsignalEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				databaseFile << "	NumberOfsignalEvents[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] = " << NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;

	databaseFile << "void loadTotalNumberOfSignalEvents( double (&totalNumberOfsignalEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				databaseFile << "	totalNumberOfsignalEvents[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] = " << totalNumberOfSignalEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;

	databaseFile << "void loadBeta( double (&beta)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				databaseFile << "	beta[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] = " << NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] / totalNumberOfSignalEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;

	databaseFile.close();
	nPolFile.close();

}
