#include "env_model.h"

LINK_ENTITY_TO_CLASS(env_model, CEnvModel);

void CEnvModel::KeyValue(KeyValueData *pkvd)
{
	CPointEntity::KeyValue(pkvd);
}

void CEnvModel::Spawn(void)
{
	Precache();
	SET_MODEL(ENT(pev), STRING(pev->model));
	UTIL_SetOrigin(pev, pev->origin);
}

void CEnvModel::Precache(void)
{
	PRECACHE_MODEL((char *)STRING(pev->model));
}