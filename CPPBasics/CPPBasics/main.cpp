#include <iostream>  // 표준 입출력(cin, cout)을 위한 기본 헤더
#include <string>    // 문자열(string) 데이터 타입을 자유롭게 쓰기 위한 헤더
#include <cstdlib>   // rand() (난수 생성), srand() (난수 시드 설정)를 쓰기 위한 헤더                    
#include <ctime>     // time(NULL)로 현재 시간을 가져와 난수 시드를 매번 바꾸기 위한 헤더                    
#include <conio.h>   // _kbhit() (키보드 입력 감지), _getch() (버퍼 없는 실시간 키 입력)용 헤더                   
#include <windows.h> // Sleep() (화면 딜레이), system("cls") (화면 지우기) 등 Windows 전용 기능 헤더                 

using namespace std; // std::cout 대신 cout으로 편하게 쓰기 위한 네임스페이스 선언

// [ 전방 선언 (Forward Declaration) ]
// monster 클래스가 character 클래스보다 아래에 정의되어 있습니다.
// 하지만 character 안에서 monster의 주소(포인터)를 매개변수로 쓰기 때문에, 
// 컴파일러에게 "monster라는 클래스가 밑에 실존하니까 에러 내지 마!" 하고 미리 알려주는 것입니다.
class monster;

// --- [ 전역 구조체 정의 ] ---
// 플레이어가 획득하는 전리품 및 퀘스트 아이템 개수를 저장하는 '가방'입니다.
struct Inventory {
	int fangs = 0;        // 고신키의 송곳니 (사용 시 공격력 증가)
	int mirrors = 0;      // 칸나의 거울 (사용 시 최대 HP 증가)
	int fans = 0;         // 카라의 부채 (사용 시 치명타율 증가)
	int jewelShards = 0;   // 사원의 구슬 조각 (메인 수집품)
};

// --- [ 캐릭터 부모 클래스 정의 ] ---
class character
{
protected: // 자식 클래스(Inuyasha, kagome 등)들에게만 접근을 허용하는 접근 제어자입니다.
	string m_charname;      // 캐릭터 이름
	string m_weaponName;    // 무기 이름
	string m_job;           // 직업
	string m_skillName;     // 보유 스킬 이름

	int m_hp;               // 현재 체력
	int m_maxHp;            // 최대 체력 (아이템 사용이나 레벨업 확장을 위해 분리)
	int m_attackDamage;     // 기본 공격력
	float m_speed;          // 턴 우선권 및 도망치기 확률을 결정하는 스피드
	float m_criticalChance; // 치명타 확률 (0.25면 25%)
	bool m_isAlwaysEscape = false; // 싯포&키라라의 특수 패시브(무조건 도망)를 위한 플래그 변수

public: // 외부(main 등)에서 자유롭게 호출할 수 있도록 허용하는 접근 제어자입니다.
	// [ 생성자 (Constructor) ] : 객체가 생성될 때 단 한 번 호출되며 멤버 변수들을 초기화합니다.
	character(string name, string weapon, string job, string skillName, int hp, int atd, float speed, float ctc)
		: m_charname(name), m_weaponName(weapon), m_job(job), m_skillName(skillName), m_hp(hp), m_maxHp(hp), m_attackDamage(atd), m_speed(speed), m_criticalChance(ctc)
	{
		// 초기화 리스트를 사용하여 깔끔하게 대입했습니다.
	}

	// [ 가상 소멸자 (Virtual Destructor) ]
	// 다형성을 쓸 때, 부모 포인터로 자식 객체를 delete 할 경우 자식의 소멸자가 정상 호출되도록 보장합니다.
	// 메모리 누수(Memory Leak)를 막기 위한 객체지향 프로그래밍(OOP)의 필수 테크닉입니다.
	virtual ~character() {}

	// [ 가상 함수 (Virtual Function) ]
	// 자식 클래스들이 자신만의 스킬이나 대사로 재정의(Overriding)할 수 있도록 virtual을 붙여줍니다.
	virtual void useSkill()
	{
		cout << m_charname << " 이(가) 기술을 시전합니다! " << endl;
	}

	virtual void printCriticalLine()
	{
		cout << m_charname << "의 치명타 공격!" << endl;
	}

	// 가영이의 '기절 화살' 패시브처럼 공격 대상의 상태를 바꾸기 위해 monster* target(포인터) 매개변수를 받습니다.
	virtual void attack(monster* target = nullptr)
	{
		// 0 ~ 99 사이의 난수를 생성합니다.
		int randomValue = rand() % 100;

		// 만약 치명타 확률이 0.25(25%)라면, 난수가 25 미만일 때 치명타가 터집니다.
		if (randomValue < (m_criticalChance * 100))
		{
			int criticalDamage = m_attackDamage * 2; // 치명타는 기본 대미지의 2배

			cout << "\n----------------------------------------" << endl;
			cout << "\n 치명타 발동 " << endl;
			cout << m_charname << "이(가) " << m_weaponName << "(으)로 공격합니다!" << endl;

			printCriticalLine(); // 가상 함수 호출 (자식이 오버라이딩한 대사가 출력됨)

			cout << "[적에게 치명타가 적용 되어 " << criticalDamage << "의 강력한 대미지를 입혔습니다!]" << endl;
			cout << "----------------------------------------" << endl;
		}
		else
		{
			// 치명타가 안 터진 일반 공격
			cout << "\n----------------------------------------" << endl;
			cout << m_charname << "이(가) " << m_weaponName << "(으)로 공격합니다!" << endl;
			cout << "[적에게 " << m_attackDamage << "의 대미지를 입혔습니다. ]" << endl;
			cout << "----------------------------------------" << endl;
		}
	}

