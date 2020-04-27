#include "List.h";
#include <string>
#include <fstream>
/* 고객리스트 */
clientNode::clientNode(string name) { //생성자
	this->Name = name;
	this->prior = this->next = NULL;
	this->head = new Reservation_list();
}

clientNode::~clientNode() //소멸자 
{
	delete this->head; //clientNode 내부의 head포인터 (예약리스트) 소멸 
}


ClientList::ClientList() { //생성자 
	this->head = NULL;
	this->cursor = NULL;
}

ClientList::~ClientList() { //소멸자 
	clear();
	
}
void  ClientList::insert(clientNode* newNode) {
	if (isEmpty()) { //첫 노드인경우 
		head = newNode; //헤드가 가리키도록 
	}
	else { //첫 노드가 아닌 경우 
		newNode->prior = cursor; //newNode의 prior가 커서를 가리키도록 
		newNode->next = cursor->next; //newNode의 next가 커서 다음 값을 가리키도록 
		cursor->next = newNode; //커서 next가 newNode를 가리키도록 
	}
	cursor = newNode; //커서 변경 
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

void ClientList::remove(string name, ClientList& clientlist) //해당 고객 삭제 
{
	if (isEmpty())
	{
		cout << "list is empty!" << endl;
		return;
	}
	clientNode* data = retrieve(name);
	if (data == NULL)
	{
		cout << "비행기 정보가 잘못 되었습니다." << endl; //오류처리 
		return;
	}

	data->head->removeAllinFlightReservation(name, clientlist); //해당 고객이 예약하거나 대기를 걸어놓은 리스트 삭제 


	if (data == head) { //삭제할 노드가 head인 경우 
		head = data->next; //head 변경  
		if (data->next != NULL) { //next가 null이 아니라면  
			data->next->prior = NULL; //next의 prior를 null값으로 변경 (head가 삭제되므로)
		}
		delete data;
		return;

	}
	data->prior->next = data->next;
	if (data->next != NULL) { //next가 null값이 아니라면 
		data->next->prior = data->prior;
	}
	delete data;

}
clientNode* ClientList::retrieve(string name)const //특정 이름의 고객 찾아서 node형태로 반환하기 
{
	clientNode* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->Name == name) return tmp;
		tmp = tmp->next;
	}
	return NULL;
}


bool ClientList::PP(string name)const { //인쇄함수 
	clientNode* tmp = retrieve(name);
	if (tmp == NULL) return false;
	ofstream fout;
	fout.open("OUTFILE.P1.txt", ios_base::out | ios_base::app);
	fout << name << " 고객님의 비행기 예약 정보 :  ";
	fout << tmp->head->getNumbers_reserved() << endl;
	fout << name << " 고객님의 비행기 대기 정보 :";
	fout << tmp->head->getNumbers_waiting() << endl;
	fout << endl;
	fout << "==========================================" << endl;
	fout.close();
}
