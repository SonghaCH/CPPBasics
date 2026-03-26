#include <iostream>
#include <string>

bool isOpenRamen(int RamenCount)
{
    bool StartCook = (RamenCount == 1) ? true : false;
    std::cout << "라면 개수 1개 확인 완료. 라면 요리 시작하겠습니다" << StartCook << std::endl;
    return StartCook;
}

int CookRamen()
{

    std::cout << "면과 스프를 넣고 보글보글 끓입니다." << std::endl;
    int CookTime = 180;
    return CookTime;

}

void EatRamen(int CookTime)
{
    std::cout << CookTime << " 초 동안 맛있게 끓였습니다! 맛있게 드십쇼!" << std::endl;
}




int main()
{
    bool myRamen = isOpenRamen(1);
    int time = CookRamen();
    EatRamen(time);
    return 0;
}