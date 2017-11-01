//Dalton Koch
//Thomas Seabolt
//ECE 3220: Software Design in C/C++
//Final Project
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <string>
#define manager_acct_num = 123456789
string manager_password = "Password123";

using namespace std;

//ABC base class
class Bank_Account {
	private:
		
	protected:
		int userID;
		long Account_Num;
		double Balance;
		string Password;
		char Acct_type;
		bool frozen;
	public:
		void deposit();
		void withdraw();
		virtual void check_balance();
		void view_translog();
		void close_Acct();
		//transfer money function
}

//derived class from ABC base class
class Checking_Acct : public Bank_Account {
	private:
		
	public:
		void check_balance();
		//pay bill function??????????
}

//derived class
class Saving_Acct : public Bank_Account {
	private:
		float Interest_Rate;
		time_t lastCompound; //see how this affects reading from file, should we have file type first and check to see if C or S then read in different
	public:
		void check_balance();
		void calcInterest();
}

class Manager_Acct {
	private:
		string Password;
		//userID?
	public:
		void freeze(long Account_Num);
		void unfreeze(long Account_Num);
		void Current_Rates();
		void Adjust_Rate();
}

void Bank_Account::deposit() {
	double amount, balance;
	string amountStr;
	cout << "How much would you like to deposit?" << endl;
	cin >> amountStr;
	sscanf(amountStr, "%.2lf", amount);
	//TODO access database, set balance
	balance += amount;
	//TODO update balance in database
}

void Bank_Account::withdraw() {
	double amount, balance;
	string amountStr;
	cout << "How much would you like to withdraw?" << endl;
	cin >> amountStr;
	sscanf(amountStr, "%.2lf", amount);
	//TODO access database, set balance
	if (balance - amount > 0) {
		balance -= amount;
		//TODO update balance in database
		cout << "$" << amount << " withdrawn from account successfully";
	}
	else {
		cout << "Insufficient funds for requested withdrawal.";
	}
}

void Bank_Account::view_translog() {
//TODO could have a transactions table. Each transaction would have an accountID, an amount, and a date. We could go through
//the table and print all amounts/dates with the specified accountID. 
}

int manager_login(int acct_num){
	string password;
	if(acct_num == manager_acct_num){
		cout << "please enter your password..." << endl;
		cin >> password;
		if(pass_encrypt(password) == pass_encrypt(manager_password)){
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

string pass_encrypt(string password){
	for(auto &c : password){
		if(islower(c)){
			c = toupper(c);
		}
		else if(isupper(c)){
			c = tolower(c);
		}
	}
	for(auto &c : password){
		if(c=='z'){
			c -= 1;
		}
		else if(c < 'z'){
			c += 1;
		}
	}
	return password;
}

void Manager_Acct::freeze(long Account_Num) {
	if (true /*TODO if account exists*/) {
		if (true /*TODO if account isn't already frozen*/) {
			//TODO set frozen to one in database for account
		}
		else {
			cout << "Account is already frozen." << endl;
		}
	}
	else {
		cout << "Account not found." << endl;
	}
}

void Manager_Acct::unfreeze(long Account_Num) {
	if (true /*TODO if account exists*/) {
		if (true /*TODO if account is frozen*/) {
			//TODO set frozen to zero in database for account
		}
		else {
			cout << "Account is not frozen." << endl;
		}
	}
	else {
		cout << "Account not found." << endl;
	}
}
	

int main(void){
	int acct_num;
	char choice;
	int a = 0;
	int b = 0;
	cout << "Welcome to Online Banking Inc." << endl;
	cout << "------------------------------------------------" << endl << endl;
	cout << "Who would you like to sign in as:" << endl
	<< "1.) Manager" << endl
	<< "2.) Customer" << endl;
	cin >> choice;
	while(a == 0){
		switch(choice){
			
			case '1':
			cout << "Welcome! Please enter your employee number:" << endl;
			cin >> acct_num;
			b = manager_login(acct_num);
			if(b==1){
				cout << "Welcome Manager!" << endl;
				cout << "------------------------------------------------" << endl << endl;
				//switch statement here with the options of freeze, view rate, change rates, and log out
				a=1;
			}
			else{
				cout << "Login failed...." << endl;
				cout << "Please try again." <, endl;
			}
			break;
			
			case '2':
			cout << "Welcome User! Please enter your Account Number:" <, endl;
			cin >> acct_num;
			b = user_login;
			if(b==1){
				//create object of the correct type based off data from sql querry
			}
			a=1;
			break;
			
			default:
			cout << "Incorrect choice please try again" << endl << endl;
			
			break;
		}
		
	}
	
	return 0;
}

