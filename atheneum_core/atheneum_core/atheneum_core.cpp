/*
Atheneum
========

Supported Commands:

LOGIN /<username> /<password>

FETCH /<database name>

UPDATE /<database name>,<id>:[new,row,data]

REPORT 
*/

#include <stdio.h>
#include <Windows.h>
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")

#include "atheneum_file.h"
#include "atheneum_db.h"
#include "mix_parsing.h"
#include "date_parsing.h"
#include "atheneum_reporting.h"

std::string generate_report(atheneum_db db, atheneum_db db_books)
{
	std::string sTmp;

	std::string sData ="[REPORT: " + date_parsing::now() + "]\n";

	for (int i = 1; i <= db.atheneum_get_row_count(); i++)
	{
		std::vector<std::string> ss = db.atheneum_get_row(i);

		sTmp += "First Name: " + ss[0] + "\nLast Name: " + ss[1] + "\nType: " + ss[2] + "\n";
		sTmp += "============\n";

		for (std::vector<std::string> sd : mix_parsing::parse_out_array_items(ss[3]))
		{
			if (atheneum_reporting::calc_sum_due(true, date_parsing(date_parsing::now()), date_parsing(sd[1])) > 0) {
				if (strstr(ss[2].c_str(), "Teacher") != 0)
					sTmp += "Due: " + sd[1] + "\nBook: " + db_books.atheneum_get_row(atoi(sd[2].c_str()))[0] + "\nAmount: " + std::to_string(atheneum_reporting::calc_sum_due(true, date_parsing(date_parsing::now()), date_parsing(sd[1]))) + "\n";
				else
					sTmp += "Due: " + sd[1] + "\nBook: " + db_books.atheneum_get_row(atoi(sd[2].c_str()))[0] + "\nAmount: " + std::to_string(atheneum_reporting::calc_sum_due(false, date_parsing(date_parsing::now()), date_parsing(sd[1]))) + "\n";
			}
			else 
			{
				sTmp += "Due: " + sd[1] + "\nBook: " + db_books.atheneum_get_row(atoi(sd[2].c_str()))[0] + "\nId: " + sd[2] + "\n";
			}
		}
		sTmp += "============\n";
		sData += sTmp;
		sTmp.clear();
	}

	return sData;
}

bool remove_book_from_student(int id, int bid, atheneum_db db, atheneum_db dbb)
{
	if (dbb.atheneum_get_row(bid)[2][0] == 't')
	{
		std::vector<std::string> book_row = dbb.atheneum_get_row(bid);
		book_row[2] = "f";
		dbb.atheneum_update_row(bid, book_row);
	}
	else
		return false;

	std::vector<std::vector<std::string>> real = mix_parsing::parse_out_array_items(db.atheneum_get_row(id)[3]);
	std::vector<std::vector<std::string>> fake;

	for (std::vector<std::string> vec: real)
	{
		if (atoi(vec[2].c_str()) != bid)
		{
			fake.push_back(vec);
		}
	}

	std::vector<std::string> isitso = db.atheneum_get_row(id);

	isitso[3] = mix_parsing::parse_in_array_items(fake);

	db.atheneum_update_row(id, isitso);

	return true;
}

bool add_book_2_student(int id,std::string out, std::string due,int bid,atheneum_db db,atheneum_db dbb)
{

	if (dbb.atheneum_get_row(bid)[2][0] == 'f')
	{
		std::vector<std::string> book_row = dbb.atheneum_get_row(bid);
		book_row[2] = "t";
		dbb.atheneum_update_row(bid, book_row);
	}
	else 
		return false;
	

	std::string raw = db.atheneum_get_row(id)[3];
	std::vector<std::vector<std::string>> real = mix_parsing::parse_out_array_items(raw);

	if (real.size() > 5 && strstr(db.atheneum_get_row(id)[2].c_str(), "Teacher") == 0)
		return false;
	else if (real.size() > 10 && strstr(db.atheneum_get_row(id)[2].c_str(), "Teacher") != 0)
		return false;

	std::vector<std::string> toadd;
	toadd.push_back(out);
	toadd.push_back(due);
	toadd.push_back(std::to_string(bid));
	
	real.push_back(toadd);

	std::string right_back_at_ya = mix_parsing::parse_in_array_items(real);

	std::vector<std::string> isitso = db.atheneum_get_row(id);
	isitso[3] = right_back_at_ya;

	db.atheneum_update_row(id, isitso);

	return true;
}

