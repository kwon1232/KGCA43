#include "TNonPersonCharacter.h"
TNonPersonCharacter::~TNonPersonCharacter()
{
	std::cout << "������ȣ��" << std::endl;
}
void		TNonPersonCharacter::SetHealth(float p)
{
	TCharacter::SetHealth(p - 10.0f);
}
void		TNonPersonCharacter::SetMana(float p)
{
	TCharacter::SetHealth(p - 10.0f);
}
void		TNonPersonCharacter::SetAttackPower(float p)
{
	TCharacter::SetHealth(p - 10.0f);
}
void		TNonPersonCharacter::SetDefensivePower(float p)
{
	TCharacter::SetHealth(p + 10.0f);
}