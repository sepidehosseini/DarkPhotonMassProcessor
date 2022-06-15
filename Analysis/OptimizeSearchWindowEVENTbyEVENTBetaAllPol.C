void OptimizeSearchWindowEVENTbyEVENTBetaAllPol()
{
	ofstream databaseFile;
	databaseFile.open("loadNumberOfEvents.h");
	ofstream nPolFile;
	nPolFile.open("NumberOfPolConf.txt");

	ildStyle->SetOptStat(0);
	float logStep = 100000.0;
//	double epol_target = -0.8;
//	double ppol_target = 0.3;
	double epol_target = 0.8;
	double ppol_target = 0.3;

	double lumi_target = 900.0;
	std::vector<float> NumberOfSigmaRight={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<float> NumberOfSigmaLeft={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<string> signalMass={ "10GeV" , "20GeV" , "30GeV" , "40GeV" , "50GeV" , "60GeV" , "70GeV" , "80GeV" , "100GeV" , "110GeV" , "120GeV" , "130GeV" , "140GeV" , "150GeV" , "160GeV" , "170GeV" , "180GeV" , "190GeV" , "200GeV" , "210GeV" , "220GeV" , "230GeV" , "240GeV" };
	std::vector<float> darkPhotonMass={ 10.0 , 20.0 , 30.0 , 40.0 , 50.0 , 60.0 , 70.0 , 80.0 , 100.0 , 110.0 , 120.0 , 130.0 , 140.0 , 150.0 , 160.0 , 170.0 , 180.0 , 190.0 , 200.0 , 210.0 , 220.0 , 230.0 , 240.0 };
	vector <int> pol_e_sign = {-1 , 1};
	vector <int> pol_p_sign = {1 , -1};

	double NumberOfBackgroundEvents[ darkPhotonMass.size() ][ NumberOfSigmaRight.size() ][ NumberOfSigmaLeft.size() ];
	double NumberOfSignalEvents[ darkPhotonMass.size() ][ NumberOfSigmaRight.size() ][ NumberOfSigmaLeft.size() ];
	vector<string> physicsProcess={ "2f_leptonic" , "other" , "4f_singleZee_leptonic" };
	//2f_leptonic, 4f_singleZee_leptonic, 4f_WW_leptonic, 4f_ZZ_leptonic, 4f_ZZnunu_leptonic, 4f_ZZ_semileptonic, 4f_ZZWWMix_leptonic, 4f_singleZnunu_leptonic
	string rootFileDir = "/nfs/dust/ilc/user/hosseins/MarlinWorkdir/DiMuonProcessor/Background/root";
	string rootFileDirSignal = "/nfs/dust/ilc/user/hosseins/DarkPhoton/rec/root";
	float crossSection_BG;
	float ePolarization_BG;
	float pPolarization_BG;
	vector<float> *MassResolution_BG{};
	vector<float> *RecoDiMuonInvMass_BG{};

	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ] = 0.0;
				NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] = 0.0;
			}
		}
	}

	int nTracks_BG = 0;
	int nRecoDiMuons_BG = 0;

	for ( unsigned int i_PhysProc = 0 ; i_PhysProc < physicsProcess.size() ; ++ i_PhysProc )
	{
		int nRR_BG = 0;
		int nLL_BG = 0;
		int nLR_BG = 0;
		int nRL_BG = 0;
		string process = ( physicsProcess[ i_PhysProc ] + " (REC)" ).c_str();
		TFile *rootFile = new TFile( ( rootFileDir + "/" + physicsProcess[ i_PhysProc ] + ".root" ).c_str() );
		cout << "Reading File: " << ( rootFileDir + "/" + physicsProcess[ i_PhysProc ] + ".root" ).c_str() << endl;
		TTree *tree = (TTree*)rootFile->Get("DiMuons");

		tree->SetBranchAddress("CrossSection",&crossSection_BG);
		tree->SetBranchAddress("ePolarization",&ePolarization_BG);
		tree->SetBranchAddress("pPolarization",&pPolarization_BG);
		tree->SetBranchAddress("RecoDiMuonInvMass",&RecoDiMuonInvMass_BG);
        	tree->SetBranchAddress("nTracks",&nTracks_BG);
        	tree->SetBranchAddress("MassResolution",&MassResolution_BG);
		tree->SetBranchAddress("nRecoDiMuons",&nRecoDiMuons_BG);

		int nEntries = tree->GetEntries();
		if ( nEntries > 10000 ) nEntries = 10000;
		for ( int i = 0 ; i < nEntries ; ++i )
		{
			int iStep = (int) ( i / ( 10.0 * logStep ) );
			float fStep = i / ( 10.0 * logStep );
			if ( iStep == fStep ) std::cout << "Calculating weight for event " << i << " / " << nEntries << " from Backgrounds" << std::endl;
			tree->GetEntry(i);
			if (ePolarization_BG == 1 && pPolarization_BG == 1) ++nRR_BG;
			if (ePolarization_BG == -1 && pPolarization_BG == -1) ++nLL_BG;
			if (ePolarization_BG == 1 && pPolarization_BG == -1) ++nRL_BG;
			if (ePolarization_BG == -1 && pPolarization_BG == 1) ++nLR_BG;
		}
		std::cout << "Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eLpR, nLR =  " << nLR_BG << std::endl;
		std::cout << "Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eRpL, nRL =  " << nRL_BG << std::endl;
		std::cout << "Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eLpL, nLL =  " << nLL_BG << std::endl;
		std::cout << "Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eRpR, nRR =  " << nRR_BG << std::endl;

		nPolFile << "\\\\\\	Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eLpR, nLR =  " << nLR_BG << std::endl;
		nPolFile << "\\\\\\	Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eRpL, nRL =  " << nRL_BG << std::endl;
		nPolFile << "\\\\\\	Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eLpL, nLL =  " << nLL_BG << std::endl;
		nPolFile << "\\\\\\	Number of " << physicsProcess[ i_PhysProc ] << " Background events with polarization eRpR, nRR =  " << nRR_BG << std::endl;
		for ( int i = 0 ; i < nEntries ; ++i )
		{
			int iStep = (int) ( i / logStep );
			double fStep = i / logStep;
			if ( iStep == fStep ) std::cout << "Getting mumu-InvMass for event " << i << " / " << nEntries << " from Backgrounds" << std::endl;
			tree->GetEntry(i);
			int nEventPerPol = 0;
	
			for (int p=0; p<pol_e_sign.size(); p++)
			{
		
				double polweight = ( 1 + ePolarization_BG * pol_e_sign[p] ) * ( 1 + pPolarization_BG * pol_p_sign[p] ) / 4.0 ;

//				double polweight = ( 1 + ePolarization_BG * epol_target ) * ( 1 + pPolarization_BG * ppol_target ) / 4.0 ;
				if ( ePolarization_BG == 1 && pPolarization_BG == 1 ) nEventPerPol = nRR_BG;
				if ( ePolarization_BG == -1 && pPolarization_BG == -1) nEventPerPol = nLL_BG;
				if ( ePolarization_BG == 1 && pPolarization_BG == -1) nEventPerPol = nRL_BG;
				if ( ePolarization_BG == -1 && pPolarization_BG == 1) nEventPerPol = nLR_BG;
				double weight = polweight * crossSection_BG * lumi_target / nEventPerPol;
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
									if ( RecoDiMuonInvMass_BG->at( i_combinations ) - NumberOfSigmaLeft[ i_L ] * MassResolution_BG->at( i_combinations ) <= 	darkPhotonMass[ i_m ] && darkPhotonMass[ i_m ] <= RecoDiMuonInvMass_BG->at( i_combinations ) + NumberOfSigmaRight[ i_R ] * MassResolution_BG->at( i_combinations ) )
									{
										NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ] += weight;
									}
								}
							}
						}
					}
				}
		std::cout << "\\\\\\	weight " << physicsProcess[ i_PhysProc ] << " BG events with Allpolarization =  " << weight << std::endl;				
			}
			
		}
	}	
	float crossSection_Sig;
	float ePolarization_Sig;
	float pPolarization_Sig;
	vector<float> *MassResolution_Sig{};
	vector<float> *RecoDiMuonInvMass_Sig{};

	int nTracks_Sig = 0;
	int nRecoDiMuons_Sig = 0;

	cout << "Processing " << darkPhotonMass.size() << " Signal Files" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++ i_m )
	{
		int nRR_Sig = 0;
		int nLL_Sig = 0;
		int nLR_Sig = 0;
		int nRL_Sig = 0;
		TFile *rootFile = new TFile( ( rootFileDirSignal + "/" + signalMass[ i_m ] + "_DST_DiMuonInvMass_1.root" ).c_str() );
		cout << "Reading File: " << ( rootFileDirSignal + "/" + signalMass[ i_m ] + "_DST_DiMuonInvMass_1.root" ).c_str() << endl;
		TTree *tree = (TTree*)rootFile->Get("DiMuons");
		tree->SetBranchAddress("CrossSection",&crossSection_Sig);
		tree->SetBranchAddress("ePolarization",&ePolarization_Sig);
		tree->SetBranchAddress("pPolarization",&pPolarization_Sig);
		tree->SetBranchAddress("RecoDiMuonInvMass",&RecoDiMuonInvMass_Sig);
        	tree->SetBranchAddress("MassResolution",&MassResolution_Sig);
		tree->SetBranchAddress("nTracks",&nTracks_Sig);
		tree->SetBranchAddress("nRecoDiMuons",&nRecoDiMuons_Sig);

		int nEntries = tree->GetEntries();
		for ( int i = 0 ; i < nEntries ; ++i )
		{
			tree->GetEntry(i);
			if (ePolarization_Sig == 1 && pPolarization_Sig == 1) ++nRR_Sig;
			if (ePolarization_Sig == -1 && pPolarization_Sig == -1) ++nLL_Sig;
			if (ePolarization_Sig == 1 && pPolarization_Sig == -1) ++nRL_Sig;
			if (ePolarization_Sig == -1 && pPolarization_Sig == 1) ++nLR_Sig;
		}
		for ( int i = 0 ; i < nEntries ; ++i )
		{
			tree->GetEntry(i);
			int nEventPerPol = 0;
			for (int p=0; p<pol_e_sign.size(); p++)
			{
				double polweight = ( 1 + ePolarization_Sig * pol_e_sign[p] ) * ( 1 + pPolarization_Sig * pol_p_sign[p] ) / 4.0 ;

//				double polweight = ( 1 + ePolarization_Sig * epol_target ) * ( 1 + pPolarization_Sig * ppol_target ) / 4.0 ;
				if ( ePolarization_Sig == 1 && pPolarization_Sig == 1 ) nEventPerPol = nRR_Sig;
				if ( ePolarization_Sig == -1 && pPolarization_Sig == -1) nEventPerPol = nLL_Sig;
				if ( ePolarization_Sig == 1 && pPolarization_Sig == -1) nEventPerPol = nRL_Sig;
				if ( ePolarization_Sig == -1 && pPolarization_Sig == 1) nEventPerPol = nLR_Sig;
				double weight = polweight * crossSection_Sig * lumi_target / nEventPerPol;
		std::cout << "\\\\\\	weight " << darkPhotonMass.size() << " Signal events with Allpolarization =  " << weight << std::endl;				
				if ( nTracks_Sig <= 10 )
				{
					for ( int i_combinations = 0 ; i_combinations < RecoDiMuonInvMass_Sig->size() ; ++i_combinations )
					{
						for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
						{
							for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
							{
								if ( RecoDiMuonInvMass_Sig->at( i_combinations ) - NumberOfSigmaLeft[ i_L ] * MassResolution_Sig->at( i_combinations ) <= darkPhotonMass[ i_m ] && darkPhotonMass[ i_m ] <= RecoDiMuonInvMass_Sig->at( i_combinations ) + NumberOfSigmaRight[ i_R ] * MassResolution_Sig->at( i_combinations ) )
								{
									NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] += weight;
								}
							}
						}
					}
				}
		std::cout << "\\\\\\	weight " << darkPhotonMass.size() << " Signal events with Allpolarization =  " << weight << std::endl;				
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

	databaseFile << "void loadDarkPhotonMasses( double (&darkPhotonMass)[ " << darkPhotonMass.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		databaseFile << "	darkPhotonMass[ " << i_m << " ] = " << darkPhotonMass[ i_m ] << ";" << std::endl;
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;



	cout << "void loadNumberOfSigmaRights( double (&NumberOfSigmaRight)[ " << NumberOfSigmaRight.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
	{
		cout << "	NumberOfSigmaRight[ " << i_R << " ] = " << NumberOfSigmaRight[ i_R ] << ";" << std::endl;
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

	databaseFile << "void loadNumberOfSigmaRights( double (&NumberOfSigmaRight)[ " << NumberOfSigmaRight.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
	{
		databaseFile << "	NumberOfSigmaRight[ " << i_R << " ] = " << NumberOfSigmaRight[ i_R ] << ";" << std::endl;
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;




	cout << "void loadNumberOfSigmaLefts( double (&NumberOfSigmaLeft)[ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
	{
		cout << "	NumberOfSigmaLeft[ " << i_L << " ] = " << NumberOfSigmaLeft[ i_L ] << ";" << std::endl;
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

	databaseFile << "void loadNumberOfSigmaLefts( double (&NumberOfSigmaLeft)[ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
	{
		databaseFile << "	NumberOfSigmaLeft[ " << i_L << " ] = " << NumberOfSigmaLeft[ i_L ] << ";" << std::endl;
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;




	cout << "void loadNumberOfBackgroundEvents( double (&NumberOfBackgroundEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	cout << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				cout << "	NumberOfBackgroundEvents[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] = " << NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	cout << "}" << std::endl;
	cout << "" << std::endl;
	cout << "" << std::endl;

	databaseFile << "void loadNumberOfBackgroundEvents( double (&NumberOfBackgroundEvents)[ " << darkPhotonMass.size() << " ][ " << NumberOfSigmaRight.size() << " ][ " << NumberOfSigmaLeft.size() << " ] )" << std::endl;
	databaseFile << "{" << std::endl;
	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				databaseFile << "	NumberOfBackgroundEvents[ " << i_m << " ][ " << i_R << " ][ " << i_L << " ] = " << NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ] << ";" << std::endl;
			}
		}
	}
	databaseFile << "}" << std::endl;
	databaseFile << "" << std::endl;
	databaseFile << "" << std::endl;




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
	databaseFile.close();
	nPolFile.close();
*/	
}
