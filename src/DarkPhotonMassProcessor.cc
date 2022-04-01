#include "DarkPhotonMassProcessor.h"
#include <iostream>
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/MCParticle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TF1.h"
#include "TTree.h"
#include "EVENT/Track.h"
#include "TLorentzVector.h"
#include "EVENT/LCCollection.h"
#include "TMVA/Reader.h"
/*
include relevant classes here
*/

using namespace lcio ;
using namespace marlin ;

DarkPhotonMassProcessor aDarkPhotonMassProcessor;

DarkPhotonMassProcessor::DarkPhotonMassProcessor() :
        Processor("DarkPhotonMassProcessor"),
        m_TrueDarkPhotonMass(0.f),
	m_nRun(0),
	m_nEvt(0),
	m_nRunSum(0),
	m_nEvtSum(0),
	m_polarizatione(0.f),
	m_polarizationp(0.f),
	polweight(0.f),
	m_crossSection(0.f),
	epol_target(-0.8),
	ppol_target(0.3),
	lumi_target(900.0),
	m_nPfos(0),
	m_PfoMuonPDG{},
	m_MCMuonPDG{},
	m_IsoMuonPDG{},
	m_nIsoLeptons(0),
	m_nMCMuons(0),
	m_nMCAntiMuons(0),
	m_nMCDiMuons(0),
	m_nRecoMuons(0),
	m_nRecoAntiMuons(0),
	m_nRecoDiMuons(0),
	m_nDiMuonIsoLeptons(0),
	m_nTracks(0),
        m_nMCTruthTracks(0),
	m_nMCParticles(0)


{
	registerInputCollection(	LCIO::RECONSTRUCTEDPARTICLE,
					"PfoCollection",
					"Name of input pfo collection",
					m_PfoCollection,
					std::string("PandoraPFOs")
					);

	registerInputCollection(	LCIO::MCPARTICLE,
					"MCParticleCollection",
					"Name of input MC collection",
					m_MCCollection, //shall be defined in .h file
					std::string("MCParticlesSkimmed")
				);

	registerInputCollection(	LCIO::RECONSTRUCTEDPARTICLE,
					"IsoLepCollection",
					"Name of input Isolated Lepton collection",
					m_IsoLepCollection,
					std::string("ISOMuons")
					);


	registerInputCollection(	LCIO::TRACK,
        	                        "Trackcollection",
        	                        "Name of input Track collection",
        	                        m_TrackCollection,
        	                        std::string("MarlinTrkTracks")
        	                        );


	registerInputCollection(	LCIO::LCRELATION,
        	                        "MCTruthTrackLinkCollection",
        	                        "Name of input Track collection",
        	                        m_MCTruthTrackLinkCollection,
        	                        std::string("MCTruthMarlinTrkTracksLink")
        	                        );

        registerInputCollection(	LCIO::LCRELATION,
                                        "TrackMCTruthLinkCollection",
                                        "Name of input Track collection",
                                        m_TrackMCTruthLinkCollection,
                                        std::string("MarlinTrkTracksMCTruthLink")
                                        );
/*
				register input/output collections here

	registerOutputCollection(	LCIO::<OutputCollectionType>,
					"OutputCollectonName",
					"Description of output Collection",
					m_OutputCollection, //shall be defined in .h file
					std::string("<Name of Output Collection in LCIO>")
				);

*/

// use for adding Cut
        registerProcessorParameter(	"RootFile",
        				"Name of the output root file",
        				m_rootFile,
        				std::string("Output.root")
        				);
        registerProcessorParameter(     "trueDarkPhotonMass",
                                        "true mass of dark photon"  ,
                                       	m_TrueDarkPhotonMass ,
                                       	float(0.0)
                                        ) ;

/*        registerProcessorParameter(     "CosConeSmall",
                                        "cosine of the smaller cone"  ,
                                       	_cosConeSmall ,
                                       	float(0.98)
                                        ) ;
        registerProcessorParameter(     "CosConeLarge",
                                        "cosine of the larger cone"  ,
                                       	_cosConeLarge ,
                                       	float(0.95)
                                        ) ;
*/
}

