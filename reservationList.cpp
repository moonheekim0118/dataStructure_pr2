#include "List.h"
#include <string>
/* 예약 리스트 */

Reservation_node::Reservation_node(clientNode* client, flightNode* flight) //생성자 
{
	this->client = client;
	this->flight = flight;
	this->reserved = false;
	this->prior = NULL; 
	this->next = NULL;
}

void Reservation_node::reservationComplete() //대기상태 -> 예약완료 
{
	this->reserved = true;
}
bool Reservation_node::status()const 
{
	return reserved; //대기 - false 반환
	                 //예약 - true 반환 
}


Reservation_list::Reservation_list() //생성자 
{
	this->head = NULL;
	this->cursor = NULL;
}
Reservation_list::~Reservation_list() { //소멸자 
	clear();
}

void Reservation_list::insert_flight(Reservation_node* newNode) //해당 비행기에 예약 고객 삽입 
{


	if (isEmpty()) { //가장 첫 노드라면 바로 head로 해주고 끝 
		head = newNode;
		return;
	}

	Reservation_node* tmp = head;
	Reservation_node* tmp_before = NULL;
	while (tmp != NULL)
	{
		int order = tmp->client->Name.compare(newNode->client->Name); //사전순 비교 
		if (order == 0) {
			cout << "해당 고객은 이미 예약 리스트에 존재합니다." << endl;
			return;
		}
		else if (order > 0) break;
		tmp_before = tmp;
		tmp = tmp->next;
	}
	newNode->prior= tmp_before;
	newNode->next= tmp;

	if (tmp_before != NULL)
	{
		tmp_before->next = newNode;
	}
	if (tmp_before == NULL)
	{
		head = newNode;
	}
	if (tmp != NULL) {
		tmp->prior= newNode;
	}

}

void Reservation_list::insert_client(Reservation_node* newNode, bool done) //고객별 예약리스트에 예약 삽입 비행기 번호순 
{
	if (done == false)
	{
		newNode->reserved = false; //대기 상태
	}
	else
		newNode->reserved = true; //예약 완료 
	if (isEmpty()) {
		head = newNode;
		return;
	}
	Reservation_node* tmp = head;
	Reservation_node* tmp_before = NULL;
	while (tmp != NULL)
	{
		int order = tmp->flight->flightNumber.compare(newNode->flight->flightNumber); //사전순 비교 
		if (order == 0) {
			cout << "해당 고객은 이미 예약 리스트에 존재합니다." << endl;
			return;
		}
		else if (order > 0) break;
		tmp_before = tmp;
		tmp = tmp->next;
	}
	newNode->prior = tmp_before;
	newNode->next = tmp;
	if (tmp_before != NULL)
	{
		tmp_before->next = newNode;
	}
	if (tmp_before == NULL) {
		head = newNode;
	}
	if (tmp != NULL) {
		tmp->prior = newNode;
	}
}

bool Reservation_list::remove_flight(string name) //비행기의 예약리스트에서 특정 고객 삭제 
{
	if (isEmpty()) {
		cout << "list is empty!";
		return false;
	}
	Reservation_node* data = retrieve_flight(name);
	if (data == NULL)
	{
		cout << "해당 고객의 예약 정보를 찾을 수 없습니다. " << endl;
		return false;
	}
	if (data == head)  //삭제할 데이터가 list의 head라면, 
	{
		head = data->next;
		if (data->next!= NULL) { //next가 null이 아니라면  
			data->next->prior= NULL; //next의 prior를 null값으로 변경 (head가 삭제되므로)
		}

		delete data;
		return true;
	}

	data->prior->next = data->next; 
	if (data->next!= NULL) { //next가 null값이 아니라면 
		data->next->prior= data->prior;
	}
	cout << "완료" << endl;
	delete data;
	return true;
}

void Reservation_list::remove_client(string flight_num) //고객의 예약리스트에서 특정 비행기 예약 삭제 
{
	if (isEmpty()) {
		cout << "list is empty!";
		return;
	}
	Reservation_node* data = retrieve_client(flight_num);
	if (data == NULL)
	{
		return;
	}
	if (data == head)  //삭제할 데이터가 list의 head라면, 
	{
		head = data->next;
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


Reservation_node* Reservation_list::retrieve_flight(string name)const {  //비행기의 예약리스트에서 특정고객 찾기 
	Reservation_node* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->client->Name == name) return tmp;
		tmp = tmp->next;
	}
	return NULL; //못찾음 
}

