void CountNumberOfBackgroundEvents()
{
	vector<string> physicsProcess={ "2f_leptonic" , "4f_singleZee_leptonic" , "4f_singleZnunu_leptonic" , "4f_WW_leptonic" , "4f_ZZ_leptonic" , "4f_ZZnunu_leptonic" , "4f_ZZ_semileptonic" , "4f_ZZWWMix_leptonic" };
	vector<int> nEvents_eLpR={ 69386837 , 18404682 , 686351 , 5202626 , 264888 , 270753 , 5580 , 0 };
	vector<int> nEvents_eRpL={ 40486071 , 17780352 , 204887 , 47389 , 214988 , 205150 , 4383 , 67540 };
	vector<int> nEvents_eLpL={ 0 , 3825006 , 0 , 0 , 0 , 0 , 0 , 0 };
	vector<int> nEvents_eRpR={ 0 , 3798991 , 0 , 0 , 0 , 0 , 0 , 0 };
	int i_PhysProc;
	std::cout << "Physics Process of Background:" << std::endl;
	for ( unsigned int i = 0 ; i < physicsProcess.size() ; ++i )
	{
		cout << " (" << i << "): " << physicsProcess[ i ] << " ";
	}
	std::cout << "" << std::endl;
	std::cout << "Enter Physics Process of Background, i_PhysProc = ";
	std::cin >> i_PhysProc;
	std::cout << "" << std::endl;
	string physProc = physicsProcess[ i_PhysProc ];
	std::cout << "Counting Background events for Physics Process:" << physProc << std::endl;
	std::cout << "Writing output on :" << ( "loadNumberOfEvents_" + physProc + ".h" ).c_str() << std::endl;
	ofstream databaseFile;
	databaseFile.open( ( "loadNumberOfEvents_" + physProc + ".h" ).c_str() );

	float logStep = 100000.0;
	double epol_target = 0.8;
	double ppol_target = 0.3;
	std::vector<float> lumi_target = { 1350.0 , 450.0 , 100.0 , 100.0 };
	std::vector<float> ePol_target = { -1.0 , +1.0 , -1.0 , +1.0 };
	std::vector<float> pPol_target = { +1.0 , -1.0 , -1.0 , +1.0 };
	std::vector<float> NumberOfSigmaRight={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<float> NumberOfSigmaLeft={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<float> darkPhotonMass={ 10.0 , 20.0 , 30.0 , 40.0 , 50.0 , 60.0 , 70.0 , 80.0 , 100.0 , 110.0 , 120.0 , 130.0 , 140.0 , 150.0 , 160.0 , 170.0 , 180.0 , 190.0 , 200.0 , 210.0 , 220.0 , 230.0 , 240.0 };
	double NumberOfBackgroundEvents[ darkPhotonMass.size() ][ NumberOfSigmaRight.size() ][ NumberOfSigmaLeft.size() ];

	string rootFileDir = "/nfs/dust/ilc/user/hosseins/MarlinWorkdir/DiMuonProcessor/Background/RootFiles";

	float crossSection_BG;
	float ePolarization_BG;
	float pPolarization_BG;
	int nTracks_BG = 0;
	int nRecoDiMuons_BG = 0;
	vector<float> *MassResolution_BG{};
	vector<float> *RecoDiMuonInvMass_BG{};

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

	int nRR_BG = nEvents_eRpR[ i_PhysProc ];
	int nLL_BG = nEvents_eLpL[ i_PhysProc ];
	int nLR_BG = nEvents_eLpR[ i_PhysProc ];
	int nRL_BG = nEvents_eRpL[ i_PhysProc ];
	string process = ( physicsProcess[ i_PhysProc ] + " (REC)" ).c_str();
	TFile *rootFile = new TFile( ( rootFileDir + "/" + physicsProcess[ i_PhysProc ] + ".root" ).c_str() );
	cout << "Reading File: " << ( rootFileDir + "/" + physicsProcess[ i_PhysProc ] + ".root" ).c_str() << endl;
	TTree *tree = (TTree*)rootFile->Get("DiMuons");

	tree->SetBranchAddress("CrossSection",&crossSection_BG);
	tree->SetBranchAddress("ePolarization",&ePolarization_BG);
	tree->SetBranchAddress("pPolarization",&pPolarization_BG);
	tree->SetBranchAddress("RecoDiMuonInvMass",&RecoDiMuonInvMass_BG);
	tree->SetBranchAddress("nTracks",&nTracks_BG);
	tree->SetBranchAddress("MassResolution_CovMat",&MassResolution_BG);
	tree->SetBranchAddress("nRecoDiMuons",&nRecoDiMuons_BG);

	int nEntries = tree->GetEntries();
//	if ( nEntries > 100000 ) nEntries = 100000;
/*
	ofstream nPolFile;
	nPolFile.open( ( "NumberOfPolConf_" + physProc + ".txt" ).c_str() );
	for ( int i = 0 ; i < nEntries ; ++i )
	{
		int iStep = (int) ( i / ( 10.0 * logStep ) );
		float fStep = i / ( 10.0 * logStep );
		if ( iStep == fStep ) std::cout << "Calculating weight for event " << i << " / " << nEntries << " from Backgrounds" << std::endl;
		tree->GetEntry(i);
		if ( nTracks_BG <= 10 )
		{
			if (ePolarization_BG == 1 && pPolarization_BG == 1) ++nRR_BG;
			if (ePolarization_BG == -1 && pPolarization_BG == -1) ++nLL_BG;
			if (ePolarization_BG == 1 && pPolarization_BG == -1) ++nRL_BG;
			if (ePolarization_BG == -1 && pPolarization_BG == 1) ++nLR_BG;
		}
	}

//	std::cout << "Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eLpR, nLR =  " << nLR_BG << std::endl;
//	std::cout << "Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eRpL, nRL =  " << nRL_BG << std::endl;
//	std::cout << "Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eLpL, nLL =  " << nLL_BG << std::endl;
//	std::cout << "Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eRpR, nRR =  " << nRR_BG << std::endl;

	nPolFile << "\\\\\\	Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eLpR, nLR =  " << nLR_BG << std::endl;
	nPolFile << "\\\\\\	Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eRpL, nRL =  " << nRL_BG << std::endl;
	nPolFile << "\\\\\\	Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eLpL, nLL =  " << nLL_BG << std::endl;
	nPolFile << "\\\\\\	Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eRpR, nRR =  " << nRR_BG << std::endl;
	nPolFile.close();
*/
	for ( int i = 0 ; i < nEntries ; ++i )
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
					for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
					{
						for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
						{
							if ( RecoDiMuonInvMass_BG->at( i_combinations ) - NumberOfSigmaLeft[ i_L ] * MassResolution_BG->at( i_combinations ) <= darkPhotonMass[ i_m ] && darkPhotonMass[ i_m ] <= RecoDiMuonInvMass_BG->at( i_combinations ) + NumberOfSigmaRight[ i_R ] * MassResolution_BG->at( i_combinations ) )
							{
								NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ] += weight;
							}
						}
					}
				}
			}
		}
	}

	cout << "void loadNumberOfBackgroundEvents" << i_PhysProc << "( double (&NumberOfBackgroundEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
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

	databaseFile << "void loadNumberOfBackgroundEvents" << i_PhysProc << "( double (&NumberOfBackgroundEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
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
