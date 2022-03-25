# include <vector>
# include <iostream>
# include <string>
# include <algorithm>
# include <random>
# include <fstream>
# include <ctime>
# include <iomanip>

using namespace std;

struct Student
{
	string name = "";
	int stdid = -1;
};

void release_cinerr();
void grouping();
void direct();
string get_localtime();
string get_classname();
string get_examname();
void outputfile(string& opfile_name, vector<Student>& stulist, bool first_oba);
void output_seating(string& opfile_name, vector<Student>& stulist, unsigned short choice, unsigned short column, unsigned short raw);
void derive_seating_normal(string& opfile_name, vector<Student>& stulist, unsigned short choice, unsigned short column, unsigned short raw);
void derive_seating_seprate(string& opfile_name, vector<Student>& stulist, unsigned short choice, unsigned short column, unsigned short raw);
void derive_seating_checkerboard(string& opfile_name, vector<Student>& stulist, unsigned short choice, unsigned short column, unsigned short raw);
void set_column_and_raw(unsigned std_num, unsigned short& column, unsigned short& raw);

int main()
{
	cout << "########## 座位表排位器主選項 ##########\n"
		<< "========================================\n"
		<< "1.單一班\n2.單雙號\nany.結束程式\n"
		<< "========================================\n"
		<< "請輸入欲執行選項:";

	unsigned short choice = 0;
	cin >> choice;
	cin.get();

	while (choice == 1 || choice == 2)
	{
		if (choice != 1 && choice != 2)
			break;

		else
		{

			if (choice == 1)
				direct();

			else
				grouping();
		}

		cout << "########## 座位表排位器主選項 ##########\n"
			<< "========================================\n"
			<< "1.單一班\n2.單雙號\nany.結束程式\n"
			<< "========================================\n"
			<< "請輸入欲執行選項:";
		cin >> choice;
		cin.get();

		if (cin.fail())
		{
			release_cinerr();
			break;
		}
	}

	return 0;
}

void release_cinerr()
{
	cin.clear();
	cin.ignore();
}

string get_classname()
{
	string classname;
	char tmp = cin.get();
	if (tmp == '\n')
		classname = "(null)";
	else {
		cin >> classname;
		classname = tmp + classname;
		tmp = cin.get();
	}

	return classname;
}

string get_examname()
{
	string examname;
	char tmp = cin.get();

	if (tmp == '\n')
		examname = "(null)";
	else
	{
		cin >> examname;
		examname = tmp + examname;
		cin.get();
	}

	return examname;
}

void derive_seating_checkerboard(string& opfile_name, vector<Student>& stulist, unsigned short choice, unsigned short column, unsigned short raw)
{
	cout << "\n[Msg] 您選擇的模式為:梅花座座位表\n"
		<< "[Msg] 教室大小為:" << column << " 排 " << raw << " 列\n"
		<< "[Msg] 後續將產生之檔案名稱:" << opfile_name << "_梅花座.csv\n";

	int seat_num = 0;

	for (int i = 1; i <= raw; i++)
	{
		if (i & 1)
		{
			for (int j = 1; j <= column; j += 2)
				seat_num++;
		}
		else
		{
			for (int j = 2; j <= column; j += 2)
				seat_num++;
		}
	}

	if (stulist.size() > seat_num)
	{
		cout << "\n[Err] 教室座位不足分配，請重新安排!\n";
		cout << "教室空間: " << seat_num << " 個座位 / 學生人數: " << stulist.size() << " 人\n";
		return;
	}

	cout << "\n[Msg] 開始產生座位表...\n";

	ofstream ofs;
	ofs.open("history\\" + opfile_name + "_梅花座.csv", ios::out);

	if (!ofs.is_open())
		cout << "[Err] 由於不明原因，無法生成座位表!\n";

	else
	{
		ofs << "座位表產生時間:" << "," << "," << get_localtime() << "," << endl << endl;

		unsigned short t_col = (column & 1) ? (column >> 1) + 1 : (column >> 1);

		for (int i = 1; i < t_col; i++)
			ofs << " " << ",";
		ofs << "黑板" <<  "," << endl << endl;
		unsigned short std_ith = 0, std_jth = 0;

		for (int i = 1; i <= raw; i++)
		{
			if (i & 1)
			{
				for (int j = 1; j <= column; j += 2)
				{
					if (std_ith < stulist.size())
					{
						ofs << stulist[std_ith].stdid << "," << " " << ",";
						std_ith++;
					}
					else
						break;
				}

				ofs << endl;

				for (int j = 1; j <= column; j += 2)
				{
					if (std_jth < stulist.size())
					{
						ofs << stulist[std_jth].name << "," << " " << ",";
						std_jth++;
					}
					else
						break;
				}

				ofs << endl;

				if (std_ith >= stulist.size())
					break;
			}
			else
			{
				for (int j = 2; j <= column; j += 2)
				{
					if (std_ith < stulist.size())
					{
						ofs << " " << "," << stulist[std_ith].stdid << ",";
						std_ith++;
					}
					else
						break;
				}

				ofs << endl;

				for (int j = 2; j <= column; j += 2)
				{
					if (std_jth < stulist.size())
					{
						ofs << " " << "," << stulist[std_jth].name << ",";
						std_jth++;
					}
					else
						break;
				}

				ofs << endl;

				if (std_ith >= stulist.size())
					break;
			}
		}

		ofs.close();
		cout << "[Msg] 已成功產生座位表:" << opfile_name << "_梅花座.csv!\n" << endl;

	}
}