	// 피해를 입는 함수입니다. 산고의 반격 패시브 처리를 위해 공격한 몬스터의 주소를 받습니다.
	virtual void takeDamage(int damage, monster* attacker = nullptr) {
		m_hp -= damage; // 현재 체력에서 대미지만큼 차감

		cout << "\n" << m_charname << "이(가) " << damage << "의 대미지를 입었습니다!" << endl;

		// 체력이 0 이하로 떨어지면 게임 오버 처리를 위해 0으로 고정합니다.
		if (m_hp <= 0)
		{
			m_hp = 0;
			cout << "!!! " << m_charname << "이(가) 더 이상 버티지 못하고 쓰러졌습니다... (GAME OVER)" << endl;
		}
		else
		{
			cout << "현재 남은 체력: " << m_hp << endl;
		}
	}

	// 도망치기 성공 여부를 판단하는 논리(bool) 함수입니다.
	virtual bool tryEscape(float monsterSpeed)
	{
		// 싯포&키라라의 특수 패시브가 true라면 묻지도 따지지도 않고 성공 처리합니다.
		if (m_isAlwaysEscape)
		{
			cout << "\n=========================================" << endl;
			cout << "[ " << m_charname << "의 특수 능력! 공중으로 날아올라 무조건 도망칩니다! ]" << endl;
			cout << "=========================================\n" << endl;
			return true;
		}

		cout << "\n-----------------------------------------" << endl;
		cout << m_charname << "이(가) 전장에서 이탈을 시도합니다!" << endl;

		// 플레이어의 스피드가 몬스터의 스피드보다 10 이상 높아야 도망에 성공합니다.
		if (m_speed >= (monsterSpeed + 10.0f))
		{
			cout << "[ 안전하게 도망쳤습니다. ]" << endl;
			cout << "-----------------------------------------\n" << endl;
			return true;
		}
		else
		{
			cout << "[ 도망칠 수 없습니다. ]" << endl;
			cout << "-----------------------------------------\n" << endl;
			return false;
		}
	}

	// [ Getter 및 Setter, 보조 메서드들 ]
	// 전투 승리 후 체력을 회복시켜 주는 함수입니다.
	void heal(int amount) {
		m_hp += amount;
		if (m_hp > m_maxHp) m_hp = m_maxHp; // 최대 체력을 초과하여 오버힐 되는 것을 방지합니다.
		cout << m_charname << "의 체력이 " << amount << " 회복되었습니다! (현재 HP: " << m_hp << "/" << m_maxHp << ")" << endl;
	}

	// 가방에서 아이템을 썼을 때 스탯을 올려주는 함수들입니다.
	void addAtk(int amount) { m_attackDamage += amount; }
	void addMaxHp(int amount) { m_maxHp += amount; m_hp += amount; } // 최대 체력과 현재 체력을 동시에 올려줍니다.
	void addCriticalChance(float amount) { m_criticalChance += amount; }

	// 캡슐화(Encapsulation) 원칙에 따라, protected 변수들을 안전하게 외부로 꺼내주는 Getter 함수들입니다.
	string getName() const { return m_charname; }
	int getHp() const { return m_hp; }
	int getMaxHp() const { return m_maxHp; }
	int getAtk() const { return m_attackDamage; }
	float getSpeed() const { return m_speed; }
	float getCriticalChance() const { return m_criticalChance; }
};

// --- [ 몬스터 부모 클래스 정의 ] ---
class monster
{
protected:
	string m_monsterName; // 몬스터 이름
	int m_hp;             // 몬스터 체력
	int m_attackDamage;   // 몬스터 공격력
	float m_speed;        // 몬스터 스피드 (선제공격권 결정)
	string m_loot;        // 처치 시 드랍하는 전리품 이름
	int m_stunTurns;      // 기절 지속 턴을 체크하는 변수 (0이면 정상 행동)

	// [수정] 몬스터가 '총 몇 번 기절했는지' 누적 카운팅하는 변수입니다.
	int m_totalStunCount;

public:
	monster(string name, int hp, int atd, float speed, string loot)
		: m_monsterName(name), m_hp(hp), m_attackDamage(atd), m_speed(speed), m_loot(loot), m_stunTurns(0), m_totalStunCount(0) {
	}

	virtual ~monster() {} // 자식 몬스터들의 안전한 소멸을 위한 가상 소멸자

	// 몬스터가 내 턴일 때 행동하는 함수입니다.
	virtual int takeTurn() {
		// 기절 수치가 0보다 크면 공격하지 못하고 턴을 스킵합니다.
		if (m_stunTurns > 0)
		{
			cout << "\n[ " << m_monsterName << "은(는) 기절 상태입니다. 남은 턴: " << m_stunTurns << " ]" << endl;
			m_stunTurns--; // 턴 소모 후 수치를 1 깎습니다.
			return 0;      // 가한 대미지는 0입니다.
		}
		return attack(); // 기절 상태가 아니라면 정상 공격을 수행합니다.
	}

	virtual int attack()
	{
		cout << "\n----------------------------------------" << endl;
		cout << m_monsterName << "이(가) 기본 공격을 시도합니다!" << endl;
		return m_attackDamage;
	}

