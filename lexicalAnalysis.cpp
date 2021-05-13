#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include<fstream>
#include<iomanip>

using namespace std;

// �ַ�����Ŀ
int charCount = 0;

// �ո���Ŀ
int spaceCount = 0;

// ������¼
int rowCount = 1;

// ������Ŀ��¼
int errorCount = 0;

// ״̬
int state = 0;

// ��ǰ��ȡ�ַ�
char thisChar;

// ���˱�־
bool isRetract = false;

// token
string token = "";

// �����ֱ�
vector<pair<string, string>> reservedTable;

// ���ű�
set<string> symbolTable;

// ������Ŀ��¼��
map<string, int> wordsCount;

// �����кż�¼
set<int> errorRowCount;

// ���ַ�������token��
void cat(const char& ch) {
	token += ch;
}

// ��ȡһ���ַ���ͬʱ�����ַ��������������ո���Ŀ
char getAChar() {
	char ch = getchar();
	if (ch != '\n' && ch != '\t' && ch != ' ' && ch != EOF) ++charCount;
	if (ch == '\n') ++rowCount;
	if (ch == ' ') ++spaceCount;
	return ch;
}

// ���û���
void retract() {
	isRetract = true;
	return;
}

// �ж��Ƿ�Ϊ��ĸ�����»���
bool isLetter(const char& ch) {
	if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))
		return true;
	else return false;
}

// �ж��Ƿ�Ϊ����
bool isDigit(const char& ch) {
	if ('0' <= ch && ch <= '9')
		return true;
	else return false;
}

// ����Ƿ��Ǳ�����
string isReserved(const string& token) {
	for (auto str : reservedTable) {
		if (str.first == token)
			return str.second;
	}
	return "";
}

// ��������¼������������ֱ���������ַ�
void error() {
	++errorCount;
	errorRowCount.insert(rowCount);
	return;
}

// ���������ļ�
ofstream fOut1("result1.txt", ios::out);
ofstream fOut2("result2.txt", ios::out);

// ������� 
void pushResult(const string& str, string attribute = "����") {
	fOut1 << "( " << rowCount << " )       " << setw(15) << left << str << "   " << attribute << endl;
	fOut2 << str << "\t" << rowCount << endl;
}