void derive_seating_seprate(string& opfile_name, vector<Student>& stulist, unsigned short choice, unsigned short column, unsigned short raw)
{
	cout << "\n[Msg] 您選擇的模式為:間隔直方座位表\n"
		<< "[Msg] 教室大小為:" << column << " 排 " << raw << " 列\n"
		<< "[Msg] 後續將產生之檔案名稱:" << opfile_name << "_間隔直方.csv\n";

	int seat_num = 0;
	if (column & 1)
		seat_num = ((column >> 1) + 1) * raw;
	else
		seat_num = (column >> 1) * raw;

	if (stulist.size() > seat_num)
	{
		cout << "\n[Err] 教室座位不足分配，請重新安排!\n";
		cout << "教室空間: " << seat_num << " 個座位 / 學生人數: "  << stulist.size() << " 人\n";
		return;
	}

	cout << "\n[Msg] 開始產生座位表...\n";

	ofstream ofs;
	ofs.open("history\\" + opfile_name + "_間隔直方.csv", ios::out);

	if (!ofs.is_open())
		cout << "[Err] 由於不明原因，無法生成座位表!\n";

	else
	{
		ofs << "座位表產生時間:" << "," << "," << get_localtime() << "," << endl << endl;

		unsigned short t_col = (column & 1) ? (column >> 1) + 1 : (column >> 1);

		for (int i = 1; i < t_col; i++)
			ofs << " " << ",";
		ofs << "黑板" << endl << endl;
		unsigned short std_ith = 0, std_jth = 0;

		for (int i = 1; i <= raw; i++)
		{
			for (int j = 1; j <= column; j += 2)
			{
				if (std_ith < stulist.size())
				{
					ofs << stulist[std_ith].stdid << "," << " " << ",";
					std_ith++;
				}
				else
					break;
			}

			ofs << endl;

			for (int j = 1; j <= column; j += 2)
			{
				if (std_jth < stulist.size())
				{
					ofs << stulist[std_jth].name << "," << " " << ",";
					std_jth++;
				}
				else
					break;
			}

			ofs << endl;

			if (std_ith >= stulist.size())
				break;
		}

		ofs.close();
		cout << "[Msg] 已成功產生座位表:" << opfile_name << "_間隔直方.csv!\n" << endl;

	}
}

void derive_seating_normal(string& opfile_name, vector<Student>& stulist, unsigned short choice, unsigned short column, unsigned short raw)
{
	cout << "\n[Msg] 您選擇的模式為:標準直方座位表\n"
		<< "[Msg] 教室大小為:" << column << " 排 " << raw << " 列\n"
		<< "[Msg] 後續將產生之檔案名稱:" << opfile_name << "_標準直方.csv\n";

	int seat_num = column * raw;
	if (stulist.size() > seat_num)
	{
		cout << "\n[Err] 教室座位不足分配，請重新安排!\n";
		cout << "教室空間: " << seat_num << " 個座位 / 學生人數: " << stulist.size() << " 人\n";
		return;
	}

	cout << "\n[Msg] 開始產生座位表...\n";

	ofstream ofs;
	ofs.open("history\\" + opfile_name + "_標準直方.csv", ios::out);

	if (!ofs.is_open())
		cout << "[Err] 由於不明原因，無法生成座位表!\n";

	else
	{
		ofs << "座位表產生時間:" << "," << "," << get_localtime() << "," << endl << endl;

		unsigned short t_col = (column & 1) ? (column >> 1) + 1 : (column >> 1);

		for (int i = 1; i < t_col; i++)
			ofs << " " << ",";
		ofs << "黑板" << endl << endl;
		unsigned short std_ith = 0, std_jth = 0;

		for (int i = 1; i <= raw; i++)
		{
			for (int j = 1; j <= column; j++)
			{
				if (std_ith < stulist.size())
				{
					ofs << stulist[std_ith].stdid << ",";
					std_ith++;
				}
				else
					break;
			}

			ofs << endl;

			for (int j = 1; j <= column; j++)
			{
				if (std_jth < stulist.size())
				{
					ofs << stulist[std_jth].name << ",";
					std_jth++;
				}
				else
					break;
			}

			ofs << endl;

			if (std_ith >= stulist.size())
				break;
		}

		ofs.close();
		cout << "[Msg] 已成功產生座位表:" << opfile_name << "_標準直方.csv!\n" << endl;

	}
}

