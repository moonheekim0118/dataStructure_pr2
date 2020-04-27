#include <new>
#include <stdexcept>
#include <iostream>
using namespace std;
class waitingList; // 대기리스트 
class Flight_list; // 비행기 리스트
class ClientList; //고객 리스트 
class Reservation_list; //예약 리스트 



class waitingNode {               // 대기리스트를 위한 노드 
public:
	waitingNode(string Name, waitingNode* next);
private:
	string name; //고객 이름 
	waitingNode* next; //다음노드 연결
	friend class waitingList;
	friend class flightNode;
};

class waitingList                 //대기리스트  (연결리스트 큐)
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


class flightNode                    //비행기 노드  
{

public:
	flightNode(string flightNumber, string from, string to, string capacity, int capacity_int); //생성자
	~flightNode(); //비행기 노드에 생성된 예약리스트와 대기리스트 소멸을 위한 소멸자 
	waitingList* queue; //대기리스트와 연결 
	Reservation_list* head; //예약리스트와 연결 
	bool reservation_statusUpdate(); //예약현황 업데이트 
	void cancle_reservation(); //예약 취소  

private:
	string flightNumber;   //비행기 번호 
	string from; //출발지
	string to; //도착지 
	string capacity; // 최대 탑승객 수 
	int capacity_int; //최대 탑승객 수 (정수형)

	int reserved_num; //예약된 고객의 수 
	flightNode* prior_flight; //이전 비행기편에 대한 포인터 
	flightNode* next_flight; //다음 비행기편에 대한 포인터        //비행기 리스트를 위한 포인터

	friend class Flight_list;
	friend class Reservation_list;

};


class Flight_list  //비행기 리스트 [비행기 번호 순 정렬]
{

public:
	Flight_list(); //생성자
	~Flight_list(); //소멸자 
	void insert(flightNode* newNode); //비행편 추가 
	void remove(string flight_num);  //비행편 삭제
	flightNode* retrieve(string flight_num) const; //특정 번호의 비행편 찾기 
	bool isEmpty() const;
	void clear(); //모두 삭제 

	//인쇄 함수 
	void PA()const;
	void PF()const;
	bool PR(string flightNum)const;

private:
	flightNode* head;
};


class clientNode //고객정보 노드 
{
public:
	Reservation_list* head; //예약 리스트와 연결 
	clientNode(string name); //생성자 
	~clientNode(); //노드에 생성된 예약리스트 소멸을 위한 소멸자 
	
private:
	string Name; //이름 
	clientNode* prior;  //이전 고객에 대한 포인터
	clientNode* next;  //다음 고객에 대한 포인터  

	friend class ClientList;
	friend class Reservation_client;
	friend class Reservation_list;
};


class ClientList {   //고객 정보 리스트 
private:
	clientNode* head;
	clientNode* cursor;
public:
	ClientList();     //생성자
	~ClientList();    //소멸자 
	void insert(clientNode* newNode);   //고객정보 추가 
	void remove(string name, ClientList& clientlist); //고객정보 삭제 
	bool isEmpty()const;
	void clear();

	//인쇄함수 
	bool PP(string name) const; 
	clientNode* retrieve(string name) const; //특정이름의 고객 노드 반환 
};



class Reservation_client  // 예약리스트를 위한 노드 
{
public:
	Reservation_client(clientNode* client, flightNode* flight); //생성자 
	bool status()const; //대기상태인지 예약상태인지 알려준다.
	void reservationComplete(); // 대기상태 - > 예약 완료 상태로 바꾸어주는 함수 
private:
	clientNode* client; //고객 노드 
	flightNode* flight; //비행기편 
	Reservation_client* next;
	bool reserved; //이 예약이 OK되었는지 대기상태를 알리는 flag
	friend class Reservation_list;
};



class Reservation_list  //예약리스트  (고객의 이름 순 정렬)
{

public:
	Reservation_list(); //생성자 
	~Reservation_list(); //소멸자 
	void insert_flight(Reservation_client* newNode); //*(비행기) 예약 고객 삽입
	void insert_client(Reservation_client* newNode, bool done); // *(고객) 예약 비행기 삽입 
	
	bool remove_flight(string name); //비행기의 예약리스트에서 특정 고객 삭제  
	void remove_client(string flight_num); //고객의 예약리스트에서 특정 비행기 예약 삭제 

	void removeAllinClientReservation(string flightNumber); //해당 비행기number에 예약한 모든 고객 예약 삭제 
	void removeAllinFlightReservation(string name, ClientList& clientlist); //해당 name의 모든 고객예약을 삭제 

	Reservation_client* retrieve_flight(string name) const; //이름으로 특정 고객 찾기 (비행기 - 예약리스트에서) 
	Reservation_client* retrieve_client(string flightNumber) const; //비행기 번호로 특정 예약 찾기(고객 -예약리스트에서)
	

	bool isEmpty() const;
	void clear();
	bool duplicate(string name) const; //중복확인  
	bool status()const; 

	string getName() const; // 특정 비행기에 예약된 고객 이름 string 형태로 모두 반환 
	string getNumbers_reserved() const; //특정 고객이 예약한 비행기 번호 string 형태로 모두 반환 
	string getNumbers_waiting() const; // 특정 고객이 대기중인 비행기 번호 string 형태로 모두 반환 
	friend class flightNode;
private:
	Reservation_client* head;

};
