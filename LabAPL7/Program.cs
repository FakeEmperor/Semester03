using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Threading;

namespace LabAPL7
{
    class Program
    {

        public abstract class Figure
        {
            public virtual string ToString() {
                return "Some Abstract Figure";   
            }
            public virtual double Square()
            {
                return 0;
            }
        }

        public class Circle : Figure
        {
            double _radius;
            public double Radius { get { return  _radius; } }
            public Circle(double r)
            {
                this._radius = r;
            }
            public override double Square()
            {
 	            return Math.PI*_radius*_radius;
            }
            public override string ToString()
            {
 	            return "[ Figure->Circle ]: Radius="+this._radius+", Square="+this.Square();
            }
        }

        public class Polygon : Figure
        {
            Point []ps;
            public Polygon(Point []ps)
            {
                this.ps = ps;
            }
            public override double Square()
            {
                int i,l=ps.Length;
                double s = 0;
 	            for(i=0; i<l; ++i)
                    s+=Math.Abs((ps[i].X+ps[i!=l-1?i:0].X)*(ps[i].Y-ps[i!=l-1?i:0].Y));
                return s/2;
            }
            public override string ToString()
            {
 	            return "[ Figure->Polygon ]: Vertices="+this.ps.Length+", Square="+this.Square();
            }
        }

        public class Line: Figure
        {
            int _length;
            int Length { get {return _length; }}
            public Line(int length){
                this._length = length;
            }
            public override double Square()
            {
 	            return 0;
            }
            public override string ToString()
            {
 	            return "[ Figure->Line ]: Length="+this.Length;
            }
        }

        static int safeInt(string msg) {
            int res = 0;
            string buf;
            bool finished = true;
            do {
                finished = true;
                Console.Write(msg);
                buf = Console.ReadLine();
                try {
                    res = Int32.Parse(buf);
                } catch (Exception e) {
                    Console.WriteLine("Invalid input: ");
                    Console.Write(e.Message);
                    finished = false;
                }
                
            } while(!finished);
            
            return res;
        }

        static public class FigureFactory {
            public enum FigureType {
                FT_MIN = 0,
                FT_CIRCLE = 0,
                FT_POLY = 1,
                FT_LINE = 2,
                FT_MAX = 2
            }

            public static Figure BuildFromInput( FigureType what ){
                switch(what){
                    case FigureType.FT_CIRCLE:
                        int rad;
                        do
                        {
                            rad = safeInt("Pass the valid (rad>=0) radius: ");
                        } while (rad < 0);
                        return new Circle(rad);
                    case FigureType.FT_LINE:
                        int len;
                        do
                            len = safeInt("Pass the valid (len>=0) length: ");
                        while (len < 0);
                        return new Line(len);
                    case FigureType.FT_POLY:
                        int nPs;
                        do
                            nPs = safeInt("Pass the valid (num>=0) number of points: ");
                        while (nPs < 0);
                        Point []ps = new Point[nPs];
                        for (int i =0; i<nPs; ++i) {
                            Console.WriteLine("Point "+(i+1)+"/"+nPs);
                            ps[i].X = safeInt("X: ");
                            ps[i].Y = safeInt("Y: ");
                        }
                        return new Polygon(ps);
                }
                return null;
            }
        }

        enum MenuChoice
        {
            UNDEFINED = -1,
            PRINT_ALL = 0,
            ADD_FIGURE = 1,
            FLUSH_SCREEN = 2,
            EXIT = 3,
        }

        static void UserInput()
        {
            MenuChoice choice = MenuChoice.UNDEFINED;
            List<Figure> lf = new List<Figure>();
            MenuChoice []menu_indexes = { MenuChoice.PRINT_ALL, MenuChoice.ADD_FIGURE, MenuChoice.FLUSH_SCREEN, MenuChoice.EXIT };
            string []menu_headers = { "Print all figures", "Add figure", "Flush the screen output", "Close the application" };
            FigureFactory.FigureType []figure_indexes = { FigureFactory.FigureType.FT_CIRCLE, FigureFactory.FigureType.FT_LINE, FigureFactory.FigureType.FT_POLY };
            string []figure_headers = {"Circle", "Line", "Polygon"};
            int l = menu_headers.Length, ll = figure_headers.Length;
            do
            {
                Console.WriteLine("Main Menu: ");
                for (int i = 0; i < l; ++i)
                    Console.WriteLine((int)menu_indexes[i] + " - " + menu_headers[i]);
                int ch = safeInt("Choose the action: ");
                switch ((MenuChoice)ch)
                {
                    case MenuChoice.PRINT_ALL:
                        Console.Clear();
                        Console.WriteLine("There are "+lf.Count+" figures: ");
                        foreach (Figure f in lf)
                            Console.WriteLine(f.ToString());
                        break;
                    case MenuChoice.FLUSH_SCREEN:
                        Console.Clear();
                        break;
                    case MenuChoice.ADD_FIGURE:
                        bool parsed = true;
                        do
                        {
                            parsed = true;
                            for (int i = 0; i < ll; ++i)
                                Console.WriteLine((int)figure_indexes[i] + " - " + figure_headers[i]);
                            ch = safeInt("Choose the type of figure [or type \""+((int)FigureFactory.FigureType.FT_MAX+1)+"\" to close]: ");
                            if (ch <(int) FigureFactory.FigureType.FT_MIN || ch > (int)FigureFactory.FigureType.FT_MAX+1)
                            {
                                parsed = false;
                                Console.WriteLine("Invalid input");
                            }
                        } while (!parsed);
                        if (ch == (int)FigureFactory.FigureType.FT_MAX + 1)
                            break;
                        else
                            lf.Add(FigureFactory.BuildFromInput((FigureFactory.FigureType)ch));
                        
                        break;
                    case MenuChoice.EXIT:
                        return;
                    default:
                        Console.WriteLine("Invalid action");
                        break;
                }

            } while (choice == MenuChoice.UNDEFINED);
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Hi, I'm LabAPL7 - Polymorphysm.");
            UserInput();
            Console.WriteLine("Application will close now...");
            Thread.Sleep(1500);
        }
    }
}
