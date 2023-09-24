#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>

int main() {
    std::string inputBuffer; // 입력을 저장할 버퍼

    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // ESC 키를 누르면 종료
            break;
        }

        char key = _getch(); // 키 입력을 받음

        if (key == '`') { // 백틱 (\`) 키를 누르면 입력을 초기화
            inputBuffer.clear();
            std::cout << "입력 초기화" << std::endl;
        }
        else {
            // 입력된 키 값을 화면에 출력
            std::cout << "입력: " << key << std::endl;
            inputBuffer += key; // 입력을 버퍼에 추가

            // 입력 버퍼가 목표 시퀀스와 일치하는지 확인
            // 방향기가 화살표가 아닌 wasd..
            if (inputBuffer == "sdk") {
                std::cout << "아도겐! =o" << std::endl;
                inputBuffer.clear(); // 버퍼 초기화
            }
            else if (inputBuffer == "swl") {
                std::cout << "파동권 !" << std::endl;
                inputBuffer.clear(); // 버퍼 초기화
            }
            else if (inputBuffer == "dio") {
                std::cout << "도동파~~ " << std::endl;
                inputBuffer.clear(); // 버퍼 초기화
            }
        }
    }

    return 0;
}