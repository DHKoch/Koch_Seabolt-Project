//Dalton Koch
//Thomas Seabolt
//ECE 3220: Software Design in C/C++
//Final Project
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

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
		void Current_Rates();
		void Adjust_Rate();
}

int main(void){
	/*
	read account number from input
	check to see if manager account number
		if yes check 
	
	*/
	return 0;
}

