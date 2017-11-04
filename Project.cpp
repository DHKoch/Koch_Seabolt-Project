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
#include <vector>

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
		getline(myfile, in);
		sscanf(in.c_str(), "%c", temp);
		myfile.close();
		return temp;
	}
}

//ABC base class
class Bank_Account {
	private:
		
	protected:
		int userID;
		long Account_Num;
		double balance;
		string Password;
		char Acct_type;
		bool frozen;
	public:
		Bank_Account(); //constructor for new account
		Bank_Account(long acct_num); //parametric constructor for existing account
		void deposit();
		void withdraw();
		virtual void check_balance();
		void view_translog();
		void close_Acct();
		//transfer money function
};

Bank_Account::Bank_Account(){
	//ask user for info and error check the info
}

Bank_Account::Bank_Account(long acct_num){
	ifstream myfile;
	char filename[30];
	string in;
	sprintf(filename,"%li.txt",acct_num);
	myfile.open(filename);
	getline(myfile, in);
	sscanf(in.c_str(), "%c", Acct_type);
	getline(myfile, in);
	sscanf(in.c_str(), "%d", userID);
	getline(myfile, in);
	sscanf(in.c_str(), "%ld", Account_Num);
	getline(myfile, in);
	sscanf(in.c_str(), "%s", Password);
	getline(myfile, in);
	sscanf(in.c_str(), "%lf", balance);
	getline(myfile, in);
	sscanf(in.c_str(), "%d", frozen);
	myfile.close();
}

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
		Checking_Acct();
		void check_balance();
		//pay bill function??????????
};

Checking_Acct::Checking_Acct() : Bank_Account(){
	//calls base constructor only so far
}

//derived class
class Saving_Acct : public Bank_Account {
	private:
		float Interest_Rate;
		
	public:
		Saving_Acct();
		//Saving_Acct(long acct_num);
		void check_balance();
		void calc_Predicted_Interest();
};

Saving_Acct::Saving_Acct() : Bank_Account(){
	int i =0;
	float rate;
	int range;
	vector<float> rates;
	vector<int> ranges;
	string filename = "Rates.txt", in;
	ifstream file;
	file.open(filename);
	if(!file.is_open()){
		//checks to see if file was opened
		cout << "error Rates.txt file not opened" << endl;
	}
	for(i=0;i<4;i++){
		getline(file, in);
		sscanf(in.c_str(), "%f", rate);
		getline(file, in);
		sscanf(in, "%d", range);
		rates.push_back(rate);
		ranges.push_back(range);
	}
	file.close();
	if(balance>range[3]){
		Interest_Rate = rates[3];
	}
	else if(balance>range[2]){
		Interest_Rate = rates[2];
	}
	else if(balance>range[1]){
		Interest_Rate = rates[1];
	}
	else if(balance>range[0]){
		Interest_Rate = rates[0];
	}
	else{
		Interest_Rate = 0.00;
	}
}
//currently don't need this, Bank_Account already has constructors to handle if account_num was/wasn't entered
//Saving_Acct::Saving_Acct(long acct_num) : Bank_Account(long acct_num){
//	int i =0;
//	float rate;
//	int range;
//	vector<float>rates;
//	vector<int>ranges;
//	string filename = "Rates.txt", in;
//	ifstream file;
//	file.open(filename);
//	if(!file.is_open()){
//		//checks to see if file was opened
//		cout << "error Rates.txt file not opened" << endl;
//	}
//	for(i=0;i<4;i++){
//		getline(file, in);
//		sscanf(in, "%f", rate);
//		getline(file, in);
//		sscanf(in, "%d", range);
//		rates.push_back(rate);
//		ranges.push_back(range);
//	}
//	file.close();
//	if (balance>range[3]) {
//		Interest_Rate = rates[3];
//	}
//	else if (balance>range[2]) {
//		Interest_Rate = rates[2];
//	}
//	else if (balance>range[1]) {
//		Interest_Rate = rates[1];
//	}
//	else if (balance>range[0]) {
//		Interest_Rate = rates[0];
//	}
//	else {
//		Interest_Rate = 0.00;
//	}
//}

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

int Manager::manager_login(long acct_num){
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
	char type_choice;
	Bank_Account* Accout;
	char type;
	string acct_str;
	long acct_num;
	char choice;
	int b = 0;
	int c = 0;
	string freeze_str;
	long freeze_num;
	
	cout << "Welcome to Online Banking Inc." << endl;
	cout << "------------------------------------------------" << endl << endl;
	cout << "Please select an option to begin" << endl
		<< "1.) Login as Manager" << endl
		<< "2.) Login as Customer" << endl
		<< "3.) Create New Customer Account" << endl;
	
	cin >> choice;
	cin.ignore();
	while(a == 0){
		switch(choice){
			
			case '1':
			Manager M();
			cout << "Welcome! Please enter your employee number:" << endl;
			getline(cin, acct_str);
			sscanf(acct_str.c_str(),"%li",acct_num);
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
						sscanf(freeze_str.c_str(),"%li",freeze_num);
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
			getline(sin,acct_str);
			sscanf(acct_str.c_str(),"%ld",acct_num);
			type = user_acct(acct_num);
			
			if(type == 'F'){
				cout << "This account number does not exist." << endl;
				cout << "If you would like to create this account please select 'Create Account' from the main menu" << endl;
			}
			
			else if(type == 'S'){
				Account = new Saving_Acct(acct_num);
			}
			
			else if(type == 'C'){
				Account = new Checking_Acct(acct_num);
			}
			//add menu for user here
			
			break;
			
			case '3':
			cout << "Welcome!" << endl;
			cout << "------------------------------------------------" << endl << endl;
			cout << "To create your account we need to gather some information from you" << endl;
			cout << "What type of Account woudl you like to open? Savings or Checking?" << endl;
			while(c == 0){
				cout << "Enter 'S' for Savings Account or 'C' for Checking Account:" << endl;
				getline(cin,type_choice);
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
			//print info to new user file
			//add menu for user, same as menu above for signed in user
			break;
			
			case '4':
			//exit program here
			return 0;
			break;
			
			default:
			cout << "Incorrect choice please try again" << endl << endl;
			
			break;
		}
		
	}
	
	return 0;
}

