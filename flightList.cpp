#include "List.h";
#include <string>
#include <fstream>
/*비행기 리스트 */

flightNode::flightNode(string flightNumber, string from, string to, string capacity, int capacity_int) //생성자 
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

flightNode::~flightNode()  //소멸자 
{
	delete this->head;   //내부에 있는 리스트에 대한 포인터들 모두 소멸 
	delete this->queue; 
}

void flightNode::cancle_reservation() { //예약 취소 (대기인원이 없을 때)
	reserved_num--;
}

bool flightNode::reservation_statusUpdate() //예약 및 예약 가능 여부 판단 
{
	if (reserved_num + 1 <= capacity_int)
	{
		reserved_num++;
		return true;
	}
	else return false;
}


Flight_list::Flight_list() { //생성자  
	this->head = NULL;
}

Flight_list::~Flight_list() { //소멸자 
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


void Flight_list::insert(flightNode* newNode) //비행편 추가  //번호순 정렬
{
	if (isEmpty()) { //첫 노드인경우 
		head = newNode; //헤드가 가리키도록 
	}
	else { //첫 노드가 아닌 경우 
		flightNode* tmp = head;
		flightNode* tmpBefore = NULL;
		while (tmp != NULL)
		{
			int order = tmp->flightNumber.compare(newNode->flightNumber);
			if (order == 0) {
				cout << "해당 비행편이 이미 존재합니다" << endl; //오류처리 
				return;
			}
			else if (order > 0) break;
			tmpBefore = tmp;
			tmp = tmp->next_flight;

		}

		//tmpBefore와 tmp 사이에 넣는다.
		newNode->prior_flight = tmpBefore;
		newNode->next_flight = tmp;
		if (tmpBefore != NULL)
		{
			tmpBefore->next_flight = newNode;
		}
		if (tmp != NULL) {
			tmp->prior_flight = newNode;
		}
	}
}


void Flight_list::remove(string flight_num)  //비행편 삭제
{
	if (isEmpty())
	{
		cout << "list is empty!" << endl;
		return;
	}

	flightNode* data = retrieve(flight_num); //해당 비행편 찾기 

	if (data == NULL)
	{
		cout << "비행기 정보가 잘못 되었습니다." << endl; //오류 처리 
		return;
	}

	data->head->removeAllinClientReservation(data->flightNumber);
	//해당 data의 reservation list 방문한다.
	// reservation list에 있는 각각 clienet들의 예약리스트에서 해당 비행기에 대한 모든 현황을 삭제한다.


	if (data == head) { //삭제할 노드가 head인 경우 
		head = data->next_flight; //head 변경  
		if (data->next_flight != NULL) { //next가 null이 아니라면  
			data->next_flight->prior_flight = NULL; //next의 prior를 null값으로 변경 (head가 삭제되므로)
		}
		delete data;
		return;

	}
	data->prior_flight->next_flight = data->next_flight;
	if (data->next_flight != NULL) { //next가 null값이 아니라면 
		data->next_flight->prior_flight = data->prior_flight;
	}
	delete data;
	return;
}

flightNode* Flight_list::retrieve(string flight_num) const  //해당 번호의 flight찾아서 해당 node반환  
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


void Flight_list::PA()const { //예약정보 모두 인쇄  비행기 순서에 따라 예약된 고객과 대기자 명단 
	ofstream fout;
	fout.open("OUTFILE.P1.txt", ios_base::out | ios_base::app);
	flightNode* tmp = head;
	fout << " -     비행기별 예약현황 및 대기현황     -" << endl;
	while (tmp != NULL)
	{
		fout << "  ● flight Number" << "[" << tmp->flightNumber << "]" << endl;
		fout << "- 예약현황 :  ";
		fout << tmp->head->getName() << endl;
		fout << "- 대기현황 :   ";
		fout << tmp->queue->getName() << endl;
		tmp = tmp->next_flight;
		fout << endl;
	}
	fout << endl;
	fout << "==========================================" << endl;
	fout.close();
}


void Flight_list::PF()const { // 비행기편에 대한 정보 인쇄 (번호, 출발지, 도착지, 최대 탑승객, 예약된 고객 수)

	ofstream fout;
	fout.open("OUTFILE.P1.txt", ios_base::out | ios_base::app);
	flightNode* tmp = head;
	fout << "  -    비행기편 정보    -" << endl;
	while (tmp != NULL)
	{
		fout << "flight Number :  " << tmp->flightNumber << endl;
		fout << " 출발지 : " << tmp->from << endl;
		fout << " 도착지 : " << tmp->to << endl;
		fout << "최대 탑승객: " << tmp->capacity << endl;
		fout << "예약된 고객 수 : " << tmp->reserved_num << endl;
		fout << endl;
		fout << endl;
		tmp = tmp->next_flight;
	}
	fout << endl;
	fout << "==========================================" << endl;
	fout.close();
}
bool Flight_list::PR(string flightNum)const { //특정 비행편예 예약된 고객을 이름순으로 인쇄 / 대기중 고객 인쇄
	flightNode* tmp = retrieve(flightNum);
	if (tmp == NULL) {
		return false;
	}
	ofstream fout;
	fout.open("OUTFILE.P1.txt", ios_base::out | ios_base::app);
	fout << "[" << flightNum << "]" << " 비행기에 예약된 고객 리스트:  ";
	fout << tmp->head->getName() << endl;
	fout <<"["<< flightNum <<"]" <<" 비행기의 예약 대기 고객 리스트:  ";
	fout << tmp->queue->getName() << endl;
	fout << endl;
	fout << "==========================================" << endl;
	fout.close();
	return true;
}
