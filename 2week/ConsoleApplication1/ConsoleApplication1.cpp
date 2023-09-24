<<<<<<< HEAD
﻿#include <iostream>
#include <conio.h>
#include <vector>

using namespace std;

int main() {
    char input = '\0';
    bool running = true;
    vector<string> inputHistory;
    vector<string> outputHistory;

    cout << " 김선호_202307075 의 어렵게 만들고 고친 VC++!...........\n" << endl;
    cout << "command>";

    while (running) {
        char command = _getch(); // 키 입력 받기

        if (command == 27) { // ESC 키
            running = false;
        }
        else if (isalpha(command)) { // 알파벳만 입력 허용
            input = command;
            cout << input; // 입력한 문자 표시
        }
        else if (command == '\r') { // 엔터 키
            switch (input) {
            case 'q':
                cout << "\n만드는건 chat gpt를 썼지만 여러가지 문제가 " << endl;
                break;
            case 'w':
                cout << "\n발생하여 chat gpt로 다시 여러번 고치면서 " << endl;
                break;
            case 'e':
                cout << "\n겨우겨우 수정한 과제..." << endl;
                break;
            case 'r':
                cout << "\n어... chat gpt처음 써봤는데 편하네요.." << endl;
                break;
            default:
                cout << "\n오류: 다른 글자가 입력되었습니다아.." << endl;
                break;
            }

            inputHistory.push_back(string(1, input)); // 입력한 커맨드 기록
            outputHistory.push_back("Output: " + string(1, input)); // 출력 메시지 기록
            input = '\0'; // 입력 초기화
            cout << "command>"; // 다음 입력을 위한 프롬프트 출력
        }
    }

    return 0;
}
=======
﻿#include <iostream>
#include <conio.h>
#include <vector>

using namespace std;

int main() {
    char input = '\0';
    bool running = true;
    vector<string> inputHistory;
    vector<string> outputHistory;

    cout << " 김선호_202307075 의 어렵게 만들고 고친 VC++!..........\n" << endl;
    cout << "command>";

    while (running) {
        char command = _getch(); // 키 입력 받기

        if (command == 27) { // ESC 키
            running = false;
        }
        else if (isalpha(command)) { // 알파벳만 입력 허용
            input = command;
            cout << input; // 입력한 문자 표시
        }
        else if (command == '\r') { // 엔터 키
            switch (input) {
            case 'q':
                cout << "\n만드는건 chat gpt를 썼지만 여러가지 문제가 " << endl;
                break;
            case 'w':
                cout << "\n발생하여 chat gpt로 다시 여러번 고치면서 " << endl;
                break;
            case 'e':
                cout << "\n겨우겨우 수정한 과제..." << endl;
                break;
            case 'r':
                cout << "\n어... chat gpt처음 써봤는데 편하네요.." << endl;
                break;
            default:
                cout << "\n오류: 다른 글자가 입력되었습니다아.." << endl;
                break;
            }

            inputHistory.push_back(string(1, input)); // 입력한 커맨드 기록
            outputHistory.push_back("Output: " + string(1, input)); // 출력 메시지 기록
            input = '\0'; // 입력 초기화
            cout << "command>"; // 다음 입력을 위한 프롬프트 출력
        }
    }

    return 0;
}
>>>>>>> 8d7f5f18459d1c4a1bfe08759f9dbdff061668e1