	virtual int useSkill() { return 0; } // 카라나 모료마루 같은 특수 몬스터만 재정의해서 씁니다.

	void takeDamage(int damage)
	{
		m_hp -= damage;
		if (m_hp <= 0) m_hp = 0; // 음수 체력이 되지 않도록 방지
	}

	// 외부(가영이의 공격 등)에서 이 몬스터를 기절시킬 때 쓰는 함수입니다.
	void setStun(int turns) {
		m_stunTurns = turns;
		m_totalStunCount++; // 기절이 걸릴 때마다 누적 횟수를 1 증가시킵니다.
	}

	// 현재 몬스터가 기절 상태인지 외부에서 파악하기 위한 Getter 함수입니다.
	int getStunTurns() const { return m_stunTurns; }

	// [수정] 현재 몬스터가 총 몇 번 기절했는지 외부(가영이)에서 확인하기 위한 Getter 함수입니다.
	int getTotalStunCount() const { return m_totalStunCount; }

	string getName() const { return m_monsterName; }
	int getHp() const { return m_hp; }
	float getSpeed() const { return m_speed; }
	string getLoot() const { return m_loot; }
};

// --- [ 플레이어 파생 클래스들 정의 ] ---

// 1. 이누야샤 (밸런스형 딜러)
class Inuyasha : public character
{
private:
	bool m_isExhausted = false; // 바람의 상처 사용 후 오는 후유증(탈진) 체크용 변수

public:
	// 자식 생성자에서 부모 생성자(character)를 먼저 호출하여 기본 스탯들을 세팅합니다.
	Inuyasha()
		: character(" 이누야샤 ", " 철쇄아 ", " 전사 ", " 바람의 상처 ", 1000, 150, 20.0f, 0.25f) {
	}

	// [ 오버라이딩 (Overriding) ] : 부모의 스킬 시스템을 이누야샤 전용 광역기로 덮어씌웁니다.
	void useSkill() override
	{
		cout << "\n========================================" << endl;
		cout << m_charname << " : " << m_skillName << "!!!" << endl;
		cout << "----------------------------------------" << endl;
		cout << "[ 콰아앙! 거대한 검기가 화면의 적을 휩씁니다. ]" << endl;
		cout << "[ 적에게 600의 고정 대미지를 입혔습니다. ]" << endl;
		cout << "[ 주의: 기술의 반동으로 이누야샤가 다음 턴에 행동 불능이 됩니다! ]" << endl;
		cout << "========================================\n" << endl;

		m_isExhausted = true; // 스킬 사용 후 탈진 상태 켜기
	}

	void printCriticalLine() override { cout << m_charname << " : 산혼철조!!" << endl; }

	bool isExhausted() const { return m_isExhausted; }
	void recover() { m_isExhausted = false; } // 탈진 상태를 해제해 주는 함수
};

// 2. 가영 (치명타 및 상태이상 특화 궁수)
class kagome : public character
{
public:
	kagome()
		: character(" 가영 ", " 파마의 활 ", " 궁수 ", " 꿰뚫어 보는 눈 ", 800, 120, 25.0f, 0.9f) {
	}

	// [ 부모 함수 덮어쓰기 ] : 치명타가 터졌을 때 적에게 1턴 기절을 부여하는 특수 평타 패시브 구현
	// [수정] 누적 5회 이상 기절한 몬스터는 '기절 면역' 상태가 되어 더 이상 기절하지 않습니다.
	void attack(monster* target = nullptr) override
	{
		int randomValue = rand() % 100;

		if (randomValue < (m_criticalChance * 100))
		{
			int criticalDamage = m_attackDamage * 2;

			cout << "\n----------------------------------------" << endl;
			cout << "\n 치명타 발동 " << endl;
			cout << m_charname << "이(가) " << m_weaponName << "(으)로 공격합니다!" << endl;

			printCriticalLine();

			cout << "[적에게 치명타가 적용 되어 " << criticalDamage << "의 강력한 대미지를 입혔습니다!]" << endl;

			// 대상 몬스터가 존재한다면 기절을 판정합니다.
			if (target != nullptr) {
				// 1. 이미 기절해 있는 몬스터는 기절이 중첩되거나 턴이 갱신되지 않습니다.
				if (target->getStunTurns() > 0) {
					cout << "[ 적은 이미 기절 상태입니다. 기절이 중첩되지 않습니다! ]" << endl;
				}
				// 2. [수정 핵심] 총 기절 횟수가 5회 이상인 몬스터는 기절 면역이 됩니다.
				else if (target->getTotalStunCount() >= 5) {
					cout << "[ 적이 기절에 내성이 생겼습니다! 더 이상 기절하지 않습니다! ]" << endl;
				}
				// 3. 기절해 있지도 않고, 기절 횟수도 5회 미만일 때만 안전하게 기절을 부여합니다.
				else {
					cout << "[ 급소에 맞아 적이 움직일 수 없게 되었습니다. (누적 기절: " << target->getTotalStunCount() + 1 << "/5 회) ]" << endl;
					target->setStun(1);
				}
			}
			cout << "----------------------------------------" << endl;
		}
		else
		{
			cout << "\n----------------------------------------" << endl;
			cout << m_charname << "이(가) " << m_weaponName << "(으)로 공격합니다!" << endl;
			cout << "[적에게 " << m_attackDamage << "의 대미지를 입혔습니다. ]" << endl;
			cout << "----------------------------------------" << endl;
		}
	}

