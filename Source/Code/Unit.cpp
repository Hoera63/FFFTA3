#include "../Header/Unit.h"

float Unit::moveSpeed = 2.0f;

Unit::Unit(int hp, int mp, int move, int jump, int evade, int baseAttack, int baseDefense, int baseMagicAttack, int baseMagicDefense, int jp, int level, int xp, bool controlAI)
{
	m_currentHP = hp;
	m_currentMP = mp;
	m_hp = hp;
	m_mp = mp;
	m_move = move;
	m_jump = jump;
	m_evade = evade;
	m_baseAttack = baseAttack;
	m_baseDefense = baseDefense;
	m_baseMagicAttack = baseMagicAttack;
	m_baseMagicDefense = baseMagicDefense;
	m_jp = jp;
	m_level = level;
	m_xp = xp;
	m_turnCounter = 0;
	m_reserve = 0;
	m_AIControl = controlAI;
	m_walking = false;
	m_attacking = false;
}

Unit::~Unit()
{
	//dtor
}

void Unit::Update(float deltaTime)
{
	AnimatingObject::Update(deltaTime);
	if (m_walking)
	{
		if (m_path.size() == 1)
		{
			m_walking = false;
			m_path.clear();
			Facing(Facing());
			return;
		}
		float xDif = m_path[1].X - m_pos.X();
		float yDif = m_path[1].Y - m_pos.Y();
		//float zDif = m_path[1].Z - m_pos.Z();
		int orgXStep = m_path[1].X - m_path[0].X;
		int orgYStep = m_path[1].Y - m_path[0].Y;
		int orgZStep = m_path[1].Z - m_path[0].Z;
		int highZ = std::max(m_path[1].Z, m_path[0].Z) + 1;
		if (fabs(xDif + yDif) < fabs(deltaTime * orgXStep * moveSpeed + deltaTime * orgYStep * moveSpeed))
		{
			NextMovingStep();
		} else if ((abs(orgXStep) + abs(orgYStep)) < 2 && orgZStep == 0)
		{
			m_pos = vector3df(m_pos.X() + deltaTime * orgXStep * moveSpeed, m_pos.Y() + deltaTime * orgYStep * moveSpeed, m_pos.Z());
		} else if (fabs(xDif + yDif) >= abs(orgXStep + orgYStep) / 2.0f)
		{
			m_pos = vector3df(m_pos.X() + deltaTime * orgXStep * moveSpeed, m_pos.Y() + deltaTime * orgYStep * moveSpeed, 
								fmin(highZ, (highZ - m_path[0].Z) * moveSpeed * deltaTime + m_pos.Z()));
		} else
		{
			m_pos = vector3df(m_pos.X() + deltaTime * orgXStep * moveSpeed, m_pos.Y() + deltaTime * orgYStep * moveSpeed, 
								fmax(m_path[1].Z, (m_path[1].Z - highZ) * moveSpeed * deltaTime + m_pos.Z()));
		}
	} else if (m_attacking)
	{
		if (Sprite().CurrentAnimation()->TotalFrames() -1 <= Sprite().CurrentFrame())
		{
			m_attacking = false;
			Facing(Facing());
		}
	} else if (m_hurting)
	{
		if (Sprite().CurrentAnimation()->TotalFrames() -1 <= Sprite().CurrentFrame())
		{
			m_hurting = false;
			Facing(Facing());
		}
	}
}

void Unit::NextMovingStep()
{
	m_path.erase(m_path.begin());
	m_pos = vector3df(m_path[0].X, m_path[0].Y, m_path[0].Z);
	int orgXStep = m_path[1].X - m_path[0].X;
	int orgYStep = m_path[1].Y - m_path[0].Y;
	int orgZStep = m_path[1].Z - m_path[0].Z;
	if (orgZStep <= 1 && orgZStep >= -1)
	{
		if (orgXStep == -1) PlayAnimation(L"Walking", false); // West
		else if (orgXStep == 1) PlayAnimation(L"BackWalking", true); // East
		else if (orgYStep == 1) PlayAnimation(L"BackWalking", false); // North
		else if (orgYStep == -1) PlayAnimation(L"Walking", true); // South
		else if (orgXStep == -2) PlayAnimation(L"Jump", false, false);
		else if (orgXStep == 2) PlayAnimation(L"BackJump", true, false);
		else if (orgYStep == 2) PlayAnimation(L"BackJump", false, false);
		else if (orgYStep == -2) PlayAnimation(L"Jump", true, false);
	} else
	{
		if (orgXStep <= 0) PlayAnimation(L"Jump", false, false);
		else if (orgXStep >= 0) PlayAnimation(L"BackJump", true, false);
		else if (orgYStep >= 0) PlayAnimation(L"BackJump", false, false);
		else if (orgYStep <= 0) PlayAnimation(L"Jump", true, false);
	}
}