void DarkPhotonMassProcessor::init()
{
	streamlog_out(DEBUG) << "	init called  " << std::endl;
	printParameters();
	m_nEvtSum=0;
	m_pTFile = new TFile(m_rootFile.c_str(), "recreate");
	m_pTTree = new TTree("DiMuons", "DiMuons");
	m_pTTree->SetDirectory(m_pTFile);
        m_pTTree->Branch("nRun", &m_nRun, "nRun/I");
        m_pTTree->Branch("nEvt", &m_nEvt, "nEvt/I");
        m_pTTree->Branch("nRunSum", &m_nRunSum, "nRunSum/I");
	m_pTTree->Branch("nEvtSum", &m_nEvtSum, "nEvtSum/I");
        m_pTTree->Branch("ePolarization", &m_polarizatione, "ePolarization/F");
        m_pTTree->Branch("pPolarization", &m_polarizationp, "pPolarization/F");
        m_pTTree->Branch("CrossSection", &m_crossSection, "CrossSection/F");
        m_pTTree->Branch("nPFOs", &m_nPfos, "nPFOs/I");
        m_pTTree->Branch("nIsoLeptons", &m_nIsoLeptons, "nIsoLeptons/I");
	m_pTTree->Branch("nMCParticles", &m_nMCParticles, "nMCParticles/I");
	m_pTTree->Branch("nTracks", &m_nTracks, "nTracks/I");
        m_pTTree->Branch("track1_isFound", &m_track1_isFound);
        m_pTTree->Branch("track2_isFound", &m_track2_isFound);
        m_pTTree->Branch("nMCTruthTracks", &m_nMCTruthTracks, "nMCTruthTracks/I");
	m_pTTree->Branch("nMCMuons", &m_nMCMuons, "nMCMuons/I");
	m_pTTree->Branch("nMCAntiMuons", &m_nMCAntiMuons, "nMCAntiMuons/I");
	m_pTTree->Branch("nMCDiMuons", &m_nMCDiMuons, "nMCDiMuons/I");
	m_pTTree->Branch("nRecoMuons", &m_nRecoMuons, "nRecoMuons/I");
	m_pTTree->Branch("nRecoAntiMuons", &m_nRecoAntiMuons, "nRecoAntiMuons/I");
	m_pTTree->Branch("nRecoDiMuons", &m_nRecoDiMuons, "nRecoDiMuons/I");
	m_pTTree->Branch("nDiMuonIsoLeptons", &m_nDiMuonIsoLeptons, "nDiMuonIsoLeptons/I");
	m_pTTree->Branch("MCMuonPDG", &m_MCMuonPDG);
	m_pTTree->Branch("IsoMuonPDG", &m_IsoMuonPDG);
	m_pTTree->Branch("MCDiMuonInvMass", &m_MCDiMuonInvMass);
	m_pTTree->Branch("RecoDiMuonInvMass", &m_RecoDiMuonInvMass);
	m_pTTree->Branch("IsoDiMuonInvMass", &m_IsoDiMuonInvMass);
	m_pTTree->Branch("MCDiMuonCostheta",&m_MCDiMuonCostheta);
	m_pTTree->Branch("RecoDiMuonCostheta",&m_RecoDiMuonCostheta);
        m_pTTree->Branch("MCMuon1Costheta",&m_MCMuon1Costheta);
        m_pTTree->Branch("MCMuon2Costheta",&m_MCMuon2Costheta);
        m_pTTree->Branch("MCMuonMaxCostheta",&m_MCMuonMaxCostheta);
        m_pTTree->Branch("lostMuonTheta",&m_lostMuonTheta);
	m_pTTree->Branch("IsoDiMuonCostheta",&m_IsoDiMuonCostheta);
        m_pTTree->Branch("MCMuonTotalMomentum1", &m_MCMuonTotalMomentum1);
        m_pTTree->Branch("MCMuonTotalMomentum2", &m_MCMuonTotalMomentum2);
        m_pTTree->Branch("RecoMuonTotalMomentum1", &m_RecoMuonTotalMomentum1);
        m_pTTree->Branch("RecoMuonTotalMomentum2", &m_RecoMuonTotalMomentum2);
        m_pTTree->Branch("PtMCParticle1", &m_PtMCParticle1);
        m_pTTree->Branch("PtMCParticle2", &m_PtMCParticle2);
        m_pTTree->Branch("PtRecoParticle1", &m_PtRecoParticle1);
        m_pTTree->Branch("PtRecoParticle2", &m_PtRecoParticle2);
        m_pTTree->Branch("RecoMuon1Costheta", &m_RecoMuon1Costheta);
        m_pTTree->Branch("RecoMuon2Costheta", &m_RecoMuon2Costheta);
        m_pTTree->Branch("MCMuon1Costheta", &m_MCMuon1Costheta);
        m_pTTree->Branch("MCMuon2Costheta", &m_MCMuon2Costheta);
        m_pTTree->Branch("RecoMuon1theta", &m_RecoMuon1theta);
        m_pTTree->Branch("RecoMuon2theta", &m_RecoMuon2theta);
        m_pTTree->Branch("MCMuon1theta", &m_MCMuon1theta);
        m_pTTree->Branch("MCMuon2theta", &m_MCMuon2theta);
        m_pTTree->Branch("EnergyRecoParticle1", &m_EnergyRecoParticle1);
        m_pTTree->Branch("EnergyRecoParticle2", &m_EnergyRecoParticle2);
        m_pTTree->Branch("EnergyMCParticle1", &m_EnergyMCParticle1);
        m_pTTree->Branch("EnergyMCParticle2", &m_EnergyMCParticle2);
        m_pTTree->Branch("PfoMuonTrackOmega1", &m_PfoMuonTrackOmega1);
        m_pTTree->Branch("PfoMuonTrackTanLambda1", &m_PfoMuonTrackTanLambda1);
        m_pTTree->Branch("PfoMuonTrackPhi1", &m_PfoMuonTrackPhi1);
        m_pTTree->Branch("PfoMuonTrackOmega2", &m_PfoMuonTrackOmega2);
        m_pTTree->Branch("PfoMuonTrackTanLambda2", &m_PfoMuonTrackTanLambda2);
        m_pTTree->Branch("PfoMuonTrackPhi2", &m_PfoMuonTrackPhi2);
        m_pTTree->Branch("MassResolution", &m_MassResolution);
        m_pTTree->Branch("TrueDarkPhotonMass", &m_TrueDarkPhotonMass);
        m_pTTree->Branch("ResidualDarkPhotonMass", &m_ResidualDarkPhotonMass);
        m_pTTree->Branch("NormalizedResidualDarkPhotonMass", &m_NormalizedResidualDarkPhotonMass);


	MCDiMuonInvMass = new TH1F("MCDiMuonInvMass",";m_{#mu#bar{#mu}} [GeV]",240,0.0,250.0);
	RecoDiMuonInvMass = new TH1F("RecoDiMuonInvMass",";m_{#mu#bar{#mu}} [GeV]",250,0.0,250.0);
	IsoDiMuonInvMass = new TH1F("IsoDiMuonInvMass",";m_{#mu#bar{#mu}} [GeV]",250,0.0,250.0);
	h_ResidualDarkPhotonMass = new TH1F("h_ResidualDarkPhotonMass",";m_{#mu#bar{#mu}}^{REC} - m_{A_{D}} [GeV]",2000,-10.0,10.0);
	h_NormalizedResidualDarkPhotonMass = new TH1F("h_NormalizedResidualDarkPhotonMass",";( m_{#mu#bar{#mu}}^{REC} - m_{A_{D}} ) / #sigma_{m}",200,-10.0,10.0);
        this->Clear();
}

