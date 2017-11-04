//Dalton Koch
//Thomas Seabolt
//ECE 3220: Software Design in C/C++
//Final Project

//the password for manager login password is "Password123"
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <string>


using namespace std;

int user_login(long acct_num){
	
}

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
};

void Bank_Account::deposit() {
	double amount, balance;
	string amountStr;
	cout << "How much would you like to deposit?" << endl;
	cin >> amountStr;
	sscanf(amountStr.c_str(), "%.2lf", amount);
	
	balance += amount;
	
}

void Bank_Account::withdraw() {
	double amount, balance;
	string amountStr;
	cout << "How much would you like to withdraw?" << endl;
	cin >> amountStr;
	sscanf(amountStr.c_str(), "%.2lf", amount);
	if (balance - amount > 0) {
		balance -= amount;
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

//derived class from ABC base class
class Checking_Acct : public Bank_Account {
	private:
		
	public:
		void check_balance();
		//pay bill function??????????
};

//derived class
class Saving_Acct : public Bank_Account {
	private:
		//float Interest_Rate;
		
	public:
		void check_balance();
		void calc_Predicted_Interest();
};

class Manager_Acct {
	private:
		string Password;
		int empl_num;
	public:
		Manager();
		int manager_login(int acct_num);
		void freeze(long Account_Num);
		void unfreeze(long Account_Num);
		void Current_Rates();
		void Adjust_Rate();
};

Manager::Mangaer(){
	empl_num = 12345678;
	Password = pass_encrypt("Password123");
}

int Manager::manager_login(int acct_num){
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

int main(void){
	string acct_str;
	long acct_num;
	char choice;
	int b = 0;
	string freeze_str;
	long freeze_num;
	cout << "Welcome to Online Banking Inc." << endl;
	cout << "------------------------------------------------" << endl << endl;
	cout << "Who would you like to sign in as:" << endl
	<< "1.) Manager" << endl
	<< "2.) Customer" << endl;
	cin >> choice;
	cin.ignore();
	while(a == 0){
		switch(choice){
			
			case '1':
			Manager M();
			cout << "Welcome! Please enter your employee number:" << endl;
			getline(cin, acct_str);
			sscanf(acct_str.c_str(),"%d",acct_num);
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
					getline(cin, choice);
					switch(choice){
					
						case '1':
						cout << "\nEnter the Account Number of the Account you would like to freeze:" << endl;
						getline(cin,freeze_str);
						sscanf(freeze_str.c_str(),"%d",freeze_num);
						M.freeze(freeze_num);
						break;
					
						case '2':
						cout << "\nEnter the Account Number of the Account you would like to unfreeze:" << endl;
						getline(cin,freeze_str);
						sscanf(freeze_str.c_str(),"%d",freeze_num);
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
						break;
					
						default:
						break;
					}
				}
				
			}
			else{
				cout << "Login failed...." << endl;
				cout << "Please try again." <, endl;
			}
			break;
			
			case '2':
			cout << "Welcome User! Please enter your Account Number:" <, endl;
			getline(sin,acct_num);
			break;
			
			default:
			cout << "Incorrect choice please try again" << endl << endl;
			
			break;
		}
		
	}
	
	return 0;
}

