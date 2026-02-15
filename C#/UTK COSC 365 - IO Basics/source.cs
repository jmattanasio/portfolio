using System.Text.RegularExpressions;
class Basic{
    public static void Main(string[] args){
    string input = Console.ReadLine() ?? string.Empty;
    if(input == "" || !Regex.IsMatch(input, @"^.\s+-?\d+\s+-?\d+$")){
        Console.WriteLine("Error reading line.");
        return;
    }
    string[] arithmetic = input.Split(' ');
    int num1 = Convert.ToInt32(arithmetic[1]);
    int num2 = Convert.ToInt32(arithmetic[2]);

    switch(arithmetic[0]){
        case "*":
            Console.WriteLine(num1 * num2);
            break;
        case "+":
            Console.WriteLine(num1 + num2);
            break;
        case "-":
            Console.WriteLine(num1 - num2);
            break;
        case "/":
            Console.WriteLine(num1 / num2);
            break;
        case "%":
            Console.WriteLine(num1 % num2);
            break;
        default:
            Console.WriteLine($"Invalid operator {arithmetic[0]}.");
            break;
        }
    }
   
}