int main()
{

	
	std::vector<std::string> headers;
	headers.push_back("str=FIRST_NAME");
	headers.push_back("str=LAST_NAME");
	headers.push_back("str=TYPE");
	headers.push_back("mixed=BOOKS_OUT"); 
	
	std::vector<std::string> book_headers;
	book_headers.push_back("str=BOOK_NAME");
	book_headers.push_back("str=BOOK_ID");

	atheneum_db db("core.am");
	atheneum_db db_book("books.am");

	if (!db_book.initialized)
		db_book.atheneum_initialize(book_headers);

	if (!db.initialized)
		db.atheneum_initialize(headers);
	
	
	WSADATA wsa;

	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6893);

	bind(s, (SOCKADDR*)&addr, sizeof(addr));

	listen(s, 500);

	while (1)
	{
		start:
		SOCKET sc = accept(s, 0, 0);
		printf("GOT CONNECTION\n");
		bool entered = false;

		while (1)
		{
			main_loop:
			char buff[5000];
			
			int sz = recv(sc, buff, 5000, 0);
			
			if (!sz > 0 || sz > 500) {
				entered = false;
				closesocket(sc);
				goto start;
				continue;
			}
			else if (sz > 0)
				buff[sz] = '\0';
			
			
			if (!entered && buff[0] == '0')
			{
				std::string user;
				std::string pass;

				for (int i = 1; i < strlen(buff); i++)
				{
					if (buff[i] == '/' && user.length() == 0) 
					{
						i++;
						while (buff[i] != ' ')
							user += buff[i], i++;
					}

					if (buff[i] == '/' && user.length() > 0) 
					{
						i++;
						while (buff[i] != ' ')
							pass += buff[i], i++;
					}
				}
				printf("LOGIN WITH %s and %s\n", user.c_str(), pass.c_str());
				if (strcmp(user.c_str(), "admin") == 0 && strcmp(pass.c_str(), "nimda") == 0)
					entered = true, send(sc, "1", 1, 0);
				else {
					send(sc, "0", 1, 0), closesocket(sc);
					goto start;
				}
				goto main_loop;
			}

			if (entered)
			{
				if (buff[0] == 'D')
					if (buff[1] == 'I')
						if (buff[2] == 'E') {
							printf("KILLED CONNECTION\n"), closesocket(sc);
							break;
						}

				std::string data = "";
				std::string sTmp = "";
				
				std::vector<std::string> vData;

				int id = -1; 
				int bid = -1;

				switch(buff[0])
				{
				case '1': //fetch <id> 
					
					if(buff[1] == '0')
					{
						for (std::string str : db.atheneum_get_header())
							data += str + ',';
						data += ";|";
						for (int i = 1; i <= db.atheneum_get_row_count(); i++)
						{
							for (std::string str : db.atheneum_get_row(i))
							{
								data += str + ',';
							}
							data += ';';
						}
					}
					else 
					{
						for (std::string str : db_book.atheneum_get_header())
							data += str + ',';
						data += ";|";
						for (int i = 1; i <= db_book.atheneum_get_row_count(); i++)
						{
							for (std::string str : db_book.atheneum_get_row(i))
							{
								data += str + ',';
							}
							data += ';';
						}
					}
					break;

				case '2': //update : /<Id> /row,data,yay,

					for (int i = 1; i < strlen(buff); i++)
					{
						if (buff[i] == '/' && id < 0)
						{
							i++;
							while (buff[i] != ' ')
								sTmp += buff[i], i++;
							id = atoi(sTmp.c_str());
							sTmp.clear();
						}

						if (buff[i] == '/')
						{
							i++;
							while (buff[i])
							{
								if (buff[i] != ',') {
									sTmp += buff[i];
								}
								else {
									vData.push_back(sTmp), sTmp.clear();
								}
								i++;
							}
						}
					}
					
					if (id == 0)
					{
						printf("ADD ROW %d with ", id);
						for (std::string str : vData)
							printf(" %s", str.c_str());
						printf("\n");
						db.atheneum_add_row(vData);
					}
					else
					{
						printf("UPDATE ROW %d with ", id);
						for (std::string str : vData)
							printf(" %s", str.c_str());
						printf("\n");
						db.atheneum_update_row(id, vData);
					}
					vData.clear();

					break;
				case '3': //report
					data = generate_report(db,db_book);

					break;

				case '4': //issue book : /<Id> /<bId>

					for (int i = 1; i < strlen(buff); i++)
					{
						if (buff[i] == '/' && id < 0)
						{
							i++;
							while (buff[i] != ' ')
								sTmp += buff[i], i++;
							id = atoi(sTmp.c_str());
							sTmp.clear();
						}

						if (buff[i] == '/')
						{
							i++;
							while (buff[i])
								sTmp += buff[i], i++;
						}
						bid = atoi(sTmp.c_str());
						sTmp.clear();
					}

					printf("ISSUE %d to %d\n", bid, id);

					if (strstr(db.atheneum_get_row(id)[2].c_str(), "Teacher") != 0) {
						if (add_book_2_student(id, date_parsing::now(), date_parsing::then(20), bid, db, db_book))
							data = "1";
						else
							data = "0";
					}
				
					else  {
						if (add_book_2_student(id, date_parsing::now(), date_parsing::then(10), bid, db, db_book))
							data = "1";
						else
							data = "0";
					}
					break;

				case '5': //return book <id> <bid>

					for (int i = 1; i < strlen(buff); i++)
					{
						if (buff[i] == '/' && id < 0)
						{
							i++;
							while (buff[i] != ' ')
								sTmp += buff[i], i++;
							id = atoi(sTmp.c_str());
							sTmp.clear();

						}
						
						if (buff[i] == '/')
						{
							i++;
							while (buff[i])
								sTmp += buff[i], i++;
							bid = atoi(sTmp.c_str());
							sTmp.clear();
						}
					}

					printf("RETURN %d from %d\n", bid, id);

					if (remove_book_from_student(id, bid, db, db_book))
						data = "1";
					else
						data = "0";

					break;

				default:
					data = "0-UNKNOWN";
					break;
				}

				send(sc, data.c_str(), data.length(), 0);

			}
		}
	}
	
	return getchar();
}

