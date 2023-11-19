#include <iostream>
#include "stack.h"

y_stack::y_stack() : topNode(nullptr), size(0), maxSize(5) {}

y_stack::~y_stack() {
    while (!isEmpty()) {
        pop();
    }
}

void y_stack::push(int value) {
    if (size < maxSize) {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        ++size;
    } else {
        // 스택이 가득 찬 경우 처리 (예: 오류 처리)
        // 여기에 필요한 코드를 추가하세요.
    }
}

void y_stack::pop() {
    if (!isEmpty()) {
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }
}

int y_stack::top() const {
    if (!isEmpty()) {
        return topNode->data;
    }
}

bool y_stack::isEmpty() const {
    return topNode == nullptr;
}

int main() {
    y_stack myStack;

    // 스택에 값을 추가하고 상태 출력
    myStack.push(1);
    std::cout << "push: ";
    // 스택의 현재 상태 출력
    std::cout << myStack.top() << " ";
    std::cout << std::endl;

    myStack.push(2);
    std::cout << "push: ";
    std::cout << myStack.top() << " ";
    std::cout << std::endl;

    myStack.push(3);
    std::cout << "push: ";
    std::cout << myStack.top() << " ";
    std::cout << std::endl;

    myStack.push(4);
    std::cout << "push: ";
    std::cout << myStack.top() << " ";
    std::cout << std::endl;

    // 스택에서 값을 제거하고 상태 출력
    myStack.pop();
    std::cout << "pop: ";
    std::cout << myStack.top() << " ";
    std::cout << std::endl;
    
    myStack.pop();
    std::cout << "pop: ";
    std::cout << myStack.top() << " ";
    std::cout << std::endl;

    // 스택이 비어 있는지 확인하고 상태 출력
    if (myStack.isEmpty()) {
        std::cout << "empty." << std::endl;
    }
    else {
        std::cout << "not empty." << std::endl;
    }

    return 0;
}
