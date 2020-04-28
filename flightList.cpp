#include "List.h"
#include <string>
#include <fstream>
/*����� ����Ʈ */

flightNode::flightNode(string flightNumber, string from, string to, string capacity, int capacity_int) //������ 
{
	this->flightNumber = flightNumber;
	this->from = from;
	this->to = to;
	this->capacity = capacity;
	this->capacity_int = capacity_int;
	this->reserved_num = 0;
	this->next_flight = NULL;
	this->prior_flight = NULL;
	this->head = new Reservation_list();
	this->queue = new waitingList();
}

flightNode::~flightNode()  //�Ҹ��� 
{
	delete this->head;   //���ο� �ִ� ����Ʈ�� ���� �����͵� ��� �Ҹ� 
	delete this->queue; 
}

void flightNode::cancle_reservation() { //���� ��� (����ο��� ���� ��)
	reserved_num--;
}

bool flightNode::reservation_statusUpdate() //���� �� ���� ���� ���� �Ǵ� 
{
	if (reserved_num + 1 <= capacity_int)
	{
		reserved_num++;
		return true;
	}
	else return false;
}


Flight_list::Flight_list() { //������  
	this->head = NULL;
}

Flight_list::~Flight_list() { //�Ҹ��� 
	clear();
}

void Flight_list::clear() {
	flightNode* tmp = head;
	while (tmp != NULL)
	{
		head = tmp;
		tmp = tmp->next_flight;
		delete head;
	}
}


void Flight_list::insert(flightNode* newNode) //������ �߰�  //��ȣ�� ����
{
	if (isEmpty()) { //ù ����ΰ�� 
		head = newNode; //��尡 ����Ű���� 
	}
	else { //ù ��尡 �ƴ� ��� 
		flightNode* tmp = head;
		flightNode* tmpBefore = NULL;
		while (tmp != NULL)
		{
			int order = tmp->flightNumber.compare(newNode->flightNumber);
			if (order == 0) {
				cout << "�ش� �������� �̹� �����մϴ�" << endl; //����ó�� 
				return;
			}
			else if (order > 0) break;
			tmpBefore = tmp;
			tmp = tmp->next_flight;

		}

		//tmpBefore�� tmp ���̿� �ִ´�.
		newNode->prior_flight = tmpBefore;
		newNode->next_flight = tmp;
		if (tmpBefore != NULL)
		{
			tmpBefore->next_flight = newNode;
		}
		if (tmpBefore == NULL) {
			head = newNode;
		}
		if (tmp != NULL) {
			tmp->prior_flight = newNode;
		}
	}
}


void Flight_list::remove(string flight_num)  //������ ����
{
	if (isEmpty())
	{
		cout << "list is empty!" << endl;
		return;
	}

	flightNode* data = retrieve(flight_num); //�ش� ������ ã�� 

	if (data == NULL)
	{
		cout << "����� ������ �߸� �Ǿ����ϴ�." << endl; //���� ó�� 
		return;
	}

	data->head->removeAllinClientReservation(data->flightNumber);
	//�ش� data�� reservation list �湮�Ѵ�.
	// reservation list�� �ִ� ���� clienet���� ���ฮ��Ʈ���� �ش� ����⿡ ���� ��� ��Ȳ�� �����Ѵ�.


	if (data == head) { //������ ��尡 head�� ��� 
		head = data->next_flight; //head ����  
		if (data->next_flight != NULL) { //next�� null�� �ƴ϶��  
			data->next_flight->prior_flight = NULL; //next�� prior�� null������ ���� (head�� �����ǹǷ�)
		}
		delete data;
		return;

	}
	data->prior_flight->next_flight = data->next_flight;
	if (data->next_flight != NULL) { //next�� null���� �ƴ϶�� 
		data->next_flight->prior_flight = data->prior_flight;
	}
	delete data;
	return;
}

flightNode* Flight_list::retrieve(string flight_num) const  //�ش� ��ȣ�� flightã�Ƽ� �ش� node��ȯ  
{
	flightNode* tmp = head;
	while (tmp != NULL)
	{
		if (tmp->flightNumber == flight_num) return tmp;
		else tmp = tmp->next_flight;
	}
	return NULL;
}

bool Flight_list::isEmpty()const {
	if (head == NULL) return true;
	else return false;
}


void Flight_list::PA()const { //�������� ��� �μ�  ����� ������ ���� ����� ���� ����� ��� 
	ofstream fout;
	fout.open("OUTFILE.P1.txt", ios_base::out | ios_base::app);
	flightNode* tmp = head;
	fout << " -     ����⺰ ������Ȳ �� �����Ȳ     -" << endl;
	while (tmp != NULL)
	{
		fout << "  �� flight Number" << "[" << tmp->flightNumber << "]" << endl;
		fout << "- ������Ȳ :  ";
		fout << tmp->head->getName() << endl;
		fout << "- �����Ȳ :   ";
		fout << tmp->queue->getName() << endl;
		tmp = tmp->next_flight;
		fout << endl;
	}
	fout << endl;
	fout << "==========================================" << endl;
	fout.close();
}


void Flight_list::PF()const { // ������� ���� ���� �μ� (��ȣ, �����, ������, �ִ� ž�°�, ����� �� ��)

	ofstream fout;
	fout.open("OUTFILE.P1.txt", ios_base::out | ios_base::app);
	flightNode* tmp = head;
	fout << "  -    ������� ����    -" << endl;
	while (tmp != NULL)
	{
		fout << "flight Number :  " << tmp->flightNumber << endl;
		fout << " ����� : " << tmp->from << endl;
		fout << " ������ : " << tmp->to << endl;
		fout << "�ִ� ž�°�: " << tmp->capacity << endl;
		fout << "����� �� �� : " << tmp->reserved_num << endl;
		fout << endl;
		fout << endl;
		tmp = tmp->next_flight;
	}
	fout << endl;
	fout << "==========================================" << endl;
	fout.close();
}
bool Flight_list::PR(string flightNum)const { //Ư�� ������ ����� ���� �̸������� �μ� / ����� �� �μ�
	flightNode* tmp = retrieve(flightNum);
	if (tmp == NULL) {
		return false;
	}
	ofstream fout;
	fout.open("OUTFILE.P1.txt", ios_base::out | ios_base::app);
	fout << "[" << flightNum << "]" << " ����⿡ ����� �� ����Ʈ:  ";
	fout << tmp->head->getName() << endl;
	fout <<"["<< flightNum <<"]" <<" ������� ���� ��� �� ����Ʈ:  ";
	fout << tmp->queue->getName() << endl;
	fout << endl;
	fout << "==========================================" << endl;
	fout.close();
	return true;
}
