
import java.util.Scanner;
class ElapsedSeconds {
   public static void main(String[] args) {
      int dataInSeconds, hours, minutes, seconds;

      Scanner s = new Scanner(System.in);

      /* This portion of the code will ask the user to enter the elapsed time
       * in seconds. We will then use the scanner 's' to have the user input 
       * the elapsed time in seconds. We will then close the scanner as we
       * are done having the user input data.
       */
      System.out.print("Enter the elapsed time in seconds: ");
      dataInSeconds = s.nextInt();
      s.close();

      /* This portion of the code will divy up the dataInSeconds variable into
       * hours, minutes, and seconds respectively. I used 3600 because if there
       * are more than or equal to 3600 elapsed seconds the hours will register
       * as 1. For minutes I have to acknowledge what was taken by the hours,
       * so I take the remainder of what hours has and 
       * then divide that by 60, 60 seconds in a minute. And for seconds I want the 
       * remainder rather than the whole number of minutes.
       */
      hours =  dataInSeconds / 3600; 
      minutes = (dataInSeconds % 3600) / 60; 
      seconds = (dataInSeconds % 3600) % 60;

      System.out.format("%d seconds is %d hours, %d minutes, %d seconds.%n", dataInSeconds, hours, minutes, seconds);
      //Using system.out.format we can use '%d' which stands for digit, to plug-in our integers. Always end on a new line '%n'
   }
}