void output_seating(string& opfile_name, vector<Student>& stulist, unsigned short choice, unsigned short column, unsigned short raw)
{

	if (choice == 1 || choice == 2 || choice == 3)
	{
		if (choice == 1)
			derive_seating_normal(opfile_name, stulist, choice, column, raw);
		else if (choice == 2)
			derive_seating_seprate(opfile_name, stulist, choice, column, raw);
		else
			derive_seating_checkerboard(opfile_name, stulist, choice, column, raw);
	}
}

void outputfile(string& opfile_name, vector<Student>& stulist, bool first_opa)
{
	ofstream ofs;
	ofs.open("history\\" + opfile_name + "_清單.csv", ios::out);

	if (!ofs.is_open())
		cout << "[Err] 由於不明原因，無法生成檔案!\n";

	else
	{
		if (first_opa)
		{
			cout << "**************************\n清單匯出時間\n";
			cout << get_localtime();
			cout << "\n**************************\n\n";
		}

		ofs << "序號" << "," << "學號" << "," << "姓名" << endl;
		for (int i = 0; i < stulist.size(); i++)
			ofs << i + 1 << "," << stulist[i].stdid << "," << stulist[i].name << endl;

		ofs.close();
		cout << "[Msg] 已成功匯出亂序名單:" << opfile_name << "_清單.csv!\n" << endl;

	}

}

string get_localtime()
{
	time_t time_ptr;
    time_ptr = time(NULL);

    tm* tm_local = localtime(&time_ptr);

	string year = to_string(tm_local->tm_year + 1900);
	string mon = to_string(tm_local->tm_mon + 1);

	string nowtime = year + "年 " + mon + "月 " + to_string(tm_local->tm_mday) + "日 "
		+ to_string(tm_local->tm_hour) + "點 " + to_string(tm_local->tm_min) + "分";

	return nowtime;
}

void set_column_and_raw(unsigned std_num, unsigned short& column, unsigned short& raw)
{
	cout << "請輸入教室座位排數:";

	do {
		cin >> column;
		cin.get();

		if (cin.fail())
		{
			release_cinerr();
			column = 0;
		}

		if (!(column >=1 && column <= 100))
		{
			cout << "[Err] 排數必須至少為\"1\"\n";
			cin >> column;
			cin.get();

			if (cin.fail())
			{
				release_cinerr();
				column = 0;
			}
		}

	} while (!(column >= 1 && column <= 100));

	cout << "請輸入每排座位個數:";
	do {
		cin >> raw;
		cin.get();

		if (cin.fail())
		{
			release_cinerr();
			raw = 0;
		}

		if (!(raw >= 1 && raw <= 100))
		{
			cout << "[Err] 座位數必須至少為\"1\"!\n";
			cin >> raw;
			cin.get();

			if (cin.fail())
			{
				release_cinerr();
				raw = 0;
			}
		}
	} while (!(raw >= 1 && raw <= 100));

}

