
import java.util.Scanner;
class NumbersMain {
   public static void main(String[] args) {
      boolean isrunning = true;
      Statistics mystats = new Statistics(); //This creates a new instance of the statistics class. 
      Scanner s = new Scanner(System.in);
      //This do-while loop only runs while "quit" has not been entered by the user. 
      //Once it is entered however the loop ends, scanner closes and we print our data.

      do {
         do {
            System.out.print("Enter an integer (\"quit\" to quit): ");
            if (!s.hasNextInt()) {
               if (s.next().equals("quit")) {
                  isrunning = false;
                  break;
               }
               System.out.println("You did not enter an integer, try again.");
            }
            else {
               mystats.addValue(s.nextInt());// This adds the next integer value from the scanner to mystats. //
               break;
            }
         } while (true);
      } while (isrunning);
      s.close();
      
      /*This block of code calls all the functions to relay the data given via the .addValue on line 18. 
       * This only shows up once the user is done by entering 'quit'.
       */
      System.out.format("Number of values entered = %d%n", mystats.getNumValues());
      System.out.format("Sum of all values        = %d%n", mystats.getSum());
      System.out.format("Product of all values    = %d%n", mystats.getProduct());
      System.out.format("Biggest value            = %d%n", mystats.getMax());
      System.out.format("Smallest value           = %d%n", mystats.getMin());
      System.out.format("Average of all values    = %.2f%n", mystats.getAverage());
   }
}