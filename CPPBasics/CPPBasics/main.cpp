#include <iostream>
#include <string>
using namespace std;

int liveMap[7][7] = { 0 };  //실시간 맵을 위한 전역 변수 모든 맵을 0(빈칸)으로 채우기

int main() {

    for (;;) //게임 오버시에는 다시 돌아와 클리어 할 수 있게 무한 루프
    {
        // 7행 7열 보물 맵 구현 (총 49칸)
        // 아이템 ID: 0(빈칸), 1(보물), 2(몬스터)
        int treasure[7][7] =
        {
            {0, 2, 0, 0, 2, 0, 1},
            {0, 0, 0, 0, 2, 0, 0},
            {0, 2, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1},
            {2, 0, 2, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 2, 0},
            {0, 2, 0, 2, 0, 0, 1}

        };

        int pX = 0;             //플레이어 위치가 0,0에서 시작을 의미
        int pY = 0;
        int findTreasure = 0;   // 보물을 찾은 개수 0부터 시작

        bool isGameover = false; //AI왈 for문을 진행하다가 게임 오버가 되었을때, 재시작을 위한 컴퓨터에게 메모지 역할

        cout << "\n[트레저 헌터 게임을 시작합니다]\n" << endl;
        cout << "\n[   조작법 -      1번 왼쪽,  2번 윗쪽,  3번 아래쪽,  4번 오른쪽    ]\n" << endl;

        for (; findTreasure < 3;)   //AI왈 보물 3개를 다 찾을 때까지 플레이해야함.
        {

            system("cls");   //게임 빌드 화면이 너무 더러워져서 바로바로 로그만 확인하고 사라지는 명령어 AI 300퍼센트 참고

            cout << "\n[트레저 헌터 게임 진행 중]" << endl;
            cout << "[조작법 - 1:왼쪽, 2:위쪽, 3:아래쪽, 4:오른쪽]" << endl;
            cout << "------------------------------------------" << endl;

            cout << "\n----------------" << endl;
            for (int i = 0; i < 7; i++)       //AI 100퍼센트 지분 실시간 지도 출력을 위해서 이중 for문을 사용함.
            {
                for (int r = 0; r < 7; r++)
                {
                    if (i == pY && r == pX)
                    {
                        cout << "P ";        // 현재 플레이어 위치
                    }
                    else if (liveMap[i][r] == 1)
                    {
                        cout << "O ";  //실시간 지도에 보물(1) 위치 표기
                    }
                    else if (liveMap[i][r] == 2)
                    {
                        cout << "X "; //실시간 지도에 괴물(2) 위치 표기
                    }
                    else
                    {
                        cout << ". ";   //실시간 지도에 빈칸(0) 표기
                    }

                }
                cout << endl;      //매우 중요! 실시간 지도 만드는데 표가 1자로 내려가지 않은 이유 고맙다 AI..
            }




            cout << "\n\n\n\n트레저 헌터 출동!   " << findTreasure << "/3 만큼 보물 획득했습니다.  |  현재위치: (" << pX << "." << pY << ")" << endl; //게임 시작선언과 보물 3가지중 n개 찾았다는 표기, 그리고 현재 위치는 x y 좌표 표기

            int move;                                      //이동방향 유저가 입력 
            cout << "\n어느 쪽으로 이동할까요? : ";
            cin >> move;

            int nextX = pX;            //플레이어의 실질적 좌표 선언
            int nextY = pY;

            if (move == 1) nextX--;          //방향 번호 입력 어차피 왼쪽 오른쪽은 nextX에서 증감, 위 아래는 nextY에서 증감
            else if (move == 2) nextY--;
            else if (move == 3) nextY++;
            else if (move == 4) nextX++;
            else
            {
                cout << "잘못 입력하셨습니다. 1,2,3,4 번 중에서 선택하여 이동하여 주시기 바랍니다." << endl;
                system("pause");
                continue;
            }

            if (nextX < 0 || nextX > 6 || nextY < 0 || nextY > 6)      //7X7 맵이기 때문에 밖으로 나가는것을 방지
            {

                cout << "벽에 막혔습니다. 다른 방향으로 이동하여 주시기 바랍니다." << endl;
                system("pause"); // 로그만 사라질 시간 확보하기 사라지는 명령어 AI 300퍼센트 참고
                continue;


            }

            pX = nextX;       //AI 피셜 안전함이 확인되면 좌표 확정 (벽에 부딪히거나 옳지 않은 선택지를 고르는것을 뜻하는것으로 보임)
            pY = nextY;

            if (treasure[pY][pX] == 1)
            {
                cout << "!!!!!!!!!!!!!보물을 발견 하셨군요!!!!!!!!!!!!!!" << endl;
                system("pause");
                liveMap[pY][pX] = 1;       //실시간 지도에 보물 위치 기록
                treasure[pY][pX] = 0;     //한번 발견했으니 정답지에서 제거 합니다.보물(1)을 빈칸(0) 으로 변경
                findTreasure++; //  n/3 개로 3개중에 1개 찾았으니 증가 시켜줍니다 
            }
            else if (treasure[pY][pX] == 2)
            {
                cout << "XXXXXXXXX 매복해있던 몬스터에게 당했습니다! 걱정마세요! 다시 시작합니다. XXXXXXXXXXXXX" << endl;
                liveMap[pY][pX] = 2;   //실시간 위치에 몬스터 위치 기록

                system("pause");
                isGameover = true;          //처음에 선언했던 isGameover = false 가 조건에 의해 게임오버 true 로 변경됩니다.
                break;
            }
            else
            {
                cout << "이 곳은 안전하군요. 위험이 도사리고 있습니다. 조심하세요! " << endl;
                system("pause");
            }


        }
        if (isGameover == false)
        {
            cout << "\n\n축하드립니다! 모든 보물을 찾으셨습니다. 당신은 진정한 트레져헌터!\n\n" << endl;
            system("pause");
            break;
        }

    }
    return 0;
}




