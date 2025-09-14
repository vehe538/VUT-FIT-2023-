using System;
using System.Collections;
using Microsoft.VisualBasic;

namespace DataTypes
{
    public class ProgramClass
    {

        public static void anyType(dynamic variable)
        {
            Console.WriteLine(variable);
        }
        
        
        public static void Main(string[] args)
        {

            var dict = new Dictionary<int, string>()
            {
                { 1, "Monday" },
                { 2, "Tuesday" },
                { 3, "Wednesday" },
                { 4, "Thursday" },
            };

            anyType(dict[2]);
            anyType(85);
            anyType(56.6589);
            anyType(true);

            var myList = new List<dynamic>()
            {
                2,5.8, true, "kokt", 3.586970
            };
            
            //myList.Sort();
            Console.WriteLine("Today is " + dict[3]);
            foreach (var number in myList)
            {
                Console.WriteLine(number);
                
            }
            
        }
    }
}