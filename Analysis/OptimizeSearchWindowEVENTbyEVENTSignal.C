void OptimizeSearchWindowEVENTbyEVENTSignal()
{
	ildStyle->SetOptStat(0);
	float logStep = 100000.0;
	float epol_target = -0.8;
	float ppol_target = +0.3;
	float lumi_target = 900.0;
	std::vector<float> NumberOfSigmaRight={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<float> NumberOfSigmaLeft={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 , 2.1 , 2.2 , 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8 , 2.9 , 3.0 };
	std::vector<string> signalMass={ "10GeV" , "20GeV" , "30GeV" , "40GeV" , "50GeV" , "60GeV" , "70GeV" , "80GeV" , "100GeV" , "110GeV" , "120GeV" , "130GeV" , "140GeV" , "150GeV" , "160GeV" , "170GeV" , "180GeV" , "190GeV" , "200GeV" , "210GeV" , "220GeV" , "230GeV" , "240GeV" };
	std::vector<float> darkPhotonMass={ 10.0 , 20.0 , 30.0 , 40.0 , 50.0 , 60.0 , 70.0 , 80.0 , 100.0 , 110.0 , 120.0 , 130.0 , 140.0 , 150.0 , 160.0 , 170.0 , 180.0 , 190.0 , 200.0 , 210.0 , 220.0 , 230.0 , 240.0 };
//	std::vector<string> signalMass={ "150GeV" };
//	std::vector<float> darkPhotonMass={ 150.0 };
	float NumberOfSignalEvents[ darkPhotonMass.size() ][ NumberOfSigmaRight.size() ][ NumberOfSigmaLeft.size() ];
	string rootFileDirSignal = "/nfs/dust/ilc/user/hosseins/DarkPhoton/rec/root";

	float crossSection_Sig;
	float ePolarization_Sig;
	float pPolarization_Sig;
	vector<float> *MassResolution_Sig{};
	vector<float> *RecoDiMuonInvMass_Sig{};

	int nTracks_Sig = 0;
	int nRecoDiMuons_Sig = 0;

	for ( unsigned int i_m = 0 ; i_m < darkPhotonMass.size() ; ++i_m )
	{
		for ( unsigned int i_R = 0 ; i_R < NumberOfSigmaRight.size() ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < NumberOfSigmaLeft.size() ; ++i_L )
			{
				NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] = 0.0;
			}
		}
	}

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
			float polweight = ( 1 + ePolarization_Sig * epol_target ) * ( 1 + pPolarization_Sig * ppol_target ) / 4.0 ;
			if ( ePolarization_Sig == 1 && pPolarization_Sig == 1 ) nEventPerPol = nRR_Sig;
			if ( ePolarization_Sig == -1 && pPolarization_Sig == -1) nEventPerPol = nLL_Sig;
			if ( ePolarization_Sig == 1 && pPolarization_Sig == -1) nEventPerPol = nRL_Sig;
			if ( ePolarization_Sig == -1 && pPolarization_Sig == 1) nEventPerPol = nLR_Sig;
			float weight = polweight * crossSection_Sig * lumi_target / nEventPerPol;
//			cout << "ePol = " << ePolarization_Sig << " , pPol = " << pPolarization_Sig << " , polWeight = " << polweight << " , crossSection = " << crossSection_Sig << " , nEventPerPol = " << nEventPerPol << " , weight = " << weight << std::endl;
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
//								cout << weight << " events added to M_AD(" << i_m << ") = " << darkPhotonMass[ i_m ] << " , #sigmaR(" << i_R << ") = " << NumberOfSigmaRight[ i_R ] << " , #sigmaL(" << i_L << ") = " << NumberOfSigmaLeft[ i_L ] << std::endl;
								NumberOfSignalEvents[ i_m ][ i_R ][ i_L ] += weight;
							}
						}
					}
				}
			}
		}
	}

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

}
