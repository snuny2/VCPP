#pragma once

//class y_stack {
	//public:
	//	y_stack();
	//	~y_stack();
	//
	//	void push();
	//	void pop();
	//
	//private:
	//	int* date;
	//};

	//노드
	class Node {
public:
	int data;
	Node* next;

	Node(int value) : data(value), next(nullptr) {}
};

class y_stack {
public:
	y_stack();
	~y_stack();

	void push(int value);
	void pop();
	int top() const;
	bool isEmpty() const;

private:
	Node* topNode;
	int size;  // 스택의 크기를 저장하는 변수 추가
	const int maxSize;  // 최대 스택 크기를 저장하는 변수 추가
};
