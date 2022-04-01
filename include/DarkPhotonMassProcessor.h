#ifndef DarkPhotonMassProcessor_h
#define DarkPhotonMassProcessor_h 1
#include <marlin/Processor.h>
#include <marlin/Global.h>
#include "marlin/VerbosityLevels.h"
#include "streamlog/streamlog.h"
#include "IMPL/LCCollectionVec.h"
#include "lcio.h"
#include <string>
#include <vector>
#include "EVENT/ReconstructedParticle.h"
#include "TLorentzVector.h"
#include "EVENT/Track.h"
#include "UTIL/LCRelationNavigator.h"
#include "LinkedLCObject.h"

class TFile;
class TH1F;
class TTree;

using namespace std ;
using namespace lcio ;
using namespace marlin ;

class DarkPhotonMassProcessor : public Processor
{
        public:
                virtual Processor*  newProcessor()
                {
                        return new DarkPhotonMassProcessor;
                }
                DarkPhotonMassProcessor();
        	virtual ~DarkPhotonMassProcessor() = default;
        	DarkPhotonMassProcessor( const DarkPhotonMassProcessor& ) = delete;
        	DarkPhotonMassProcessor &operator = ( const DarkPhotonMassProcessor& ) = delete;
        	virtual void init();
        	virtual void Clear();
        	virtual void processRunHeader();
        	virtual void processEvent( EVENT::LCEvent *pLCEvent );
                virtual void check();
                virtual void end();
		float getMassResolution( const EVENT::ReconstructedParticle *pRecParticle1 , const EVENT::ReconstructedParticle *pRecParticle2 );

        private:

        	typedef std::vector<int>		IntVector;
        	typedef std::vector<double>		DoubleVector;
        	typedef std::vector<float>		FloatVector;
		std::string				m_rootFile{};
		std::string				m_PfoCollection{};
		std::string				m_MCCollection{};
		std::string				m_IsoLepCollection{};
		std::string	 			m_PhysicsProcess{};
		std::string				m_TrackCollection{};
                std::string				m_MCTruthTrackLinkCollection{};
                std::string				m_TrackMCTruthLinkCollection{};
		float					m_TrueDarkPhotonMass;

		int					m_nRun;
		int					m_nEvt;
		int					m_nRunSum;
		int					m_nEvtSum;

		float					m_polarizatione;
		float					m_polarizationp;
		float					polweight;
		float					m_crossSection;
//                float                                 _cosConeSmall;
//                float                                 _cosConeLarge;

		double					epol_target=-0.8;
		double					ppol_target=+0.3;
		double					lumi_target=900.0;

		int					m_nPfos;
		IntVector		 		m_PfoMuonPDG{};
		IntVector				m_MCMuonPDG{};
		IntVector		 		m_IsoMuonPDG{};
		int					m_nIsoLeptons;
		int					m_nMCMuons;
		int 					m_nMCAntiMuons;
		int  					m_nMCDiMuons;
		int					m_nRecoMuons;
		int 					m_nRecoAntiMuons;
		int  					m_nRecoDiMuons;
		int 					m_nDiMuonIsoLeptons;
		int 					m_nTracks;
                int                                     m_nMCTruthTracks;
		int 					m_nMCParticles;


		FloatVector                           m_RecoDiMuonInvMass{};
                FloatVector                           m_RecoMuonTotalMomentum1{};
                FloatVector                           m_RecoMuonTotalMomentum2{};
                FloatVector                           m_MCMuonTotalMomentum1{};
                FloatVector                           m_MCMuonTotalMomentum2{};
                FloatVector                           m_PtMCParticle1{};
                FloatVector                           m_PtMCParticle2{};
                FloatVector                           m_PtRecoParticle1{};
                FloatVector                           m_PtRecoParticle2{};
                FloatVector                           m_MCMuon2theta{};
                FloatVector                           m_MCMuon1theta{};
                FloatVector                           m_RecoMuon1theta{};
                FloatVector                           m_RecoMuon2theta{};
                FloatVector                           m_MCMuon2Costheta{};
                FloatVector                           m_MCMuon1Costheta{};
                FloatVector                           m_RecoMuon1Costheta{};
                FloatVector                           m_RecoMuon2Costheta{};
                FloatVector                           m_EnergyRecoParticle1{};
                FloatVector                           m_EnergyRecoParticle2{};
                FloatVector                           m_EnergyMCParticle1{};
                FloatVector                           m_EnergyMCParticle2{};
                FloatVector                           m_PfoMuonTrackOmega1{};
                FloatVector                           m_PfoMuonTrackTanLambda1{};
                FloatVector                           m_PfoMuonTrackPhi1{};
                FloatVector                           m_PfoMuonTrackOmega2{};
                FloatVector                           m_PfoMuonTrackTanLambda2{};
                FloatVector                           m_PfoMuonTrackPhi2{};
		FloatVector 			      m_MassResolution{};
		FloatVector 			      m_trueDarkPhotonMass{};
		FloatVector 			      m_ResidualDarkPhotonMass{};
		FloatVector 			      m_NormalizedResidualDarkPhotonMass{};
                IntVector                             m_track1_isFound{};
                IntVector                             m_track2_isFound{};


		FloatVector				m_MCDiMuonInvMass{};
		FloatVector				m_MCDiMuonCostheta{};
		FloatVector				m_RecoDiMuonCostheta{};
                FloatVector				m_MCMuonMaxCostheta{};
                FloatVector				m_lostMuonTheta{};
		FloatVector				m_IsoDiMuonCostheta{};
                FloatVector				m_costheta{};

                double                                  m_coneEC{};

		FloatVector				m_IsoDiMuonInvMass{};


                TFile					*m_pTFile{};
                TTree					*m_pTTree{};
                TH1F                                    *MCDiMuonInvMass{};
		TH1F					*RecoDiMuonInvMass{};
		TH1F					*IsoDiMuonInvMass{};
		TH1F					*h_ResidualDarkPhotonMass{};
		TH1F					*h_NormalizedResidualDarkPhotonMass{};


};
#endif
