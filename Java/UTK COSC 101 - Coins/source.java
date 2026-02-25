


import java.util.Scanner;
class Coins {
    public static void main (String[] args){
        /*I am going to store the user input in 'dataInChange'
         * I also made variables to store each of the transactions of 
         * quarters, dimes, nickels, and pennies.
         * And then the value of each coin is defined as well.
        */

        int dataInChange, numQ, numD, numN, numP;
        int qValue=25, dValue=10,nValue=5;
        
        /*This block of code will ask the user to input
         * an amount of change ranging from 0-99.
         * We then close the scanner since this is the 
         * only user input we will accept during this program.
         */
        Scanner myScanner = new Scanner(System.in);
        System.out.print("Enter the amount of change (0-99): ");
        dataInChange = myScanner.nextInt();
        myScanner.close();

        /*This is the calculation block of code where we will
         * divide the change into its correct coin amounts.
         * The number of quarters is equal to the change the user
         * inputed divided by 25, and of course in the event that the 
         * output of that is a decimal we only take the whole number.
         * The number of dimes takes the remainder of the quarters transaction
         * and then divides that by 10. The number of nickels acknowledges
         * both previous transactions and simply takes the remainder of those 
         * transactions and divides by 5. And all that is leftover is allocated
         * to pennies. With modulo and integer division stacking, you can just 
         * constantly take the remainder then divide. 
         */
        numQ =  dataInChange / qValue; 
        numD = (dataInChange % qValue) / dValue ; 
        numN = ((dataInChange % qValue)% dValue) / nValue;
        numP = ((dataInChange % qValue)% dValue) % nValue;


        /*We use system.out.format in order to plug and play the calculated values. 
        * '%d%n' simply means that after each digit there will be a new line. That's
        * why there is no space between the %n and the next string of text. So, our
        * output will be four separate lines with integers at the end, indicating each
        * amount of which coin is needed to equal the user's inputed change amount.
         */
        System.out.format("Number of quarters: %d%nNumber of dimes: %d%nNumber of nickels: %d%nNumber of pennies: %d%n", numQ, numD, numN, numP);
    }
}
