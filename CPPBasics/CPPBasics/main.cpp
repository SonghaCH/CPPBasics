#include <iostream>
#include <cstdlib>     //랜덤함수마법사
#include <ctime>       //랜덤함수마법사

using namespace std;

int main()
{
    srand(time(NULL));    //랜덤함수
    int JackPot = (rand() % 20) + 1;    //랜덤함수

    int CountCoin = 10;
    int ChoiceNumber;
    int EnterButton;

    cout << "*반갑습니다. 행운의 뽑기 시간입니다.*" << endl;
    cout << "\n코인은 총 10개이며 10번안에 맞추지 못할시 게임오버 됩니다." << endl;
    cout << "\n1부터 20까지 숫자이며 정답은 단 1개입니다." << endl;


    for (;;)  //0또는 1번을 눌렀을때, 다시 시도하는 반복문
    {

        cout << "<진행하려면 (1)번을, 종료 하시려면 (0)번을 눌러주세요.>\n" << endl;


        cin >> EnterButton;
        if (EnterButton == 1)
        {
            cout << "게임을 시작하겠습니다!" << endl;
            break;
        }
        else if (EnterButton == 0)
        {
            cout << "종료 버튼을 누르셨습니다! \n게임이 종료 됩니다." << endl;
            exit;
        }

        else
        {
            cout << "\n잘못 입력하셨습니다. 1번 혹은 0번을 입력해주세요" << endl;
        }

    }

    cout << "1부터 20까지 입력하여 당첨을 찾아십시오." << endl;

    for (int i = 1; i <= CountCoin; i++)
    {
        for (; ; ) //1부터 20사이만 입력하도록 반복
        {

            cin >> ChoiceNumber;
            if (ChoiceNumber >= 1 && ChoiceNumber <= 20)  //정상 입력시 진행
            {
                break;
            }

            else
            {
                cout << "\n1부터 20사이만 입력해주세요.(코인은 차감되지않습니다.)" << endl;
            }
        }

        if (ChoiceNumber == JackPot)
        {
            cout << "\n7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7\n잭팟! 정답입니다! 상품은 주강사님의 *격려* 입니다\n축하드립니다!" << endl;
            cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
            return 0;
        }
        else
        {
            cout << "\n아쉽게도 정답이 아닙니다" << endl;
            cout << "남은 코인:  " << CountCoin - i << " 개 남았습니다. 상품이 매우 값진 보상이니 화이팅!" << endl;

            if (i == CountCoin)
            {
                cout << "\n\n\n\n\n모든 코인을 소진하셨습니다. \nG  a   m  e       O  v  e   r" << endl;
                cout << "\n\n\n\n<<아쉽게도 정답은 [" << JackPot << "] 이었습니다.>>" << endl;

            }
        }





    }


}






















