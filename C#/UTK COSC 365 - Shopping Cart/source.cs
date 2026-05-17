using System;
using System.IO;
using System.Collections.Generic;

class Cart {
    public static void Main(string[] nothing) {
        Cart cart = new Cart();
    }
    private List<Item> _items;
    public Cart() {
        _items = new List<Item>();
        double totalSum = 0;
        Console.Write("Enter shopping cart file: ");
        string filename = Console.ReadLine()?.Trim() ?? string.Empty;
        using(StreamReader sr = new StreamReader(filename)){
            string? line;
            while ((line = sr.ReadLine()) != null){
                string[] data = line.Split(new[] { ' ' }, 2, StringSplitOptions.RemoveEmptyEntries);
                string itemName = data[1];
                double cost = double.Parse(data[0]);
                totalSum += cost;
                var existingItem = _items.Find(i=> i.Name == itemName);
                if (existingItem != null){
                    existingItem.Cost += cost;
                }else{
                    _items.Add(new Item(cost, itemName));
                }
            }
        }
        _items.Sort();
        foreach (var item in _items) {
            string formattedprice = item.Cost.ToString("F2");
            Console.Write($"{item.Name,-25}");
            Console.Write("  $");
            Console.Write($"{formattedprice,7}\n");
        }
        Console.WriteLine($"Total = ${totalSum:F2}");
        
        
        

    }
    class Item : IComparable<Item> {
        public double Cost {get; set;}
        public string Name{get;}

        public Item(double cost, string itemName){
                Cost = cost;
                Name = itemName;
        }

        public int CompareTo(Item? other){
            if (other == null) return 1;
            int result =  other.Cost.CompareTo(Cost);
            if(result == 0){
                result = Name.CompareTo(other.Name);
            }
            return result;
        }
    }
}