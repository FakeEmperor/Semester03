using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace LabAPL3
{
    
    class Program
    {
       
       static bool HasPathAlt(List<List<bool>> matrix, Point from, Point to){
           List<Point> lp = new List<Point>(); //point of visited
           Point NA = new Point(-1,-1);
           List<Point> available = new List<Point>(); //point of visited
           int i,u;
           bool finished = false;
           int rows = matrix.Count>0?matrix[0].Count:0,
               lines = matrix.Count;
           Point current = from, temp;
           Point[] points = new Point[4];
           //true - wall, false - path
           if(matrix[from.Y][from.X] == true ||
               matrix[to.Y][to.X] == true ) 
                   return false;
           int numPs = 0;
           lp.Add(current); //as we visited current

           do {
               numPs = 0;
               if (current.X == to.X && current.Y == to.Y)
                   return true;
               //check if all is done
               for (i = 0; i < 4; ++i)
               {
                   
                   temp = NA;
                   points[i] = NA;
                   
                   switch (i)
                   {
                       case 0:
                           if (current.Y + 1 < lines ) {
                               temp.X = current.X;
                               temp.Y = current.Y+1;
                           }
                           
                           break;
                       case 1:
                           if (current.Y - 1 >= 0)
                           {
                               temp.X = current.X;
                               temp.Y = current.Y - 1;
                           }
                           break;
                       case 2:
                           if (current.X + 1 < rows )
                           {
                               temp.X = current.X +1 ;
                               temp.Y = current.Y;
                           }
                           break;
                       case 3:
                           if (current.X - 1 >= 0)
                           {
                               temp.X = current.X -1;
                               temp.Y = current.Y;
                               
                           }
                           break;
                   }
                   if (temp.X != -1 && matrix[temp.Y][temp.X] == false
                       && lp.IndexOf( temp ) == -1 )
                   {
                       points[i] = temp;
                       numPs++;
                   }

               }
               if (numPs == 0)
               {
                   //check if has other paths
                   if (available.Count > 0)
                   {
                       current = available[0];
                       available.RemoveAt(0);
                       lp.Add(current);
                   }
                   else
                       finished = true;
               }
               else
               {
                   bool used_kokoko = false;
                   for (i = 0; i < 4; ++i)
                   {
                       if (points[i].X != -1)
                       {
                           if (used_kokoko)
                               //others - move to availables
                               available.Add(points[i]);
                           else
                           {
                               //move by first
                               used_kokoko = true;
                               current = points[i];
                           }

                       }
                   }

                   //fill visited with current
                   lp.Add(current);
               }
               
           } while( !finished );

           return false;
       }

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

        static List<List<bool>> InputMatrix(out Point from, out Point to)
        {
            List<List<bool>> matrix;
            string input;
            bool parsed = false;
            int a = -1;
            int d = 0;
            int u = 0;

            from = new Point(-1,-1);
            to =new Point(-1,-1);
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
            for (d=0; d<2; ++d) {
               Console.WriteLine("Path from point "+ (d==0?"A":"B") +":");
               numRead = 0;
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
                            if (numRead == 0) {
                                if (d == 0)
                                    from.X = temp;
                                else
                                    to.X = temp;
                            }
                                
                            else
                                if (d == 0)
                                    from.Y = temp;
                                else
                                    to.Y = temp;
                            numRead++;
                            u = i + 1;

                        }
                        catch (SystemException e)
                        {
                            Console.WriteLine("Parse Failed!");
                        }
                    } while (i != input.Length);
                } while (numRead < 2);
            }
           

            return matrix;
        }

        static void Main(string[] args)
        {
            Point a, b;
            List<List<bool>> matrix = InputMatrix(out a, out b);
            bool hp = HasPathAlt(matrix, a, b);
            Console.Write("From A to B path has");
            if(!hp)
                Console.Write(" not");
            Console.WriteLine(" been found.");
            Console.ReadKey();
        }
    }
}
