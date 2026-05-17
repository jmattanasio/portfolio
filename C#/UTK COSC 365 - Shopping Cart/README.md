Overview
The purpose of this program is to give you practice reading different data types from a file and outputting a report in a clean format using interpolated strings.

The first thing your program will need to do is ask the user for a shopping cart file name, which will be prompted with "Enter shopping cart file: ".

If the file could not be opened output the exception message and quit.

Otherwise, read each element from the file. The file’s format will be as follows:

<price> <item name multiple words>
The price will be a double, and the item name will be a string of multiple words.

Your job is to calculate the total cost of each item as well as the total cost of a cart.

When giving the total cost of the items, list the items by total cost in descending order. If two items have the same total cost, sort by name in alphabetic order.

You can use the .Sort() function in the List, but you will need to implement the interface IComparable<Item>. The method you will need to implement is CompareTo. This is where you can tell C# how to sort the Item objects.

Item Class
Create a class called Item. It will contain the name of one item as well as its total value. Each item will be a unique name, but as you can see the private member _items is a list of Item objects. So, if the list already contains the item, you just need to update the total price.

IComparable<Item>
You will need to implement IComparable<Item> so that when you sort the list, it will sort it according to the the rules above.

Output Format
Print the items to the Console using the following format:

The item’s name in a left-justified field, 25 characters wide.

A space and a dollar sign $

The total cost of the items in a right-justified field, 7 characters wide. The output needs to be in fixed notation with a 2-digit precision.

After all of the items are printed, print the total.

The string "Total = $".

The total of all of the items in fixed notation with a 2-digit precision.

Examples
Input File
12.50 John Dole
1.75 Beef Tips
0.75 Beef Tips
Output
Enter shopping cart file: test1.cart
John Dole                  $  12.50
Beef Tips                  $   2.50
Total = $15.00
Input File
12.50 John Dole
1.75 Beef Tips
10.75 Beef Tips
Output
Enter shopping cart file: test2.cart
Beef Tips                  $  12.50
John Dole                  $  12.50
Total = $25.00