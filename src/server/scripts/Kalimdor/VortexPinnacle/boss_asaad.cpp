/*
 * Copyright (C) 2011-2012 DarkCore <http://www.darkpeninsula.eu/>
 * Copyright (C) 2011-2012 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include "vortex_pinnacle.h"
#include "ScriptPCH.h"

enum eSpells
{
    SPELL_CHAIN_LIGHTNING_N       = 87622,
    SPELL_CHAIN_LIGHTNING_H       = 93993,
};

class boss_asaad : public CreatureScript
{
public:
    boss_asaad() : CreatureScript("boss_asaad") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_asaadAI (creature);
    }

    struct boss_asaadAI : public ScriptedAI
    {
        boss_asaadAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 ChainLightningTimer;

        void Reset()
        {
            ChainLightningTimer = 15000;
        }

        void EnterCombat(Unit* /*who*/)
        {
        }

        void JustDied(Unit* /*Killer*/)
        {
            instance->SetData(DATA_ASAAD, DONE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (ChainLightningTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    me->CastSpell(target, DUNGEON_MODE(SPELL_CHAIN_LIGHTNING_N, SPELL_CHAIN_LIGHTNING_H), true);

                ChainLightningTimer = urand(12*IN_MILLISECONDS, 17*IN_MILLISECONDS);
            } else ChainLightningTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_asaad()
{
    new boss_asaad();
}