	void useSkill() override
	{
		cout << "\n========================================" << endl;
		cout << m_charname << "은(는) 사용 가능한 액티브 스킬이 없습니다." << endl;
		cout << "========================================\n" << endl;
	}

	void printCriticalLine() override { cout << m_charname << " : 닿아라 파마의 화살! " << endl; }
};

// 3. 미륵 (스킬 위주의 마법사)
class miroku : public character
{
public:
	miroku()
		: character(" 미륵 ", " 석장 ", " 마법사 ", " 풍혈 ", 800, 130, 20.0f, 0.2f) {
	}

	void useSkill() override
	{
		cout << "\n========================================" << endl;
		cout << m_charname << " : 열려라!! " << m_skillName << "!!!" << endl;
		cout << "----------------------------------------" << endl;
		cout << "[ 미륵의 오른손 봉인이 해제 되며 웜홀이 열립니다 ]" << endl;
		cout << "[ 적에게 360의 고정 대미지를 입혔습니다.  ]" << endl;
		cout << "========================================\n" << endl;
	}

	void printCriticalLine() override { cout << m_charname << " : 까불지마 " << endl; }
};

// 4. 산고 (연격과 반격의 하이브리드 전사)
class sango : public character
{
public:
	sango()
		: character(" 산고 ", " 비래골 & 단검 ", " 웨폰 마스터리 ", " 연격,반격", 900, 90, 20.0f, 0.07f) {
	}

	// [ 평타 오버라이딩 ] : 산고의 패시브인 '연격'(대미지 2배) 구현
	void attack(monster* target = nullptr) override
	{
		int originDamage = m_attackDamage; // 원래 공격력을 백업해 둡니다.
		m_attackDamage = originDamage * 2; // 일시적으로 공격력을 2배로 뻥튀기합니다.

		cout << "\n 산고의 패시브 연격: 비래골의 부메랑 특성으로 공격력의 2배만큼 대미지를 줍니다" << endl;
		character::attack(target); // 부모의 공격 로직(치명타 판정 등)을 그대로 재사용합니다.
		m_attackDamage = originDamage; // 공격이 끝난 후 다시 원래 공격력으로 원상 복구합니다.
	}

	// [ 피격 오버라이딩 ] : 산고의 패시브인 '반격'(맞은 대미지의 2배 반사) 구현
	void takeDamage(int damage, monster* attacker = nullptr) override {
		character::takeDamage(damage, attacker); // 일단 부모 로직대로 정상적으로 대미지를 입습니다.

		// 만약 나를 때린 몬스터가 존재하고, 산고가 그 대미지를 받고도 살아남았다면 반격합니다.
		if (attacker != nullptr && m_hp > 0) {
			int counterDamage = damage * 2;
			cout << "\n[ 산고의 패시브 반격 발동! ]" << endl;
			cout << "[ 받은 대미지의 2배인 " << counterDamage << "의 대미지를 " << attacker->getName() << "에게 되돌려줍니다! ]" << endl;
			attacker->takeDamage(counterDamage);
		}
	}

	void printCriticalLine() override { cout << m_charname << " : 가라, 비래골!!! 제대로 들어갔다!" << endl; }
};

// 5. 싯포&키라라 (생존 및 도주 특화 서머너)
class shippo_kirara : public character
{
public:
	shippo_kirara()
		: character(" 싯포&키라라 ", " 싯포의 요술 도구", " 서머너 ", " 여우불 ", 900, 130, 30.0f, 0.5f)
	{
		m_isAlwaysEscape = true; // 무조건 도망치기 패시브를 true로 켜줍니다.
	}

	void useSkill() override
	{
		cout << "\n=========================================" << endl;
		cout << m_charname << " : 에잇! 여우불! 키라라, 기선제압이야!" << endl;
		cout << "-----------------------------------------" << endl;
		cout << "[ 키라라가 싯포를 태우고 공중으로 날아올라 선제공격을 가합니다! ]" << endl;
		cout << "=========================================\n" << endl;
	}

	void printCriticalLine() override { cout << m_charname << " : 싯포님의 요술을 얕보지 마라!" << endl; }
};

// --- [ 몬스터 파생 클래스들 ] ---

// 1. 고신키 (공격할 때마다 스피드가 빨라짐)
class goshinki : public monster
{
public:
	goshinki() : monster("고신키", 1200, 30, 15.0f, "고신키의 날카로운 송곳니") {}

	int attack() override
	{
		cout << "\n----------------------------------------" << endl;
		cout << m_monsterName << "이(가) 날카로운 이빨로 물어뜯습니다!" << endl;
		m_speed += 1.0f; // 공격을 가할 때마다 스피드가 증가하여 도망치기 힘들어집니다.
		cout << "[ 고신키의 움직임이 빨라집니다! (스피드 증가) ]" << endl;
		return m_attackDamage;
	}
};

// 2. 칸나 (공격할 때마다 자신의 피를 흡수함)
class kanna : public monster
{
public:
	kanna() : monster("칸나", 900, 100, 10.0f, "칸나의 거울") {}

	int attack() override
	{
		cout << "\n----------------------------------------" << endl;
		cout << m_monsterName << "이(가) 하얀 거울을 들어 올립니다." << endl;
		m_hp += 10; // 플레이어에게 피해를 주면서 자신의 체력을 10 흡수하여 회복합니다.
		cout << "[ 칸나의 거울이 생기를 흡수하여 체력을 10 회복합니다. ]" << endl;
		return m_attackDamage;
	}
};

