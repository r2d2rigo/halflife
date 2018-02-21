#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "monsters.h"
#include "player.h"
#include "gamerules.h"

enum cubemap_e {
	CUBEMAP_IDLE1 = 0,
};

LINK_ENTITY_TO_CLASS(weapon_cubemap, CWeaponCubemap);

int CWeaponCubemap::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = NULL;
	p->iMaxAmmo1 = -1;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 0;
	p->iPosition = 1;
	p->iId = WEAPON_CUBEMAP;
	p->iWeight = CUBEMAP_WEIGHT;

	return 1;
}

int CWeaponCubemap::AddToPlayer(CBasePlayer *pPlayer)
{
	if (CBasePlayerWeapon::AddToPlayer(pPlayer))
	{
		MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
		WRITE_BYTE(m_iId);
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

void CWeaponCubemap::Spawn()
{
	pev->classname = MAKE_STRING("weapon_cubemap"); // hack to allow for old names
	Precache();
	m_iId = WEAPON_CUBEMAP;
	SET_MODEL(ENT(pev), "models/w_cubemap.mdl");

	m_iDefaultAmmo = 0;

	FallInit();// get ready to fall down.
}


void CWeaponCubemap::Precache(void)
{
	PRECACHE_MODEL("models/v_cubemap.mdl");
	PRECACHE_MODEL("models/w_cubemap.mdl");
	PRECACHE_MODEL("models/p_9mmhandgun.mdl");
}

BOOL CWeaponCubemap::Deploy()
{
	return DefaultDeploy("models/v_cubemap.mdl", "models/p_9mmhandgun.mdl", CUBEMAP_IDLE1, "onehanded", UseDecrement(), pev->body);
}


void CWeaponCubemap::Holster(int skiplocal /* = 0 */)
{
	m_fInReload = FALSE;// cancel any reload in progress.

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1.0;
	m_flTimeWeaponIdle = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
	SendWeaponAnim(CUBEMAP_IDLE1);
}

void CWeaponCubemap::WeaponIdle(void)
{
	ResetEmptySound();

	m_pPlayer->GetAutoaimVector(AUTOAIM_10DEGREES);

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	int iAnim = CUBEMAP_IDLE1;

	SendWeaponAnim(iAnim, UseDecrement() ? 1 : 0, 0);
}