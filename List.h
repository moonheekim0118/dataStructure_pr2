#include <new>
#include <stdexcept>
#include <iostream>
using namespace std;
class waitingList; // ��⸮��Ʈ 
class Flight_list; // ����� ����Ʈ
class ClientList; //�� ����Ʈ 
class Reservation_list; //���� ����Ʈ 



class waitingNode {               // ��⸮��Ʈ�� ���� ��� 
public:
	waitingNode(string Name, waitingNode* next);
private:
	string name; //�� �̸� 
	waitingNode* next; //������� ����
	friend class waitingList;
	friend class flightNode;
};

class waitingList                 //��⸮��Ʈ  (���Ḯ��Ʈ ť)
{
public:
	waitingList();
	~waitingList();
	void Enqueue(string name);
	string Dequeue();
	void Clear();
	bool isEmpty() const;
	void remove(string name);
	string getName() const;
private:
	waitingNode* front;
	waitingNode* rear;

};


class flightNode                    //����� ���  
{

public:
	flightNode(string flightNumber, string from, string to, string capacity, int capacity_int); //������
	~flightNode(); //����� ��忡 ������ ���ฮ��Ʈ�� ��⸮��Ʈ �Ҹ��� ���� �Ҹ��� 
	waitingList* queue; //��⸮��Ʈ�� ���� 
	Reservation_list* head; //���ฮ��Ʈ�� ���� 
	bool reservation_statusUpdate(); //������Ȳ ������Ʈ 
	void cancle_reservation(); //���� ���  

private:
	string flightNumber;   //����� ��ȣ 
	string from; //�����
	string to; //������ 
	string capacity; // �ִ� ž�°� �� 
	int capacity_int; //�ִ� ž�°� �� (������)

	int reserved_num; //����� ���� �� 
	flightNode* prior_flight; //���� ������� ���� ������ 
	flightNode* next_flight; //���� ������� ���� ������        //����� ����Ʈ�� ���� ������

	friend class Flight_list;
	friend class Reservation_list;

};


class Flight_list  //����� ����Ʈ [����� ��ȣ �� ����]
{

public:
	Flight_list(); //������
	~Flight_list(); //�Ҹ��� 
	void insert(flightNode* newNode); //������ �߰� 
	void remove(string flight_num);  //������ ����
	flightNode* retrieve(string flight_num) const; //Ư�� ��ȣ�� ������ ã�� 
	bool isEmpty() const;
	void clear(); //��� ���� 

	//�μ� �Լ� 
	void PA()const;
	void PF()const;
	bool PR(string flightNum)const;

private:
	flightNode* head;
};


class clientNode //������ ��� 
{
public:
	Reservation_list* head; //���� ����Ʈ�� ���� 
	clientNode(string name); //������ 
	~clientNode(); //��忡 ������ ���ฮ��Ʈ �Ҹ��� ���� �Ҹ��� 
	
private:
	string Name; //�̸� 
	clientNode* prior;  //���� ���� ���� ������
	clientNode* next;  //���� ���� ���� ������  

	friend class ClientList;
	friend class Reservation_client;
	friend class Reservation_list;
};


class ClientList {   //�� ���� ����Ʈ 
private:
	clientNode* head;
	clientNode* cursor;
public:
	ClientList();     //������
	~ClientList();    //�Ҹ��� 
	void insert(clientNode* newNode);   //������ �߰� 
	void remove(string name, ClientList& clientlist); //������ ���� 
	bool isEmpty()const;
	void clear();

	//�μ��Լ� 
	bool PP(string name) const; 
	clientNode* retrieve(string name) const; //Ư���̸��� �� ��� ��ȯ 
};



class Reservation_node  // ���ฮ��Ʈ�� ���� ��� 
{
public:
	Reservation_node(clientNode* client, flightNode* flight); //������ 
	bool status()const; //���������� ����������� �˷��ش�.
	void reservationComplete(); // ������ - > ���� �Ϸ� ���·� �ٲپ��ִ� �Լ� 
private:
	clientNode* client; //�� ��� 
	flightNode* flight; //������� 
	Reservation_node* next;
	Reservation_node* prior;
	bool reserved; //�� ������ OK�Ǿ����� �����¸� �˸��� flag
	friend class Reservation_list;
};



class Reservation_list  //���ฮ��Ʈ  (���� �̸� �� ����)
{

public:
	Reservation_list(); //������ 
	~Reservation_list(); //�Ҹ��� 
	void insert_flight(Reservation_node* newNode); //*(�����) ���� �� ����
	void insert_client(Reservation_node* newNode, bool done); // *(��) ���� ����� ���� 
	
	bool remove_flight(string name); //������� ���ฮ��Ʈ���� Ư�� �� ����  
	void remove_client(string flight_num); //���� ���ฮ��Ʈ���� Ư�� ����� ���� ���� 

	void removeAllinClientReservation(string flightNumber); //�ش� �����number�� ������ ��� �� ���� ���� 
	void removeAllinFlightReservation(string name, ClientList& clientlist); //�ش� name�� ��� �������� ���� 

	Reservation_node* retrieve_flight(string name) const; //�̸����� Ư�� �� ã�� (����� - ���ฮ��Ʈ����) 
	Reservation_node* retrieve_client(string flightNumber) const; //����� ��ȣ�� Ư�� ���� ã��(�� -���ฮ��Ʈ����)
	

	bool isEmpty() const;
	void clear();
	bool duplicate(string name) const; //�ߺ�Ȯ��  

	string getName() const; // Ư�� ����⿡ ����� �� �̸� string ���·� ��� ��ȯ 
	string getNumbers_reserved() const; //Ư�� ���� ������ ����� ��ȣ string ���·� ��� ��ȯ 
	string getNumbers_waiting() const; // Ư�� ���� ������� ����� ��ȣ string ���·� ��� ��ȯ 
	friend class flightNode;
private:
	Reservation_node* head;
	Reservation_node* cursor;

};
