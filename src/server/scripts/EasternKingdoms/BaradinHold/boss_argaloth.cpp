/*
 * Copyright (C) 2011-2012 DarkCore <http://www.darkpeninsula.eu/>
 * Copyright (C) 2011-2012 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include"ScriptPCH.h"
#include"baradin_hold.h"

enum Spells
{
    SPELL_BERSERK              = 47008,
    SPELL_CONSUMING_DARKNESS   = 88954,
    SPELL_METEOR_SLASH         = 88942,
    SPELL_FEL_FIRESTORM        = 88972,
};

enum Events
{
    EVENT_BERSERK = 1,
    EVENT_CONSUMING_DARKNESS,
    EVENT_METEOR_SLASH,
};

class boss_argaloth: public CreatureScript
{
    public:
        boss_argaloth() : CreatureScript("boss_argaloth") { }

    struct boss_argalothAI: public BossAI
    {
        boss_argalothAI(Creature* creature) : BossAI(creature, DATA_ARGALOTH) { }

        uint32 fel_firestorm_casted;

        void Reset()
        {
            _Reset();
            me->RemoveAurasDueToSpell(SPELL_BERSERK);
            events.ScheduleEvent(EVENT_BERSERK, 300 *IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CONSUMING_DARKNESS, 14 *IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_METEOR_SLASH, 10 *IN_MILLISECONDS);
            fel_firestorm_casted = 0;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->GetHealthPct() < 66 && fel_firestorm_casted == 0)
            {
                DoCast(SPELL_FEL_FIRESTORM);
                events.DelayEvents(3 *IN_MILLISECONDS);
                fel_firestorm_casted = 1;
            }
            if (me->GetHealthPct() < 33 && fel_firestorm_casted == 1)
            {
                DoCast(SPELL_FEL_FIRESTORM);
                events.DelayEvents(3 *IN_MILLISECONDS);
                fel_firestorm_casted = 2;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CONSUMING_DARKNESS:
                    DoCast(SPELL_CONSUMING_DARKNESS);
                    events.RescheduleEvent(EVENT_CONSUMING_DARKNESS, 22 *IN_MILLISECONDS);
                    break;
                case EVENT_METEOR_SLASH:
                    DoCast(SPELL_METEOR_SLASH);
                    events.RescheduleEvent(EVENT_METEOR_SLASH, 15 *IN_MILLISECONDS);
                    break;
                case EVENT_BERSERK:
                    DoCast(me, SPELL_BERSERK);
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
     };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_argalothAI(creature);
    }
};

class spell_meteor_slash : public SpellScriptLoader
{
    public:
        spell_meteor_slash() : SpellScriptLoader("spell_meteor_slash") { }
        
        class spell_meteor_slash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_meteor_slash_SpellScript);

            std::list<Unit*> targetList;

            void FilterTargets(std::list<Unit*>& unitList)
            {
                targetList = unitList;
            }

            void DamageCaster(SpellEffIndex /*effIndex*/)
            {
                if(!targetList.empty())
                {
                    int32 dmg = int32(GetHitDamage() / targetList.size());
                    SetHitDamage(dmg);
                }
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_meteor_slash_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
                OnEffectHitTarget += SpellEffectFn(spell_meteor_slash_SpellScript::DamageCaster, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_meteor_slash_SpellScript();
        }
};

void AddSC_boss_argaloth()
{
    new boss_argaloth();
    new spell_meteor_slash();
}