void DarkPhotonMassProcessor::Clear()
{
	m_PhysicsProcess = "";
	m_PfoMuonPDG.clear();
	m_nPfos = 0;
	m_MCMuonPDG.clear();
	m_IsoMuonPDG.clear();
	m_nIsoLeptons = 0;
	m_nMCParticles = 0;
	m_nTracks = 0;
        m_nMCTruthTracks = 0;
        m_crossSection = 0.0;
	m_polarizatione = 0;
	m_polarizationp = 0;
	m_nMCMuons = 0;
	m_nMCAntiMuons = 0;
	m_nMCDiMuons = 0;
	m_nRecoMuons = 0;
	m_nRecoAntiMuons = 0;
	m_nRecoDiMuons = 0;
	m_nDiMuonIsoLeptons = 0;
	m_MCDiMuonInvMass.clear();
	m_RecoDiMuonInvMass.clear();
        m_RecoMuonTotalMomentum1.clear();
        m_RecoMuonTotalMomentum2.clear();
        m_MCMuonTotalMomentum1.clear();
        m_MCMuonTotalMomentum2.clear();
        m_PtRecoParticle1.clear();
        m_PtRecoParticle2.clear();
        m_PtMCParticle1.clear();
        m_PtMCParticle2.clear();
        m_MCMuon1Costheta.clear();
        m_MCMuon2Costheta.clear();
        m_RecoMuon1Costheta.clear();
        m_RecoMuon2Costheta.clear();
        m_MCMuon1theta.clear();
        m_MCMuon2theta.clear();
        m_RecoMuon1theta.clear();
        m_RecoMuon2theta.clear();
        m_EnergyRecoParticle1.clear();
        m_EnergyRecoParticle2.clear();
        m_EnergyMCParticle1.clear();
        m_EnergyMCParticle2.clear();
        m_PfoMuonTrackOmega1.clear();
        m_PfoMuonTrackTanLambda1.clear();
        m_PfoMuonTrackPhi1.clear();
        m_PfoMuonTrackOmega2.clear();
        m_PfoMuonTrackTanLambda2.clear();
        m_PfoMuonTrackPhi2.clear();
//        m_IsoMuonTotalMomentum1.clear();
//        m_IsoMuonTotalMomentum2.clear();
	m_IsoDiMuonInvMass.clear();
	m_MCDiMuonCostheta.clear();
        m_RecoDiMuonCostheta.clear();
        m_MCMuon1Costheta.clear();
        m_MCMuon2Costheta.clear();
        m_MCMuonMaxCostheta.clear();
        m_lostMuonTheta.clear();
	m_IsoDiMuonCostheta.clear();
        m_track1_isFound.clear();
        m_track2_isFound.clear();
	m_MassResolution.clear();
	m_trueDarkPhotonMass.clear();
	m_ResidualDarkPhotonMass.clear();
	m_NormalizedResidualDarkPhotonMass.clear();

}

