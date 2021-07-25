//txtparser.h
#ifndef DSL_TXTPARSER_H
#define DSL_TXTPARSER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "stdlib.h"
//#include "Cards.h"

using namespace std;

std::vector<std::string> split(std::string &s, char delimeter){
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, delimeter))
       tokens.push_back(item);
    return tokens;
}

// ������� ������� ���������� ������ �����, �� ���������� ������ � �����������
// �������� �� ����� ���� ��� �������
/*
std::vector<std::string> split(std::string & s, char delimeter){
	// ������� - ���� s="HEROES:195-210", �� � ����� �������� � 0 �� 210
	int x = s.find(":"); 
	s.erase(0, x+1);
	///////////////////////////////////
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, delimeter)){
       tokens.push_back(item);
    }
    return tokens;
}

void set_type_card(std::vector<int> vec, int type){
	for(int i=0; i<vec.size(); i++){
		if(vec[i] < cards.size() ){
			cards[vec[i]]->setType(type);
		}	
	}
}
void set_type_card_all(int type){
	for(int i=0; i<cards.size(); i++)
		cards[i]->setType(type);
}
void set_rare_card(std::vector<int> vec, int rare){
	for(int i=0; i<vec.size(); i++){
		if(vec[i] < cards.size() ) // �������������� ������
			cards[vec[i]]->setRare(rare);
	}	
}
void set_rare_card_all(int rare){	
	for(int i=0; i<cards.size(); i++)
		cards[i]->setRare(rare);
}
void set_elem_card(std::vector<int> vec, int elem){
	for(int i=0; i<vec.size(); i++){
		if(vec[i] < cards.size() ) // �������������� ������
			cards[vec[i]]->setElem(elem);
	}
}
void set_elem_card_all(int elem){
	for(int i=0; i<cards.size(); i++)
		cards[i]->setElem(elem);
}
*/

std::vector< std::vector<std::string> > get_strings_obj() 
{
	std::vector< std::vector<std::string> > vec;
	std::string line;
    std::ifstream in("src/obj_info.txt");
    if(in.is_open())
        while(getline(in, line))
            vec.push_back(split(line, ' '));
    in.close();
	return vec;
}

class MyParser
{
	std::vector<char> vecChar;
	std::vector<std::string> vecWords;

	ifstream fin;
	char buff[50];
	char c;
	char* ptrChar; // ???????? ?? ????????
public:
	MyParser() { }
	
	void openFile(const std::string& f)
	{
		fin.open("info_cards.txt");
	}
	void closeFile()
	{
		fin.close();
	}
	// ������� 1 ������
	void readChar()
	{
		// ���� �� ����� �����
		if( !fin.eof() ){
			// ���������� ��������� ������ � <�>
			fin.read(&c, 1);
			// ��������� ������ � ������
			vecChar.push_back(c);
		} else {
			// ��������� ���� �.�. ��� �����
			fin.close();
		}
	}
	// ������� ��� ���������� �������
	void readCharAll(){
		while( !fin.eof()){
			fin.read(&c, 1);
			vecChar.push_back(c);
		}
		fin.close();
	}
	// ??????? ?????????? ??????? ? ???????
	void coutVector(){
		for(std::vector<char>::iterator it = vecChar.begin(); it != vecChar.end(); it++)
			cout << *it;
		cout << endl;
	}
	// ??????? ????? ?? ?????
	void readWord()
	{
		fin >> buff;
		coutBuffer();
	}
	// ??????? ?????? ?? ?????
	void readLine() 
	{
		fin.getline(buff, 50);
	}
	// ??????? ?????????? ?????? ? ???????
	void coutBuffer()
	{
		cout << "buffer = <" << buff << ">" << endl;
	}
	
	void readXXX()
	{
		string predlojenie;
		bool flag = true;
		int n = 0;
		for(int i=0; i<vecChar.size(); i++) {
			if( flag )
			{
				buff[i] = vecChar[i];
				if( vecChar[i] == '!' || vecChar[i] == '?' /* . , ... */ )
				{
					for(int j=n; j<=i; j++)
						predlojenie.push_back(vecChar[j]);
					cout << predlojenie.c_str() << endl;
					predlojenie.clear();
					flag = false;
					n = i+1;
				}
			} else {
				if(vecChar[i] != ' ') {
					i--;
					flag = true;
				} else
					n++;
			}
		}
	}
	void readXXY()
	{
		std::string word;
		for(int i=0; i<vecChar.size(); i++) {
			if( vecChar[i] == '!' || vecChar[i] == '?' || vecChar[i] == ' ' || vecChar[i] == '\n') {
				if(word.size() != 0) {
					vecWords.push_back( word );
					word.clear();
				}
			} else {
				// ????????? ? ????? ?????
				word.push_back( vecChar[i] );
			}
		}
		//coutWords();
	}
	void coutWords()
	{
		for(int i=0; i<vecWords.size(); i++)
			cout << vecWords[i].c_str() << ' ';
	}
};

