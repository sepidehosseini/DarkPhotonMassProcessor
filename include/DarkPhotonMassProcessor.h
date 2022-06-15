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
//		float getMassResolution( const EVENT::ReconstructedParticle *pRecParticle1 , const EVENT::ReconstructedParticle *pRecParticle2 );
//		float getMassResolution( const EVENT::Track *track1 , const EVENT::Track *track2 );

        private:

        	typedef std::vector<int>		IntVector;
        	typedef std::vector<double>		DoubleVector;
        	typedef std::vector<float>		FloatVector;
		std::string				m_rootFile{};
		std::string				m_PfoCollection{};
		std::string				m_TrackCollection{};
		float					m_Bfield;
		float					c;
		float					mm2m;
		float					eV2GeV;
		float					eB;
		float					muonMass;
		float					sigmaInvPt;

		int					m_nRun;
		int					m_nEvt;
		int					m_nRunSum;
		int					m_nEvtSum;
		float					m_polarizatione;
		float					m_polarizationp;
		float					m_crossSection;
		int					m_nPfos;
                int 					m_nTracks;
		int					m_nRecoMuons;
		int 					m_nRecoAntiMuons;
		int  					m_nRecoDiMuons;
                FloatVector				m_RecoDiMuonInvMass{};
                FloatVector				m_RecoDiMuonCostheta{};
                FloatVector				m_RecoMuonTotalMomentum1{};
                FloatVector				m_RecoMuonTotalMomentum2{};
                FloatVector				m_PtRecoParticle1{};
                FloatVector				m_PtRecoParticle2{};
                FloatVector				m_RecoMuon1Costheta{};
                FloatVector				m_RecoMuon2Costheta{};
                FloatVector				m_RecoMuon1theta{};
                FloatVector				m_RecoMuon2theta{};
                FloatVector				m_RecoMuon1Px{};
                FloatVector				m_RecoMuon1Py{};
                FloatVector				m_RecoMuon1Pz{};
                FloatVector				m_EnergyRecoParticle1{};
                FloatVector				m_RecoMuon1SigmaPx2{};
                FloatVector				m_RecoMuon1SigmaPxPy{};
                FloatVector				m_RecoMuon1SigmaPy2{};
                FloatVector				m_RecoMuon1SigmaPxPz{};
                FloatVector				m_RecoMuon1SigmaPyPz{};
                FloatVector				m_RecoMuon1SigmaPz2{};
                FloatVector				m_RecoMuon1SigmaPxE{};
                FloatVector				m_RecoMuon1SigmaPyE{};
                FloatVector				m_RecoMuon1SigmaPzE{};
                FloatVector				m_RecoMuon1SigmaE2{};
                FloatVector				m_RecoMuon2Px{};
                FloatVector				m_RecoMuon2Py{};
                FloatVector				m_RecoMuon2Pz{};
                FloatVector				m_EnergyRecoParticle2{};
                FloatVector				m_RecoMuon2SigmaPx2{};
                FloatVector				m_RecoMuon2SigmaPxPy{};
                FloatVector				m_RecoMuon2SigmaPy2{};
                FloatVector				m_RecoMuon2SigmaPxPz{};
                FloatVector				m_RecoMuon2SigmaPyPz{};
                FloatVector				m_RecoMuon2SigmaPz2{};
                FloatVector				m_RecoMuon2SigmaPxE{};
                FloatVector				m_RecoMuon2SigmaPyE{};
                FloatVector				m_RecoMuon2SigmaPzE{};
                FloatVector				m_RecoMuon2SigmaE2{};
                FloatVector                             m_MassResolution_pT{};
                FloatVector                             m_MassResolution_CovMat{};




                TFile					*m_pTFile{};
                TTree					*m_pTTree{};


};
#endif
