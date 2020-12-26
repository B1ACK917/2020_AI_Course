#include <iostream>
#include <mysql.h>
#include <string>
using namespace std;

#define DO_QUERY(a,b) if(mysql_query(a,b)) throw string(mysql_error(a))

//��ʾsc��
void show_sc_table(MYSQL & mysql) {
	DO_QUERY(&mysql, "select * from sc");
	//��ȡ��ѯ���
	MYSQL_RES* result = mysql_store_result(&mysql);
	//��ȡ������
	uint64_t row_num = mysql_num_rows(result);
	//��ȡ������
	int num_fields = mysql_num_fields(result);
	//��ȡÿһ������
	MYSQL_FIELD* fields = mysql_fetch_fields(result);
	//��ʾ��ͷ
	for (auto i = 0; i < num_fields; i++)
		cout << fields[i].name << "\t";
	cout << endl;
	//�������ÿһ��
	for (auto i = 0;i < row_num;++i) {
		MYSQL_ROW row = mysql_fetch_row(result);
		for (auto j = 0;j < num_fields;++j)
			cout << row[j] << "\t";
		cout << endl;
	}
}

void insert_rows_into_sc_table(MYSQL& mysql) {
	string query_head = "insert into sc value";
	string new_query = "";
	string sno, cno, grade;
	char c;
	cout << "Ready To Insert Value Into SC Table" << endl;
	while (1) {
		//�������
		new_query = query_head;
		cout << "Please input sno (eg:10086): ";
		cin >> sno;
		cout << "Please input cno (eg:001): ";
		cin >> cno;
		cout << "Please input grade (eg:78): ";
		cin >> grade;
		//�������
		new_query += "('" + sno + "','" + cno + "'," + grade + ");";
		//ִ�в���
		DO_QUERY(&mysql, new_query.c_str());
		//��ʾsc��
		show_sc_table(mysql);
		cout << "Insert again? y:yes,n:no\t";
		cin >> c;
		if (c == 'n')
			break;
	}
}

int main() {
	//һЩ�������壬���ݿ��˻������
	int num = 0;
	char host[] = "localhost";
	char account[] = "root";
	char key[] = "B1ack@917";
	char database[] = "jxgl";
	int port = 3306;
	MYSQL mysql;
	try {
		//ʹ��mysql_init��ʼ��MYSQL�ṹ
		if (!mysql_init(&mysql))
			throw string("Initialize MYSQL Failed");
		//ʹ��account,key���ӵ����ݿ�
		if (!mysql_real_connect(&mysql, host, account, key, database, port, 0, 0))
			throw string("Connect To " + string(database) + " Failed");
		cout << "Connect to " << database << " Succeed" << endl;
		//����GBK��������ʾ����
		DO_QUERY(&mysql, "SET NAMES gbk;");
		if (mysql_list_tables(&mysql, "sc")->row_count) {
			//����Ƿ��Ѿ�����sc��
			cout << "SC Table Already Exists, Dropping" << endl;
			//������ڣ�ɾ��sc��
			DO_QUERY(&mysql, "drop table sc;");
			cout << "Dropped SC Table" << endl;
		}
		//����sc��
		DO_QUERY(&mysql, "create table sc(sno char(16), cno char(16), grade int);");
		cout << "Create SC Table" << endl;
		insert_rows_into_sc_table(mysql);
	}
	catch (string& errorMessage) {
		cout << errorMessage << endl;
	}
	return 0;
}