

import java.util.Scanner;

class forloops {
   public static void main(String[] args) {
      String input;
      int n;
      double total = 0;
      Scanner s = new Scanner(System.in);

      /*For the for loop i simply chose a large enough number so that it will
       * inherently run forver, until our user decides to quit the program.
       */
      for(int i = 0; i < 9999999; i++) {
         System.out.print("Sum or product (type 'quit' to quit): ");
      //Once the user types the string 'quit' the seemingly infinite for loop breaks.
         input = s.next();
         if (input.equals("quit")) {
            break;
         }
         else if (!input.equals("sum") && !input.equals("product")) {
            System.out.println("Invalid operation, try again.");
            continue;
         }
         System.out.print("How many values? ");
         n = s.nextInt();

         /*  This is the for loop that asks for a value and then instantly 
         adds the inputted value to the running total*/
         if (input.equals("sum")) {
            for(int p = 0; p < n; p++) {
               System.out.print("Enter value #" + p + ": ");
               total += s.nextDouble();
            }
            System.out.format("Sum %.3f%n", total);
         }
         else {
            total = 1; 
            /*Same concept for product however we cannot multiply by zero hence the 
             * total = 1 above.
            */
            for(int y = 0; y < n; y++) {
               System.out.print("Enter value #" + y + ": ");
               total *= s.nextDouble();
            }
            System.out.format("Product %.3f%n", total); // Rounding to three decimals with a newline character
         }
      }
      s.close();
   }
}