// 3. 카라 (30% 확률로 광역 스킬 사용)
class kagura : public monster
{
public:
	kagura() : monster("카라", 1000, 60, 35.0f, "카라의 부채") {}

	int takeTurn() override
	{
		// 만약 기절 상태라면 부모 클래스의 턴 스킵 로직을 수행합니다.
		if (m_stunTurns > 0) return monster::takeTurn();

		// 30% 확률로 특수 스킬을 발동합니다.
		if ((rand() % 100) < 30) return useSkill();
		else return attack(); // 나머지 70% 확률로는 일반 평타
	}

	int attack() override
	{
		cout << "\n----------------------------------------" << endl;
		cout << m_monsterName << "이(가) 가볍게 부채를 휘두릅니다." << endl;
		return m_attackDamage;
	}

	int useSkill() override
	{
		cout << "\n========================================" << endl;
		cout << m_monsterName << " : 풍인의 춤!!!" << endl;
		return 150; // 스킬 대미지 150 반환
	}
};

// 4. 모료마루 (등딱지 보스 - 10% 확률로 초강력 스킬 시전 후 기절)
class moryomaru : public monster
{
public:
	moryomaru() : monster("모료마루", 2000, 110, 50.0f, "단단한 등딱지") {}

	int takeTurn() override
	{
		if (m_stunTurns > 0) return monster::takeTurn();

		// 10% 확률로 아주 강력한 일격을 날립니다.
		if ((rand() % 100) < 10) return useSkill();
		else return attack();
	}

	int attack() override
	{
		cout << "\n----------------------------------------" << endl;
		cout << m_monsterName << "이(가) 강력한 일격을 날립니다!" << endl;
		return m_attackDamage;
	}

	int useSkill() override
	{
		cout << "\n========================================" << endl;
		cout << m_monsterName << " : 금강창파!!!" << endl;
		setStun(2); // 너무 무리해서 기를 쓴 탓에 시전 후 자신도 2턴간 쉼표(기절) 상태가 됩니다.
		return 400; // 대미지는 400으로 치명적입니다.
	}
};

// --- [ 보스 클리어 시 엔딩 연출 함수 ] ---
void showEndingScenario() {
	system("cls"); // 화면을 싹 비우고 엔딩 연출 시작
	cout << "==================================================" << endl;
	cout << "                    에 필 로 그                    " << endl;
	cout << "==================================================\n" << endl;

	Sleep(1000); // 1000ms = 1초 동안 프로그램 실행을 멈춰 연출력을 줍니다.
	cout << "모료마루의 단단한 등딱지가 부서지며 전장에 고요함이 찾아왔다.\n" << endl;
	Sleep(1500);

	cout << "[ 엔딩 ]" << endl;
	cout << "--------------------------------------------------" << endl;
	cout << "전국으로 흩어진 이누야샤 일행은 사원의 조각을 모아" << endl;
	cout << "나락을 향하여 가기 위해 다시 집결하여 결의를 다졌다." << endl;
	cout << "--------------------------------------------------" << endl;
	cout << "끝\n" << endl;
	Sleep(2000);

	cout << "==================================================" << endl;
	cout << "      플레이 해주셔서 감사합니다. (Enter로 종료)  " << endl;
	cout << "==================================================" << endl;

	// 사용자가 Enter(아스키코드 13번)를 누를 때까지 무한 루프로 대기합니다.
	while (_getch() != 13);
}

