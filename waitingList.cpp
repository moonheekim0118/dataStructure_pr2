#include "List.h"
#include <string>
/* 대기리스트(큐) */

waitingNode::waitingNode(string Name, waitingNode* next) { //생성자
	this->name = Name;
	this->next = next;
}

waitingList::waitingList() {//생성자 
	front = NULL;
	rear = NULL;
}
waitingList::~waitingList() { //소멸자 
	Clear();
}
void waitingList::Enqueue(string client_name) { //대기 명단예 집어넣기 
	waitingNode* newNode = new waitingNode(client_name, NULL);

	if (isEmpty()) {
		front = newNode;
		rear = newNode;
	} //요소가 처음들어온다면 front도 해당 요소를 가리켜줘야한다.
	else {
		rear->next = newNode;
		rear = newNode;
	}

}

string waitingList::getName()const { //대기 리스트에 있는 모든 고객 이름 string형태로 반환 
	string str = "";
	waitingNode* tmp = front;
	while (tmp != NULL)
	{
		str.append(tmp->name + "  ");
		tmp = tmp->next;
	}
	return str;
}


string waitingList::Dequeue() { //대기리스트에서 삭제 (예약리스트로 가기 위해) (큐 들어온 순서)  
	waitingNode* tmp = front;
	string removed = front->name;
	if (front->next == NULL) front = NULL;
	else { front = front->next; } //front를 뒷 요소로 옮겨준다.
	delete tmp;
	return removed;
}

void waitingList::Clear() { //대기명단 모두 삭제 
	waitingNode* tmp = front;
	while (tmp != NULL) {
		tmp = tmp->next;
		delete front;
		front = tmp;
	}
}

void waitingList::remove(string name) //특정 고객 대기 명단에서 삭제 , 고객이 모든 현황을 삭제하는 경우 
{
	waitingNode* tmp = front;
	waitingNode* tmpBefore = NULL;
	while (tmp != NULL)
	{
		if (tmp->name == name)
		{
			if (tmpBefore != NULL) {
				tmpBefore->next = tmp->next;
				if (tmp == rear)
				{
					rear = tmpBefore;
				}
			}
			if (tmp == front)
			{
				front = tmp->next;
			}
			delete tmp;
			return;
		}
		tmpBefore = tmp;
		tmp = tmp->next;
	}
}

bool waitingList::isEmpty() const  //대기명단 empty인지 확인 
{

	if (front == NULL) return true;
	else return false;
}
