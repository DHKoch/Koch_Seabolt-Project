# Online Banking Inc. Application

## Developers:
Dalton Koch,
Thomas Seabolt

### General Description
The Online Banking Inc. Application is a program that immitates a potentially real online banking application. The user has the option to login as the Manager or as a Customer. The user may also create an account with the bank through the application. The Manager is asked to enter the employee number and password that corresponds to the manager account. The manager does not have a file associated with the information, instead the manager information is hard coded into the program. Once logged in the manager can freeze user accounts or unfreeze them. The manager can also view and adjust the interest rates that effect the savings account in the system. When the manager is finished with his tasks he may log out and return to the main menu. 

If a customer wishes to login, the user will be asked to enter the account number for the specific account they would liek to login to. if the account number is valid the user will be asked to enter the password for the account. If the information for the user login is correct the menu for the user is displayed. The customer has the options of: View Balance, Deposit, Withdraw, Transfer, View Transaction Log, CLose Account and exit to the main menu. Viewing the balance of the account will display the current balance. If the account is a savings account then the current interest rate will also be displayed. If the user wishes to deposit into the account the amount input is checked and if correct will be deposited and added to the transaction lag. The withdraw option checks the amount input and also checks if the current balance is mor than the requested withdraw amount so that the account does not become overdrawn. If the amount requested passes the checks the transaction will proceed and be added to the transaction log. The transfer option will ask the user to enter the account number of the account they would liek to transfer to. If the account exists the customer is asked to enter the amount they would like to transfer. the amount entered goes through the same checks as if it were a withdraw. if the amount passes the checks the transfer will happen and both transaction logs of each accoutn will be updated. If the customer wishes to close the account they will be prompted again as to make sure that they want to close the account. If they do wish to close the account the information in the account will be cleared and the user will be exited to the main menu. 

If the user would like to make an account they will be asked to enter the desired information for the account they would like to create. once the accoutn has been created they are presented with the same menu as the logged on Customer explained above.

### Directions for Use
The beginning menu allows for any type of input except for spaces between characters. If you would like to login as the manager the employee number is '12345678' and the password is 'password123'. The manager account does not have a seperate file, all the manager information is hard coded into the program. If the employee number or password is incorrect you will be returned to the user menu. Once the correct info is entered the next menu is the user menu. If you selecte to freeze a customer account you will be asked to enter the account number for the account you woudl like to freeze. The available accounts are '11111111', '22222222' '11113333', '12121234' and any of the files in the repository. If the account number is valid the frozen boolean varible for the account is set to 1 and that account can no longer be signed into. The manager can also unfreeze customer accounts which follows a similar process as above. The manager can view the current interest rates that are for th set ranges of account balances. The manager can also change the interest rates through the 'Adjust Interest rates' option. This option allows the manager to select the account balance range they would like to change and set the new rate. The rate that you enter shoudl be in the decimal form, for example "0.04" will set the rate to 4%. if you would like to change the rate to 0% you will be asked if you are sure as a form of double checking. any incorrect input is handled as long as there are no spaces between characters.  

The customer login asks the user for their account number. The program accepts alll types of inputs as ong as no spaces are entered. If the account does not exists an error will be displayed. The user is then asked to enter the password for that account. If the account password does not match the user password then the program returns to the main menu. the available accoutn numbers an passwords can be found in the account files in th repository or you can create your own to test with. Once the login information is correct a menu is then displayed with the available options as mentioned in the general description above. The deposit reads in the amount from the user and makes sure it is a valid input. If the input is valid the amount will be added to the account balance. The results of this can be seen by selecting the view balance option before and after the deposit. The same process can be done with the withdraw option. All transactions are recorded in the transaction log and can be viewed to confirm the results. The transfer option requires the user to input the account number of the account they wish to transfer funds to. This account number is checked to make sure that it is not account that the transfer is being sent from. The amoutn beign transferred is also check and if valid the transfer is preformed and added to the transaction log. If you would like to test the close account option you will be exited to the main menu and unable to log into the account that you closed. transfers will also not be allowed to the close account.  

The option to create an account will ask you to enter in the desired account number. This number is checked for valid input and whether or not the account number is already in use. The user is then asked to enter the desired user ID and the account file is created. The starting balance in $1000.00. The user then has the same menu as a loggeed in user. when the user is done they can log out and they will be returned to the main menu. 