FacingSide Unit::Facing()
{
	 if (Sprite().CurrentAnimation()->Name().find(L"Back") != -1)
	 {
	 	if (Sprite().Mirrored()) return East;
	 	else return North;
	 } else
	 {
	 	if (Sprite().Mirrored()) return South;
	 	else return West;
	 }
}

void Unit::Facing(FacingSide face, stringw animationName)
{
	switch (face)
	{
		case North:
		{
			PlayAnimation(stringw(L"Back") + animationName, false);
			return;
		}
		case East:
		{
			PlayAnimation(stringw(L"Back") + animationName, true);
			return;
		}
		case West:
		{
			PlayAnimation(animationName, false);
			return;
		}
		case South:
		{
			PlayAnimation(animationName, true);
			return;
		}
		default:
			return;
	}
}

void Unit::ReserveFill(int value)
{
	m_reserve += std::min(value, m_turnCounter);
	m_turnCounter -= std::min(value, m_turnCounter);
}

void Unit::TurnPassed(bool walk, bool action)
{
	m_turnCounter = std::min(0, m_turnCounter - 500 - (walk?300:0) - (action?200:0));
	m_reserve = 0;
}

bool Unit::ClockTick()
{
	if (m_currentHP <= 0) return false;
	m_turnCounter += m_speed + m_reserve;
	m_reserve = 0;
	return m_turnCounter >= 1000;
}

HitData Unit::GetDamageData(int action, Unit* theUnit)
{
	// Needs to update after more data is availble
	HitData data;
	data.Damage = 0;
	data.HitRate = 0;
	if (theUnit == NULL) return data;
	FacingSide myFac;
	float xDif = theUnit->BFPosition().X() - m_pos.X();
	float yDif = theUnit->BFPosition().Y() - m_pos.Y();
	if (fabs(xDif) >= fabs(yDif))
	{
		if (xDif < 0) myFac = West; else myFac = East;
	} else
	{
		if (yDif < 0) myFac = South; else myFac = North;
	}
	Facing(myFac);
	FacingSide otherFac = theUnit->Facing();
	if (action == 0)
	{
		data.HitRate = std::min(theUnit->Evade(), 100);
		switch(myFac)
		{
			case North:
			{
				switch(otherFac)
				{
					case North:
					{
						data.HitRate /= 4;
						break;
					}
					case East:
					{
						data.HitRate /= 2;
						break;
					}
					case South:
					{
						break;
					}
					case West:
					{
						data.HitRate /= 2;
						break;
					}
				}
				break;
			}
			case East:
			{
				switch(otherFac)
				{
					case North:
					{
						data.HitRate /= 2;
						break;
					}
					case East:
					{
						data.HitRate /= 4;
						break;
					}
					case South:
					{
						data.HitRate /= 2;
						break;
					}
					case West:
					{
						break;
					}
				}
				break;
			}
			case South:
			{
				switch(otherFac)
				{
					case North:
					{
						break;
					}
					case East:
					{
						data.HitRate /= 2;
						break;
					}
					case South:
					{
						data.HitRate /= 4;
						break;
					}
					case West:
					{
						data.HitRate /= 2;
						break;
					}
				}
				break;
			}
			case West:
			{
				switch(otherFac)
				{
					case North:
					{
						data.HitRate /= 2;
						break;
					}
					case East:
					{
						break;
					}
					case South:
					{
						data.HitRate /= 2;
						break;
					}
					case West:
					{
						data.HitRate /= 4;
						break;
					}
				}
				break;
			}
		}
		data.HitRate = 100 - data.HitRate;
		data.HitRate = std::max(5, data.HitRate);
		data.HitRate = std::min(95, data.HitRate);
		data.Damage = m_baseAttack - (theUnit->BaseDefense() / 2);
	}
	return data;
}

vector<stringw> Unit::GetActionListAsString()
{
	vector<stringw> actionList;
	actionList.push_back(L"*Attack");
	return actionList;
}

void Unit::OrderMove(const vector<vector3di>& thePath)
{
	m_walking = true;
	m_path = thePath;
}

void Unit::Attack()
{
	Facing(Facing(), L"Attack");
	Sprite().Looping(false);
	m_attacking = true;
}

