#include "extdll.h"
#include "util.h"
#include "cbase.h"

#ifndef ENV_MODEL_H
#define ENV_MODEL_H

class CEnvModel : public CPointEntity
{
	void Spawn(void);
	void Precache(void);
	void KeyValue(KeyValueData *pkvd);
	virtual int	ObjectCaps(void) { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
};

#endif