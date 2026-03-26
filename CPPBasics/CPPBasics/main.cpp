#include <iostream>
#include <string>
#include <ctime>        //랜덤함수를 위해 헤더 추가
#include <cstdlib>      //  ==
using namespace std;

class Unit
{
public:

	string m_UnitName;
	int m_hp;
	int m_power;
	int m_speed;


	Unit(string name, int hp, int power, int speed) //생성자를 통해 하나 하나 대입하는 수고를 덜기
		: m_UnitName(name), m_hp(hp), m_power(power), m_speed(speed)
	{

	}
	virtual void attack(Unit& target)  //AI가 마린의 특수 공격을 위해 추가 권장								
	{                                  //컴퓨터가 일반 유닛으로 인지하여 
		cout << "\n\n <" << m_UnitName << "> 이 <" << target.m_UnitName << "> 을 공격!" << endl;
		target.takeDamage(m_power);
	}


	virtual void takeDamage(int damage)
	{
		m_hp -= damage;
	}




};

class Marine : public Unit
{
public:
	Marine() : Unit("Marine", 100, 10, 15)    //부모 생성자를 기반으로 마린 생성자 작성
	{

	}
	
	void attack(Unit& target) override     //부모 클래스의 공격 기반이지만 마린 만의 특징을 작성
	{									  
		Unit::attack(target);
		cout << "  [ You wanna piece of me, boy? ]  마린의 점점 강력해지는 공격" << endl;

		m_power += 3;                    //스팀팩 스킬 구현  때릴때마다 공격력 3이 올라가지만
		if (m_hp > 1)                    // HP 1을 잃는다. 하지만 피가 1보다 많을때만 적용 된다.
		{
			m_hp -= 1;
			cout << " 마린이 공격마다 스팀팩을 주사합니다! 공격력이 " << m_power << "로 상승하지만 1씩 체력이 깎입니다! " << endl;
		}
		
	}

};

class Zergling : public Unit
{
public:
	Zergling() : Unit("Zergling", 40 * 2, 5 * 2, 20)    //부모 클래스로 저글링 생성자(사실 그냥 연산하여 기입해도 되지만 2마리 라는것을 강조)
	{

	}

	void attack(Unit& target) override
	{
		Unit::attack(target);
		cout << " 저글링 2마리가 맹렬하게 달려들어 " << target.m_UnitName << " 의 발목을 물어뜯습니다! " << endl;
	}


};

class Zealot : public Unit
{
public:
	int m_shield = 30;
	int m_attackTimer = 0;

	Zealot() : Unit("Zealot", 100, 16, 12) //질럿 생성자
	{
		
	}

	void attack(Unit& target) override
	{
		if (m_attackTimer == 0)
		{
			Unit::attack(target);
			m_attackTimer = 1;
			cout << "[ My life for Aiur! ] 질럿의 강력한 충전 공격!\n\n" << endl;
		}
		else
		{
			m_attackTimer = 0;    // 쉬었으니 다음 턴엔 공격 가능
			cout << "\n\n질럿 공격 충전중...\n\n" << endl;
		}
	}

	void takeDamage(int damage) override     //내구력이 있는 질럿 보호막 추가
	{
		if (m_shield > 0)
		{
			m_shield -= damage;
			cout << " 질럿 보호막 작동! (남은 보호막: " << (m_shield > 0 ? m_shield : 0) << ")" << endl;

			if (m_shield < 0)
			{
				m_hp += m_shield;
				m_shield = 0;
			}

		}
		else
			Unit::takeDamage(damage);
	}
};


	int main()
	{
		srand((unsigned int)time(NULL));     //AI 참조 하여 랜덤문 추가
		Marine m;                            //각 유닛들 소환
		Zergling z;
		Zealot ze;

		cout << "--- 종족별 3파전 대난투 시작! ---" << endl;
		cout << "----------------------------------" << endl;

		int turn = 1;

		while (true)

		{
			system("cls");
			cout << "\n[TURN " << turn << "]" << endl;
			cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
			cout << "  마린 HP: " << m.m_hp << " | 저글링 HP: " << z.m_hp << " | 질럿 HP: " << ze.m_hp << " (S: " << ze.m_shield << ")" << endl;
			cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
			bool mAlive = m.m_hp > 0;                       //1명 남을때까지 싸우기
			bool zAlive = z.m_hp > 0;
			bool zeAlive = ze.m_hp > 0;




			int aliveCount = (int)mAlive + (int)zAlive + (int)zeAlive;           //살아있는 유닛 1개 이하면 종료
			if (aliveCount <= 1) break;

			cout << "\n[TURN " << turn << "]" << endl;

			if (mAlive)
			{
				if (zAlive && zeAlive)
				{
					(rand() % 2 == 0) ? m.attack(z) : m.attack(ze);    //랜덤으로 2중 1명 때리기
				}
				else if (zAlive) {
					m.attack(z);
				}
				else if (zeAlive) {
					m.attack(ze);
				}
			}

			if (zAlive)
			{
				if (mAlive && zeAlive)
				{
					(rand() % 2 == 0) ? z.attack(m) : z.attack(ze);
				}
				else if (mAlive)
				{
					z.attack(m);
				}
				else if (zeAlive)
				{
					z.attack(ze);
				}
			}

			if (zeAlive)
			{
				if (mAlive && zAlive)
				{
					(rand() % 2 == 0) ? ze.attack(m) : ze.attack(z);
				}
				else if (mAlive)
				{
					ze.attack(m);
				}
				else if (zAlive)
				{
					ze.attack(z);
				}
			}

			turn++;        //빨리 지나가기 방지
			system("pause");
		}
		cout << "\n======================================" << endl;
		if (m.m_hp > 0) cout << " 최종 승자: 테란 마린!!" << endl;
		else if (z.m_hp > 0) cout << " 최종 승자: 저그 저글링!!" << endl;
		else if (ze.m_hp > 0) cout << " 최종 승자: 프로토스 질럿!!" << endl;
		else cout << " 모두가 전사했습니다... 무승부!" << endl;
		cout << "======================================" << endl;

		return 0;
	}



