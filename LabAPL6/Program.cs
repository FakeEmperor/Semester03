using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LabAPL6
{
    class Program
    {
        struct Ranking
        {
            public int max_len;
            public int num_rk;
        }

        static Ranking Analyze(string s, out char c)
        {
            Dictionary<char, Ranking> dic = new Dictionary<char, Ranking>();
            Ranking maxR = new Ranking(), r;
            c = (char)0;
            char curchar = (char)0, ch;
            int i = 0,l = s.Length, curlen = 0;
            for (; i < l; ++i)
            {
                ch = s[i];
                if (curchar != ch)
                {
                    //update value
                    if (curchar != 0)
                    {
                        if (!dic.ContainsKey(curchar))
                            dic.Add(curchar, new Ranking());
                        dic.TryGetValue(curchar,out r);
                        r.max_len = Math.Max(r.max_len,curlen);
                        ++r.num_rk;
                        dic[curchar] = r;
                        //CHOOSE MAX or update max
                        if (maxR.max_len < curlen || curchar == c)
                        {
                            maxR = r;
                            c = curchar;
                        }
                    }
                    curlen = 1;
                    curchar = ch;
                }
                else //update
                    ++curlen;
            }
            //last update BIDLOKOD
            //update value
            if (curchar != 0)
            {
                if (!dic.ContainsKey(curchar))
                    dic.Add(curchar, new Ranking());
                dic.TryGetValue(curchar, out r);
                r.max_len = Math.Max(r.max_len, curlen);
                ++r.num_rk;
                dic[curchar] = r;
                //CHOOSE MAX or update max
                if (maxR.max_len < curlen || curchar == c)
                {
                    maxR = r;
                    c = curchar;
                }
            }

            return maxR;
        }

        static void Main(string[] args)
        {
            Console.Write("Write down the string: ");
            string s = Console.ReadLine();
            char ch = (char)0;
            Ranking r = Analyze(s,out ch);

            Console.WriteLine("Most char: "+ch);
            Console.WriteLine("Max len in: "+r.max_len);
            Console.WriteLine("Number of c: "+r.num_rk);


            Console.ReadKey();
        }
    }
}
