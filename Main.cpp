#include "List.h"
#include <fstream>
#include <string>
#include <string.h>

int main(void)
{
	Flight_list flightlist;  //����� ����
	ClientList clientlist; //�� ���� 
	ifstream in("samplein.p1.txt"); //���� ���� 

	if (in.is_open())
	{
		while (!in.eof())
		{
			string str;
			char* infoPtr[5] = { NULL, };
			getline(in, str); //���پ� �о���� 
			cout << str << endl;
			string cmd = str; //�տ� �α��ڰ� ��ɾ�
			cmd = cmd.substr(0, 2);

			char info[100];
			if (str.length() >= 3) {
				strcpy_s(info, str.substr(3, str.length()).c_str());
			}
			char* context = NULL;
			char* tok = strtok_s(info, " ",&context);

			if (cmd == "IF")//������ �߰� 
			{
				for (int i = 0; i < 4; i++) //����� ���� ���� �������� �����ϱ� 
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

			else if (cmd == "DF")//������ ����Ʈ���� ���� 
			{
				infoPtr[0] = tok;
				string flightnum(infoPtr[0]);
				flightNode* flight = flightlist.retrieve(flightnum);
				while (!flight->queue->isEmpty()) //client�� Reservervation list�� ������ ����� ����� ���� 
				{
					string name = flight->queue->Dequeue(); //����� �������� 
					clientNode* tmp = clientlist.retrieve(name); //�ش� ����� clientNode ã�� 
					tmp->head->remove_client(flightnum); //�ش� clientNode���� ������ ������� ��Ȳ ���� 
				}
				flightlist.remove(flightnum); //����� ����Ʈ���� ����� ���� 

			}

			else if (cmd == "RP")//���� ������ ����
			{
				for (int i = 0; i < 2; i++) //�� ���� �ޱ� 
				{
					infoPtr[i] = tok;
					tok = strtok_s(NULL, " ",&context);
				}
				string name(infoPtr[0]); //�� �̸� 
				string flightnum(infoPtr[1]); //����� ��ȣ 
				flightNode* flight = flightlist.retrieve(flightnum);

				if (flight == NULL)
				{
					cout << "�ش� ����Ⱑ �������� �ʽ��ϴ�" << endl;
					continue;
				}
				if (flight->head->duplicate(name)) { continue; } //�̹� �ߺ��ȴٸ� �ƿ� �������� �ʴ´�. 

		
		
				clientNode* client = clientlist.retrieve(name);
				if (client == NULL) //�ش� ���� �̹� �� ����Ʈ�� �ִ��� Ȯ���Ѵ�.
				                     //���ٸ� ������Ʈ�� �߰��ϰ�, reservation list�� ���������� �߰����ش�. 
				{
					clientNode* newNode = new clientNode(name);
					clientlist.insert(newNode);
					client = newNode;

				}
				//�ִٸ� ������Ʈ���� ã�� �� ����� reservation list�� ���������� �߰����ش� .
				Reservation_client* newReservation1 = new Reservation_client(client, flight); //��������� ���ฮ��Ʈ�� ���� ��� 
				Reservation_client* newReservation2 = new Reservation_client(client, flight); //������� ���ฮ��Ʈ�� ���� ��� 
				if (flight->reservation_statusUpdate()) //���� ���� 
				{
					flight->head->insert_flight(newReservation1); // ������� ���� ����Ʈ�� �ش� �� ���� 
					client->head->insert_client(newReservation2, true); //���� ���� ����Ʈ�� �ش� ����� ���� 
				}
				else  //����Ұ� 
				{
					flight->queue->Enqueue(name); //����� ����ܿ� �ֱ� 
					client->head->insert_client(newReservation2, false); //���� ���� ��Ȳ false�� ���־ ������� ǥ��
				}
			}

			else if (cmd == "CP") //�� ���� ��� 
			{
				for (int i = 0; i < 2; i++) //�� ���� �ޱ� 
				{
					infoPtr[i] = tok;
					tok = strtok_s(NULL, " ",&context);
				}
				string name(infoPtr[0]);
				string flightnum(infoPtr[1]);
				if (flightnum == "0")  //�ش� �� ���� ��� ���� 
				{
					clientlist.remove(name, clientlist);
					continue;
				}

				flightNode* flight = flightlist.retrieve(flightnum);  //Ư�� ����� ã�� 
				clientNode* client = clientlist.retrieve(name); //�ش� �� ��� ã��

				// ��ҵ� ����� number�� �߸� �Է��ϰų�, ���ฮ��Ʈ�� �ƴ϶� ��⸮��Ʈ�� �ö� ���� ���� ��� 
				if (flight->head->remove_flight(name)) { //�ش� ������� ���ฮ��Ʈ���� ���� �ߴٸ�
					if (!flight->queue->isEmpty()) //��� �ο��� �ٷ� ������ 
					{
						string waitingList = flight->queue->Dequeue(); //��� �ϼ��� �������� 
						clientNode* waiting = clientlist.retrieve(waitingList); //�ش� ��� ���� �� ��� �������� 
						Reservation_client* newNode = new Reservation_client(waiting, flight);
						flight->head->insert_flight(newNode); //������� ���ฮ��Ʈ�� �߰�
						Reservation_client* tmp = waiting->head->retrieve_client(flightnum);
						tmp->reservationComplete(); //�ش� client�� reservation ���¸� ��⿡�� ����Ϸ�� �ٲ۴�. 
					}
					else  //��� �ο��� ������ 
					{
						flight->cancle_reservation(); //�ο� ���� 
					}
				}
				else { //���ฮ��Ʈ�� ���ٸ� 
					flight->queue->remove(name); //�ش� ����� ��⸮��Ʈ���� �� ���� �õ�  
				}
				client->head->remove_client(flightnum); //client ���ฮ��Ʈ������ �ش� ����� ���� 

			}
			else if (cmd == "PA")// �������� ��� �μ�  ����� ������ ���� ����� ���� ����� ��� 
			{
				flightlist.PA();
			}
			else if (cmd == "PF") // ������� ���� ���� �μ� (��ȣ, �����, ������, �ִ� ž�°�, ����� �� ��)
			{
				flightlist.PF();
			}
			else if (cmd == "PR") //Ư�� ������ ����� ���� �̸������� �μ� / ����� �� �μ� 
			{
				infoPtr[0] = tok;
				string flightNum(infoPtr[0]);
				if (flightlist.PR(flightNum) == false) cout << "����Ⱑ �������� �ʽ��ϴ�." << endl;
			}
			else if (cmd == "PP")  //������ �̸��� ���� ���� ������� ��ȣ������ �μ� 
			{
				infoPtr[0] = tok;
				string name(infoPtr[0]);
				if (clientlist.PP(name) == false) cout << "�ش� ���� �������� �ʽ��ϴ�" << endl;
			}
		}
	}
	else
	{
		cout << "������ ���� �� �����ϴ�." << endl;
	}

	in.close();
	return 0;
}