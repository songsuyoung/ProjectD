#pragma once

UENUM()
enum class PDESkillType : uint8
{
	Base = 0, /*Mouse : 기본 공격 0 */
	SkillQ = 1, /*Q : 공격1(스킬) */
	SkillE = 2, /*E : 공격2(스킬) */
	SkillW = 3, /*W : 방어/회복(스킬) */
};