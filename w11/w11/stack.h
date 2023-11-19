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

	//���
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
	int size;  // ������ ũ�⸦ �����ϴ� ���� �߰�
	const int maxSize;  // �ִ� ���� ũ�⸦ �����ϴ� ���� �߰�
};