void Unit::Attacked(HitData theData)
{
	if (GetIrrlicht::GetRandomInt(0, 100) <= theData.HitRate) // Hits
	{
		m_currentHP -= GetIrrlicht::GetRandomInt(round(theData.Damage * 0.9f), round(theData.Damage * 1.1f));
		if (m_currentHP <= 0)
		{
			m_currentHP = 0;
			Facing(Facing(), L"Dying");
			Sprite().Looping(false);
		} else
		{
			Facing(Facing(), L"Hurt");
			Sprite().Looping(false);
			m_hurting = true;
		}
	} else // Miss
	{
		Facing(Facing(), L"Evade");
		Sprite().Looping(false);
		m_hurting = true;
	}
}

// XML 
void Unit::ReadXML(IXMLReader* xml)
{
	do 
	{
		switch (xml->getNodeType())
		{
			case irr::io::EXN_ELEMENT:
			{
				if (NameTag.equals_ignore_case(xml->getNodeName()))
				{
					NamedObject::ReadXML(xml);
				} else if (AnimatingObjectTag.equals_ignore_case(xml->getNodeName()))
				{
					AnimatingObject::ReadXML(xml);
				} else if (UnitTag.equals_ignore_case(xml->getNodeName()))
				{
					m_currentHP = xml->getAttributeValueAsInt(L"CurrentHP");
					m_currentMP = xml->getAttributeValueAsInt(L"CurrentMP");
					m_hp = xml->getAttributeValueAsInt(L"MaxHP");
					m_mp = xml->getAttributeValueAsInt(L"MaxMP");
					m_move = xml->getAttributeValueAsInt(L"Move");
					m_jump = xml->getAttributeValueAsInt(L"Jump");
					m_evade = xml->getAttributeValueAsInt(L"Evade");
					m_baseAttack = xml->getAttributeValueAsInt(L"BaseAttack");
					m_baseDefense = xml->getAttributeValueAsInt(L"BaseDefense");
					m_baseMagicAttack = xml->getAttributeValueAsInt(L"BaseMagicAttack");
					m_baseMagicDefense = xml->getAttributeValueAsInt(L"BaseMagicDefense");
					m_speed = xml->getAttributeValueAsInt(L"Speed");
					m_jp = xml->getAttributeValueAsInt(L"JP");
					m_level = xml->getAttributeValueAsInt(L"Level");
					m_xp = xml->getAttributeValueAsInt(L"XP");
				} else if (BattleFieldPositionTag.equals_ignore_case(xml->getNodeName()))
				{
					m_pos.ReadXML(xml);
				} else return;
				break;
			}
			case irr::io::EXN_ELEMENT_END:
				if (UnitTag.equals_ignore_case(xml->getNodeName()))
				{
					return;
				} 
				break;
			default:
				break;			
		}
	} while (xml->read());
}

void Unit::WriteXML(IXMLWriter* xml)
{
	irr::core::array<stringw> nameArray;
	nameArray.push_back(L"CurrentHP");
	nameArray.push_back(L"CurrentMP");
	nameArray.push_back(L"MaxHP");
	nameArray.push_back(L"MaxMP");
	nameArray.push_back(L"Move");
	nameArray.push_back(L"Jump");
	nameArray.push_back(L"Evade");
	nameArray.push_back(L"BaseAttack");
	nameArray.push_back(L"BaseDefense");
	nameArray.push_back(L"BaseMagicAttack");
	nameArray.push_back(L"BaseMagicDefense");
	nameArray.push_back(L"Speed");
	nameArray.push_back(L"JP");
	nameArray.push_back(L"Level");
	nameArray.push_back(L"XP");
	irr::core::array<stringw> valueArray;
	valueArray.push_back(stringw(m_currentHP));
	valueArray.push_back(stringw(m_currentMP));
	valueArray.push_back(stringw(m_hp));
	valueArray.push_back(stringw(m_mp));
	valueArray.push_back(stringw(m_move));
	valueArray.push_back(stringw(m_jump));
	valueArray.push_back(stringw(m_evade));
	valueArray.push_back(stringw(m_baseAttack));
	valueArray.push_back(stringw(m_baseDefense));
	valueArray.push_back(stringw(m_baseMagicAttack));
	valueArray.push_back(stringw(m_baseMagicDefense));
	valueArray.push_back(stringw(m_speed));
	valueArray.push_back(stringw(m_jp));
	valueArray.push_back(stringw(m_level));
	valueArray.push_back(stringw(m_xp));
	xml->writeElement(UnitTag.c_str(), false, nameArray, valueArray);
	xml->writeLineBreak();
	NamedObject::WriteXML(xml);
	AnimatingObject::WriteXML(xml);
	m_pos.WriteXML(xml);
	xml->writeClosingTag(UnitTag.c_str());
	xml->writeLineBreak();
}

 
