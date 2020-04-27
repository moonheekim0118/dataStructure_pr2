#include "List.h"
#include <string>
/* ��⸮��Ʈ(ť) */

waitingNode::waitingNode(string Name, waitingNode* next) { //������
	this->name = Name;
	this->next = next;
}

waitingList::waitingList() {//������ 
	front = NULL;
	rear = NULL;
}
waitingList::~waitingList() { //�Ҹ��� 
	Clear();
}
void waitingList::Enqueue(string client_name) { //��� ��ܿ� ����ֱ� 
	waitingNode* newNode = new waitingNode(client_name, NULL);

	if (isEmpty()) {
		front = newNode;
		rear = newNode;
	} //��Ұ� ó�����´ٸ� front�� �ش� ��Ҹ� ����������Ѵ�.
	else {
		rear->next = newNode;
		rear = newNode;
	}

}

string waitingList::getName()const { //��� ����Ʈ�� �ִ� ��� �� �̸� string���·� ��ȯ 
	string str = "";
	waitingNode* tmp = front;
	while (tmp != NULL)
	{
		str.append(tmp->name + "  ");
		tmp = tmp->next;
	}
	return str;
}


string waitingList::Dequeue() { //��⸮��Ʈ���� ���� (���ฮ��Ʈ�� ���� ����) (ť ���� ����)  
	waitingNode* tmp = front;
	string removed = front->name;
	if (front->next == NULL) front = NULL;
	else { front = front->next; } //front�� �� ��ҷ� �Ű��ش�.
	delete tmp;
	return removed;
}

void waitingList::Clear() { //����� ��� ���� 
	waitingNode* tmp = front;
	while (tmp != NULL) {
		tmp = tmp->next;
		delete front;
		front = tmp;
	}
}

void waitingList::remove(string name) //Ư�� �� ��� ��ܿ��� ���� , ���� ��� ��Ȳ�� �����ϴ� ��� 
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

bool waitingList::isEmpty() const  //����� empty���� Ȯ�� 
{

	if (front == NULL) return true;
	else return false;
}