// --- [ 전투 시스템 함수 ] ---
// 플레이어 포인터와 몬스터 포인터를 받아와 실제로 치고받는 메인 전투 시스템입니다.
bool startBattle(character* player, monster* enemy) {
	system("cls");
	cout << "\n몬스터 [" << enemy->getName() << "]와(과) 마주쳤습니다!\n" << endl;

	cout << "1. 전투 시작  2. 도망치기" << endl;
	cout << "선택하세요: ";
	int initialChoice;
	cin >> initialChoice;

	if (initialChoice == 2) {
		// 도망치기를 시도하고 성공 시 true를 반환받아 맵으로 돌아갑니다.
		if (player->tryEscape(enemy->getSpeed())) {
			cout << "\n[Enter 키를 눌러 맵으로 돌아갑니다.]" << endl;
			while (_getch() != 13);
			return false; // 전투 없이 평화롭게 도망쳤으므로 false 반환
		}
		else {
			cout << "\n도망에 실패하여 강제로 전투에 돌입합니다!" << endl;
			cout << "[Enter 키를 누르세요.]" << endl;
			while (_getch() != 13);
		}
	}

	// 몬스터의 스피드가 플레이어보다 빠르면 몬스터 선제 공격으로 시작합니다.
	bool isPlayerTurn = true;
	if (enemy->getSpeed() > player->getSpeed()) {
		isPlayerTurn = false;
	}

	// 플레이어와 몬스터 둘 다 살아있을 때만 무한 반복하는 전투 루프입니다.
	while (player->getHp() > 0 && enemy->getHp() > 0) {
		system("cls");
		cout << "=========================================" << endl;
		cout << "                전 투 화 면               " << endl;
		cout << "=========================================" << endl;
		cout << " [플레이어] " << player->getName() << " HP: " << player->getHp() << " ATK: " << player->getAtk() << " CRT: " << (player->getCriticalChance() * 100) << "%" << endl;
		cout << " [몬스터]   " << enemy->getName() << " HP: " << enemy->getHp() << endl;
		cout << "=========================================" << endl;

		// 플레이어의 공격 턴일 때
		if (isPlayerTurn) {
			// [ 다형성 다운캐스팅 (Downcasting) ]
			// player 포인터는 부모 타입이라 이누야샤 전용의 '탈진' 변수에 접근하지 못합니다.
			// 그래서 "이거 사실 이누야샤 객체 맞아!" 하고 자식 타입으로 안전하게 형변환 해주는 것입니다.
			Inuyasha* inu = dynamic_cast<Inuyasha*>(player);

			// 이누야샤이고, 탈진 상태라면 턴을 강제 스킵합니다.
			if (inu != nullptr && inu->isExhausted()) {
				cout << "\n[ 이누야샤는 바람의 상처를 쓰고 지쳐서 움직일 수 없습니다! ]" << endl;
				inu->recover(); // 다음 턴에는 행동할 수 있게 탈진을 풀어줍니다.

				cout << "\n[Enter 키를 눌러 턴을 넘깁니다.]" << endl;
				while (_getch() != 13);
			}
			else {
				// 가영이는 액티브 스킬이 없으므로 인터페이스를 다르게 출력합니다.
				if (player->getName().find("가영") != string::npos) {
					cout << "1. 일반 공격 (가영이는 액티브 스킬이 없습니다.)" << endl;
				}
				else {
					cout << "1. 일반 공격  2. 스킬 사용" << endl;
				}

				cout << "행동을 선택하세요: ";
				int action;
				cin >> action;

				if (action == 1) {
					player->attack(enemy); // 일반 공격 (가영이는 기절 부여)
					enemy->takeDamage(player->getAtk());
				}
				else if (action == 2 && player->getName().find("가영") != string::npos) {
					cout << "가영이는 액티브 스킬이 없습니다! 턴을 낭비했습니다." << endl;
				}
				else if (action == 2) {
					player->useSkill();
					// 각 캐릭터별 고유 스킬 대미지 수치를 적용해 줍니다.
					int playerDmg = 200;
					if (player->getName().find("이누야샤") != string::npos) playerDmg = 600;
					else if (player->getName().find("미륵") != string::npos) playerDmg = 360;

					enemy->takeDamage(playerDmg);
				}
				else {
					cout << "잘못된 입력입니다. 턴을 넘깁니다." << endl;
				}

				cout << "\n[Enter 키를 눌러 턴을 넘깁니다.]" << endl;
				while (_getch() != 13);
			}
		}
		// 몬스터의 공격 턴일 때
		else {
			system("cls");
			cout << "=========================================" << endl;
			cout << "                몬스터의 턴               " << endl;
			cout << "=========================================" << endl;

			int monsterDmg = enemy->takeTurn();
			if (monsterDmg > 0) {
				player->takeDamage(monsterDmg, enemy); // 플레이어가 대미지를 입음 (산고 반격 체크)
			}

			cout << "\n[Enter 키를 눌러 턴을 넘깁니다.]" << endl;
			while (_getch() != 13);
		}

		// 턴을 토글(Toggling)하여 플레이어 턴 -> 몬스터 턴 -> 플레이어 턴 순으로 무한 순환시킵니다.
		isPlayerTurn = !isPlayerTurn;
	}

	system("cls");
	cout << "=========================================" << endl;
	cout << "                전투 종료                 " << endl;
	cout << "=========================================" << endl;

	// 혹시 전투가 끝났는데 이누야샤의 탈진 상태가 켜진 채 맵으로 나가면 안 되므로 한 번 더 풀어줍니다.
	Inuyasha* inuAfter = dynamic_cast<Inuyasha*>(player);
	if (inuAfter != nullptr) {
		inuAfter->recover();
	}

	if (player->getHp() <= 0) {
		cout << "\n전투에서 패배했습니다... 게임 오버." << endl;
		cout << "\n[Enter 키를 눌러 종료합니다.]" << endl;
		while (_getch() != 13);
		return false; // 패배 시 false 반환
	}
	else {
		cout << "\n" << enemy->getName() << "을(를) 물리쳤습니다!" << endl;
		cout << "\n[Enter 키를 누르세요.]" << endl;
		while (_getch() != 13);
		return true; // 승리 시 true 반환
	}
}

// 2D 맵 데이터 정의 (가로 5칸, 세로 5칸)
const int MAP_SIZE = 5;
int worldMap[MAP_SIZE][MAP_SIZE] =
{
	{0, 1, 0, 0, 0}, // 1: 고신키
	{0, 0, 0, 0, 0},
	{0, 0, 2, 0, 0}, // 2: 칸나
	{0, 0, 0, 0, 0},
	{0, 0, 3, 0, 9}  // 3: 카라, 9: 모료마루(보스)
};

