#ifndef LinkedLCObject_h_1
#define LinkedLCObject_h_1


#include "lcio.h"
#include <EVENT/MCParticle.h>
#include <EVENT/Track.h>
#include "UTIL/LCRelationNavigator.h"
#include "DDMarlinCED.h"


EVENT::Track* getLinkedTrack( EVENT::MCParticle *mcp , LCRelationNavigator TrackMCParticleNav , LCRelationNavigator MCParticleTrackNav , float minWeight);

#endif
