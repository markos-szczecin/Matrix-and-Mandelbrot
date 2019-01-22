using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            String m1 = "";
            String m2 = "";
            if (args.Length > 1)
            {
                m1 = args[0];
                m2 = args[1];
                Int32 ID = (Int32)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds;
                String destFileName = "product_" + ID.ToString();
                MatrixMulti matrix = new MatrixMulti(m1, m2, "matrixProd/" + destFileName);
                System.Console.WriteLine(destFileName);
            } else
            {
                System.Console.WriteLine("Brak nazw macieży");
            }
        }
    }
}