// 캐릭터 정보 도감 (강화된 도움말)
void showCharacterEncyclopedia() {
	bool inMenu = true;
	while (inMenu) {
		system("cls");
		cout << "==================================================" << endl;
		cout << "                캐릭터 정보 도감                    " << endl;
		cout << "==================================================" << endl;
		cout << "정보를 보고 싶은 캐릭터를 선택하세요 (뒤로가기: 0):" << endl;
		cout << "1. 이누야샤  2. 가영  3. 미륵  4. 산고  5. 싯포&키라라" << endl;
		cout << "선택 : ";
		int subChoice;
		cin >> subChoice;

		system("cls");
		if (subChoice == 0) {
			inMenu = false; // 도감 밖으로 탈출
		}
		else if (subChoice == 1) {
			cout << "=== [ 이누야샤 (전사) ] ===" << endl;
			cout << "HP : 1000 | ATK : 150 | SPEED : 20.0 | CRT : 25%" << endl;
			cout << "액티브 스킬 [바람의 상처] : 적에게 600의 고정 대미지를 주지만, 다음 턴에 행동불능이 됩니다." << endl;
			cout << "치명타 대사 : 산혼철조!!" << endl;
		}
		else if (subChoice == 2) {
			cout << "=== [ 가영 (궁수) ] ===" << endl;
			cout << "HP : 800 | ATK : 120 | SPEED : 25.0 | CRT : 90%" << endl;
			cout << "특징 : 액티브 스킬은 없지만 치명타 확률이 무려 90%에 달하며 치명타 시 상대를 1턴 기절시킵니다. 단, 한 적에게는 최대 5번까지만 기절이 유효합니다." << endl;
			cout << "치명타 대사 : 닿아라 파마의 화살!" << endl;
		}
		else if (subChoice == 3) {
			cout << "=== [ 미륵 (마법사) ] ===" << endl;
			cout << "HP : 800 | ATK : 130 | SPEED : 20.0 | CRT : 20%" << endl;
			cout << "액티브 스킬 [풍혈] : 적에게 360의 강력한 고정 대미지를 입힙니다." << endl;
			cout << "치명타 대사 : 까불지마" << endl;
		}
		else if (subChoice == 4) {
			cout << "=== [ 산고 (웨폰 마스터리) ] ===" << endl;
			cout << "HP : 900 | ATK : 90 | SPEED : 20.0 | CRT : 7%" << endl;
			cout << "특징 : 패시브 연격(공격 시 대미지 2배)과 반격(피격 시 받은 대미지 2배 반사)을 보유한 하이브리드 캐릭터입니다." << endl;
			cout << "치명타 대사 : 가라, 비래골!!! 제대로 들어갔다!" << endl;
		}
		else if (subChoice == 5) {
			cout << "=== [ 싯포&키라라 (서머너) ] ===" << endl;
			cout << "HP : 900 | ATK : 130 | SPEED : 30.0 | CRT : 50%" << endl;
			cout << "특징 : 매우 빠른 스피드와 공중 이동 능력 덕분에 도망치기 커맨드 사용 시 100% 확률로 도망칠 수 있습니다." << endl;
			cout << "치명타 대사 : 싯포님의 요술을 얕보지 마라!" << endl;
		}

		if (subChoice != 0) {
			cout << "\n==================================================" << endl;
			cout << "[Enter 키를 누르면 도감 목록으로 돌아갑니다.]" << endl;
			while (_getch() != 13);
		}
	}
}