int main() {
	// ���뷭���
	ifstream fReservedTable("reservedTable.txt", ios::in);
	string t1, t2;
	if (!fReservedTable.is_open())
		cout << "�޷����뷭�����" << endl;
	while (!fReservedTable.eof()) {
		fReservedTable >> t1 >> t2;
		reservedTable.push_back(make_pair(t1, t2));
	}
	fReservedTable.close();

	// �򿪲����ļ�
	if (freopen("test1.txt", "r", stdin) == NULL) {
		cout << "�޷��򿪲����ļ�����" << endl;
		return 0;
	}

	fOut1 << "�ʷ����������\n" << endl;
	fOut1 << "������    �Ǻ�            ����" << endl;
	do {
		switch (state) {
		case 0: {
			token = "";
			// ��ȡ��һ���ַ�
			if (isRetract) {
				isRetract = false;
			}
			else thisChar = getAChar();
			while (thisChar == ' ' || thisChar == '\t' || thisChar == '\n') {
				thisChar = getAChar();
			}

			// �ж�thisChar
			if (isLetter(thisChar) || thisChar == '_') {
				state = 1;
			}
			else if ('1' <= thisChar && thisChar <= '9') {
				state = 2;
			}
			else if (thisChar == '0') {
				state = 8;
			}
			else if (thisChar == '=') {
				state = 12;
			}
			else if (thisChar == '<') {
				state = 13;
			}
			else if (thisChar == '>') {
				state = 14;
			}
			else if (thisChar == '&') {
				state = 15;
			}
			else if (thisChar == '|') {
				state = 16;
			}
			else if (thisChar == '/') {
				state = 17;
			}
			else if (thisChar == '!' || thisChar == ':' || thisChar == '.' || thisChar == ',' || thisChar == ';' || thisChar == '('
				|| thisChar == ')' || thisChar == '[' || thisChar == ']' || thisChar == '{' || thisChar == '}') {
				string temp = "";
				temp += thisChar;
				++wordsCount[temp];
				pushResult(temp);
				state = 0;
			}
			else if (thisChar == '\'') {
				string temp = "";
				thisChar = getAChar();
				temp += thisChar;
				// �ж�\n��\t�����
				if (thisChar == '\\') {
					thisChar = getAChar();
					temp += thisChar;
					thisChar = getAChar();
					if (thisChar == '\'') {
						++wordsCount["word"];
						pushResult("word", temp);
					}
					else {
						error();
						state = 0;
					}
				}
				else {// �ж�char��
					thisChar = getAChar();
					if (thisChar == '\'') {
						++wordsCount["word"];
						if (temp == " ")
							pushResult("word", "space");
						else
							pushResult("word", temp);
					}
					else {
						error();
						state = 0;
					}
				}
			}
			else if (thisChar == '\"') {
				string temp = "";
				while (1) {
					thisChar = getAChar();
					if (thisChar == EOF || thisChar == '\n') {
						error();
						state = 0;
						break;
					}
					else if (thisChar == '\"') {
						++wordsCount["str"];
						pushResult("str", temp);
						state = 0;
						break;
					}
					temp += thisChar;
				}
			}
			else if (thisChar == '+' || thisChar == '-' || thisChar == '*') {
				string temp = "";
				temp += thisChar;
				++wordsCount[temp];
				pushResult(isReserved(temp), temp);
				state = 0;
			}
			else {
				state = 18;
			}
			break;
		}
		case 1: {
			cat(thisChar);
			thisChar = getAChar();
			if (isLetter(thisChar) || isDigit(thisChar) || thisChar == '_') state = 1;
			else {
				// ����һ��
				retract();
				// �ص�״̬0
				state = 0;
				string signal = isReserved(token);
				if (signal != "") {// ������
					++wordsCount[token];
					pushResult(signal);
				}
				else {// �Ǳ�����
					++wordsCount["id"];
					auto ptr = symbolTable.find(token);
					if (ptr == symbolTable.end()) {
						symbolTable.insert(token);
					}
					pushResult(isReserved("id"), token);
				}
			}
			break;
		}

		case 2: {
			cat(thisChar);
			thisChar = getAChar();
			if (isDigit(thisChar)) state = 2;
			else if (thisChar == 'E') state = 5;
			else if (thisChar == '.') state = 3;
			else {
				++wordsCount["num"];
				retract();
				state = 0;
				pushResult("num", token);
			}
			break;
		}

		case 3: {
			cat(thisChar);
			thisChar = getAChar();
			if (isDigit(thisChar)) state = 4;
			else {
				error();
				state = 0;
			}
			break;
		}

		case 4: {
			cat(thisChar);
			thisChar = getAChar();
			if (isDigit(thisChar)) state = 4;
			else if (thisChar == 'E') state = 5;
			else {
				++wordsCount["num"];
				retract();
				state = 0;
				pushResult("num", token);
			}
			break;
		}

		case 5: {
			cat(thisChar);
			thisChar = getAChar();
			if (isDigit(thisChar)) state = 7;
			else if (thisChar == '+' || thisChar == '-') state = 6;
			else {
				error();
				state = 0;
			}
			break;
		}

		case 6: {
			cat(thisChar);
			thisChar = getAChar();
			if (isDigit(thisChar)) state = 7;
			else {
				error();
				state = 0;
			}
			break;
		}

		case 7: {
			cat(thisChar);
			thisChar = getAChar();
			if (isDigit(thisChar)) state = 7;
			else {
				++wordsCount["num"];
				retract();
				state = 0;
				pushResult("num", token);
			}
			break;
		}

		case 8: {
			cat(thisChar);
			thisChar = getAChar();
			if (thisChar == '.') state = 3;
			else if ('0' <= thisChar && thisChar <= '7') state = 9;
			else if (thisChar == 'x' || thisChar == 'X') state = 10;
			else if ('8' <= thisChar && thisChar <= '9') { error(); state = 0; }
			else {
				++wordsCount["num"];
				retract();
				state = 0;
				pushResult("num", token);
			}
			break;
		}

		case 9: {
			cat(thisChar);
			thisChar = getAChar();
			if ('0' <= thisChar && thisChar <= '7') state = 9;
			else {
				++wordsCount["num"];
				retract();
				state = 0;
				pushResult("num", token);
			}
			break;
		}

		case 10: {
			cat(thisChar);
			thisChar = getAChar();
			if (('a' <= thisChar && thisChar <= 'f') || ('A' <= thisChar && thisChar <= 'F')
				|| isDigit(thisChar)) state = 11;
			else {
				error();
				state = 0;
			}
			break;
		}

		case 11: {
			cat(thisChar);
			thisChar = getAChar();
			if (('a' <= thisChar && thisChar <= 'f') || ('A' <= thisChar && thisChar <= 'F')
				|| isDigit(thisChar)) state = 11;
			else {
				++wordsCount["num"];
				retract();
				state = 0;
				pushResult("num", token);
			}
			break;
		}

		case 12: {
			cat(thisChar);
			thisChar = getAChar();
			if (thisChar == '=') {
				++wordsCount["=="];
				cat(thisChar);
				pushResult(isReserved("=="), token);
			}
			else {
				++wordsCount["="];
				pushResult(isReserved("="));
				retract();
			}
			state = 0;
			break;
		}

		case 13: {
			cat(thisChar);
			thisChar = getAChar();
			if (thisChar == '=') {
				++wordsCount["<="];
				cat(thisChar);
				pushResult(isReserved("<="), token);
			}
			else if (thisChar == '>') {
				++wordsCount["<>"];
				cat(thisChar);
				pushResult(isReserved("<>"), token);
			}
			else {
				++wordsCount["<"];
				pushResult(isReserved("<"), token);
				retract();
			}
			state = 0;
			break;
		}

		case 14: {
			cat(thisChar);
			thisChar = getAChar();
			if (thisChar == '=') {
				++wordsCount[">="];
				cat(thisChar);
				pushResult(isReserved(">="), token);
			}
			else {
				++wordsCount[">"];
				pushResult(isReserved(">"), token);
				retract();
			}
			state = 0;
			break;
		}

		case 15: {
			cat(thisChar);
			thisChar = getAChar();
			if (thisChar == '&') {
				++wordsCount["&&"];
				cat(thisChar);
				pushResult(isReserved("&&"), token);
			}
			else {
				++wordsCount["&"];
				pushResult(isReserved("&"), token);
				retract();
			}
			state = 0;
			break;
		}

		case 16: {
			cat(thisChar);
			thisChar = getAChar();
			if (thisChar == '|') {
				++wordsCount["||"];
				cat(thisChar);
				pushResult(isReserved("||"), token);
			}
			else {
				++wordsCount["|"];
				pushResult(isReserved("|"), token);
				retract();
			}
			state = 0;
			break;
		}

		case 17: {
			cat(thisChar);
			thisChar = getAChar();
			// ��������ע�ͷ�(//)
			if (thisChar == '/') {
				while (thisChar != '\n' && thisChar != EOF) {

					thisChar = getAChar();
				}
			}
			else if (thisChar == '*') {// ������ע�ͷ�(/*)
				while (1) {
					thisChar = getAChar();
					if (thisChar == EOF) {
						error();
						break;
					}
					else if (thisChar == '*') {
						thisChar = getAChar();
						if (thisChar == '/') {
							break;
						}
					}
				}
			}
			else {
				++wordsCount["/"];
				pushResult(isReserved("/"), token);
				retract();
			}
			state = 0;
			break;
		}

		case 18: {
			error();
			state = 0;
			break;
		}

		default: {
			error();
			state = 0;
			break;
		}
		}
	} while (thisChar != EOF);
	fclose(stdin);
	cout << "          �ʷ�������������        " << endl;
	cout << "----------------------------------" << endl;
	// ͳ���������
	cout << "ͳ�����������" << endl;
	cout << "������" << rowCount << endl;
	cout << "�ַ�������" << charCount << endl;
	cout << "�ո���Ŀ��" << spaceCount << endl;

	// ���ű����
	cout << "----------------------------------" << endl;
	cout << "���ű�" << endl;
	for (auto t : symbolTable) {
		cout << " (  " << setw(10) << left << t << "  ) " << endl;
	}
	// ������Ŀ��¼�����
	cout << "----------------------------------" << endl;
	cout << "������Ŀ��¼(���ʣ����ִ���)!" << endl;
	for (auto t : wordsCount) {
		cout << "  " << setw(20) << left << t.first << t.second << endl;
	}
	// �����¼���
	cout << "----------------------------------" << endl;
	cout << "�����¼" << endl;
	cout << "����������" << errorCount << endl;
	cout << "����������";
	for (auto t : errorRowCount) {
		cout << t << " ";
	}
	cout << endl;


	return 0;
}