void grouping()
{
	string classname, examname;
	cout << "請輸入開課系級(可按下enter忽略):";
	classname = get_classname();

	cout << "請輸入考試名稱(可按下enter忽略):";
	examname = get_examname();

	cout << "\n[Msg] 後續將產生之檔案名稱: \n";
	string ofname_1 = "#" + classname + "#" + examname + "_" + "單號";
	cout << ofname_1 << "_清單.csv\n";
	string ofname_2 = "#" + classname + "#" + examname + "_" + "雙號";
	cout << ofname_2 << "_清單.csv\n";

	cout << "\n[Msg] ※輸入或貼上格式錯誤將導致程式直接中止!";
	cout << "\n請輸入或貼上學生清單(學號 姓名)\n(輸入完畢後，再在新行輸入0開始排位。):\n";

	vector<Student> std_list_odd, std_list_even;
	std_list_odd.reserve(50);
	std_list_even.reserve(50);

	string name;
	Student std_rec;

	cin >> std_rec.stdid;
	while (cin.fail())
	{
		release_cinerr();
		cout << "[Err] 輸入非法的學號! 請重新輸入:";
		cin >> std_rec.stdid;
	}

	while (std_rec.stdid != 0)
	{
		cin >> name;

		std_rec.name = name;

		if (std_rec.stdid & 1)
			std_list_odd.push_back(std_rec);
		else
			std_list_even.push_back(std_rec);

		cin >> std_rec.stdid;
		while (cin.fail())
		{
			release_cinerr();
			cout << "[Err] 輸入非法的學號! 請重新輸入:";
			cin >> std_rec.stdid;
		}
	}

	if (std_list_odd.size() == 0 && std_list_even.size() == 0)
	{
		cout << "[Err] 輸入資料為空! 結束本階段程序。\n";
		cout << "[End] ----------------------------------------------\n\n\n\n\n";
	}

	srand(time(NULL));
	unsigned seed = rand();
	shuffle(begin(std_list_even), end(std_list_even), default_random_engine(seed));
	shuffle(begin(std_list_odd), end(std_list_odd), default_random_engine(seed));

	cout << "[msg] " << "單號人數:" << std_list_odd.size() << " 雙號人數:" << std_list_even.size() << endl;
	// write file
	outputfile(ofname_1, std_list_odd, true);
	outputfile(ofname_2, std_list_even, false);
	cout << "[End] ----------------------------------------------\n\n\n\n\n";

	cout << "**************************\n"
		<< "1.標準直方\n"
		<< "2.間隔直方\n"
		<< "3.梅花座\n"
		<< "any. 不產生/退回清單生成\n"
		<< "**************************\n";
	cout << "請選擇欲產生的座位表模式:";
	unsigned short choice = 0, column = 0, raw = 0;
	cin >> choice;
	cin.get();
	if (cin.fail())
	{
		release_cinerr();
		cout << "[End] ----------------------------------------------\n\n\n\n\n";
		return;
	}

	cout << "\n單號教室:\n";
	set_column_and_raw(std_list_odd.size(), column, raw);
	output_seating(ofname_1, std_list_odd, choice, column, raw);

	cout << "\n雙號教室:\n";
	set_column_and_raw(std_list_odd.size(), column, raw);
	output_seating(ofname_2, std_list_even, choice, column, raw);
	cout << "[End] ----------------------------------------------\n\n\n\n\n";

	std_list_odd.clear();
	std_list_even.clear();
}

void direct()
{
	string classname, examname;
	cout << "請輸入開課系級(可按下enter忽略):";
	classname = get_classname();

	cout << "請輸入考試名稱(可按下enter忽略):";
	examname = get_examname();
	string ofname = "#" + classname + "#" + examname + "_" + "單一班";

	cout << "\n[Msg] 後續將產生之檔案名稱: " << ofname + "_清單.csv\n";

	cout << "\n[Msg] ※輸入或貼上格式錯誤將導致程式直接中止!";
	cout << "\n請輸入或貼上學生清單(學號 姓名)\n(輸入完畢後，再在新行輸入0開始排位。):\n";

	vector<Student> std_list;
	std_list.reserve(100);
	string name;
	Student std_rec;
	cin >> std_rec.stdid;
	while (cin.fail())
	{
		release_cinerr();
		cout << "[Err] 輸入非法的學號! 請重新輸入:";
		cin >> std_rec.stdid;
	}

	while (std_rec.stdid != 0)
	{
		cin >> name;
		std_rec.name = name;
		std_list.push_back(std_rec);

		cin >> std_rec.stdid;
		while (cin.fail())
		{
			release_cinerr();
			cout << "[Err] 輸入非法的學號! 請重新輸入:";
			cin >> std_rec.stdid;
		}
	}

	if (std_list.size() == 0)
	{
		cout << "[Err] 輸入資料為空! 結束本階段程序。\n";
		cout << "[End] ----------------------------------------------\n\n\n\n\n";
	}

	srand(time(NULL));
	unsigned seed = rand();
	shuffle(begin(std_list), end(std_list), default_random_engine(seed));

	// write file

	outputfile(ofname, std_list, true);
	cout << "[End] ----------------------------------------------\n\n\n\n\n";
	cout << "**************************\n"
		<< "1.標準直方\n"
		<< "2.間隔直方\n"
		<< "3.梅花座\n"
		<< "any. 不產生/退回清單生成\n"
		<< "**************************\n";
	cout << "請選擇欲產生的座位表模式:";
	unsigned short choice = 0, column = 0, raw = 0;
	cin >> choice;
	cin.get();

	if (cin.fail())
	{
		release_cinerr();
		cout << "[End] ----------------------------------------------\n\n\n\n\n";
		return;
	}

	set_column_and_raw(std_list.size(), column, raw);

	output_seating(ofname, std_list, choice, column, raw);
	cout << "[End] ----------------------------------------------\n\n\n\n\n";
	std_list.clear();
}
