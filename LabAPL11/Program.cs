using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;

namespace LabAPL11
{
    class Program
    {
        internal struct Pair
        {
            public string one,
                   two;
            public Pair(string one, string two)
            {
                this.one = one;
                this.two = two;
            }
        }

        static bool arePair(string s1, string s2)
        {
            List<char> a1 = s1.ToList(), a2 = s2.ToList();
            if (s1.Length == s2.Length)
            {
                for (int index, i = a2.Count - 1; i > 0; --i)
                {
                    index = a1.FindIndex((char c) => { return c == a2[i]; });
                    if (index < 0)
                        return false;
                    else {
                        a2.RemoveAt(i);
                        a1.RemoveAt(index);
                    }
                        
                        
                }
                return true;
            }
            return false;
        }

        static void genPairsFor(List<Pair> pairs, List<string> words, int i)
        {
            for (int index = 0, s = words.Count; index < s - 1; ++index)
                if (i!=index)
                    if(arePair(words[index], words[index + 1]))
                        pairs.Add(new Pair(words[index], words[index + 1]));
        }
        static void Main(string[] args)
        {
            bool finished = false;
            List<string> words = new List<string>() ;
            List<Pair> pairs = new List<Pair>();
            char []splitters = {' ','!', '.', ',',';','?'};
            do
            {
                words.AddRange(Console.ReadLine().Split(splitters).ToList());
                if (words.Count > 0 && words[words.Count - 1].ToLower() == "finish")
                    finished = true;
            } while (!finished);
            if (words.Count > 0)
                do
                {
                    genPairsFor(pairs, words, words.Count - 1);
                    words.RemoveAt(words.Count-1);
                } while (words.Count > 0);
            if(pairs.Count>0)
                foreach (var pair in pairs) 
                    Console.WriteLine("Pair: " + pair.one + "<->" + pair.two);
                
            else
                Console.WriteLine("Nothing to write - no pairs");
            Console.ReadKey();
        }
    }
}
