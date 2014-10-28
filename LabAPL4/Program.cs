using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LabAPL4
{
    class Program
    {
        static string Reverse(string s)
        {
            char[] charArray = s.ToCharArray();
            Array.Reverse(charArray);
            return new string(charArray);
        }
        static string parseInput(string input  ){
            string s = "";
            foreach (Char c in input)
                if (s.IndexOf(c) == -1)
                    s += c;
            return s;
        }
        static int writeLines(string parsed, int i)
        {
            int[] what = new int[i];
            int u,d,p, num = 0;
            for (u = 0; u < i; ++u)
                what[u] = 0;
            string t;
            bool needed = i>0?true:false;
            d = i - 1;
            p = i;
            //do string
            while (d>=0)
            {
                t = "";
                p = i;
                for (u = i-1; u >=0; --u)
                {
                    t += parsed[what[u]];
                    if (u == p - 1)
                    {
                        if (what[u] == parsed.Length - 1) {
                            what[u] = 0;
                            p--;
                        }
                            
                        else
                            what[u]++;
                    }
                }
                Console.WriteLine(Reverse(t));
                ++num;
                if (d == p)
                    d--;
                    
            }
            return num;
        }
        static int expectedCount( int l)
        {
            int a = 0;
            for (int i = 1; i <= l; ++i)
                a += (int)Math.Pow((double)l, (double)i);
            return a;
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Type something...");
            string input = Console.ReadLine();
            string parsed = parseInput(input);
            int n = 0;
            for (int i = 0; i <= parsed.Length; ++i)
                n+=writeLines(parsed,i);
            Console.WriteLine("Total Word Count: {0}", n);
            Console.WriteLine("Expected Word Count: {0}", expectedCount(parsed.Length));
            Console.ReadLine();
        }
    }
}
