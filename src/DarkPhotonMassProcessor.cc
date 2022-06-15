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
	m_Bfield(0.f),
	c(0.),
	mm2m(0.),
	eV2GeV(0.),
	eB(0.),
	muonMass(0.),
	sigmaInvPt(0.),
	m_nRun(0),
	m_nEvt(0),
	m_nRunSum(0),
	m_nEvtSum(0),
	m_polarizatione(0.f),
	m_polarizationp(0.f),
	m_crossSection(0.f),
	m_nPfos(0),
        m_nTracks(0),
	m_nRecoMuons(0),
	m_nRecoAntiMuons(0),
	m_nRecoDiMuons(0)
{
	registerInputCollection(	LCIO::RECONSTRUCTEDPARTICLE,
					"PfoCollection",
					"Name of input pfo collection",
					m_PfoCollection,
					std::string("PandoraPFOs")
					);

	registerInputCollection(	LCIO::TRACK,
        	                        "Trackcollection",
        	                        "Name of input Track collection",
        	                        m_TrackCollection,
        	                        std::string("MarlinTrkTracks")
        	                        );

        registerProcessorParameter(	"RootFile",
        				"Name of the output root file",
        				m_rootFile,
        				std::string("Output.root")
        				);
}

void DarkPhotonMassProcessor::init()
{
	streamlog_out(DEBUG) << "	init called  " << std::endl;
	m_Bfield = 3.5;
	c = 2.99792458e8;
	mm2m = 1e-3;
	eV2GeV = 1e-9;
	eB = m_Bfield * c * mm2m * eV2GeV;
	muonMass = 0.1056583755;
	sigmaInvPt = 2.0 * pow( 10 , -5 );

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
	m_pTTree->Branch("nTracks", &m_nTracks, "nTracks/I");
	m_pTTree->Branch("nRecoMuons", &m_nRecoMuons, "nRecoMuons/I");
	m_pTTree->Branch("nRecoAntiMuons", &m_nRecoAntiMuons, "nRecoAntiMuons/I");
	m_pTTree->Branch("nRecoDiMuons", &m_nRecoDiMuons, "nRecoDiMuons/I");
	m_pTTree->Branch("RecoDiMuonInvMass", &m_RecoDiMuonInvMass);
	m_pTTree->Branch("RecoDiMuonCostheta",&m_RecoDiMuonCostheta);
        m_pTTree->Branch("RecoMuonTotalMomentum1", &m_RecoMuonTotalMomentum1);
        m_pTTree->Branch("RecoMuonTotalMomentum2", &m_RecoMuonTotalMomentum2);
        m_pTTree->Branch("PtRecoParticle1", &m_PtRecoParticle1);
        m_pTTree->Branch("PtRecoParticle2", &m_PtRecoParticle2);
        m_pTTree->Branch("RecoMuon1Costheta", &m_RecoMuon1Costheta);
        m_pTTree->Branch("RecoMuon2Costheta", &m_RecoMuon2Costheta);
        m_pTTree->Branch("RecoMuon1theta", &m_RecoMuon1theta);
        m_pTTree->Branch("RecoMuon2theta", &m_RecoMuon2theta);
        m_pTTree->Branch("RecoMuon1Px", &m_RecoMuon1Px);
        m_pTTree->Branch("RecoMuon1Py", &m_RecoMuon1Py);
        m_pTTree->Branch("RecoMuon1Pz", &m_RecoMuon1Pz);
        m_pTTree->Branch("RecoMuon1E", &m_EnergyRecoParticle1);
        m_pTTree->Branch("RecoMuon1SigmaPx2", &m_RecoMuon1SigmaPx2);
        m_pTTree->Branch("RecoMuon1SigmaPxPy", &m_RecoMuon1SigmaPxPy);
        m_pTTree->Branch("RecoMuon1SigmaPy2", &m_RecoMuon1SigmaPy2);
        m_pTTree->Branch("RecoMuon1SigmaPxPz", &m_RecoMuon1SigmaPxPz);
        m_pTTree->Branch("RecoMuon1SigmaPyPz", &m_RecoMuon1SigmaPyPz);
        m_pTTree->Branch("RecoMuon1SigmaPz2", &m_RecoMuon1SigmaPz2);
        m_pTTree->Branch("RecoMuon1SigmaPxE", &m_RecoMuon1SigmaPxE);
        m_pTTree->Branch("RecoMuon1SigmaPyE", &m_RecoMuon1SigmaPyE);
        m_pTTree->Branch("RecoMuon1SigmaPzE", &m_RecoMuon1SigmaPzE);
        m_pTTree->Branch("RecoMuon1SigmaE2", &m_RecoMuon1SigmaE2);
        m_pTTree->Branch("RecoMuon2Px", &m_RecoMuon2Px);
        m_pTTree->Branch("RecoMuon2Py", &m_RecoMuon2Py);
        m_pTTree->Branch("RecoMuon2Pz", &m_RecoMuon2Pz);
        m_pTTree->Branch("RecoMuon2E", &m_EnergyRecoParticle2);
        m_pTTree->Branch("RecoMuon2SigmaPx2", &m_RecoMuon2SigmaPx2);
        m_pTTree->Branch("RecoMuon2SigmaPxPy", &m_RecoMuon2SigmaPxPy);
        m_pTTree->Branch("RecoMuon2SigmaPy2", &m_RecoMuon2SigmaPy2);
        m_pTTree->Branch("RecoMuon2SigmaPxPz", &m_RecoMuon2SigmaPxPz);
        m_pTTree->Branch("RecoMuon2SigmaPyPz", &m_RecoMuon2SigmaPyPz);
        m_pTTree->Branch("RecoMuon2SigmaPz2", &m_RecoMuon2SigmaPz2);
        m_pTTree->Branch("RecoMuon2SigmaPxE", &m_RecoMuon2SigmaPxE);
        m_pTTree->Branch("RecoMuon2SigmaPyE", &m_RecoMuon2SigmaPyE);
        m_pTTree->Branch("RecoMuon2SigmaPzE", &m_RecoMuon2SigmaPzE);
        m_pTTree->Branch("RecoMuon2SigmaE2", &m_RecoMuon2SigmaE2);
        m_pTTree->Branch("MassResolution_pT", &m_MassResolution_pT);
        m_pTTree->Branch("MassResolution_CovMat", &m_MassResolution_CovMat);
        this->Clear();
}