struct parser_JamProject 
{
	bool debug;
	ifstream f;
	std::vector<std::string> strings; // ������
	std::vector<char> chars; // �������
	std::vector< std::vector<int> > numbers;
	//=======================================
	
	parser_JamProject() : debug(false) { }
	
	void start(){
		if(debug) cout << "parser_JamProject.start() BEGIN" << endl;
		loadAndFileToChars(); // ����������� .txt -> vector<char> chars;
		charsToStrings(); // vector<char> chars -> vector<string> strings;
		for(int i=0; i<strings.size(); ++i){
			std::vector<int> vec = getNumbersFromString(strings[i]);
			numbers.push_back(vec);
		}
		if(debug) cout << "parser_JamProject.start() END" << endl;
	}
	
	void loadAndFileToChars()
	{
		if(debug) cout << "parser_JamProject.loadAndFileToChars() BEGIN" << endl;
		char c; // ������
		f.open("src/map1_info.txt");
		while( !f.eof()){
			f.read(&c, 1);
			chars.push_back(c);
		}
		chars.push_back('\n'); // �������
		f.close();
		if(debug) cout << "parser_JamProject.loadAndFileToChars() END" << endl;
	}
	
	void charsToStrings()
	{
		if(debug) cout << "parser_JamProject.charsToStrings() BEGIN" << endl;
		std::string str;
		for(int i=0; i<chars.size(); i++){
			// ���� ������ ����� ������
			if(chars[i] == '\n'){
				strings.push_back(str);
				str.clear();
			} else {
				str += chars[i];
			}
		}
		if(debug) cout << "parser_JamProject.charsToStrings() END" << endl;
	}
	
	std::vector<int> getNumbersFromString(std::string str)
	{
		if(debug) cout << "parser_JamProject.getNumbersFromString() BEGIN" << endl;
		std::vector<int> numbers;
		std::vector<std::string> vec_str = split(str, ' ');
		for(int i=0; i<vec_str.size(); ++i){
			if(debug) cout << vec_str[i] << '_';
			numbers.push_back( atoi(vec_str[i].c_str()) );
		}
			
		if(debug) cout << "parser_JamProject.getNumbersFromString() END" << endl;
		return numbers;
	}
	/*
	std::vector<int> getIdsFromString(std::string word)
	{
		int num = search(word);
		std::vector<int> ids;
		std::vector<std::string> tokens = split(strings[num], ',');
		//cout << "tokens.size()=" << tokens.size() << " ,";
		for(int i=0; i<tokens.size(); i++){
			int x = tokens[i].find('-');
			// ���� ���� ��������� ������� '-' � ������
			if( -1 < x && x < 1000 ){
				// x - ����� �������, ��� ���������� '-'
				int begin = atoi( tokens[i].substr(0, x).c_str() ); 
				int end   = atoi( tokens[i].substr(x+1, tokens[i].size()).c_str() );
				//cout << "x=" << x << ", begin=" << begin <<", end=" << end << endl;
				for(int j=begin; j<=end; j++)
					ids.push_back(j-1);
			} else {
				ids.push_back( atoi(tokens[i].c_str())-1 );
			}
		}
		/////////////////////////////////////////////////////////////////////
		//cout << word.c_str() << " ";
		//for(int i=0; i<ids.size(); i++) cout << ids[i] << " ";
		//cout << endl;
		/////////////////////////////////////////////////////////////////////
		return ids;
	}
	*/
	// �������� ����� ������ � ������� ���������� ����� ���������
	int search(std::string str)
	{
		for(int i=0; i<strings.size();i++){
			int x = strings[i].find(str);
			if( -1 < x && x < 1000 )
				return i;
		}
		return -1;
	}

	void cout_chars()
	{	
		if(debug) cout << "parser_JamProject.cout_chars() BEGIN" << endl;
		for(int i=0; i<chars.size(); i++)
			cout << chars[i];
		cout << endl;
		if(debug) cout << "parser_JamProject.cout_chars() END" << endl;
	}
	