Reservation_node* Reservation_list::retrieve_client(string flightNumber)const { //고객의 예약리스트에서 특정 비행기 찾기 
	Reservation_node* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->flight->flightNumber == flightNumber) return tmp;
		tmp = tmp->next;
	}
	return NULL; //못찾음 
}

//한 비행기가 통째로 삭제될 때 고객의 예약리스트에 대해 사용하는 삭제함수
void Reservation_list::removeAllinClientReservation(string flightNumber) //고객의 reservation list에서 특정flightNumber 예약현황 모두 삭제 
{
	Reservation_node* tmp = head;
	while (tmp != NULL)
	{
		clientNode* client = tmp->client;
		client->head->remove_client(flightNumber); //해당 클라이언트로부터 해당 비행기 예약현황 삭제 
		tmp = tmp->next;
	}
}

//한 고객이 통째로 삭제될 때 비행기의 예약리스트에 대해 사용하는 삭제함수 
void Reservation_list::removeAllinFlightReservation(string name, ClientList& clientlist) //해당 flgiht -> reservation list에서 name 예약현황 모두 삭제 
{
	Reservation_node* tmp = head;
	while (tmp != NULL)
	{
		flightNode* flight = tmp->flight;

		if (tmp->status()) { //예약 완료 상태라면 
			flight->head->remove_flight(name); //해당 비행기 에서 name을 가진 고객의 예약정보 삭제
			if (!flight->queue->isEmpty()) //예약정보 삭제하고, 대기 인원이 있을 때 대기인원을 예약리스트로 옮겨준다.
			{
				string newName = flight->queue->Dequeue();
				clientNode* waiting = clientlist.retrieve(newName);//일순위 대기자의 clientNode 가져오기 
				Reservation_node* newNode = new Reservation_node(waiting, flight);
				flight->head->insert_flight(newNode);
				Reservation_node* tmp = waiting->head->retrieve_client(flight->flightNumber);
				//방금 예약된 고객의 예약 리스트로 가서 (대기)를 (예약완료)로 바꾸어준다.
				tmp->reservationComplete();
			}
			else { //대기인원이 없을 때 , 예약인원을 -1 해준다. 
				flight->cancle_reservation();
			}
		}
		else  //대기 상태라면 
		{
			flight->queue->remove(name); //해당 비행기에서 name을 가진 고객의 대기현황도 삭제 
		}
		tmp = tmp->next;
	}

}



bool Reservation_list::duplicate(string name)const //flight 노드 기준으로 중복 확인  
{
	Reservation_node* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->client->Name == name) { return true; }
		tmp = tmp->next;
	}
	return false;
}

bool Reservation_list::isEmpty()const
{
	if (head == NULL) return true;
	else return false;
}

void Reservation_list::clear()
{
	Reservation_node* tmp = head;
	while (tmp != NULL)
	{
		head = tmp;
		tmp = tmp->next;
		delete head;
	}
	head = NULL;
}



string Reservation_list::getName()const //예약리스트에 저장된 모든 이름 string 형태로 반환  
{
	string str = "";
	Reservation_node* tmp = head;
	while (tmp != NULL)
	{
		str.append(tmp->client->Name + "  ");
		tmp = tmp->next;
	}
	return str;
}


string Reservation_list::getNumbers_reserved()const {//예약리스트에 저장된 '예약완료된' 모든 비행기 번호 string 형태로 반환
	string str = "";
	Reservation_node* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->reserved == true)
		{
			str.append(tmp->flight->flightNumber + " ");
		}
		tmp = tmp->next;
	}
	return str;
}

string Reservation_list::getNumbers_waiting() const //예약리스트에 저장된 대기상태의 모든 비행기 번호 string형태로 반환
{
	string str = "";
	Reservation_node* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->reserved == false)
		{
			str.append(tmp->flight->flightNumber + " ");
		}
		tmp = tmp->next;
	}
	return str;
}
