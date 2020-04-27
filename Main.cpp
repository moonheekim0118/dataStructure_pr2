#include "List.h"
#include <fstream>
#include <string>
#include <string.h>

int main(void)
{
	Flight_list flightlist;  //비행기 저장
	ClientList clientlist; //고객 저장 
	ifstream in("samplein.p1.txt"); //파일 열기 

	if (in.is_open())
	{
		while (!in.eof())
		{
			string str;
			char* infoPtr[5] = { NULL, };
			getline(in, str); //한줄씩 읽어오기 
			cout << str << endl;
			string cmd = str; //앞에 두글자가 명령어
			cmd = cmd.substr(0, 2);

			char info[100];
			if (str.length() >= 3) {
				strcpy_s(info, str.substr(3, str.length()).c_str());
			}
			char* context = NULL;
			char* tok = strtok_s(info, " ",&context);

			if (cmd == "IF")//비행편 추가 
			{
				for (int i = 0; i < 4; i++) //비행기 정보 공백 기준으로 저장하기 
				{
					infoPtr[i] = tok;
					tok = strtok_s(NULL," ",&context);
				}

				string flightnum(infoPtr[0]);
				string fromcity(infoPtr[1]);
				string tocity(infoPtr[2]);
				string capa(infoPtr[3]);
				int capaInt = stoi(capa);
				flightNode* newnode = new flightNode(flightnum, fromcity, tocity, capa, capaInt);
				flightlist.insert(newnode);
			}

			else if (cmd == "DF")//비행편 리스트에서 삭제 
			{
				infoPtr[0] = tok;
				string flightnum(infoPtr[0]);
				flightNode* flight = flightlist.retrieve(flightnum);
				while (!flight->queue->isEmpty()) //client의 Reservervation list에 삭제될 비행기 대기명단 삭제 
				{
					string name = flight->queue->Dequeue(); //대기명단 가져오기 
					clientNode* tmp = clientlist.retrieve(name); //해당 명단의 clientNode 찾기 
					tmp->head->remove_client(flightnum); //해당 clientNode에서 삭제될 비행기의 현황 삭제 
				}
				flightlist.remove(flightnum); //비행기 리스트에서 비행기 삭제 

			}

			else if (cmd == "RP")//고객을 비행편에 예약
			{
				for (int i = 0; i < 2; i++) //고객 정보 받기 
				{
					infoPtr[i] = tok;
					tok = strtok_s(NULL, " ",&context);
				}
				string name(infoPtr[0]); //고객 이름 
				string flightnum(infoPtr[1]); //비행기 번호 
				flightNode* flight = flightlist.retrieve(flightnum);

				if (flight == NULL)
				{
					cout << "해당 비행기가 존재하지 않습니다" << endl;
					continue;
				}
				if (flight->head->duplicate(name)) { continue; } //이미 중복된다면 아예 연산하지 않는다. 

		
		
				clientNode* client = clientlist.retrieve(name);
				if (client == NULL) //해당 고객이 이미 고객 리스트에 있는지 확인한다.
				                     //없다면 고객리스트에 추가하고, reservation list에 예약정보를 추가해준다. 
				{
					clientNode* newNode = new clientNode(name);
					clientlist.insert(newNode);
					client = newNode;

				}
				//있다면 고객리스트에서 찾은 고객 노드의 reservation list에 예약정보를 추가해준다 .
				Reservation_client* newReservation1 = new Reservation_client(client, flight); //비행기노드의 예약리스트를 위한 노드 
				Reservation_client* newReservation2 = new Reservation_client(client, flight); //고객노드의 예약리스트를 위한 노드 
				if (flight->reservation_statusUpdate()) //예약 가능 
				{
					flight->head->insert_flight(newReservation1); // 비행기의 예약 리스트에 해당 고객 삽입 
					client->head->insert_client(newReservation2, true); //고객의 예약 리스트에 해당 비행기 삽입 
				}
				else  //예약불가 
				{
					flight->queue->Enqueue(name); //비행기 대기명단에 넣기 
					client->head->insert_client(newReservation2, false); //고객의 예약 현황 false로 해주어서 대기임을 표시
				}
			}

			else if (cmd == "CP") //고객 예약 취소 
			{
				for (int i = 0; i < 2; i++) //고객 정보 받기 
				{
					infoPtr[i] = tok;
					tok = strtok_s(NULL, " ",&context);
				}
				string name(infoPtr[0]);
				string flightnum(infoPtr[1]);
				if (flightnum == "0")  //해당 고객 정보 모두 삭제 
				{
					clientlist.remove(name, clientlist);
					continue;
				}

				flightNode* flight = flightlist.retrieve(flightnum);  //특정 비행기 찾기 
				clientNode* client = clientlist.retrieve(name); //해당 고객 노드 찾기

				// 취소될 비행기 number를 잘못 입력하거나, 예약리스트가 아니라 대기리스트에 올라가 있을 때를 대비 
				if (flight->head->remove_flight(name)) { //해당 비행기의 예약리스트에서 삭제 했다면
					if (!flight->queue->isEmpty()) //대기 인원이 바로 있으면 
					{
						string waitingList = flight->queue->Dequeue(); //대기 일순위 가져오기 
						clientNode* waiting = clientlist.retrieve(waitingList); //해당 대기 고객의 고객 노드 가져오기 
						Reservation_client* newNode = new Reservation_client(waiting, flight);
						flight->head->insert_flight(newNode); //비행기의 예약리스트에 추가
						Reservation_client* tmp = waiting->head->retrieve_client(flightnum);
						tmp->reservationComplete(); //해당 client의 reservation 상태를 대기에서 예약완료로 바꾼다. 
					}
					else  //대기 인원이 없으면 
					{
						flight->cancle_reservation(); //인원 증가 
					}
				}
				else { //예약리스트에 없다면 
					flight->queue->remove(name); //해당 비행기 대기리스트에서 고객 삭제 시도  
				}
				client->head->remove_client(flightnum); //client 예약리스트에서도 해당 비행기 삭제 

			}
			else if (cmd == "PA")// 예약정보 모두 인쇄  비행기 순서에 따라 예약된 고객과 대기자 명단 
			{
				flightlist.PA();
			}
			else if (cmd == "PF") // 비행기편에 대한 정보 인쇄 (번호, 출발지, 도착지, 최대 탑승객, 예약된 고객 수)
			{
				flightlist.PF();
			}
			else if (cmd == "PR") //특정 비행편예 예약된 고객을 이름순으로 인쇄 / 대기중 고객 인쇄 
			{
				infoPtr[0] = tok;
				string flightNum(infoPtr[0]);
				if (flightlist.PR(flightNum) == false) cout << "비행기가 존재하지 않습니다." << endl;
			}
			else if (cmd == "PP")  //지정된 이름의 고객에 대한 비행기편 번호순으로 인쇄 
			{
				infoPtr[0] = tok;
				string name(infoPtr[0]);
				if (clientlist.PP(name) == false) cout << "해당 고객이 존재하지 않습니다" << endl;
			}
		}
	}
	else
	{
		cout << "파일을 읽을 수 없습니다." << endl;
	}

	in.close();
	return 0;
}