	void cout_strings()
	{
		if(debug) cout << "parser_JamProject.cout_strings() BEGIN" << endl;
		for(int i=0; i<strings.size(); i++)
			cout << strings[i].c_str() << endl;
		cout << endl;
		if(debug) cout << "parser_JamProject.cout_strings() END" << endl;
	}
};
/*
struct parserBerserk{
	ifstream f;
	std::vector<std::string> strings; // ������
	std::vector<char> chars; // �������
	/////////////////////////
	parserBerserk(){
		loadAndFileToChars(); // ����������� .txt -> vector<char> chars;
		//cout_chars();
		charsToStrings(); // vector<char> chars -> vector<string> strings;
		//cout_strings();
		//////////////////////////////////////////////////////////////
		set_type_card_all(UNIT); // ������� ����� - ��������
		set_type_card( getIdsFromString("WEAPONS:"), WEAPON  );
		set_type_card( getIdsFromString("TERRAIN:"), TERRAIN );
		set_type_card( getIdsFromString("EVENTS:" ), EVENT   );
		set_type_card( getIdsFromString("HEROES:" ), HERO    );
		set_type_card( getIdsFromString("SKILLS:" ), SKILL   );
		//////////////////////////////////////////////////////////////
		set_rare_card_all(COMMON); // - ������� ����� - ������
		set_rare_card( getIdsFromString("UNUSUAL:"), UNUSUAL );
		set_rare_card( getIdsFromString("RARE:"   ), RARE    );
		set_rare_card( getIdsFromString("ULTRA:"  ), ULTRA   );
		//////////////////////////////////////////////////////////////
		set_elem_card_all(NEUTRALS); // ������� ����� - �����������
		set_elem_card( getIdsFromString("STEPPES:"  ), STEPPES  );
		set_elem_card( getIdsFromString("MOUNTAINS:"), MOUNTAINS);
		set_elem_card( getIdsFromString("WOODS:"    ), WOODS    );
		set_elem_card( getIdsFromString("SWAMPS:"   ), SWAMPS   );
		set_elem_card( getIdsFromString("DARK:"     ), DARK     );
		//////////////////////////////////////////////////////////////
	}
	std::vector<int> getIdsFromString(std::string word){
		int num = search(word);
		std::vector<int> ids;
		std::vector<std::string> tokens = split(strings[num], ',');
		//cout << "tokens.size()=" << tokens.size() << " ,";
		for(int i=0; i<tokens.size(); i++){
			int x = tokens[i].find('-');
			// ���� ���� ��������� ������� '-' � ������
			if( -1 < x && x < 1000 ){
				// x - ����� �������, ��� ���������� '-'
				int begin = atoi( tokens[i].substr(0, x).c_str() ); 
				int end   = atoi( tokens[i].substr(x+1, tokens[i].size()).c_str() );
				//cout << "x=" << x << ", begin=" << begin <<", end=" << end << endl;
				for(int j=begin; j<=end; j++)
					ids.push_back(j-1);
			} else {
				ids.push_back( atoi(tokens[i].c_str())-1 );
			}
		}
		/////////////////////////////////////////////////////////////////////
		//cout << word.c_str() << " ";
		//for(int i=0; i<ids.size(); i++) cout << ids[i] << " ";
		//cout << endl;
		/////////////////////////////////////////////////////////////////////
		return ids;
	}
	// �������� ����� ������ � ������� ���������� ����� ���������
	int search(std::string str){
		for(int i=0; i<strings.size();i++){
			int x = strings[i].find(str);
			if( -1 < x && x < 1000 )
				return i;
		}
		return -1;
	}
	void charsToStrings(){
		std::string str;
		for(int i=0; i<chars.size(); i++){
			// ���� ������ ����� ������
			if(chars[i] == '\n'){
				strings.push_back(str);
				str.clear();
			} else {
				str += chars[i];
			}
		}
	}
	void cout_chars(){
		for(int i=0; i<chars.size(); i++)
			cout << chars[i];
		cout << endl;
	}
	void cout_strings(){
		for(int i=0; i<strings.size(); i++)
			cout << strings[i].c_str() << endl;
		cout << endl;
	}
	void loadAndFileToChars(){
		char c; // ������
		f.open("info_cards.txt");
		while( !f.eof()){
			f.read(&c, 1);
			chars.push_back(c);
		}
		chars.push_back('\n'); // �������
		f.close();
	}
};
// ������������ ����� load_textures()
void load7(){
	parserBerserk p;
}
*/
#endif //DSL_TXT_PARSER_H
