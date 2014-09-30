using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LabAPL2
{
    class Program
    {
        static int getInt(string toShow){
            int toParse = -1;
            bool converted = false;
            Console.WriteLine(toShow);
            do
            {
                try
                {
                    toParse = int.Parse(Console.ReadLine());
                    if(toParse>0)
                        converted = true;
                    else
                        throw new System.ArgumentOutOfRangeException();
                    
                }
                catch (System.Exception)
                {
                    Console.Write("Exception occured during input. Try Again: ");
                }
            } while (!converted);
            return toParse;
        }
        static void Main(string[] args)
        {
            string input;
            Console.WriteLine("Write string");
            input = Console.ReadLine();
            int k = getInt("Write k");
            int i=k-1,u,iOld = 0;
            do {
                //if we are on space character
                if (i >= input.Length) {
                    //shift backwards
                    i = input.Length;
                    if(iOld>0)
                        --iOld; 
                }
                   
                //we are in word
                else
                {
                    int state = 0;
                    // reading up to last space character
                   
                    if(!Char.IsSeparator(input[i]))
                        while (i < input.Length && state != 2)
                        {
                            switch (state)
                            {
                            case 0:
                                if (Char.IsSeparator(input[i]))
                                    state = 1;
                                break;
                            case 1:
                                if (!Char.IsSeparator(input[i]))
                                    state = 2;
                                break;
                            }
                        ++i;
                        }
                    if(state==2)
                        --i;
                        
                }
                    
                string subs = input.Substring(iOld, i - iOld);
                for (u = 0; u < subs.Length && Char.IsSeparator(subs[u]); ++u) ;
                subs = subs.Remove(0,u);
                Console.WriteLine( subs );
                iOld = i;
                i+=k;
            } while(i <= input.Length);
            Console.ReadKey();
            
        }
    }
}
