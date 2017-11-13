//Dalton Koch
//Thomas Seabolt
//ECE 3220: Software Design in C/C++
//Final Project

/*
NOTICE:

Please read the Banking App 1st release word document before running the demo
The last paragraph in the document includes some instructions for running the program
The Account Text files are also included in the zip file, these are necessary for the demo to run

this current iteration only accpets the expected input type, this is ann issue we are working on

The structure of the account text files is:

Account type (Char)
User Id (int)
Account number (Long int)
Account Password (string)
Balance (float)
Frozen (Bool)



*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

//returns type of account
char user_acct(long acct_num){
	char temp;
	ifstream myfile;
	string in;
	char filename[30];
	int n = sprintf(filename,"%li.txt",acct_num);
	myfile.open(filename);
	if(!myfile.is_open()){
		return 'F';
	}
	else{
		myfile >> temp;
		myfile.close();
		return temp;
	}
}

string pass_encrypt(string password) {
	for (auto &c : password) {
		if (islower(c)) {
			c = toupper(c);
		}
		else if (isupper(c)) {
			c = tolower(c);
		}
	}
	for (auto &c : password) {
		if (c == 'z') {
			c -= 1;
		}
		else if (c < 'z') {
			c += 1;
		}
	}
	return password;
}

//ABC base class
class Bank_Account {
	private:
		
	protected:
		int userID = 0;
		long Account_Num;
		double balance;
		string Password;
		char Acct_type;
		
	public:
		bool frozen;
		Bank_Account(); //constructor for new account
		Bank_Account(long acct_num); //parametric constructor for existing account
		virtual void deposit();
		virtual void withdraw();
		virtual void check_balance();
		//void view_translog();
		void close_Acct();
		void print_to_file();
		int does_exist();
		//transfer money function
};

Bank_Account::Bank_Account(){
	string in;
	int i = 0;
	cout << "Thank You for selecting to make an account with Bank inc." << endl;
	cout << "First we need to gather some information from you" << endl;
	cout << "What 8-digit acccount number would you like to have?" << endl;
	while(i==0){
		cin >> Account_Num;
		cin.ignore();
		//sscanf(in.c_str(),"%08li", Account_Num);
		if(Account_Num > 99999999 || Account_Num <= 0){
			cout << "Invalid choice of Account Number! Must be 8-digits or less. Please try again:" << endl;
		}
		else{
			i = 1;
		}
	}
	cout << "Enter your userID:" << endl;
	cin >> userID;
	cin.ignore();
	//sscanf(in.c_str(),"%d", userID);
	cout << "Enter your password you would like to have for this account:" << endl;
	cin >> Password;
	cin.ignore();
	//sscanf(in.c_str(),"%s", Password);
	frozen = 0;
	cout << "Your starting balance will be $1000!" << endl;
	balance = 1000.00;
	
}

Bank_Account::Bank_Account(long acct_num){
	
	ifstream myfile;
	char filename[30];
	string in;
	sprintf(filename,"%li.txt",acct_num);
	myfile.open(filename);
	if(!myfile.is_open()){
		cout << "This Account does not exist." << endl;
		return;
		cout << "test Here" << endl;
	}
	myfile >> Acct_type;
	myfile >> userID;
	myfile >> Account_Num;
	myfile >> Password;
	myfile >> balance;
	myfile >> frozen;
	myfile.close();
}

void Bank_Account::check_balance(){
	cout << "Your current baland is: $" << balance << endl;
}

void Bank_Account::print_to_file(){
	char filename[30];
	ofstream myfile;
	sprintf(filename,"%08li.txt",Account_Num);
	myfile.open(filename,ios::out);
	myfile << Acct_type;
	myfile << char(13) << char(10);
	myfile << userID;
	myfile << char(13) << char(10);
	myfile << Account_Num;
	myfile << char(13) << char(10);
	myfile << Password;
	myfile << char(13) << char(10);
	myfile << balance;
	myfile << char(13) << char(10);
	myfile << frozen;
	myfile << char(13) << char(10);
	myfile.close();
}

int Bank_Account::does_exist(){
	if(userID == 0){
		//account does not exist
		return 0;
	}
	else{
		return 1;
	}
}

void Bank_Account::deposit() {
	double amount;
	//string amountStr;
	cout << "How much would you like to deposit?" << endl;
	cin >> amount;
	cin.ignore();
	//sscanf(amountStr.c_str(), "%.2lf", amount);
	cout << amount << endl;
	cout << balance << endl;
	balance += amount;
	cout << balance << endl;
	
	this->print_to_file();
}

void Bank_Account::withdraw() {
	double amount;
	//string amountStr;
	cout << "How much would you like to withdraw?" << endl;
	cin >> amount;
	cin.ignore();
	//sscanf(amountStr.c_str(), "%.2lf", amount);
	if (balance - amount > 0) {
		balance -= amount;
		cout << "$" << amount << " withdrawn from account successfully";
	}
	else {
		cout << "Insufficient funds for requested withdrawal.";
	}
	this->print_to_file();
}

void Bank_Account::close_Acct(){
	balance = 0;
	Acct_type = 'F';
	userID = 0;
	cout << "Accout: " << Account_Num << " has now been closed" << endl;
	this->print_to_file();
}



//derived class from ABC base class
class Checking_Acct : public Bank_Account {
	private:
		
	public:
		Checking_Acct();
		Checking_Acct(long acct_num);
		//void check_balance();
		//pay bill function??????????
};

Checking_Acct::Checking_Acct() : Bank_Account(){
	Acct_type = 'C';
}

Checking_Acct::Checking_Acct(long acct_num) : Bank_Account(acct_num) {
	//calls base constructor only so far
}

//derived class
class Saving_Acct : public Bank_Account {
	private:
		float Interest_Rate;
		
	public:
		Saving_Acct();
		Saving_Acct(long acct_num);
		void deposit();
		void withdraw();
		void check_balance();
		void calc_Predicted_Interest();
		void Get_rate();
};

//default value of -1 for account number, so we only need one svaing constructor
Saving_Acct::Saving_Acct(long acct_num) : Bank_Account(acct_num){
	this->Get_rate();
}

Saving_Acct::Saving_Acct() : Bank_Account(){
	Acct_type = 'S';
	this->Get_rate();
	cout << "The Interest Rate set for this account is: " << Interest_Rate*100 << "%" << endl;
}

void Saving_Acct::deposit() {
	double amount;
	//string amountStr;
	cout << "How much would you like to deposit?" << endl;
	cin >> amount;
	cin.ignore();
	//sscanf(amountStr.c_str(), "%.2lf", amount);
	
	balance += amount;
	cout << "$" << amount << " deposited to account successfully" << endl;
	this->Get_rate();
	this->print_to_file();
}

void Saving_Acct::withdraw() {
	double amount;
	//string amountStr;
	cout << "How much would you like to withdraw?" << endl;
	cin >> amount;
	cin.ignore();
	//sscanf(amountStr.c_str(), "%.2lf", amount);
	if (balance - amount >= 0){
		balance -= amount;
		cout << "$" << amount << " withdrawn from account successfully" << endl;
		this->Get_rate();
		this->print_to_file();
	}
	else {
		cout << "Insufficient funds for requested withdrawal." << endl;
	}
	
}

void Saving_Acct::check_balance(){
	cout << "Your current balance is: $" << balance << endl;
	cout << "The Interest Rate of this account is:" << Interest_Rate*100 << "%" << endl;
}

void Saving_Acct::Get_rate(){
	int i =0;
	float rate;
	int range;
	vector<float>rates;
	vector<int>ranges;
	string filename = "Rates.txt";
	ifstream file;
	file.open(filename);
	if(!file.is_open()){
		//checks to see if file was opened
		cout << "error Rates.txt file not opened" << endl;
	}
	for(i=0;i<4;i++){
		file >> range;
		file >> rate;
		rates.push_back(rate);
		ranges.push_back(range);
	}
	file.close();
	if (balance>=ranges[3]) {
		Interest_Rate = rates[3];
	}
	else if (balance>=ranges[2]) {
		Interest_Rate = rates[2];
	}
	else if (balance>=ranges[1]) {
		Interest_Rate = rates[1];
	}
	else if (balance>=ranges[0]) {
		Interest_Rate = rates[0];
	}
	else {
		Interest_Rate = 0.00;
	}
}

void Saving_Acct::calc_Predicted_Interest(){
	double temp;
	int t;
	string in;
	cout << "For how many years would you like to see predicted interest earned on this account? Please neter the number of years:" << endl;
	cin >> t;
	cin.ignore();
	//sscanf(in.c_str(),"%d",t);
	temp = balance* pow(1+Interest_Rate,t);
	cout << "Your expected account balance after " << t << " years is: $" << temp << endl;
}

class Manager_Acct {
	private:
		string Password;
		int empl_num;
	public:
		Manager_Acct();
		int manager_login(long acct_num);
		void freeze(long Account_Num);
		void unfreeze(long Account_Num);
		void Current_Rates();
		void Adjust_Rate();
};

Manager_Acct::Manager_Acct(){
	empl_num = 12345678;
	Password = pass_encrypt("Password123");
}

int Manager_Acct::manager_login(long acct_num){
	string pass;
	if(acct_num == empl_num){
		cout << "please enter your password..." << endl;
		getline(cin,pass);
		if(pass_encrypt(pass) == Password){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
	return 0;
}

void Manager_Acct::freeze(long Account_Num) {
	Bank_Account temp(Account_Num);
	if(temp.does_exist() == 0){
		cout << "Account: " << Account_Num << " does not exist. Account cannot be frozen" << endl;
	}
	else{
		temp.frozen = 1;
		temp.print_to_file();
	}
}

void Manager_Acct::unfreeze(long Account_Num) {
	Bank_Account temp(Account_Num);
	if(temp.does_exist() == 0){
		cout << "Account: " << Account_Num << " does not exist. Account cannot be frozen" << endl;
	}
	else{
		temp.frozen = 0;
		temp.print_to_file();
	}
}

void Manager_Acct::Current_Rates(){
	int i = 0;
	float rate;
	int range;
	vector<float>rates;
	vector<int>ranges;
	string filename = "Rates.txt", in;
	ifstream file;
	file.open(filename);
	if(!file.is_open()){
		//checks to see if file was opened
		cout << "error Rates.txt file not opened" << endl;
	}
	for(i=0;i<4;i++){
		file >> range;
		file >> rate;
		rates.push_back(rate);
		ranges.push_back(range);
	}
	file.close();
	for(i=0;i<4;i++){
		cout << "For accounts with a balance > $" << ranges[i] << " the rate is:" << rates[i]*100 << "%" << endl;
	}
}

void Manager_Acct::Adjust_Rate(){
	string in;
	int i = 0;
	float rate;
	int range;
	char choice;
	float temp;
	vector<float>rates;
	vector<int>ranges;
	string filename = "Rates.txt";
	ifstream file;
	file.open(filename);
	if(!file.is_open()){
		//checks to see if file was opened
		cout << "error Rates.txt file not opened" << endl;
	}
	for(i=0;i<4;i++){
		file >> range;
		file >> rate;
		rates.push_back(rate);
		ranges.push_back(range);
	}
	file.close();
	cout << "For what range of account balance would you like to adjust the interest rate?" << endl;
	cout << "1.) >1000" << endl
	<< "2.) >5000" << endl
	<< "3.) >10000" << endl
	<< "4.) >100000" << endl;
	cin >> choice;
	cin.ignore();
	switch(choice){
		case '1':
			cout << "Enter the new rate for accounts with a balance between $4999 - $1000:" << endl;
			cin >> temp;
			cin.ignore();
			rates[0] = temp;
			//sscanf(in.c_str(),"%f",rates[0]);
			//need some error check for rates enetered
		break;
		
		case '2':
			cout << "Enter the new rate for accounts with a balance between $5000 - $9999:" << endl;
			cin >> temp;
			cin.ignore();
			rates[1] = temp;
			//sscanf(in.c_str(),"%f",rates[1]);
			//need some error check for rates enetered
		break;
		
		case '3':
			cout << "Enter the new rate for accounts with a balance between $10000 - $99999:" << endl;
			cin >> temp;
			cin.ignore();
			rates[2] = temp;
			//sscanf(in.c_str(),"%f",rates[2]);
			//need some error check for rates enetered
		break;
		
		case '4':
			cout << "Enter the new rate for accounts with a balance above $100000:" << endl;
			cin >> temp;
			cin.ignore();
			rates[3] = temp;
			//sscanf(in.c_str(),"%f",rates[3]);
			//need some error check for rates enetered
		break;
		
		default:
		cout << "Invalid choice! Returning to menu..." << endl;
		break;
	}
	ofstream myfile;
	myfile.open(filename);
	for(i=0;i<4;i++){
		myfile << ranges[i] << " " << rates[i] << char(13) << char(10);
	}
	myfile.close();
}

int main(void){
	string in;
	char type_choice, type, choice;
	Bank_Account* Account;
	Manager_Acct M;
	string acct_str, freeze_str;
	long acct_num, freeze_num;
	int a = 0, b = 0, c = 0, z = 0;
	char choice2 = 0;
	cout << "Welcome to Online Banking Inc." << endl;
		cout << "------------------------------------------------" << endl << endl;
	while(a == 0){
		cout << "Please select an option to begin" << endl
			<< "1.) Login as Manager" << endl
			<< "2.) Login as Customer" << endl
			<< "3.) Create New Customer Account" << endl;
	
		cin >> choice;
		cin.ignore();
		switch(choice){
			
			case '1':
			cout << "Welcome! Please enter your employee number:" << endl;
			cin >> acct_num;
			cin.ignore();
			b = M.manager_login(acct_num);
			if(b==1){
				cout << "Welcome Manager!" << endl;
				cout << "------------------------------------------------" << endl << endl;
				while(a == 0){
					cout << "What would you like to do?" << endl;
					cout << "1.) Freeze Customer Account" << endl;
					cout << "2.) Un-Freeze Customer Account" << endl;
					cout << "3.) View Current Interest Rates" << endl;
					cout << "4.) Adjust Interest Rates" << endl;
					cout << "5.) Log Out" << endl;
					cin >> choice;
					cin.ignore();
					switch(choice){
					
						case '1':
						cout << "\nEnter the Account Number of the Account you would like to freeze:" << endl;
						cin >> freeze_num;
						cin.ignore();
						//sscanf(freeze_str.c_str(),"%li",freeze_num);
						M.freeze(freeze_num);
						break;
					
						case '2':
						cout << "\nEnter the Account Number of the Account you would like to unfreeze:" << endl;
						cin >> freeze_num;
						cin.ignore();
						//sscanf(freeze_str.c_str(),"%d",freeze_num);
						M.unfreeze(freeze_num);
						break;
					
						case '3':
						cout << "Displaying the currect Interest rates..." << endl;
						M.Current_Rates();
						break;
					
						case '4':
						M.Adjust_Rate();
						break;
						
						case '5':
						cout << "Closing Online Banking Inc. Thank you!" << endl;
						cout << "------------------------------------------------" << endl << endl;
						return 0;
						break;
					
						default:
						cout << "Invalid choice! Please try again" << endl;
						break;
					}
				}
				
			}
			else{
				cout << "Login failed...." << endl;
				cout << "Please try again." << endl;
			}
			break;
			
			case '2':
				cout << "Welcome User! Please enter your Account Number:" << endl;
				cin >> acct_num;
				cin.ignore();
				//sscanf(acct_str.c_str(),"%ld",acct_num);
				type = user_acct(acct_num);
			
				if(type == 'F'){
					cout << "This account number does not exist." << endl;
					cout << "If you would like to create this account please select 'Create Account' from the main menu" << endl;
					return 0;
				}
			
				else if(type == 'S'){
					Account = new Saving_Acct(acct_num);
				}
			
				else if(type == 'C'){
					Account = new Checking_Acct(acct_num);
				}
				//add menu for user here and switch statement
				cout << "Login Successful!" << endl;
				cout << "------------------------------------------------" << endl << endl;
				while(z == 0){
					cout << "What would you like to do? Please select one of the following options:" << endl;
					cout << "1.) View Account Balance" << endl
					<< "2.) Deposit into Account" << endl
					<< "3.) Withdraw from Account" << endl
					<< "4.) Close Account" << endl
					<< "5.) Log Out" << endl;
					cin >> choice2;
					cin.ignore();
					//sscanf(in.c_str(),"%d",choice2);
					switch(choice2){
						case '1':
						Account->check_balance();
						break;
						
						case '2':
						Account->deposit();
						break;
						
						case '3':
						Account->withdraw();
						break;
						
						case '4':
						//maybe add something to make sure user wants to close account
						Account->close_Acct();
						return 0;
						break;
						
						case '5':
						cout << "Closing Online Banking Inc. Thank you!" << endl;
						cout << "------------------------------------------------" << endl << endl;
						return 0;
						break;
						
						default:
						cout << "Invalid Choice! Please try again" << endl;
						break;
					}
				}
			
			break;
			
			case '3':
			cout << "Welcome!" << endl;
			cout << "------------------------------------------------" << endl << endl;
			cout << "To create your account we need to gather some information from you" << endl;
			cout << "What type of Account woudl you like to open? Savings or Checking?" << endl;
			while(c == 0){
				cout << "Enter 'S' for Savings Account or 'C' for Checking Account:" << endl;
				cin >> type_choice;
				cin.ignore();
				switch(type_choice){
					case 'S':
					Account = new Saving_Acct();
					c = 1;
					break;
					
					case 'C':
					Account = new Checking_Acct();
					c = 1;
					break;
					
					default:
					cout << "Invalid choice! Please Try again" << endl;
					break;
				}
			}
			Account->print_to_file();
			while(z == 0){
					cout << "What would you like to do? Please select one of the following options:" << endl;
					cout << "1.) View Account Balance" << endl
					<< "2.) Deposit into Account" << endl
					<< "3.) Withdraw from Account" << endl
					<< "4.) Close Account" << endl
					<< "5.) Log Out" << endl;
					cin >> choice2;
					cin.ignore();
					switch(choice2){
						case '1':
						Account->check_balance();
						break;
						
						case '2':
						Account->deposit();
						break;
						
						case '3':
						Account->withdraw();
						break;
						
						case '4':
						//maybe add something to make sure user wants to close account
						Account->close_Acct();
						return 0;
						break;
						
						case '5':
						cout << "Closing Online Banking Inc. Thank you!" << endl;
						cout << "------------------------------------------------" << endl << endl;
						return 0;
						break;
						
					}
				}
			break;
			
			case '4':
			cout << "Closing Online Banking Inc. Thank you!" << endl;
			cout << "------------------------------------------------" << endl << endl;
			return 0;
			break;
			
			default:
			cout << "Incorrect choice please try again" << endl << endl;
			
			break;
		}
		
	}
	
	return 0;
}