void DarkPhotonMassProcessor::Clear()
{
	m_polarizatione = 0;
	m_polarizationp = 0;
        m_crossSection = 0.0;
        m_nPfos = 0;
        m_nTracks = 0;
	m_nRecoMuons = 0;
	m_nRecoAntiMuons = 0;
	m_nRecoDiMuons = 0;
	m_RecoDiMuonInvMass.clear();
        m_RecoDiMuonCostheta.clear();
        m_RecoMuonTotalMomentum1.clear();
        m_RecoMuonTotalMomentum2.clear();
        m_PtRecoParticle1.clear();
        m_PtRecoParticle2.clear();
        m_RecoMuon1Costheta.clear();
        m_RecoMuon2Costheta.clear();
        m_RecoMuon1theta.clear();
        m_RecoMuon2theta.clear();
        m_RecoMuon1Px.clear();
        m_RecoMuon1Py.clear();
        m_RecoMuon1Pz.clear();
        m_EnergyRecoParticle1.clear();
        m_RecoMuon1SigmaPx2.clear();
        m_RecoMuon1SigmaPxPy.clear();
        m_RecoMuon1SigmaPy2.clear();
        m_RecoMuon1SigmaPxPz.clear();
        m_RecoMuon1SigmaPyPz.clear();
        m_RecoMuon1SigmaPz2.clear();
        m_RecoMuon1SigmaPxE.clear();
        m_RecoMuon1SigmaPyE.clear();
        m_RecoMuon1SigmaPzE.clear();
        m_RecoMuon1SigmaE2.clear();
        m_RecoMuon2Px.clear();
        m_RecoMuon2Py.clear();
        m_RecoMuon2Pz.clear();
        m_EnergyRecoParticle2.clear();
        m_RecoMuon2SigmaPx2.clear();
        m_RecoMuon2SigmaPxPy.clear();
        m_RecoMuon2SigmaPy2.clear();
        m_RecoMuon2SigmaPxPz.clear();
        m_RecoMuon2SigmaPyPz.clear();
        m_RecoMuon2SigmaPz2.clear();
        m_RecoMuon2SigmaPxE.clear();
        m_RecoMuon2SigmaPyE.clear();
        m_RecoMuon2SigmaPzE.clear();
        m_RecoMuon2SigmaE2.clear();
	m_MassResolution_pT.clear();
	m_MassResolution_CovMat.clear();

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
		streamlog_out(MESSAGE) << "	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
		streamlog_out(MESSAGE) << "	|||||||||||||||||||||| processing event:	" << m_nEvt << "||||||||||||||||||||||||" << std::endl;
		streamlog_out(MESSAGE) << "	||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
		const EVENT::LCCollection *PfoCollection = pLCEvent->getCollection(m_PfoCollection);
		m_nPfos = PfoCollection->getNumberOfElements();
		const EVENT::LCCollection *Trackcollection = pLCEvent->getCollection(m_TrackCollection);
		m_nTracks = Trackcollection->getNumberOfElements();

		m_crossSection = pLCEvent->getParameters().getFloatVal("crossSection");
		m_polarizatione = pLCEvent->getParameters().getFloatVal("beamPol1");
		m_polarizationp = pLCEvent->getParameters().getFloatVal("beamPol2");
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
				streamlog_out(DEBUG8) << "Found reconstructed muon at " << pRecParticle1 << std::endl;
				for (int j = 0; j < m_nPfos; ++j)
				{
					const EVENT::ReconstructedParticle *pRecParticle2 = dynamic_cast<EVENT::ReconstructedParticle*>(PfoCollection->getElementAt(j));
					if ( (pRecParticle2->getType()) == -13 )// && !foundDiMuon)
					{
						streamlog_out(DEBUG8) << "Found reconstructed anti-muon at " << j << std::endl;
                                                float massResolution = 0.0;
						m_EnergyRecoParticle1.push_back(pRecParticle1->getEnergy());
						m_EnergyRecoParticle2.push_back(pRecParticle2->getEnergy());
						TVector3 MomentumRecoParticle1 = TVector3(pRecParticle1->getMomentum());
                                                m_RecoMuon1Px.push_back( MomentumRecoParticle1.Px() );
                                                m_RecoMuon1Py.push_back( MomentumRecoParticle1.Py() );
                                                m_RecoMuon1Pz.push_back( MomentumRecoParticle1.Pz() );
                                                m_RecoMuon1SigmaPx2.push_back( pRecParticle1->getCovMatrix()[ 0 ] );
                                                m_RecoMuon1SigmaPxPy.push_back( pRecParticle1->getCovMatrix()[ 1 ] );
                                                m_RecoMuon1SigmaPy2.push_back( pRecParticle1->getCovMatrix()[ 2 ] );
                                                m_RecoMuon1SigmaPxPz.push_back( pRecParticle1->getCovMatrix()[ 3 ] );
                                                m_RecoMuon1SigmaPyPz.push_back( pRecParticle1->getCovMatrix()[ 4 ] );
                                                m_RecoMuon1SigmaPz2.push_back( pRecParticle1->getCovMatrix()[ 5 ] );
                                                m_RecoMuon1SigmaPxE.push_back( pRecParticle1->getCovMatrix()[ 6 ] );
                                                m_RecoMuon1SigmaPyE.push_back( pRecParticle1->getCovMatrix()[ 7 ] );
                                                m_RecoMuon1SigmaPzE.push_back( pRecParticle1->getCovMatrix()[ 8 ] );
                                                m_RecoMuon1SigmaE2.push_back( pRecParticle1->getCovMatrix()[ 9 ] );
				   		TVector3 MomentumRecoParticle2 = TVector3(pRecParticle2->getMomentum());
                                                m_RecoMuon2Px.push_back( MomentumRecoParticle2.Px() );
                                                m_RecoMuon2Py.push_back( MomentumRecoParticle2.Py() );
                                                m_RecoMuon2Pz.push_back( MomentumRecoParticle2.Pz() );
                                                m_RecoMuon2SigmaPx2.push_back( pRecParticle2->getCovMatrix()[ 0 ] );
                                                m_RecoMuon2SigmaPxPy.push_back( pRecParticle2->getCovMatrix()[ 1 ] );
                                                m_RecoMuon2SigmaPy2.push_back( pRecParticle2->getCovMatrix()[ 2 ] );
                                                m_RecoMuon2SigmaPxPz.push_back( pRecParticle2->getCovMatrix()[ 3 ] );
                                                m_RecoMuon2SigmaPyPz.push_back( pRecParticle2->getCovMatrix()[ 4 ] );
                                                m_RecoMuon2SigmaPz2.push_back( pRecParticle2->getCovMatrix()[ 5 ] );
                                                m_RecoMuon2SigmaPxE.push_back( pRecParticle2->getCovMatrix()[ 6 ] );
                                                m_RecoMuon2SigmaPyE.push_back( pRecParticle2->getCovMatrix()[ 7 ] );
                                                m_RecoMuon2SigmaPzE.push_back( pRecParticle2->getCovMatrix()[ 8 ] );
                                                m_RecoMuon2SigmaE2.push_back( pRecParticle2->getCovMatrix()[ 9 ] );
						m_RecoMuon1Costheta.push_back(MomentumRecoParticle1.CosTheta());
						m_RecoMuon1theta.push_back(MomentumRecoParticle1.Theta());
						m_RecoMuon2Costheta.push_back(MomentumRecoParticle2.CosTheta());
						m_RecoMuon2theta.push_back(MomentumRecoParticle2.Theta());
						TVector3 PtRecoParticle1( MomentumRecoParticle1.Px() , MomentumRecoParticle1.Py() , 0.0 );
						m_PtRecoParticle1.push_back(PtRecoParticle1.Mag());
						TVector3 PtRecoParticle2( MomentumRecoParticle2.Px() , MomentumRecoParticle2.Py() , 0.0 );
						m_PtRecoParticle2.push_back(PtRecoParticle2.Mag());
						m_RecoDiMuonCostheta.push_back( MomentumRecoParticle1.Dot(MomentumRecoParticle2) / ( MomentumRecoParticle1.Mag() * MomentumRecoParticle2.Mag() ) );
						TLorentzVector PfoMuon1_tlv = TLorentzVector(pRecParticle1->getMomentum(),pRecParticle1->getEnergy());
						TLorentzVector PfoMuon2_tlv = TLorentzVector(pRecParticle2->getMomentum(),pRecParticle2->getEnergy());
						m_RecoMuonTotalMomentum1.push_back(((PfoMuon1_tlv).Vect()).Mag());
						m_RecoMuonTotalMomentum2.push_back(((PfoMuon2_tlv).Vect()).Mag());
						m_RecoDiMuonInvMass.push_back( ( PfoMuon1_tlv + PfoMuon2_tlv ).M() );
						streamlog_out(DEBUG8) << "		Invariant Mass of RecoDiMuons =  " << ( PfoMuon1_tlv + PfoMuon2_tlv ).M() << " GeV" << std::endl;
//						foundDiMuon = true;
//						EVENT::Track *track1 = (pRecParticle1->getTracks()[0]);
//						EVENT::Track *track2 = (pRecParticle2->getTracks()[0]);
//						massResolution = getMassResolution( track1 , track2 );
						m_MassResolution_pT.push_back( massResolution );
						++m_nRecoDiMuons;
//						massResolution = getMassResolution( pRecParticle1 , pRecParticle2 );
						streamlog_out(DEBUG8) << "		Mass Resolution =  " << massResolution << " GeV" << std::endl;
						m_MassResolution_CovMat.push_back( massResolution );
					}
				}
			}
		}
		++m_nEvtSum;
	        m_pTTree->Fill();
	}

	catch (...)
	{
		streamlog_out(WARNING) << "Could not extract input particle collection: " << m_PfoCollection << std::endl;
	}
}

