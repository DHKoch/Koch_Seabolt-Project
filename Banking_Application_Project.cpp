//Dalton Koch
//Thomas Seabolt
//ECE 3220: Software Design in C/C++
//Final Project


#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

//determines if input length is not one character
int check_input(string inStr) {
	if(inStr.length() != 1) {
		return 0;//invalid
	}
	if(atof(inStr.c_str()) == 0 || atof(inStr.c_str())>8){
		return 0;
	}
	return 1;//valid
}

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


float check_rate(string rate){
	float n = 0;
	string in;
	char choice;
	n = atof(rate.c_str());
	if(n==0){
		cout << "If you are trying to change the rate to 0.00% please enter 'Y' if not enter 'N' to try again." << endl;
		getline(cin,in);
		if(strncmp(in.c_str(),"Y",1) == 0){   // strcmp, strncmp
			return 0;
		}
		else if(strncmp(in.c_str(),"N",1) == 0){
			cout << "Rate not changed. Returning to main menu." << endl << endl;
			return -1;
		}
		else{
			cout << "Invalid choice! Rate not changed. Returning to main menu." << endl << endl;
			return -1;
		}
	}
	else if(n<0){
		cout << "Rate cannot be negative please try again!" << endl;
		return -1;
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
		void close_Acct();
		void print_to_file();
		int does_exist();
		void print_to_translog(double value, char type);
		void print_translog();
		virtual void transfer(long acct_num);
		char getAcccountType();
		void operator+(double amt);
		void operator-(double amt);
		virtual void Calc_Predicted_Interest(){};
		long get_num();
};

void Bank_Account::operator+(double amt) {
	balance += amt;
	this->print_to_file();
}

void Bank_Account::operator-(double amt) {
	balance -= amt;
	this->print_to_file();
}

char Bank_Account::getAcccountType(){
	return Acct_Type;
}

long Bank_Account::get_num(){
	return Account_Num;
}

//gets account number, userID, password from user, creates account
Bank_Account::Bank_Account(){
	
	string in;
	ifstream myfile;
	char filename[30];
	int i = 0;
	cout << "Thank you for making an account with Online Banking Inc." << endl;
	cout << "First we need to gather some information from you" << endl;
	cout << "What 8-digit acccount number would you like to have? Please enter without spaces." << endl;
	while(i==0){
		getline(cin,in);
		Account_Num = check_num(in);
		sprintf(filename,"%08li.txt",Account_Num);
		myfile.open(filename);
		if(Account_Num > 99999999 || Account_Num == 0){
			cout << "Invalid choice of Account Number! Must be a numbers and 8-digits or less. Please try again:" << endl;
		}
		else if(myfile.is_open()){
			cout << "This Account already exists. If you would like to login to this account please select the 'Login as Customer' option from the main menu" << endl;
			throw 1;
		}
		else{
			myfile.close();
			i = 1;
		}
	}
	i = 0;
	cout << "Enter your userID. Please no spaces." << endl;
	while(i == 0){
		getline(cin,in);
		
		userID = atol(in.c_str());
		if(userID <= 0){
			cout << "Invalid choice for Used ID please try agaiin" << endl;
			cout << "Enter a User ID that is a positive number without spaces. " << endl;
		}
		else{
			i = 1;
		}
	}
	cout << "Enter your password you would like to have for this account:" << endl;
	getline(cin,password);
	
	frozen = 0;
	cout << "Your starting balance will be $1000!" << endl;
	balance = 1000.00;
	
}

//if account exists, gets info from its file
Bank_Account::Bank_Account(long acct_num){

	ifstream myfile;
	char filename[30];
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
	getline(cin,in);
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
		myfile << "Previous Balance: " << balance - value << "  ";
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
	if ( myfile.peek() == std::ifstream::traits_type::eof() ){
		cout << "This Account does not have a transaction log yet." << endl;
		throw 1;
	}
	while(getline(myfile,in)){
		cout << in << endl;
	}
}

//checks if account exists
int Bank_Account::does_exist(){
	
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
	getline(cin,amount);
	n = atof(amount.c_str());
	if(n <= 0){
		throw 1;
	}
	
	this->operator+(n);
	cout << "The new balance of account - " << Account_Num << "is: $" << balance << endl;
	//updates file with new balance
	this->print_to_translog(n,'D');
}

//withdraw from account
void Bank_Account::withdraw() throw(int,char){
	double n = 0;
	string amount;
	cout << "How much would you like to withdraw?" << endl;
	getline(cin,amount);
	n = atof(amount.c_str());
	if(n <= 0){
		throw 1;
	}
	if (balance - n > 0) {//if funds are available
		this->operator-(n);
		cout << "$" << amount << " withdrawn from account successfully";
	}
	else {
		throw 'a';
	}
	//updates file with new balance
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
	Bank_Account* temp;
	try{
		temp = new Bank_Account(acct_num);
	}
	catch(int i){
		cout << "Account does not exist, transfer cancelled." << endl;
		return;
	}
	if(temp->Account_Num == this->Account_Num){
		cout << "Cannot transfer into current account" << endl;
	}
	else {
		string amountStr;
		double amount = 0;
		cout << "How much would you like to transfer? Please enter value with no spaces. Cannot be $0.00" << endl;
		getline(cin, amountStr);
		sscanf(amountStr.c_str(), "%lf", &amount);
		if (amount < 0) {
			cout << "Amount cannot be negative." << endl;
		}
		else if(amount == 0){
			cout << "Amount may not be 0 or an Invalid input. Please enter positive numbers only." << endl;
		}
		else if (amount > this->balance) {
			cout << "Insufficient funds for transfer." << endl;
		}
		else {
			*(temp) + amount;
			this->operator-(amount);
			temp->print_to_translog(amount, 'T');
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
		void Calc_Predicted_Interest(){}; //empty function
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
		virtual void transfer(long acct_num);
};

//sets rate
Saving_Acct::Saving_Acct(long acct_num) : Bank_Account(acct_num){
	this->Get_rate();
}

//sets type to C, sets rate
Saving_Acct::Saving_Acct() : Bank_Account(){
	Acct_Type = 'S';
	this->Get_rate();
	cout << "\nThe Interest Rate set for this account is: " << Interest_Rate*100 << "%" << endl;
}

//deposit to account
void Saving_Acct::deposit() throw(int){
	string amount;
	double n = 0;
	cout << "\nHow much would you like to deposit?" << endl;
	getline(cin,amount);
	n = atof(amount.c_str());
	if(n <= 0){
		throw 1;
	}
	this->operator+(n);
	cout << "\nThe new balance of account: " << Account_Num << " is: " << balance << endl;
	//get new rate
	this->Get_rate();
	this->print_to_translog(n,'D');
}

//withdraw from account
void Saving_Acct::withdraw() throw(int,char){
	string amount;
	double n = 0;
	cout << "\nHow much would you like to withdraw?" << endl;
	getline(cin,amount);
	n = atof(amount.c_str());
	if(n <= 0){
		throw 1;
	}
	if (balance - n >= 0){//if funds are available
		this->operator-(n);
		cout << "$" << amount << " withdrawn from account successfully" << endl;
		//set new rate
		this->Get_rate();
		this->print_to_translog(n,'W');
	}
	else {
		throw 'a';
	}
	
}

void Saving_Acct::transfer(long acct_num){
	Bank_Account* temp;
	try{
		temp = new Bank_Account(acct_num);
	}
	catch(int i){
		cout << "Account does not exist, transfer cancelled." << endl;
		return;
	}
	if(temp->get_num() == this->Account_Num){
		cout << "Cannot transfer into current account" << endl;
	}
	else {
		string amountStr;
		double amount = 0;
		cout << "How much would you like to transfer? Please enter value with no spaces. Cannot be $0.00" << endl;
		getline(cin, amountStr);
		sscanf(amountStr.c_str(), "%lf", &amount);
		if (amount < 0) {
			cout << "Amount cannot be negative." << endl;
		}
		else if(amount == 0){
			cout << "Amount may not be 0 or an Invalid input. Please enter positive numbers only." << endl;
		}
		else if (amount > this->balance) {
			cout << "Insufficient funds for transfer." << endl;
		}
		else {
			*(temp) + amount;
			this->operator-(amount);
			temp->print_to_translog(amount, 'T');
			this->print_to_translog((amount * -1), 'T');
			cout << "Amount transferred successfully." << endl;
		}
	}
	this->Get_rate();
}

//print balance
void Saving_Acct::check_balance(){
	cout << "\nYour current balance is: $" << balance << endl;
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
	string t;
	long n;
	cout << "\nFor how many years would you like to see predicted interest earned on this account? Please neter the number of years:" << endl;
	getline(cin,t);
	n = check_num(t);
	if(n == 0){
		cout << "\nInvalid Input for years. Returning to User Menu." << endl << endl;
		return;
	}
	//interest calculation
	temp = balance* pow(1+Interest_Rate,n);
	cout << "\nYour expected account balance after " << n << " years is: $" << temp << endl << endl;
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
		cout << "Please enter your password." << endl;
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
	Bank_Account* temp;
	try{
		temp = new Bank_Account(Account_Num);
	}
	catch(int r){
		return;
	}
	if(temp->does_exist()==0){
		return;
	}
	temp->frozen = 1;
	temp->print_to_file();
	
}

//same as freeze's logic, but opposite function. Sets frozen to 0 instead of 1
void Manager_Acct::unfreeze(long Account_Num) {
	Bank_Account* temp;
	try{
		temp = new Bank_Account(Account_Num);
	}
	catch(int r){
		return;
	}
	if(temp->does_exist()==0){
		return;
	}
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
	string filename = "Rates.txt";
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
		cout << "For accounts with a balance > $" << ranges[i] << " the rate is:" << rates[i]*100 << "%" << endl; 	
		//print rate/range at i (I think this would be fine as the last line in the first for loop)
	}
}

//adjust interest rates
void Manager_Acct::Adjust_Rate(){
	string in;
	int i = 0;
	float rate;
	int range;
	int choice;
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
	getline(cin,in);
	if (check_input(in) == 0) {
			choice = -1; //force to go to default
		}
		else {
			sscanf(in.c_str(), "%d", &choice);
		}
	//go to selected range
	switch(choice){
		case 1:
			cout << "Enter the new rate as a decimal for accounts with a balance between $4999 - $1000:" << endl;
			getline(cin,in);
			temp = check_rate(in);
			if(temp == -1)
				break;
			rates[0] = temp;
		break;
		
		case 2:
			cout << "Enter the new rate as a decimal for accounts with a balance between $5000 - $9999:" << endl;
			getline(cin,in);
			temp = check_rate(in);
			if(temp == -1)
				break;
			rates[1] = temp;
		break;
		
		case 3:
			cout << "Enter the new rate as a decimal for accounts with a balance between $10000 - $99999:" << endl;
			getline(cin,in);
			temp = check_rate(in);
			if(temp == -1)
				break;
			rates[2] = temp;
		break;
		
		case 4:
			cout << "Enter the new rate as a decimal for accounts with a balance above $100000:" << endl;
			getline(cin,in);
			temp = check_rate(in);
			if(temp == -1)
				break;
			rates[3] = temp;
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
	char type_choice, type;
	string choiceStr, choiceStr2;
	Bank_Account* Account;
	Manager_Acct M;
	string acct_str, freeze_str;
	long acct_num, freeze_num;
	int a = 0, b = 0, c = 0, z = 0, u =0, k = 0, choice = -1, choice2 = -1;
	cout << "\nWelcome to Online Banking Inc." << endl;
		cout << "------------------------------------------------" << endl << endl;
	while(a == 0){
		cout << "\nPlease select an option to begin" << endl
			<< "1.) Login as Manager" << endl
			<< "2.) Login as Customer" << endl
			<< "3.) Create New Customer Account" << endl
			<< "4.) Exit Program" << endl;
		getline(cin,choiceStr);
		if (check_input(choiceStr) == 0) {
			choice = -1; //force to go to default
		}
		else {
			sscanf(choiceStr.c_str(), "%d", &choice);
		}
		//go to choice
		switch(choice){
			//manager
			case 1:
			cout << "\nWelcome! Please enter your employee number without spaces, or enter -1 to cancel:" << endl;//attempt to login
			getline(cin,in);
			acct_num = check_num(in);
			if(acct_num == 0){
				cout << "\nAccount number not entered, returning to main menu." << endl;
				break;
			}
			b = M.manager_login(acct_num);
			//successful login
			if(b==1){
				cout << "\nWelcome Manager!" << endl;
				cout << "------------------------------------------------" << endl << endl;
				k = 0;
				while(k == 0){
					cout << "\nWhat would you like to do?" << endl;
					cout << "1.) Freeze Customer Account" << endl;
					cout << "2.) Un-Freeze Customer Account" << endl;
					cout << "3.) View Current Interest Rates" << endl;
					cout << "4.) Adjust Interest Rates" << endl;
					cout << "5.) Log Out" << endl;
					choiceStr = ""; //reset values
					choice = -1;
					getline(cin,choiceStr);
					if (check_input(choiceStr) == 0) {
						choice = -1; //-1 will go to default in the switch
					}
					else {
						sscanf(choiceStr.c_str(), "%d", &choice);//string is only one character long. If it is an int, choice will use it. Otherwise, choice is -1 and will go to default
					}
					switch(choice){
						//freeze
						case 1:
						cout << "\nEnter the Account Number of the Account you would like to freeze without spaces, or enter -1 to cancel." << endl;
						getline(cin,in);
						freeze_num = check_num(in);
						if(freeze_num == 0){
							cout << "\nInvalid input for account number! Returning to manager menu." << endl;
							break;
						}
						
						M.freeze(freeze_num);
						break;
						//unfreeze
						case 2:
						cout << "\nEnter the Account Number of the Account you would like to unfreeze:" << endl;
						getline(cin,in);
						freeze_num = check_num(in);
						if(freeze_num == 0){
							cout << "\nInvalid input for account number! Returning to manager menu." << endl;
							break;
						}
					
						M.unfreeze(freeze_num);
				
						break;
						//view interest rates
						case 3:
						cout << "\nDisplaying the current Interest rates..." << endl;
						M.Current_Rates();
						break;
						//adjust interest rates
						case 4:
						M.Adjust_Rate();
						break;
						//exit
						case 5:
						cout << "\nReturning to main menu..." << endl;
						cout << "------------------------------------------------" << endl << endl;
						k = 1;
						break;
						//invalid choice
						default:
						cout << "\nInvalid choice! Please try again" << endl;
						break;
					}
				}
				
			}
			//login unsuccessful
			else{
				cout << "\nLogin failed...." << endl;
				cout << "\nPlease try again." << endl;
			}
			break;
			//existing user
			case 2:
				cout << "\nWelcome User! Please enter your Account Number, or enter -1 to cancel:" << endl;//attempt to login, need to add password check still
				getline(cin,in);
				acct_num = check_num(in);
				if(acct_num == 0){
					cout << "\nAccount number not entered, returning to main menu." << endl << endl;
					break;
				}
				type = user_acct(acct_num);
				//account is closed
				if(type == 'F'){
					cout << "\nThis account number does not exist." << endl;
					cout << "\nIf you would like to create this account please select 'Create Account' from the main menu" << endl;
					break;
				}
				//savings
				else if(type == 'S'){
					try{
						Account = new Saving_Acct(acct_num);
						Account->check_password();
					}
					catch(int){
						cout << "\nIncorrect Password for account! Returning to main menu." << endl << endl;
						break;
					}
					
					if(Account->frozen == 1){
						cout << "\nThis account is frozen. Please speak to a Bank Manager to unfreeze this account." << endl;
						cout << "\nReturing to main menu" << endl;
						cout << "------------------------------------------------" << endl << endl;
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
						cout << "\nIncorrect Password for account! Returning to main menu." << endl << endl;
						break;
					}
				
					if(Account->frozen == 1){
						cout << "\nThis account is frozen. Please speak to a Bank Manager to unfreeze this account." << endl;
						cout << "\nReturing to main menu" << endl;
						break;
					}
				}
				//add menu for user here and switch statement
				cout << "\nLogin Successful!" << endl;
				cout << "------------------------------------------------" << endl << endl;
				z = 0;
				while(z == 0){
					cout << "\nWhat would you like to do? Please select one of the following options:" << endl;
					cout << "1.) View Account Balance" << endl
					<< "2.) Deposit into Account" << endl
					<< "3.) Withdraw from Account" << endl
					<< "4.) Create Transfer" << endl
					<< "5.) View Transaction Log" << endl
					<< "6.) Calculate Predicted Interest" << endl
					<< "7.) Close Account" << endl
					<< "8.) Log Out" << endl;
					getline(cin,choiceStr2);
					if (check_input(choiceStr2) == 0) {
						choice2 = -1; //force to go to default
					}
					else {
						sscanf(choiceStr2.c_str(), "%d", &choice2);
					}
					switch(choice2){
						//check balance
						case 1:
						Account->check_balance();
						break;
						//deposit
						case 2:
						try{
							Account->deposit();
						}
						catch(int e){
							cout << "\nInvalid value for amount to deposit into account! Returning to User menu." << endl << endl;
						}
						break;
						//withdraw
						case 3:
						try{
							Account->withdraw();
						}
						catch(int e){
							cout << "\nInvalid value for amount to withdraw from account! Returning to User menu." << endl << endl;
						}
						catch(char q){
							cout << "\nInsuficient funds for withdraw! Please try again" << endl << endl;
						}
						break;
						//transfer
						case 4:
							cout << "\nWhat is the account number you would like to transfer to?" << endl;
							getline(cin, acct_str);
							acct_num = check_num(acct_str);
							if(acct_num == 0){
								cout << "\nInvalid input for account number! Returning to User Menu." << endl;
								break;
							}
							Account->transfer(acct_num);
						break;
						//transaction log
						case 5:
							try{
								Account->print_translog();
							}
							catch(int){
								cout << "\nReturning to user menu." << endl;
								cout << "------------------------------------------------" << endl << endl;
							}
						break;
						
						case 6:
							if(Account->getAcccountType() != 'S'){
								cout << "\nInterest can only be calculated on Savings Accounts. Returning to User Menu." << endl;
								cout << "------------------------------------------------" << endl << endl;
								break;
							}
							else{
								Account->Calc_Predicted_Interest();
							}
						break;
						
						//close account
						case 7:
						cout << "\nAre you sure you would like to close your account? Enter '1' for yes or '2' for no." << endl;
						u = 0;
						while(u==0){
							choiceStr = ""; //reset values 
							choice = -1;
							getline(cin,choiceStr);
							if (check_input(choiceStr) == 0) {
								choice = -1; //force to go to default
							}
							else {
								sscanf(choiceStr.c_str(), "%d", &choice);
							}
							switch(choice){
								case 1:
									cout << "\nClosing account..." << endl;
									Account->close_Acct();
									cout << "\nClosing Online Banking Inc. Thank you!" << endl;
									cout << "------------------------------------------------" << endl << endl;
									return 0;
								break;
							
								case 2:
									cout << "\nAccount not closed. Returning to User menu." << endl;
									u = 1;
								break;
								
								default:
									cout << "\nIncorrect choice please try again." << endl;
								break;
							}
						}
						break;
						//exit
						case 8:
						cout << "\nReturning to main menu..." << endl;
						cout << "------------------------------------------------" << endl << endl;
						z = 1;
						break;
						//default
						default:
						cout << "\nInvalid Choice! Please try again" << endl;
						break;
					}
				}
			
			break;
			//new user
			case 3:
			cout << "\nWelcome!" << endl;
			cout << "------------------------------------------------" << endl << endl;
			cout << "To create your account we need to gather some information from you" << endl;
			cout << "\nWhat type of Account woudl you like to open? Savings or Checking?" << endl;
			c=0;
			while(c == 0){
				in = "";
				cout << "Enter 'S' for Savings Account or 'C' for Checking Account:" << endl;
				getline(cin, in);
				type_choice = in.at(0);
				
				switch(type_choice){
					case 'S':
					try{
						Account = new Saving_Acct();
					}
					catch(int i){
						c=2;
						break;
					}
					c = 1;
					break;
					
					case 'C':
					try{
						Account = new Checking_Acct();
					}
					catch(int i){
						c=2;
						break;
					}
					c = 1;
					break;
					
					default:
					cout << "\nInvalid choice! Please Try again" << endl;
					break;
				}
			}
			if(c==2){
				cout << "\nReturning to Main Menu" << endl; 
				cout << "------------------------------------------------" << endl << endl;
				break;
			}
			//create file
			Account->print_to_file();
			z = 0;
			while(z == 0){
					cout << "\nWhat would you like to do? Please select one of the following options:" << endl;
					cout << "1.) View Account Balance" << endl
					<< "2.) Deposit into Account" << endl
					<< "3.) Withdraw from Account" << endl
					<< "4.) Create Transfer" << endl
					<< "5.) View Transaction Log" << endl
					<< "6.) Calculate Predicted Interest" << endl
					<< "7.) Close Account" << endl
					<< "8.) Log Out" << endl;
					getline(cin,choiceStr2);
					if (check_input(choiceStr2) == 0) {
						choice2 = -1; //force to go to default
					}
					else {
						sscanf(choiceStr2.c_str(), "%d", &choice2);
					}
					switch(choice2){
						//check balance
						case 1:
						Account->check_balance();
						break;
						//deposit
						case 2:
						try{
							Account->deposit();
						}
						catch(int e){
							cout << "\nInvalid value for amount to deposit into account! Returning to User menu." << endl << endl;
						}
						break;
						//withdraw
						case 3:
						try{
							Account->withdraw();
						}
						catch(int e){
							cout << "\nInvalid value for amount to withdraw from account! Returning to User menu." << endl << endl;
						}
						catch(char q){
							cout << "\nInsuficient funds for withdraw! Please try again" << endl << endl;
						}
						break;
						//transfer
						case 4:
							cout << "\nWhat is the account number you would like to transfer to?" << endl;
							getline(cin, acct_str);
							acct_num = check_num(acct_str);
							if(acct_num == 0){
								cout << "\nInvalid input for account number! Returning to User Menu." << endl;
								break;
							}
							Account->transfer(acct_num);
						break;
						//transaction log
						case 5:
							try{
								Account->print_translog();
							}
							catch(int){
								cout << "\nReturning to user menu." << endl;
								cout << "------------------------------------------------" << endl << endl;
							}
						break;
						
						case 6:
							if(Account->getAcccountType() != 'S'){
								cout << "\nInterest can only be calculated on Savings Accounts. Returning to User Menu." << endl;
								cout << "------------------------------------------------" << endl << endl;
								break;
							}
							else{
								Account->Calc_Predicted_Interest();
							}
						break;
						
						//close account
						case 7:
						cout << "\nAre you sure you would like to close your account? Enter '1' for yes or '2' for no." << endl;
						u = 0;
						while(u==0){
							choiceStr = ""; //reset values 
							choice = -1;
							getline(cin,choiceStr);
							if (check_input(choiceStr) == 0) {
								choice = -1; //force to go to default
							}
							else {
								sscanf(choiceStr.c_str(), "%d", &choice);
							}
							switch(choice){
								case 1:
									cout << "\nClosing account..." << endl;
									Account->close_Acct();
									cout << "\nClosing Online Banking Inc. Thank you!" << endl;
									cout << "------------------------------------------------" << endl << endl;
									return 0;
								break;
							
								case 2:
									cout << "\nAccount not closed. Returning to User menu." << endl;
									u = 1;
								break;
								
								default:
									cout << "\nIncorrect choice please try again." << endl;
								break;
							}
						}
						break;
						//exit
						case 8:
						cout << "\nReturning to main menu..." << endl;
						cout << "------------------------------------------------" << endl << endl;
						z = 1;
						break;
						//default
						default:
						cout << "\nInvalid Choice! Please try again" << endl;
						break;
					}
				}
			break;
			
			case 4:
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