void DarkPhotonMassProcessor::processRunHeader()
{
	m_nRun = 0;
	m_nEvt = 0;
	++m_nRunSum;
}

void DarkPhotonMassProcessor::processEvent( EVENT::LCEvent *pLCEvent )
{
	try
	{
		this->Clear();
		m_nEvt = pLCEvent->getEventNumber();
		const EVENT::LCCollection *PfoCollection = pLCEvent->getCollection(m_PfoCollection);
		m_nPfos = PfoCollection->getNumberOfElements();
		const EVENT::LCCollection *MCCollection = pLCEvent->getCollection(m_MCCollection);
		m_nMCParticles = MCCollection->getNumberOfElements();
		const EVENT::LCCollection *Trackcollection = pLCEvent->getCollection(m_TrackCollection);
		m_nTracks = Trackcollection->getNumberOfElements();

        	LCRelationNavigator TrackMCParticleNav( pLCEvent->getCollection( m_TrackMCTruthLinkCollection ) );
                LCRelationNavigator MCParticleTrackNav( pLCEvent->getCollection( m_MCTruthTrackLinkCollection ) );

		m_crossSection = pLCEvent->getParameters().getFloatVal("crossSection");
		m_polarizatione = pLCEvent->getParameters().getFloatVal("beamPol1");
		m_polarizationp = pLCEvent->getParameters().getFloatVal("beamPol2");
//               double coneEnergy = 0.0;
//                double coneEnergyCharged = 0.0;
//                double coneEnergyNeutral = 0.0;
                TLorentzVector pCone( 0.0 , 0.0 , 0.0 , 0.0 );
		for (int i = 0; i < m_nPfos; ++i)
		{
			const EVENT::ReconstructedParticle *pRecParticle1 = dynamic_cast<EVENT::ReconstructedParticle*>(PfoCollection->getElementAt(i));
			if ( ( pRecParticle1->getType()) == 13 )
			{
				++m_nRecoMuons;
			}
			if ( ( pRecParticle1->getType()) == -13 )
			{
				++m_nRecoAntiMuons;
			}
			if ( (pRecParticle1->getType()) == 13 )
			{
				streamlog_out(MESSAGE) << "Found reconstructed muon at " << pRecParticle1 << std::endl;
				for (int j = 0; j < m_nPfos; ++j)
				{
					const EVENT::ReconstructedParticle *pRecParticle2 = dynamic_cast<EVENT::ReconstructedParticle*>(PfoCollection->getElementAt(j));
					if ( (pRecParticle2->getType()) == -13 )// && !foundDiMuon)
					{
						streamlog_out(MESSAGE) << "Found reconstructed anti-muon at " << j << std::endl;
						m_PfoMuonPDG.push_back(pRecParticle2->getType());
						m_EnergyRecoParticle1.push_back(pRecParticle1->getEnergy());
						m_EnergyRecoParticle2.push_back(pRecParticle2->getEnergy());
						TVector3 MomentumRecoParticle1 = TVector3(pRecParticle1->getMomentum());
				   		TVector3 MomentumRecoParticle2 = TVector3(pRecParticle2->getMomentum());
						m_RecoMuon1Costheta.push_back(MomentumRecoParticle1.CosTheta());
						m_RecoMuon1theta.push_back(MomentumRecoParticle1.Theta());
						m_RecoMuon2Costheta.push_back(MomentumRecoParticle2.CosTheta());
						m_RecoMuon2theta.push_back(MomentumRecoParticle2.Theta());
						TVector3 PtRecoParticle1( MomentumRecoParticle1.Px() , MomentumRecoParticle1.Py() , 0.0 );
						m_PtRecoParticle1.push_back(PtRecoParticle1.Mag());
						TVector3 PtRecoParticle2( MomentumRecoParticle2.Px() , MomentumRecoParticle2.Py() , 0.0 );
						m_PtRecoParticle2.push_back(PtRecoParticle2.Mag());
						if ( pRecParticle1->getTracks().size() == 0 ) m_lostMuonTheta.push_back( MomentumRecoParticle1.Theta() );
						if ( pRecParticle2->getTracks().size() == 0 ) m_lostMuonTheta.push_back( MomentumRecoParticle2.Theta() );
						m_RecoDiMuonCostheta.push_back( MomentumRecoParticle1.Dot(MomentumRecoParticle2) / ( MomentumRecoParticle1.Mag() * MomentumRecoParticle2.Mag() ) );
						TLorentzVector PfoMuon1_tlv = TLorentzVector(pRecParticle1->getMomentum(),pRecParticle1->getEnergy());
						TLorentzVector PfoMuon2_tlv = TLorentzVector(pRecParticle2->getMomentum(),pRecParticle2->getEnergy());
						m_RecoMuonTotalMomentum1.push_back(((PfoMuon1_tlv).Vect()).Mag());
						m_RecoMuonTotalMomentum2.push_back(((PfoMuon2_tlv).Vect()).Mag());
						m_RecoDiMuonInvMass.push_back( ( PfoMuon1_tlv + PfoMuon2_tlv ).M() );
						RecoDiMuonInvMass->Fill( ( PfoMuon1_tlv + PfoMuon2_tlv ).M() );
						streamlog_out(MESSAGE) << "		Invariant Mass of RecoDiMuons =  " << ( PfoMuon1_tlv + PfoMuon2_tlv ).M() << " GeV" << std::endl;
//						foundDiMuon = true;
/*						m_PfoMuonTrackOmega1.push_back((pRecParticle1->getTracks()[0])->getOmega());
						m_PfoMuonTrackTanLambda1.push_back((pRecParticle1->getTracks()[0])->getTanLambda());
						m_PfoMuonTrackPhi1.push_back((pRecParticle1->getTracks()[0])->getPhi());
						m_PfoMuonTrackOmega2.push_back((pRecParticle2->getTracks()[0])->getOmega());
						m_PfoMuonTrackTanLambda2.push_back((pRecParticle2->getTracks()[0])->getTanLambda());
						m_PfoMuonTrackPhi2.push_back((pRecParticle2->getTracks()[0])->getPhi());
*/						++m_nRecoDiMuons;
						float massResolution = getMassResolution( pRecParticle1 , pRecParticle2 );
						streamlog_out(MESSAGE) << "		Mass Resolution =  " << massResolution << " GeV" << std::endl;
						m_MassResolution.push_back( massResolution );
						m_trueDarkPhotonMass.push_back( m_TrueDarkPhotonMass );
						m_ResidualDarkPhotonMass.push_back( ( PfoMuon1_tlv + PfoMuon2_tlv ).M() - m_TrueDarkPhotonMass );
						m_NormalizedResidualDarkPhotonMass.push_back( ( ( PfoMuon1_tlv + PfoMuon2_tlv ).M() - m_TrueDarkPhotonMass ) / massResolution );
						h_ResidualDarkPhotonMass->Fill( ( PfoMuon1_tlv + PfoMuon2_tlv ).M() - m_TrueDarkPhotonMass );
						h_NormalizedResidualDarkPhotonMass->Fill( ( ( PfoMuon1_tlv + PfoMuon2_tlv ).M() - m_TrueDarkPhotonMass ) / massResolution );
					}
				}
			}
		}
		for (int i = 0; i < m_nMCParticles; ++i)
		{
//			const EVENT::LCCollection *MCCollection = pLCEvent->getCollection(m_MCCollection);
			EVENT::MCParticle *pMCParticle1 = dynamic_cast<EVENT::MCParticle*>(MCCollection->getElementAt(i));
			if ( ( pMCParticle1->getPDG()) == 13 && pMCParticle1->getGeneratorStatus() == 1 )// && !foundDiMuon )
			{
				streamlog_out(MESSAGE) << "Found MC muon at " << i << std::endl;
                                ++m_nMCMuons;
                                Track* track1 = getLinkedTrack( pMCParticle1 , TrackMCParticleNav , MCParticleTrackNav , 0.9 );
                                if ( track1 != NULL )
                                {
                                        m_track1_isFound.push_back( 1 );
                                }
                                else
                                {
                                        m_track1_isFound.push_back( 0 );
                                }
				for (int j = 0; j < m_nMCParticles; ++j)
				{
					EVENT::MCParticle *pMCParticle2 = dynamic_cast<EVENT::MCParticle*>(MCCollection->getElementAt(j));
					if ( ( pMCParticle2->getPDG()) == -13 && pMCParticle2->getGeneratorStatus() == 1 )// && !foundDiMuon )
					{
                                                streamlog_out(MESSAGE) << "Found MC anti-muon at " << j << std::endl;
                                                ++m_nMCAntiMuons;
                                                Track* track2 = getLinkedTrack( pMCParticle2 , TrackMCParticleNav , MCParticleTrackNav , 0.9 );
                                                if ( track2 != NULL )
                                                {
                                                        m_track2_isFound.push_back( 1 );
                                                }
                                                else
                                                {
                                                        m_track2_isFound.push_back( 0 );
                                                }
						m_MCMuonPDG.push_back(pMCParticle1->getPDG());
						m_MCMuonPDG.push_back(pMCParticle2->getPDG());
                                                m_EnergyMCParticle1.push_back(pMCParticle1->getEnergy());
                                                m_EnergyMCParticle2.push_back(pMCParticle2->getEnergy());
						TVector3 MomentumMCParticle1 = TVector3(pMCParticle1->getMomentum());
                                                TVector3 PtMCParticle1( MomentumMCParticle1.Px() , MomentumMCParticle1.Py() , 0.0 );
                                                m_PtMCParticle1.push_back(PtMCParticle1.Mag());
                                                m_MCMuon1Costheta.push_back(MomentumMCParticle1.CosTheta());
                                                m_MCMuon1theta.push_back(MomentumMCParticle1.Theta());
						TVector3 MomentumMCParticle2 = TVector3(pMCParticle2->getMomentum());
                                                TVector3 PtMCParticle2( MomentumMCParticle2.Px() , MomentumMCParticle2.Py() , 0.0 );
                                                m_PtMCParticle2.push_back(PtMCParticle2.Mag());
                                                m_MCMuon2Costheta.push_back(MomentumMCParticle2.CosTheta());
                                                m_MCMuon2theta.push_back(MomentumMCParticle2.Theta());
                                                m_MCMuonMaxCostheta.push_back( ( fabs( MomentumMCParticle1.CosTheta() ) >= fabs( MomentumMCParticle2.CosTheta() ) ? MomentumMCParticle1.CosTheta() : MomentumMCParticle2.CosTheta() ) );
						m_MCDiMuonCostheta.push_back( MomentumMCParticle1.Dot(MomentumMCParticle2) / ( MomentumMCParticle1.Mag() * MomentumMCParticle2.Mag() ) );
						TLorentzVector MCMuon1_tlv = TLorentzVector(pMCParticle1->getMomentum(),pMCParticle1->getEnergy());
						TLorentzVector MCMuon2_tlv = TLorentzVector(pMCParticle2->getMomentum(),pMCParticle2->getEnergy());
                                                m_MCMuonTotalMomentum1.push_back(((MCMuon1_tlv).Vect()).Mag());
                                                m_MCMuonTotalMomentum2.push_back(((MCMuon2_tlv).Vect()).Mag());
						m_MCDiMuonInvMass.push_back( ( MCMuon1_tlv + MCMuon2_tlv ).M() );
						MCDiMuonInvMass->Fill( ( MCMuon1_tlv + MCMuon2_tlv ).M() );
						streamlog_out(MESSAGE) << "Invariant Mass of DiMuons =  " << ( MCMuon1_tlv + MCMuon2_tlv ).M() << " GeV" << std::endl;
//						foundDiMuon = true;
						++m_nMCDiMuons;
					}
				}
			}
		}
		++m_nEvtSum;

	}

	catch (...)
	{
		streamlog_out(WARNING) << "Could not extract input particle collection: " << m_PfoCollection << std::endl;
		streamlog_out(WARNING) << "Could not extract input particle collection: " << m_MCCollection << std::endl;
	}
        m_pTTree->Fill();
}

