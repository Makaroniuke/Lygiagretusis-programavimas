using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Lab1a
{
    class Plane
    {
        public string Name { get; set; }
        public int Quantity { get; set; }
        public double Price { get; set; }
        public int Result { get; set; }


        //public Plane(string name, int quantity, double price)
        //{
        //    this.Name = name;
        //    this.Quantity = quantity;
        //    this.Price = price;
        //}




        public int CountFibonacci(int number)
        {
            if (number == 0) return 0; //To return the first Fibonacci number   
            if (number == 1) return 1; //To return the second Fibonacci number   
            return CountFibonacci(number - 1) + CountFibonacci(number - 2);
        }

        internal int CompareTo(Plane item)
        {
            return this.Result.CompareTo(item.Result);
        }

        public int ReturnResult()
        {
            return CountFibonacci(this.Quantity);
        }
    }
}
