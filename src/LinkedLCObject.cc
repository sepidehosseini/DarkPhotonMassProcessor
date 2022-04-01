#include "LinkedLCObject.h"
EVENT::Track* getLinkedTrack( EVENT::MCParticle *mcp , LCRelationNavigator TrackMCParticleNav , LCRelationNavigator MCParticleTrackNav , float minWeight)
{
	EVENT::Track* linkedTrack = NULL;
	streamlog_out(DEBUG1) << "" << std::endl;
	streamlog_out(DEBUG1) << "	Look for Track linked to MCPartilce:" << std::endl;
	streamlog_out(DEBUG1) << *mcp << std::endl;
	float maxWeightMCPtoTRK = 0.0;
	float maxWeightTRKtoMCP = 0.0;
	float trk_mcp_weight = 0.0;
	float mcp_trk_weight = 0.0;
	int i_mcp_trk_MaxWeight = -1;
	int i_trk_mcp_MaxWeight = -1;
	const EVENT::LCObjectVec& TrackVector = MCParticleTrackNav.getRelatedToObjects( mcp );
	const EVENT::FloatVec&  Trackweightvec = MCParticleTrackNav.getRelatedToWeights( mcp );
	for ( unsigned int i_Track = 0; i_Track < TrackVector.size(); i_Track++ )
	{
		mcp_trk_weight = Trackweightvec.at( i_Track );
		if ( mcp_trk_weight > maxWeightMCPtoTRK && mcp_trk_weight >= minWeight )
		{
			maxWeightMCPtoTRK = mcp_trk_weight;
			i_mcp_trk_MaxWeight = i_Track;
		}
	}
	if ( i_mcp_trk_MaxWeight != -1 )
	{
		Track*  testTrack = (Track*) TrackVector.at( i_mcp_trk_MaxWeight );
		const EVENT::LCObjectVec& MCPVector = TrackMCParticleNav.getRelatedToObjects( testTrack );
		const EVENT::FloatVec&  MCPweightvec = TrackMCParticleNav.getRelatedToWeights( testTrack );
		for ( unsigned int i_mcp = 0 ; i_mcp < MCPVector.size() ; ++i_mcp )
		{
			trk_mcp_weight = MCPweightvec.at( i_mcp );
			if ( trk_mcp_weight > maxWeightTRKtoMCP && trk_mcp_weight >= minWeight )
			{
				trk_mcp_weight = maxWeightTRKtoMCP;
				i_trk_mcp_MaxWeight = i_mcp;
			}
		}
		if ( i_trk_mcp_MaxWeight != -1 )
		{
			MCParticle* testMCP = (MCParticle*) MCPVector.at( i_trk_mcp_MaxWeight );
			if ( testMCP == mcp ) linkedTrack = testTrack;
		}
	}
	if ( linkedTrack != NULL )
	{
		streamlog_out(DEBUG1) << "	Found Linked Track to MCPartilce:" << std::endl;
		streamlog_out(DEBUG1) << *linkedTrack << std::endl;
	}
	else
	{
		streamlog_out(DEBUG1) << "	Could not Find Linked Track to MCPartilce" << std::endl;
	}
	return linkedTrack;



}
