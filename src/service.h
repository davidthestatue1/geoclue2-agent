#ifndef __SERVICE_H
#define __SERVICE_H
#define _GNU_SOURCE

#include "app.h"

#define GEOCLUE_AGENT_SERVICE_TYPE (geoclue_agent_service_get_type ())
G_DECLARE_FINAL_TYPE (GeoClueAgentService, geoclue_agent_service, GEOCLUE_AGENT, SERVICE, GeoClueAgentApp)

GeoClueAgentService     *geoclue_agent_service_new         (void);


#endif /* __SERVICE_H */