// === [ 메인 실행 진입점 ] ===
int main() {
	// 매 실행마다 완전히 새로운 랜덤 시드값을 주기 위해 타임 스탬프를 난수 시드로 지정합니다.
	srand(static_cast<unsigned int>(time(NULL)));

	Inventory myBag;         // 가방 생성
	character* player = nullptr; // 다형성을 위해 부모 포인터로 플레이어를 선언합니다.

	// 캐릭터 생성 루프
	bool selectingCharacter = true;
	while (selectingCharacter) {
		system("cls");
		cout << "==================================================" << endl;
		cout << " [ 시나리오 ]" << endl;
		cout << " 사원의 구슬 조각을 모으기 위한 모험을 시작합니다." << endl;
		cout << "==================================================\n" << endl;

		cout << "조각을 찾으러 떠날 캐릭터를 선택하세요:" << endl;
		cout << "1. 이누야샤  2. 가영  3. 미륵  4. 산고  5. 싯포&키라라" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << "6. [도움말] 캐릭터 정보 보기" << endl;
		cout << "선택 : ";
		int choice;
		cin >> choice;

		// 선택한 번호에 따라 실제 힙(Heap) 메모리에 해당 자식 객체를 동적으로 생성합니다.
		if (choice == 1) { player = new Inuyasha(); selectingCharacter = false; }
		else if (choice == 2) { player = new kagome(); selectingCharacter = false; }
		else if (choice == 3) { player = new miroku(); selectingCharacter = false; }
		else if (choice == 4) { player = new sango(); selectingCharacter = false; }
		else if (choice == 5) { player = new shippo_kirara(); selectingCharacter = false; }
		else if (choice == 6) { showCharacterEncyclopedia(); } // 도감은 계속 봅니다.
	}

	int playerX = 0; // 플레이어 초기 X 위치 좌표
	int playerY = 0; // 플레이어 초기 Y 위치 좌표
	bool isRunning = true; // 게임 루프 실행 플래그
	string systemMessage = "방향키(W,A,S,D)로 움직이세요. [I]를 누르면 가방을 엽니다.";

	// 최초 맵 출력
	system("cls");
	cout << "=========================================" << endl;
	cout << "        이누야샤 실시간 탐험 (종료: Q)   " << endl;
	cout << "=========================================" << endl;
	cout << "\n";
	for (int i = 0; i < MAP_SIZE; i++) {
		cout << "\t";
		for (int j = 0; j < MAP_SIZE; j++) {
			if (i == playerY && j == playerX) cout << "P "; // 현재 있는 곳은 플레이어 표기
			else if (worldMap[i][j] == 9) cout << "B ";     // 약속대로 보스만 맵에 노출
			else cout << ". ";                            // [수정 사항 반영] 일반 몬스터는 마스킹해서 안 보여줌
		}
		cout << endl;
	}
	cout << "\n=========================================" << endl;
	cout << " [알림] : " << systemMessage << endl;
	cout << "=========================================" << endl;

	// 메인 탐험 루프 (실시간 조작 환경)
	while (isRunning) {
		// _kbhit()은 키보드가 눌리는 순간에만 true를 반환하는 함수입니다.
		// 덕분에 입력을 기다리지 않고 실시간으로 다른 연산이나 렌더링이 가능해집니다.
		if (_kbhit()) {
			char key = _getch(); // 버퍼 없이 누른 키 그대로 문자값으로 변환하여 가져옵니다.
			system("cls");       // 키 입력이 감지되면 화면을 싹 지우고 다시 그릴 준비를 합니다.

			// 플레이어 이동 로직 (맵 경계 밖으로 벗어나지 않도록 조건식 포함)
			if ((key == 'w' || key == 'W') && playerY > 0) playerY--;
			else if ((key == 's' || key == 'S') && playerY < MAP_SIZE - 1) playerY++;
			else if ((key == 'a' || key == 'A') && playerX > 0) playerX--;
			else if ((key == 'd' || key == 'D') && playerX < MAP_SIZE - 1) playerX++;

			// 인벤토리 열기
			else if (key == 'i' || key == 'I') {
				system("cls");
				cout << "=========================================" << endl;
				cout << "                가 방 (인벤토리)          " << endl;
				cout << "=========================================" << endl;
				cout << " [1] 고신키의 날카로운 송곳니 : " << myBag.fangs << " 개 (사용 시: 공격력 +5)" << endl;
				cout << " [2] 칸나의 거울 : " << myBag.mirrors << " 개 (사용 시: 최대 HP +50)" << endl;
				cout << " [3] 카라의 부채 : " << myBag.fans << " 개 (사용 시: 치명타율 +10%)" << endl;
				cout << " [4] 사원의 구슬 조각 : " << myBag.jewelShards << " 개 (고유 수집품)" << endl;
				cout << "=========================================" << endl;
				cout << "현재 " << player->getName() << " 스탯 -> HP: " << player->getHp() << "/" << player->getMaxHp() << " | ATK: " << player->getAtk() << " | CRT: " << (player->getCriticalChance() * 100) << "%" << endl;
				cout << "사용할 아이템의 번호를 입력하세요 (나가기: 0): ";

				int itemChoice;
				cin >> itemChoice;

				// 아이템 사용 처리
				if (itemChoice == 1 && myBag.fangs > 0) {
					myBag.fangs--; player->addAtk(5);
					systemMessage = "고신키의 날카로운 송곳니를 사용하여 공격력이 5 증가했습니다!";
				}
				else if (itemChoice == 2 && myBag.mirrors > 0) {
					myBag.mirrors--; player->addMaxHp(50);
					systemMessage = "칸나의 거울을 사용하여 최대 HP가 50 증가했습니다!";
				}
				else if (itemChoice == 3 && myBag.fans > 0) {
					myBag.fans--; player->addCriticalChance(0.10f);
					systemMessage = "카라의 부채를 사용하여 치명타율이 10% 증가했습니다!";
				}
				else if (itemChoice == 0) systemMessage = "가방을 닫았습니다.";
				else systemMessage = "아이템이 부족하거나 잘못된 선택입니다!";
			}
			// 게임 종료 키
			else if (key == 'q' || key == 'Q') {
				cout << "게임을 종료합니다." << endl;
				break;
			}

			// [ 몬스터 조우 판정 ]
			int currentTile = worldMap[playerY][playerX];
			if (currentTile != 0) {
				monster* enemy = nullptr;

				if (currentTile == 1) enemy = new goshinki();
				else if (currentTile == 2) enemy = new kanna();
				else if (currentTile == 3) enemy = new kagura();
				else if (currentTile == 9) enemy = new moryomaru();

				if (enemy != nullptr) {
					bool victory = startBattle(player, enemy);

					if (victory) {
						if (currentTile == 9) {
							delete enemy;
							showEndingScenario();
							isRunning = false;
							break;
						}

						systemMessage = enemy->getName() + " 처치! 구슬 조각을 획득하여 HP가 1000 회복되었습니다.";

						if (currentTile == 1) myBag.fangs++;
						else if (currentTile == 2) myBag.mirrors++;
						else if (currentTile == 3) myBag.fans++;

						myBag.jewelShards++;
						player->heal(1000);

						worldMap[playerY][playerX] = 0;
					}
					else {
						if (player->getHp() <= 0) { delete enemy; break; }
						systemMessage = "전투에서 도망쳤습니다.";
					}
					delete enemy;
				}
			}

			// 맵 다시 렌더링
			if (isRunning) {
				cout << "=========================================" << endl;
				cout << "        이누야샤 실시간 탐험 (종료: Q)   " << endl;
				cout << "=========================================" << endl;

				cout << "\n";
				for (int i = 0; i < MAP_SIZE; i++) {
					cout << "\t";
					for (int j = 0; j < MAP_SIZE; j++) {
						if (i == playerY && j == playerX) cout << "P ";
						else if (worldMap[i][j] == 9) cout << "B ";
						else cout << ". ";
					}
					cout << endl;
				}
				cout << "\n=========================================" << endl;
				cout << " [알림] : " << systemMessage << endl;
				cout << "=========================================" << endl;
			}
		}
		Sleep(50);
	}

	delete player;
	return 0;
}