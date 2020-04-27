#include "List.h"
#include <string>
/* ���� ����Ʈ */

Reservation_client::Reservation_client(clientNode* client, flightNode* flight) //������ 
{
	this->client = client;
	this->flight = flight;
	this->reserved = false;
	this->next = NULL;
}

void Reservation_client::reservationComplete() //������ -> ����Ϸ� 
{
	this->reserved = true;
}
bool Reservation_client::status()const 
{
	return reserved; //��� - false ��ȯ
	                 //���� - true ��ȯ 
}


Reservation_list::Reservation_list() //������ 
{
	this->head = NULL;
}
Reservation_list::~Reservation_list() { //�Ҹ��� 
	clear();
}

void Reservation_list::insert_flight(Reservation_client* newNode) //�ش� ����⿡ ���� �� ���� 
{
	if (isEmpty()) { //���� ù ����� �ٷ� head�� ���ְ� �� 
		head = newNode;
		return;
	}

	Reservation_client* tmp = head;
	Reservation_client* tmp_before = NULL;
	while (tmp != NULL)
	{
		int order = tmp->client->Name.compare(newNode->client->Name); //������ �� 
		if (order == 0) {
			cout << "�ش� ���� �̹� ���� ����Ʈ�� �����մϴ�." << endl;
			return;
		}
		else if (order > 0) break;
		tmp_before = tmp;
		tmp = tmp->next;
	}

	if (tmp_before != NULL)
	{
		tmp_before->next = newNode;
	}
	if (tmp_before == NULL)
	{
		head = newNode;
	}
	newNode->next = tmp;
}

void Reservation_list::insert_client(Reservation_client* newNode, bool done) //���� ���ฮ��Ʈ�� ���� ���� ����� ��ȣ�� 
{
	if (done == false)
	{
		newNode->reserved = false; //��� ����
	}
	else
		newNode->reserved = true; //���� �Ϸ� 
	if (isEmpty()) {
		head = newNode;
		return;
	}
	Reservation_client* tmp = head;
	Reservation_client* tmp_before = NULL;
	while (tmp != NULL)
	{
		int order = tmp->flight->flightNumber.compare(newNode->flight->flightNumber); //������ �� 
		if (order == 0) {
			cout << "�ش� ���� �̹� ���� ����Ʈ�� �����մϴ�." << endl;
			return;
		}
		else if (order > 0) break;
		tmp_before = tmp;
		tmp = tmp->next;
	}

	if (tmp_before != NULL)
	{
		tmp_before->next = newNode;
	}
	if (tmp_before == NULL) {
		head = newNode;
	}
	newNode->next = tmp;

}

bool Reservation_list::remove_flight(string name) //������� ���ฮ��Ʈ���� Ư�� �� ���� 
{
	Reservation_client* data = retrieve_flight(name);
	if (data == NULL)
	{
		cout << "�ش� ���� ���� ������ ã�� �� �����ϴ�. " << endl;
		return false;
	}
	if (data == head)  //������ �����Ͱ� list�� head���, 
	{
		head = data->next;
		delete data;
		return true;
	}

	Reservation_client* tmp = head;
	while (tmp->next != data) {
		tmp = tmp->next;
	}
	tmp->next = data->next;
	delete data;
	return true;
}

void Reservation_list::remove_client(string flight_num) //���� ���ฮ��Ʈ���� Ư�� ����� ���� ���� 
{
	Reservation_client* data = retrieve_client(flight_num);
	if (data == NULL)
	{
		return;
	}
	if (data == head)  //������ �����Ͱ� list�� head���, 
	{
		head = data->next;
		delete data;
		return;
	}

	Reservation_client* tmp = head;
	while (tmp->next != data) {
		tmp = tmp->next;
	}
	tmp->next = data->next;
	delete data;
}


Reservation_client* Reservation_list::retrieve_flight(string name)const {  //������� ���ฮ��Ʈ���� Ư���� ã�� 
	Reservation_client* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->client->Name == name) return tmp;
		tmp = tmp->next;
	}
	return NULL; //��ã�� 
}

Reservation_client* Reservation_list::retrieve_client(string flightNumber)const { //���� ���ฮ��Ʈ���� Ư�� ����� ã�� 
	Reservation_client* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->flight->flightNumber == flightNumber) return tmp;
		tmp = tmp->next;
	}
	return NULL; //��ã�� 
}

//�� ����Ⱑ ��°�� ������ �� ���� ���ฮ��Ʈ�� ���� ����ϴ� �����Լ�
void Reservation_list::removeAllinClientReservation(string flightNumber) //���� reservation list���� Ư��flightNumber ������Ȳ ��� ���� 
{
	Reservation_client* tmp = head;
	while (tmp != NULL)
	{
		clientNode* client = tmp->client;
		client->head->remove_client(flightNumber); //�ش� Ŭ���̾�Ʈ�κ��� �ش� ����� ������Ȳ ���� 
		tmp = tmp->next;
	}
}

//�� ���� ��°�� ������ �� ������� ���ฮ��Ʈ�� ���� ����ϴ� �����Լ� 
void Reservation_list::removeAllinFlightReservation(string name, ClientList& clientlist) //�ش� flgiht -> reservation list���� name ������Ȳ ��� ���� 
{
	Reservation_client* tmp = head;
	while (tmp != NULL)
	{
		flightNode* flight = tmp->flight;

		if (tmp->status()) { //���� �Ϸ� ���¶�� 
			flight->head->remove_flight(name); //�ش� ����� ���� name�� ���� ���� �������� ����
			if (!flight->queue->isEmpty()) //�������� �����ϰ�, ��� �ο��� ���� �� ����ο��� ���ฮ��Ʈ�� �Ű��ش�.
			{
				string newName = flight->queue->Dequeue();
				clientNode* waiting = clientlist.retrieve(newName);//�ϼ��� ������� clientNode �������� 
				Reservation_client* newNode = new Reservation_client(waiting, flight);
				flight->head->insert_flight(newNode);
				Reservation_client* tmp = waiting->head->retrieve_client(flight->flightNumber);
				//��� ����� ���� ���� ����Ʈ�� ���� (���)�� (����Ϸ�)�� �ٲپ��ش�.
				tmp->reservationComplete();
			}
			else { //����ο��� ���� �� , �����ο��� -1 ���ش�. 
				flight->cancle_reservation();
			}
		}
		else  //��� ���¶�� 
		{
			flight->queue->remove(name); //�ش� ����⿡�� name�� ���� ���� �����Ȳ�� ���� 
		}
		tmp = tmp->next;
	}

}



bool Reservation_list::duplicate(string name)const //flight ��� �������� �ߺ� Ȯ��  
{
	Reservation_client* tmp = head;
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
	Reservation_client* tmp = head;
	while (tmp != NULL)
	{
		head = tmp;
		tmp = tmp->next;
		delete head;
	}
	head = NULL;
}



string Reservation_list::getName()const //���ฮ��Ʈ�� ����� ��� �̸� string ���·� ��ȯ  
{
	string str = "";
	Reservation_client* tmp = head;
	while (tmp != NULL)
	{
		str.append(tmp->client->Name + "  ");
		tmp = tmp->next;
	}
	return str;
}


string Reservation_list::getNumbers_reserved()const {//���ฮ��Ʈ�� ����� '����Ϸ��' ��� ����� ��ȣ string ���·� ��ȯ
	string str = "";
	Reservation_client* tmp = head;
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

string Reservation_list::getNumbers_waiting() const //���ฮ��Ʈ�� ����� �������� ��� ����� ��ȣ string���·� ��ȯ
{
	string str = "";
	Reservation_client* tmp = head;
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
