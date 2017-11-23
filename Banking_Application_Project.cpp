//Dalton Koch
//Thomas Seabolt
//ECE 3220: Software Design in C/C++
//Final Project

/*
NOTICE:

Please read the Banking App 1st release word document before running the demo
The last paragraph in the document includes some instructions for running the program
The Account Text files are also included in the zip file, these are necessary for the demo to run

this current iteration only accepts the expected input type, this is ann issue we are working on

The structure of the account text files is:

Account type (Char)
User Id (int)
Account number (Long int)
Account password (string)
Balance (float)
Frozen (Bool)
*/

/*
work on cancel option for the user when creating accountor manager login, something that returns them to main menu
work on switch statement choice input
work on print translog function
add operators
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
	int n = sprintf(filename,"%08li.txt",acct_num);
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

long check_num(string num){
	long n = 0;
	n = atol(num.c_str());
	if(n<=0){
		return 0;
	}
	else{
		return n;
	}
}

//encrypts password 
string pass_encrypt(string password) {
	//change case of all characters
	for (auto &c : password) {
		if (islower(c)) {
			c = toupper(c);
		}
		else if (isupper(c)) {
			c = tolower(c);
		}
	}
	//change every z to y, everything else to one character higher
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
		string password;
		char Acct_Type;
		
	public:
		bool frozen;
		Bank_Account(); //constructor for new account
		Bank_Account(long acct_num); //parametric constructor for existing account
		virtual void deposit() throw(int);
		virtual void withdraw() throw(int,char);
		virtual void check_balance();
		void check_password() throw(int);
		//void view_translog();
		void close_Acct();
		void print_to_file();
		int does_exist();
		void print_to_translog(double value, char type);
		void print_translog();
		void transfer(long acct_num);
};

//gets account number, userID, password from user, creates account
Bank_Account::Bank_Account(){
	string in;
	int i = 0;
	cout << "Thank you for making an account with Online Banking Inc." << endl;
	cout << "First we need to gather some information from you" << endl;
	cout << "What 8-digit acccount number would you like to have?" << endl;
	while(i==0){
		cin >> in;
		Account_Num = check_num(in);
		if(Account_Num > 99999999 || Account_Num == 0){
			cout << "Invalid choice of Account Number! Must be 8-digits or less. Please try again:" << endl;
		}
		else{
			i = 1;
		}
	}
	i = 0;
	cout << "Enter your userID:" << endl;
	while(i == 0){
		cin >> in;
		userID = atol(in.c_str());
		if(userID <= 0){
			cout << "Invalid choice for Used ID please try agaiin" << endl;
			cout << "Enter a User ID: " << endl;
		}
		else{
			i = 1;
		}
	}
	cout << "Enter your password you would like to have for this account:" << endl;
	cin >> password;
	cin.ignore();
	
	frozen = 0;
	cout << "Your starting balance will be $1000!" << endl;
	balance = 1000.00;
	
}

//if account exists, gets info from its file
Bank_Account::Bank_Account(long acct_num){

	ifstream myfile;
	char filename[30];
	string in;
	sprintf(filename,"%08li.txt",acct_num);
	myfile.open(filename);
	if(!myfile.is_open()){
		cout << "This Account does not exist." << endl;
		throw 1;
		
	}
	myfile >> Acct_Type;
	myfile >> userID;
	myfile >> Account_Num;
	myfile >> password;
	myfile >> balance;
	myfile >> frozen;
	myfile.close();
	
}

void Bank_Account::check_password() throw(int){
	string in;
	cout << "Please enter the password for this account: ";
	cin >> in;
	cin.ignore();
	if(in != password){
		throw 1;
	}
}

//prints balance
void Bank_Account::check_balance(){
	cout << "Your current baland is: $" << balance << endl;
}

//writes new data to file
void Bank_Account::print_to_file(){
	char filename[30];
	ofstream myfile;
	sprintf(filename,"%08li.txt",Account_Num);
	myfile.open(filename,ios::out);
	myfile << Acct_Type;
	myfile << char(13) << char(10);
	myfile << userID;
	myfile << char(13) << char(10);
	myfile << Account_Num;
	myfile << char(13) << char(10);
	myfile << password;
	myfile << char(13) << char(10);
	myfile << balance;
	myfile << char(13) << char(10);
	myfile << frozen;
	myfile << char(13) << char(10);
	myfile.close();
}

void Bank_Account::print_to_translog(double value, char type){
	
	char filename[30];
	ofstream myfile;
	sprintf(filename,"%08li_translog.txt",Account_Num);
	myfile.open(filename,ios::out | ios::app);
	
	if(type == 'D'){
		myfile << "Transaction Type: Deposit" << "  ";
		myfile << "Amount Deposited: " << value << "  ";
		myfile << "Previous Balance: " << balance - value << "  ";
		myfile << "New Balance: " << balance << "  ";
		myfile << char(13) << char(10);
	}
	
	if(type == 'W'){
		myfile << "Transaction Type: Withdraw" << "  ";
		myfile << "Amount Withdrawn: " << value << "  ";
		myfile << "Previous Balance: " << balance + value << "  ";
		myfile << "New Balance: " << balance << "  ";
		myfile << char(13) << char(10);
	}
	
	if(type == 'T'){
		myfile << "Transaction Type: Transfer" << "  ";
		myfile << "Amount Transferred: " << value << "  ";
		myfile << "Previous Balance: " << balance + value << "  ";
		myfile << "New Balance: " << balance << "  ";
		myfile << char(13) << char(10);
	}
	myfile.close();
}

void Bank_Account::print_translog(){
	ifstream myfile;
	char filename[30];
	string in;
	sprintf(filename,"%08li_translog.txt",Account_Num);
	myfile.open(filename);
	if(!myfile.is_open()){
		cout << "This Account does not have a transaction log yet." << endl;
		throw 1;
	}
	while(getline(myfile,in)){
		cout << in << endl;
	}
}

//checks if account exists
int Bank_Account::does_exist(){
	cout << userID << endl;
	if(userID == 0){
		//account does not exist
		return 0;
	}
	else{
		return 1;
	}
}

//deposit money into account
void Bank_Account::deposit() throw(int){
	string amount;
	double n = 0;
	cout << "How much would you like to deposit?" << endl;
	cin >> amount;
	cin.ignore();
	n = atof(amount.c_str());
	if(n <= 0){
		throw 1;
	}
	
	balance += n;
	cout << "The new balance of account - " << Account_Num << "is: $" << balance << endl;
	//updates file with new balance
	this->print_to_file();
	this->print_to_translog(n,'D');
}

//withdraw from account
void Bank_Account::withdraw() throw(int,char){
	double n = 0;
	string amount;
	cout << "How much would you like to withdraw?" << endl;
	cin >> amount;
	cin.ignore();
	n = atof(amount.c_str());
	if(n <= 0){
		throw 1;
	}
	if (balance - n > 0) {//if funds are available
		balance -= n;
		cout << "$" << amount << " withdrawn from account successfully";
	}
	else {
		throw 'a';
	}
	//updates file with new balance
	this->print_to_file();
	this->print_to_translog(n,'W');
}

//closes account
void Bank_Account::close_Acct(){
	balance = 0;
	//closed type
	Acct_Type = 'F';
	userID = 0;
	cout << "Accout: " << Account_Num << " has now been closed" << endl;
	//updates file
	this->print_to_file();
}

void Bank_Account::transfer(long acct_num) {
	Bank_Account transferee(acct_num);
	if (!transferee.does_exist()) {//account doesn't exist
		cout << "Account does not exist, transfer cancelled." << endl;
	}
	else {
		string amountStr;
		double amount = 0;
		cout << "How much would you like to transfer?" << endl;
		getline(cin, amountStr);
		sscanf(amountStr.c_str(), "%lf", &amount);
		if (amount < 0) {
			cout << "Amount cannot be negative." << endl;
		}
		else if (amount > this->balance) {
			cout << "Insufficient funds for transfer." << endl;
		}
		else {
			cout << "1: " << transferee.balance << "   2: " << this->balance << endl;
			transferee.balance += amount;
			this->balance -= amount;
			transferee.print_to_file();
			this->print_to_file();
			transferee.print_to_translog(amount, 'T');
			this->print_to_translog((amount * -1), 'T');
			cout << "Amount transferred successfully." << endl;
		}
	}
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

//set type to C
Checking_Acct::Checking_Acct() : Bank_Account(){
	Acct_Type = 'C';
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
		void deposit() throw(int);
		void withdraw() throw(int,char);
		void check_balance();
		void Calc_Predicted_Interest();
		void Get_rate();
};

//sets rate
Saving_Acct::Saving_Acct(long acct_num) : Bank_Account(acct_num){
	this->Get_rate();
}

//sets type to C, sets rate
Saving_Acct::Saving_Acct() : Bank_Account(){
	Acct_Type = 'S';
	this->Get_rate();
	cout << "The Interest Rate set for this account is: " << Interest_Rate*100 << "%" << endl;
}

//deposit to account
void Saving_Acct::deposit() throw(int){
	string amount;
	double n = 0;
	cout << "How much would you like to deposit?" << endl;
	cin >> amount;
	cin.ignore();
	n = atof(amount.c_str());
	if(n <= 0){
		throw 1;
	}
	balance += n;
	cout << "The new balance of account: " << Account_Num << " is: " << balance << endl;
	//get new rate
	this->Get_rate();
	this->print_to_file();
	this->print_to_translog(n,'D');
}

//withdraw from account
void Saving_Acct::withdraw() throw(int,char){
	string amount;
	double n = 0;
	cout << "How much would you like to withdraw?" << endl;
	cin >> amount;
	cin.ignore();
	n = atof(amount.c_str());
	if(n <= 0){
		throw 1;
	}
	if (balance - n >= 0){//if funds are available
		balance -= n;
		cout << "$" << amount << " withdrawn from account successfully" << endl;
		//set new rate
		this->Get_rate();
		this->print_to_file();
		this->print_to_translog(n,'W');
	}
	else {
		throw 'a';
	}
	
}

//print balance
void Saving_Acct::check_balance(){
	cout << "Your current balance is: $" << balance << endl;
	cout << "The Interest Rate of this account is:" << Interest_Rate*100 << "%" << endl;
}

//get rate for account
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
	//get interest rates and their associated balance ranges
	for(i=0;i<4;i++){
		file >> range;
		file >> rate;
		rates.push_back(rate);
		ranges.push_back(range);
	}
	file.close();
	//set rate based on data collected from file
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

//predicts interest
void Saving_Acct::Calc_Predicted_Interest(){
	double temp;
	int t;
	string in;
	cout << "For how many years would you like to see predicted interest earned on this account? Please neter the number of years:" << endl;
	cin >> t;
	cin.ignore();
	//sscanf(in.c_str(),"%d",t);
	//interest calculation
	temp = balance* pow(1+Interest_Rate,t);
	cout << "Your expected account balance after " << t << " years is: $" << temp << endl;
}

//class for managing accounts
class Manager_Acct {
	private:
		string password;
		int empl_num;
	public:
		Manager_Acct();
		int manager_login(long acct_num);
		void freeze(long Account_Num);
		void unfreeze(long Account_Num);
		void Current_Rates();
		void Adjust_Rate();
};

//default employee number and password
Manager_Acct::Manager_Acct(){
	empl_num = 12345678;
	password = pass_encrypt("password123");
}

//logs in if acct_num entered equals empl_num and password is correct
int Manager_Acct::manager_login(long acct_num){
	string pass;
	if(acct_num == empl_num){
		cout << "please enter your password..." << endl;
		getline(cin,pass);
		if(pass_encrypt(pass) == password){
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

//freeze an account
void Manager_Acct::freeze(long Account_Num) {
	Bank_Account temp(Account_Num);
	if(temp.does_exist() == 0){//make sure account exists
		cout << "Account: " << Account_Num << " does not exist. Account cannot be frozen" << endl;
	}
	else{//if it exists, set frozen to one and write to file
		temp.frozen = 1;
		temp.print_to_file();
	}
}

//same as freeze's logic, but opposite function. Sets frozen to 0 instead of 1
void Manager_Acct::unfreeze(long Account_Num) {
	Bank_Account* temp;
	cout << "Account Number: " << Account_Num << endl;
	try{
		temp = new Bank_Account(Account_Num);
	}
	catch(char q){
		//empty catch
	}
	catch(int r){
		cout << "here" << endl;
		return;
	}
	cout << "Here 2" << endl;
	temp->frozen = 0;
	temp->print_to_file();
	
}

//view current interest rates/ ranges
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
	//get all rates/ranges
	for(i=0;i<4;i++){
		file >> range;
		file >> rate;
		rates.push_back(rate);
		ranges.push_back(range);
	}
	file.close();
	for(i=0;i<4;i++){
		cout << "For accounts with a balance > $" << ranges[i] << " the rate is:" << rates[i]*100 << "%" << endl; 	//print rate/range at i (I think this would be fine as the last line in the first for loop)
	}
}

//adjust interest rates
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
	//get current values
	for(i=0;i<4;i++){
		file >> range;
		file >> rate;
		rates.push_back(rate);
		ranges.push_back(range);
	}
	file.close();
	cout << "For what range of account balance would you like to adjust the interest rate?" << endl;
	cout << "1.) $1000-4999" << endl
	<< "2.) $5000-9999" << endl
	<< "3.) $10000-99999" << endl
	<< "4.) >$100000" << endl;
	cin >> choice;
	cin.ignore();
	//go to selected range
	switch(choice){
		case '1':
			cout << "Enter the new rate for accounts with a balance between $4999 - $1000:" << endl;
			cin >> temp;
			cin.ignore();
			rates[0] = temp;
			//sscanf(in.c_str(),"%f",rates[0]);
			//need some error check for rates entered
		break;
		
		case '2':
			cout << "Enter the new rate for accounts with a balance between $5000 - $9999:" << endl;
			cin >> temp;
			cin.ignore();
			rates[1] = temp;
			//sscanf(in.c_str(),"%f",rates[1]);
			//need some error check for rates entered
		break;
		
		case '3':
			cout << "Enter the new rate for accounts with a balance between $10000 - $99999:" << endl;
			cin >> temp;
			cin.ignore();
			rates[2] = temp;
			//sscanf(in.c_str(),"%f",rates[2]);
			//need some error check for rates entered
		break;
		
		case '4':
			cout << "Enter the new rate for accounts with a balance above $100000:" << endl;
			cin >> temp;
			cin.ignore();
			rates[3] = temp;
			//sscanf(in.c_str(),"%f",rates[3]);
			//need some error check for rates entered
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

//main, presents menu to user and allows them to perform actions on their accounts
int main(void){
	string in;
	char type_choice, type, choice;
	Bank_Account* Account;
	Manager_Acct M;
	string acct_str, freeze_str;
	long acct_num, freeze_num;
	int a = 0, b = 0, c = 0, z = 0, u =0;
	char choice2;
	cout << "Welcome to Online Banking Inc." << endl;
		cout << "------------------------------------------------" << endl << endl;
	while(a == 0){
		cout << "Please select an option to begin" << endl
			<< "1.) Login as Manager" << endl
			<< "2.) Login as Customer" << endl
			<< "3.) Create New Customer Account" << endl
			<< "4.) Exit Program" << endl;
		cin >> choice;
		cin.ignore();
		//go to choice
		switch(choice){
			//manager
			case '1':
			cout << "Welcome! Please enter your employee number:" << endl;//attempt to login
			cin >> in;
			cin.ignore();
			acct_num = check_num(in);
			if(acct_num == 0){
				cout << "Invalid employee number! Returning to main menu." << endl;
				break;
			}
			b = M.manager_login(acct_num);
			//successful login
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
						//freeze
						case '1':
						cout << "\nEnter the Account Number of the Account you would like to freeze:" << endl;
						cin >> in;
						freeze_num = check_num(in);
						if(freeze_num == 0){
							cout << "Invalid input for account number! Returning to manager menu." << endl;
							break;
						}
						
						M.freeze(freeze_num);
						break;
						//unfreeze
						case '2':
						cout << "\nEnter the Account Number of the Account you would like to unfreeze:" << endl;
						cin >> in;
						freeze_num = check_num(in);
						if(freeze_num == 0){
							cout << "Invalid input for account number! Returning to manager menu." << endl;
							break;
						}
					
						M.unfreeze(freeze_num);
				
						break;
						//view interest rates
						case '3':
						cout << "Displaying the currect Interest rates..." << endl;
						M.Current_Rates();
						break;
						//adjust interest rates
						case '4':
						M.Adjust_Rate();
						break;
						//exit
						case '5':
						cout << "Closing Online Banking Inc. Thank you!" << endl;
						cout << "------------------------------------------------" << endl << endl;
						return 0;
						break;
						//invalid choice
						default:
						cout << "Invalid choice! Please try again" << endl;
						break;
					}
				}
				
			}
			//login unsuccessful
			else{
				cout << "Login failed...." << endl;
				cout << "Please try again." << endl;
			}
			break;
			//existing user
			case '2':
				cout << "Welcome User! Please enter your Account Number:" << endl;//attempt to login, need to add password check still
				cin >> in;
				cin.ignore();
				acct_num = check_num(in);
				if(acct_num == 0){
					cout << "Invalid input for account number! Returning to main menu." << endl << endl;
					break;
				}
				type = user_acct(acct_num);
				//account is closed
				if(type == 'F'){
					cout << "This account number does not exist." << endl;
					cout << "If you would like to create this account please select 'Create Account' from the main menu" << endl;
					return 0;
				}
				//savings
				else if(type == 'S'){
					try{
						Account = new Saving_Acct(acct_num);
						Account->check_password();
					}
					catch(int){
						cout << "Incorrect Password for account! Returning to main menu." << endl << endl;
						break;
					}
					
					if(Account->frozen == 1){
						cout << "This account is frozen. Please speak to a Bank Manager to unfreeze this account." << endl;
						cout << "Returing to main menu" << endl;
						break;
					}
				}
				//checking
				else if(type == 'C'){
					try{
						Account = new Checking_Acct(acct_num);
						Account->check_password();
					}
					catch(int){
						cout << "Incorrect Password for account! Returning to main menu." << endl << endl;
						break;
					}
				
					catch(char t){
						cout << "This account is frozen. Please speak to a Bank Manager to unfreeze this account." << endl;
						cout << "Returing to main menu" << endl;
						break;
					}
				}
				//add menu for user here and switch statement
				cout << "Login Successful!" << endl;
				cout << "------------------------------------------------" << endl << endl;
				while(z == 0){
					cout << "What would you like to do? Please select one of the following options:" << endl;
					cout << "1.) View Account Balance" << endl
					<< "2.) Deposit into Account" << endl
					<< "3.) Withdraw from Account" << endl
					<< "4.) Create Transfer" << endl
					<< "5.) View Transaction Log" << endl
					<< "6.) Close Account" << endl
					<< "7.) Log Out" << endl;
					cin >> choice2;
					cin.ignore();
					switch(choice2){
						//check balance
						case '1':
						Account->check_balance();
						break;
						//deposit
						case '2':
						try{
							Account->deposit();
						}
						catch(int e){
							cout << "Invalid value for amount to deposit into account! Returning to User menu." << endl << endl;
						}
						break;
						//withdraw
						case '3':
						try{
							Account->withdraw();
						}
						catch(int e){
							cout << "Invalid value for amount to withdraw from account! Returning to User menu." << endl << endl;
						}
						catch(char q){
							cout << "Insuficient funds for withdraw! Please try again" << endl << endl;
						}
						break;
						//transfer
						case '4':
							cout << "What is the account number you would like to transfer to?" << endl;
							getline(cin, acct_str);
							sscanf(acct_str.c_str(), "%ld", &acct_num);
							Account->transfer(acct_num);
						break;
						//transaction log
						case '5':
							try{
								Account->print_translog();
							}
							catch(int){
								cout << "Returning to user menu." << endl;
								
							}
						break;
						//close account
						case '6':
						cout << "\nAre you sure you would like to close your account? Enter 'Y' for yes or 'N' for no." << endl;
						u = 0;
						while(u==0){
							cin >> choice;
							cin.ignore();
							switch(choice){
								case 'Y':
									cout << "Closing account..." << endl;
									Account->close_Acct();
									cout << "Closing Online Banking Inc. Thank you!" << endl;
									cout << "------------------------------------------------" << endl << endl;
									return 0;
								break;
							
								case 'N':
									cout << "Account not closed. Returning to User menu." << endl;
									u = 1;
								break;
								
								default:
									cout << "Incorrect choice please try again." << endl;
								break;
							}
						}
						break;
						//exit
						case '7':
						cout << "Closing Online Banking Inc. Thank you!" << endl;
						cout << "------------------------------------------------" << endl << endl;
						return 0;
						break;
						//default
						default:
						cout << "Invalid Choice! Please try again" << endl;
						break;
					}
				}
			
			break;
			//new user
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
			//create file
			Account->print_to_file();
			while(z == 0){
				//same choices as existing user, could make this a function to cut down on code
					cout << "What would you like to do? Please select one of the following options:" << endl;
					cout << "1.) View Account Balance" << endl
					<< "2.) Deposit into Account" << endl
					<< "3.) Withdraw from Account" << endl
					<< "4.) Create Transfer" << endl
					<< "5.) View Transaction Log" << endl
					<< "6.) Close Account" << endl
					<< "7.) Log Out" << endl;
					cin >> choice2;
					cin.ignore();
					switch(choice2){
						case '1':
						Account->check_balance();
						break;
						
						//deposit
						case '2':
						try{
							Account->deposit();
						}
						catch(int e){
							cout << "Invalid value for amount to deposit into account! Returning to User menu." << endl << endl;
						}
						break;
						
						//withdraw
						case '3':
						try{
							Account->withdraw();
						}
						catch(int e){
							cout << "Invalid value for amount to withdraw from account! Returning to User menu." << endl << endl;
						}
						catch(char q){
							cout << "Insuficient funds for withdraw! Please try again" << endl << endl;
						}
						break;
						
						case '4':
							cout << "What is the account number you would like to transfer to?" << endl;
							getline(cin, acct_str);
							sscanf(acct_str.c_str(), "%ld", &acct_num);
							Account->transfer(acct_num);
						break;
						
						case '5':
							try{
								Account->print_translog();
							}
							catch(int){
								cout << "Returning to user menu." << endl;
								
							}
						break;
						
						case '6':
						cout << "\nAre you sure you would like to close your account? Enter 'Y' for yes or 'N' for no." << endl;
						while(u==0){
							cin >> choice;
							cin.ignore();
							switch(choice){
								case 'Y':
									cout << "Closing account..." << endl;
									Account->close_Acct();
									cout << "Closing Online Banking Inc. Thank you!" << endl;
									cout << "------------------------------------------------" << endl << endl;
									return 0;
								break;
							
								case 'N':
									cout << "Account not closed. Returning to User menu." << endl;
									u=1;
								break;
								
								default:
									cout << "Incorrect choice please try again." << endl;
								break;
							}
						}
						break;
						
						case '7':
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

