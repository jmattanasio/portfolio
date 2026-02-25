import java.util.Scanner;
class AdvancedCalc {
   public static void main(String[] args) {
    //Just variable definition and Scanner initializing
        int left, right;
        String operator;
        int result;
        Scanner s = new Scanner(System.in);
//Asking the user for an int, string, int and storing those as our declared variables.
//Close the scanner as we are done with the inputs from the user.
        System.out.print("Enter <left> <op> <right>: ");
        left = s.nextInt();
        operator = s.next();
        right = s.nextInt();
        s.close();

        /*The thing that we're evalutaing for different cases is the operator.
         * That's why the syntax is switch(operator). I used system.out.format 
         * because I think a) it is cleaner than concatenation and b) allowed for
         * a plug and play for each case regardless of operator. It is more fluid
         * than a hard coded output. Note that each case rather than being the operator
         * without quotes, it needs quotes since it is a string. %n and println so that
         * each time the code ends we get a new clean line.
        */

        switch(operator){
            case "+":
                result = left + right;
                System.out.format("%d %s %d = %d%n", left, operator, right, result);
                break;
            case "-":
                result = left - right;
                System.out.format("%d %s %d = %d%n", left, operator, right, result);
                break;
            case "/":
                result = left / right;
                System.out.format("%d %s %d = %d%n", left, operator, right, result);
                break;
            case "*":
                result = left * right;
                System.out.format("%d %s %d = %d%n", left, operator, right, result);
                break;
            case "%":
                result = left % right;
                System.out.format("%d %s %d = %d%n", left, operator, right, result);
                break; 
            case "^":
                result = (int)(Math.pow((double)left, (double)right));
                System.out.format("%d %s %d = %d%n", left, operator, right, result);
                break;
            default:
                System.out.println("Invalid Operator!");
        }
        
   }
}