/*
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
	streamlog_out(DEBUG8) << "		sigma_M2 =  " << sigma_M2 << " GeV^2" << std::endl;
	return sigma_M;

}

float DarkPhotonMassProcessor::getMassResolution( const EVENT::Track *track1 , const EVENT::Track *track2 )
{

	float tanLambda1 = track1->getTanLambda();
	float tanLambda2 = track2->getTanLambda();
	float Omega1 = track1->getOmega();
	float Omega2 = track2->getOmega();

	float PT1 = eB / fabs( Omega1 );
	float PT2 = eB / fabs( Omega2 );

	float Px1 = PT1 * std::cos( track1->getPhi() );
	float Py1 = PT1 * std::sin( track1->getPhi() );
	float Pz1 = PT1 * track1->getTanLambda();
	float E1 = sqrt( pow( PT1 , 2.0 ) * ( 1.0 + pow( tanLambda1 , 2.0 ) ) + pow( muonMass , 2.0 ) );
	float Px2 = PT2 * std::cos( track2->getPhi() );
	float Py2 = PT2 * std::sin( track2->getPhi() );
	float Pz2 = PT2 * track2->getTanLambda();
	float E2 = sqrt( pow( PT2 , 2.0 ) * ( 1.0 + pow( tanLambda2 , 2.0 ) ) + pow( muonMass , 2.0 ) );


	double M = sqrt( pow( E1 + E2 , 2 ) - pow( Px1 + Px2 , 2 ) - pow( Py1 + Py2 , 2 ) - pow( Pz1 + Pz2 , 2 ) );

	float dM_dPx1		= -1.0 * ( Px1 + Px2 ) / M;
	float dPx1_dPT1		= std::cos( track1->getPhi() );
	float dM_dPy1		= -1.0 * ( Py1 + Py2 ) / M;
	float dPy1_dPT1		= std::sin( track1->getPhi() );
	float dM_dPz1		= -1.0 * ( Pz1 + Pz2 ) / M;
	float dPz1_dPT1		= tanLambda1;
	float dM_dE1		= ( E1 + E2 ) / M;
	float dE1_dPT1		= PT1 * ( 1.0 + pow( tanLambda1 , 2 ) ) / E1;

	float dM_dPx2		= -1.0 * ( Px1 + Px2 ) / M;
	float dPx2_dPT2		= std::cos( track2->getPhi() );
	float dM_dPy2		= -1.0 * ( Py1 + Py2 ) / M;
	float dPy2_dPT2		= std::sin( track2->getPhi() );
	float dM_dPz2		= -1.0 * ( Pz1 + Pz2 ) / M;
	float dPz2_dPT2		= tanLambda2;
	float dM_dE2		= ( E1 + E2 ) / M;
	float dE2_dPT2		= PT2 * ( 1.0 + pow( tanLambda2 , 2 ) ) / E2;

	double dM_dPT1	= ( dM_dE1 * dE1_dPT1 ) + ( dM_dPx1 * dPx1_dPT1 ) + ( dM_dPy1 * dPy1_dPT1 ) + ( dM_dPz1 * dPz1_dPT1 );
	double dM_dPT2	= ( dM_dE2 * dE2_dPT2 ) + ( dM_dPx2 * dPx2_dPT2 ) + ( dM_dPy2 * dPy2_dPT2 ) + ( dM_dPz2 * dPz2_dPT2 );

	float sigma_MPT		= sigmaInvPt * sqrt( pow( dM_dPT1 , 2 ) * pow( PT1 , 4 ) + pow( dM_dPT2 , 2 ) * pow( PT2 , 4 ) );
	streamlog_out(DEBUG8) << "		sigma_MPT =  " << sigma_MPT << " GeV" << std::endl;
	return sigma_MPT;

}
*/

void DarkPhotonMassProcessor::check()
{

}

void DarkPhotonMassProcessor::end()
{

	m_pTFile->cd();
	m_pTTree->Write();
	m_pTFile->Close();
	delete m_pTFile;

}