float DarkPhotonMassProcessor::getMassResolution( const EVENT::ReconstructedParticle *pRecParticle1 , const EVENT::ReconstructedParticle *pRecParticle2 )
{
	float Px1 = pRecParticle1->getMomentum()[0];
	float Py1 = pRecParticle1->getMomentum()[1];
	float Pz1 = pRecParticle1->getMomentum()[2];
	float E1 = pRecParticle1->getEnergy();
	float Px2 = pRecParticle2->getMomentum()[0];
	float Py2 = pRecParticle2->getMomentum()[1];
	float Pz2 = pRecParticle2->getMomentum()[2];
	float E2 = pRecParticle2->getEnergy();
	double M = sqrt( pow( E1 + E2 , 2 ) - pow( Px1 + Px2 , 2 ) - pow( Py1 + Py2 , 2 ) - pow( Pz1 + Pz2 , 2 ) );
	float sigma_Px12	= pRecParticle1->getCovMatrix()[ 0 ];
	float sigma_Px1Py1	= pRecParticle1->getCovMatrix()[ 1 ];
	float sigma_Py12	= pRecParticle1->getCovMatrix()[ 2 ];
	float sigma_Px1Pz1	= pRecParticle1->getCovMatrix()[ 3 ];
	float sigma_Py1Pz1	= pRecParticle1->getCovMatrix()[ 4 ];
	float sigma_Pz12	= pRecParticle1->getCovMatrix()[ 5 ];
	float sigma_Px1E1	= pRecParticle1->getCovMatrix()[ 6 ];
	float sigma_Py1E1	= pRecParticle1->getCovMatrix()[ 7 ];
	float sigma_Pz1E1	= pRecParticle1->getCovMatrix()[ 8 ];
	float sigma_E12		= pRecParticle1->getCovMatrix()[ 9 ];
	float sigma_Px22	= pRecParticle2->getCovMatrix()[ 0 ];
	float sigma_Px2Py2	= pRecParticle2->getCovMatrix()[ 1 ];
	float sigma_Py22	= pRecParticle2->getCovMatrix()[ 2 ];
	float sigma_Px2Pz2	= pRecParticle2->getCovMatrix()[ 3 ];
	float sigma_Py2Pz2	= pRecParticle2->getCovMatrix()[ 4 ];
	float sigma_Pz22	= pRecParticle2->getCovMatrix()[ 5 ];
	float sigma_Px2E2	= pRecParticle2->getCovMatrix()[ 6 ];
	float sigma_Py2E2	= pRecParticle2->getCovMatrix()[ 7 ];
	float sigma_Pz2E2	= pRecParticle2->getCovMatrix()[ 8 ];
	float sigma_E22		= pRecParticle2->getCovMatrix()[ 9 ];

	float dM_dPx		= -1.0 * ( Px1 + Px2 ) / M;
	float dM_dPy		= -1.0 * ( Py1 + Py2 ) / M;
	float dM_dPz		= -1.0 * ( Pz1 + Pz2 ) / M;
	float dM_dE		= ( E1 + E2 ) / M;
	double sigma_M2		= pow( dM_dPx , 2 ) * ( sigma_Px12 + sigma_Px22 )
				+ pow( dM_dPy , 2 ) * ( sigma_Py12 + sigma_Py22 )
				+ pow( dM_dPz , 2 ) * (sigma_Pz12 + sigma_Pz22 )
				+ pow( dM_dE , 2 ) * ( sigma_E12 + sigma_E22 )
				+ 2.0 * dM_dPx * dM_dPy * ( sigma_Px1Py1 + sigma_Px2Py2 )
				+ 2.0 * dM_dPx * dM_dPz * ( sigma_Px1Pz1 + sigma_Px2Pz2 )
				+ 2.0 * dM_dPz * dM_dPy * ( sigma_Py1Pz1 + sigma_Py2Pz2 )
				+ 2.0 * dM_dE * dM_dPx * ( sigma_Px1E1 + sigma_Px2E2 )
				+ 2.0 * dM_dE * dM_dPy * ( sigma_Py1E1 + sigma_Py2E2 )
				+ 2.0 * dM_dE * dM_dPz * ( sigma_Pz1E1 + sigma_Pz2E2 );
	float sigma_M		= sqrt( sigma_M2 );
	streamlog_out(MESSAGE) << "		sigma_M2 =  " << sigma_M2 << " GeV^2" << std::endl;
	return sigma_M;

}

void DarkPhotonMassProcessor::check()
{

}

void DarkPhotonMassProcessor::end()
{

	m_pTFile->cd();
	m_pTTree->Write();
        MCDiMuonInvMass->Write();
	RecoDiMuonInvMass->Write();
	IsoDiMuonInvMass->Write();
	h_ResidualDarkPhotonMass->Write();
	h_NormalizedResidualDarkPhotonMass->Write();
	m_pTFile->Close();
	delete m_pTFile;

}
