#include "List.h";
#include <string>
#include <fstream>
/* ������Ʈ */
clientNode::clientNode(string name) { //������
	this->Name = name;
	this->prior = this->next = NULL;
	this->head = new Reservation_list();
}

clientNode::~clientNode() //�Ҹ��� 
{
	delete this->head; //clientNode ������ head������ (���ฮ��Ʈ) �Ҹ� 
}


ClientList::ClientList() { //������ 
	this->head = NULL;
	this->cursor = NULL;
}

ClientList::~ClientList() { //�Ҹ��� 
	clear();
	
}
void  ClientList::insert(clientNode* newNode) {
	if (isEmpty()) { //ù ����ΰ�� 
		head = newNode; //��尡 ����Ű���� 
	}
	else { //ù ��尡 �ƴ� ��� 
		newNode->prior = cursor; //newNode�� prior�� Ŀ���� ����Ű���� 
		newNode->next = cursor->next; //newNode�� next�� Ŀ�� ���� ���� ����Ű���� 
		cursor->next = newNode; //Ŀ�� next�� newNode�� ����Ű���� 
	}
	cursor = newNode; //Ŀ�� ���� 
}

bool ClientList::isEmpty()const {
	if (head == NULL) return true;
	else return false;
}

void ClientList::clear() {
	clientNode* tmp = head;
	while (tmp != NULL)
	{
		head = tmp;
		tmp = tmp->next;
		delete head;
	}
	cursor = NULL;
}

void ClientList::remove(string name, ClientList& clientlist) //�ش� �� ���� 
{
	if (isEmpty())
	{
		cout << "list is empty!" << endl;
		return;
	}
	clientNode* data = retrieve(name);
	if (data == NULL)
	{
		cout << "����� ������ �߸� �Ǿ����ϴ�." << endl; //����ó�� 
		return;
	}

	data->head->removeAllinFlightReservation(name, clientlist); //�ش� ���� �����ϰų� ��⸦ �ɾ���� ����Ʈ ���� 


	if (data == head) { //������ ��尡 head�� ��� 
		head = data->next; //head ����  
		if (data->next != NULL) { //next�� null�� �ƴ϶��  
			data->next->prior = NULL; //next�� prior�� null������ ���� (head�� �����ǹǷ�)
		}
		delete data;
		return;

	}
	data->prior->next = data->next;
	if (data->next != NULL) { //next�� null���� �ƴ϶�� 
		data->next->prior = data->prior;
	}
	delete data;

}
clientNode* ClientList::retrieve(string name)const //Ư�� �̸��� �� ã�Ƽ� node���·� ��ȯ�ϱ� 
{
	clientNode* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->Name == name) return tmp;
		tmp = tmp->next;
	}
	return NULL;
}


bool ClientList::PP(string name)const { //�μ��Լ� 
	clientNode* tmp = retrieve(name);
	if (tmp == NULL) return false;
	ofstream fout;
	fout.open("OUTFILE.P1.txt", ios_base::out | ios_base::app);
	fout << name << " ������ ����� ���� ���� :  ";
	fout << tmp->head->getNumbers_reserved() << endl;
	fout << name << " ������ ����� ��� ���� :";
	fout << tmp->head->getNumbers_waiting() << endl;
	fout << endl;
	fout << "==========================================" << endl;
	fout.close();
}
