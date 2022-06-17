#include "loadNumberOfBackgroundEvents.h"
#include "loadNumberOfSignalEvents.h"

void OptimizeSearchWindow()
{
	ildStyle->SetOptStat(0);
	float logStep = 10000.0;
	float epol_target = -0.8;
	float ppol_target = +0.3;
	float lumi_target = 900.0;


	const int nMass = 23;
	const int nSigmaR = 30;
	const int nSigmaL = 30;
	int nSigR = 30;
	int nSigL = 30;

	vector<float> maxRangeAlpha={1.8,0.65,0.5,0.5,0.5,0.5,0.6,0.65,0.5,0.5,0.5,0.4,0.4,0.3,0.3,0.3,0.3,0.2,0.2,0.2,0.2,0.1,0.1};
	vector<int> colorCodes={kBlack,kRed,kGreen+2,kBlue+2,kMagenta+2,kBlack,kRed,kGreen+2,kBlue+2,kMagenta+2,kBlack,kRed,kGreen+2,kBlue+2,kMagenta+2,kBlack,kRed,kGreen+2,kBlue+2,kMagenta+2,kBlack,kRed,kGreen+2,kBlue+2,kMagenta+2,kBlack,kRed,kGreen+2,kBlue+2,kMagenta+2,kBlack,kRed,kGreen+2,kBlue+2,kMagenta+2};
	vector<int> markerStyle={4,4,4,4,4,25,25,25,25,25,26,26,26,26,26,28,28,28,28,28,30,30,30,30,30,32,32,32,32,32};
//	std::vector<float> NumberOfSigmaRight={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 };
//	std::vector<float> NumberOfSigmaLeft={ 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5 , 1.6 , 1.7 , 1.8 , 1.9 , 2.0 };
	std::vector<string> signalMass={ "10GeV" , "20GeV" , "30GeV" , "40GeV" , "50GeV" , "60GeV" , "70GeV" , "80GeV" , "100GeV" , "110GeV" , "120GeV" , "130GeV" , "140GeV" , "150GeV" , "160GeV" , "170GeV" , "180GeV" , "190GeV" , "200GeV" , "210GeV" , "220GeV" , "230GeV" , "240GeV" };
//	std::vector<float> darkPhotonMass={ 10.0 , 20.0 , 30.0 , 40.0 , 50.0 , 60.0 , 70.0 , 80.0 , 100.0 , 110.0 , 120.0 , 130.0 , 140.0 , 150.0 , 160.0 , 170.0 , 180.0 , 190.0 , 200.0 , 210.0 , 220.0 , 230.0 , 240.0 };

	double darkPhotonMass[ nMass ]{ 0.0 };
	double NumberOfSigmaRight[ nSigmaR ]{ 0.0 };
	double NumberOfSigmaLeft[ nSigmaL ]{ 0.0 };
	double NumberOfBackgroundEvents[ nMass ][ nSigmaR ][ nSigmaL ];
	double NumberOfSignalEvents[ nMass ][ nSigmaR ][ nSigmaL ];
	double totalNumberOfSignalEvents[ nMass ][ nSigmaR ][ nSigmaL ];
	double beta[ nMass ][ nSigmaR ][ nSigmaL ];
	double alpha[ nMass ][ nSigmaR ][ nSigmaL ];
	double bestAlpha[ nMass ];
	double bestNSigmaRight[ nMass ];
	double bestNSigmaLeft[ nMass ];
	double bestepsilonlimit[ nMass ];
	for ( int m = 0 ; m < nMass ; ++m )
	{
		bestAlpha[ m ] = 0.0;
		bestNSigmaRight[ m ] = 0.0;
		bestNSigmaLeft[ m ] = 0.0;
		for ( int r = 0 ; r < nSigmaR ; ++r )
		{
			for ( int l = 0 ; l < nSigmaL ; ++l )
			{
				NumberOfBackgroundEvents[ m ][ r ][ l ] = 0.0;
				NumberOfSignalEvents[ m ][ r ][ l ] = 0.0;
				totalNumberOfSignalEvents[ m ][ r ][ l ] = 0.0;
				beta[ m ][ r ][ l ] = 0.0;
				alpha[ m ][ r ][ l ] = 0.0;
			}
		}
	}


//	double darkPhotonMassResolution[ nMass ]{ 0.0 };
//	double darkPhotonMass[23];
	double AlphaPlus[ nMass ][ nSigmaR ][ nSigmaL ];
	double AlphaMinus[ nMass ][ nSigmaR ][ nSigmaL ];
	double bestAlphaPlus[ nMass ];
	double bestNumberOfSigmaRight[ nMass ];
	double bestNumberOfSigmaLeft[ nMass ];
	double bestNSigmaRatio[ nMass ];
	double bestSigmaRight[ nMass ];
	double bestSigmaLeft[ nMass ];
	double nSignalBackgroundRatio[ nMass ][ nSigmaR ][ nSigmaL ];

	loadDarkPhotonMasses( darkPhotonMass );
	loadNumberOfSigmaRights( NumberOfSigmaRight );
	loadNumberOfSigmaLefts( NumberOfSigmaLeft );
	loadNumberOfBackgroundEvents( NumberOfBackgroundEvents );
	loadNumberOfSignalEvents( NumberOfSignalEvents );
	loadTotalNumberOfSignalEvents( totalNumberOfSignalEvents );
	loadBeta( beta );


	TCanvas *can_alpha[ nMass ];
	TMultiGraph *tmg_alpha[ nMass ];
	TGraph *tgr_alpha[ nMass ][ nSigR ];
	TLegend *leg_alpha[ nMass ];

	double minAlpha = 1000000.0;
//	cout << "m,r,l,Nbackground,NSignal,Alpha"<< std::endl;
	for ( int m = 0 ; m < nMass ; ++m )
	{
		int darkPhotonMassInt = (int) darkPhotonMass[ m ];
		string head = ( "m_{A_{D}} = " + to_string( darkPhotonMassInt ) + " GeV" ).c_str();
		int nChar = head.length();
		char header[nChar + 1];
		strcpy(header, head.c_str());
		tmg_alpha[ m ] = new TMultiGraph();
		leg_alpha[ m ] = new TLegend( 0.1 , 0.05 * maxRangeAlpha[ m ] , NumberOfSigmaLeft[ nSigL - 2 ] , 0.35 * maxRangeAlpha[ m ] , header ,"");
//		leg_alpha[ m ] = new TLegend( 0.1 , 0.1 , NumberOfSigmaLeft[ nSigL - 2 ] , 0.35  , header ,"");
		leg_alpha[ m ]->SetNColumns( 5 );
		leg_alpha[ m ]->SetFillStyle( 0 );
		minAlpha = 1000000.0;
		for ( int r = 0 ; r < nSigR ; ++r )
		{
			for ( int l = 0 ; l < nSigL ; ++l )
			{
//				cout << "NumberOfBackgroundEvents[ " << m << " ][ " << r << " ][ " << l << " ] = " << NumberOfBackgroundEvents[ m ][ r ][ l ] << std::endl;
//				cout << "NumberOfSignalEvents[ " << m << " ][ " << r << " ][ " << l << " ] = " << NumberOfSignalEvents[ m ][ r ][ l ] << std::endl;
//				cout << "totalNumberOfSignalEvents[ " << m << " ][ " << r << " ][ " << l << " ] = " << totalNumberOfSignalEvents[ m ][ r ][ l ] << std::endl;
//				cout << "beta[ " << m << " ][ " << r << " ][ " << l << " ] = " << beta[ m ][ r ][ l ] << std::endl;
				alpha[ m ][ r ][ l ] = 2.0 * ( 1.0 + sqrt( 1.0 + NumberOfBackgroundEvents[ m ][ r ][ l ] ) ) / NumberOfSignalEvents[ m ][ r ][ l ];
//				cout << m << "," << r << "," << l << "," << NumberOfBackgroundEvents[ m ][ r ][ l ] << "," << NumberOfSignalEvents[ m ][ r ][ l ] << "," << alpha[ m ][ r ][ l ] << std::endl;
				if ( alpha[ m ][ r ][ l ] <= minAlpha )
				{
					minAlpha = alpha[ m ][ r ][ l ];
					bestAlpha[ m ] = alpha[ m ][ r ][ l ];
					bestNSigmaRight[ m ] = NumberOfSigmaRight[ r ];
					bestNSigmaLeft[ m ] = NumberOfSigmaLeft[ l ];
					double epsilonlimit = sqrt( minAlpha ) * 0.01;
					bestepsilonlimit[ m ] = sqrt( alpha[ m ][ r ][ l ] ) * 0.01;
				}
//					cout << epsilonlimit << std::endl;
			}
			tgr_alpha[ m ][ r ] = new TGraph( nSigL , NumberOfSigmaLeft , alpha[ m ][ r ] );
			tgr_alpha[ m ][ r ]->SetDrawOption("AP");
			tgr_alpha[ m ][ r ]->SetLineColor( colorCodes[ r ] );
			tgr_alpha[ m ][ r ]->SetMarkerColor( colorCodes[ r ] );
			tgr_alpha[ m ][ r ]->SetMarkerStyle( markerStyle[ r ] );
			tgr_alpha[ m ][ r ]->SetLineWidth( 1 );
			tgr_alpha[ m ][ r ]->SetMarkerSize( 1.0 );
			tmg_alpha[ m ]->Add( tgr_alpha[ m ][ r ] );
			
			string graphName = ("##sigma_{R} = " + to_string( NumberOfSigmaRight[ r ] ) ).c_str();
			int nCharTitle = graphName.length();
			char char_title[nCharTitle + 1];
			strcpy(char_title, graphName.c_str());
			leg_alpha[ m ]->AddEntry( tgr_alpha[ m ][ r ] , char_title , "LP" );
		}
		tmg_alpha[ m ]->GetHistogram()->GetXaxis()->SetRangeUser( 0.0 , NumberOfSigmaLeft[ nSigL - 1 ] );
		tmg_alpha[ m ]->GetHistogram()->GetXaxis()->SetTitle( "##sigmaL" );
		tmg_alpha[ m ]->GetHistogram()->GetYaxis()->SetRangeUser( 0.0 , maxRangeAlpha[ m ] );
		tmg_alpha[ m ]->GetHistogram()->GetYaxis()->SetTitle( "#alpha" );
		tmg_alpha[ m ]->GetHistogram()->SetTitle( ("m_{A_{D}} = " + to_string( darkPhotonMassInt ) + "GeV").c_str() );		
		can_alpha[ m ] = new TCanvas( header , "" , 800 , 800 );
		tmg_alpha[ m ]->Draw("alp");
		gPad->Update();
		can_alpha[ m ]->Update();
		leg_alpha[ m ]->Draw("same");
		gPad->Update();
		can_alpha[ m ]->Update();
		can_alpha[ m ]->SaveAs( ( "Plots/alpha_m_" + to_string( darkPhotonMassInt ) + "_GeV.pdf" ).c_str() );
		can_alpha[ m ]->SaveAs( ( "Plots/alpha_m_" + to_string( darkPhotonMassInt ) + "_GeV.root" ).c_str() );
		tmg_alpha[ m ]->GetHistogram()->GetYaxis()->SetRangeUser( bestAlpha[ m ] - 0.05 * bestAlpha[ m ] , bestAlpha[ m ] + 0.05 * bestAlpha[ m ] );
		tmg_alpha[ m ]->GetHistogram()->GetXaxis()->SetRangeUser( bestNSigmaLeft[ m ] - 0.35 , bestNSigmaLeft[ m ] + 0.35 );
		tmg_alpha[ m ]->GetHistogram()->SetTitle( ("m_{A_{D}} = " + to_string( darkPhotonMassInt ) + "GeV").c_str() );
		tmg_alpha[ m ]->GetYaxis()->SetLabelSize(0.05);
		tmg_alpha[ m ]->GetYaxis()->SetTitleOffset(1.7);
		tmg_alpha[ m ]->GetXaxis()->SetTitleOffset(0.8);
		TEllipse *circle = new TEllipse( bestNSigmaLeft[ m ] , bestAlpha[ m ] , 0.35 / 20.0 , 0.003 * bestAlpha[ m ] );
		circle->SetLineColor( kYellow + 1 );
		circle->SetFillStyle( 0 );
		circle->Draw("same");
		string bestAlphaText = ( "#alpha_{min} = " + to_string( bestAlpha[ m ] ) ).c_str();
		int n = bestAlphaText.length();
		char t[n + 1];
		strcpy(t, bestAlphaText.c_str());
		TLatex *latexBestAlpha = new TLatex( bestNSigmaLeft[ m ] + 2.0 * 0.35 / 20.0 , bestAlpha[ m ] - 2.0 * 0.003 * bestAlpha[ m ] , t );
		latexBestAlpha->SetTextSize(0.03);
		latexBestAlpha->Draw();
		int l = (int)( bestNSigmaLeft[ m ] );
		int ll = (int)( bestNSigmaLeft[ m ] * 10 - l * 10 );
		int r = (int)( bestNSigmaRight[ m ] );
		int rr = (int)( bestNSigmaRight[ m ] * 10 - r * 10 );
		string textBestSigma = ( "##sigma_{L} = " + to_string( l ) + "." + to_string( ll ) + " , ##sigma_{R} = " + to_string( r ) + "." + to_string( rr ) ).c_str();
		int nn = textBestSigma.length();
		char tt[nn + 1];
		strcpy(tt, textBestSigma.c_str());
		TLatex *latexBestsigma = new TLatex( bestNSigmaLeft[ m ] + 2.0 * 0.35 / 20.0 , bestAlpha[ m ] - 4.5 * 0.003 * bestAlpha[ m ] , tt );
		latexBestsigma->SetTextSize(0.03);
		latexBestsigma->Draw();

		gPad->Update();
		can_alpha[ m ]->Update();
		can_alpha[ m ]->Modified();
		can_alpha[ m ]->SaveAs( ( "Plots/alpha_m_" + to_string( darkPhotonMassInt ) + "_GeV_zoomed.pdf" ).c_str() );
		can_alpha[ m ]->SaveAs( ( "Plots/alpha_m_" + to_string( darkPhotonMassInt ) + "_GeV_zoomed.root" ).c_str() );
		
		
		TGraph *bestNumberSigmaRight = new TGraph( nMass , darkPhotonMass , bestNSigmaRight );
		TCanvas *can_bestNSigmaRight = new TCanvas( "can_bestNSigmaRight" , "" , 800 , 800 );
		bestNumberSigmaRight->SetTitle( ";m_{A_{D}} [GeV];best ##sigma_{R}" );
		bestNumberSigmaRight->GetXaxis()->SetRangeUser( 0.0 , 260.0 );
		bestNumberSigmaRight->GetYaxis()->SetRangeUser( 0.0 , 1.8 );
		bestNumberSigmaRight->GetYaxis()->SetTitleOffset(1.3);
		bestNumberSigmaRight->Draw("al");
		gPad->SetTopMargin(0.014);
		gPad->SetRightMargin(0.014);
		gPad->SetLeftMargin(0.20);
		gPad->SetBottomMargin(0.15);
		can_bestNSigmaRight->SaveAs( "Plots/bestNumberSigmaRight.pdf" );
		can_bestNSigmaRight->SaveAs( "Plots/bestNumberSigmaRight.root" );	
		
		TGraph *bestNumberSigmaLeft = new TGraph( nMass , darkPhotonMass , bestNSigmaLeft );
		TCanvas *can_bestNSigmaLeft = new TCanvas( "can_bestNSigmaLeft" , "" , 800 , 800 );
		bestNumberSigmaLeft->SetTitle( ";m_{A_{D}} [GeV];best ##sigma_{L}" );
		bestNumberSigmaLeft->GetXaxis()->SetRangeUser( 0.0 , 260.0 );
		bestNumberSigmaLeft->GetYaxis()->SetRangeUser( 0.0 , 3.0 );
		bestNumberSigmaLeft->GetYaxis()->SetTitleOffset(1.3);
		bestNumberSigmaLeft->Draw("al");
		gPad->SetTopMargin(0.014);
		gPad->SetRightMargin(0.014);
		gPad->SetLeftMargin(0.20);
		gPad->SetBottomMargin(0.15);
		can_bestNSigmaLeft->SaveAs( "Plots/bestNumberSigmaLeft.pdf" );
		can_bestNSigmaLeft->SaveAs( "Plots/bestNumberSigmaLeft.root" );	
		
		TGraph *epsilonlimit = new TGraph( nMass , darkPhotonMass , bestepsilonlimit );
		TCanvas *can_epsilon = new TCanvas( "can_epsilon" , "" , 800 , 800 );
		epsilonlimit->SetTitle( ";m_{A_{D}} [GeV];#epsilon_{limit}" );
		epsilonlimit->GetXaxis()->SetRangeUser( 0.0 , 260.0 );
		epsilonlimit->GetYaxis()->SetRangeUser( 0.0 , 0.01 );
		epsilonlimit->GetYaxis()->SetTitleOffset(1.3);
		epsilonlimit->Draw("al");
		gPad->SetTopMargin(0.014);
		gPad->SetRightMargin(0.014);
		gPad->SetLeftMargin(0.20);
		gPad->SetBottomMargin(0.15);
		can_epsilon->SaveAs( "Plots/epsilonlimit.pdf" );
		can_epsilon->SaveAs( "Plots/epsilonlimit.root" );	
			
		
		TGraph *BestAlpha = new TGraph( nMass , darkPhotonMass , bestAlpha );
		TCanvas *can_bestAlpha = new TCanvas( "can_bestAlpha" , "" , 800 , 800 );
		BestAlpha->SetTitle( ";m_{A_{D}} [GeV];best#alpha" );
		BestAlpha->GetXaxis()->SetRangeUser( 0.0 , 260.0 );
		BestAlpha->GetYaxis()->SetRangeUser( 0.0 , 0.4 );
		BestAlpha->GetYaxis()->SetTitleOffset(1.3);
		BestAlpha->Draw("al");
		gPad->SetTopMargin(0.014);
		gPad->SetRightMargin(0.014);
		gPad->SetLeftMargin(0.20);
		gPad->SetBottomMargin(0.15);
		can_bestAlpha->SaveAs( "Plots/bestAlpha.pdf" );
		can_bestAlpha->SaveAs( "Plots/bestAlpha.root" );	
			
	}




/*

	TLegend *leg_nSignalBackgroundRatio[ nMass ];
	vector<string> physicsProcess={ "2f_leptonic" , "other" , "4f_singleZee_leptonic" };
	//2f_leptonic, 4f_singleZee_leptonic, 4f_WW_leptonic, 4f_ZZ_leptonic, 4f_ZZnunu_leptonic, 4f_ZZ_semileptonic, 4f_ZZWWMix_leptonic, 4f_singleZnunu_leptonic
	string rootFileDir = "/nfs/dust/ilc/user/hosseins/MarlinWorkdir/DiMuonProcessor/Background/root";
	string rootFileDirSignal = "/nfs/dust/ilc/user/hosseins/DarkPhoton/rec/root";

	cout << "Processing " << nMass << " Signal Files" << std::endl;
//	double minAlpha = 1000000.0;
	for ( unsigned int i_m = 0 ; i_m < nMass ; ++ i_m )
	{
		TFile *rootFile = new TFile( ( rootFileDirSignal + "/" + signalMass[ i_m ] + "_DST_DiMuonInvMass_1.root" ).c_str() );
		cout << "Reading File: " << ( rootFileDirSignal + "/" + signalMass[ i_m ] + "_DST_DiMuonInvMass_1.root" ).c_str() << endl;
		TTree *tree = (TTree*)rootFile->Get("DiMuons");

		int darkPhotonMassInt = (int) darkPhotonMass[ i_m ];
		string head = ( "m_{A_{D}} = " + to_string( darkPhotonMassInt ) + " GeV , #sigma_{m} = " + to_string( NumberOfSigmaRight[ i_m ] ) + " GeV" ).c_str();
		int nChar = head.length();
		char header[nChar + 1];
		strcpy(header, head.c_str());
		tmg_nSignalBackgroundRatio[ i_m ] = new TMultiGraph();
		tmg_nSignalBackgroundRatio[ i_m ]->GetHistogram()->GetXaxis()->SetLimits( 0.0 , NumberOfSigmaLeft[ nSigL - 1 ] );
		tmg_nSignalBackgroundRatio[ i_m ]->GetHistogram()->GetYaxis()->SetRangeUser( 0.0 , maxRangeNevent[ i_m ] );
		leg_nSignalBackgroundRatio[ i_m ] = new TLegend( 0.2 , 0.05 * maxRangeNevent[ i_m ] , NumberOfSigmaLeft[ nSigL - 2 ] , 0.5 * maxRangeNevent[ i_m ] , header ,"");
		leg_nSignalBackgroundRatio[ i_m ]->SetNColumns( 5 );
		leg_nSignalBackgroundRatio[ i_m ]->SetFillStyle( 0 );


		tmg_alpha[ i_m ] = new TMultiGraph();
		tmg_alpha[ i_m ]->GetHistogram()->GetXaxis()->SetLimits( 0.0 , NumberOfSigmaLeft[ nSigL - 1 ] );
//		leg_alpha[ i_m ] = new TLegend( 0.1 , 0.05 * maxRangeAlpha[ i_m ] , NumberOfSigmaLeft[ nSigL - 2 ] , 0.35 * maxRangeAlpha[ i_m ] , header ,"");
//		leg_alpha[ i_m ]->SetNColumns( 5 );
//		leg_alpha[ i_m ]->SetFillStyle( 0 );
//		minAlpha = 1000000.0;

		int nEntries = tree->GetEntries();
		for ( unsigned int i_R = 0 ; i_R < nSigR ; ++i_R )
		{
			for ( unsigned int i_L = 0 ; i_L < nSigL ; ++i_L )
			{
				double nSignal = NumberOfSignalEvents[ i_m ][ i_R ][ i_L ];
				double nBackGround = NumberOfBackgroundEvents[ i_m ][ i_R ][ i_L ];
				double ratio = nSignal / sqrt( nSignal + nBackGround );
//				cout << "Mass = " << darkPhotonMassInt << " GeV ,	nSigmaRight = " << nSigmaRight[ i_R ] << " ,	nSigmaLeft = " << nSigmaLeft[ i_L ] << " ,	 Nsignal = " << nSignal << " ,	Nbackground = " << nBackGround << " ,	Ratio = " << ratio << std::endl;
				nSignalBackgroundRatio[ i_m ][ i_R ][ i_L ] = ratio;
				AlphaPlus[ i_m ][ i_R ][ i_L ] = 2.0 / nSignal + 2.0 * sqrt( 1 + nBackGround ) / nSignal;
				AlphaMinus[ i_m ][ i_R ][ i_L ] = 2.0 / nSignal - 2.0 * sqrt( 1 + nBackGround ) / nSignal;
				if ( AlphaPlus[ i_m ][ i_R ][ i_L ] <= minAlpha )
				{
					minAlpha = AlphaPlus[ i_m ][ i_R ][ i_L ];
					bestAlphaPlus[ i_m ] = AlphaPlus[ i_m ][ i_R ][ i_L ];
					bestNumberOfSigmaRight[ i_m ] = NumberOfSigmaRight[ i_R ];
					bestNumberOfSigmaLeft[ i_m ] = NumberOfSigmaLeft[ i_L ];
					bestNSigmaRatio[ i_m ] = bestNumberOfSigmaLeft[ i_m ] / bestNumberOfSigmaRight[ i_m ];
//					bestSigmaRight[ i_m ] = NumberOfSigmaRight[ i_R ] * darkPhotonMassResolution[ i_m ];
//					bestSigmaLeft[ i_m ] = NumberOfSigmaLeft[ i_L ] * darkPhotonMassResolution[ i_m ];
				}


			}
			tgr_alpha[ i_m ][ i_R ] = new TGraph( nSigL , NumberOfSigmaLeft , AlphaPlus[ i_m ][ i_R ] );
			tgr_alpha[ i_m ][ i_R ]->SetDrawOption("AP");
			tmg_alpha[ i_m ]->Add( tgr_alpha[ i_m ][ i_R ] );


		}

		can_alpha[ i_m ] = new TCanvas( header , "" , 800 , 800 );
		tmg_alpha[ i_m ]->Draw("alp");
		gPad->Update();
		can_alpha[ i_m ]->Update();
//		leg_alpha[ i_m ]->Draw("same");
		gPad->Update();
		can_alpha[ i_m ]->Update();
		can_alpha[ i_m ]->SaveAs( ( "Plots/alpha_m_" + to_string( darkPhotonMassInt ) + "_GeV.pdf" ).c_str() );
		can_alpha[ i_m ]->SaveAs( ( "Plots/alpha_m_" + to_string( darkPhotonMassInt ) + "_GeV.root" ).c_str() );
		tmg_alpha[ i_m ]->GetHistogram()->GetYaxis()->SetRangeUser( bestAlphaPlus[ i_m ] - 0.05 * bestAlphaPlus[ i_m ] , bestAlphaPlus[ i_m ] + 0.05 * bestAlphaPlus[ i_m ] );
		tmg_alpha[ i_m ]->GetHistogram()->GetXaxis()->SetRangeUser( bestNumberOfSigmaLeft[ i_m ] - 0.35 , bestNumberOfSigmaLeft[ i_m ] + 0.35 );
		tmg_alpha[ i_m ]->GetYaxis()->SetLabelSize(0.05);
		tmg_alpha[ i_m ]->GetYaxis()->SetTitleOffset(1.7);
		tmg_alpha[ i_m ]->GetXaxis()->SetTitleOffset(0.8);
		TEllipse *circle = new TEllipse( bestNumberOfSigmaLeft[ i_m ] , bestAlphaPlus[ i_m ] , 0.35 / 20.0 , 0.003 * bestAlphaPlus[ i_m ] );
		circle->SetLineColor( kYellow + 1 );
		circle->SetFillStyle( 0 );
		circle->Draw("same");
		string text = ( "#alpha_{min} = " + to_string( bestAlphaPlus[ i_m ] ) ).c_str();
		int n = text.length();
		char t[n + 1];
		strcpy(t, text.c_str());
		TLatex *latexBestAlpha = new TLatex( bestNumberOfSigmaLeft[ i_m ] + 2.0 * 0.35 / 20.0 , bestAlphaPlus[ i_m ] - 2.0 * 0.003 * bestAlphaPlus[ i_m ] , t );
		latexBestAlpha->SetTextSize(0.03);
		latexBestAlpha->Draw();
		int l = (int)( bestNumberOfSigmaLeft[ i_m ] );
		int ll = (int)( bestNumberOfSigmaLeft[ i_m ] * 10 - l * 10 );
		int r = (int)( bestNumberOfSigmaRight[ i_m ] );
		int rr = (int)( bestNumberOfSigmaRight[ i_m ] * 10 - r * 10 );
		string textBestSigma = ( "##sigma_{L} = " + to_string( l ) + "." + to_string( ll ) + " , ##sigma_{R} = " + to_string( r ) + "." + to_string( rr ) ).c_str();
		int nn = textBestSigma.length();
		char tt[nn + 1];
		strcpy(tt, textBestSigma.c_str());
		TLatex *latexBestsigma = new TLatex( bestNumberOfSigmaLeft[ i_m ] + 2.0 * 0.35 / 20.0 , bestAlphaPlus[ i_m ] - 3.5 * 0.003 * bestAlphaPlus[ i_m ] , tt );
		latexBestsigma->SetTextSize(0.03);
		latexBestsigma->Draw();

//		TText *textBestAlpha = new TText( bestNumberOfSigmaLeft[ i_m ] + 2.0 * 0.35 / 20.0 , bestAlphaPlus[ i_m ] - 2.0 * 0.003 * bestAlphaPlus[ i_m ] , t );
//		textBestAlpha->SetTextSize(0.03);
//		textBestAlpha->Draw();
		gPad->Update();
		can_alpha[ i_m ]->Update();
		can_alpha[ i_m ]->Modified();
		can_alpha[ i_m ]->SaveAs( ( "Plots/alpha_m_" + to_string( darkPhotonMassInt ) + "_GeV_zoomed.pdf" ).c_str() );
		can_alpha[ i_m ]->SaveAs( ( "Plots/alpha_m_" + to_string( darkPhotonMassInt ) + "_GeV_zoomed.root" ).c_str() );


	}
	TGraph *gr_bestAlpha = new TGraph( nMass , darkPhotonMass , bestAlphaPlus );

	TCanvas *can_bestAlpha = new TCanvas( "can_bestAlpha" , "" , 800 , 800 );
	gr_bestAlpha->SetTitle( ";m_{A_{D}} [GeV];best #alpha^{+}" );
	gr_bestAlpha->GetXaxis()->SetRangeUser( 0.0 , 260.0 );
	gr_bestAlpha->GetYaxis()->SetRangeUser( 0.0 , 12.5 );
	gr_bestAlpha->GetYaxis()->SetTitleOffset(1.3);
	gr_bestAlpha->Draw("al");
	gPad->SetTopMargin(0.014);
	gPad->SetRightMargin(0.014);
	gPad->SetLeftMargin(0.20);
	gPad->SetBottomMargin(0.15);
	can_bestAlpha->SaveAs( "Plots/bestAlpha.pdf" );
	can_bestAlpha->SaveAs( "Plots/bestAlpha.root" );
*/
}
