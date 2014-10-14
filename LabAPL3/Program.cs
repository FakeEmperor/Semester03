using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LabAPL3
{
    class Program
    {
        static void InDepthWalk(List<List<bool>> matrix, List<bool> visited, int from)
        {
            visited[from] = true;
            int L = visited.Count;
            for (int i = 0; i < L; ++i)
                if (matrix[from][i] == true && visited[i] == false)
                    InDepthWalk(matrix, visited, i);
        }

        static bool HasPath(List<List<bool>> matrix, int from, int to)
        {
            int i = 0,L = matrix.Count;
            List<bool> visited = new List<bool>();
            for (i = 0; i < L; ++i)
                visited.Add(false);

            InDepthWalk(matrix, visited, from);

            for (i = 0; i < L; ++i)
                if (visited[i] == true && to == i)
                    return true;
            return false;
        }

        static List<List<bool>> InputMatrix(out int from, out int to)
        {
            List<List<bool>> matrix;
            string input;
            bool parsed = false;
            int a = -1;
            int u = 0;

            from = 0;
            to = 0;
            Console.WriteLine("Write matrix size");
            do
            {
                input = Console.ReadLine();
                try
                {
                    a = Int32.Parse(input);
                    parsed = true;
                }
                catch (SystemException e)
                {
                    Console.WriteLine("Parse failed!");
                    parsed = false;
                }

            } while (!parsed);

            Console.WriteLine("Matrix:");
            matrix = new List<List<bool>>();
            for (int i = 0; i < a; ++i)
            {
                matrix.Add(new List<bool>());
                input = Console.ReadLine();
                int s = input.Length;
                int r = 0;
                for (u = 0; u < s; ++u)
                {
                    try
                    {
                        if(input[u] == '1' || input[u] == '0' ) {
                            matrix[i].Add(input[u]=='1'?true:false);
                            r++;
                        }
                        parsed = true;
                    }
                    catch (SystemException e)
                    {
                        Console.WriteLine("Parse failed!");
                        parsed = false;
                    }
                };
                if(r!=a) {
                    Console.WriteLine("Matrix is invalid");
                    System.Environment.Exit(1);
                }
            }
            Console.WriteLine("Path from A to B:");
            
            int numRead = 0;
           
            do
            {
                u = 0;
                int i=0;
                input = Console.ReadLine();
                do
                {
                    for (i = u; i < input.Length; ++i)
                        if (Char.IsWhiteSpace(input[i]))
                            break;
                    try
                    {
                        int temp = Int32.Parse(input.Substring(u, i - u));
                        if (numRead == 0)
                            from = temp;
                        else
                            to = temp;
                        numRead++;
                        u = i + 1;

                    }
                    catch (SystemException e)
                    {
                        Console.WriteLine("Parse Failed!");
                    }
                } while (i != input.Length);
            } while (numRead < 2);

            return matrix;
        }

        static void Main(string[] args)
        {
            int a, b;
            List<List<bool>> matrix = InputMatrix(out a, out b);
            bool hp = HasPath(matrix, a, b);
            Console.Write("From A to B path has");
            if(!hp)
                Console.Write(" not");
            Console.WriteLine(" been found.");
            Console.ReadKey();
        }
    }
}
